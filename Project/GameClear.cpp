#include "GameClear.h"
#include "GameDefine.h"

//変更するシーン(外部参照、実体はGameApp)
extern int			gChangeScene;

extern int			gStageNo;

/**
* コンストラクタ
*/
CGameClear::CGameClear()
{
}

/**
* デストラクタ
*/
CGameClear::~CGameClear()
{

}

/**
* 読み込み
*/
bool CGameClear::Load() {

	//テクスチャ読み込み
	if (!m_BackImage.Load("GameClear.png"))
	{
		return false;
	}

	return true;
}

/**
* 初期化
*/
void CGameClear::Initialize() {

}

/**
* 更新
*/
void CGameClear::Update() {

	//Enterキーで画面遷移
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
* 描画
*/
void CGameClear::Render() {

	m_BackImage.Render(0, 0);
	CGraphicsUtilities::RenderString(400, 500, MOF_COLOR_WHITE, "Press Enter Key");
	CGraphicsUtilities::RenderString(490, 550, MOF_COLOR_WHITE, "or");
	CGraphicsUtilities::RenderString(400, 600, MOF_COLOR_WHITE, "Mouse LButton");
	//CGraphicsUtilities::RenderString(0, 0, MOF_XRGB(255, 255, 255), "ゲームクリア画面");
}

/**
* デバッグ描画
*/
void CGameClear::RenderDebug() {

}

/**
* 解放
*/
void CGameClear::Release() {
	m_BackImage.Release();
}