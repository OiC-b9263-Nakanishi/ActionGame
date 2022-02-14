#include "Enemy.h"

/**
* �R���X�g���N�^
*/
CEnemy::CEnemy():
m_EneTexture(NULL),
m_EneMotion(),
m_EnePos(0,0),
m_EneMove(0,0),
m_bShow(false),
m_bReverse(false),
m_bBoost(false),
m_SrcRect(),
m_EneHP(0),
m_ShotWait(0),
m_ShotWaitSet(0),
m_TargetPos(0,0),
m_DamageWait(0){
}

/**
* �f�X�g���N�^
*/
CEnemy::~CEnemy() {
//delete[]�ŉ�������悤��Release���f�X�g���N�^�Ŏ��s
	Release();
}

/**
* ������
*/
void CEnemy::Initialize(float px, float py, int type) {
	m_Type = type;
	m_EnePos.x = px;
	m_EnePos.y = py;
	m_EneMove.x = -3.0f;
	m_EneMove.y = 0.0f;
	m_bReverse = true;
	m_bShow = true;
	m_bBoost = false;
	m_EneHP = 30;
	m_DamageWait = 0;
	m_ShotWait = 0;
	m_ShotWaitSet = 80;
	m_TargetPos = CVector2(0, 0);
	m_SE_Shot.Load("SE_Shot.mp3");
	m_SE_Explosion.Load("Explosion_Enemy.mp3");

	switch (m_Type)
	{
		case 0:
			m_EneMove.x = -3.0f;
			break;

		case 1:
			m_EneMove.x = -4.0f;
			break;

		case 2:
			m_EneMove.x = -5.0f;
	}

	//�A�j���[�V�������쐬
	SpriteAnimationCreate anim[] = {

	  {	
		"�ړ�",
		0,0,
		60,64,
		TRUE,{{5,0,0},{5,1,0},{5,2,0},{5,3,0}}
	   },

	   {
		   "�_���[�W",
		   0,210,
		   60,64,
		   FALSE,{{20,0,0}}
	   },

		{
			"�V���b�g",
			0,220,
			60,64,
			FALSE,{{20,0,0}}
		},
	};
	m_EneMotion.Create(anim, MOTION_COUNT);
}

/**
* �X�V
*/
void CEnemy::Update(CEnemyShot* shot,int smax) {
	//��\��
	if (!m_bShow)
	{
		return;
	}
	
	//�d�͂ɂ�艺����
	m_EneMove.y += GRAVITY;
	if (m_EneMove.y >= 20.0f)
	{
		m_EneMove.y = 20.0f;
	}

	//��ʊO��HP��0�ɂ���
	if (m_EnePos.y >= g_pGraphics->GetTargetHeight() - 65 && m_EneHP > 0)
	{
		m_EneHP = 0;
		m_SE_Explosion.Play();
		//�����G�t�F�N�g����
		m_EneEffectManager->Start(m_EnePos.x + m_SrcRect.GetWidth() * 0.5f,
			m_EnePos.y + m_SrcRect.GetHeight() * 0.5f, EFC_EXPLOSIONENEMY);
	}

	//���ۂɍ��W��ω�
	m_EnePos.x += m_EneMove.x;
	m_EnePos.y += m_EneMove.y;

	//�A�j���[�V�����X�V
	m_EneMotion.AddTimer(CUtilities::GetFrameSecond());
	m_SrcRect = m_EneMotion.GetSrcRect();

	//�_���[�W�C���^�[�o������
	if (m_DamageWait > 0)
	{
		m_DamageWait--;
	}

	//�G�^�C�v�ʍs��
	switch (m_Type)
	{
		case 0:
			//�G�i�ߋ����j
			EnemyType0();
			break;

		case 1:
			//�G�i�������j
			EnemyType1(shot,smax);
			break;

		case 2:
			EnemyType2();
			break;
	}

	//�����C��
	if (m_EnePos.x > m_TargetPos.x && m_Type == 1 && m_bBoost)
	{
		m_bReverse = true;
	}
	else if(m_EnePos.x < m_TargetPos.x && m_Type == 1 && m_bBoost)
	{
		m_bReverse = false;
	}
	
}

/**
* �_���[�W����
*/
void CEnemy::Damage(int dmg, bool bRev) {
	m_EneHP -= dmg;
	m_DamageWait = 30;
	if (bRev)
	{
		m_EneMove.x = -5.0f;
		m_bReverse = false;
	}
	else
	{
		m_EneMove.x = 5.0f;
		m_bReverse = true;
	}
	m_EneMotion.ChangeMotion(MOTION_DAMAGE);
	//�_���[�W�G�t�F�N�g����
	m_EneEffectManager->Start(m_EnePos.x + m_SrcRect.GetWidth() * 0.5f,
		m_EnePos.y + m_SrcRect.GetHeight() * 0.5f, EFC_DAMAGE);
}

