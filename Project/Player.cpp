#include "Player.h"

/**
* コンストラクタ
*/
CPlayer::CPlayer():
m_Texture(),
m_Motion(),
m_Pos(0.0f,0.0f),
m_bMove(false),
m_Move(0.0f,0.0f),
m_bJump(false),
m_bReverse(false),
m_SrcRect(),
m_HP(100),
m_DamageWait(0),
m_FrameTexture(),
m_HPTexture(),
m_DashWait(0),
m_PlayerEffectManager(),
m_bEnd(false),
m_AttackCnt(0),
m_JumpCnt(0),
m_bJump2(false),
m_pEndEffect()
{
}


/**
* デストラクタ
*/
CPlayer::~CPlayer()
{

}

/**
* 読み込み
*/
bool CPlayer::Load() {
	//テクスチャ読み込み
	if (!m_Texture.Load("Player.png"))
	{
		return false;
	}
	//HPテクスチャ読み込み
	if (!m_FrameTexture.Load("Frame.png"))
	{
		return false;
	}
	if (!m_HPTexture.Load("HP.png"))
	{
		return false;
	}
	//SE読み込み
	m_SE_Attack.Load("Player_Attack.mp3");
	m_SE_Damage.Load("Damage.mp3");
	m_SE_Guard.Load("Player_Guard.mp3");
	m_SE_Explosion.Load("Explosion.mp3");
	m_SE_Dash.Load("Player_Dash.mp3");
	m_SE_Heal.Load("ItemHeal.mp3");

	//アニメーション作成
	SpriteAnimationCreate anim[] = {
		{
			"待機",
			 0,0,
			 60,64,
			 TRUE,{{5,0,0},{5,1,0},{5,2,0},{5,3,0},{5,4,0},{5,5,0},{5,6,0},{5,7,0}}
		},

		{
			"移動",
			 0,70,
			 60,64,
			 TRUE,{{5,0,0},{5,1,0},{5,2,0},{5,3,0},{5,4,0},{5,5,0}}
		},

		{
			"ジャンプ開始",
			 0,140,
			 60,64,
			 FALSE,{{5,0,0},{5,1,0},{5,2,0},{5,3,0}}
		},

		{
			"ジャンプ終了",
			240,140,
			60,64,
			FALSE,{{2,0,0},{2,1,0}}
		},

		{
			"ダッシュ開始",
			180,280,
			60,64,
			FALSE,{{2,0,0},{8,1,0}}
		},

	    {
			"ダッシュ終了",
			300,280,
			60,64,
			FALSE,{{4,0,0},{4,1,0}}

	    },

		{
			"攻撃",
			0,420,
			90,64,
			FALSE,{{2,0,0},{2,1,0},{2,2,0},{2,3,0},{2,4,0},{2,5,0},{2,6,0}}
		},

		{
			"ダメージ",
			480,0,
			60,64,
			FALSE,{{20,0,0}}

		},

		{
			"ガード",
			110,630,
			60,64,
			FALSE,{{20,0,0}}
		},

		{
			"攻撃2",
			0,350,
			90,64,
			FALSE,{{2,0,0},{2,1,0},{2,2,0},{2,3,0},{2,4,0},{2,5,0},{2,6,0}}
		},
	};
	m_Motion.Create(anim, MOTION_COUNT);
	return true;
}

/**
* 初期化
*/
void CPlayer::Initialize() {
	m_Pos.x = 75;
	m_Pos.y = 608;
	m_bMove = false;
	m_Move.x = 0.0f;
	m_Move.y = 0.0f;
	m_bReverse = false;
	m_bJump = false;
	m_HP = 100;
	m_DamageWait = 0;
	m_bEnd = false;
	m_pEndEffect = NULL;
	m_Motion.ChangeMotion(MOTION_WAIT);
	m_DashWait = 0;
	m_AttackCnt = 0;
	m_bGoal = false;
}

