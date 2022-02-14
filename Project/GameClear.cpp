#include "GameClear.h"
#include "GameDefine.h"

//�ύX����V�[��(�O���Q�ƁA���̂�GameApp)
extern int			gChangeScene;

extern int			gStageNo;

/**
* �R���X�g���N�^
*/
CGameClear::CGameClear()
{
}

/**
* �f�X�g���N�^
*/
CGameClear::~CGameClear()
{

}

/**
* �ǂݍ���
*/
bool CGameClear::Load() {

	//�e�N�X�`���ǂݍ���
	if (!m_BackImage.Load("GameClear.png"))
	{
		return false;
	}

	return true;
}

/**
* ������
*/
void CGameClear::Initialize() {

}

/**
* �X�V
*/
void CGameClear::Update() {

	//Enter�L�[�ŉ�ʑJ��
	if (g_pInput->IsKeyPush(MOFKEY_RETURN) || g_pInput->IsMouseKeyPush(MOFMOUSE_LBUTTON) && gStageNo != STAGENO_TUTORIAL)
	{
		gChangeScene = SCENENO_TITLE;
	}
	else if (g_pInput->IsKeyPush(MOFKEY_RETURN) || g_pInput->IsMouseKeyPush(MOFMOUSE_LBUTTON) && gStageNo == STAGENO_TUTORIAL)
	{
		gChangeScene = SCENENO_STAGESELECT;
	}
}

/**
* �`��
*/
void CGameClear::Render() {

	m_BackImage.Render(0, 0);
	CGraphicsUtilities::RenderString(400, 500, MOF_COLOR_WHITE, "Press Enter Key");
	CGraphicsUtilities::RenderString(490, 550, MOF_COLOR_WHITE, "or");
	CGraphicsUtilities::RenderString(400, 600, MOF_COLOR_WHITE, "Mouse LButton");
	//CGraphicsUtilities::RenderString(0, 0, MOF_XRGB(255, 255, 255), "�Q�[���N���A���");
}

/**
* �f�o�b�O�`��
*/
void CGameClear::RenderDebug() {

}

/**
* ���
*/
void CGameClear::Release() {
	m_BackImage.Release();
}