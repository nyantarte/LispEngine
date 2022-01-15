#include "Rouge.h"



std::shared_ptr<CMoveFunctor> CMoveFunctor::s_Mock(new CMoveFunctor());
std::shared_ptr<CMoveFunctor> CMoveStairsFunctor::s_Self(new CMoveStairsFunctor());
std::shared_ptr<CMoveFunctor> CMoveNextRoomFunctor::s_Self(new CMoveNextRoomFunctor());

int CRouge::m_searchBuf[ROUGE_GRID_NUM_Y][ROUGE_GRID_NUM_X];
std::mt19937 CRouge::m_rand;


CMoveFunctor* CMoveStairsFunctor::operator()(CRouge& r, CPositionInfo& target) {
	if (r.StairPos() == target.pos) {
		return nullptr;
	}
	int rIdx = r.GetRoomInside(target.pos);
	CPoint rCenter;
	r.RoomRect(rIdx).GetCenterPos(rCenter);
	if (rCenter == target.pos) {
		return CMoveNextRoomFunctor::s_Self.get();
	}
	if (r.Search(target.pos.x, target.pos.y, rCenter.x, rCenter.y, 0, target.moveDirList)) {
		return CMoveNextRoomFunctor::s_Self.get();
	}
	CPoint& sPos = r.StairPos();
	if (!r.Search(target.pos.x, target.pos.y, sPos.x, sPos.y, 0, target.moveDirList)) {
		return nullptr;
	}
	return nullptr;
}

CMoveFunctor* CMoveNextRoomFunctor::operator()(CRouge& r, CPositionInfo& target) {
	int rIdx = r.GetRoomInside(target.pos);
	if ((ROUGE_DEF_ROOM_NUM_MAX - 1) == rIdx) {
		return nullptr;
	}

	CPoint rCenter;
	r.RoomRect(rIdx+1).GetCenterPos(rCenter);
	if (r.Search(target.pos.x, target.pos.y, rCenter.x, rCenter.y, 0, target.moveDirList)) {
		return nullptr;
	}

	return nullptr;
}
CRouge::CRouge() {
	m_particleManager.particleTbl().resize(BULLET_MAX_NUM);
}
void CRouge::reset() {
	m_isBattle = false;
	m_moveCharaIdx = 0;
	m_particleManager.reset();
	for (int i = 0; i < ROUGE_GRID_NUM_Y; ++i) {

		for (int j = 0; j < ROUGE_GRID_NUM_X; ++j) {
			m_tiles[i][j] = INVALID_TILE;
		}
	}
	int roomNum = 0;
	
	SetRect(&m_roomRects[0], ROUGE_GRID_NUM_X / 2 + 1, 0, ROUGE_GRID_NUM_X, ROUGE_GRID_NUM_Y);
	SetRect(&m_roomRects[1], 0, 0, ROUGE_GRID_NUM_X / 2, ROUGE_GRID_NUM_Y / 2);
	SetRect(&m_roomRects[2], 0, ROUGE_GRID_NUM_Y / 2 + 1, ROUGE_GRID_NUM_X / 2, ROUGE_GRID_NUM_Y);
	for (int i = 0; i < ROUGE_DEF_ROOM_NUM_MAX; ++i) {
		RECT r = m_roomRects[i];
		SplitRect(&r, &m_roomRects[i]);
	}


	for (int i = 0; i < ROUGE_DEF_ROOM_NUM_MAX; ++i) {
		RECT* pR = &m_roomRects[i];
		for (int j = pR->top; j < pR->bottom; ++j) {
			for (int k = pR->left; k < pR->right; ++k) {
				m_tiles[j][k] = i + 1;
			}

		}
	}

	for (int i = 0; i < (ROUGE_DEF_ROOM_NUM_MAX - 1); ++i) {
		CreateRoute(&m_roomRects[i], &m_roomRects[i + 1]);
		m_roomRects[i].GetCenterPos(m_paths[i].first);
		m_roomRects[i+1].GetCenterPos(m_paths[i].second);

	}
	
	POINT stairPos;
	stairPos.x = m_roomRects[ROUGE_DEF_ROOM_NUM_MAX - 1].left +
		(m_roomRects[ROUGE_DEF_ROOM_NUM_MAX - 1].right - m_roomRects[ROUGE_DEF_ROOM_NUM_MAX - 1].left) / 2;
	stairPos.y = m_roomRects[ROUGE_DEF_ROOM_NUM_MAX - 1].top +
		(m_roomRects[ROUGE_DEF_ROOM_NUM_MAX - 1].bottom - m_roomRects[ROUGE_DEF_ROOM_NUM_MAX - 1].top) / 2;

	
	
	CPoint& po=m_charaPositions[0].pos;
	m_roomRects[0].GetCenterPos(po);
	
	for (int l = 0; l < ROUGE_CHARA_NUM; ++l) {
	

		m_charaPositions[l].moveDirList.clear();
		m_charaPositions[l].cur = m_charaPositions[l].moveDirList.end();
		m_charaPositions[l].moveFunctor = nullptr;
	}
	int i = 1;

		for (int j = 1; j < ROUGE_DEF_ROOM_NUM_MAX; ++j) {
			CRect& room = m_roomRects[j];
			int size = room.GetWidth() * room.GetHeight();
			int cNum = GetIntRand(1, min(ROUGE_CHARA_NUM-i,size));
			for (int l = 0; l < cNum; ++l) {
				CPoint& cPos = m_charaPositions[i].pos;
				do {
					cPos.x = GetIntRand(room.left, room.right);
					cPos.y = GetIntRand(room.top, room.bottom);
					bool isConflict = false;
					for (int k = 0; k < i; ++k) {
						CPoint& checkPos = m_charaPositions[k].pos;
						if (checkPos.x == cPos.x && checkPos.y == cPos.y) {
							isConflict = true;
							break;
						}
					}
					if (!isConflict) {
						

						break;
					}
				} while (true);
				++i;
			}

		}

		m_tiles[stairPos.y][stairPos.x] = -2;
		m_stairPos = stairPos;
}

