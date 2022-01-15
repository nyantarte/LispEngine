#pragma once
#include"Win32EngineFramework.h"

class CLispRouge :
    public LispScriptEngine::CLispPrimitive<CRouge>
{
    SIZE m_gridSize;
public:
    static LispScriptEngine::lisp_ptr s_selfType;

public:
    CLispRouge(int xGridSize, int yGridSize);

    void paint(image_render_target_ptr pTarget);

    std::string toString(){
        return value().toString();
    }
};

