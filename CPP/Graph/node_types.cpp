#include"node_types.h"
#include"GraphManager.h"
void CRougeNode::Paint(image_render_target_ptr pTarget, CDWTextFormat& f) {
	int gridXSize = border().GetWidth() / ROUGE_GRID_NUM_X;
	int gridYSize=border().GetHeight()/ ROUGE_GRID_NUM_Y;
	//pTarget->BeginDraw();
	for (int i = 0; i < ROUGE_GRID_NUM_Y; ++i) {
		for (int j = 0; j < ROUGE_GRID_NUM_X; ++j) {
			char c = m_rouge.GetTile(j, i);
			if (-1 != c) {
				pTarget->FillRectangle(j * gridXSize, i * gridYSize, gridXSize, gridYSize);
			}

		}
	}

	brush_ptr pBrush = pTarget->GetCurrentBrush();
	pTarget->SetCurrentBrush(brush_ptr(new CD2DSolidBrush(*pTarget, D2D1::ColorF(D2D1::ColorF::Red))));
	for (int i = 1; i < ROUGE_CHARA_NUM; ++i) {
		if (0 >= m_rouge.GetCharaPositionInfo(i).chara->getCurrentStatus().life) {
			continue;
		}
		CPoint& pos = m_rouge.GetCharaPositions(i);
		pTarget->FillRectangle(gridXSize * pos.x, gridYSize * pos.y, gridXSize, gridYSize);
	}
	pTarget->SetCurrentBrush(brush_ptr(new CD2DSolidBrush(*pTarget, D2D1::ColorF(D2D1::ColorF::Blue))));
	CPoint& stairPos = m_rouge.StairPos();
	pTarget->FillRectangle(gridXSize * stairPos.x, gridYSize * stairPos.y, gridXSize, gridYSize);

	pTarget->SetCurrentBrush(brush_ptr(new CD2DSolidBrush(*pTarget, D2D1::ColorF(D2D1::ColorF::Green))));
	{
		CPoint& pos = m_rouge.GetCharaPositions(0);
		pTarget->FillRectangle(gridXSize * pos.x, gridYSize * pos.y, gridXSize, gridYSize);
	}
	CRouge::ParticleManagerType pm = m_rouge.ParticleManager();
	pTarget->SetCurrentBrush(brush_ptr(new CD2DSolidBrush(*pTarget, D2D1::ColorF(D2D1::ColorF::Black))));
	for (CBullet& b : pm.particleTbl()) {
		if (0 < b.life()) {
			CPoint bp = b.pos().ToPoint();
			CRect br(bp, b.rad(), b.rad());
			pTarget->FillRectangle(br.left, br.top, br.GetWidth(), br.GetHeight());
		}
	}
	//pTarget->EndDraw();
	pTarget->SetCurrentBrush(pBrush);
}
void CRougeNode::update() {
	m_rouge.update();
	
}
void CRougeNode::setPCChara(chara_ptr p) {
	m_pcChara = p;
	m_pcChara->flags() |= CCharactor::PLAYER_USE;
	m_rouge.GetCharaPositionInfo(0).chara = m_pcChara;

}

void CRougeNode::reset() {
	m_rouge.reset();
	m_rouge.GetCharaPositionInfo(0).chara = m_pcChara;
	for (int i = 1; i < ROUGE_CHARA_NUM; ++i) {
		int cIdx = m_rouge.GetIntRand(0, CCharaManager::s_Self->charaTbl().size());
		m_rouge.GetCharaPositionInfo(i).chara = CCharaManager::s_Self->charaTbl()[cIdx]->clone();

	}

}
void CTextNode::Paint(image_render_target_ptr target, CDWTextFormat& f) {
	CRect& r = border();
	int size = f.GetDWTextFormat()->GetFontSize();
	//target->BeginDraw();
	target->DrawText(r.left, r.top - size / 4, m_caption.c_str(), f);
	//target->EndDraw();

}
void CNextSceneNode::exec() {
	
	switch (m_moveType) {
	case MOVE_TYPE::POP:
		CGraphManager::GetInstance()->popFromFile(m_target);
		break;
	case MOVE_TYPE::PUSH:
		CGraphManager::GetInstance()->pushFromFile(m_target);
		break;
	case MOVE_TYPE::RETURN:
		CGraphManager::GetInstance()->returnGraph();
		break;
	}
}