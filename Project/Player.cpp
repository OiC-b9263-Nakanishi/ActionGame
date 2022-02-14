#include "Player.h"

/**
* �R���X�g���N�^
*/
CPlayer::CPlayer():
m_Texture(),
m_Motion(),
m_Pos(0.0f,0.0f),
m_bMove(false),
m_Move(0.0f,0.0f),
m_bJump(false),
m_bReverse(false),
m_SrcRect(),
m_HP(100),
m_DamageWait(0),
m_FrameTexture(),
m_HPTexture(),
m_DashWait(0),
m_PlayerEffectManager(),
m_bEnd(false),
m_AttackCnt(0),
m_JumpCnt(0),
m_bJump2(false),
m_pEndEffect()
{
}


/**
* �f�X�g���N�^
*/
CPlayer::~CPlayer()
{

}

/**
* �ǂݍ���
*/
bool CPlayer::Load() {
	//�e�N�X�`���ǂݍ���
	if (!m_Texture.Load("Player.png"))
	{
		return false;
	}
	//HP�e�N�X�`���ǂݍ���
	if (!m_FrameTexture.Load("Frame.png"))
	{
		return false;
	}
	if (!m_HPTexture.Load("HP.png"))
	{
		return false;
	}
	//SE�ǂݍ���
	m_SE_Attack.Load("Player_Attack.mp3");
	m_SE_Damage.Load("Damage.mp3");
	m_SE_Guard.Load("Player_Guard.mp3");
	m_SE_Explosion.Load("Explosion.mp3");
	m_SE_Dash.Load("Player_Dash.mp3");
	m_SE_Heal.Load("ItemHeal.mp3");

	//�A�j���[�V�����쐬
	SpriteAnimationCreate anim[] = {
		{
			"�ҋ@",
			 0,0,
			 60,64,
			 TRUE,{{5,0,0},{5,1,0},{5,2,0},{5,3,0},{5,4,0},{5,5,0},{5,6,0},{5,7,0}}
		},

		{
			"�ړ�",
			 0,70,
			 60,64,
			 TRUE,{{5,0,0},{5,1,0},{5,2,0},{5,3,0},{5,4,0},{5,5,0}}
		},

		{
			"�W�����v�J�n",
			 0,140,
			 60,64,
			 FALSE,{{5,0,0},{5,1,0},{5,2,0},{5,3,0}}
		},

		{
			"�W�����v�I��",
			240,140,
			60,64,
			FALSE,{{2,0,0},{2,1,0}}
		},

		{
			"�_�b�V���J�n",
			180,280,
			60,64,
			FALSE,{{2,0,0},{8,1,0}}
		},

	    {
			"�_�b�V���I��",
			300,280,
			60,64,
			FALSE,{{4,0,0},{4,1,0}}

	    },

		{
			"�U��",
			0,420,
			90,64,
			FALSE,{{2,0,0},{2,1,0},{2,2,0},{2,3,0},{2,4,0},{2,5,0},{2,6,0}}
		},

		{
			"�_���[�W",
			480,0,
			60,64,
			FALSE,{{20,0,0}}

		},

		{
			"�K�[�h",
			110,630,
			60,64,
			FALSE,{{20,0,0}}
		},

		{
			"�U��2",
			0,350,
			90,64,
			FALSE,{{2,0,0},{2,1,0},{2,2,0},{2,3,0},{2,4,0},{2,5,0},{2,6,0}}
		},
	};
	m_Motion.Create(anim, MOTION_COUNT);
	return true;
}

/**
* ������
*/
void CPlayer::Initialize() {
	m_Pos.x = 75;
	m_Pos.y = 608;
	m_bMove = false;
	m_Move.x = 0.0f;
	m_Move.y = 0.0f;
	m_bReverse = false;
	m_bJump = false;
	m_HP = 100;
	m_DamageWait = 0;
	m_bEnd = false;
	m_pEndEffect = NULL;
	m_Motion.ChangeMotion(MOTION_WAIT);
	m_DashWait = 0;
	m_AttackCnt = 0;
	m_bGoal = false;
}

