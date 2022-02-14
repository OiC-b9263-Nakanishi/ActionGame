#include "GameDefine.h"
#include "GameOver.h"

//変更するシーン(外部参照、実体はGameApp)
extern int			gChangeScene;

/**
* コンストラクタ
*/
CGameOver::CGameOver()
{
}

/**
* デストラクタ
*/
CGameOver::~CGameOver() {

}

/**
* 読み込み
*/
bool CGameOver::Load() {
	//テクスチャ読み込み
	if (!m_BackImage.Load("GameOver.png"))
	{
		return false;
	}

	return true;
}

/**
* 初期化
*/
void CGameOver::Initialize() {

}

/**
* 更新
*/
void CGameOver::Update() {

	//Enterキーで画面遷移
	if (g_pInput->IsKeyPush(MOFKEY_RETURN) || g_pInput->IsMouseKeyPush(MOFMOUSE_LBUTTON))
	{
		gChangeScene = SCENENO_TITLE;
	}
}

/**
* 描画
*/
void CGameOver::Render() {
	m_BackImage.Render(0, 0);
	CGraphicsUtilities::RenderString(400, 500, MOF_COLOR_RED, "Press Enter Key");
	CGraphicsUtilities::RenderString(490, 550, MOF_COLOR_RED, "or");
	CGraphicsUtilities::RenderString(400, 600, MOF_COLOR_RED, "Mouse LButton");
	//CGraphicsUtilities::RenderString(0, 0, MOF_XRGB(255, 255, 255), "ゲームオーバー画面");
}

/**
* デバッグ描画
*/
void CGameOver::RenderDebug() {

}

/**
* 解放
*/
void CGameOver::Release() {
	m_BackImage.Release();
}