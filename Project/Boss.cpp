#include "Boss.h"

CBoss::CBoss() :
m_BossTexture(),
m_BossMotion(),
m_BossPos(0, 0),
m_BossMove(0, 0),
m_BossHP(0),
m_bShow(false),
m_bReverse(false),
m_bClear(false),
m_SrcRect(),
m_ShotWait(0),
m_ShotWaitSet(0),
m_DamageWait(0),
m_TargetPos(0, 0),
m_ShotCnt(0) {
}

CBoss::~CBoss() {

}

bool CBoss::Load() {
	//ボス画像読込
	if (!m_BossTexture.Load("Boss.png"))
	{
		return false;
	}

	//アニメーション作成
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
	m_BossMotion.Create(anim, MOTION_COUNT);
	return true;
}

void CBoss::Initialize(float px, float py) {
	m_BossPos.x = px;
	m_BossPos.y = py;
	m_BossMove.x = 0.0f;
	m_BossMove.y = 0.0f;
	m_bShow = true;
	m_bReverse = true;
	m_ShotWait = 0;
	m_ShotWaitSet = 100;
	m_DamageWait = 80;
	m_BossHP = 100;
	m_TargetPos = CVector2(0, 0);
	m_ShotCnt = 0;
	m_RushCnt = 0;
	m_bClear = false;
	m_pEndEffect = NULL;
	m_SE_Shot.Load("SE_Shot.mp3");
	m_SE_Explosion.Load("Explosion_Boss.mp3");
}

void CBoss::Update(CEnemyShot* shot, int smax) {
	//非表示
	if (!GetShow())
	{
		return;
	}

	//重力によって下がる
	m_BossMove.y += GRAVITY;
	if (m_BossMove.y >= 20.0f)
	{
		m_BossMove.y = 20.0f;
	}

	//実際に座標を変化
	m_BossPos.x += m_BossMove.x;
	m_BossPos.y += m_BossMove.y;

	//HPがなくなった場合、爆発の終了を待機して終了(Player.cppのUpdate参照)
	if (m_BossHP <= 0)
	{
		m_bClear = true;
	}

	//HP残量でボスパターン変化
	if (m_BossHP > 0)
	{
		if (m_BossHP >= 80)
		{
			BossPattern1(shot, smax);
		}
		else if (m_BossHP < 80 && m_BossHP > 50)
		{
			BossPattern2(shot, smax);
		}
		else if (m_BossHP <= 50 && m_BossHP > 30)
		{
			BossPattern3(shot, smax);
		}
		else if (m_BossHP <= 30)
		{
			BossPattern4(shot, smax);
		}
	}

	if (m_BossPos.x > m_TargetPos.x && m_Shot)
	{
		m_bReverse = true;
	}
	else if (m_BossPos.x < m_TargetPos.x && m_Shot)
	{
		m_bReverse = false;
	}

	//ダメージ中動作
	if (m_BossMotion.GetMotionNo() == MOTION_DAMAGE)
	{
		//終了で待機状態に戻す
		if (m_BossMotion.IsEndMotion())
		{
			m_BossMotion.ChangeMotion(MOTION_MOVE);
			if (m_BossHP <= 0)
			{
				m_bShow = false;
				//爆発エフェクトを発生させる
				m_bEffectManager->Start(m_BossPos.x + m_SrcRect.GetWidth() * 0.5f,
					m_BossPos.y + m_SrcRect.GetHeight() * 0.5f, EFC_EXPLOSIONENEMY);
				//爆発SE
				m_SE_Explosion.Play();
			}
			m_BossMove.x = ((m_bReverse) ? -3.0f : 3.0f);
		}
		else
		{
			if (m_BossMove.x > 0)
			{
				m_BossMove.x -= 0.2f;
				if (m_BossMove.x <= 0)
				{
					m_BossMove.x = 0;
				}
			}
			else if (m_BossMove.x < 0)
			{
				m_BossMove.x += 0.2f;
				if (m_BossMove.x >= 0)
				{
					m_BossMove.x = 0;
				}
			}
		}
	}

	//アニメーション更新
	m_BossMotion.AddTimer(CUtilities::GetFrameSecond());
	m_SrcRect = m_BossMotion.GetSrcRect();

	//ダメージインターバル減少
	if (m_DamageWait > 0)
	{
		m_DamageWait--;
	}
}

