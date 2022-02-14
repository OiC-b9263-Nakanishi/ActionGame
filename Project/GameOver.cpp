#include "GameDefine.h"
#include "GameOver.h"

//�ύX����V�[��(�O���Q�ƁA���̂�GameApp)
extern int			gChangeScene;

/**
* �R���X�g���N�^
*/
CGameOver::CGameOver()
{
}

/**
* �f�X�g���N�^
*/
CGameOver::~CGameOver() {

}

/**
* �ǂݍ���
*/
bool CGameOver::Load() {
	//�e�N�X�`���ǂݍ���
	if (!m_BackImage.Load("GameOver.png"))
	{
		return false;
	}

	return true;
}

/**
* ������
*/
void CGameOver::Initialize() {

}

/**
* �X�V
*/
void CGameOver::Update() {

	//Enter�L�[�ŉ�ʑJ��
	if (g_pInput->IsKeyPush(MOFKEY_RETURN) || g_pInput->IsMouseKeyPush(MOFMOUSE_LBUTTON))
	{
		gChangeScene = SCENENO_TITLE;
	}
}

/**
* �`��
*/
void CGameOver::Render() {
	m_BackImage.Render(0, 0);
	CGraphicsUtilities::RenderString(400, 500, MOF_COLOR_RED, "Press Enter Key");
	CGraphicsUtilities::RenderString(490, 550, MOF_COLOR_RED, "or");
	CGraphicsUtilities::RenderString(400, 600, MOF_COLOR_RED, "Mouse LButton");
	//CGraphicsUtilities::RenderString(0, 0, MOF_XRGB(255, 255, 255), "�Q�[���I�[�o�[���");
}

/**
* �f�o�b�O�`��
*/
void CGameOver::RenderDebug() {

}

/**
* ���
*/
void CGameOver::Release() {
	m_BackImage.Release();
}