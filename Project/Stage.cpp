#include "Stage.h"
#include "GameDefine.h"

//ステージ番号
extern int		gStageNo;

/**
* コンストラクタ
*/
CStage::CStage() :
m_ChipTexture(),
m_BackTexture(),
m_ChipSize(0),
m_XCount(0),
m_YCount(0),
m_pChipData(NULL),
m_Scroll(0, 0),
m_EnemyTextureCount(0),
m_pEnemyTexture(NULL),
m_EnemyCount(0),
m_ItemTextureCount(0),
m_pItemTexture(NULL),
m_ItemCount(0){

}

/**
* デストラクタ
*/
CStage::~CStage() {
}

/**
* 読み込み
*
* [in]		pName		ステージファイル名
*/
bool CStage::Load(char* pName) {

	
	if(gStageNo >= STAGENO_TUTORIAL)
	{
		//テキストファイルを開く
		FILE* fp = fopen(pName, "rt");
	
		if (fp == NULL)
		{
			return false;
		}
	
		//ファイルの全容量を調べる
		fseek(fp, 0, SEEK_END);
		long fSize = ftell(fp);
		fseek(fp, 0, SEEK_SET);
	
		//ファイルサイズ分だけのメモリを確保する
		char* pBuffer = (char*)malloc(fSize + 1);
	
		//ファイルを全てバッファに読み込む
		fSize = fread(pBuffer, 1, fSize, fp);
		pBuffer[fSize] = '\0';
		char* pstr;
	
		//テクスチャの読み込み
		pstr = strtok(pBuffer, ",");
		if (!m_BackTexture.Load(pstr))
		{
			return false;
		}
		pstr = strtok(NULL, ",");
		if (!m_ChipTexture.Load(pstr))
		{
			return false;
		}
	
		//チップサイズ
		pstr = strtok(NULL, ",");
		m_ChipSize = atof(pstr);
	
		//マップのチップ数
		pstr = strtok(NULL, ",");
		m_XCount = atoi(pstr);
		pstr = strtok(NULL, ",");
		m_YCount = atoi(pstr);
	
		//マップチップ用のメモリ確保
		m_pChipData = (char*)malloc(m_XCount * m_YCount);
	
		//チップデータの読み込み
		for (int y = 0; y < m_YCount; y++)
		{
			for (int x = 0; x < m_XCount; x++)
			{
				pstr = strtok(NULL, ",");
				m_pChipData[y * m_XCount + x] = atoi(pstr);
			}
		}
	
		//敵テクスチャ読み込み
		pstr = strtok(NULL, ",");
		m_EnemyTextureCount = atoi(pstr);
		m_pEnemyTexture = new CTexture[m_EnemyTextureCount];
		for (int i = 0; i < m_EnemyTextureCount; i++)
		{
			pstr = strtok(NULL, ",");
	
			if (!m_pEnemyTexture[i].Load(pstr))
			{
				return false;
			}
		}
	
		//配置データ読み込み
		m_pEnemyData = (char*)malloc(m_XCount * m_YCount);
		m_EnemyCount = 0;
		for (int y = 0; y < m_YCount; y++)
		{
			for (int x = 0; x < m_XCount; x++)
			{
				pstr = strtok(NULL, ",");
				m_pEnemyData[y * m_XCount + x] = atoi(pstr);
				if (m_pEnemyData[y * m_XCount + x] > 0)
				{
					m_EnemyCount++;
				}
			}
		}
	
		//アイテムテクスチャ読み込み
		pstr = strtok(NULL, ",");
		m_ItemTextureCount = atoi(pstr);
		m_pItemTexture = new CTexture[m_ItemTextureCount];
		for (int i = 0; i < m_ItemTextureCount; i++)
		{
			pstr = strtok(NULL, ",");
			if (!m_pItemTexture[i].Load(pstr))
			{
				return false;
			}
		}
	
		//配置データ読み込み
		m_pItemData = (char*)malloc(m_XCount * m_YCount);
		m_ItemCount = 0;
		for (int y = 0; y < m_YCount; y++)
		{
			for (int x = 0; x < m_XCount; x++)
			{
				pstr = strtok(NULL, ",");
				m_pItemData[y * m_XCount + x] = atoi(pstr);
				if (m_pItemData[y * m_XCount + x] > 0)
				{
					m_ItemCount++;
				}
			}
		}

		pstr = strtok(NULL, ",");
		m_BossPosY = atof(pstr);

		//ファイルを閉じる
		fclose(fp);
		free(pBuffer);
	}
	else
	{
		//テキストファイルを開く
		FILE* fp = fopen(pName, "rt");

		if (fp == NULL)
		{
			return false;
		}

		//ファイルの全容量を調べる
		fseek(fp, 0, SEEK_END);
		long fSize = ftell(fp);
		fseek(fp, 0, SEEK_SET);
	
		//ファイルサイズ分だけのメモリを確保する
		char* pBuffer = (char*)malloc(fSize + 1);
	
		//ファイルを全てバッファに読み込む
		fSize = fread(pBuffer, 1, fSize, fp);
		pBuffer[fSize] = '\0';
		char* pstr;
	
		//テクスチャの読み込み
		pstr = strtok(pBuffer, ",");
		if (!m_BackTexture.Load(pstr))
		{
			return false;
		}
		pstr = strtok(NULL, ",");
		if (!m_ChipTexture.Load(pstr))
		{
			return false;
		}
	
		//チップサイズ
		pstr = strtok(NULL, ",");
		m_ChipSize = atof(pstr);
	
		//マップのチップ数
		pstr = strtok(NULL, ",");
		m_XCount = atoi(pstr);
		pstr = strtok(NULL, ",");
		m_YCount = atoi(pstr);
	
		//マップチップ用のメモリ確保
		m_pChipData = (char*)malloc(m_XCount * m_YCount);
	
		//チップデータの読み込み
		for (int y = 0; y < m_YCount; y++)
		{
			for (int x = 0; x < m_XCount; x++)
			{
				pstr = strtok(NULL, ",");
				m_pChipData[y * m_XCount + x] = atoi(pstr);
			}
		}
	
		//敵テクスチャ読み込み
		pstr = strtok(NULL, ",");
		m_EnemyTextureCount = atoi(pstr);
		m_pEnemyTexture = new CTexture[m_EnemyTextureCount];
		for (int i = 0; i < m_EnemyTextureCount; i++)
		{
			pstr = strtok(NULL, ",");
	
			if (!m_pEnemyTexture[i].Load(pstr))
			{
				return false;
			}
		}
	
		//配置データ読み込み
		m_pEnemyData = (char*)malloc(m_XCount * m_YCount);
		m_EnemyCount = 0;
		for (int y = 0; y < m_YCount; y++)
		{
			for (int x = 0; x < m_XCount; x++)
			{
				pstr = strtok(NULL, ",");
				m_pEnemyData[y * m_XCount + x] = atoi(pstr);
				if (m_pEnemyData[y * m_XCount + x] > 0)
				{
					m_EnemyCount++;
				}
			}
		}
	
		//アイテムテクスチャ読み込み
		pstr = strtok(NULL, ",");
		m_ItemTextureCount = atoi(pstr);
		m_pItemTexture = new CTexture[m_ItemTextureCount];
		for (int i = 0; i < m_ItemTextureCount; i++)
		{
			pstr = strtok(NULL, ",");
			if (!m_pItemTexture[i].Load(pstr))
			{
				return false;
			}
		}
	
		//配置データ読み込み
		m_pItemData = (char*)malloc(m_XCount * m_YCount);
		m_ItemCount = 0;
		for (int y = 0; y < m_YCount; y++)
		{
			for (int x = 0; x < m_XCount; x++)
			{
				pstr = strtok(NULL, ",");
				m_pItemData[y * m_XCount + x] = atoi(pstr);
				if (m_pItemData[y * m_XCount + x] > 0)
				{
					m_ItemCount++;
				}
			}
		}
		//ボスの位置を読み込み
		pstr = strtok(NULL, ",");
		m_BossPosX = atof(pstr);
	
		pstr = strtok(NULL, ",");
		m_BossPosY = atof(pstr);
	
		//ファイルを閉じる
		fclose(fp);
		free(pBuffer);
	}
	
	return true;
}

