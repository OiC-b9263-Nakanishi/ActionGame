#include "EnemyShot.h"

/**
* �R���X�g���N�^
*/
CEnemyShot::CEnemyShot():
m_ShotTexture(NULL),
m_ShotPos(0.0f,0.0f),
m_ShotSpd(0.0f,0.0f),
m_bShow(false)
{
}

/**
* �f�X�g���N�^
*/
CEnemyShot::~CEnemyShot() {

}

/**
* ������
*/
void CEnemyShot::Initialize() {
	m_ShotPos = Vector2(0, 0);
	m_ShotSpd = Vector2(0, 0);
	m_bShow = false;
}

/**
* ����
*
* ����
* [in]		px		����X���W
* [in]		py		����Y���W
* [in]		sx		�ړ�X���x
* [in]		sy		�ړ�Y���x
*/
void CEnemyShot::Fire(const Vector2& p,const Vector2& s) {
	m_ShotPos = p;
	m_ShotSpd = s;
	m_bShow = true;
}

/**
* �X�V
*/
void CEnemyShot::Update(float wx,float wy) {
	//��\��
	if (!m_bShow)
	{
		return;
	}
	//���x�𗘗p�����ړ�
	m_ShotPos += m_ShotSpd;

	//��ʊO�ŏ���
	if (m_ShotPos.x + m_ShotTexture->GetWidth() < 0 || m_ShotPos.x - wx > g_pGraphics->GetTargetWidth() ||
		m_ShotPos.y + m_ShotTexture->GetHeight() < 0 || m_ShotPos.y - wy > g_pGraphics->GetTargetHeight())
	{
		m_bShow = false;
	}
}

/**
* �`��
*/
void CEnemyShot::Render(float wx,float wy) {
	//��\��
	if (!m_bShow)
	{
		return;
	}
	//�e�N�X�`���\��
	m_ShotTexture->Render(m_ShotPos.x - wx, m_ShotPos.y - wy);
}

/**
* �f�o�b�O�`��
*/
void CEnemyShot::RenderDebug() {
	//��\��
	if (!m_bShow)
	{
		return;
	}
	//�����蔻��̕\��
	CGraphicsUtilities::RenderRect(m_ShotPos.x, m_ShotPos.y, m_ShotPos.x + m_ShotTexture->GetWidth(),
		m_ShotPos.y + m_ShotTexture->GetHeight(), MOF_XRGB(255, 0, 0));
}

/**
* �e�z�񂩂��\���̂��̂���������
*/
CEnemyShot* CEnemyShot::FindAvailableShot(CEnemyShot* shot, int smax) {
	for (int i = 0; i < smax; i++)
	{
		if (!shot[i].GetShow())
		{
			return &shot[i];
		}
	}
	return NULL;
}