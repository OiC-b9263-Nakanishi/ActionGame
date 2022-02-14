
#include "Title.h"
#include "GameDefine.h"

//�ύX����V�[��(�O���Q�ƁA���̂�GameApp)
extern int				gChangeScene;

/**
* �R���X�g���N�^
*/
CTitle::CTitle(): 
	m_BackImage(){
	
}

/**
* �f�X�g���N�^
*/
CTitle::~CTitle() {

}

/**
* �ǂݍ���
*/
bool CTitle::Load() {
	//�^�C�g���摜�̓ǂݍ���
	if (!m_BackImage.Load("Title.png"))
	{
		return false;
	}

	return true;
}

/**
* ������
*/
void CTitle::Initialize() {

}

/**
* �X�V
*/
void CTitle::Update() {

	//Enter�L�[�ŃQ�[����ʂ։�ʑJ��
	if (g_pInput->IsKeyPush(MOFKEY_RETURN) || g_pInput->IsMouseKeyPush(MOFMOUSE_LBUTTON))
	{
		gChangeScene = SCENENO_STAGESELECT;
	}
}

/**
* �`��
*/
void CTitle::Render() {
	CGraphicsUtilities::RenderFillRect(0, 0, 1024, 768, MOF_COLOR_WHITE);
	//CGraphicsUtilities::RenderString(0, 0, MOF_XRGB(0, 0, 255), "�^�C�g�����");
	m_BackImage.Render(0,200);
	CGraphicsUtilities::RenderString(400, 500, MOF_COLOR_BLUE, "Press Enter Key");
	CGraphicsUtilities::RenderString(490, 550, MOF_COLOR_BLUE, "or");
	CGraphicsUtilities::RenderString(400, 600, MOF_COLOR_BLUE, "Mouse LButton");
}

/**
* �f�o�b�O�`��
*/
void CTitle::RenderDebug() {

}

/**
* ���
*/
void CTitle::Release() {
	m_BackImage.Release();
}