/**
* ダメージ処理
*/
void CBoss::Damage(int dmg, bool bRev) {
	m_BossHP -= dmg;
	m_DamageWait = 60;
	if (bRev)
	{
		m_BossMove.x = -3.0f;
		m_bReverse = false;
	}
	else
	{
		m_BossMove.x = 3.0f;
		m_bReverse = true;
	}
	m_BossMotion.ChangeMotion(MOTION_DAMAGE);
	//ダメージエフェクトを発生させる
	m_bEffectManager->Start(m_BossPos.x + m_SrcRect.GetWidth() * 0.5f,
		m_BossPos.y + m_SrcRect.GetHeight() * 0.5f, EFC_DAMAGE);
}

/**
* ステージとの当たり判定
* [in]		ox		X埋まり量
* [in]		oy		Y埋まり量
*/
void CBoss::ColisionStage(float ox, float oy) {
	m_BossPos.x += ox;
	m_BossPos.y += oy;

	//落下中下埋まり、ジャンプ中上埋まりは移動を初期化
	if (oy < 0 && m_BossMove.y > 0)
	{
		if (m_RushCnt > 70)
		{
			m_RushDes = false;
		}
		m_BossMove.y = 0;
	}
	else if (oy > 0 && m_BossMove.y < 0)
	{
		m_BossMove.y = 0;
	}
	//左移動中左埋まり、右移動中右埋まりは移動初期化
	if (ox < 0 && m_BossMove.x > 0)
	{
		m_RushDes = false;
		if (m_BossMotion.GetMotionNo() == MOTION_DAMAGE)
		{
			m_BossMove.x = 0;
		}
		else
		{
			m_BossMove.x *= -1;
			m_bReverse = true;
		}
	}
	else if (ox > 0 && m_BossMove.x < 0)
	{
		m_RushDes = false;
		if (m_BossMotion.GetMotionNo() == MOTION_DAMAGE)
		{
			m_BossMove.x = 0;
		}
		else
		{
			m_BossMove.x *= -1;
			m_bReverse = false;
		}
	}
}

/**
* 描画
*/
void CBoss::Render(float wx, float wy) {
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
	CRectangle br = m_SrcRect;

	//描画位置
	float bx = m_BossPos.x - wx;
	float by = m_BossPos.y - wy;

	//反転フラグONの場合、矩形を反転
	if (m_bReverse)
	{
		float tmp = br.Right;
		br.Right = br.Left;
		br.Left = tmp;
	}

	//テクスチャ描画
	m_BossTexture.RenderScale(bx, by, 2, br);
	
	//ボスHPUI表示
	if (m_BossHP >= 80)
	{
		CGraphicsUtilities::RenderFillCircle(bx + 50, by, 10, MOF_XRGB(0, 0, 255));
	}
	else if (m_BossHP < 80 && m_BossHP > 50)
	{
		CGraphicsUtilities::RenderFillCircle(bx + 50, by, 10, MOF_XRGB(0, 255, 0));
	}
	else if (m_BossHP <= 50 && m_BossHP > 30)
	{
		CGraphicsUtilities::RenderFillCircle(bx + 50, by, 10, MOF_XRGB(255, 255, 0));
	}
	else
	{
		CGraphicsUtilities::RenderFillCircle(bx + 50, by, 10, MOF_XRGB(255, 0, 0));
	}
}

