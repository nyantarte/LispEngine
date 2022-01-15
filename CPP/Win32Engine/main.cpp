#include"Win32EngineFramework.h"
#include"Win32EngineApp.h"
#include"Win32EngineWindow.h"

int main(int argc, char** argv)
{
    CoInitialize(NULL);
    CWin32EngineApp app;
    app.setMainWndSize(640, 480);
    /*
    if (1 == argc) {
        app.RunScriptString(
            "(xscreen 640 480)\n(while (isRunning) ((procEvents)) )");
    }
    else {
        app.RunScriptFile(argv[1]);
    }
    */

    if (1 < argc) {
        app.LoadGraphFromFile(argv[1]);
        app.run();
        
    }



    CoUninitialize();
    return 0;

}