void CEnemy::EnemyType0()
{	
	if (m_EneMotion.GetMotionNo() == MOTION_MOVE)
	{
		if (m_bBoost)
		{
			if (m_bReverse)
			{
				m_EneMove.x = -7.0f;
			}
			else
			{
				m_EneMove.x = 7.0f;
			}
		}
		else
		{
			if (m_bReverse)
			{
				m_EneMove.x = -3.0f;
			}
			else
			{
				m_EneMove.x = 3.0f;
			}
		}
	}
	if (m_EneMotion.GetMotionNo() == MOTION_DAMAGE)
	{
		//�I���őҋ@�ɖ߂�
		if (m_EneMotion.IsEndMotion())
		{
			m_EneMotion.ChangeMotion(MOTION_MOVE);
			if (m_EneHP <= 0)
			{
				m_bShow = false;
				m_SE_Explosion.Play();
				//�����G�t�F�N�g����
				m_EneEffectManager->Start(m_EnePos.x + m_SrcRect.GetWidth() * 0.5f,
					m_EnePos.y + m_SrcRect.GetHeight() * 0.5f, EFC_EXPLOSIONENEMY);
			}
		}
		else
		{
			if (m_EneMove.x > 0)
			{
				m_EneMove.x -= 0.2f;
				if (m_EneMove.x <= 0)
				{
					m_EneMove.x = 0;
				}
			}
			else if (m_EneMove.x < 0)
			{
				m_EneMove.x += 0.2f;
				if (m_EneMove.x >= 0)
				{
					m_EneMove.x = 0;
				}
			}
		}
	}
}

void CEnemy::EnemyType1(CEnemyShot* shot,int smax)
{
	if (m_EneMotion.GetMotionNo() == MOTION_MOVE)
	{
		if (m_bReverse)
		{
			m_EneMove.x = 0.0f;
		}
		else
		{
			m_EneMove.x = 0.0f;
		}
	}
	if (m_bBoost && m_EneMotion.GetMotionNo() != MOTION_DAMAGE)
	{
		m_EneMotion.ChangeMotion(MOTION_SHOT);
	}
	if (m_EneMotion.GetMotionNo() == MOTION_SHOT)
	{
		if (m_EneMotion.IsEndMotion())
		{
			m_bBoost = false;
			m_EneMotion.ChangeMotion(MOTION_MOVE);
		}
		if (!m_bReverse)
		{
			if (m_ShotWait <= 0)
			{
				CEnemyShot* newShot = CEnemyShot::FindAvailableShot(shot, smax);
				//���g�p�̒e���������ꍇ
				if (newShot)
				{
					m_ShotWait = m_ShotWaitSet;
					//�ڕW�n�_�Ɍ��������߂̕���
					Vector2 direction = m_TargetPos - m_EnePos;
					//�ڕW�n�_�܂ł̋���
					float distance = CVector2Utilities::Length(direction);
					//������0�ȉ��̏ꍇ�͔��˂��Ȃ�
					if (distance > 0)
					{
						//�����𐳋K��
						direction /= distance;
						newShot->Fire(m_EnePos, direction * 5);
						m_SE_Shot.Play();
					}
				}
			}
			else
			{
				m_ShotWait--;
			}
		}
		else
		{
			if (m_ShotWait <= 0)
			{
				CEnemyShot* newShot = CEnemyShot::FindAvailableShot(shot, smax);
				//���g�p�̒e���������ꍇ
				if (newShot)
				{
					m_ShotWait = m_ShotWaitSet;
					//�ڕW�n�_�Ɍ��������߂̕���
					Vector2 direction = m_TargetPos - m_EnePos;
					//�ڕW�n�_�܂ł̋���
					float distance = CVector2Utilities::Length(direction);
					//������0�ȉ��̏ꍇ�͔��˂��Ȃ�
					if (distance > 0)
					{
						//�����𐳋K��
						direction /= distance;
						newShot->Fire(m_EnePos, direction * 5);
						m_SE_Shot.Play();
					}
				}
			}
			else
			{
				m_ShotWait--;
			}
		}
	}
	else if(m_EneMotion.GetMotionNo() == MOTION_DAMAGE)
	{
		//�I���őҋ@�ɖ߂�
		if (m_EneMotion.IsEndMotion())
		{
			m_EneMotion.ChangeMotion(MOTION_MOVE);
			if (m_EneHP <= 0)
			{
				m_bShow = false;
				m_SE_Explosion.Play();
				//�����G�t�F�N�g����
				m_EneEffectManager->Start(m_EnePos.x + m_SrcRect.GetWidth() * 0.5f,
					m_EnePos.y + m_SrcRect.GetHeight() * 0.5f, EFC_EXPLOSIONENEMY);
			}
		}
		else
		{
			if (m_EneMove.x > 0)
			{
				m_EneMove.x -= 0.2f;
				if (m_EneMove.x <= 0)
				{
					m_EneMove.x = 0;
				}
			}
			else if (m_EneMove.x < 0)
			{
				m_EneMove.x += 0.2f;
				if (m_EneMove.x >= 0)
				{
					m_EneMove.x = 0;
				}
			}
		}
	}
	else
	{
		m_EneMotion.ChangeMotion(MOTION_MOVE);
	}
}

