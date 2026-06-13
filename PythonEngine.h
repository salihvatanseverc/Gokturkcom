#pragma once
#include <windows.h>
#include <string>

class PythonEngine {
private:
    HMODULE hPythonDLL;

    typedef void(*Ptr_Py_SetPath)(const wchar_t*);
    typedef void(*Ptr_Py_Initialize)(void);
    typedef int(*Ptr_PyRun_SimpleString)(const char*);
    typedef void(*Ptr_Py_Finalize)(void);

    Ptr_Py_SetPath pPy_SetPath;
    Ptr_Py_Initialize pPy_Initialize;
    Ptr_PyRun_SimpleString pPyRun_SimpleString;
    Ptr_Py_Finalize pPy_Finalize;

public:
    PythonEngine();
    ~PythonEngine();
    bool Init();
    void Run(const std::string& code);
};