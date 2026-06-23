#include <windows.h>
#include <iostream>

int main() {
    const char* pythonPath = "Runtime\\python.exe";
    const char* scriptPath = "test.py";

    std::string cmd = std::string(pythonPath) + " " + scriptPath;

    STARTUPINFOA si = { sizeof(si) };
    PROCESS_INFORMATION pi;

    if (!CreateProcessA(
        NULL,
        cmd.data(),
        NULL,
        NULL,
        FALSE,
        0,
        NULL,
        NULL,
        &si,
        &pi
    )) {
        std::cout << "Python baslatilamadi\n";
        return 1;
    }

    WaitForSingleObject(pi.hProcess, INFINITE);

    CloseHandle(pi.hProcess);
    CloseHandle(pi.hThread);

    std::cout << "Python tamamlandi\n";
    return 0;
}