#include <windows.h>
#include <string>
#include "PythonEngine.h"

// Küresel nesneler
PythonEngine motor;
HWND hEditBox;

// Pencere olaylarını kontrol eden fonksiyon
LRESULT CALLBACK WindowProcedure(HWND hWnd, UINT msg, WPARAM wp, LPARAM lp) {
    switch (msg) {
        case WM_CREATE:
            hEditBox = CreateWindowW(L"EDIT", L"print('Arayuzden Merhaba!')", 
                WS_CHILD | WS_VISIBLE | WS_BORDER | ES_MULTILINE | ES_AUTOVSCROLL,
                20, 20, 440, 200, hWnd, NULL, NULL, NULL);

            CreateWindowW(L"BUTTON", L"Kodu Calistir", 
                WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,
                20, 240, 150, 40, hWnd, (HMENU)1, NULL, NULL);
            break;

        case WM_COMMAND:
            if (LOWORD(wp) == 1) {
                int length = GetWindowTextLengthW(hEditBox);
                if (length > 0) {
                    std::wstring wCode(length, L'\0');
                    GetWindowTextW(hEditBox, &wCode[0], length + 1);

                    std::string code(wCode.begin(), wCode.end());
                    motor.Run(code);
                }
            }
            break;

        case WM_DESTROY:
            PostQuitMessage(0);
            break;
        default:
            return DefWindowProcW(hWnd, msg, wp, lp);
    }
    return 0;
}

int WINAPI WinMain(HINSTANCE hInst, HINSTANCE hPrevInst, LPSTR args, int ncmdshow) {
    // -----------------------------------------------------------------
    // KRİTİK ADIM: ÇALIŞMA DİZİNİNİ EXE'NİN OLDUĞU YERE ZORLA KİLİTLİYORUZ
    // -----------------------------------------------------------------
    wchar_t exeBuffer[MAX_PATH];
    GetModuleFileNameW(NULL, exeBuffer, MAX_PATH);
    std::wstring exePath(exeBuffer);
    size_t pos = exePath.find_last_of(L"\\/");
    std::wstring currentDir = exePath.substr(0, pos);
    
    // Windows'a "Aktif olarak bu klasörde çalışıyorsun" emrini veriyoruz
    SetCurrentDirectoryW(currentDir.c_str());
    // -----------------------------------------------------------------

    WNDCLASSW wc = {0};
    wc.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
    wc.hCursor = LoadCursor(NULL, IDC_ARROW);
    wc.hInstance = hInst;
    wc.lpszClassName = L"GokturkGuiClass";
    wc.lpfnWndProc = WindowProcedure;

    if (!RegisterClassW(&wc)) return -1;

    // Çalışma dizini kilitlendikten sonra motoru başlatıyoruz
    if (!motor.Init()) {
        MessageBoxW(NULL, L"Python Motoru Baslatilamadi!", L"Hata", MB_ICONERROR | MB_OK);
        return -1;
    }

    HWND hWnd = CreateWindowW(L"GokturkGuiClass", L"Gokturk Programlama Dili", 
        WS_OVERLAPPEDWINDOW | WS_VISIBLE,
        100, 100, 500, 360, NULL, NULL, hInst, NULL);

    if (!hWnd) return -1;

    MSG msg = {0};
    while (GetMessageW(&msg, NULL, 0, 0)) {
        TranslateMessage(&msg);
        DispatchMessageW(&msg);
    }

    return (int)msg.wParam;
}