void CRouge::SplitRect(RECT* pIn, RECT* pOut) {
	if (GetBoolRand()) {
		int w = GetIntRand(ROUGE_GRID_MIN_W, pIn->right - pIn->left - ROUGE_GRID_MIN_W);
		int h = GetIntRand(ROUGE_GRID_MIN_H, pIn->bottom - pIn->top - ROUGE_GRID_MIN_H);
		*pOut = *pIn;
		pOut->right = pOut->left + w;
		pOut->bottom = pOut->top + h;
	}
	else if (ROUGE_GRID_MIN_W < (pIn->right - pIn->left) / 2 || ROUGE_GRID_MIN_H < (pIn->bottom - pIn->top) / 2) {
		*pOut = *pIn;
	}
	else if (GetBoolRand()) {
		RECT r;
		SetRect(&r, pIn->left, pIn->top, pIn->left + (pIn->right - pIn->left) / 2, pIn->bottom);
		SplitRect(&r, pOut);
	}
	else {
		RECT r;
		SetRect(&r, pIn->left, pIn->top, pIn->right, pIn->top - (pIn->bottom - pIn->top));
		SplitRect(&r, pOut);

	}


}
void CRouge::print() {
/*	for (int i = 0; i < ROUGE_GRID_NUM_Y; ++i) {
		for (int j = 0; j < ROUGE_GRID_NUM_X; ++j) {
			char buf[16];
			sprintf_s(buf, sizeof(buf), "%2d", m_tiles[i][j]);
			std::cout << buf << ",";
		}
		std::cout << std::endl;

	}
	*/
	std::cout << toString();

}
std::string CRouge:: toString() {
	std::stringstream ss;
	for (int i = 0; i < ROUGE_GRID_NUM_Y; ++i) {
		for (int j = 0; j < ROUGE_GRID_NUM_X; ++j) {
//			char buf[16];
//			sprintf_s(buf, sizeof(buf), "%2d", m_tiles[i][j]);
//			std::cout << buf << ",";
			ss << std::setw(2) << (int)m_tiles[i][j]<<",";
		}
		ss << std::endl;

	}
	return ss.str();
}
void CRouge::update() {
	
	CPositionInfo& pi = m_charaPositions[m_moveCharaIdx];
	if (0 < m_battleCount) {
		--m_battleCount;
		m_particleManager.update();
		CSize gridSize(m_rougeSize.GetWidth() / ROUGE_GRID_NUM_X,
			m_rougeSize.GetHeight() / ROUGE_GRID_NUM_Y);
		for (CBullet& b : m_particleManager.particleTbl()) {
			if (0 >= b.life()) {
				continue;
			}
			for (CPositionInfo& pi : m_charaPositions) {
				if (b.getOwner() != pi.chara) {
					CVector cPos(pi.pos.x * gridSize.cx, pi.pos.y * gridSize.cy);
					if (b.pos().isInside(cPos, b.rad())) {
						b.life() = 0;
						pi.chara->getCurrentStatus().life -= b.getOwner()->getCurrentStatus().atk;
						m_battleCount = 0;
						break;
					}
				}
			}
		}
		if (0< m_battleCount){
			return;
		}
	}else if (pi.moveDirList.end()==pi.cur && nullptr!=pi.chara) {
		if (!(CCharactor::FLAG::FIELD_OBJECT & pi.chara->flags())) {
			
			memset(m_searchBuf, -1, sizeof(m_searchBuf));
			pi.moveDirList.clear();
			
			if (nullptr == pi.moveFunctor) {

				if (0!=(pi.chara->flags() & CCharactor::PLAYER_USE)) {
					pi.moveFunctor = CMoveStairsFunctor::s_Self.get();


				}
				else {
					pi.moveFunctor = CMoveFunctor::s_Mock.get();
				}
			}
			pi.moveFunctor = (*(pi.moveFunctor))(*this, pi);
			if (1 < pi.moveDirList.size()) {
				pi.moveDirList.erase(pi.moveDirList.begin());
				pi.cur = pi.moveDirList.begin();
			}
		}

		

	}
	else if(pi.moveDirList.end() != pi.cur){
		
		CPoint& nextPos = *pi.cur;
		if (0!=(CCharactor::PLAYER_USE & pi.chara->flags())) {

			for (int i = 1; i < ROUGE_CHARA_NUM; ++i) {

				CPositionInfo& checkPI = m_charaPositions[i];
				if (FireIfInRange(pi, checkPI)) {
					return;
				}
				
			}
		}
		else {
			CPositionInfo& checkPI = m_charaPositions[0];
			if (FireIfInRange(pi, checkPI)) {
				return;
			}


		}
		pi.pos = *pi.cur;
		++pi.cur;
	//	std::cout << pi.pos.x << "," << pi.pos.y << std::endl;
	}

	m_moveCharaIdx = (m_moveCharaIdx + 1) % ROUGE_CHARA_NUM;

}

