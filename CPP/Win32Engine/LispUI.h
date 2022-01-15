#pragma once
#include"Win32EngineFramework.h"
class CLispUI :public LispScriptEngine::CLispPrimitive<ui_ptr> {
public:
	static LispScriptEngine::lisp_ptr s_selfType;

public:
	CLispUI(ui_ptr p)
		:CLispPrimitive(p,s_selfType)
	{}


	void paint(image_render_target_ptr pTarget, CDWTextFormat& f);
};