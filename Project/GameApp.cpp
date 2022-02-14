/*************************************************************************//*!
					
					@file	GameApp.cpp
					@brief	基本ゲームアプリ。

															@author	濱田　享
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

//現在のシーン
int			gNowScene = SCENENO_TITLE;

//変更するシーン
int			gChangeScene = SCENENO_TITLE;

//シーンクラス
CTitle		 gTitleScene;
CGame		 gGameScene;
CGameOver	 gGameOverScene;
CGameClear   gGameClearScene;
CStageSelect gStageSelectScene;

//BGM
CSoundBuffer m_StageBGM;

//デバッグ表示フラグ
bool	m_Debug = false;

/*************************************************************************//*!
		@brief			アプリケーションの初期化
		@param			None
				
		@return			TRUE		成功<br>
						それ以外	失敗、エラーコードが戻り値となる
*//**************************************************************************/
MofBool CGameApp::Initialize(void){

	//リソース配置ディレクトリ設定
	CUtilities::SetCurrentDirectory("Resource");

	//シーン情報の読み込み
	gTitleScene.Load();
	gStageSelectScene.Load();
	gGameScene.Load();
	gGameOverScene.Load();
	gGameClearScene.Load();

	//BGM
	m_StageBGM.Load("Stage_BGM.mp3");
	m_StageBGM.SetVolume(0.4f);
	m_StageBGM.SetLoop(TRUE);

	//最初のシーンの初期化
	gTitleScene.Initialize();
	
	return TRUE;
}
/*************************************************************************//*!
		@brief			アプリケーションの更新
		@param			None
				
		@return			TRUE		成功<br>
						それ以外	失敗、エラーコードが戻り値となる
*//**************************************************************************/
MofBool CGameApp::Update(void){
	//キーの更新
	g_pInput->RefreshKey();

	//シーンによって更新
	switch (gNowScene)
	{
		//タイトル画面
		case SCENENO_TITLE:
			gTitleScene.Update();
			break;

		//ゲーム画面
		case SCENENO_GAME:
			gGameScene.Update();
			break;

		//ゲームオーバー画面
		case SCENENO_GAMEOVER:
			gGameOverScene.Update();
			break;

		//ゲームクリア画面
		case SCENENO_GAMECLEAR:
			gGameClearScene.Update();
			break;

		//ステージセレクト画面
		case SCENENO_STAGESELECT:
			gStageSelectScene.Update();
			break;
		
	}

	//シーン変更があった場合変更先シーンを初期化
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

	//デバッグ表示の切り替え
	/*if (g_pInput->IsKeyPush(MOFKEY_F1))
	{
		m_Debug = ((m_Debug) ? false : true);
	}*/

	m_StageBGM.SetLoop(TRUE);
	return TRUE;
}
/*************************************************************************//*!
		@brief			アプリケーションの描画
		@param			None
				
		@return			TRUE		成功<br>
						それ以外	失敗、エラーコードが戻り値となる
*//**************************************************************************/
MofBool CGameApp::Render(void){
	//描画開始
	g_pGraphics->RenderStart();
	//画面のクリア
	g_pGraphics->ClearTarget(0.0f,0.0f,1.0f,0.0f,1.0f,0);

	//シーンによって描画
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

	//デバッグ描画
	if (m_Debug)
	{
		//シーンによって描画
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

	//描画の終了
	g_pGraphics->RenderEnd();
	return TRUE;
}
/*************************************************************************//*!
		@brief			アプリケーションの解放
		@param			None
				
		@return			TRUE		成功<br>
						それ以外	失敗、エラーコードが戻り値となる
*//**************************************************************************/
MofBool CGameApp::Release(void){
	gTitleScene.Release();
	gGameScene.Release();
	gGameOverScene.Release();
	gGameClearScene.Release();
	gStageSelectScene.Release();
	//BGM解放
	m_StageBGM.Release();
	return TRUE;
}