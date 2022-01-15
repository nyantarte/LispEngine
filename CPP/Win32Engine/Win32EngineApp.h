#pragma once
#include"Win32EngineFramework.h"
#include"Win32EngineWindow.h"

class CWin32EngineApp :
    public CWin32App
{

    typedef std::shared_ptr<CWin32EngineWindow> win_ptr;
    win_ptr m_mainWindow;
    std::mt19937 m_rand;


    CWin32Keystate m_keyState[9];
    CPoint m_cursorPos;

    std::string m_curDirPath;

    typedef std::vector<chara_ptr> CharaTbl;
    CharaTbl m_charaTbl;
    typedef std::map<std::string, LispScriptEngine::lisp_ptr> StateTbl;
    StateTbl m_stateTbl;

    
    OnIdleCallbackTbl m_onIdleTbl;
   

public:
    CWin32EngineApp();
    ~CWin32EngineApp();
    static CWin32EngineApp* GetInstance() {
        return reinterpret_cast<CWin32EngineApp*>(CWin32App::s_Self);
    }

    /*
    void RunScriptString(std::string src, LispScriptEngine::lisp_ptr This=nullptr);
    void RunScriptFile(std::string fileName, LispScriptEngine::lisp_ptr This = nullptr);
    void LoadScriptFromString(LispScriptEngine::CVM& vm, std::string src, LispScriptEngine::lisp_ptr This = nullptr);
*/
    void LoadGraphFromFile(std::string fileName);

    int GetIntRand(int max) {
        return GetIntRand(0, max);
    }
    int GetIntRand(int min, int max);
    bool GetBoolRand();
    int GetIntRand() {
        return m_rand();
    }
    
    virtual int run();
    virtual void ProcEvents();
    CharaTbl& getCharaTbl() {
        return m_charaTbl;
    }

    bool IsMouseClicked() {
        return  /*GetAsyncKeyState(VK_LBUTTON) & 0x01*/m_keyState[8].isTriggered();
    }
    CPoint& GetCursorPos() {
        return m_cursorPos;
    }
    OnIdleCallbackTbl& GetIdleCallbackTbl() {
        return m_onIdleTbl;
    }
    void setMainWndSize(int w, int h);
private:
    /*
    static void xscreen(int argNum, void* p);


    static void gtarget(int argNum, void* p);
    static void gline(int argNum, void* p);
    static void gcircle(int argNum, void* p);
    static void gbox(int argNum, void* p);
    static void gbegin(int argNum, void* p);
    static void gend(int argNum, void* p);
    static void gclear(int argNum, void* p);
    static void createRouge(int argNum, void* p);
    static void resetRouge(int argNum, void* p);
    
    static void is_running(int argNum, void* p);
    static void proc_events(int argNum, void* p);

    static void bpressed(int argNum, void* p);
    static void draw(int argNum, void* p);
    static void run(int argNum, void* p);
    static void gtext(int argNum, void* p);
    static void gload(int argNum, void* p);
    static void update(int argNum, void* p);

    static void createChara(int argNum, void* p);
    static void createGridLayout(int argNum, void* p);
    static void createButton(int argNum, void* p);
    static void createText(int argNum, void* p);
    static void setBorder(int argNum, void* p);
    static void add(int argNum, void* p);

    static void setState(int argNum, void* p);
    static void getState(int argNum, void* p);

    static void inputTriggered(int argNum, void* p);
    static void getCursorPos(int argNum, void* p);

    */

};

