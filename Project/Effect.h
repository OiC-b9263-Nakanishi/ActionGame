#pragma once
#include "Mof.h"
#include "GameDefine.h"

enum tag_EFFECTTYPE {
	EFC_DAMAGE,				//ダメージ
	EFC_EXPLOSIONENEMY,		//敵爆破
	EFC_EXPLOSIONPLAYER,	//プレイヤー爆破
	EFC_DASH_PLAYER,		//プレイヤーダッシュ
	EFC_GUARD_PLAYER,		//プレイヤーガード
	//EFC_DASH_PLAYER2,		//プレイヤーダッシュ

	EFC_TYPECOUNT,
};

class CEffect {
private:
	CTexture*						m_EfcTexture;
	CSpriteMotionController			m_EfcMotion;
	Vector2							m_EfcPos;
	bool							m_bShow;
	CRectangle						m_SrcRect;

public:
	CEffect();
	~CEffect();
	void Initialize(int type);
	void Start(float px, float py);
	void Update();
	void Render(float wx, float wy);
	void RenderDebug(float wx, float wy);
	void Release();
	void SetTexture(CTexture* pt) { m_EfcTexture = pt; }
	bool GetShow() { return m_bShow; }
};