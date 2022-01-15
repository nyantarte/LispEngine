#include"Win32EngineApp.h"
#if 0
#include "LispRouge.h"

#include"LispUI.h"
#include"ButtonEx.h"
void CWin32EngineApp::xscreen(int argNum, void* p) {
    LispScriptEngine::CVM* lpVM = (LispScriptEngine::CVM*)p;
    int h = lpVM->popInt();
    int w = lpVM->popInt();
    if (nullptr == CWin32EngineApp::GetInstance()->m_mainWindow) {
        CWin32EngineApp::GetInstance()->m_mainWindow.reset(new /*CWin32Window*/CWin32EngineWindow(_T("MainWindow"), w, h));
    }
    else {
        CWin32EngineApp::GetInstance()->m_mainWindow->resize(w, h);
    }

}
void CWin32EngineApp::gtarget(int argNum, void* p) {}
void CWin32EngineApp::gline(int argNum, void* p) {
    LispScriptEngine::CVM* lpVM = (LispScriptEngine::CVM*)p;
    int y2 = lpVM->popInt();
    int x2 = lpVM->popInt();
    int y1 = lpVM->popInt();
    int x1 = lpVM->popInt();

    CWin32EngineApp::GetInstance()->m_mainWindow->GetCurRenderTarget()->DrawLine(x1, y1, x2, y2, 1);


}
void CWin32EngineApp::gcircle(int argNum, void* p) {
    LispScriptEngine::CVM* lpVM = (LispScriptEngine::CVM*)p;
    int r = lpVM->popInt();
    int y = lpVM->popInt();
    int x = lpVM->popInt();


    CWin32EngineApp::GetInstance()->m_mainWindow->GetCurRenderTarget()->DrawEllipse(x, y, r, r, 1);

}
void CWin32EngineApp::gbox(int argNum, void* p) {
    LispScriptEngine::CVM* lpVM = (LispScriptEngine::CVM*)p;
    int h = lpVM->popInt();
    int w = lpVM->popInt();
    int t = lpVM->popInt();
    int l = lpVM->popInt();

    CWin32EngineApp::GetInstance()->m_mainWindow->GetCurRenderTarget()->DrawRectangle(l, t, w, h, 1);

}
void CWin32EngineApp::gbegin(int argNum, void* p) {
    CWin32EngineApp::GetInstance()->m_mainWindow->GetCurRenderTarget()->BeginDraw();

}
void CWin32EngineApp::gend(int argNum, void* p) {
    CWin32EngineApp::GetInstance()->m_mainWindow->GetCurRenderTarget()->EndDraw();

}
void CWin32EngineApp::gclear(int argNum, void* p) {
    LispScriptEngine::CVM* lpVM = (LispScriptEngine::CVM*)p;
    
        CWin32EngineApp::GetInstance()->m_mainWindow->GetCurRenderTarget()->Clear();
    
}

void CWin32EngineApp::createRouge(int argNum, void* p) {
    LispScriptEngine::CVM* lpVM = (LispScriptEngine::CVM*)p;

    SIZE size;
    CWin32EngineApp::GetInstance()->m_mainWindow->GetSize(size);
    lpVM->setRCValue(LispScriptEngine::lisp_ptr(new CLispRouge(size.cx / ROUGE_GRID_NUM_X, size.cy / ROUGE_GRID_NUM_Y)));
}
void CWin32EngineApp::resetRouge(int argNum, void* p) {
    LispScriptEngine::CVM* lpVM = (LispScriptEngine::CVM*)p;
    LispScriptEngine::lisp_ptr obj = lpVM->popObj();
    CLispRouge* pR = dynamic_cast<CLispRouge*>(obj.get());
    assert(nullptr != pR);
    pR->value().reset();
    pR->value().GetCharaPositionInfo(0).moveType = TO_STAIRS;
}