/**
* デバッグ描画
*/
void CBoss::RenderDebug(float wx, float wy) {

	//非表示
	if (!m_bShow)
	{
		return;
	}
	//当たり判定描画
	CRectangle hr = GetRect();
	CGraphicsUtilities::RenderRect(hr.Left - wx, hr.Top - wy, hr.Right - wx, hr.Bottom - wy, MOF_XRGB(255, 0, 0));

	//判定円描画
	m_BossSerchCircle = GetBossCircle();
	m_BossSerchCircle.x -= wx;
	m_BossSerchCircle.y -= wy;
	CGraphicsUtilities::RenderCircle(m_BossSerchCircle, MOF_XRGB(255, 255, 255));

	m_BossShotCircle = GetBossShotCircle();
	m_BossShotCircle.x -= wx;
	m_BossShotCircle.y -= wy;
	CGraphicsUtilities::RenderCircle(m_BossShotCircle, MOF_XRGB(255, 255, 255));
}

/**
* 解放
*/
void CBoss::Release() {
	m_BossTexture.Release();
	m_SE_Shot.Release();
	m_SE_Explosion.Release();
}

void CBoss::BossPattern1(CEnemyShot* shot, int smax) {

	if (m_BossMotion.GetMotionNo() == MOTION_MOVE)
	{
		if (m_Rush)
		{
			if (m_bReverse)
			{
				m_BossMove.x = -5.0f;
			}
			else
			{
				m_BossMove.x = 5.0f;
			}
		}
		else
		{
			m_BossMove.x = 0.0f;
		}

	}

	if (m_Shot && m_BossMotion.GetMotionNo() != MOTION_DAMAGE)
	{
		m_BossMotion.ChangeMotion(MOTION_SHOT);
	}
	if (m_BossMotion.GetMotionNo() == MOTION_SHOT)
	{
		//移動停止
		m_BossMove.x = 0.0f;
		if (m_BossMotion.IsEndMotion())
		{
			m_Shot = false;
			m_BossMotion.ChangeMotion(MOTION_MOVE);
		}
		if (!m_bReverse)
		{
			BossShot(shot, smax, m_ShotWaitSet, 1);
		}
		else
		{
			BossShot(shot, smax, m_ShotWaitSet, 1);
		}
	}
}

void CBoss::BossPattern2(CEnemyShot* shot, int smax) {

	if (m_RushDes)
	{
		if (m_RushCnt <= 70)
		{
			m_BossMove.x = 0.0f;
			m_BossMove.y = -3.0f;
			m_RushCnt++;
			if (m_RushCnt > 70)
			{
				//プレイヤーの方向に突っ込んでくるようにする
				CVector2 direction = m_TargetPos - m_BossPos;
				float distance = CVector2Utilities::Length(direction);
				direction /= distance;

				if (m_bReverse)
				{	
					m_BossMove = direction * 10;
					m_BossMove.x = -10.0f;				
				}
				else
				{
					m_BossMove = direction * 10;
					m_BossMove.x = 10.0f;
				}
			}
		}
		return;
	}
	if (m_BossMotion.GetMotionNo() == MOTION_MOVE)
	{
		if (m_Rush)
		{
			m_RushCnt = 0;
			m_RushDes = true;
		}
		else
		{
			m_BossMove.x = 0.0f;
		}
	}

	if (m_Shot && m_BossMotion.GetMotionNo() != MOTION_DAMAGE)
	{
		m_BossMotion.ChangeMotion(MOTION_SHOT);
	}
	if (m_BossMotion.GetMotionNo() == MOTION_SHOT)
	{
		//移動停止
		m_BossMove.x = 0.0f;
		if (m_BossMotion.IsEndMotion())
		{
			m_Shot = false;
			m_BossMotion.ChangeMotion(MOTION_MOVE);
		}
		if (!m_bReverse)
		{
			BossShot(shot, smax, m_ShotCnt % 2 != 0 ? 5 : 40, 1);
		}
		else
		{
			BossShot(shot, smax, m_ShotCnt % 2 != 0 ? 5 : 40, 1);
		}
	}
}

