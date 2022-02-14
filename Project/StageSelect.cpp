#include "StageSelect.h"
#include "GameDefine.h"
//変更するシーン
extern int		gChangeScene;

//ステージ番号
extern int			gStageNo = STAGENO_1;

/**
* コンストラクタ
*/
CStageSelect::CStageSelect()
{
}

/**
* デストラクタ
*/
CStageSelect::~CStageSelect()
{
}

/**
* 読み込み
*/
bool CStageSelect::Load()
{
	return false;
}

/**
* 初期化
*/
void CStageSelect::Initialize()
{
}

/**
* 更新
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
* 描画
*/
void CStageSelect::Render(){
	CGraphicsUtilities::RenderString(10, 10, "数字の1でStage1");
	CGraphicsUtilities::RenderString(10, 40, "数字の2でStage2");
	CGraphicsUtilities::RenderString(10, 70, "数字の3でチュートリアル");
}

/**
* デバッグ描画
*/
void CStageSelect::RenderDebug()
{
}

/**
* 解放
*/
void CStageSelect::Release()
{
}
