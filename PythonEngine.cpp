#include "PythonEngine.h"
#include <iostream>

PythonEngine::PythonEngine() 
    : hPythonDLL(nullptr), pPy_SetPath(nullptr), pPy_Initialize(nullptr), 
      pPyRun_SimpleString(nullptr), pPy_Finalize(nullptr) {}

bool PythonEngine::Init() {
    // 1. EXE'nin bulunduğu tam klasör yolunu alıyoruz
    wchar_t buffer[MAX_PATH];
    GetModuleFileNameW(NULL, buffer, MAX_PATH);
    std::wstring exePath(buffer);
    size_t pos = exePath.find_last_of(L"\\/");
    std::wstring currentDir = exePath.substr(0, pos);

    // 2. Python'ın aradığı yolları Windows'un hafızasına (Environment) ekiyoruz
    std::wstring pythonDir = currentDir + L"\\language\\Python";
    std::wstring pythonZip = currentDir + L"\\language\\Python\\python.zip";
    
    // Windows'a PYTHONPATH değişkenini kaydediyoruz (Böylece encodings hatası imkansız hale geliyor)
    std::wstring envValue = pythonZip + L";" + pythonDir;
    SetEnvironmentVariableW(L"PYTHONPATH", envValue.c_str());
    SetEnvironmentVariableW(L"PYTHONHOME", pythonDir.c_str());

    // 3. DLL dosyasını tam konumundan yüklüyoruz
    std::wstring fullDllPath = pythonDir + L"\\python313.dll";
    hPythonDLL = LoadLibraryW(fullDllPath.c_str());
    
    if (!hPythonDLL) {
        return false;
    }

    pPy_SetPath         = (Ptr_Py_SetPath)GetProcAddress(hPythonDLL, "Py_SetPath");
    pPy_Initialize      = (Ptr_Py_Initialize)GetProcAddress(hPythonDLL, "Py_Initialize");
    pPyRun_SimpleString = (Ptr_PyRun_SimpleString)GetProcAddress(hPythonDLL, "PyRun_SimpleString");
    pPy_Finalize        = (Ptr_Py_Finalize)GetProcAddress(hPythonDLL, "Py_Finalize");

    if (pPy_SetPath && pPy_Initialize && pPyRun_SimpleString && pPy_Finalize) {
        // Çevre değişkenlerini garantiye aldığımız için SetPath'e de tam yolu veriyoruz
        pPy_SetPath(envValue.c_str());

        pPy_Initialize();
        return true;
    }

    return false;
}

void PythonEngine::Run(const std::string& code) {
    if (pPyRun_SimpleString) {
        pPyRun_SimpleString(code.c_str());
    }
}

PythonEngine::~PythonEngine() {
    if (pPy_Finalize) pPy_Finalize();
    if (hPythonDLL) FreeLibrary(hPythonDLL);
}