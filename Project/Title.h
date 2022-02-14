#pragma once

#include "Mof.h"

class CTitle {
private:
	
	//ƒ^ƒCƒgƒ‹‰æ–Ê‰æ‘œ
	CTexture			m_BackImage;

public:
	CTitle();
	~CTitle();
	bool Load();
	void Initialize();
	void Update();
	void Render();
	void RenderDebug();
	void Release();
};