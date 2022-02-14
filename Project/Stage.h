#pragma once
#include "Mof.h"
#include "Player.h"
#include "Enemy.h"
#include "Item.h"
#include "Boss.h"

#define		LEFTSLOPE		2
#define		RIGHTSLOPE		3


class CStage {
private:
	CTexture			m_ChipTexture;
	CTexture			m_BackTexture;

	float				m_ChipSize;
	int					m_XCount;
	int					m_YCount;
	char*				m_pChipData;

	Vector2				m_Scroll;

	float				m_ScrollX;
	float				m_ScrollY;

	//�G�֌W
	int					m_EnemyTextureCount;
	CTexture*			m_pEnemyTexture;
	char*				m_pEnemyData;
	int					m_EnemyCount;

	//�A�C�e���֌W
	int					m_ItemTextureCount;
	CTexture*			m_pItemTexture;
	char*				m_pItemData;
	int					m_ItemCount;

	//�{�X���W
	float				m_BossPosX;
	float				m_BossPosY;

public:
	CStage();
	~CStage();
	bool Load(char* pName);
	void Initialize(CEnemy* pEnemy,CItem* pItem);
	void Update(CPlayer& pl);
	void Render();
	void RenderDebug();
	void Release();
	float GetScrollX() { return m_Scroll.x; }
	float GetScrollY() { return m_Scroll.y; }
	bool Colision(CRectangle r, float& ox, float& oy);

	bool IsGround(float px, float py);

	//�G�̐�
	int GetEnemyCount() { return m_EnemyCount; }

	//�A�C�e����
	int GetItemCount() { return m_ItemCount; }

	//�{�X���W
	float GetBossPosX() { return m_BossPosX; }
	float GetBossPosY() { return m_BossPosY; }
};