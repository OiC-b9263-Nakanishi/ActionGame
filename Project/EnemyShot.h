#pragma once

#include "Mof.h"

class CEnemyShot {
private:
	CTexture*		m_ShotTexture;;
	CVector2		m_ShotPos;
	CVector2		m_ShotSpd;
	bool			m_bShow;

public:
	CEnemyShot();
	~CEnemyShot();
	void Initialize();
	void Fire(const Vector2& p, const Vector2& s);
	void Update(float wx,float wy);
	void Render(float wx,float wy);
	void RenderDebug();
	void SetTexture(CTexture* st) { m_ShotTexture = st; }
	bool GetShow() { return m_bShow; }
	void SetShow(bool bs) { m_bShow = bs; }
	CRectangle GetRect() {
		return CRectangle(m_ShotPos.x, m_ShotPos.y,
			m_ShotPos.x + m_ShotTexture->GetWidth(), m_ShotPos.y + m_ShotTexture->GetHeight());
	}
	//’e‚Ì”z—ñ‚©‚ç”ñ•\¦‚Ì‚à‚Ì‚ğŒŸõ
	static CEnemyShot* FindAvailableShot(CEnemyShot* shot, int smax);
};