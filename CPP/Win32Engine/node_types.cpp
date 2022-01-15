#include"Win32EngineApp.h"
CImageNode::CImageNode() {
	

}
CImageNode::~CImageNode() {

}
bool CImageNode::OnIdle() {
	return false;
}

void CImageNode::onActivate(bool flag) {
	if (flag) {
		CWin32EngineApp::GetInstance()->GetIdleCallbackTbl().insert(this);
	}
	else {
		CWin32EngineApp::GetInstance()->GetIdleCallbackTbl().erase(this);

	}
}
CTextNode::CTextNode() {
	
}
CTextNode::~CTextNode() {
}
bool CTextNode::OnIdle() 
{
	if (CWin32EngineApp::GetInstance()->IsMouseClicked()) {
		CPoint& pos = CWin32EngineApp::GetInstance()->GetCursorPos();
		if (border().IsInside(pos)) {
			node_ptr onClickNode=CGraphManager::GetInstance()->getNode(m_onClick);
			if (nullptr != onClickNode) {
				
				onClickNode->exec();
				return false;
			}
		}
	}
	return true;
}
void CTextNode::onActivate(bool flag) {
	if (flag) {
		CWin32EngineApp::GetInstance()->GetIdleCallbackTbl().insert(this);
	}
	else {
		CWin32EngineApp::GetInstance()->GetIdleCallbackTbl().erase(this);

	}
}

