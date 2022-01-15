#pragma once
#include"Win32EngineFramework.h"

class CLispADV :public LispScriptEngine::CLispObject {
	CADV m_adv;
	LispScriptEngine::CVM m_vm;
	
public:
	static LispScriptEngine::lisp_ptr s_selfType;

public:
	CLispADV(image_render_target_ptr target)
	: CLispObject(s_selfType),
	  m_adv(target)
	{}



	void setText(std::string txt) {
		m_vm.setIsRunning(false);
		//m_adv.addText(utf8_to_wide(txt));
	}

	void loadImage(int idx, std::string txt);
	void clear(int idx);
	
};