#include "Boss.h"

CBoss::CBoss() :
m_BossTexture(),
m_BossMotion(),
m_BossPos(0, 0),
m_BossMove(0, 0),
m_BossHP(0),
m_bShow(false),
m_bReverse(false),
m_bClear(false),
m_SrcRect(),
m_ShotWait(0),
m_ShotWaitSet(0),
m_DamageWait(0),
m_TargetPos(0, 0),
m_ShotCnt(0) {
}

CBoss::~CBoss() {

}

bool CBoss::Load() {
	//�{�X�摜�Ǎ�
	if (!m_BossTexture.Load("Boss.png"))
	{
		return false;
	}

	//�A�j���[�V�����쐬
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
	m_BossMotion.Create(anim, MOTION_COUNT);
	return true;
}

void CBoss::Initialize(float px, float py) {
	m_BossPos.x = px;
	m_BossPos.y = py;
	m_BossMove.x = 0.0f;
	m_BossMove.y = 0.0f;
	m_bShow = true;
	m_bReverse = true;
	m_ShotWait = 0;
	m_ShotWaitSet = 100;
	m_DamageWait = 80;
	m_BossHP = 100;
	m_TargetPos = CVector2(0, 0);
	m_ShotCnt = 0;
	m_RushCnt = 0;
	m_bClear = false;
	m_pEndEffect = NULL;
	m_SE_Shot.Load("SE_Shot.mp3");
	m_SE_Explosion.Load("Explosion_Boss.mp3");
}

void CBoss::Update(CEnemyShot* shot, int smax) {
	//��\��
	if (!GetShow())
	{
		return;
	}

	//�d�͂ɂ���ĉ�����
	m_BossMove.y += GRAVITY;
	if (m_BossMove.y >= 20.0f)
	{
		m_BossMove.y = 20.0f;
	}

	//���ۂɍ��W��ω�
	m_BossPos.x += m_BossMove.x;
	m_BossPos.y += m_BossMove.y;

	//HP���Ȃ��Ȃ����ꍇ�A�����̏I����ҋ@���ďI��(Player.cpp��Update�Q��)
	if (m_BossHP <= 0)
	{
		m_bClear = true;
	}

	//HP�c�ʂŃ{�X�p�^�[���ω�
	if (m_BossHP > 0)
	{
		if (m_BossHP >= 80)
		{
			BossPattern1(shot, smax);
		}
		else if (m_BossHP < 80 && m_BossHP > 50)
		{
			BossPattern2(shot, smax);
		}
		else if (m_BossHP <= 50 && m_BossHP > 30)
		{
			BossPattern3(shot, smax);
		}
		else if (m_BossHP <= 30)
		{
			BossPattern4(shot, smax);
		}
	}

	if (m_BossPos.x > m_TargetPos.x && m_Shot)
	{
		m_bReverse = true;
	}
	else if (m_BossPos.x < m_TargetPos.x && m_Shot)
	{
		m_bReverse = false;
	}

	//�_���[�W������
	if (m_BossMotion.GetMotionNo() == MOTION_DAMAGE)
	{
		//�I���őҋ@��Ԃɖ߂�
		if (m_BossMotion.IsEndMotion())
		{
			m_BossMotion.ChangeMotion(MOTION_MOVE);
			if (m_BossHP <= 0)
			{
				m_bShow = false;
				//�����G�t�F�N�g�𔭐�������
				m_bEffectManager->Start(m_BossPos.x + m_SrcRect.GetWidth() * 0.5f,
					m_BossPos.y + m_SrcRect.GetHeight() * 0.5f, EFC_EXPLOSIONENEMY);
				//����SE
				m_SE_Explosion.Play();
			}
			m_BossMove.x = ((m_bReverse) ? -3.0f : 3.0f);
		}
		else
		{
			if (m_BossMove.x > 0)
			{
				m_BossMove.x -= 0.2f;
				if (m_BossMove.x <= 0)
				{
					m_BossMove.x = 0;
				}
			}
			else if (m_BossMove.x < 0)
			{
				m_BossMove.x += 0.2f;
				if (m_BossMove.x >= 0)
				{
					m_BossMove.x = 0;
				}
			}
		}
	}

	//�A�j���[�V�����X�V
	m_BossMotion.AddTimer(CUtilities::GetFrameSecond());
	m_SrcRect = m_BossMotion.GetSrcRect();

	//�_���[�W�C���^�[�o������
	if (m_DamageWait > 0)
	{
		m_DamageWait--;
	}
}

