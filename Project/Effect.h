#pragma once
#include "Mof.h"
#include "GameDefine.h"

enum tag_EFFECTTYPE {
	EFC_DAMAGE,				//�_���[�W
	EFC_EXPLOSIONENEMY,		//�G���j
	EFC_EXPLOSIONPLAYER,	//�v���C���[���j
	EFC_DASH_PLAYER,		//�v���C���[�_�b�V��
	EFC_GUARD_PLAYER,		//�v���C���[�K�[�h
	//EFC_DASH_PLAYER2,		//�v���C���[�_�b�V��

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