/**
*�X�V
*/
void CPlayer::Update() {

	//HP���Ȃ��Ȃ����ꍇ�A�����̏I����ҋ@���ďI��
	if (m_HP <= 0)
	{
		if (!m_pEndEffect || !m_pEndEffect->GetShow())
		{
			m_bEnd = true;
		}
		return;
	}
	//�ړ��t���O�A�ړ������������ۑ�
	m_bMove = false;

	//2�i�U���̔���
	if (m_Motion.GetMotionNo() == MOTION_ATTACK)
	{
		if (g_pInput->IsMouseKeyPush(MOFMOUSE_LBUTTON))
		{
			m_AttackCnt = 1;
		}
	}

	//�U�����A���n�̏ꍇ�̓���
	if (m_Motion.GetMotionNo() == MOTION_ATTACK || m_Motion.GetMotionNo() == MOTION_ATTACK2 || 
		m_Motion.GetMotionNo() == MOTION_JUMPEND)
	{
		//�I���őҋ@�ɖ߂�
		if (m_Motion.IsEndMotion())
		{
			if (m_AttackCnt == 1)
			{
				m_SE_Attack.Play();
				m_Motion.ChangeMotion(MOTION_ATTACK2);
			}
			else
			{
				m_Motion.ChangeMotion(MOTION_WAIT);
			}
			m_AttackCnt = 0;
		}
	}
	else if (m_Motion.GetMotionNo() == MOTION_DAMAGE)
	{
		//�I���őҋ@�ɖ߂�
		if (m_Motion.IsEndMotion())
		{
			m_Motion.ChangeMotion(MOTION_WAIT);
		}
	}
	//�_�b�V�����̓���
	else if (m_Motion.GetMotionNo() == MOTION_DASHSTART)
	{
		if (m_bReverse)
		{
			//�G�t�F�N�g
			m_pDashEffect = m_PlayerEffectManager->Start(m_Pos.x + 50 + m_SrcRect.GetWidth() * 0.5f,
				m_Pos.y + m_SrcRect.GetHeight() * 0.5f, EFC_DASH_PLAYER);
		}
		else
		{
			//�G�t�F�N�g
			m_pDashEffect = m_PlayerEffectManager->Start(m_Pos.x - 50 + m_SrcRect.GetWidth() * 0.5f,
				m_Pos.y + m_SrcRect.GetHeight() * 0.5f, EFC_DASH_PLAYER);
		}
		
		if (m_Motion.IsEndMotion())
		{
			m_Motion.ChangeMotion(MOTION_DASHEND);
		}
	}
	else if (m_Motion.GetMotionNo() == MOTION_DASHEND)
	{
		if (m_Motion.IsEndMotion())
		{
			m_Motion.ChangeMotion(MOTION_WAIT);
		}
	}
	//�K�[�h��
	else if (m_Motion.GetMotionNo() == MOTION_GUARD)
	{
		//�G�t�F�N�g
		m_pGuardEffect = m_PlayerEffectManager->Start(m_Pos.x + m_SrcRect.GetWidth() * 0.5f,
			m_Pos.y + m_SrcRect.GetHeight() * 0.5f, EFC_GUARD_PLAYER);
		if (!g_pInput->IsKeyHold(MOFKEY_F))
		{
			m_Motion.ChangeMotion(MOTION_WAIT);
		}
	}
	else
	{
		//�L�[���͂ɂ�铮��
		UpdateKey();
	}

	//�ړ��X�V
	UpdateMove();

	//���ۂɍ��W���ړ�
	m_Pos.x += m_Move.x;
	m_Pos.y += m_Move.y;

	//��ʊO�ŗ����Ƃ���HP��0�ɂ���
	if (m_Pos.y  >= g_pGraphics->GetTargetHeight() - PLAYER_FALL && m_HP > 0)
	{
		m_HP = 0;
		m_SE_Explosion.Play();
		//�����G�t�F�N�g����
		m_pEndEffect = m_PlayerEffectManager->Start(m_Pos.x + m_SrcRect.GetWidth() * 0.5f,
			m_Pos.y + m_SrcRect.GetHeight() * 0.5f, EFC_EXPLOSIONPLAYER);
	}

	//�A�j���[�V�����X�V
	m_Motion.AddTimer(CUtilities::GetFrameSecond());
	m_SrcRect = m_Motion.GetSrcRect();

	//�_���[�W�C���^�[�o������
	if (m_DamageWait > 0)
	{
		m_DamageWait--;
	}
	//�_�b�V���C���^�[�o������
	if (m_DashWait > 0)
	{
		m_DashWait--;
	}

	//�n�ʂɐڐG����ƃW�����v�I��
	if (m_Pos.y + m_SrcRect.GetHeight() >= g_pGraphics->GetTargetHeight())
	{
		m_Pos.y = g_pGraphics->GetTargetHeight() - m_SrcRect.GetHeight();
		m_Move.y = 0;

		if (m_bJump)
		{
			m_bJump = false;
			m_Motion.ChangeMotion(MOTION_JUMPEND);
		}
		if (m_bJump2)
		{
			m_bJump2 = false;
			m_Motion.ChangeMotion(MOTION_JUMPEND);
		}
	}
}

