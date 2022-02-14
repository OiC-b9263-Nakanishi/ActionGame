#include "StageSelect.h"
#include "GameDefine.h"
//�ύX����V�[��
extern int		gChangeScene;

//�X�e�[�W�ԍ�
extern int			gStageNo = STAGENO_1;

/**
* �R���X�g���N�^
*/
CStageSelect::CStageSelect()
{
}

/**
* �f�X�g���N�^
*/
CStageSelect::~CStageSelect()
{
}

/**
* �ǂݍ���
*/
bool CStageSelect::Load()
{
	return false;
}

/**
* ������
*/
void CStageSelect::Initialize()
{
}

/**
* �X�V
*/
void CStageSelect::Update()
{
	if (g_pInput->IsKeyPush(MOFKEY_1))
	{
		gStageNo = STAGENO_1;
		gChangeScene = SCENENO_GAME;
	}
	else if (g_pInput->IsKeyPush(MOFKEY_2))
	{
		gStageNo = STAGENO_2;
		gChangeScene = SCENENO_GAME;
	}
	else if (g_pInput->IsKeyPush(MOFKEY_3))
	{
		gStageNo = STAGENO_TUTORIAL;
		gChangeScene = SCENENO_GAME;
	}
}

/**
* �`��
*/
void CStageSelect::Render(){
	CGraphicsUtilities::RenderString(10, 10, "������1��Stage1");
	CGraphicsUtilities::RenderString(10, 40, "������2��Stage2");
	CGraphicsUtilities::RenderString(10, 70, "������3�Ń`���[�g���A��");
}

/**
* �f�o�b�O�`��
*/
void CStageSelect::RenderDebug()
{
}

/**
* ���
*/
void CStageSelect::Release()
{
}
