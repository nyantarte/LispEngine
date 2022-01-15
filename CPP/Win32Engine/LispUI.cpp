#include"LispUI.h"

LispScriptEngine::lisp_ptr CLispUI::s_selfType(new LispScriptEngine::CLispType("ui"));
void CLispUI::paint(image_render_target_ptr pTarget, CDWTextFormat& f) {

	D2DLib::ID2DUI* lpUI = reinterpret_cast<D2DLib::ID2DUI*>(value().get());

	lpUI->Paint(pTarget, f);
}