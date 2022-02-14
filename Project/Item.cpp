#include "Item.h"

/**
* コンストラクタ
*/
CItem::CItem():
m_ItemTexture(NULL),
m_ItemMotion(),
m_ItemPos(0.0f,0.0f),
m_ItemMove(0.0f,0.0f),
m_bShow(false),
m_SrcRect(){
}

/**
* デストラクタ
*/
CItem::~CItem() {
	//delete[]で解放されるようにReleaseをデストラクタで実行
	Release();
}

/**
* 初期化
* [in]   px		X座標
* [in]   py		Y座標
* [in]   type   敵タイプ
*/
void CItem::Initialize(float px, float py, int type) {
	m_Type = type;
	m_ItemPos.x = px;
	m_ItemPos.y = py;
	m_ItemMove.x = 0.0f;
	m_ItemMove.y = 0.0f;
	m_bShow = true;

	//アニメーション作成
	SpriteAnimationCreate anim = {
		"アイテム",
		0,0,
		32,32,
		TRUE,{{5,0,0}, {5,1,0}, {5,2,0}, {5,3,0}}
	};
	m_ItemMotion.Create(anim);
}

/**
* 更新
*/
void CItem::Update() {
	//非表示
	if (!m_bShow)
	{
		return;
	}
	//重力によって下がる
	m_ItemMove.y += GRAVITY;
	if (m_ItemMove.y >= 20.0f)
	{
		m_ItemMove.y = 20.0f;
	}

	//座標移動
	m_ItemPos.x += m_ItemMove.x;
	m_ItemPos.y += m_ItemMove.y;

	//アニメーション更新
	m_ItemMotion.AddTimer(CUtilities::GetFrameSecond());
	m_SrcRect = m_ItemMotion.GetSrcRect();
}

/**
* ステージとの当たり
* [in]		ox			X埋まり量
* [in]      oy			Y埋まり量
*/
void CItem::ColisionStage(float ox, float oy) {
	m_ItemPos.x += ox;
	m_ItemPos.y += oy;

	//落下中下埋まり、ジャンプ中上埋まりは初期化
	if (oy < 0 && m_ItemMove.y > 0)
	{
		m_ItemMove.y = 0;
	}
	else if (oy > 0 && m_ItemMove.y < 0)
	{
		m_ItemMove.y = 0;
	}
	//左移動中左埋まり、右移動中右埋まり
	if (ox < 0 && m_ItemMove.x > 0)
	{
		m_ItemMove.x = 0;
	}
	else if (ox > 0 && m_ItemMove.x < 0)
	{
		m_ItemMove.x = 0;
	}
}

/**
* 描画
* [in]		wx		ワールド変化
* [in]		wy		ワールド変化
*/
void CItem::Render(float wx, float wy) {
	//非表示
	if (!m_bShow)
	{
		return;
	}
	//テクスチャ描画
	m_ItemTexture->Render(m_ItemPos.x - wx, m_ItemPos.y - wy, m_SrcRect);
}

/**
* デバッグ描画
* [in]		wx		ワールド変化
* [in]		wy		ワールド変化
*/
void CItem::RenderDebug(float wx, float wy) {
	//非表示
	if (m_bShow)
	{
		return;
	}

	//当たり判定表示
	CRectangle hr = GetRect();
	CGraphicsUtilities::RenderRect(hr.Left - wx, hr.Top - wy, hr.Right - wx, hr.Bottom - wy, MOF_XRGB(255, 0, 0));
}

/**
* 解放
*/
void CItem::Release() {
	m_ItemMotion.Release();
}