void CWin32EngineApp::is_running(int argNum, void* p) {
    LispScriptEngine::CVM* lpVM = (LispScriptEngine::CVM*)p;
    lpVM->setRCValue(!CWin32EngineApp::GetInstance()->m_mainWindow->isClosed());

}
void CWin32EngineApp::proc_events(int argNum, void* p) {
    LispScriptEngine::CVM* lpVM = (LispScriptEngine::CVM*)p;
    CWin32EngineApp::GetInstance()->ProcEvents();

}
void CWin32EngineApp::bpressed(int argNum, void* p) {
    LispScriptEngine::CVM* lpVM = (LispScriptEngine::CVM*)p;

    int btn = lpVM->popInt();
    int cntrl = lpVM->popInt();



    lpVM->setRCValue(CWin32EngineApp::GetInstance()->m_keyState[btn].isPressed());


}
void CWin32EngineApp::draw(int argNum, void* p) {
    LispScriptEngine::CVM* lpVM = (LispScriptEngine::CVM*)p;
    LispScriptEngine::lisp_ptr obj = lpVM->popObj();
    if (CLispRouge::s_selfType == obj->getType()) {
        CLispRouge* lpRouge = reinterpret_cast<CLispRouge*>(obj.get());
        lpRouge->paint(CWin32EngineApp::GetInstance()->m_mainWindow->GetCurRenderTarget());

    }
    else if (CLispUI::s_selfType == obj->getType()) {
        CLispUI* lpUI = reinterpret_cast<CLispUI*>(obj.get());
        lpUI->paint(CWin32EngineApp::GetInstance()->m_mainWindow->GetCurRenderTarget(),
            *CWin32EngineApp::GetInstance()->m_mainWindow->GetTextFormat().get());
    }

}
void CWin32EngineApp::gtext(int argNum, void* p) {
    LispScriptEngine::CVM* lpVM = (LispScriptEngine::CVM*)p;
    
}

void CWin32EngineApp::gload(int argNum, void* p) {
    LispScriptEngine::CVM* lpVM = (LispScriptEngine::CVM*)p;
    

}

void CWin32EngineApp::update(int argNum, void* p) {
    LispScriptEngine::CVM* lpVM = (LispScriptEngine::CVM*)p;

    LispScriptEngine::lisp_ptr obj = lpVM->popObj();
    CLispRouge* lpRouge = reinterpret_cast<CLispRouge*>(obj.get());
    lpRouge->value().update();

}
void CWin32EngineApp::createChara(int argNum, void* p) {
    std::cout << "Begin createChara" << std::endl;
    LispScriptEngine::CVM* lpVM = (LispScriptEngine::CVM*)p;

    std::string name = lpVM->popStr();
    CWin32EngineApp::GetInstance()->m_charaTbl.push_back(chara_ptr(new CCharactor(name)));
    std::cout << "Charactor regist succedded.Name=" << name << std::endl;
    std::cout << "Charactor total num=" << CWin32EngineApp::GetInstance()->m_charaTbl.size() << std::endl;
    std::cout << "End createChara" << std::endl;
}
void CWin32EngineApp::createGridLayout(int argNum, void* p) {
    
    LispScriptEngine::CVM* lpVM = (LispScriptEngine::CVM*)p;
    int yGridNum = lpVM->popInt();
    int xGridNum = lpVM->popInt();
    
    lpVM->setRCValue(LispScriptEngine::lisp_ptr(
        new CLispUI(ui_ptr(new D2DLib::CGridLayout(xGridNum,yGridNum)))
    ));
    
}
void CWin32EngineApp::createButton(int argNum, void* p) {
    LispScriptEngine::CVM* lpVM = (LispScriptEngine::CVM*)p;

    int tag = -1;
    if (1 < argNum) {
        tag = lpVM->popInt();
    }
    std::string caption = lpVM->popStr();
    lpVM->setRCValue(LispScriptEngine::lisp_ptr(
        new CLispUI(ui_ptr(new CButtonEx(caption,tag)))
    ));

}
void CWin32EngineApp::createText(int argNum, void* p) {
    LispScriptEngine::CVM* lpVM = (LispScriptEngine::CVM*)p;
    std::string caption = lpVM->popStr();
    lpVM->setRCValue(LispScriptEngine::lisp_ptr(
        new CLispUI(ui_ptr(new D2DLib::CText(caption)))
    ));
   
}
void CWin32EngineApp::setBorder(int argNum, void* p) {
    LispScriptEngine::CVM* lpVM = (LispScriptEngine::CVM*)p;
    int h = lpVM->popInt();
    int w = lpVM->popInt();
    int y = lpVM->popInt();
    int x = lpVM->popInt();

    LispScriptEngine::lisp_ptr obj = lpVM->popObj();
    if (CLispUI::s_selfType == obj->getType()) {
        CLispUI* lpUI = reinterpret_cast<CLispUI*>(obj.get());

        CRect border(x, y, w, h);

        lpUI->value()->SetBorderRect(border);

    }

}

