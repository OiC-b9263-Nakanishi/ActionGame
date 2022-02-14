#pragma once
#include "Mof.h"
#include "Player.h"
#include "Stage.h"
#include "Enemy.h"
#include "EffectManager.h"
#include "Item.h"
#include "Boss.h"


class CGame {
private:

	//�v���C���[
	CPlayer		m_Player;
	//�X�e�[�W
	CStage		m_Stage;
	//�G
	CEnemy*		m_EnemyArray;
	//�G�t�F�N�g
	CEffectManager m_EffectManager;
	//�A�C�e��
	CItem*		   m_ItemArray;
	//�{�X
	CBoss		  m_Boss;

public:
	CGame();
	~CGame();
	bool Load();
	void Initialize();
	void Update();
	void Render();
	void RenderDebug();
	void Release();
};