/**
*更新
*/
void CPlayer::Update() {

	//HPがなくなった場合、爆発の終了を待機して終了
	if (m_HP <= 0)
	{
		if (!m_pEndEffect || !m_pEndEffect->GetShow())
		{
			m_bEnd = true;
		}
		return;
	}
	//移動フラグ、移動があったか保存
	m_bMove = false;

	//2段攻撃の判定
	if (m_Motion.GetMotionNo() == MOTION_ATTACK)
	{
		if (g_pInput->IsMouseKeyPush(MOFMOUSE_LBUTTON))
		{
			m_AttackCnt = 1;
		}
	}

	//攻撃中、着地の場合の動作
	if (m_Motion.GetMotionNo() == MOTION_ATTACK || m_Motion.GetMotionNo() == MOTION_ATTACK2 || 
		m_Motion.GetMotionNo() == MOTION_JUMPEND)
	{
		//終了で待機に戻す
		if (m_Motion.IsEndMotion())
		{
			if (m_AttackCnt == 1)
			{
				m_SE_Attack.Play();
				m_Motion.ChangeMotion(MOTION_ATTACK2);
			}
			else
			{
				m_Motion.ChangeMotion(MOTION_WAIT);
			}
			m_AttackCnt = 0;
		}
	}
	else if (m_Motion.GetMotionNo() == MOTION_DAMAGE)
	{
		//終了で待機に戻す
		if (m_Motion.IsEndMotion())
		{
			m_Motion.ChangeMotion(MOTION_WAIT);
		}
	}
	//ダッシュ中の動作
	else if (m_Motion.GetMotionNo() == MOTION_DASHSTART)
	{
		if (m_bReverse)
		{
			//エフェクト
			m_pDashEffect = m_PlayerEffectManager->Start(m_Pos.x + 50 + m_SrcRect.GetWidth() * 0.5f,
				m_Pos.y + m_SrcRect.GetHeight() * 0.5f, EFC_DASH_PLAYER);
		}
		else
		{
			//エフェクト
			m_pDashEffect = m_PlayerEffectManager->Start(m_Pos.x - 50 + m_SrcRect.GetWidth() * 0.5f,
				m_Pos.y + m_SrcRect.GetHeight() * 0.5f, EFC_DASH_PLAYER);
		}
		
		if (m_Motion.IsEndMotion())
		{
			m_Motion.ChangeMotion(MOTION_DASHEND);
		}
	}
	else if (m_Motion.GetMotionNo() == MOTION_DASHEND)
	{
		if (m_Motion.IsEndMotion())
		{
			m_Motion.ChangeMotion(MOTION_WAIT);
		}
	}
	//ガード中
	else if (m_Motion.GetMotionNo() == MOTION_GUARD)
	{
		//エフェクト
		m_pGuardEffect = m_PlayerEffectManager->Start(m_Pos.x + m_SrcRect.GetWidth() * 0.5f,
			m_Pos.y + m_SrcRect.GetHeight() * 0.5f, EFC_GUARD_PLAYER);
		if (!g_pInput->IsKeyHold(MOFKEY_F))
		{
			m_Motion.ChangeMotion(MOTION_WAIT);
		}
	}
	else
	{
		//キー入力による動作
		UpdateKey();
	}

	//移動更新
	UpdateMove();

	//実際に座標を移動
	m_Pos.x += m_Move.x;
	m_Pos.y += m_Move.y;

	//画面外で落下としてHPを0にする
	if (m_Pos.y  >= g_pGraphics->GetTargetHeight() - PLAYER_FALL && m_HP > 0)
	{
		m_HP = 0;
		m_SE_Explosion.Play();
		//爆発エフェクト発生
		m_pEndEffect = m_PlayerEffectManager->Start(m_Pos.x + m_SrcRect.GetWidth() * 0.5f,
			m_Pos.y + m_SrcRect.GetHeight() * 0.5f, EFC_EXPLOSIONPLAYER);
	}

	//アニメーション更新
	m_Motion.AddTimer(CUtilities::GetFrameSecond());
	m_SrcRect = m_Motion.GetSrcRect();

	//ダメージインターバル減少
	if (m_DamageWait > 0)
	{
		m_DamageWait--;
	}
	//ダッシュインターバル減少
	if (m_DashWait > 0)
	{
		m_DashWait--;
	}

	//地面に接触するとジャンプ終了
	if (m_Pos.y + m_SrcRect.GetHeight() >= g_pGraphics->GetTargetHeight())
	{
		m_Pos.y = g_pGraphics->GetTargetHeight() - m_SrcRect.GetHeight();
		m_Move.y = 0;

		if (m_bJump)
		{
			m_bJump = false;
			m_Motion.ChangeMotion(MOTION_JUMPEND);
		}
		if (m_bJump2)
		{
			m_bJump2 = false;
			m_Motion.ChangeMotion(MOTION_JUMPEND);
		}
	}
}

