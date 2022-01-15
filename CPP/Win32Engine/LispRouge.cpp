#include"LispRouge.h"
LispScriptEngine::lisp_ptr CLispRouge::s_selfType(new LispScriptEngine::CLispType("rouge"));
CLispRouge::CLispRouge(int xGridSize, int yGridSize)
:CLispPrimitive(s_selfType),
 m_gridSize({xGridSize,yGridSize})
{}
void CLispRouge::paint(image_render_target_ptr pTarget) {
	pTarget->BeginDraw();
	for (int i = 0; i < ROUGE_GRID_NUM_Y; ++i) {
		for (int j = 0; j < ROUGE_GRID_NUM_X; ++j) {
			char c=value().GetTile(j, i);
			if (-1 != c) {
				pTarget->FillRectangle(j * m_gridSize.cx, i * m_gridSize.cy, m_gridSize.cx, m_gridSize.cy);
			}
		
		}
	}

	brush_ptr pBrush = pTarget->GetCurrentBrush();
	pTarget->SetCurrentBrush(brush_ptr(new CD2DSolidBrush(*pTarget, D2D1::ColorF(D2D1::ColorF::Red))));
	for (int i = 1; i < ROUGE_CHARA_NUM; ++i) {
		CPoint& pos = value().GetCharaPositions(i);
		pTarget->FillRectangle(m_gridSize.cx*pos.x,m_gridSize.cy*pos.y, m_gridSize.cx, m_gridSize.cy);
	}
	pTarget->SetCurrentBrush(brush_ptr(new CD2DSolidBrush(*pTarget, D2D1::ColorF(D2D1::ColorF::Green))));
	{
		CPoint& pos = value().GetCharaPositions(0);
		pTarget->FillRectangle(m_gridSize.cx * pos.x, m_gridSize.cy * pos.y, m_gridSize.cx, m_gridSize.cy);
	}
	
	pTarget->EndDraw();
	pTarget->SetCurrentBrush(pBrush);
}