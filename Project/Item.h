#pragma once
#include "Mof.h"
#include "GameDefine.h"

//アイテム種類列挙
enum tag_ITEMTYPE {
	ITEM_RECOVER,		//回復
	ITEM_GOAL,			//ゴール

	ITEM_TYPECOUNT,
};

class CItem {
private:
	CTexture*					m_ItemTexture;
	CSpriteMotionController		m_ItemMotion;
	int							m_Type;
	Vector2						m_ItemPos;
	Vector2						m_ItemMove;
	bool						m_bShow;
	CRectangle					m_SrcRect;

public:
	CItem();
	~CItem();
	void Initialize(float px, float py, int type);
	void Update();
	void ColisionStage(float ox, float oy);
	void Render(float wx, float wy);
	void RenderDebug(float wx, float wy);
	void Release();
	void SetTexture(CTexture* pt) { m_ItemTexture = pt; }
	bool GetShow() { return m_bShow; }
	void SetShow(bool bs) { m_bShow = bs; }
	int GetType() { return m_Type; }
	CRectangle GetRect() { return CRectangle(m_ItemPos.x, m_ItemPos.y, m_ItemPos.x + m_SrcRect.GetWidth(), m_ItemPos.y + m_SrcRect.GetHeight()); }
};