/**
* キー入力
*/
void CPlayer::UpdateKey() {
	//キーボード移動
	//入力では座標ではなく速度を変化させる
	if (g_pInput->IsKeyHold(MOFKEY_A))
	{
		m_Move.x -= PLAYER_SPEED;
		m_bReverse = true;
		m_bMove = true;

		if (m_Move.x < -PLAYER_MAXSPEED)
		{
			m_Move.x = -PLAYER_MAXSPEED;
		}
		if (m_Motion.GetMotionNo() == MOTION_WAIT)
		{
			m_Motion.ChangeMotion(MOTION_MOVE);
		}
	}
	else if (g_pInput->IsKeyHold(MOFKEY_D))
	{
		m_Move.x += PLAYER_SPEED;
		m_bReverse = false;
		m_bMove = true;

		if (m_Move.x > PLAYER_MAXSPEED)
		{
			m_Move.x = PLAYER_MAXSPEED;
		}
		if (m_Motion.GetMotionNo() == MOTION_WAIT)
		{
			m_Motion.ChangeMotion(MOTION_MOVE);
		}
	}

	//上キーでジャンプ
	if (g_pInput->IsKeyPush(MOFKEY_W) && !m_bJump && !m_bJump2)
	{
		m_JumpCnt = 1;
		m_bJump = true;
		m_Move.y = PLAYER_JUMP;
		m_Motion.ChangeMotion(MOTION_JUMPSTART);
	}
	else if (g_pInput->IsKeyPush(MOFKEY_W) && m_bJump && !m_bJump2)
	{
		if (m_JumpCnt == 1)
		{
			m_Move.y = PLAYER_JUMP;
			m_Motion.ChangeMotion(MOTION_JUMPSTART);
		}
		m_JumpCnt = 0;
	}

	//マウス左クリックキーで攻撃
	if (g_pInput->IsMouseKeyPush(MOFMOUSE_LBUTTON))
	{
		m_SE_Attack.Play();
		m_Motion.ChangeMotion(MOTION_ATTACK);
	}
	else
	{
		m_AttackCnt = 0;
	}


	//マウス右クリックでダッシュ
	if (g_pInput->IsMouseKeyPush(MOFMOUSE_RBUTTON) && !m_bJump)
	{
		m_bMove = true;
		if (m_DashWait > 0)
		{
			return;
		}
		if (!m_bReverse)
		{
			m_Move.x = PLAYER_DASHSPEED;
			m_DamageWait = 40;
			m_DashWait = 70;
		}
		else
		{
			m_Move.x = -PLAYER_DASHSPEED;
			m_DamageWait = 40;
			m_DashWait = 70;
		}
		m_SE_Dash.Play();
		m_Motion.ChangeMotion(MOTION_DASHSTART);
	}
	//Gキーでガード
	if (g_pInput->IsKeyHold(MOFKEY_F) && !m_bJump)
	{
		m_Motion.ChangeMotion(MOTION_GUARD);
	}
	
}

/**
* 移動更新
*/
void CPlayer::UpdateMove() {

	//フレームでの移動入力が無ければ減速処理を実行する
	if (!m_bMove)
	{
		if (m_Move.x > 0)
		{
			m_Move.x -= PLAYER_SPEED;

			if (m_Move.x <= 0)
			{
				m_Move.x = 0;
			}
		}
		else if (m_Move.x < 0)
		{
			m_Move.x += PLAYER_SPEED;

			if (m_Move.x >= 0)
			{
				m_Move.x = 0;
			}
		}
		else if (m_Motion.GetMotionNo() == MOTION_MOVE)
		{
			m_Motion.ChangeMotion(MOTION_WAIT);
		}
	}

	//重力作用
	m_Move.y += GRAVITY;
	if (m_Move.y >= 20.0f)
	{
		m_Move.y = 20.0f;
	}
}

