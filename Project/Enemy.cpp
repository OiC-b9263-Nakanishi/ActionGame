#include "Enemy.h"

/**
* コンストラクタ
*/
CEnemy::CEnemy():
m_EneTexture(NULL),
m_EneMotion(),
m_EnePos(0,0),
m_EneMove(0,0),
m_bShow(false),
m_bReverse(false),
m_bBoost(false),
m_SrcRect(),
m_EneHP(0),
m_ShotWait(0),
m_ShotWaitSet(0),
m_TargetPos(0,0),
m_DamageWait(0){
}

/**
* デストラクタ
*/
CEnemy::~CEnemy() {
//delete[]で解放されるようにReleaseをデストラクタで実行
	Release();
}

/**
* 初期化
*/
void CEnemy::Initialize(float px, float py, int type) {
	m_Type = type;
	m_EnePos.x = px;
	m_EnePos.y = py;
	m_EneMove.x = -3.0f;
	m_EneMove.y = 0.0f;
	m_bReverse = true;
	m_bShow = true;
	m_bBoost = false;
	m_EneHP = 30;
	m_DamageWait = 0;
	m_ShotWait = 0;
	m_ShotWaitSet = 80;
	m_TargetPos = CVector2(0, 0);
	m_SE_Shot.Load("SE_Shot.mp3");
	m_SE_Explosion.Load("Explosion_Enemy.mp3");

	switch (m_Type)
	{
		case 0:
			m_EneMove.x = -3.0f;
			break;

		case 1:
			m_EneMove.x = -4.0f;
			break;

		case 2:
			m_EneMove.x = -5.0f;
	}

	//アニメーションを作成
	SpriteAnimationCreate anim[] = {

	  {	
		"移動",
		0,0,
		60,64,
		TRUE,{{5,0,0},{5,1,0},{5,2,0},{5,3,0}}
	   },

	   {
		   "ダメージ",
		   0,210,
		   60,64,
		   FALSE,{{20,0,0}}
	   },

		{
			"ショット",
			0,220,
			60,64,
			FALSE,{{20,0,0}}
		},
	};
	m_EneMotion.Create(anim, MOTION_COUNT);
}

/**
* 更新
*/
void CEnemy::Update(CEnemyShot* shot,int smax) {
	//非表示
	if (!m_bShow)
	{
		return;
	}
	
	//重力により下がる
	m_EneMove.y += GRAVITY;
	if (m_EneMove.y >= 20.0f)
	{
		m_EneMove.y = 20.0f;
	}

	//画面外でHPを0にする
	if (m_EnePos.y >= g_pGraphics->GetTargetHeight() - 65 && m_EneHP > 0)
	{
		m_EneHP = 0;
		m_SE_Explosion.Play();
		//爆発エフェクト発生
		m_EneEffectManager->Start(m_EnePos.x + m_SrcRect.GetWidth() * 0.5f,
			m_EnePos.y + m_SrcRect.GetHeight() * 0.5f, EFC_EXPLOSIONENEMY);
	}

	//実際に座標を変化
	m_EnePos.x += m_EneMove.x;
	m_EnePos.y += m_EneMove.y;

	//アニメーション更新
	m_EneMotion.AddTimer(CUtilities::GetFrameSecond());
	m_SrcRect = m_EneMotion.GetSrcRect();

	//ダメージインターバル減少
	if (m_DamageWait > 0)
	{
		m_DamageWait--;
	}

	//敵タイプ別行動
	switch (m_Type)
	{
		case 0:
			//敵（近距離）
			EnemyType0();
			break;

		case 1:
			//敵（遠距離）
			EnemyType1(shot,smax);
			break;

		case 2:
			EnemyType2();
			break;
	}

	//向き修正
	if (m_EnePos.x > m_TargetPos.x && m_Type == 1 && m_bBoost)
	{
		m_bReverse = true;
	}
	else if(m_EnePos.x < m_TargetPos.x && m_Type == 1 && m_bBoost)
	{
		m_bReverse = false;
	}
	
}

/**
* ダメージ処理
*/
void CEnemy::Damage(int dmg, bool bRev) {
	m_EneHP -= dmg;
	m_DamageWait = 30;
	if (bRev)
	{
		m_EneMove.x = -5.0f;
		m_bReverse = false;
	}
	else
	{
		m_EneMove.x = 5.0f;
		m_bReverse = true;
	}
	m_EneMotion.ChangeMotion(MOTION_DAMAGE);
	//ダメージエフェクト発生
	m_EneEffectManager->Start(m_EnePos.x + m_SrcRect.GetWidth() * 0.5f,
		m_EnePos.y + m_SrcRect.GetHeight() * 0.5f, EFC_DAMAGE);
}

