#include "GaussianBlur.h"
#include <QtWidgets/QApplication>
#include <Windows.h>

#include "CppLib.h"

typedef bool(__cdecl *pInit)();

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    GaussianBlur w;
    w.show();

    //---------- Explicit linking test C++ ----------//
    int x = 0;

    HMODULE hModule;
    hModule = LoadLibrary(TEXT("C:\\Users\\kamil\\source\\repos\\GaussianBlur\\x64\\Debug\\CppLib.dll"));

    if (hModule == NULL)
    {
        // TODO - dialog with error - GetLastError()
    }
    else
    {
        pInit initCpp = (pInit)GetProcAddress(hModule, "initCpp");

        if (initCpp == NULL)
        {
            // TODO - dialog with error - GetLastError()
        }
        else
        {
            x = initCpp();
            FreeLibrary(hModule);
        }
    }

    //---------- Explicit linking test Asm ----------//
    int y = 0;

    HMODULE hModule2;
    hModule2 = LoadLibrary(TEXT("C:\\Users\\kamil\\source\\repos\\GaussianBlur\\x64\\Debug\\AsmLib.dll"));

    if (hModule2 == NULL)
    {
        // TODO - dialog with error - GetLastError()
    }
    else
    {
        pInit initAsm = (pInit)GetProcAddress(hModule2, "initAsm");

        if (initAsm == NULL)
        {
            // TODO - dialog with error - GetLastError()
        }
        else
        {
            y = initAsm();
            FreeLibrary(hModule2);
        }
    }

    //-------------------------------------------//
    return a.exec();
}
