#include "Win32EngineApp.h"
#include "Win32EngineWindow.h"
CWin32EngineApp::CWin32EngineApp()
{
    m_keyState[0].setKey(KEY_LEFT);
    m_keyState[1].setKey(KEY_RIGHT);
    m_keyState[2].setKey(KEY_UP);
    m_keyState[3].setKey(KEY_DOWN);
    m_keyState[4].setKey(KEY_X);
    m_keyState[5].setKey(KEY_Y);
    m_keyState[6].setKey(KEY_A);
    m_keyState[7].setKey(KEY_B);
    m_keyState[8].setKey(MOUSE_L_BTN);
    m_rand.seed(time(NULL));
    LPSTR buf[256];
    GetCurrentDirectoryA(sizeof(buf), (LPSTR)buf);
    m_curDirPath = (char*)buf;
    std::cout << "Current directory is " << m_curDirPath << std::endl;
}
CWin32EngineApp::~CWin32EngineApp() {
    m_onIdleTbl.clear();
}
int CWin32EngineApp::GetIntRand(int min, int max) {

    std::uniform_real_distribution<> dist1(min, max);
    return (int)dist1(m_rand);
}
bool CWin32EngineApp::GetBoolRand() {
    return (bool)(m_rand() % 2);
}
#if 0
void CWin32EngineApp::RunScriptString(std::string src, LispScriptEngine::lisp_ptr This) {
    LispScriptEngine::CVM vm;
    LoadScriptFromString(vm, src);
    vm.run();
}
void CWin32EngineApp::RunScriptFile(std::string fileName, LispScriptEngine::lisp_ptr This) {
    std::ifstream in(fileName.c_str());
    std::string buf;
    std::string line;
    while (std::getline(in,line)) {
        buf += line;
        buf += "\n";
    }

    std::cout << fileName << ">>Start" << std::endl;
    RunScriptString(buf);
    std::cout << fileName << ">>End" << std::endl;

}
void CWin32EngineApp::LoadScriptFromString(LispScriptEngine::CVM& vm, std::string src, LispScriptEngine::lisp_ptr This) {
    LispScriptEngine::CLisp2ByteCode gen;
    LispScriptEngine::CLispLexer lexer(src);
    LispScriptEngine::CLispParser parser;
    if (nullptr != This) {
        parser.getRoot()->regist("this", This);
    }
    parser.getRoot()->regist("xscreen", LispScriptEngine::lisp_ptr(new LispScriptEngine::CLispNativeFunc(&CWin32EngineApp::xscreen)));
    parser.getRoot()->regist("gtarget", LispScriptEngine::lisp_ptr(new LispScriptEngine::CLispNativeFunc(&CWin32EngineApp::gtarget)));
    parser.getRoot()->regist("gline", LispScriptEngine::lisp_ptr(new LispScriptEngine::CLispNativeFunc(&CWin32EngineApp::gline)));
    parser.getRoot()->regist("gcircle", LispScriptEngine::lisp_ptr(new LispScriptEngine::CLispNativeFunc(&CWin32EngineApp::gcircle)));
    parser.getRoot()->regist("gbox", LispScriptEngine::lisp_ptr(new LispScriptEngine::CLispNativeFunc(&CWin32EngineApp::gbox)));
    parser.getRoot()->regist("gbegin", LispScriptEngine::lisp_ptr(new LispScriptEngine::CLispNativeFunc(&CWin32EngineApp::gbegin)));
    parser.getRoot()->regist("gend", LispScriptEngine::lisp_ptr(new LispScriptEngine::CLispNativeFunc(&CWin32EngineApp::gend)));
    parser.getRoot()->regist("createRouge", LispScriptEngine::lisp_ptr(new LispScriptEngine::CLispNativeFunc(&CWin32EngineApp::createRouge)));
    parser.getRoot()->regist("resetRouge", LispScriptEngine::lisp_ptr(new LispScriptEngine::CLispNativeFunc(&CWin32EngineApp::resetRouge)));
    parser.getRoot()->regist("gclear", LispScriptEngine::lisp_ptr(new LispScriptEngine::CLispNativeFunc(&CWin32EngineApp::gclear)));
    parser.getRoot()->regist("isRunning", LispScriptEngine::lisp_ptr(new LispScriptEngine::CLispNativeFunc(&CWin32EngineApp::is_running)));
    parser.getRoot()->regist("procEvents", LispScriptEngine::lisp_ptr(new LispScriptEngine::CLispNativeFunc(&CWin32EngineApp::proc_events)));

    parser.getRoot()->regist("bpressed", LispScriptEngine::lisp_ptr(new LispScriptEngine::CLispNativeFunc(&CWin32EngineApp::bpressed)));
    parser.getRoot()->regist("B_LEFT", LispScriptEngine::lisp_ptr(new LispScriptEngine::CLispInt(0)));
    parser.getRoot()->regist("B_LRIGHT", LispScriptEngine::lisp_ptr(new LispScriptEngine::CLispInt(1)));
    parser.getRoot()->regist("B_LUP", LispScriptEngine::lisp_ptr(new LispScriptEngine::CLispInt(2)));
    parser.getRoot()->regist("B_LDOWN", LispScriptEngine::lisp_ptr(new LispScriptEngine::CLispInt(3)));
    parser.getRoot()->regist("B_RUP", LispScriptEngine::lisp_ptr(new LispScriptEngine::CLispInt(4)));
    parser.getRoot()->regist("B_LEFT", LispScriptEngine::lisp_ptr(new LispScriptEngine::CLispInt(5)));
    parser.getRoot()->regist("B_RIGHT", LispScriptEngine::lisp_ptr(new LispScriptEngine::CLispInt(6)));
    parser.getRoot()->regist("B_RDOWN", LispScriptEngine::lisp_ptr(new LispScriptEngine::CLispInt(7)));
    parser.getRoot()->regist("M_LCLICK", LispScriptEngine::lisp_ptr(new LispScriptEngine::CLispInt(8)));
    parser.getRoot()->regist("draw", LispScriptEngine::lisp_ptr(new LispScriptEngine::CLispNativeFunc(&CWin32EngineApp::draw)));
    parser.getRoot()->regist("bg", LispScriptEngine::lisp_ptr(new LispScriptEngine::CLispInt(0)));
    parser.getRoot()->regist("left", LispScriptEngine::lisp_ptr(new LispScriptEngine::CLispInt(1)));
    parser.getRoot()->regist("right", LispScriptEngine::lisp_ptr(new LispScriptEngine::CLispInt(2)));
    parser.getRoot()->regist("center", LispScriptEngine::lisp_ptr(new LispScriptEngine::CLispInt(3)));
    parser.getRoot()->regist("update", LispScriptEngine::lisp_ptr(new LispScriptEngine::CLispNativeFunc(&CWin32EngineApp::update)));
    parser.getRoot()->regist("createChara", LispScriptEngine::lisp_ptr(new LispScriptEngine::CLispNativeFunc(&CWin32EngineApp::createChara)));
    parser.getRoot()->regist("createGridLayout", LispScriptEngine::lisp_ptr(new LispScriptEngine::CLispNativeFunc(&CWin32EngineApp::createGridLayout)));
    parser.getRoot()->regist("createButton", LispScriptEngine::lisp_ptr(new LispScriptEngine::CLispNativeFunc(&CWin32EngineApp::createButton)));
    parser.getRoot()->regist("createText", LispScriptEngine::lisp_ptr(new LispScriptEngine::CLispNativeFunc(&CWin32EngineApp::createText)));
    parser.getRoot()->regist("setBorder", LispScriptEngine::lisp_ptr(new LispScriptEngine::CLispNativeFunc(&CWin32EngineApp::setBorder)));
    parser.getRoot()->regist("add", LispScriptEngine::lisp_ptr(new LispScriptEngine::CLispNativeFunc(&CWin32EngineApp::add)));

    parser.getRoot()->regist("setState", LispScriptEngine::lisp_ptr(new LispScriptEngine::CLispNativeFunc(&CWin32EngineApp::setState)));
    parser.getRoot()->regist("getState", LispScriptEngine::lisp_ptr(new LispScriptEngine::CLispNativeFunc(&CWin32EngineApp::getState)));
    parser.getRoot()->regist("triggered", LispScriptEngine::lisp_ptr(new LispScriptEngine::CLispNativeFunc(&CWin32EngineApp::inputTriggered)));
    parser.getRoot()->regist("cursorPos", LispScriptEngine::lisp_ptr(new LispScriptEngine::CLispNativeFunc(&CWin32EngineApp::getCursorPos)));


    parser.parse(lexer);
    gen.parse(parser, vm);
    vm.dump();

}
#endif
void CWin32EngineApp::LoadGraphFromFile(std::string fileName) {
    
    CGraphManager::GetInstance()->pushFromFile(fileName);
    node_ptr root = CGraphManager::GetInstance()->getRoot();
    if (nullptr != root) {
        CScreenNode* lpScreen = dynamic_cast<CScreenNode*>(root.get());
        if (nullptr != lpScreen) {
            setMainWndSize(lpScreen->size().cx, lpScreen->size().cy);
        }
    }
    root->onActivate(true);
}
int CWin32EngineApp::run() {
    while (!m_mainWindow->isClosed()) {
        ProcEvents();
    
    }
    return 0;
}
void CWin32EngineApp::ProcEvents() {
    CWin32EngineApp::GetInstance()->m_mainWindow->GetCurRenderTarget()->BeginDraw();
    CWin32EngineApp::GetInstance()->m_mainWindow->GetCurRenderTarget()->Clear();

    CGraphManager::GetInstance()->Paint(CWin32EngineApp::GetInstance()->m_mainWindow->GetCurRenderTarget(),
        *CWin32EngineApp::GetInstance()->m_mainWindow->GetTextFormat());
    CWin32EngineApp::GetInstance()->m_mainWindow->GetCurRenderTarget()->EndDraw();


    CWin32EngineApp::GetInstance()->m_mainWindow->repaint();
    CWin32App::ProcEvents();


    for (CWin32Keystate& ks : m_keyState) {
        ks.update();
    }
    ::GetCursorPos(&m_cursorPos);
    ScreenToClient(CWin32EngineApp::GetInstance()->m_mainWindow->GetHWND(), &m_cursorPos);
    CGraphManager::GetInstance()->update();
    node_ptr prevRoot = CGraphManager::GetInstance()->getRoot();
    OnIdleCallbackTbl::iterator it = m_onIdleTbl.begin();
    while (it != m_onIdleTbl.end()) {
        if (!(*it)->OnIdle()) {
            it = m_onIdleTbl.erase(it);
            continue;

        }
        ++it;
    }

    node_ptr curRoot = CGraphManager::GetInstance()->getRoot();
    
    if (curRoot != prevRoot) {
        curRoot->onActivate(true);
    }


}
void CWin32EngineApp::setMainWndSize(int w, int h) {
    if (nullptr == CWin32EngineApp::GetInstance()->m_mainWindow) {
        CWin32EngineApp::GetInstance()->m_mainWindow.reset(new /*CWin32Window*/CWin32EngineWindow(_T("MainWindow"),
            w, h));
    }
    else {
        CWin32EngineApp::GetInstance()->m_mainWindow->resize(
            w,h);

    }
}