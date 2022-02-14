#include "Game.h"
#include "GameDefine.h"

//変更するシーン(実体はGameApp)
extern int			gChangeScene;

//ステージ番号
extern int			gStageNo;

//敵弾
#define		ENEMYSHOT_COUNT		100
CEnemyShot	gShotArray[ENEMYSHOT_COUNT];
CTexture	gEnemyShotTexture;

//ボス弾
#define		BOSSSHOT_COUNT		100
CEnemyShot  gBossShotArray[BOSSSHOT_COUNT];
CTexture	gBossShotTexture;

//ステージ
char* Stage[3] = {"Stage.txt","Stage(仮2).txt","Tutorial(Move).txt"};

/**
* コンストラクタ
*/
CGame::CGame()
{
}

/**
* デストラクタ
*/
CGame::~CGame()
{
}

/**
* 読み込み
*/
bool CGame::Load() {
	//プレイヤー素材読み込み
	m_Player.Load();
	//ステージ読み込み
	m_Stage.Load(Stage[gStageNo]);
	//敵メモリ確保
	m_EnemyArray = new CEnemy[m_Stage.GetEnemyCount()];
	//敵弾読込
	if (!gEnemyShotTexture.Load("eneshot.png"))
	{
		return false;
	}
	//アイテムメモリ確保
	m_ItemArray = new CItem[m_Stage.GetItemCount()];
	//エフェクト素材読み込み
	m_EffectManager.Load();
	//ボス素材読み込み
	m_Boss.Load();
	//ボス弾読込
	if (!gBossShotTexture.Load("bossShot.png"))
	{
		return false;
	}
	return true;
}

/**
* 初期化
*/
void CGame::Initialize() {

	//プレイヤーの状態初期化
	m_Player.Initialize();
	//ステージ状態初期化
	m_Stage.Initialize(m_EnemyArray, m_ItemArray);
	//エフェクト状態初期化
	m_EffectManager.Initialize();
	//プレイヤーと敵にエフェクトクラス設定
	m_Player.SetEffectManager(&m_EffectManager);
	m_Boss.SetEffectManager(&m_EffectManager);
	for (int i = 0; i < m_Stage.GetEnemyCount(); i++)
	{
		m_EnemyArray[i].SetEffectManager(&m_EffectManager);
	}
	//敵弾初期化
	for (int i = 0; i < ENEMYSHOT_COUNT; i++)
	{
		gShotArray[i].Initialize();
		gShotArray[i].SetTexture(&gEnemyShotTexture);
	}
	//ボス状態初期化
	m_Boss.Initialize(m_Stage.GetBossPosX(),m_Stage.GetBossPosY());
	for (int i = 0; i < BOSSSHOT_COUNT; i++)
	{
		gBossShotArray[i].Initialize();
		gBossShotArray[i].SetTexture(&gBossShotTexture);
	}
}

/**
* 更新
*/
void CGame::Update() {

	//敵用射撃プレイヤー座標
	CRectangle prec = m_Player.GetRect();
	CVector2   ps = prec.GetCenter();

	//プレイヤー更新
	m_Player.Update();

	//ステージとプレイヤーの当たり判定
	float ox = 0, oy = 0;
	if (m_Stage.Colision(m_Player.GetRect(), ox, oy))
	{
		m_Player.ColisionStage(ox, oy);
	}

	//敵更新
	for (int i = 0; i < m_Stage.GetEnemyCount(); i++)
	{
		if (!m_EnemyArray[i].GetShow())
		{
			continue;
		}
		m_EnemyArray[i].SetTargetPos(ps);
		m_EnemyArray[i].Update(gShotArray,ENEMYSHOT_COUNT);
		float ox = 0; oy = 0;
		if (m_Stage.Colision(m_EnemyArray[i].GetRect(), ox, oy))
		{
			m_EnemyArray[i].ColisionStage(ox, oy);
		}
	}
	//ボス当たり判定
	float Box = 0, Boy = 0;
	if (m_Stage.Colision(m_Boss.GetRect(), Box, Boy))
	{
		m_Boss.ColisionStage(Box, Boy);
	}
	//敵弾更新
	for (int i = 0; i < ENEMYSHOT_COUNT; i++)
	{
		gShotArray[i].Update(m_Stage.GetScrollX(),m_Stage.GetScrollY());
	}
	//アイテムの更新
	for (int i = 0; i < m_Stage.GetItemCount(); i++)
	{
		if (!m_ItemArray[i].GetShow())
		{
			continue;
		}
		m_ItemArray[i].Update();
		float ox = 0; oy = 0;
		if (m_Stage.Colision(m_ItemArray[i].GetRect(), ox, oy))
		{
			m_ItemArray[i].ColisionStage(ox, oy);
		}
	}
	//当たり判定実行
	for (int i = 0; i < m_Stage.GetEnemyCount(); i++)
	{
		m_Player.ColisionEnemy(m_EnemyArray[i]);
		for (int j = 0; j <  m_Stage.GetEnemyCount(); j++)
		{
			if (i == j)
			{
				continue;
			}
			m_EnemyArray[i].ColisionEnemy(m_EnemyArray[j]);
		}
	}
	for (int i = 0; i < m_Stage.GetItemCount(); i++)
	{
		m_Player.ColisionItem(m_ItemArray[i]);
	}
	for (int i = 0; i < ENEMYSHOT_COUNT; i++)
	{
		m_Player.ColisionEnemyShot(gShotArray[i]);
	}
	m_Player.ColisionBoss(m_Boss);
	for (int i = 0; i < BOSSSHOT_COUNT; i++)
	{
		m_Player.ColisionBossShot(gBossShotArray[i]);
	}
	//ステージ更新
	m_Stage.Update(m_Player);
	//エフェクト更新
	m_EffectManager.Update();
	//ボス更新
	m_Boss.SetTargetPos(ps);
	m_Boss.Update(gBossShotArray,BOSSSHOT_COUNT);
	//ボス弾更新
	for (int i = 0; i < BOSSSHOT_COUNT; i++)
	{
		gBossShotArray[i].Update(m_Stage.GetScrollX(), m_Stage.GetScrollY());
	}
	
	//プレイヤー終了フラグでゲームオーバー画面へ
	if (m_Player.IsEnd())
	{
		gChangeScene = SCENENO_GAMEOVER;
	}
	//ボスを撃破でゲームクリア画面へ
	if (!m_Boss.GetShow())
	{
		if (m_Boss.IsClear())
		{
			gChangeScene = SCENENO_GAMECLEAR;
		}
	}
	//ゴールフラグでゲームクリア画面へ
	if (m_Player.IsGoal())
	{
		gChangeScene = SCENENO_GAMECLEAR;
	}
}