/**
* 初期化
*/
void CStage::Initialize(CEnemy* pEnemy,CItem* pItem) {
	m_Scroll.x = 0;
	m_Scroll.y = 0;
	int n = 0;
	for (int y = 0; y < m_YCount; y++)
	{
		for (int x = 0; x < m_XCount; x++)
		{
			//配置番号
			//番号0は配置しない
			char on = m_pEnemyData[y * m_XCount + x] - 1;
			if (on < 0)
			{
				continue;
			}
			pEnemy[n].SetTexture(&m_pEnemyTexture[on]);
			pEnemy[n++].Initialize(x * m_ChipSize, y * m_ChipSize, on);
		}
	}
	
	n = 0;
	for (int y = 0; y < m_YCount; y++)
	{
		for (int x = 0; x < m_XCount; x++)
		{
			//配置番号
			//番号０は配置しない
			char on = m_pItemData[y * m_XCount + x] - 1;
			if (on < 0)
			{
				continue;
			}
			pItem[n].SetTexture(&m_pItemTexture[on]);
			pItem[n++].Initialize(x * m_ChipSize, y * m_ChipSize, on);
		}
	}
}

/**
* 更新
*
* 引数
* [in]		pl		プレイヤー、スクロール判定に使用
*/
void CStage::Update(CPlayer& pl) {

	//プレイヤー矩形取得
	CRectangle prec = pl.GetRect();

	//スクリーン幅
	float sw = CGraphicsUtilities::GetGraphics()->GetTargetWidth();

	//ステージ全体幅
	float stgw = m_ChipSize * m_XCount;

	//座標が画面端の場合(各端から220pixel)スクロールを行って補正する
	if (prec.Left - m_Scroll.x < 220)
	{
		m_Scroll.x -= 220 - (prec.Left - m_Scroll.x);
		if (m_Scroll.x <= 0)
		{
			m_Scroll.x = 0;
		}
	}
	else if (prec.Right - m_Scroll.x > sw - 220)
	{
		m_Scroll.x += (prec.Right - m_Scroll.x) - (sw - 220);
		if (m_Scroll.x >= stgw - sw)
		{
			m_Scroll.x = stgw - sw;
		}
	}
}

