#include"ButtonEx.h"
#include"Win32EngineApp.h"
CButtonEx::CButtonEx(std::string caption, int tag)
	:CButton(caption,tag)

{
	CWin32EngineApp::GetInstance()->GetIdleCallbackTbl().push_back(this);

	
}
CButtonEx::~CButtonEx() {
	
	OnIdleCallbackTbl& tbl = CWin32EngineApp::GetInstance()->GetIdleCallbackTbl();
	for (OnIdleCallbackTbl::iterator it = tbl.begin(); it != tbl.end(); ++it) {
		if (this==*it) {
			tbl.erase(it);
			return;
		}
	}
}
void CButtonEx::OnIdle() {

	if (CWin32EngineApp::GetInstance()->IsMouseClicked()) {

		if (GetBorderRect().IsInside(CWin32EngineApp::GetInstance()->GetCursorPos())) {
			SetIsClicked(true);
			return;
		}
	
	}
	SetIsClicked(false);
}