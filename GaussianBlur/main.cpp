#include "GaussianBlur.h"
#include <QtWidgets/QApplication>
#include <Windows.h>

#include "CppLib.h"

typedef void(__cdecl* funInit)();

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    GaussianBlur w;
    w.show();

    //---------- Explicit linking test ----------//
    
    HMODULE hModule;
    hModule = LoadLibrary(TEXT("C:\\Users\\kamil\\source\\repos\\GaussianBlur\\x64\\Debug"));
    
    if (hModule == NULL)
    { 


        // TODO - dialog with error - GetLastError()
    }
    else
    {
        funInit initFun = (funInit)GetProcAddress(hModule, "Init");

        if (initFun == NULL)
        {
            // TODO - dialog with error - GetLastError()
        }
        else
        {
            initFun();
            FreeLibrary(hModule);
        }
    }

    //-------------------------------------------//

    return a.exec();
}
