#pragma once
#include "Mof.h"

class CGameOver {
private:
	CTexture	m_BackImage;

public:
	CGameOver();
	~CGameOver();
	bool Load();
	void Initialize();
	void Update();
	void Render();
	void RenderDebug();
	void Release();
};