#pragma once
#include "Mof.h"
#include "GameDefine.h"
#include "Enemy.h"
#include "Boss.h"
#include "EffectManager.h"
#include "Item.h"

//移動速度
#define			PLAYER_SPEED			0.3f

//移動最大速度
#define			PLAYER_MAXSPEED			5.0f

//移動最大速度
#define			PLAYER_DASHSPEED		12.0f

//ジャンプ初速
#define			PLAYER_JUMP				-10.0f

//落下補正
#define			PLAYER_FALL				65

//攻撃幅
#define			PLAYER_ATTACKWIDTH		 20

//当たり判定減衰幅
#define			PLAYER_RECTDECREASE		 12

//索敵範囲用円
#define			PLAYER_CIRCLE			100

//ガード幅
#define			PLAYER_GUARDWIDTH		30

#define			DAMAGE_REDUCE			0.5


class CPlayer {
private:

	CTexture					m_Texture;
	CSpriteMotionController		m_Motion;
	CVector2					m_Pos;
	bool						m_bMove;
	CVector2					m_Move;
	bool						m_bJump;
	bool						m_bJump2;
	bool						m_bReverse;
	CRectangle					m_SrcRect;
	int							m_HP;
	int							m_DamageWait;

	//HP関連画像
	CTexture					m_FrameTexture;
	CTexture					m_HPTexture;

	//エフェクト
	CEffectManager*				m_PlayerEffectManager;

	//ゲームオーバー
	CEffect*					m_pEndEffect;
	bool						m_bEnd;

	//ダッシュクールタイム
	int							m_DashWait;

	//攻撃カウント
	int							m_AttackCnt;

	//ジャンプカウント			
	int							m_JumpCnt;

	//ダッシュエフェクト
	CEffect*					m_pDashEffect;

	//ガードエフェクト
	CEffect*					m_pGuardEffect;

	//ゴールフラグ(アイテム用)
	bool						m_bGoal;

	//プレイヤーSE
	CSoundBuffer				m_SE_Attack;
	CSoundBuffer				m_SE_Damage;
	CSoundBuffer				m_SE_Guard;
	CSoundBuffer				m_SE_Explosion;
	CSoundBuffer				m_SE_Dash;
	CSoundBuffer				m_SE_Heal;

	//モーション種類定義
	enum tag_MOTION {

		MOTION_WAIT,
		MOTION_MOVE,
		MOTION_JUMPSTART,
		MOTION_JUMPEND,
		MOTION_DASHSTART,
		MOTION_DASHEND,
		MOTION_ATTACK,
		MOTION_DAMAGE,
		MOTION_GUARD,
		MOTION_ATTACK2,

		MOTION_COUNT,
	};

public:
	CPlayer();
	~CPlayer();
	bool Load();
	void Initialize();
	void Update();
	void UpdateKey();
	void UpdateMove();
	void Render(float wx, float wy);
	void RenderDebug(float wx, float wy);
	void Release();
	bool IsAttack() { return m_Motion.GetMotionNo() == MOTION_ATTACK || m_Motion.GetMotionNo() == MOTION_ATTACK2; }
	bool IsGuard() { return m_Motion.GetMotionNo() == MOTION_GUARD; }

	CRectangle GetRect() {
		if (IsAttack())
		{
			return CRectangle(m_Pos.x + PLAYER_RECTDECREASE, m_Pos.y + PLAYER_RECTDECREASE,
								m_Pos.x + m_SrcRect.GetWidth() - PLAYER_RECTDECREASE - PLAYER_ATTACKWIDTH, m_Pos.y + m_SrcRect.GetHeight());
		}
		return CRectangle(m_Pos.x + PLAYER_RECTDECREASE, m_Pos.y + PLAYER_RECTDECREASE,
								m_Pos.x + m_SrcRect.GetWidth() - PLAYER_RECTDECREASE, m_Pos.y + m_SrcRect.GetHeight());
	}
	CRectangle GetAttackRect() {
		//反転中
		if (m_bReverse)
		{
			return CRectangle(m_Pos.x - PLAYER_ATTACKWIDTH, m_Pos.y,
								m_Pos.x + PLAYER_RECTDECREASE, m_Pos.y + m_SrcRect.GetHeight());
		}
		return CRectangle(m_Pos.x + m_SrcRect.GetWidth() - PLAYER_RECTDECREASE - PLAYER_ATTACKWIDTH, m_Pos.y,
								m_Pos.x + m_SrcRect.GetWidth() , m_Pos.y + m_SrcRect.GetHeight());
	}
	CRectangle GetGuardRect() {
		if (m_bReverse)
		{
			return CRectangle(m_Pos.x + m_SrcRect.GetWidth() , m_Pos.y + m_SrcRect.GetHeight(),
								m_Pos.x - m_SrcRect.GetWidth() + PLAYER_RECTDECREASE + PLAYER_GUARDWIDTH, m_Pos.y);
		}
		return CRectangle(m_Pos.x + m_SrcRect.GetWidth() - PLAYER_RECTDECREASE - PLAYER_GUARDWIDTH, m_Pos.y,
								m_Pos.x + m_SrcRect.GetWidth(), m_Pos.y + m_SrcRect.GetHeight());
	}

	//ステージとの当たり
	void ColisionStage(float ox, float oy);

	//敵との衝突
	bool ColisionEnemy(CEnemy& ene);

	//HP表示
	void RenderStatus();

	//エフェクト
	void SetEffectManager(CEffectManager* pmng) { m_PlayerEffectManager = pmng; }

	//ゲームオーバー
	bool IsEnd() { return m_bEnd; }

	//アイテム
	bool ColisionItem(CItem& itm);

	//敵との索敵範囲判定用円
	CCircle GetSearchPlayerCircle()
	{
		return CCircle(m_Pos.x + PLAYER_RECTDECREASE, m_Pos.y + PLAYER_RECTDECREASE, PLAYER_CIRCLE);
	}

	const CVector2 GetPosition() { return m_Pos; }

	//敵弾との当たり判定
	void ColisionEnemyShot(CEnemyShot& shot);

	//ボスとの当たり判定
	bool ColisionBoss(CBoss& boss);

	//ボス弾との当たり判定
	void ColisionBossShot(CEnemyShot& shot);

	//ゴールアイテム取得
	bool IsGoal(void) { return m_bGoal; }
};