void CEnemy::EnemyType2()
{
	if (m_EneMotion.GetMotionNo() == MOTION_DAMAGE)
	{
		//�I���őҋ@�ɖ߂�
		if (m_EneMotion.IsEndMotion())
		{
			m_EneMotion.ChangeMotion(MOTION_MOVE);
			if (m_EneHP <= 0)
			{
				m_SE_Explosion.Play();
				m_bShow = false;
				//�����G�t�F�N�g����
				m_EneEffectManager->Start(m_EnePos.x + m_SrcRect.GetWidth() * 0.5f,
					m_EnePos.y + m_SrcRect.GetHeight() * 0.5f, EFC_EXPLOSIONENEMY);
			}
			if (m_bReverse)
			{
				m_EneMove.x = -5.0f;
			}
			else
			{
				m_EneMove.x = 5.0f;
			}

		}
		else
		{
			if (m_EneMove.x > 0)
			{
				m_EneMove.x -= 0.2f;
				if (m_EneMove.x <= 0)
				{
					m_EneMove.x = 0;
				}
			}
			else if (m_EneMove.x < 0)
			{
				m_EneMove.x += 0.2f;
				if (m_EneMove.x >= 0)
				{
					m_EneMove.x = 0;
				}
			}
		}
	}
}

/**
* �X�e�[�W�Ƃ̓�����
* [in]		ox			X���܂��
* [in]		oy			Y���܂��
*/
void CEnemy::ColisionStage(float ox,float oy) {
	m_EnePos.x += ox;
	m_EnePos.y += oy;

	//�����������܂�A�W�����v���㖄�܂�͈ړ���������
	if (oy < 0 && m_EneMove.y > 0)
	{
		m_EneMove.y = 0;
	}
	else if (oy > 0 && m_EneMove.y < 0)
	{
		m_EneMove.y = 0;
	}
	//���ړ����̍����܂�A�E�ړ����̉E���܂�͈ړ���������
	if (ox < 0 && m_EneMove.x > 0)
	{
		if (m_EneMotion.GetMotionNo() == MOTION_DAMAGE)
		{
			m_EneMove.x = 0;
		}
		else
		{
			m_EneMove.x *= -1;
			m_bReverse = true;
		}
	}
	else if (ox > 0 && m_EneMove.x < 0)
	{
		if (m_EneMotion.GetMotionNo() == MOTION_DAMAGE)
		{
			m_EneMove.x = 0;
		}
		else
		{
			m_EneMove.x *= -1;
			m_bReverse = false;
		}
	}
}

/**
* �G���m�̏Փ�
*/
bool CEnemy::ColisionEnemy(CEnemy& ene) {
	if (!ene.GetShow())
	{
		return false;
	}
	//HP��0�̎����肵�Ȃ�
	if (ene.m_EneHP <= 0)
	{
		return false;
	}
	
	//�G��`���m�Ŕ���
	CRectangle erec = GetRect();
	CRectangle erectarget = ene.GetRect();

	if (erec.CollisionRect(erectarget))
	{
		if (m_bReverse)
		{
			m_bReverse = false;
			m_EneMove.x *= -1;
		}
		else
		{
			m_bReverse = true;
			m_EneMove.x *= -1;
		}
		return true;
	}

	return false;
}

/**
* �`��
*/
void CEnemy::Render(float wx,float wy) {
	//��\��
	if (!m_bShow)
	{
		return;
	}
	//�C���^�[�o��2�t���[�����ɕ`�悵�Ȃ�
	if (m_DamageWait % 4 >= 2)
	{
		return;
	}

	//�`���`
	CRectangle dr = m_SrcRect;

	//���]�t���O��ON�̏ꍇ�A��`�𔽓]
	if (m_bReverse)
	{
		float tmp = dr.Right;
		dr.Right = dr.Left;
		dr.Left = tmp;
	}
	//�e�N�X�`���`��
	m_EneTexture->Render(m_EnePos.x - wx, m_EnePos.y - wy, dr);
}

/**
* �f�o�b�O�`��
*/
void CEnemy::RenderDebug(float wx, float wy) {
	//��\��
	if (!m_bShow)
	{
		return;
	}
	//�����蔻��\��
	CRectangle hr = GetRect();
	CGraphicsUtilities::RenderRect(hr.Left - wx, hr.Top - wy, hr.Right - wx, hr.Bottom - wy, MOF_XRGB(255, 0, 0));

	//���G�͈͉~�\��
	m_EneSearchCircle = GetSearchEnemyCircle();
	m_EneSearchCircle.x -= wx;
	m_EneSearchCircle.y -= wy;
	CGraphicsUtilities::RenderCircle(m_EneSearchCircle, MOF_XRGB(255, 255, 255));
}

/**
* ���
*/
void CEnemy::Release() {
	m_EneMotion.Release();
	m_SE_Shot.Release();
	m_SE_Explosion.Release();
}