/**
* ステージとの当たり判定
* [in]		ox			[X埋まり量]
* [in]		oy			[Y埋まり量]
*/
void CPlayer::ColisionStage(float ox, float oy) {
	m_Pos.x += ox;
	m_Pos.y += oy;

	//落下中の下埋まり、ジャンプ中の上埋まりの場合は移動を初期化
	if (oy < 0 && m_Move.y > 0)
	{
		m_Move.y = 0;
		if (m_bJump)
		{
			m_Motion.ChangeMotion(MOTION_JUMPEND);
			m_bJump = false;
		}
		if (m_bJump2)
		{
			m_Motion.ChangeMotion(MOTION_JUMPEND);
			m_bJump2 = false;
		}
	}
	else if (oy > 0 && m_Move.y < 0)
	{
		m_Move.y = 0;
	}

	//左移動中の左埋まり、右移動中の右埋まりの場合は移動を初期化
	if (ox < 0 && m_Move.x > 0)
	{
		m_Move.x = 0;
	}
	else if (ox > 0 && m_Move.x < 0)
	{
		m_Move.x = 0;
	}
}

/**
* 敵との当たり判定
* [in]     ene		判定を行う敵
*/
bool CPlayer::ColisionEnemy(CEnemy& ene) {
	if (!ene.GetShow())
	{
		return false;
	}
	//HPがなくなると当たり判定しない
	if (m_HP <= 0)
	{
		return false;
	}
	//ダメージ中は当たり判定を行わない
	if (m_DamageWait > 0 || ene.GetDamageWait() > 0)
	{
		return true;
	}

	//敵矩形とプレイヤー矩形でダメージ
	CRectangle prec = GetRect();
	CRectangle erec = ene.GetRect();

	if (prec.CollisionRect(erec) && m_Motion.GetMotionNo() != MOTION_GUARD)
	{
		m_HP -= 5;
		m_DamageWait = 60;
		if (prec.Left < erec.Left)
		{
			m_Move.x -= 5.0f;
			m_bReverse = false;
		}
		else
		{
			m_Move.x += 5.0f;
			m_bReverse = true;
		}
		m_Motion.ChangeMotion(MOTION_DAMAGE);

		if (m_HP <= 0)
		{
			m_SE_Explosion.Play();
			//爆発エフェクト発生
			m_pEndEffect = m_PlayerEffectManager->Start(
				m_Pos.x + m_SrcRect.GetWidth() * 0.5f,
				m_Pos.y + m_SrcRect.GetHeight() * 0.5f, EFC_EXPLOSIONPLAYER);
		}
		else
		{
			m_SE_Damage.Play();
			//ダメージエフェクト発生
			m_PlayerEffectManager->Start(m_Pos.x + m_SrcRect.GetWidth() * 0.5f,
				m_Pos.y + m_SrcRect.GetHeight()*0.5f, EFC_DAMAGE);
		}
		
		return true;
	}
	else if (prec.CollisionRect(erec) && m_Motion.GetMotionNo() == MOTION_GUARD)
	{
		prec = GetGuardRect();
		m_DamageWait = 30;
		m_HP -= 5 * DAMAGE_REDUCE;

		if (m_HP <= 0)
		{
			m_SE_Explosion.Play();
			//爆発エフェクト発生
			m_pEndEffect = m_PlayerEffectManager->Start(
				m_Pos.x + m_SrcRect.GetWidth() * 0.5f,
				m_Pos.y + m_SrcRect.GetHeight() * 0.5f, EFC_EXPLOSIONPLAYER);
		}
		else
		{
			m_SE_Guard.Play();
			//ダメージエフェクト発生
			m_PlayerEffectManager->Start(m_Pos.x + m_SrcRect.GetWidth() * 0.5f,
				m_Pos.y + m_SrcRect.GetHeight() * 0.5f, EFC_DAMAGE);
		}
		return true;
	}

	//敵の索敵範囲円との判定
	CCircle enecir = ene.GetSearchEnemyCircle();
	CCircle plcir = GetSearchPlayerCircle();
	if (plcir.CollisionCircle(enecir))
	{
		ene.OnBoost();
	}
	else
	{
		ene.OffBoost();
	}

	//攻撃中の場合攻撃との当たり判定を実行
	if (m_Motion.GetMotionNo() != MOTION_ATTACK && m_Motion.GetMotionNo() != MOTION_ATTACK2)
	{
		return false;
	}

	//敵矩形とプレイヤー矩形で敵にダメージ
	prec = GetAttackRect();
	if (prec.CollisionRect(erec))
	{
		ene.Damage(10, m_bReverse);
		return true;
	}
	return false;
}