/**
* �L�[����
*/
void CPlayer::UpdateKey() {
	//�L�[�{�[�h�ړ�
	//���͂ł͍��W�ł͂Ȃ����x��ω�������
	if (g_pInput->IsKeyHold(MOFKEY_A))
	{
		m_Move.x -= PLAYER_SPEED;
		m_bReverse = true;
		m_bMove = true;

		if (m_Move.x < -PLAYER_MAXSPEED)
		{
			m_Move.x = -PLAYER_MAXSPEED;
		}
		if (m_Motion.GetMotionNo() == MOTION_WAIT)
		{
			m_Motion.ChangeMotion(MOTION_MOVE);
		}
	}
	else if (g_pInput->IsKeyHold(MOFKEY_D))
	{
		m_Move.x += PLAYER_SPEED;
		m_bReverse = false;
		m_bMove = true;

		if (m_Move.x > PLAYER_MAXSPEED)
		{
			m_Move.x = PLAYER_MAXSPEED;
		}
		if (m_Motion.GetMotionNo() == MOTION_WAIT)
		{
			m_Motion.ChangeMotion(MOTION_MOVE);
		}
	}

	//��L�[�ŃW�����v
	if (g_pInput->IsKeyPush(MOFKEY_W) && !m_bJump && !m_bJump2)
	{
		m_JumpCnt = 1;
		m_bJump = true;
		m_Move.y = PLAYER_JUMP;
		m_Motion.ChangeMotion(MOTION_JUMPSTART);
	}
	else if (g_pInput->IsKeyPush(MOFKEY_W) && m_bJump && !m_bJump2)
	{
		if (m_JumpCnt == 1)
		{
			m_Move.y = PLAYER_JUMP;
			m_Motion.ChangeMotion(MOTION_JUMPSTART);
		}
		m_JumpCnt = 0;
	}

	//�}�E�X���N���b�N�L�[�ōU��
	if (g_pInput->IsMouseKeyPush(MOFMOUSE_LBUTTON))
	{
		m_SE_Attack.Play();
		m_Motion.ChangeMotion(MOTION_ATTACK);
	}
	else
	{
		m_AttackCnt = 0;
	}


	//�}�E�X�E�N���b�N�Ń_�b�V��
	if (g_pInput->IsMouseKeyPush(MOFMOUSE_RBUTTON) && !m_bJump)
	{
		m_bMove = true;
		if (m_DashWait > 0)
		{
			return;
		}
		if (!m_bReverse)
		{
			m_Move.x = PLAYER_DASHSPEED;
			m_DamageWait = 40;
			m_DashWait = 70;
		}
		else
		{
			m_Move.x = -PLAYER_DASHSPEED;
			m_DamageWait = 40;
			m_DashWait = 70;
		}
		m_SE_Dash.Play();
		m_Motion.ChangeMotion(MOTION_DASHSTART);
	}
	//G�L�[�ŃK�[�h
	if (g_pInput->IsKeyHold(MOFKEY_F) && !m_bJump)
	{
		m_Motion.ChangeMotion(MOTION_GUARD);
	}
	
}

