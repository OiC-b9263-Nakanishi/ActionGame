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

	//プレイヤー
	CPlayer		m_Player;
	//ステージ
	CStage		m_Stage;
	//敵
	CEnemy*		m_EnemyArray;
	//エフェクト
	CEffectManager m_EffectManager;
	//アイテム
	CItem*		   m_ItemArray;
	//ボス
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