/**
* ボスとの当たり判定
* [in]	boss 判定するボス
*/
bool CPlayer::ColisionBoss(CBoss& boss) {
	if (!boss.GetShow())
	{
		return true;
	}
	if (m_HP <= 0)
	{
		return false;
	}
	//ダメージ中のため当たり判定を行わない
	if (m_DamageWait > 0 || boss.GetDamageWait() > 0)
	{
		return false;
	}

	//ボスの矩形と自分の矩形でダメージ
	CRectangle prec = GetRect();
	CRectangle brec = boss.GetRect();

	if (prec.CollisionRect(brec) && m_Motion.GetMotionNo() != MOTION_GUARD)
	{
		m_HP -= 10;
		m_DamageWait = 60;
		if (m_HP <= 0)
		{
			m_SE_Explosion.Play();
			//爆発エフェクト発生
			m_pEndEffect = m_PlayerEffectManager->Start(
				m_Pos.x + m_SrcRect.GetWidth() * 0.5f,
				m_Pos.y + m_SrcRect.GetHeight() * 0.5f, EFC_EXPLOSIONPLAYER);
		}
		else
		{
			m_SE_Damage.Play();
			//ダメージエフェクト発生
			m_PlayerEffectManager->Start(m_Pos.x + m_SrcRect.GetWidth() * 0.5f,
				m_Pos.y + m_SrcRect.GetHeight() * 0.5f, EFC_DAMAGE);
		}
		if (prec.Left < brec.Left)
		{
			m_Move.x = -5.0f;
			m_bReverse = false;
		}
		else
		{
			m_Move.x = 5.0f;
			m_bReverse = true;
		}
		
		m_Motion.ChangeMotion(MOTION_DAMAGE);
		return true;
	}
	else if (prec.CollisionRect(brec) && m_Motion.GetMotionNo() == MOTION_GUARD)
	{
		m_HP -= 10 * DAMAGE_REDUCE;
		m_DamageWait = 30;

		if (m_HP <= 0)
		{
			m_SE_Explosion.Play();
			//爆発エフェクト発生
			m_pEndEffect = m_PlayerEffectManager->Start(
				m_Pos.x + m_SrcRect.GetWidth() * 0.5f,
				m_Pos.y + m_SrcRect.GetHeight() * 0.5f, EFC_EXPLOSIONPLAYER);
		}
		else
		{
			m_SE_Guard.Play();
			//ダメージエフェクト発生
			m_PlayerEffectManager->Start(m_Pos.x + m_SrcRect.GetWidth() * 0.5f,
				m_Pos.y + m_SrcRect.GetHeight() * 0.5f, EFC_DAMAGE);
		}
		return true;
	}

	//ボスの判定円と接触判定
	CCircle bscir(boss.GetBossCircle());
	CCircle bsshotcir(boss.GetBossShotCircle());
	CCircle plcir = GetSearchPlayerCircle();

	if (plcir.CollisionCircle(bscir) && plcir.CollisionCircle(bsshotcir))
	{
		boss.OnRush();
		boss.OffShot();
	}
	else if (plcir.CollisionCircle(bsshotcir))
	{
		boss.OnShot();
	}
	else
	{
		boss.OffRush();
		boss.OffShot();
	}

	//攻撃中の場合のみ攻撃との当たり判定を実行
	if (m_Motion.GetMotionNo() != MOTION_ATTACK && m_Motion.GetMotionNo() != MOTION_ATTACK2)
	{
		return false;
	}

	//ボス矩形とプレイヤー矩形で敵にダメージ
	prec = GetAttackRect();
	if (prec.CollisionRect(brec))
	{
		boss.Damage(10, m_bReverse);
		return true;
	}
	return false;
}