/**
* 描画
*/
void CGame::Render() {

	//ステージ描画
	m_Stage.Render();
	//プレイヤー描画
	m_Player.Render(m_Stage.GetScrollX(),m_Stage.GetScrollY());
	//敵描画
	for (int i = 0; i < m_Stage.GetEnemyCount(); i++)
	{
		m_EnemyArray[i].Render(m_Stage.GetScrollX(), m_Stage.GetScrollY());
	}
	//敵弾描画
	for (int i = 0; i < ENEMYSHOT_COUNT; i++)
	{
		gShotArray[i].Render(m_Stage.GetScrollX(),m_Stage.GetScrollY());
	}
	//アイテム描画
	for (int i = 0; i < m_Stage.GetItemCount(); i++)
	{
		m_ItemArray[i].Render(m_Stage.GetScrollX(), m_Stage.GetScrollY());
	}
	//エフェクト描画
	m_EffectManager.Render(m_Stage.GetScrollX(), m_Stage.GetScrollY());
	//プレイヤーの状態描画
	m_Player.RenderStatus();
	//ボス描画
	m_Boss.Render(m_Stage.GetScrollX(),m_Stage.GetScrollY());
	//ボス弾描画
	for (int i = 0; i < BOSSSHOT_COUNT; i++)
	{
		gBossShotArray[i].Render(m_Stage.GetScrollX(), m_Stage.GetScrollY());
	}
	if (gStageNo == STAGENO_TUTORIAL)
	{
		CGraphicsUtilities::RenderString(10, 100, "移動：A,D、ジャンプ：W、攻撃：マウス左、回避：マウス右、ガード：F", MOF_COLOR_WHITE);
		CGraphicsUtilities::RenderString(10, 130, "操作を確認出来たらアイテムを取ってステージを終了してください", MOF_COLOR_WHITE);
	}
}

/**
* デバッグ描画
*/
void CGame::RenderDebug() {

	//ステージデバッグ描画
	m_Stage.RenderDebug();
	//プレイヤーデバッグ描画
	m_Player.RenderDebug(m_Stage.GetScrollX(),m_Stage.GetScrollY());
	//敵デバッグ描画
	for (int i = 0; i < m_Stage.GetEnemyCount(); i++)
	{
		m_EnemyArray->RenderDebug(m_Stage.GetScrollX(), m_Stage.GetScrollY());
	}
	//アイテムデバッグ描画
	for (int i = 0; i < m_Stage.GetItemCount(); i++)
	{
		m_ItemArray->RenderDebug(m_Stage.GetScrollX(), m_Stage.GetScrollY());
	}
	//エフェクトデバッグ描画
	m_EffectManager.RenderDebug(m_Stage.GetScrollX(), m_Stage.GetScrollY());
	//ボスデバッグ描画
	m_Boss.RenderDebug(m_Stage.GetScrollX(), m_Stage.GetScrollY());
}

/**
* 解放
*/
void CGame::Release() {
	//ステージ解放
	m_Stage.Release();
	//プレイヤー解放
	m_Player.Release();
	//敵解放
	if (m_EnemyArray)
	{
		delete[] m_EnemyArray;
		m_EnemyArray = NULL;
	}
	//アイテム解放
	if (m_ItemArray)
	{
		delete[] m_ItemArray;
		m_ItemArray = NULL;
	}
	//エフェクト解放
	m_EffectManager.Release();
	//敵弾解放
	gEnemyShotTexture.Release();
	//ボス解放
	m_Boss.Release();
	gBossShotTexture.Release();
}