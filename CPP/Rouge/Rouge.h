#pragma once
#include"RougeFramework.h"
#include"RougeConfig.h"



typedef std::list<CPoint> MoveDirList; /*! 一フレーム毎の移動先の座標を保持するリスト*/
class CRouge;

struct CPositionInfo;

/*!
*	@brief マップ内の各オブジェクト移動ルート決定用の関数オブジェクト
*/
class CMoveFunctor {
public:
	static std::shared_ptr<CMoveFunctor> s_Mock;	/*! 何もしない時に使用するインスタンスう*/
public:

	/*
	*	@param r	マップを管理しているオブジェクト
	*	@param target	移動対象となるオブジェクト
	*	@return		次回に実施する移動判定オブジェクトのインスタンス
	*/
	virtual CMoveFunctor* operator()(CRouge& r, CPositionInfo& target){
		return nullptr;
	}
};
class CMoveStairsFunctor :public CMoveFunctor {
public:
	static std::shared_ptr<CMoveFunctor> s_Self;
public:
	virtual CMoveFunctor* operator()(CRouge& r, CPositionInfo& target);
};
class CMoveNextRoomFunctor:public CMoveFunctor {
public:
	static std::shared_ptr<CMoveFunctor> s_Self;
public:
	virtual CMoveFunctor* operator()(CRouge& r, CPositionInfo& target);
};

struct CPositionInfo {
	CPoint pos;
	CMoveFunctor* moveFunctor;

	chara_ptr chara;
	MoveDirList moveDirList;
	MoveDirList::iterator cur;
};

class CRouge
{

public:
	static const int INVALID_TILE = -1;
	static const int ROUTE_TILE = 0;
private:
	char m_tiles[ROUGE_GRID_NUM_Y][ROUGE_GRID_NUM_X];
	static int m_searchBuf[ROUGE_GRID_NUM_Y][ROUGE_GRID_NUM_X];
	CPositionInfo m_charaPositions[ROUGE_CHARA_NUM];
	CPoint m_stairPos;
	int m_moveCharaIdx;
	CRect m_roomRects[ROUGE_DEF_ROOM_NUM_MAX];

	static  std::mt19937 m_rand;
	CRect m_rougeSize;
public:
	typedef CParticleManager<CBullet> ParticleManagerType;
	ParticleManagerType m_particleManager;
	bool m_isBattle;
	int m_battleCount;
	
public:
	CRouge();
	void reset();

	void print();
	std::string toString();
	void update();
	char GetTile(int x, int y) {
		return m_tiles[y][x];
	}

	CPoint& GetCharaPositions(int i) {
		assert(0 <= i && i < ROUGE_CHARA_NUM);
		return m_charaPositions[i].pos;
	}
	CPositionInfo& GetCharaPositionInfo(int i) {
		assert(0 <= i && i < ROUGE_CHARA_NUM);
		return m_charaPositions[i];

	}
	CPoint& StairPos() {
		return m_stairPos;
	}
	ParticleManagerType ParticleManager() {
		return m_particleManager;
	}
	CRect& RougeSize() {
		return m_rougeSize;
	}
	int GetIntRand(int min, int max);

	int GetRoomInside(CPoint& pos);
	int GetRoomInside(int x, int y);
	CRect& RoomRect(int i) {
		return m_roomRects[i];
	}
	bool Search(int x, int y, int tx, int ty, int stepNum, std::list<CPoint>& r);
private:
	void SplitRect(RECT* pIn, RECT* pOut);
	void CreateRoute(RECT* pR1, RECT* pR2);



	bool GetBoolRand();
	bool FireIfInRange(CPositionInfo& atk, CPositionInfo& def);

};