/**
* �_���[�W����
*/
void CBoss::Damage(int dmg, bool bRev) {
	m_BossHP -= dmg;
	m_DamageWait = 60;
	if (bRev)
	{
		m_BossMove.x = -3.0f;
		m_bReverse = false;
	}
	else
	{
		m_BossMove.x = 3.0f;
		m_bReverse = true;
	}
	m_BossMotion.ChangeMotion(MOTION_DAMAGE);
	//�_���[�W�G�t�F�N�g�𔭐�������
	m_bEffectManager->Start(m_BossPos.x + m_SrcRect.GetWidth() * 0.5f,
		m_BossPos.y + m_SrcRect.GetHeight() * 0.5f, EFC_DAMAGE);
}

/**
* �X�e�[�W�Ƃ̓����蔻��
* [in]		ox		X���܂��
* [in]		oy		Y���܂��
*/
void CBoss::ColisionStage(float ox, float oy) {
	m_BossPos.x += ox;
	m_BossPos.y += oy;

	//�����������܂�A�W�����v���㖄�܂�͈ړ���������
	if (oy < 0 && m_BossMove.y > 0)
	{
		if (m_RushCnt > 70)
		{
			m_RushDes = false;
		}
		m_BossMove.y = 0;
	}
	else if (oy > 0 && m_BossMove.y < 0)
	{
		m_BossMove.y = 0;
	}
	//���ړ��������܂�A�E�ړ����E���܂�͈ړ�������
	if (ox < 0 && m_BossMove.x > 0)
	{
		m_RushDes = false;
		if (m_BossMotion.GetMotionNo() == MOTION_DAMAGE)
		{
			m_BossMove.x = 0;
		}
		else
		{
			m_BossMove.x *= -1;
			m_bReverse = true;
		}
	}
	else if (ox > 0 && m_BossMove.x < 0)
	{
		m_RushDes = false;
		if (m_BossMotion.GetMotionNo() == MOTION_DAMAGE)
		{
			m_BossMove.x = 0;
		}
		else
		{
			m_BossMove.x *= -1;
			m_bReverse = false;
		}
	}
}

/**
* �`��
*/
void CBoss::Render(float wx, float wy) {
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
	CRectangle br = m_SrcRect;

	//�`��ʒu
	float bx = m_BossPos.x - wx;
	float by = m_BossPos.y - wy;

	//���]�t���OON�̏ꍇ�A��`�𔽓]
	if (m_bReverse)
	{
		float tmp = br.Right;
		br.Right = br.Left;
		br.Left = tmp;
	}

	//�e�N�X�`���`��
	m_BossTexture.RenderScale(bx, by, 2, br);
	
	//�{�XHPUI�\��
	if (m_BossHP >= 80)
	{
		CGraphicsUtilities::RenderFillCircle(bx + 50, by, 10, MOF_XRGB(0, 0, 255));
	}
	else if (m_BossHP < 80 && m_BossHP > 50)
	{
		CGraphicsUtilities::RenderFillCircle(bx + 50, by, 10, MOF_XRGB(0, 255, 0));
	}
	else if (m_BossHP <= 50 && m_BossHP > 30)
	{
		CGraphicsUtilities::RenderFillCircle(bx + 50, by, 10, MOF_XRGB(255, 255, 0));
	}
	else
	{
		CGraphicsUtilities::RenderFillCircle(bx + 50, by, 10, MOF_XRGB(255, 0, 0));
	}
}