void CEnemy::EnemyType0()
{	
	if (m_EneMotion.GetMotionNo() == MOTION_MOVE)
	{
		if (m_bBoost)
		{
			if (m_bReverse)
			{
				m_EneMove.x = -7.0f;
			}
			else
			{
				m_EneMove.x = 7.0f;
			}
		}
		else
		{
			if (m_bReverse)
			{
				m_EneMove.x = -3.0f;
			}
			else
			{
				m_EneMove.x = 3.0f;
			}
		}
	}
	if (m_EneMotion.GetMotionNo() == MOTION_DAMAGE)
	{
		//終了で待機に戻す
		if (m_EneMotion.IsEndMotion())
		{
			m_EneMotion.ChangeMotion(MOTION_MOVE);
			if (m_EneHP <= 0)
			{
				m_bShow = false;
				m_SE_Explosion.Play();
				//爆発エフェクト発生
				m_EneEffectManager->Start(m_EnePos.x + m_SrcRect.GetWidth() * 0.5f,
					m_EnePos.y + m_SrcRect.GetHeight() * 0.5f, EFC_EXPLOSIONENEMY);
			}
		}
		else
		{
			if (m_EneMove.x > 0)
			{
				m_EneMove.x -= 0.2f;
				if (m_EneMove.x <= 0)
				{
					m_EneMove.x = 0;
				}
			}
			else if (m_EneMove.x < 0)
			{
				m_EneMove.x += 0.2f;
				if (m_EneMove.x >= 0)
				{
					m_EneMove.x = 0;
				}
			}
		}
	}
}

void CEnemy::EnemyType1(CEnemyShot* shot,int smax)
{
	if (m_EneMotion.GetMotionNo() == MOTION_MOVE)
	{
		if (m_bReverse)
		{
			m_EneMove.x = 0.0f;
		}
		else
		{
			m_EneMove.x = 0.0f;
		}
	}
	if (m_bBoost && m_EneMotion.GetMotionNo() != MOTION_DAMAGE)
	{
		m_EneMotion.ChangeMotion(MOTION_SHOT);
	}
	if (m_EneMotion.GetMotionNo() == MOTION_SHOT)
	{
		if (m_EneMotion.IsEndMotion())
		{
			m_bBoost = false;
			m_EneMotion.ChangeMotion(MOTION_MOVE);
		}
		if (!m_bReverse)
		{
			if (m_ShotWait <= 0)
			{
				CEnemyShot* newShot = CEnemyShot::FindAvailableShot(shot, smax);
				//未使用の弾があった場合
				if (newShot)
				{
					m_ShotWait = m_ShotWaitSet;
					//目標地点に向かうための方向
					Vector2 direction = m_TargetPos - m_EnePos;
					//目標地点までの距離
					float distance = CVector2Utilities::Length(direction);
					//距離が0以下の場合は発射しない
					if (distance > 0)
					{
						//方向を正規化
						direction /= distance;
						newShot->Fire(m_EnePos, direction * 5);
						m_SE_Shot.Play();
					}
				}
			}
			else
			{
				m_ShotWait--;
			}
		}
		else
		{
			if (m_ShotWait <= 0)
			{
				CEnemyShot* newShot = CEnemyShot::FindAvailableShot(shot, smax);
				//未使用の弾があった場合
				if (newShot)
				{
					m_ShotWait = m_ShotWaitSet;
					//目標地点に向かうための方向
					Vector2 direction = m_TargetPos - m_EnePos;
					//目標地点までの距離
					float distance = CVector2Utilities::Length(direction);
					//距離が0以下の場合は発射しない
					if (distance > 0)
					{
						//方向を正規化
						direction /= distance;
						newShot->Fire(m_EnePos, direction * 5);
						m_SE_Shot.Play();
					}
				}
			}
			else
			{
				m_ShotWait--;
			}
		}
	}
	else if(m_EneMotion.GetMotionNo() == MOTION_DAMAGE)
	{
		//終了で待機に戻す
		if (m_EneMotion.IsEndMotion())
		{
			m_EneMotion.ChangeMotion(MOTION_MOVE);
			if (m_EneHP <= 0)
			{
				m_bShow = false;
				m_SE_Explosion.Play();
				//爆発エフェクト発生
				m_EneEffectManager->Start(m_EnePos.x + m_SrcRect.GetWidth() * 0.5f,
					m_EnePos.y + m_SrcRect.GetHeight() * 0.5f, EFC_EXPLOSIONENEMY);
			}
		}
		else
		{
			if (m_EneMove.x > 0)
			{
				m_EneMove.x -= 0.2f;
				if (m_EneMove.x <= 0)
				{
					m_EneMove.x = 0;
				}
			}
			else if (m_EneMove.x < 0)
			{
				m_EneMove.x += 0.2f;
				if (m_EneMove.x >= 0)
				{
					m_EneMove.x = 0;
				}
			}
		}
	}
	else
	{
		m_EneMotion.ChangeMotion(MOTION_MOVE);
	}
}

