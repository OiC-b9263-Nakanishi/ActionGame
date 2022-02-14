
#include "Title.h"
#include "GameDefine.h"

//変更するシーン(外部参照、実体はGameApp)
extern int				gChangeScene;

/**
* コンストラクタ
*/
CTitle::CTitle(): 
	m_BackImage(){
	
}

/**
* デストラクタ
*/
CTitle::~CTitle() {

}

/**
* 読み込み
*/
bool CTitle::Load() {
	//タイトル画像の読み込み
	if (!m_BackImage.Load("Title.png"))
	{
		return false;
	}

	return true;
}

/**
* 初期化
*/
void CTitle::Initialize() {

}

/**
* 更新
*/
void CTitle::Update() {

	//Enterキーでゲーム画面へ画面遷移
	if (g_pInput->IsKeyPush(MOFKEY_RETURN) || g_pInput->IsMouseKeyPush(MOFMOUSE_LBUTTON))
	{
		gChangeScene = SCENENO_STAGESELECT;
	}
}

/**
* 描画
*/
void CTitle::Render() {
	CGraphicsUtilities::RenderFillRect(0, 0, 1024, 768, MOF_COLOR_WHITE);
	//CGraphicsUtilities::RenderString(0, 0, MOF_XRGB(0, 0, 255), "タイトル画面");
	m_BackImage.Render(0,200);
	CGraphicsUtilities::RenderString(400, 500, MOF_COLOR_BLUE, "Press Enter Key");
	CGraphicsUtilities::RenderString(490, 550, MOF_COLOR_BLUE, "or");
	CGraphicsUtilities::RenderString(400, 600, MOF_COLOR_BLUE, "Mouse LButton");
}

/**
* デバッグ描画
*/
void CTitle::RenderDebug() {

}

/**
* 解放
*/
void CTitle::Release() {
	m_BackImage.Release();
}