/**
* �f�o�b�O�`��
*/
void CBoss::RenderDebug(float wx, float wy) {

	//��\��
	if (!m_bShow)
	{
		return;
	}
	//�����蔻��`��
	CRectangle hr = GetRect();
	CGraphicsUtilities::RenderRect(hr.Left - wx, hr.Top - wy, hr.Right - wx, hr.Bottom - wy, MOF_XRGB(255, 0, 0));

	//����~�`��
	m_BossSerchCircle = GetBossCircle();
	m_BossSerchCircle.x -= wx;
	m_BossSerchCircle.y -= wy;
	CGraphicsUtilities::RenderCircle(m_BossSerchCircle, MOF_XRGB(255, 255, 255));

	m_BossShotCircle = GetBossShotCircle();
	m_BossShotCircle.x -= wx;
	m_BossShotCircle.y -= wy;
	CGraphicsUtilities::RenderCircle(m_BossShotCircle, MOF_XRGB(255, 255, 255));
}

/**
* ���
*/
void CBoss::Release() {
	m_BossTexture.Release();
	m_SE_Shot.Release();
	m_SE_Explosion.Release();
}

void CBoss::BossPattern1(CEnemyShot* shot, int smax) {

	if (m_BossMotion.GetMotionNo() == MOTION_MOVE)
	{
		if (m_Rush)
		{
			if (m_bReverse)
			{
				m_BossMove.x = -5.0f;
			}
			else
			{
				m_BossMove.x = 5.0f;
			}
		}
		else
		{
			m_BossMove.x = 0.0f;
		}

	}

	if (m_Shot && m_BossMotion.GetMotionNo() != MOTION_DAMAGE)
	{
		m_BossMotion.ChangeMotion(MOTION_SHOT);
	}
	if (m_BossMotion.GetMotionNo() == MOTION_SHOT)
	{
		//�ړ���~
		m_BossMove.x = 0.0f;
		if (m_BossMotion.IsEndMotion())
		{
			m_Shot = false;
			m_BossMotion.ChangeMotion(MOTION_MOVE);
		}
		if (!m_bReverse)
		{
			BossShot(shot, smax, m_ShotWaitSet, 1);
		}
		else
		{
			BossShot(shot, smax, m_ShotWaitSet, 1);
		}
	}
}

void CBoss::BossPattern2(CEnemyShot* shot, int smax) {

	if (m_RushDes)
	{
		if (m_RushCnt <= 70)
		{
			m_BossMove.x = 0.0f;
			m_BossMove.y = -3.0f;
			m_RushCnt++;
			if (m_RushCnt > 70)
			{
				//�v���C���[�̕����ɓ˂�����ł���悤�ɂ���
				CVector2 direction = m_TargetPos - m_BossPos;
				float distance = CVector2Utilities::Length(direction);
				direction /= distance;

				if (m_bReverse)
				{	
					m_BossMove = direction * 10;
					m_BossMove.x = -10.0f;				
				}
				else
				{
					m_BossMove = direction * 10;
					m_BossMove.x = 10.0f;
				}
			}
		}
		return;
	}
	if (m_BossMotion.GetMotionNo() == MOTION_MOVE)
	{
		if (m_Rush)
		{
			m_RushCnt = 0;
			m_RushDes = true;
		}
		else
		{
			m_BossMove.x = 0.0f;
		}
	}

	if (m_Shot && m_BossMotion.GetMotionNo() != MOTION_DAMAGE)
	{
		m_BossMotion.ChangeMotion(MOTION_SHOT);
	}
	if (m_BossMotion.GetMotionNo() == MOTION_SHOT)
	{
		//�ړ���~
		m_BossMove.x = 0.0f;
		if (m_BossMotion.IsEndMotion())
		{
			m_Shot = false;
			m_BossMotion.ChangeMotion(MOTION_MOVE);
		}
		if (!m_bReverse)
		{
			BossShot(shot, smax, m_ShotCnt % 2 != 0 ? 5 : 40, 1);
		}
		else
		{
			BossShot(shot, smax, m_ShotCnt % 2 != 0 ? 5 : 40, 1);
		}
	}
}

