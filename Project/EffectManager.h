#pragma once
#include "Mof.h"
#include "Effect.h"

//�Ǘ��G�t�F�N�g��
#define				EFFECTCOUNT			5

class CEffectManager {
private:
	//�G�t�F�N�g(�z��)
	CEffect			m_Effect[EFFECTCOUNT][EFC_TYPECOUNT];
	//�G�t�F�N�g�摜
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