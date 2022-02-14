#pragma once
#include "Mof.h"
#include "GameDefine.h"
#include "Enemy.h"
#include "Boss.h"
#include "EffectManager.h"
#include "Item.h"

//�ړ����x
#define			PLAYER_SPEED			0.3f

//�ړ��ő呬�x
#define			PLAYER_MAXSPEED			5.0f

//�ړ��ő呬�x
#define			PLAYER_DASHSPEED		12.0f

//�W�����v����
#define			PLAYER_JUMP				-10.0f

//�����␳
#define			PLAYER_FALL				65

//�U����
#define			PLAYER_ATTACKWIDTH		 20

//�����蔻�茸����
#define			PLAYER_RECTDECREASE		 12

//���G�͈͗p�~
#define			PLAYER_CIRCLE			100

//�K�[�h��
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

	//HP�֘A�摜
	CTexture					m_FrameTexture;
	CTexture					m_HPTexture;

	//�G�t�F�N�g
	CEffectManager*				m_PlayerEffectManager;

	//�Q�[���I�[�o�[
	CEffect*					m_pEndEffect;
	bool						m_bEnd;

	//�_�b�V���N�[���^�C��
	int							m_DashWait;

	//�U���J�E���g
	int							m_AttackCnt;

	//�W�����v�J�E���g			
	int							m_JumpCnt;

	//�_�b�V���G�t�F�N�g
	CEffect*					m_pDashEffect;

	//�K�[�h�G�t�F�N�g
	CEffect*					m_pGuardEffect;

	//�S�[���t���O(�A�C�e���p)
	bool						m_bGoal;

	//�v���C���[SE
	CSoundBuffer				m_SE_Attack;
	CSoundBuffer				m_SE_Damage;
	CSoundBuffer				m_SE_Guard;
	CSoundBuffer				m_SE_Explosion;
	CSoundBuffer				m_SE_Dash;
	CSoundBuffer				m_SE_Heal;

	//���[�V������ޒ�`
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
		//���]��
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

	//�X�e�[�W�Ƃ̓�����
	void ColisionStage(float ox, float oy);

	//�G�Ƃ̏Փ�
	bool ColisionEnemy(CEnemy& ene);

	//HP�\��
	void RenderStatus();

	//�G�t�F�N�g
	void SetEffectManager(CEffectManager* pmng) { m_PlayerEffectManager = pmng; }

	//�Q�[���I�[�o�[
	bool IsEnd() { return m_bEnd; }

	//�A�C�e��
	bool ColisionItem(CItem& itm);

	//�G�Ƃ̍��G�͈͔���p�~
	CCircle GetSearchPlayerCircle()
	{
		return CCircle(m_Pos.x + PLAYER_RECTDECREASE, m_Pos.y + PLAYER_RECTDECREASE, PLAYER_CIRCLE);
	}

	const CVector2 GetPosition() { return m_Pos; }

	//�G�e�Ƃ̓����蔻��
	void ColisionEnemyShot(CEnemyShot& shot);

	//�{�X�Ƃ̓����蔻��
	bool ColisionBoss(CBoss& boss);

	//�{�X�e�Ƃ̓����蔻��
	void ColisionBossShot(CEnemyShot& shot);

	//�S�[���A�C�e���擾
	bool IsGoal(void) { return m_bGoal; }
};