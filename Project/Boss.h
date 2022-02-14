#pragma once
#include "Mof.h"
#include "Enemy.h"
#include "EffectManager.h"
#include "EnemyShot.h"

//当たり判定減衰幅
#define		BOSS_RECTDECREASE		10

//索敵範囲円
#define		BOSS_SEARCH				150
#define		BOSS_SEARCH_SHOT		250

class CBoss {
private:
	CTexture					m_BossTexture;
	CSpriteMotionController		m_BossMotion;
	CVector2					m_BossPos;
	CVector2					m_BossMove;
	bool						m_bShow;
	int							m_BossHP;
	int							m_DamageWait;
	bool						m_bReverse;
	CVector2					m_TargetPos;
	bool						m_bClear;
	CRectangle					m_SrcRect;

	//索敵範囲円
	CCircle						m_BossSerchCircle;
	CCircle						m_BossShotCircle;

	//攻撃判定フラグ
	bool						m_Rush;   //近接用
	bool						m_Shot;   //遠距離用

	//モーション定義
	enum tag_BossMotion {
		MOTION_MOVE,
		MOTION_DAMAGE,
		MOTION_SHOT,

		MOTION_COUNT,
	};

	//エフェクト
	CEffectManager*		m_bEffectManager;
	CEffect*			m_pEndEffect;

	//遠距離攻撃
	int					m_ShotWait;
	int					m_ShotWaitSet;
	int					m_ShotCnt;

	bool				m_RushDes;
	int					m_RushCnt;

	//SE
	CSoundBuffer		m_SE_Shot;
	CSoundBuffer		m_SE_Explosion;


public:
	CBoss();
	~CBoss();
	bool Load();
	void Initialize(float px,float py);
	void Update(CEnemyShot* shot,int smax);
	void Render(float wx,float wy);
	void RenderDebug(float wx, float wy);
	void Release();
	bool GetShow() { return m_bShow; }
	bool GetReverse() { return m_bReverse; }
	void ColisionStage(float ox, float oy);
	//当たり判定矩形
	CRectangle GetRect() {
		return CRectangle(m_BossPos.x + BOSS_RECTDECREASE * 2 , m_BossPos.y + BOSS_RECTDECREASE * 2,
			m_BossPos.x + m_SrcRect.GetWidth() * 2 - BOSS_RECTDECREASE, m_BossPos.y  + m_SrcRect.GetHeight() + BOSS_RECTDECREASE * 4);
	}

	//ダメージ関連
	void Damage(int dmg, bool bRev);
	int GetDamageWait() { return m_DamageWait; }

	//エフェクト
	void SetEffectManager(CEffectManager* bmng) { m_bEffectManager = bmng; }

	//攻撃判定円
	CCircle GetBossCircle() {
		return CCircle(m_BossPos.x + BOSS_RECTDECREASE, m_BossPos.y + BOSS_RECTDECREASE, BOSS_SEARCH);
	}
	CCircle GetBossShotCircle() {
		return CCircle(m_BossPos.x + BOSS_RECTDECREASE, m_BossPos.y + BOSS_RECTDECREASE, BOSS_SEARCH_SHOT);
	}

	//攻撃判定
	void OnRush() { m_Rush = true; }
	void OffRush() { m_Rush = false; }
	void OnShot() { m_Shot = true; }
	void OffShot() { m_Shot = false; }

	//遠距離攻撃用
	void SetTargetPos(const CVector2& t) { m_TargetPos = t; }

	//ボス行動パターン
	void BossPattern1(CEnemyShot* shot,int smax);
	void BossPattern2(CEnemyShot* shot, int smax);
	void BossPattern3(CEnemyShot* shot, int smax);
	void BossPattern4(CEnemyShot* shot, int smax);

	void BossShot(CEnemyShot* shot, int smax,int wait,int scnt);

	bool IsClear() { return m_bClear; }
};