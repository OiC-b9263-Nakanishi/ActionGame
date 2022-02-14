#pragma once
#include "Mof.h"
#include "Enemy.h"
#include "EffectManager.h"
#include "EnemyShot.h"

//�����蔻�茸����
#define		BOSS_RECTDECREASE		10

//���G�͈͉~
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

	//���G�͈͉~
	CCircle						m_BossSerchCircle;
	CCircle						m_BossShotCircle;

	//�U������t���O
	bool						m_Rush;   //�ߐڗp
	bool						m_Shot;   //�������p

	//���[�V������`
	enum tag_BossMotion {
		MOTION_MOVE,
		MOTION_DAMAGE,
		MOTION_SHOT,

		MOTION_COUNT,
	};

	//�G�t�F�N�g
	CEffectManager*		m_bEffectManager;
	CEffect*			m_pEndEffect;

	//�������U��
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
	//�����蔻���`
	CRectangle GetRect() {
		return CRectangle(m_BossPos.x + BOSS_RECTDECREASE * 2 , m_BossPos.y + BOSS_RECTDECREASE * 2,
			m_BossPos.x + m_SrcRect.GetWidth() * 2 - BOSS_RECTDECREASE, m_BossPos.y  + m_SrcRect.GetHeight() + BOSS_RECTDECREASE * 4);
	}

	//�_���[�W�֘A
	void Damage(int dmg, bool bRev);
	int GetDamageWait() { return m_DamageWait; }

	//�G�t�F�N�g
	void SetEffectManager(CEffectManager* bmng) { m_bEffectManager = bmng; }

	//�U������~
	CCircle GetBossCircle() {
		return CCircle(m_BossPos.x + BOSS_RECTDECREASE, m_BossPos.y + BOSS_RECTDECREASE, BOSS_SEARCH);
	}
	CCircle GetBossShotCircle() {
		return CCircle(m_BossPos.x + BOSS_RECTDECREASE, m_BossPos.y + BOSS_RECTDECREASE, BOSS_SEARCH_SHOT);
	}

	//�U������
	void OnRush() { m_Rush = true; }
	void OffRush() { m_Rush = false; }
	void OnShot() { m_Shot = true; }
	void OffShot() { m_Shot = false; }

	//�������U���p
	void SetTargetPos(const CVector2& t) { m_TargetPos = t; }

	//�{�X�s���p�^�[��
	void BossPattern1(CEnemyShot* shot,int smax);
	void BossPattern2(CEnemyShot* shot, int smax);
	void BossPattern3(CEnemyShot* shot, int smax);
	void BossPattern4(CEnemyShot* shot, int smax);

	void BossShot(CEnemyShot* shot, int smax,int wait,int scnt);

	bool IsClear() { return m_bClear; }
};