/**
* 当たり判定(アイテム)
* 引数のアイテムに対して矩形同士の当たり判定を実行する
* 当たっていた場合はプレイヤーの状態を変更
* 
* [in]		itm		判定を行うアイテム
*/
bool CPlayer::ColisionItem(CItem& itm) {
	if (!itm.GetShow())
	{
		return false;
	}

	//アイテム矩形とプレイヤー矩形で当たり判定
	CRectangle prec = GetRect();
	CRectangle irec = itm.GetRect();
	if (prec.CollisionRect(irec))
	{
		itm.SetShow(false);
		switch (itm.GetType())
		{
			case ITEM_RECOVER:
				m_SE_Heal.Play();
				m_HP += 30;
				if (m_HP >= 100)
				{
					m_HP = 100;
				}
				break;

			case ITEM_GOAL:
				m_bGoal = true;
				break;
		}
		return true;
	}
	return false;
}

/**
* 敵弾との当たり判定
*
* 引数
* [in]		shot		判定を行う敵弾
*/
void CPlayer::ColisionEnemyShot(CEnemyShot& shot) {
	CRectangle pr = GetRect();
	CRectangle sr = shot.GetRect();
	if (!shot.GetShow())
	{
		return;
	}
	//ダメージインターバル
	if (m_DamageWait > 0)
	{
		return;
	}

	if (sr.CollisionRect(pr) && m_Motion.GetMotionNo() != MOTION_GUARD)
	{
		m_HP -= 10;
		shot.SetShow(false);
		m_DamageWait = 60;
		

		if (pr.Left < sr.Left)
		{
			m_Move.x -= 5;
		}
		else
		{
			m_Move.x = 5;
		}
		
		m_Motion.ChangeMotion(MOTION_DAMAGE);

		if (m_HP <= 0)
		{
			m_SE_Explosion.Play();
			//爆発エフェクト発生
			m_pEndEffect = m_PlayerEffectManager->Start(
				m_Pos.x + m_SrcRect.GetWidth() * 0.5f,
				m_Pos.y + m_SrcRect.GetHeight() * 0.5f, EFC_EXPLOSIONPLAYER);
		}
		else
		{
			m_SE_Damage.Play();
			//ダメージエフェクト発生
			m_PlayerEffectManager->Start(m_Pos.x + m_SrcRect.GetWidth() * 0.5f,
				m_Pos.y + m_SrcRect.GetHeight() * 0.5f, EFC_DAMAGE);
		}
	}
	else if (sr.CollisionRect(pr) && m_Motion.GetMotionNo() == MOTION_GUARD)
	{
		m_HP -= 10 * DAMAGE_REDUCE;
		shot.SetShow(false);
		m_DamageWait = 30;
		

		if (m_HP <= 0)
		{
			m_SE_Explosion.Play();
			//爆発エフェクト発生
			m_pEndEffect = m_PlayerEffectManager->Start(
				m_Pos.x + m_SrcRect.GetWidth() * 0.5f,
				m_Pos.y + m_SrcRect.GetHeight() * 0.5f, EFC_EXPLOSIONPLAYER);
		}
		else
		{
			m_SE_Guard.Play();
			//ダメージエフェクト発生
			m_PlayerEffectManager->Start(m_Pos.x + m_SrcRect.GetWidth() * 0.5f,
				m_Pos.y + m_SrcRect.GetHeight() * 0.5f, EFC_DAMAGE);
		}
	}
}

/**
* ボス弾との当たり判定
*/
void CPlayer::ColisionBossShot(CEnemyShot& shot) {
	
	//プレイヤー矩形
	CRectangle pr = GetRect();
	//ボス弾矩形
	CRectangle bsr = shot.GetRect();

	if (!shot.GetShow())
	{
		return;
	}
	//ダメージインターバル
	if (m_DamageWait > 0)
	{
		return;
	}
	if (pr.CollisionRect(bsr) && m_Motion.GetMotionNo() != MOTION_GUARD)
	{
		m_HP -= 10;
		shot.SetShow(false);
		m_DamageWait = 60;
		if (m_HP <= 0)
		{
			m_SE_Explosion.Play();
			//爆発エフェクト発生
			m_pEndEffect = m_PlayerEffectManager->Start(
				m_Pos.x + m_SrcRect.GetWidth() * 0.5f,
				m_Pos.y + m_SrcRect.GetHeight() * 0.5f, EFC_EXPLOSIONPLAYER);
		}
		else
		{
			m_SE_Damage.Play();
			//ダメージエフェクト発生
			m_PlayerEffectManager->Start(m_Pos.x + m_SrcRect.GetWidth() * 0.5f,
				m_Pos.y + m_SrcRect.GetHeight() * 0.5f, EFC_DAMAGE);
		}
		if (pr.Left < bsr.Left)
		{
			m_Move.x -= 5;
		}
		else
		{
			m_Move.x += 5;
		}
		m_Motion.ChangeMotion(MOTION_DAMAGE);
	}
	else if (pr.CollisionRect(bsr) && m_Motion.GetMotionNo() == MOTION_GUARD)
	{
		m_HP -= 10 * DAMAGE_REDUCE;
		m_DamageWait = 30;
		shot.SetShow(false);

		if (m_HP <= 0)
		{
			m_SE_Explosion.Play();
			//爆発エフェクト発生
			m_pEndEffect = m_PlayerEffectManager->Start(
				m_Pos.x + m_SrcRect.GetWidth() * 0.5f,
				m_Pos.y + m_SrcRect.GetHeight() * 0.5f, EFC_EXPLOSIONPLAYER);
		}
		else
		{
			m_SE_Guard.Play();
			//ダメージエフェクト発生
			m_PlayerEffectManager->Start(m_Pos.x + m_SrcRect.GetWidth() * 0.5f,
				m_Pos.y + m_SrcRect.GetHeight() * 0.5f, EFC_DAMAGE);
		}
	}
}

