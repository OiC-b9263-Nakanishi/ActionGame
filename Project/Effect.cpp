#include "Effect.h"

/**
* コンストラクタ
*/
CEffect::CEffect():
m_EfcTexture(NULL),
m_EfcMotion(),
m_EfcPos(0.0f,0.0f),
m_bShow(false),
m_SrcRect(){
}

/**
* デストラクタ
*/
CEffect::~CEffect() {

}

/**
* 初期化
*/
void CEffect::Initialize(int type) {
	m_EfcPos.x = 0.0f;
	m_EfcPos.y = 0.0f;
	m_bShow = false;

	switch (type)
	{
		case EFC_DAMAGE:
		{
			//アニメーション作成(ダメージ)
			SpriteAnimationCreate anim[] = {
				{
					"エフェクト",
					0,0,
					128,128,
					FALSE,{{2,0,0},{2,1,0},{2,2,0},{2,3,0}}
				},
			};
			m_EfcMotion.Create(anim, 1);
			break;
		}
		case EFC_EXPLOSIONENEMY:
		{
			//アニメーション作成(敵爆破)
			SpriteAnimationCreate anim[] = {
				{
					"エフェクト",
					0,0,
					120,120,
					FALSE,{{3,0,0},{3,1,0},{3,2,0},{3,3,0},{3,4,0},
						   {3,5,0},{3,6,0},{3,7,0},{3,8,0},{3,9,0},
						   {3,10,0},{3,11,0},{3,12,0},{3,13,0},{3,14,0},{3,15,0}}
				},
			};
			m_EfcMotion.Create(anim, 1);
			break;
		}
		case EFC_EXPLOSIONPLAYER:
		{
			//アニメーション作成(プレイヤー爆破)
			SpriteAnimationCreate anim[] = {
				{
					"エフェクト",
					0,0,
					180,180,
					FALSE,{{3,0,0},{3,1,0},{3,2,0},{3,3,0},{3,4,0},
						   {3,5,0},{3,6,0},{3,7,0},{3,8,0},{3,9,0},{3,10,0},
						   {3,0,1},{3,1,1},{3,2,1},{3,3,1},{3,4,1},
						   {3,5,1},{3,6,1},{3,7,1},{3,8,1},{3,9,1},{3,10,1}}
				},
			};
			m_EfcMotion.Create(anim, 1);
			break;
		}

		case EFC_DASH_PLAYER:
		{
			//アニメーション作成（プレイヤーダッシュ）
			SpriteAnimationCreate anim[] = {
				{
					"エフェクト",
					210,40,
					40,105,
					FALSE,{{1,0,0}}
				},
			};
			m_EfcMotion.Create(anim, 1);
			break;
		}

		case EFC_GUARD_PLAYER:
		{
			//アニメーション作成(プレイヤーガード)
			SpriteAnimationCreate anim[] = {
				{
					"エフェクト",
					110,550,
					120,110,
					FALSE,{{5,0,0}}
				},
			};
			m_EfcMotion.Create(anim, 1);
			break;
		}

		/*case EFC_DASH_PLAYER2:
		{
			//アニメーション作成(プレイヤーダッシュ2)
			SpriteAnimationCreate anim[] = {

				{
					"エフェクト",
					160,30,
					40,125,
					FALSE,{{1,0,0}}
				},
			};
			m_EfcMotion.Create(anim, 1);
			break;
		}*/
	}
}

/**
* 開始
*/
void CEffect::Start(float px, float py) {

	m_SrcRect = m_EfcMotion.GetSrcRect();
	m_EfcPos.x = px - m_SrcRect.GetWidth() * 0.5f;
	m_EfcPos.y = py - m_SrcRect.GetHeight() * 0.5f;
	m_bShow = true;
	m_EfcMotion.ChangeMotion(0);
}

/**
* 更新
*/
void CEffect::Update() {
	//非表示
	if (!m_bShow)
	{
		return;
	}
	//アニメーション更新
	m_EfcMotion.AddTimer(CUtilities::GetFrameSecond());
	m_SrcRect = m_EfcMotion.GetSrcRect();

	//アニメーション終了で非表示
	if (m_EfcMotion.IsEndMotion())
	{
		m_bShow = false;
	}
}

/**
* 描画
*/
void CEffect::Render(float wx,float wy) {
	//非表示
	if (!m_bShow)
	{
		return;
	}
	//テクスチャ描画
	m_EfcTexture->Render(m_EfcPos.x - wx, m_EfcPos.y - wy, m_SrcRect);
}

/**
* デバッグ描画
*/
void CEffect::RenderDebug(float wx, float wy) {
	//非表示
	if (!m_bShow)
	{
		return;
	}
}

/**
* 解放
*/
void CEffect::Release() {
	m_EfcMotion.Release();
}