/**
* 当たり判定
*
* [in]		r		判定矩形
* [out]		ox		X埋まり
* [out]		oy		Y埋まり
*/
bool CStage::Colision(CRectangle r,float& ox,float& oy) {
	bool re = false;

	//当たり判定する矩形の左上と右下のチップ位置を求める
	int lc = r.Left / m_ChipSize;
	int rc = r.Right / m_ChipSize;
	int tc = r.Top / m_ChipSize;
	int bc = r.Bottom / m_ChipSize;

	//ステージの範囲外にはならないようにする
	if (lc < 0) { lc = 0; }
	if (tc < 0) { tc = 0; }
	if (rc >= m_XCount) { rc = m_XCount - 1; }
	if (bc >= m_YCount) { bc = m_YCount - 1; }

	//当たり判定をする矩形の左上から右下の範囲のみ当たり判定をおこなう
	//それ以外の番号は当たることはないので判定が必要ない
	for (int y = tc; y <= bc; y++)
	{
		for (int x = lc; x <= rc; x++)
		{
			//描画するチップ番号
			//チップ番号０は当たり判定しない
			char cn = m_pChipData[y * m_XCount + x] - 1;
			if (cn < 0 || cn == 7 || cn == 1)
			{
				continue;
			}

			//マップチップ矩形
			CRectangle cr(x * m_ChipSize, y * m_ChipSize,
				x * m_ChipSize + m_ChipSize, y * m_ChipSize + m_ChipSize);

			//当たり判定用キャラクタ矩形
			CRectangle brec = r;
			brec.Top = brec.Bottom - 1; //下の矩形は上側を下と同じ値にする
			brec.Expansion(-6, 0);      //横の範囲を少し狭める

			//当たり判定(下)
			if (cr.CollisionRect(brec))
			{
				re = true;
				//チップが斜面の場合自分の立ち位置から高さの割合を求める
				//斜面(左)
				if (cn == LEFTSLOPE)
				{
					float sp = (brec.Right - cr.Left) / cr.GetWidth();
					if (sp < 0.0f) { sp = 0.0f; }
					else if (sp > 1.0f) { sp = 1.0f; }

					//斜面の上を求める
					float cTop = cr.Bottom - cr.GetHeight() * sp;
					if (brec.Bottom < cTop) { continue; }

					//下埋まりなので坂上端から矩形の下端を引いた値が埋まり値
					oy += cTop - brec.Bottom;
					r.Top += cTop - brec.Bottom;
					r.Bottom += cTop - brec.Bottom;
				}
				//斜面(右)
				else if (cn == RIGHTSLOPE)
				{
					float sp = (cr.Right - brec.Left) / cr.GetWidth();
					if (sp > 1.0f) { sp = 1.0f; }
					else if (sp < 0.0f) { sp = 0.0f; }

					//斜面の上を求める
					float cTop = cr.Bottom - cr.GetHeight() * sp;
					if (brec.Bottom  < cTop) { continue; }

					//下埋まりなので坂上端から矩形の下線を引いた値が埋まり値
					oy += cTop - brec.Bottom;
					r.Top += cTop - brec.Bottom;
					r.Bottom += cTop - brec.Bottom;
				}
				else
				{
					oy += cr.Top - brec.Bottom;
					r.Top += cr.Top - brec.Bottom;
					r.Bottom += cr.Top - brec.Bottom;
				}

			}
			//斜面以外の時
			if (cn != LEFTSLOPE && cn != RIGHTSLOPE) 
			{
				CRectangle lrec = r;
				lrec.Right = lrec.Left + 1;   //左の矩形は右側を左と同じ値にする
				lrec.Expansion(0, -6);        //縦の範囲を少し狭める
				CRectangle rrec = r;
				rrec.Left = rrec.Right - 1;   //右の矩形は左側を右と同じ値にする
				rrec.Expansion(0, -6);        //縦の範囲を少し狭める

				//当たり判定(左)
				if (cr.CollisionRect(lrec))
				{
					re = true;

					ox += cr.Right - lrec.Left;
					r.Left += cr.Right - lrec.Left;
					r.Right += cr.Right - lrec.Left;
				}
				//当たり判定(右)
				else if (cr.CollisionRect(rrec))
				{
					re = true;
					ox += cr.Left - rrec.Right;
					r.Left += cr.Left - rrec.Right;
					r.Right += cr.Left - rrec.Right;
				}

				CRectangle trec = r;
				trec.Bottom = trec.Top + 1;
				trec.Expansion(-6, 0);

				//当たり判定(上)
				if (cr.CollisionRect(trec))
				{
					re = true;
					oy += cr.Bottom - trec.Top;
					r.Top += cr.Bottom - trec.Top;
					r.Bottom += cr.Bottom - trec.Top;
				}
			}
		}
	}
	return re;
}

