#include "Effect.h"

/**
* �R���X�g���N�^
*/
CEffect::CEffect():
m_EfcTexture(NULL),
m_EfcMotion(),
m_EfcPos(0.0f,0.0f),
m_bShow(false),
m_SrcRect(){
}

/**
* �f�X�g���N�^
*/
CEffect::~CEffect() {

}

/**
* ������
*/
void CEffect::Initialize(int type) {
	m_EfcPos.x = 0.0f;
	m_EfcPos.y = 0.0f;
	m_bShow = false;

	switch (type)
	{
		case EFC_DAMAGE:
		{
			//�A�j���[�V�����쐬(�_���[�W)
			SpriteAnimationCreate anim[] = {
				{
					"�G�t�F�N�g",
					0,0,
					128,128,
					FALSE,{{2,0,0},{2,1,0},{2,2,0},{2,3,0}}
				},
			};
			m_EfcMotion.Create(anim, 1);
			break;
		}
		case EFC_EXPLOSIONENEMY:
		{
			//�A�j���[�V�����쐬(�G���j)
			SpriteAnimationCreate anim[] = {
				{
					"�G�t�F�N�g",
					0,0,
					120,120,
					FALSE,{{3,0,0},{3,1,0},{3,2,0},{3,3,0},{3,4,0},
						   {3,5,0},{3,6,0},{3,7,0},{3,8,0},{3,9,0},
						   {3,10,0},{3,11,0},{3,12,0},{3,13,0},{3,14,0},{3,15,0}}
				},
			};
			m_EfcMotion.Create(anim, 1);
			break;
		}
		case EFC_EXPLOSIONPLAYER:
		{
			//�A�j���[�V�����쐬(�v���C���[���j)
			SpriteAnimationCreate anim[] = {
				{
					"�G�t�F�N�g",
					0,0,
					180,180,
					FALSE,{{3,0,0},{3,1,0},{3,2,0},{3,3,0},{3,4,0},
						   {3,5,0},{3,6,0},{3,7,0},{3,8,0},{3,9,0},{3,10,0},
						   {3,0,1},{3,1,1},{3,2,1},{3,3,1},{3,4,1},
						   {3,5,1},{3,6,1},{3,7,1},{3,8,1},{3,9,1},{3,10,1}}
				},
			};
			m_EfcMotion.Create(anim, 1);
			break;
		}

		case EFC_DASH_PLAYER:
		{
			//�A�j���[�V�����쐬�i�v���C���[�_�b�V���j
			SpriteAnimationCreate anim[] = {
				{
					"�G�t�F�N�g",
					210,40,
					40,105,
					FALSE,{{1,0,0}}
				},
			};
			m_EfcMotion.Create(anim, 1);
			break;
		}

		case EFC_GUARD_PLAYER:
		{
			//�A�j���[�V�����쐬(�v���C���[�K�[�h)
			SpriteAnimationCreate anim[] = {
				{
					"�G�t�F�N�g",
					110,550,
					120,110,
					FALSE,{{5,0,0}}
				},
			};
			m_EfcMotion.Create(anim, 1);
			break;
		}

		/*case EFC_DASH_PLAYER2:
		{
			//�A�j���[�V�����쐬(�v���C���[�_�b�V��2)
			SpriteAnimationCreate anim[] = {

				{
					"�G�t�F�N�g",
					160,30,
					40,125,
					FALSE,{{1,0,0}}
				},
			};
			m_EfcMotion.Create(anim, 1);
			break;
		}*/
	}
}

/**
* �J�n
*/
void CEffect::Start(float px, float py) {

	m_SrcRect = m_EfcMotion.GetSrcRect();
	m_EfcPos.x = px - m_SrcRect.GetWidth() * 0.5f;
	m_EfcPos.y = py - m_SrcRect.GetHeight() * 0.5f;
	m_bShow = true;
	m_EfcMotion.ChangeMotion(0);
}

/**
* �X�V
*/
void CEffect::Update() {
	//��\��
	if (!m_bShow)
	{
		return;
	}
	//�A�j���[�V�����X�V
	m_EfcMotion.AddTimer(CUtilities::GetFrameSecond());
	m_SrcRect = m_EfcMotion.GetSrcRect();

	//�A�j���[�V�����I���Ŕ�\��
	if (m_EfcMotion.IsEndMotion())
	{
		m_bShow = false;
	}
}

/**
* �`��
*/
void CEffect::Render(float wx,float wy) {
	//��\��
	if (!m_bShow)
	{
		return;
	}
	//�e�N�X�`���`��
	m_EfcTexture->Render(m_EfcPos.x - wx, m_EfcPos.y - wy, m_SrcRect);
}

/**
* �f�o�b�O�`��
*/
void CEffect::RenderDebug(float wx, float wy) {
	//��\��
	if (!m_bShow)
	{
		return;
	}
}

/**
* ���
*/
void CEffect::Release() {
	m_EfcMotion.Release();
}