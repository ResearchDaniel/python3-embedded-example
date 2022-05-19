#pragma once

#ifdef EXPORTING_PYTHON_INTERPRETER  // DYNAMIC
// If we are building DLL files we must declare dllexport/dllimport
#ifdef PYTHON_INTERPRETER_EXPORTS
#ifdef _WIN32
#define PYTHON_INTERPRETER_API __declspec(dllexport)
#else  // UNIX (GCC)
#define PYTHON_INTERPRETER_API __attribute__((visibility("default")))
#endif
#else
#ifdef _WIN32
#define PYTHON_INTERPRETER_API __declspec(dllimport)
#else
#define PYTHON_INTERPRETER_API
#endif
#endif
#else  // STATIC
#define PYTHON_INTERPRETER_API
#endif