void CBoss::BossPattern3(CEnemyShot* shot, int smax) {

	if (m_BossMotion.GetMotionNo() == MOTION_MOVE)
	{
		if (m_Rush)
		{
			if (m_bReverse)
			{
				m_BossMove.x = -4.0f;
				m_BossMove.y = -5.0f;
			}
			else
			{
				m_BossMove.x = 4.0f;
				m_BossMove.y = -5.0f;
			}
		}
		else
		{
			m_BossMove.x = 0.0f;
		}
	}

	if (m_Shot && m_BossMotion.GetMotionNo() != MOTION_DAMAGE)
	{
		m_BossMotion.ChangeMotion(MOTION_SHOT);
	}
	if (m_BossMotion.GetMotionNo() == MOTION_SHOT)
	{
		//�ړ���~
		m_BossMove.x = 0.0f;
		if (m_BossMotion.IsEndMotion())
		{
			m_Shot = false;
			m_BossMotion.ChangeMotion(MOTION_MOVE);
		}
		if (!m_bReverse)
		{
			BossShot(shot, smax, m_ShotCnt % 3 != 0 ? 5 : 40, 1);
		}
		else
		{
			BossShot(shot, smax, m_ShotCnt % 3 != 0 ? 5 : 40, 1);
		}
	}
}

void CBoss::BossPattern4(CEnemyShot* shot, int smax)
{
	if (m_BossMotion.GetMotionNo() == MOTION_MOVE)
	{
		if (m_Rush)
		{
			if (m_bReverse)
			{
				m_BossMove.x = -5.0f;
				m_BossMove.y = -3.0f;
			}
			else
			{
				m_BossMove.x = 5.0f;
				m_BossMove.y = -3.0f;
			}

		}
		else
		{
			m_BossMove.x = 0.0f;
		}

	}

	if (m_Shot && m_BossMotion.GetMotionNo() != MOTION_DAMAGE)
	{
		m_BossMotion.ChangeMotion(MOTION_SHOT);
	}
	if (m_BossMotion.GetMotionNo() == MOTION_SHOT)
	{
		//�ړ���~
		m_BossMove.x = 0.0f;
		if (m_BossMotion.IsEndMotion())
		{
			m_Shot = false;
			m_BossMotion.ChangeMotion(MOTION_MOVE);
		}
		if (!m_bReverse)
		{
			BossShot(shot, smax, m_ShotCnt % 3 != 0 ? 5 : 75, 3);

		}
		else
		{
			BossShot(shot, smax, m_ShotCnt % 3 != 0 ? 5 : 75, 3);
		}
	}
}

void CBoss::BossShot(CEnemyShot* shot, int smax, int wait, int scnt) {
	if (m_ShotWait > 0)
	{
		m_ShotWait--;
		return;
	}
	m_ShotWaitSet = wait;
	//�ڕW�n�_�Ɍ��������߂̕���
	CVector2 direction = m_TargetPos - m_BossPos;
	//�ڕW�n�_�܂ł̋���
	float distance = CVector2Utilities::Length(direction);
	//������0�ȉ��̏ꍇ�͔��˂��Ȃ�
	if (distance <= 0)
	{
		return;
	}
	m_ShotCnt++;
	m_ShotWait = m_ShotWaitSet;
	//�����𐳋K��
	direction /= distance;
	
	//�e�̉��Z�p�x
	float shotAngle = 20.0f;
	//�v���C���[�����ւ̊p�x
	float dir = atan2(direction.y, direction.x);
	//�������ː�
	int sCnt = scnt;
	//���S�̒e���v���C���[�̒��S�ɂ����悤�ɏ����p�x�𒲐�
	float ad = MOF_ToRadian(-shotAngle * (sCnt * 0.5f - 0.5f));

	for (int cnt = 0; cnt < sCnt; cnt++)
	{
		//���g�p�̒e��T��
		CEnemyShot* newShot = CEnemyShot::FindAvailableShot(shot, smax);
		if (!newShot)
		{
			return;
		}

		direction.x = cos(dir + ad);
		direction.y = sin(dir + ad);
		//���g�p�̒e���������ꍇ
		newShot->Fire(m_BossPos, direction * 5);
		m_SE_Shot.Play();
		ad += MOF_ToRadian(shotAngle);
	}
}