/**
* �ړ��X�V
*/
void CPlayer::UpdateMove() {

	//�t���[���ł̈ړ����͂�������Ό������������s����
	if (!m_bMove)
	{
		if (m_Move.x > 0)
		{
			m_Move.x -= PLAYER_SPEED;

			if (m_Move.x <= 0)
			{
				m_Move.x = 0;
			}
		}
		else if (m_Move.x < 0)
		{
			m_Move.x += PLAYER_SPEED;

			if (m_Move.x >= 0)
			{
				m_Move.x = 0;
			}
		}
		else if (m_Motion.GetMotionNo() == MOTION_MOVE)
		{
			m_Motion.ChangeMotion(MOTION_WAIT);
		}
	}

	//�d�͍�p
	m_Move.y += GRAVITY;
	if (m_Move.y >= 20.0f)
	{
		m_Move.y = 20.0f;
	}
}

/**
* �X�e�[�W�Ƃ̓����蔻��
* [in]		ox			[X���܂��]
* [in]		oy			[Y���܂��]
*/
void CPlayer::ColisionStage(float ox, float oy) {
	m_Pos.x += ox;
	m_Pos.y += oy;

	//�������̉����܂�A�W�����v���̏㖄�܂�̏ꍇ�͈ړ���������
	if (oy < 0 && m_Move.y > 0)
	{
		m_Move.y = 0;
		if (m_bJump)
		{
			m_Motion.ChangeMotion(MOTION_JUMPEND);
			m_bJump = false;
		}
		if (m_bJump2)
		{
			m_Motion.ChangeMotion(MOTION_JUMPEND);
			m_bJump2 = false;
		}
	}
	else if (oy > 0 && m_Move.y < 0)
	{
		m_Move.y = 0;
	}

	//���ړ����̍����܂�A�E�ړ����̉E���܂�̏ꍇ�͈ړ���������
	if (ox < 0 && m_Move.x > 0)
	{
		m_Move.x = 0;
	}
	else if (ox > 0 && m_Move.x < 0)
	{
		m_Move.x = 0;
	}
}

