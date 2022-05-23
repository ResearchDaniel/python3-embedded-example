#include "python3interpreter/pythoninterpreter.h"

#include <iostream>
#include <iterator>
#include <sstream>
#include <pybind11/embed.h>
namespace py = pybind11;

// Internal functions to handle differences in std::filesystem::path.
// The path uses wchar_t on Windows and char on other platforms.
// Use function overloading to provide cross-platform string assignments
PyStatus setPyConfigString(PyConfig* config, wchar_t **config_str, const char* val) {
    return PyConfig_SetBytesString(config, config_str, val);
}
PyStatus setPyConfigString(PyConfig* config, wchar_t **config_str, const wchar_t* val) {
    return PyConfig_SetString(config, config_str, val);
}

static void PyStatusExitOnError(PyStatus status)
{
  if (PyStatus_Exception(status))
  {
    std::cerr << "Internal error initializing Python!";
    // This calls `exit`.
    Py_ExitStatusException(status);
  }
}

PythonInterpreter::PythonInterpreter(fs::path exePath, std::vector<fs::path> externalSearchPaths, bool useSystemPython) {
    
    PyStatus status;
    PyPreConfig preconfig;
    if (useSystemPython) {
        PyPreConfig_InitPythonConfig(&preconfig);
    } else {
        PyPreConfig_InitIsolatedConfig(&preconfig);
    }
    preconfig.utf8_mode = 1;

    status = Py_PreInitialize(&preconfig);
    PyStatusExitOnError(status);

    // Python now speaks UTF-8
    PyConfig config;
    if (useSystemPython) {
        PyConfig_InitPythonConfig(&config);
    } else {
        PyConfig_InitIsolatedConfig(&config);
    }
    
    config.user_site_directory = useSystemPython;
    
    auto exeDir = exePath.parent_path();
    auto pythonDir = getPythonDir(exeDir);
    
    auto pythonExe = getPythonExe(pythonDir);
    status = setPyConfigString(&config, &config.program_name, pythonExe.c_str());
    PyStatusExitOnError(status);
    

    // Python will search for modules relative to the home directory.
    // On Mac/Linux:
    // home/bin/pythonMajor.Minor (executable)
    // home/lib/pythonMajor.Minor/site-packages (installed python modules)
    // On Windows:
    // home/
    // home/Lib/site-packages
    auto pythonHome = pythonDir;
    status = setPyConfigString(&config, &config.home, pythonHome.c_str());
    PyStatusExitOnError(status);
    
    // Add external Python module search paths
    std::stringstream pyPath;
    // Different delimiters depending on OS
    // https://docs.python.org/3/c-api/init_config.html#c.PyConfig.pythonpath_env
#ifdef _WIN32
    const auto delim = ";";
#else
    const auto delim = ":";
#endif
    for (const auto& path : externalSearchPaths) {
        pyPath << path.string() << delim;
    }
    status = setPyConfigString(&config, &config.pythonpath_env, pyPath.str().c_str());

    PyStatusExitOnError(status);

    status = Py_InitializeFromConfig(&config);
    PyConfig_Clear(&config);
    if (PyStatus_Exception(status)) {
        Py_ExitStatusException(status);
    }
    
    // We can now use the Python API
}

fs::path PythonInterpreter::getPythonDir(fs::path exeDir) {
#ifdef __APPLE__
    auto pythonDir = exeDir.parent_path() / "Resources/python";
#elif defined(_WIN32)
    auto pythonDir = exeDir / "python";
#else
    auto pythonDir = exeDir / "python";
#endif
    return pythonDir;
}

fs::path PythonInterpreter::getPythonExe(fs::path pythonDir) {
#if defined(_WIN32)
    auto pythonExe = pythonDir / "python.exe";
#else
    std::stringstream pyVer;
    pyVer << "python" << PYTHON_VERSION_MAJOR << "." << PYTHON_VERSION_MINOR;
    auto pythonExe = pythonDir / "bin" / fs::path(pyVer.str());
#endif
    return pythonExe;
}

PythonInterpreter::~PythonInterpreter() {
    Py_Finalize();
}
