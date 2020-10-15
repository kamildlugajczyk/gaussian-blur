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

    //---------- Explicit linking test ----------//

    int x = 0;
    HMODULE hModule;
    hModule = LoadLibrary(TEXT("C:\\Users\\kamil\\source\\repos\\GaussianBlur\\x64\\Debug\\CppLib.dll"));

    if (hModule == NULL)
    {
        // TODO - dialog with error - GetLastError()
    }
    else
    {
        pInit init = (pInit)GetProcAddress(hModule, "init");

        if (init == NULL)
        {
            // TODO - dialog with error - GetLastError()
        }
        else
        {
            x = init();
            FreeLibrary(hModule);
        }
    }

    //-------------------------------------------//

    return a.exec();
}