/**
* �G�Ƃ̓����蔻��
* [in]     ene		������s���G
*/
bool CPlayer::ColisionEnemy(CEnemy& ene) {
	if (!ene.GetShow())
	{
		return false;
	}
	//HP���Ȃ��Ȃ�Ɠ����蔻�肵�Ȃ�
	if (m_HP <= 0)
	{
		return false;
	}
	//�_���[�W���͓����蔻����s��Ȃ�
	if (m_DamageWait > 0 || ene.GetDamageWait() > 0)
	{
		return true;
	}

	//�G��`�ƃv���C���[��`�Ń_���[�W
	CRectangle prec = GetRect();
	CRectangle erec = ene.GetRect();

	if (prec.CollisionRect(erec) && m_Motion.GetMotionNo() != MOTION_GUARD)
	{
		m_HP -= 5;
		m_DamageWait = 60;
		if (prec.Left < erec.Left)
		{
			m_Move.x -= 5.0f;
			m_bReverse = false;
		}
		else
		{
			m_Move.x += 5.0f;
			m_bReverse = true;
		}
		m_Motion.ChangeMotion(MOTION_DAMAGE);

		if (m_HP <= 0)
		{
			m_SE_Explosion.Play();
			//�����G�t�F�N�g����
			m_pEndEffect = m_PlayerEffectManager->Start(
				m_Pos.x + m_SrcRect.GetWidth() * 0.5f,
				m_Pos.y + m_SrcRect.GetHeight() * 0.5f, EFC_EXPLOSIONPLAYER);
		}
		else
		{
			m_SE_Damage.Play();
			//�_���[�W�G�t�F�N�g����
			m_PlayerEffectManager->Start(m_Pos.x + m_SrcRect.GetWidth() * 0.5f,
				m_Pos.y + m_SrcRect.GetHeight()*0.5f, EFC_DAMAGE);
		}
		
		return true;
	}
	else if (prec.CollisionRect(erec) && m_Motion.GetMotionNo() == MOTION_GUARD)
	{
		prec = GetGuardRect();
		m_DamageWait = 30;
		m_HP -= 5 * DAMAGE_REDUCE;

		if (m_HP <= 0)
		{
			m_SE_Explosion.Play();
			//�����G�t�F�N�g����
			m_pEndEffect = m_PlayerEffectManager->Start(
				m_Pos.x + m_SrcRect.GetWidth() * 0.5f,
				m_Pos.y + m_SrcRect.GetHeight() * 0.5f, EFC_EXPLOSIONPLAYER);
		}
		else
		{
			m_SE_Guard.Play();
			//�_���[�W�G�t�F�N�g����
			m_PlayerEffectManager->Start(m_Pos.x + m_SrcRect.GetWidth() * 0.5f,
				m_Pos.y + m_SrcRect.GetHeight() * 0.5f, EFC_DAMAGE);
		}
		return true;
	}

	//�G�̍��G�͈͉~�Ƃ̔���
	CCircle enecir = ene.GetSearchEnemyCircle();
	CCircle plcir = GetSearchPlayerCircle();
	if (plcir.CollisionCircle(enecir))
	{
		ene.OnBoost();
	}
	else
	{
		ene.OffBoost();
	}

	//�U�����̏ꍇ�U���Ƃ̓����蔻������s
	if (m_Motion.GetMotionNo() != MOTION_ATTACK && m_Motion.GetMotionNo() != MOTION_ATTACK2)
	{
		return false;
	}

	//�G��`�ƃv���C���[��`�œG�Ƀ_���[�W
	prec = GetAttackRect();
	if (prec.CollisionRect(erec))
	{
		ene.Damage(10, m_bReverse);
		return true;
	}
	return false;
}

