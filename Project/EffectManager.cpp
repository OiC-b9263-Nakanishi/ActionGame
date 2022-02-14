#include "EffectManager.h"

/**
* �R���X�g���N�^
*/
CEffectManager::CEffectManager()
{
}

/**
* �f�X�g���N�^
*/
CEffectManager::~CEffectManager() {

}

/**
* �ǂݍ���
*/
bool CEffectManager::Load() {
	//�e�N�X�`���ǂݍ���
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
* ������
*/
void CEffectManager::Initialize() {
	//�G�t�F�N�g��b�ݒ�
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
* �J�n
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
* �X�V
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
* �`��
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
* �f�o�b�O�`��
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
* ���
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