void CBoss::BossPattern3(CEnemyShot* shot, int smax) {

	if (m_BossMotion.GetMotionNo() == MOTION_MOVE)
	{
		if (m_Rush)
		{
			if (m_bReverse)
			{
				m_BossMove.x = -4.0f;
				m_BossMove.y = -5.0f;
			}
			else
			{
				m_BossMove.x = 4.0f;
				m_BossMove.y = -5.0f;
			}
		}
		else
		{
			m_BossMove.x = 0.0f;
		}
	}

	if (m_Shot && m_BossMotion.GetMotionNo() != MOTION_DAMAGE)
	{
		m_BossMotion.ChangeMotion(MOTION_SHOT);
	}
	if (m_BossMotion.GetMotionNo() == MOTION_SHOT)
	{
		//移動停止
		m_BossMove.x = 0.0f;
		if (m_BossMotion.IsEndMotion())
		{
			m_Shot = false;
			m_BossMotion.ChangeMotion(MOTION_MOVE);
		}
		if (!m_bReverse)
		{
			BossShot(shot, smax, m_ShotCnt % 3 != 0 ? 5 : 40, 1);
		}
		else
		{
			BossShot(shot, smax, m_ShotCnt % 3 != 0 ? 5 : 40, 1);
		}
	}
}

void CBoss::BossPattern4(CEnemyShot* shot, int smax)
{
	if (m_BossMotion.GetMotionNo() == MOTION_MOVE)
	{
		if (m_Rush)
		{
			if (m_bReverse)
			{
				m_BossMove.x = -5.0f;
				m_BossMove.y = -3.0f;
			}
			else
			{
				m_BossMove.x = 5.0f;
				m_BossMove.y = -3.0f;
			}

		}
		else
		{
			m_BossMove.x = 0.0f;
		}

	}

	if (m_Shot && m_BossMotion.GetMotionNo() != MOTION_DAMAGE)
	{
		m_BossMotion.ChangeMotion(MOTION_SHOT);
	}
	if (m_BossMotion.GetMotionNo() == MOTION_SHOT)
	{
		//移動停止
		m_BossMove.x = 0.0f;
		if (m_BossMotion.IsEndMotion())
		{
			m_Shot = false;
			m_BossMotion.ChangeMotion(MOTION_MOVE);
		}
		if (!m_bReverse)
		{
			BossShot(shot, smax, m_ShotCnt % 3 != 0 ? 5 : 75, 3);

		}
		else
		{
			BossShot(shot, smax, m_ShotCnt % 3 != 0 ? 5 : 75, 3);
		}
	}
}

void CBoss::BossShot(CEnemyShot* shot, int smax, int wait, int scnt) {
	if (m_ShotWait > 0)
	{
		m_ShotWait--;
		return;
	}
	m_ShotWaitSet = wait;
	//目標地点に向かうための方向
	CVector2 direction = m_TargetPos - m_BossPos;
	//目標地点までの距離
	float distance = CVector2Utilities::Length(direction);
	//距離が0以下の場合は発射しない
	if (distance <= 0)
	{
		return;
	}
	m_ShotCnt++;
	m_ShotWait = m_ShotWaitSet;
	//方向を正規化
	direction /= distance;
	
	//弾の加算角度
	float shotAngle = 20.0f;
	//プレイヤー方向への角度
	float dir = atan2(direction.y, direction.x);
	//同時発射数
	int sCnt = scnt;
	//中心の弾がプレイヤーの中心にいくように初期角度を調整
	float ad = MOF_ToRadian(-shotAngle * (sCnt * 0.5f - 0.5f));

	for (int cnt = 0; cnt < sCnt; cnt++)
	{
		//未使用の弾を探索
		CEnemyShot* newShot = CEnemyShot::FindAvailableShot(shot, smax);
		if (!newShot)
		{
			return;
		}

		direction.x = cos(dir + ad);
		direction.y = sin(dir + ad);
		//未使用の弾があった場合
		newShot->Fire(m_BossPos, direction * 5);
		m_SE_Shot.Play();
		ad += MOF_ToRadian(shotAngle);
	}
}