/**
* �{�X�Ƃ̓����蔻��
* [in]	boss ���肷��{�X
*/
bool CPlayer::ColisionBoss(CBoss& boss) {
	if (!boss.GetShow())
	{
		return true;
	}
	if (m_HP <= 0)
	{
		return false;
	}
	//�_���[�W���̂��ߓ����蔻����s��Ȃ�
	if (m_DamageWait > 0 || boss.GetDamageWait() > 0)
	{
		return false;
	}

	//�{�X�̋�`�Ǝ����̋�`�Ń_���[�W
	CRectangle prec = GetRect();
	CRectangle brec = boss.GetRect();

	if (prec.CollisionRect(brec) && m_Motion.GetMotionNo() != MOTION_GUARD)
	{
		m_HP -= 10;
		m_DamageWait = 60;
		if (m_HP <= 0)
		{
			m_SE_Explosion.Play();
			//�����G�t�F�N�g����
			m_pEndEffect = m_PlayerEffectManager->Start(
				m_Pos.x + m_SrcRect.GetWidth() * 0.5f,
				m_Pos.y + m_SrcRect.GetHeight() * 0.5f, EFC_EXPLOSIONPLAYER);
		}
		else
		{
			m_SE_Damage.Play();
			//�_���[�W�G�t�F�N�g����
			m_PlayerEffectManager->Start(m_Pos.x + m_SrcRect.GetWidth() * 0.5f,
				m_Pos.y + m_SrcRect.GetHeight() * 0.5f, EFC_DAMAGE);
		}
		if (prec.Left < brec.Left)
		{
			m_Move.x = -5.0f;
			m_bReverse = false;
		}
		else
		{
			m_Move.x = 5.0f;
			m_bReverse = true;
		}
		
		m_Motion.ChangeMotion(MOTION_DAMAGE);
		return true;
	}
	else if (prec.CollisionRect(brec) && m_Motion.GetMotionNo() == MOTION_GUARD)
	{
		m_HP -= 10 * DAMAGE_REDUCE;
		m_DamageWait = 30;

		if (m_HP <= 0)
		{
			m_SE_Explosion.Play();
			//�����G�t�F�N�g����
			m_pEndEffect = m_PlayerEffectManager->Start(
				m_Pos.x + m_SrcRect.GetWidth() * 0.5f,
				m_Pos.y + m_SrcRect.GetHeight() * 0.5f, EFC_EXPLOSIONPLAYER);
		}
		else
		{
			m_SE_Guard.Play();
			//�_���[�W�G�t�F�N�g����
			m_PlayerEffectManager->Start(m_Pos.x + m_SrcRect.GetWidth() * 0.5f,
				m_Pos.y + m_SrcRect.GetHeight() * 0.5f, EFC_DAMAGE);
		}
		return true;
	}

	//�{�X�̔���~�ƐڐG����
	CCircle bscir(boss.GetBossCircle());
	CCircle bsshotcir(boss.GetBossShotCircle());
	CCircle plcir = GetSearchPlayerCircle();

	if (plcir.CollisionCircle(bscir) && plcir.CollisionCircle(bsshotcir))
	{
		boss.OnRush();
		boss.OffShot();
	}
	else if (plcir.CollisionCircle(bsshotcir))
	{
		boss.OnShot();
	}
	else
	{
		boss.OffRush();
		boss.OffShot();
	}

	//�U�����̏ꍇ�̂ݍU���Ƃ̓����蔻������s
	if (m_Motion.GetMotionNo() != MOTION_ATTACK && m_Motion.GetMotionNo() != MOTION_ATTACK2)
	{
		return false;
	}

	//�{�X��`�ƃv���C���[��`�œG�Ƀ_���[�W
	prec = GetAttackRect();
	if (prec.CollisionRect(brec))
	{
		boss.Damage(10, m_bReverse);
		return true;
	}
	return false;
}

/**
* �����蔻��(�A�C�e��)
* �����̃A�C�e���ɑ΂��ċ�`���m�̓����蔻������s����
* �������Ă����ꍇ�̓v���C���[�̏�Ԃ�ύX
* 
* [in]		itm		������s���A�C�e��
*/
bool CPlayer::ColisionItem(CItem& itm) {
	if (!itm.GetShow())
	{
		return false;
	}

	//�A�C�e����`�ƃv���C���[��`�œ����蔻��
	CRectangle prec = GetRect();
	CRectangle irec = itm.GetRect();
	if (prec.CollisionRect(irec))
	{
		itm.SetShow(false);
		switch (itm.GetType())
		{
			case ITEM_RECOVER:
				m_SE_Heal.Play();
				m_HP += 30;
				if (m_HP >= 100)
				{
					m_HP = 100;
				}
				break;

			case ITEM_GOAL:
				m_bGoal = true;
				break;
		}
		return true;
	}
	return false;
}

