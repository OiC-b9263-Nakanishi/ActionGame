#include "EffectManager.h"

/**
* コンストラクタ
*/
CEffectManager::CEffectManager()
{
}

/**
* デストラクタ
*/
CEffectManager::~CEffectManager() {

}

/**
* 読み込み
*/
bool CEffectManager::Load() {
	//テクスチャ読み込み
	char* name[] = {
		"Effect01.png",
		"Effect02.png",
		"Effect03.png",
		"Effect05.png",
		"Effect04.png",
	};
	for (int i = 0; i < EFC_TYPECOUNT; i++)
	{
		if (!m_Texture[i].Load(name[i]))
		{
			return false;
		}
	}
	return true;
}

/**
* 初期化
*/
void CEffectManager::Initialize() {
	//エフェクト基礎設定
	for (int j = 0; j < EFC_TYPECOUNT; j++)
	{
		for (int i = 0; i < EFC_TYPECOUNT; i++)
		{
			m_Effect[i][j].SetTexture(&m_Texture[j]);
			m_Effect[i][j].Initialize(j);
		}
	}
}

/**
* 開始
*/
CEffect* CEffectManager::Start(float px, float py, int type) {
	for (int i = 0; i < EFFECTCOUNT; i++)
	{
		if (m_Effect[i][type].GetShow())
		{
			continue;
		}
		m_Effect[i][type].Start(px, py);
		return &m_Effect[i][type];
	}
	return NULL;
}

/**
* 更新
*/
void CEffectManager::Update() {
	for (int j = 0; j < EFC_TYPECOUNT; j++)
	{
		for (int i = 0; i < EFFECTCOUNT; i++)
		{
			m_Effect[i][j].Update();
		}
	}
}

/**
* 描画
*/
void CEffectManager::Render(float wx, float wy) {
	for (int j = 0; j < EFC_TYPECOUNT; j++)
	{
		for (int i = 0; i < EFC_TYPECOUNT; i++)
		{
			m_Effect[i][j].Render(wx, wy);
		}
	}
}

/**
* デバッグ描画
*/
void CEffectManager::RenderDebug(float wx, float wy) {
	for (int j = 0; j < EFC_TYPECOUNT; j++)
	{
		for (int i = 0; i < EFC_TYPECOUNT; i++)
		{
			m_Effect[i][j].RenderDebug(wx, wy);
		}
	}
}

/**
* 解放
*/
void CEffectManager::Release() {
	for (int j = 0; j < EFC_TYPECOUNT; j++)
	{
		for (int i = 0; i < EFC_TYPECOUNT; i++)
		{
			m_Effect[i][j].Release();
		}
		m_Texture[j].Release();
	}
}