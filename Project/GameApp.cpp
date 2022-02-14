/*************************************************************************//*!
					
					@file	GameApp.cpp
					@brief	��{�Q�[���A�v���B

															@author	�_�c�@��
															@date	2014.05.14
*//**************************************************************************/

//INCLUDE
#include	"GameApp.h"
#include	"GameDefine.h"
#include	"Title.h"
#include	"Game.h"
#include	"GameOver.h"
#include	"GameClear.h"
#include	"StageSelect.h"

//���݂̃V�[��
int			gNowScene = SCENENO_TITLE;

//�ύX����V�[��
int			gChangeScene = SCENENO_TITLE;

//�V�[���N���X
CTitle		 gTitleScene;
CGame		 gGameScene;
CGameOver	 gGameOverScene;
CGameClear   gGameClearScene;
CStageSelect gStageSelectScene;

//BGM
CSoundBuffer m_StageBGM;

//�f�o�b�O�\���t���O
bool	m_Debug = false;

/*************************************************************************//*!
		@brief			�A�v���P�[�V�����̏�����
		@param			None
				
		@return			TRUE		����<br>
						����ȊO	���s�A�G���[�R�[�h���߂�l�ƂȂ�
*//**************************************************************************/
MofBool CGameApp::Initialize(void){

	//���\�[�X�z�u�f�B���N�g���ݒ�
	CUtilities::SetCurrentDirectory("Resource");

	//�V�[�����̓ǂݍ���
	gTitleScene.Load();
	gStageSelectScene.Load();
	gGameScene.Load();
	gGameOverScene.Load();
	gGameClearScene.Load();

	//BGM
	m_StageBGM.Load("Stage_BGM.mp3");
	m_StageBGM.SetVolume(0.4f);
	m_StageBGM.SetLoop(TRUE);

	//�ŏ��̃V�[���̏�����
	gTitleScene.Initialize();
	
	return TRUE;
}
/*************************************************************************//*!
		@brief			�A�v���P�[�V�����̍X�V
		@param			None
				
		@return			TRUE		����<br>
						����ȊO	���s�A�G���[�R�[�h���߂�l�ƂȂ�
*//**************************************************************************/
MofBool CGameApp::Update(void){
	//�L�[�̍X�V
	g_pInput->RefreshKey();

	//�V�[���ɂ���čX�V
	switch (gNowScene)
	{
		//�^�C�g�����
		case SCENENO_TITLE:
			gTitleScene.Update();
			break;

		//�Q�[�����
		case SCENENO_GAME:
			gGameScene.Update();
			break;

		//�Q�[���I�[�o�[���
		case SCENENO_GAMEOVER:
			gGameOverScene.Update();
			break;

		//�Q�[���N���A���
		case SCENENO_GAMECLEAR:
			gGameClearScene.Update();
			break;

		//�X�e�[�W�Z���N�g���
		case SCENENO_STAGESELECT:
			gStageSelectScene.Update();
			break;
		
	}

	//�V�[���ύX���������ꍇ�ύX��V�[����������
	if (gChangeScene != gNowScene)
	{
		switch (gChangeScene)
		{
			case SCENENO_TITLE:
				gTitleScene.Initialize();
				break;
				
			case SCENENO_GAME:
				gGameScene.Release();
				gGameScene.Load();
				m_StageBGM.Play();
				gGameScene.Initialize();
				break;

			case SCENENO_GAMEOVER:
				gGameOverScene.Initialize();
				break;

			case SCENENO_GAMECLEAR:
				gGameClearScene.Initialize();
				break;

			case SCENENO_STAGESELECT:
				gStageSelectScene.Initialize();
				break;
		}
		gNowScene = gChangeScene;
	}

	//�f�o�b�O�\���̐؂�ւ�
	/*if (g_pInput->IsKeyPush(MOFKEY_F1))
	{
		m_Debug = ((m_Debug) ? false : true);
	}*/

	m_StageBGM.SetLoop(TRUE);
	return TRUE;
}
/*************************************************************************//*!
		@brief			�A�v���P�[�V�����̕`��
		@param			None
				
		@return			TRUE		����<br>
						����ȊO	���s�A�G���[�R�[�h���߂�l�ƂȂ�
*//**************************************************************************/
MofBool CGameApp::Render(void){
	//�`��J�n
	g_pGraphics->RenderStart();
	//��ʂ̃N���A
	g_pGraphics->ClearTarget(0.0f,0.0f,1.0f,0.0f,1.0f,0);

	//�V�[���ɂ���ĕ`��
	switch (gNowScene)
	{
		case SCENENO_TITLE:
			gTitleScene.Render();
			break;

		case SCENENO_GAME:
			gGameScene.Render();
			break;

		case SCENENO_GAMEOVER:
			gGameOverScene.Render();
			m_StageBGM.Stop();
			break;

		case SCENENO_GAMECLEAR:
			gGameClearScene.Render();
			m_StageBGM.Stop();
			break;

		case SCENENO_STAGESELECT:
			gStageSelectScene.Render();
			break;
	}

	//�f�o�b�O�`��
	if (m_Debug)
	{
		//�V�[���ɂ���ĕ`��
		switch (gNowScene)
		{
			case SCENENO_TITLE:
				gTitleScene.RenderDebug();
				break;

			case SCENENO_GAME:
				gGameScene.RenderDebug();
				break;

			case SCENENO_GAMEOVER:
				gGameOverScene.RenderDebug();
				break;

			case SCENENO_GAMECLEAR:
				gGameClearScene.RenderDebug();
				break;
		}
	}

	//�`��̏I��
	g_pGraphics->RenderEnd();
	return TRUE;
}
/*************************************************************************//*!
		@brief			�A�v���P�[�V�����̉��
		@param			None
				
		@return			TRUE		����<br>
						����ȊO	���s�A�G���[�R�[�h���߂�l�ƂȂ�
*//**************************************************************************/
MofBool CGameApp::Release(void){
	gTitleScene.Release();
	gGameScene.Release();
	gGameOverScene.Release();
	gGameClearScene.Release();
	gStageSelectScene.Release();
	//BGM���
	m_StageBGM.Release();
	return TRUE;
}