/**
* �G�e�Ƃ̓����蔻��
*
* ����
* [in]		shot		������s���G�e
*/
void CPlayer::ColisionEnemyShot(CEnemyShot& shot) {
	CRectangle pr = GetRect();
	CRectangle sr = shot.GetRect();
	if (!shot.GetShow())
	{
		return;
	}
	//�_���[�W�C���^�[�o��
	if (m_DamageWait > 0)
	{
		return;
	}

	if (sr.CollisionRect(pr) && m_Motion.GetMotionNo() != MOTION_GUARD)
	{
		m_HP -= 10;
		shot.SetShow(false);
		m_DamageWait = 60;
		

		if (pr.Left < sr.Left)
		{
			m_Move.x -= 5;
		}
		else
		{
			m_Move.x = 5;
		}
		
		m_Motion.ChangeMotion(MOTION_DAMAGE);

		if (m_HP <= 0)
		{
			m_SE_Explosion.Play();
			//�����G�t�F�N�g����
			m_pEndEffect = m_PlayerEffectManager->Start(
				m_Pos.x + m_SrcRect.GetWidth() * 0.5f,
				m_Pos.y + m_SrcRect.GetHeight() * 0.5f, EFC_EXPLOSIONPLAYER);
		}
		else
		{
			m_SE_Damage.Play();
			//�_���[�W�G�t�F�N�g����
			m_PlayerEffectManager->Start(m_Pos.x + m_SrcRect.GetWidth() * 0.5f,
				m_Pos.y + m_SrcRect.GetHeight() * 0.5f, EFC_DAMAGE);
		}
	}
	else if (sr.CollisionRect(pr) && m_Motion.GetMotionNo() == MOTION_GUARD)
	{
		m_HP -= 10 * DAMAGE_REDUCE;
		shot.SetShow(false);
		m_DamageWait = 30;
		

		if (m_HP <= 0)
		{
			m_SE_Explosion.Play();
			//�����G�t�F�N�g����
			m_pEndEffect = m_PlayerEffectManager->Start(
				m_Pos.x + m_SrcRect.GetWidth() * 0.5f,
				m_Pos.y + m_SrcRect.GetHeight() * 0.5f, EFC_EXPLOSIONPLAYER);
		}
		else
		{
			m_SE_Guard.Play();
			//�_���[�W�G�t�F�N�g����
			m_PlayerEffectManager->Start(m_Pos.x + m_SrcRect.GetWidth() * 0.5f,
				m_Pos.y + m_SrcRect.GetHeight() * 0.5f, EFC_DAMAGE);
		}
	}
}

/**
* �{�X�e�Ƃ̓����蔻��
*/
void CPlayer::ColisionBossShot(CEnemyShot& shot) {
	
	//�v���C���[��`
	CRectangle pr = GetRect();
	//�{�X�e��`
	CRectangle bsr = shot.GetRect();

	if (!shot.GetShow())
	{
		return;
	}
	//�_���[�W�C���^�[�o��
	if (m_DamageWait > 0)
	{
		return;
	}
	if (pr.CollisionRect(bsr) && m_Motion.GetMotionNo() != MOTION_GUARD)
	{
		m_HP -= 10;
		shot.SetShow(false);
		m_DamageWait = 60;
		if (m_HP <= 0)
		{
			m_SE_Explosion.Play();
			//�����G�t�F�N�g����
			m_pEndEffect = m_PlayerEffectManager->Start(
				m_Pos.x + m_SrcRect.GetWidth() * 0.5f,
				m_Pos.y + m_SrcRect.GetHeight() * 0.5f, EFC_EXPLOSIONPLAYER);
		}
		else
		{
			m_SE_Damage.Play();
			//�_���[�W�G�t�F�N�g����
			m_PlayerEffectManager->Start(m_Pos.x + m_SrcRect.GetWidth() * 0.5f,
				m_Pos.y + m_SrcRect.GetHeight() * 0.5f, EFC_DAMAGE);
		}
		if (pr.Left < bsr.Left)
		{
			m_Move.x -= 5;
		}
		else
		{
			m_Move.x += 5;
		}
		m_Motion.ChangeMotion(MOTION_DAMAGE);
	}
	else if (pr.CollisionRect(bsr) && m_Motion.GetMotionNo() == MOTION_GUARD)
	{
		m_HP -= 10 * DAMAGE_REDUCE;
		m_DamageWait = 30;
		shot.SetShow(false);

		if (m_HP <= 0)
		{
			m_SE_Explosion.Play();
			//�����G�t�F�N�g����
			m_pEndEffect = m_PlayerEffectManager->Start(
				m_Pos.x + m_SrcRect.GetWidth() * 0.5f,
				m_Pos.y + m_SrcRect.GetHeight() * 0.5f, EFC_EXPLOSIONPLAYER);
		}
		else
		{
			m_SE_Guard.Play();
			//�_���[�W�G�t�F�N�g����
			m_PlayerEffectManager->Start(m_Pos.x + m_SrcRect.GetWidth() * 0.5f,
				m_Pos.y + m_SrcRect.GetHeight() * 0.5f, EFC_DAMAGE);
		}
	}
}

