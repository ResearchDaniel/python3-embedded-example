#pragma once

#include "python3interpreter/pythoninterpreterdefines.h"

#include <filesystem>
#include <vector>
namespace fs = std::filesystem;

class PYTHON_INTERPRETER_API PythonInterpreter {
public:
    /*
     * Initializes Python
     * @param exePath Path to the main executable
     * @param externalSearchPaths Additional Python module search paths
     * @param useSystemPython Setup system installed Python if true, isolated mode otherwise.
     *
     * @post Python API can be called
     */
    PythonInterpreter(fs::path exePath, std::vector<fs::path> externalSearchPaths, bool useSystemPython = false);
    ~PythonInterpreter();
    
    /*
     * Return root folder containing Python:
     * root/bin
     * root/lib
     */
    static fs::path getPythonDir(fs::path exeDir);
};