void CWin32EngineApp::add(int argNum, void* p) {
    LispScriptEngine::CVM* lpVM = (LispScriptEngine::CVM*)p;
    if (4==argNum) {
        LispScriptEngine::lisp_ptr childUI = lpVM->popObj();
        int yGrid = lpVM->popInt();
        int xGrid = lpVM->popInt();
        LispScriptEngine::lisp_ptr layout = lpVM->popObj();
        if (layout->getType() == CLispUI::s_selfType) {
            D2DLib::CGridLayout* pGridLayout = dynamic_cast<D2DLib::CGridLayout*>(
                reinterpret_cast<CLispUI*>(layout.get())->value().get());
            assert(nullptr != pGridLayout);

            
            pGridLayout->Add(xGrid,yGrid,(reinterpret_cast<CLispUI*>(childUI.get()))->value());
        }
    }
}
void CWin32EngineApp::setState(int argNum, void* p) {
    std::cout << "Begin setState" << std::endl;
    LispScriptEngine::CVM* lpVM = (LispScriptEngine::CVM*)p;
    LispScriptEngine::lisp_ptr o = lpVM->popObj();
    std::string name = lpVM->popStr();
    CWin32EngineApp::GetInstance()->m_stateTbl.insert(StateTbl::value_type(name, o));
    std::cout << "End setState" << std::endl;
}
void CWin32EngineApp::getState(int argNum, void* p) {
    std::cout << "Begin getState" << std::endl;
    LispScriptEngine::CVM* lpVM = (LispScriptEngine::CVM*)p;
    std::string name = lpVM->popStr();
    StateTbl::iterator it=CWin32EngineApp::GetInstance()->m_stateTbl.find(name);
    if (CWin32EngineApp::GetInstance()->m_stateTbl.end() != it) {
        lpVM->setRCValue(it->second);
    }
    else {
        throw LispScriptEngine::CLispRuntimeError();
    }
    std::cout << "End getState" << std::endl;


}

void CWin32EngineApp::inputTriggered(int argNum, void* p) {
    LispScriptEngine::CVM* lpVM = (LispScriptEngine::CVM*)p;
    int k = lpVM->popInt();
    lpVM->setRCValue(CWin32EngineApp::GetInstance()->m_keyState[k].isTriggered());
}
void CWin32EngineApp::getCursorPos(int argNum, void* p) {

    LispScriptEngine::CVM* lpVM = (LispScriptEngine::CVM*)p;

    CPoint& pos = CWin32EngineApp::GetInstance()->m_cursorPos;
    LispScriptEngine::CLispArray* lpA = new LispScriptEngine::CLispArray();
    lpA->value().push_back(LispScriptEngine::lisp_ptr(new LispScriptEngine::CLispInt(pos.x)));
    lpA->value().push_back(LispScriptEngine::lisp_ptr(new LispScriptEngine::CLispInt(pos.y)));

    lpVM->setRCValue((LispScriptEngine::lisp_ptr(lpA)));

}
#endif