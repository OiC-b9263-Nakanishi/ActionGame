#pragma once
#include "Mof.h"
#include "Effect.h"

//管理エフェクト数
#define				EFFECTCOUNT			5

class CEffectManager {
private:
	//エフェクト(配列)
	CEffect			m_Effect[EFFECTCOUNT][EFC_TYPECOUNT];
	//エフェクト画像
	CTexture		m_Texture[EFC_TYPECOUNT];

public:
	CEffectManager();
	~CEffectManager();
	bool Load();
	void Initialize();
	CEffect* Start(float px ,float py, int type);
	void Update();
	void Render(float wx, float wy);
	void RenderDebug(float wx, float wy);
	void Release();
};