bool CRouge::Search(int x, int y, int tx,int ty, int stepNum, std::list<CPoint>& r) {
	float goalDist = 0;
	static CPoint searchDir[] = {
		CPoint(-1,0),
		CPoint(1,0),
		CPoint(0,-1),
		CPoint(0,1)
	};
	float dirDist[4];
	CPoint targetPos(tx, ty);
	CPoint curPos(x, y);
	int dist = m_searchBuf[y][x];
	if (x == tx && y == ty) {
		goto Succ;
	}
	//éwíËÇ≥ÇÍÇΩç¿ïWÇ™äKíiÇæÇ¡ÇΩ
		goalDist = targetPos.GetDist(curPos);
		for (int i = 0; i < 4; ++i) {
			CPoint& dir = searchDir[i];
			dirDist[i] = targetPos.GetDist(x + dir.x, y + dir.y);
		}
	

	//Ç∑Ç≈Ç…íTçıçœÇ›ÇÃç¿ïWÇæÇ¡ÇΩ
	if (-1 < dist || -1 == GetTile(x, y)) {
		return false;
	}
	m_searchBuf[y][x] = stepNum;

	for (int i = 0; i < 4; ++i) {
		float dist = dirDist[i];
		CPoint& dir = searchDir[i];
		if (dist < goalDist && Search(x + dir.x, y + dir.y, targetPos.x,targetPos.y, stepNum + 1, r)) {
			goto Succ;
		}
	}
Succ:
	//å©Ç¬Ç©Ç¡ÇΩ
	r.push_front(CPoint(x, y));
	return true;

}



