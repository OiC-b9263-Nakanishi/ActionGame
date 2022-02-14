#pragma once
#include "Mof.h"

class CGameClear{

private:
	CTexture		m_BackImage;

public:
	CGameClear();
	~CGameClear();
	bool Load();
	void Initialize();
	void Update();
	void Render();
	void RenderDebug();
	void Release();

};