/**
* 描画
*/
void CPlayer::Render(float wx, float wy) {

	//インターバル2フレームごとに描画しない
	if (m_DamageWait % 4 >= 2)
	{
		return;
	}
	//HPが0の場合描画しない
	if (m_HP <= 0)
	{
		return;
	}

	//描画矩形
	CRectangle dr = m_SrcRect;

	//描画位置
	float px = m_Pos.x - wx;
	float py = m_Pos.y - wy;

	//反転フラグがtrueの場合描画矩形を反転
	if (m_bReverse)
	{
		float tmp = dr.Right;
		dr.Right = dr.Left;
		dr.Left = tmp;

		if (m_Motion.GetMotionNo() == MOTION_ATTACK || m_Motion.GetMotionNo() == MOTION_ATTACK2)
		{
			px -= PLAYER_ATTACKWIDTH;
		}
	}
	
	//テクスチャ描画
	m_Texture.Render(px, py, dr);
}

/**
* ステータス描画
*/
void CPlayer::RenderStatus() {
	//HPに応じて矩形幅を変化
	CRectangle rec(0, 0, 532 * (m_HP*0.01f), 64);
	m_HPTexture.Render(248, 20, rec);

	//フレームを上部に描画
	m_FrameTexture.Render(0, 0);
}

/**
* デバッグ描画
*/
void CPlayer::RenderDebug(float wx, float wy) {
	//位置の描画
	CGraphicsUtilities::RenderString(10, 70, "プレイヤーの位置 X : %.0f,Y:%.0f", m_Pos.x, m_Pos.y);
	//当たり判定
	CRectangle hr = GetRect();
	CGraphicsUtilities::RenderRect(hr.Left - wx, hr.Top - wy, hr.Right - wx, hr.Bottom - wy, MOF_XRGB(0, 255, 0));
	//攻撃の当たり判定表示
	if (IsAttack())
	{
		CRectangle hr = GetAttackRect();
		CGraphicsUtilities::RenderRect(hr.Left - wx, hr.Top - wy, hr.Right - wx, hr.Bottom - wy, MOF_XRGB(255, 0, 0));
	}
	//ガードの当たり判定表示
	if (IsGuard())
	{
		CRectangle hr = GetGuardRect();
		CGraphicsUtilities::RenderRect(hr.Left -wx, hr.Top - wy, hr.Right - wx, hr.Bottom - wy, MOF_XRGB(255, 0, 0));
	}

	CCircle pc = GetSearchPlayerCircle();
	pc.x -= wx;
	pc.y -= wy;

	CGraphicsUtilities::RenderCircle(pc, MOF_XRGB(0, 0, 255));
}

/**
* 解放
*/
void CPlayer::Release() {
	m_Texture.Release();
	m_Motion.Release();
	m_FrameTexture.Release();
	m_HPTexture.Release();
	m_SE_Attack.Release();
	m_SE_Damage.Release();
	m_SE_Guard.Release();
	m_SE_Explosion.Release();
	m_SE_Dash.Release();
	m_SE_Heal.Release();
}