bool CStage::IsGround(float px, float py) {
	bool re = false;

	//当たり判定する矩形の左上と右下のチップ位置を求める
	int x = px / m_ChipSize;
	int y = py / m_ChipSize;

	//ステージの範囲外にならないようにする
	if (x < 0 || x >= m_XCount || y < 0 || y >= m_YCount)
	{
		return false;
	}

	//描画するチップ番号
	//チップ番号０は当たり判定しない
	char cn = m_pChipData[y * m_XCount + x] - 1;
	if (cn < 0)
	{
		return false;
	}

	return true;
}

/**
* 描画
*/
void CStage::Render() {

	//遠景の描画
	int scw = g_pGraphics->GetTargetWidth();
	int sch = g_pGraphics->GetTargetHeight();
	int wn = m_BackTexture.GetWidth();
	int hn = m_BackTexture.GetHeight();

	for (float y = ((int)-m_Scroll.y % hn) - hn; y < sch; y += hn)
	{
		for (float x = ((int)-m_Scroll.x % wn) - wn; x < scw; x += wn)
		{
			m_BackTexture.Render(x, y);
		}
	}

	//テクスチャの横幅からマップチップの縦オフセットを求める
	int tcx = m_ChipTexture.GetWidth() / m_ChipSize;

	//マップチップ描画
	for (int y = 0; y < m_YCount; y++)
	{
		for (int x = 0; x < m_XCount; x++)
		{
			//描画するチップ番号
			//チップ番号０は描画しない
			char cn = m_pChipData[y * m_XCount + x] - 1;
			if (cn < 0)
			{
				continue;
			}

			//マップチップ矩形
			CRectangle cr(m_ChipSize * (cn % tcx), m_ChipSize * (cn / tcx),
				m_ChipSize * (cn % tcx + 1), m_ChipSize * (cn / tcx + 1));

			//マップチップ描画
			m_ChipTexture.Render(-m_Scroll.x + x * m_ChipSize,
								 -m_Scroll.y + y * m_ChipSize, cr);
		}
	}
}

/**
* デバッグ描画
*/
void CStage::RenderDebug() {
	//位置の描画
	CGraphicsUtilities::RenderString(10, 100, "スクロール X : %.0f,Y : %.0f", m_Scroll.x, m_Scroll.y);
}

/**
* 解放
*/
void CStage::Release() {
	m_ChipTexture.Release();
	m_BackTexture.Release();
	if (m_pChipData)
	{
		free(m_pChipData);
		m_pChipData = NULL;
	}
	//敵用テクスチャ解放処理
	if (m_pEnemyData)
	{
		free(m_pEnemyData);
		m_pEnemyData = NULL;
	}
	if (m_pEnemyTexture)
	{
		delete[] m_pEnemyTexture;
		m_pEnemyTexture = NULL;
	}
	//アイテム用テクスチャ解放処理
	if (m_pItemData)
	{
		free(m_pItemData);
		m_pItemData = NULL;
	}
	if (m_pItemTexture)
	{
		delete[] m_pItemTexture;
		m_pItemTexture = NULL;
	}
}