void CEnemy::EnemyType2()
{
	if (m_EneMotion.GetMotionNo() == MOTION_DAMAGE)
	{
		//終了で待機に戻す
		if (m_EneMotion.IsEndMotion())
		{
			m_EneMotion.ChangeMotion(MOTION_MOVE);
			if (m_EneHP <= 0)
			{
				m_SE_Explosion.Play();
				m_bShow = false;
				//爆発エフェクト発生
				m_EneEffectManager->Start(m_EnePos.x + m_SrcRect.GetWidth() * 0.5f,
					m_EnePos.y + m_SrcRect.GetHeight() * 0.5f, EFC_EXPLOSIONENEMY);
			}
			if (m_bReverse)
			{
				m_EneMove.x = -5.0f;
			}
			else
			{
				m_EneMove.x = 5.0f;
			}

		}
		else
		{
			if (m_EneMove.x > 0)
			{
				m_EneMove.x -= 0.2f;
				if (m_EneMove.x <= 0)
				{
					m_EneMove.x = 0;
				}
			}
			else if (m_EneMove.x < 0)
			{
				m_EneMove.x += 0.2f;
				if (m_EneMove.x >= 0)
				{
					m_EneMove.x = 0;
				}
			}
		}
	}
}

/**
* ステージとの当たり
* [in]		ox			X埋まり量
* [in]		oy			Y埋まり量
*/
void CEnemy::ColisionStage(float ox,float oy) {
	m_EnePos.x += ox;
	m_EnePos.y += oy;

	//落下中下埋まり、ジャンプ中上埋まりは移動を初期化
	if (oy < 0 && m_EneMove.y > 0)
	{
		m_EneMove.y = 0;
	}
	else if (oy > 0 && m_EneMove.y < 0)
	{
		m_EneMove.y = 0;
	}
	//左移動中の左埋まり、右移動中の右埋まりは移動を初期化
	if (ox < 0 && m_EneMove.x > 0)
	{
		if (m_EneMotion.GetMotionNo() == MOTION_DAMAGE)
		{
			m_EneMove.x = 0;
		}
		else
		{
			m_EneMove.x *= -1;
			m_bReverse = true;
		}
	}
	else if (ox > 0 && m_EneMove.x < 0)
	{
		if (m_EneMotion.GetMotionNo() == MOTION_DAMAGE)
		{
			m_EneMove.x = 0;
		}
		else
		{
			m_EneMove.x *= -1;
			m_bReverse = false;
		}
	}
}

/**
* 敵同士の衝突
*/
bool CEnemy::ColisionEnemy(CEnemy& ene) {
	if (!ene.GetShow())
	{
		return false;
	}
	//HPが0の時判定しない
	if (ene.m_EneHP <= 0)
	{
		return false;
	}
	
	//敵矩形同士で判定
	CRectangle erec = GetRect();
	CRectangle erectarget = ene.GetRect();

	if (erec.CollisionRect(erectarget))
	{
		if (m_bReverse)
		{
			m_bReverse = false;
			m_EneMove.x *= -1;
		}
		else
		{
			m_bReverse = true;
			m_EneMove.x *= -1;
		}
		return true;
	}

	return false;
}

/**
* 描画
*/
void CEnemy::Render(float wx,float wy) {
	//非表示
	if (!m_bShow)
	{
		return;
	}
	//インターバル2フレーム毎に描画しない
	if (m_DamageWait % 4 >= 2)
	{
		return;
	}

	//描画矩形
	CRectangle dr = m_SrcRect;

	//反転フラグがONの場合、矩形を反転
	if (m_bReverse)
	{
		float tmp = dr.Right;
		dr.Right = dr.Left;
		dr.Left = tmp;
	}
	//テクスチャ描画
	m_EneTexture->Render(m_EnePos.x - wx, m_EnePos.y - wy, dr);
}

/**
* デバッグ描画
*/
void CEnemy::RenderDebug(float wx, float wy) {
	//非表示
	if (!m_bShow)
	{
		return;
	}
	//当たり判定表示
	CRectangle hr = GetRect();
	CGraphicsUtilities::RenderRect(hr.Left - wx, hr.Top - wy, hr.Right - wx, hr.Bottom - wy, MOF_XRGB(255, 0, 0));

	//索敵範囲円表示
	m_EneSearchCircle = GetSearchEnemyCircle();
	m_EneSearchCircle.x -= wx;
	m_EneSearchCircle.y -= wy;
	CGraphicsUtilities::RenderCircle(m_EneSearchCircle, MOF_XRGB(255, 255, 255));
}

/**
* 解放
*/
void CEnemy::Release() {
	m_EneMotion.Release();
	m_SE_Shot.Release();
	m_SE_Explosion.Release();
}