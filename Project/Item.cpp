#include "Item.h"

/**
* �R���X�g���N�^
*/
CItem::CItem():
m_ItemTexture(NULL),
m_ItemMotion(),
m_ItemPos(0.0f,0.0f),
m_ItemMove(0.0f,0.0f),
m_bShow(false),
m_SrcRect(){
}

/**
* �f�X�g���N�^
*/
CItem::~CItem() {
	//delete[]�ŉ�������悤��Release���f�X�g���N�^�Ŏ��s
	Release();
}

/**
* ������
* [in]   px		X���W
* [in]   py		Y���W
* [in]   type   �G�^�C�v
*/
void CItem::Initialize(float px, float py, int type) {
	m_Type = type;
	m_ItemPos.x = px;
	m_ItemPos.y = py;
	m_ItemMove.x = 0.0f;
	m_ItemMove.y = 0.0f;
	m_bShow = true;

	//�A�j���[�V�����쐬
	SpriteAnimationCreate anim = {
		"�A�C�e��",
		0,0,
		32,32,
		TRUE,{{5,0,0}, {5,1,0}, {5,2,0}, {5,3,0}}
	};
	m_ItemMotion.Create(anim);
}

/**
* �X�V
*/
void CItem::Update() {
	//��\��
	if (!m_bShow)
	{
		return;
	}
	//�d�͂ɂ���ĉ�����
	m_ItemMove.y += GRAVITY;
	if (m_ItemMove.y >= 20.0f)
	{
		m_ItemMove.y = 20.0f;
	}

	//���W�ړ�
	m_ItemPos.x += m_ItemMove.x;
	m_ItemPos.y += m_ItemMove.y;

	//�A�j���[�V�����X�V
	m_ItemMotion.AddTimer(CUtilities::GetFrameSecond());
	m_SrcRect = m_ItemMotion.GetSrcRect();
}

/**
* �X�e�[�W�Ƃ̓�����
* [in]		ox			X���܂��
* [in]      oy			Y���܂��
*/
void CItem::ColisionStage(float ox, float oy) {
	m_ItemPos.x += ox;
	m_ItemPos.y += oy;

	//�����������܂�A�W�����v���㖄�܂�͏�����
	if (oy < 0 && m_ItemMove.y > 0)
	{
		m_ItemMove.y = 0;
	}
	else if (oy > 0 && m_ItemMove.y < 0)
	{
		m_ItemMove.y = 0;
	}
	//���ړ��������܂�A�E�ړ����E���܂�
	if (ox < 0 && m_ItemMove.x > 0)
	{
		m_ItemMove.x = 0;
	}
	else if (ox > 0 && m_ItemMove.x < 0)
	{
		m_ItemMove.x = 0;
	}
}

/**
* �`��
* [in]		wx		���[���h�ω�
* [in]		wy		���[���h�ω�
*/
void CItem::Render(float wx, float wy) {
	//��\��
	if (!m_bShow)
	{
		return;
	}
	//�e�N�X�`���`��
	m_ItemTexture->Render(m_ItemPos.x - wx, m_ItemPos.y - wy, m_SrcRect);
}

/**
* �f�o�b�O�`��
* [in]		wx		���[���h�ω�
* [in]		wy		���[���h�ω�
*/
void CItem::RenderDebug(float wx, float wy) {
	//��\��
	if (m_bShow)
	{
		return;
	}

	//�����蔻��\��
	CRectangle hr = GetRect();
	CGraphicsUtilities::RenderRect(hr.Left - wx, hr.Top - wy, hr.Right - wx, hr.Bottom - wy, MOF_XRGB(255, 0, 0));
}

/**
* ���
*/
void CItem::Release() {
	m_ItemMotion.Release();
}