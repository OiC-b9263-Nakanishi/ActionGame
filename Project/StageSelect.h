#pragma once
#include "Mof.h"

class CStageSelect {
private:

public:
	CStageSelect();
	~CStageSelect();
	bool Load();
	void Initialize();
	void Update();
	void Render();
	void RenderDebug();
	void Release();
};