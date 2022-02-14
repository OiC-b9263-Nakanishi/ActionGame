#pragma once
#include "Mof.h"
#include "GameDefine.h"
#include "EffectManager.h"
#include "EnemyShot.h"

//�����蔻�茸����
#define				ENEMY_RECTDECREASE		10

//���G�͈͉~
#define				ENEMY_SEARCH			150

class CEnemy {
private:
	CTexture*					m_EneTexture;
	CSpriteMotionController		m_EneMotion;
	int							m_Type;
	CVector2					m_EnePos;
	CVector2					m_EneMove;
	bool						m_bShow;
	bool						m_bReverse;
	CRectangle					m_SrcRect;
	int							m_EneHP;
	int							m_DamageWait;

	//���G�͈͉~
	CCircle                     m_EneSearchCircle;

	//�G�t�F�N�g
	CEffectManager*				m_EneEffectManager;

	//�G�ʍs���p�t���O
	bool						m_bBoost;

	//�G�e�֘A
	int							m_ShotWait;
	int							m_ShotWaitSet;
	CVector2					m_TargetPos;

	//SE
	CSoundBuffer				m_SE_Shot;
	CSoundBuffer				m_SE_Explosion;

	//���[�V������`
	enum tag_EnemyMOTION {
		MOTION_MOVE,
		MOTION_DAMAGE,
		MOTION_SHOT,

		MOTION_COUNT,
	};

public:
	CEnemy();
	~CEnemy();
	void Initialize(float px, float py, int type);
	bool Load();
	void Update(CEnemyShot* shot,int smax);
	void ColisionStage(float ox, float oy);
	void Render(float wx, float wy);
	void RenderDebug(float wx, float wy);
	void Release();
	void SetTexture(CTexture* pt) { 
		m_EneTexture = pt;
	}
	bool GetShow() { return m_bShow; }
	CRectangle GetRect() {
		return CRectangle(m_EnePos.x + ENEMY_RECTDECREASE, m_EnePos.y + ENEMY_RECTDECREASE,
			m_EnePos.x + m_SrcRect.GetWidth() - ENEMY_RECTDECREASE, m_EnePos.y + m_SrcRect.GetHeight());
	}
	//�_���[�W
	void Damage(int dmg, bool bRev);
	int GetDamageWait() { return m_DamageWait; }

	//�G�t�F�N�g
	void SetEffectManager(CEffectManager* pmng) { m_EneEffectManager = pmng; }

	//����p�~
	CCircle GetSearchEnemyCircle()
	{
		return CCircle(m_EnePos.x + ENEMY_RECTDECREASE, m_EnePos.y + ENEMY_RECTDECREASE, ENEMY_SEARCH);
	}

	void EnemyType0();
	void EnemyType1(CEnemyShot* shot,int smax);
	void EnemyType2();

	void OnBoost() {
		m_bBoost = true;
	}
	void OffBoost() {
		m_bBoost = false;
	}

	//�G�������p
	void SetTargetPos(const CVector2& t) { m_TargetPos = t; }

	//�G���m�̏Փ�
	bool ColisionEnemy(CEnemy& ene);
};