void CRouge::CreateRoute(RECT* pR1, RECT* pR2) {
	int _x1 = pR1->left + (pR1->right - pR1->left) / 2;
	int _y1 = pR1->top + (pR1->bottom - pR1->top) / 2;
	int _x2 = pR2->left + (pR2->right - pR2->left) / 2;
	int _y2 = pR2->top + (pR2->bottom - pR2->top) / 2;

	float x1 = _x1 + 0.5;
	float y1 = _y1 + 0.5;
	float x2 = _x2 + 0.5;
	float y2 = _y2 + 0.5;

	float x = x1;
	float y = y1;

	float cell_w = 1.0;
	float cell_h = 1.0;
	float step_x = 0 < (x2 - x1) ? 1 : -1;
	float step_y = 0 < (y2 - y1) ? 1 : -1;
	float delta_x = cell_w / abs(x2 - x1);
	float delta_y = cell_h / abs(y2 - y1);
	float tmp;
	float max_x = delta_x * modff((x1 / cell_w), &tmp);
	float max_y = delta_y * modff((y1 / cell_h), &tmp);

	m_tiles[(int)y][(int)x] = ROUTE_TILE;
	bool reached_x = false;
	bool reached_y = false;
	
	while (!(reached_x && reached_y)) {

		if (max_x < max_y) {
			max_x += delta_x;
			x += step_x;
		}
		else {
			max_y += delta_y;
			y += step_y;
		}

		m_tiles[(int)y][(int)x] = ROUTE_TILE;

		if ((step_x > 0 && x >= x2) ||
			(step_x <= 0 && x <= x2)) {
			reached_x = true;
		}
		if ((step_y > 0 && y >= y2) ||
			(step_y <= 0 && y <= y2)) {
			reached_y = true;
		}
	}
	
}
int CRouge::GetIntRand(int min, int max) {

	std::uniform_real_distribution<> dist1(min, max);
	return (int)dist1(m_rand);
}
bool CRouge::GetBoolRand() {
	return (bool)(m_rand() % 2);
}
int CRouge::GetRoomInside(CPoint& pos) {
	for (int i = 0; i < ROUGE_DEF_ROOM_NUM_MAX; ++i) {
		if (m_roomRects[i].IsInside(pos)) {
			return i;
		}
	
	}

	
	return -1;
}
int CRouge::GetRoomInside(int x, int y) {
	CPoint pos(x, y);
	return GetRoomInside(pos);
}
bool CRouge::FireIfInRange(CPositionInfo& atk, CPositionInfo& def) {
	if (nullptr == atk.chara || nullptr == def.chara) {
		return false;
	}
	if (0 >=def.chara->getCurrentStatus().life) {
		return false;
	}
	CSize gridSize(m_rougeSize.GetWidth() / ROUGE_GRID_NUM_X,
		m_rougeSize.GetHeight() / ROUGE_GRID_NUM_Y);

	if (atk.chara->arts().range() > atk.pos.GetDist(def.pos)) {
		m_isBattle = true;
		CVector cPos(atk.pos.x * gridSize.cx, atk.pos.y * gridSize.cy);
		CVector tPos(def.pos.x * gridSize.cx, def.pos.y * gridSize.cy);
		CVector dir = tPos - cPos;
		dir /= BULLET_MOVE_FRAME_NUM;

		CBullet& b = m_particleManager.add(cPos, dir, BULLET_MOVE_FRAME_NUM + 1, atk.chara->arts().rad());
		b.setOwner(atk.chara);
		m_battleCount = BULLET_MOVE_FRAME_NUM + 2;
		return true;
	}

	return false;

}
