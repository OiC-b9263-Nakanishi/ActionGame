#include "EnemyShot.h"

/**
* コンストラクタ
*/
CEnemyShot::CEnemyShot():
m_ShotTexture(NULL),
m_ShotPos(0.0f,0.0f),
m_ShotSpd(0.0f,0.0f),
m_bShow(false)
{
}

/**
* デストラクタ
*/
CEnemyShot::~CEnemyShot() {

}

/**
* 初期化
*/
void CEnemyShot::Initialize() {
	m_ShotPos = Vector2(0, 0);
	m_ShotSpd = Vector2(0, 0);
	m_bShow = false;
}

/**
* 発射
*
* 引数
* [in]		px		発射X座標
* [in]		py		発射Y座標
* [in]		sx		移動X速度
* [in]		sy		移動Y速度
*/
void CEnemyShot::Fire(const Vector2& p,const Vector2& s) {
	m_ShotPos = p;
	m_ShotSpd = s;
	m_bShow = true;
}

/**
* 更新
*/
void CEnemyShot::Update(float wx,float wy) {
	//非表示
	if (!m_bShow)
	{
		return;
	}
	//速度を利用した移動
	m_ShotPos += m_ShotSpd;

	//画面外で消去
	if (m_ShotPos.x + m_ShotTexture->GetWidth() < 0 || m_ShotPos.x - wx > g_pGraphics->GetTargetWidth() ||
		m_ShotPos.y + m_ShotTexture->GetHeight() < 0 || m_ShotPos.y - wy > g_pGraphics->GetTargetHeight())
	{
		m_bShow = false;
	}
}

/**
* 描画
*/
void CEnemyShot::Render(float wx,float wy) {
	//非表示
	if (!m_bShow)
	{
		return;
	}
	//テクスチャ表示
	m_ShotTexture->Render(m_ShotPos.x - wx, m_ShotPos.y - wy);
}

/**
* デバッグ描画
*/
void CEnemyShot::RenderDebug() {
	//非表示
	if (!m_bShow)
	{
		return;
	}
	//当たり判定の表示
	CGraphicsUtilities::RenderRect(m_ShotPos.x, m_ShotPos.y, m_ShotPos.x + m_ShotTexture->GetWidth(),
		m_ShotPos.y + m_ShotTexture->GetHeight(), MOF_XRGB(255, 0, 0));
}

/**
* 弾配列から非表示のものを検索する
*/
CEnemyShot* CEnemyShot::FindAvailableShot(CEnemyShot* shot, int smax) {
	for (int i = 0; i < smax; i++)
	{
		if (!shot[i].GetShow())
		{
			return &shot[i];
		}
	}
	return NULL;
}