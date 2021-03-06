#pragma once

#include "Mof.h"

class CTitle {
private:
	
	//タイトル画面画像
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