/**
* �`��
*/
void CPlayer::Render(float wx, float wy) {

	//�C���^�[�o��2�t���[�����Ƃɕ`�悵�Ȃ�
	if (m_DamageWait % 4 >= 2)
	{
		return;
	}
	//HP��0�̏ꍇ�`�悵�Ȃ�
	if (m_HP <= 0)
	{
		return;
	}

	//�`���`
	CRectangle dr = m_SrcRect;

	//�`��ʒu
	float px = m_Pos.x - wx;
	float py = m_Pos.y - wy;

	//���]�t���O��true�̏ꍇ�`���`�𔽓]
	if (m_bReverse)
	{
		float tmp = dr.Right;
		dr.Right = dr.Left;
		dr.Left = tmp;

		if (m_Motion.GetMotionNo() == MOTION_ATTACK || m_Motion.GetMotionNo() == MOTION_ATTACK2)
		{
			px -= PLAYER_ATTACKWIDTH;
		}
	}
	
	//�e�N�X�`���`��
	m_Texture.Render(px, py, dr);
}

/**
* �X�e�[�^�X�`��
*/
void CPlayer::RenderStatus() {
	//HP�ɉ����ċ�`����ω�
	CRectangle rec(0, 0, 532 * (m_HP*0.01f), 64);
	m_HPTexture.Render(248, 20, rec);

	//�t���[�����㕔�ɕ`��
	m_FrameTexture.Render(0, 0);
}

/**
* �f�o�b�O�`��
*/
void CPlayer::RenderDebug(float wx, float wy) {
	//�ʒu�̕`��
	CGraphicsUtilities::RenderString(10, 70, "�v���C���[�̈ʒu X : %.0f,Y:%.0f", m_Pos.x, m_Pos.y);
	//�����蔻��
	CRectangle hr = GetRect();
	CGraphicsUtilities::RenderRect(hr.Left - wx, hr.Top - wy, hr.Right - wx, hr.Bottom - wy, MOF_XRGB(0, 255, 0));
	//�U���̓����蔻��\��
	if (IsAttack())
	{
		CRectangle hr = GetAttackRect();
		CGraphicsUtilities::RenderRect(hr.Left - wx, hr.Top - wy, hr.Right - wx, hr.Bottom - wy, MOF_XRGB(255, 0, 0));
	}
	//�K�[�h�̓����蔻��\��
	if (IsGuard())
	{
		CRectangle hr = GetGuardRect();
		CGraphicsUtilities::RenderRect(hr.Left -wx, hr.Top - wy, hr.Right - wx, hr.Bottom - wy, MOF_XRGB(255, 0, 0));
	}

	CCircle pc = GetSearchPlayerCircle();
	pc.x -= wx;
	pc.y -= wy;

	CGraphicsUtilities::RenderCircle(pc, MOF_XRGB(0, 0, 255));
}

/**
* ���
*/
void CPlayer::Release() {
	m_Texture.Release();
	m_Motion.Release();
	m_FrameTexture.Release();
	m_HPTexture.Release();
	m_SE_Attack.Release();
	m_SE_Damage.Release();
	m_SE_Guard.Release();
	m_SE_Explosion.Release();
	m_SE_Dash.Release();
	m_SE_Heal.Release();
}