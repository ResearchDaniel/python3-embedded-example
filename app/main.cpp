#include "python3interpreter/pythoninterpreter.h"
#include <pybind11/embed.h>

#include <iostream>
#include <filesystem>
namespace py = pybind11;
namespace fs = std::filesystem;

int main(int argc, char** argv) {
    fs::path exePath(argv[0]);

    auto exeDir = exePath.parent_path();
    // Search paths for external scripts
    auto pythonDir = PythonInterpreter::getPythonDir(exeDir);
    auto scriptsPath = pythonDir / "PyScripts";
    std::vector<fs::path> externalSearchPaths {
        exeDir,
        scriptsPath
    };
    PythonInterpreter interp(exePath, externalSearchPaths);
    
    
    py::exec(R"(
        kwargs = dict(name="World", number=42)
        message = "Python code execution succeeded: Hello, {name}! The answer is {number}\n".format(**kwargs)
        print(message)
    )");
    
    // Import embedded modules
    try {
        auto fast_calc = py::module_::import("fast_calc");
        auto result = fast_calc.attr("add")(1, 2).cast<int>();
        std::cout << "fast_calc 1+2=" << result << std::endl;
        std::cout << "Embedded module import succeeded" << std::endl;
    } catch(py::error_already_set& ex) {
        std::cerr << ex.what();
    }
    
    // Import module installed via pip
    try {
        py::module_ calc = py::module_::import("numpy");
        py::exec(R"(
            import numpy
            print(f"Pip installed package import succeeded (Numpy version: {numpy.version.version})\n")
        )");
    } catch(py::error_already_set& ex) {
        std::cerr << ex.what();
    }
    
    // Import custom module imported via external files (in scriptsPath)
    try {
        py::module_ calc = py::module_::import("calc");
        py::object result = calc.attr("add")(1, 2);
        std::cout << "calc 1+2=" << result.cast<int>() << std::endl;
        std::cout << "Custom module import succeeded" << std::endl;
    } catch(py::error_already_set& ex) {
        std::cerr << ex.what();
    }
    
    // Import system modules
    try {
        py::module_ sys = py::module_::import("sys");
        py::print(sys.attr("path"));
    } catch(py::error_already_set& ex) {
        std::cerr << ex.what();
    }

}
