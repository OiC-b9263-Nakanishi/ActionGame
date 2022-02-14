#include "Game.h"
#include "GameDefine.h"

//�ύX����V�[��(���̂�GameApp)
extern int			gChangeScene;

//�X�e�[�W�ԍ�
extern int			gStageNo;

//�G�e
#define		ENEMYSHOT_COUNT		100
CEnemyShot	gShotArray[ENEMYSHOT_COUNT];
CTexture	gEnemyShotTexture;

//�{�X�e
#define		BOSSSHOT_COUNT		100
CEnemyShot  gBossShotArray[BOSSSHOT_COUNT];
CTexture	gBossShotTexture;

//�X�e�[�W
char* Stage[3] = {"Stage.txt","Stage(��2).txt","Tutorial(Move).txt"};

/**
* �R���X�g���N�^
*/
CGame::CGame()
{
}

/**
* �f�X�g���N�^
*/
CGame::~CGame()
{
}

/**
* �ǂݍ���
*/
bool CGame::Load() {
	//�v���C���[�f�ޓǂݍ���
	m_Player.Load();
	//�X�e�[�W�ǂݍ���
	m_Stage.Load(Stage[gStageNo]);
	//�G�������m��
	m_EnemyArray = new CEnemy[m_Stage.GetEnemyCount()];
	//�G�e�Ǎ�
	if (!gEnemyShotTexture.Load("eneshot.png"))
	{
		return false;
	}
	//�A�C�e���������m��
	m_ItemArray = new CItem[m_Stage.GetItemCount()];
	//�G�t�F�N�g�f�ޓǂݍ���
	m_EffectManager.Load();
	//�{�X�f�ޓǂݍ���
	m_Boss.Load();
	//�{�X�e�Ǎ�
	if (!gBossShotTexture.Load("bossShot.png"))
	{
		return false;
	}
	return true;
}

/**
* ������
*/
void CGame::Initialize() {

	//�v���C���[�̏�ԏ�����
	m_Player.Initialize();
	//�X�e�[�W��ԏ�����
	m_Stage.Initialize(m_EnemyArray, m_ItemArray);
	//�G�t�F�N�g��ԏ�����
	m_EffectManager.Initialize();
	//�v���C���[�ƓG�ɃG�t�F�N�g�N���X�ݒ�
	m_Player.SetEffectManager(&m_EffectManager);
	m_Boss.SetEffectManager(&m_EffectManager);
	for (int i = 0; i < m_Stage.GetEnemyCount(); i++)
	{
		m_EnemyArray[i].SetEffectManager(&m_EffectManager);
	}
	//�G�e������
	for (int i = 0; i < ENEMYSHOT_COUNT; i++)
	{
		gShotArray[i].Initialize();
		gShotArray[i].SetTexture(&gEnemyShotTexture);
	}
	//�{�X��ԏ�����
	m_Boss.Initialize(m_Stage.GetBossPosX(),m_Stage.GetBossPosY());
	for (int i = 0; i < BOSSSHOT_COUNT; i++)
	{
		gBossShotArray[i].Initialize();
		gBossShotArray[i].SetTexture(&gBossShotTexture);
	}
}

/**
* �X�V
*/
void CGame::Update() {

	//�G�p�ˌ��v���C���[���W
	CRectangle prec = m_Player.GetRect();
	CVector2   ps = prec.GetCenter();

	//�v���C���[�X�V
	m_Player.Update();

	//�X�e�[�W�ƃv���C���[�̓����蔻��
	float ox = 0, oy = 0;
	if (m_Stage.Colision(m_Player.GetRect(), ox, oy))
	{
		m_Player.ColisionStage(ox, oy);
	}

	//�G�X�V
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
	//�{�X�����蔻��
	float Box = 0, Boy = 0;
	if (m_Stage.Colision(m_Boss.GetRect(), Box, Boy))
	{
		m_Boss.ColisionStage(Box, Boy);
	}
	//�G�e�X�V
	for (int i = 0; i < ENEMYSHOT_COUNT; i++)
	{
		gShotArray[i].Update(m_Stage.GetScrollX(),m_Stage.GetScrollY());
	}
	//�A�C�e���̍X�V
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
	//�����蔻����s
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
	//�X�e�[�W�X�V
	m_Stage.Update(m_Player);
	//�G�t�F�N�g�X�V
	m_EffectManager.Update();
	//�{�X�X�V
	m_Boss.SetTargetPos(ps);
	m_Boss.Update(gBossShotArray,BOSSSHOT_COUNT);
	//�{�X�e�X�V
	for (int i = 0; i < BOSSSHOT_COUNT; i++)
	{
		gBossShotArray[i].Update(m_Stage.GetScrollX(), m_Stage.GetScrollY());
	}
	
	//�v���C���[�I���t���O�ŃQ�[���I�[�o�[��ʂ�
	if (m_Player.IsEnd())
	{
		gChangeScene = SCENENO_GAMEOVER;
	}
	//�{�X�����j�ŃQ�[���N���A��ʂ�
	if (!m_Boss.GetShow())
	{
		if (m_Boss.IsClear())
		{
			gChangeScene = SCENENO_GAMECLEAR;
		}
	}
	//�S�[���t���O�ŃQ�[���N���A��ʂ�
	if (m_Player.IsGoal())
	{
		gChangeScene = SCENENO_GAMECLEAR;
	}
}

/**
* �`��
*/
void CGame::Render() {

	//�X�e�[�W�`��
	m_Stage.Render();
	//�v���C���[�`��
	m_Player.Render(m_Stage.GetScrollX(),m_Stage.GetScrollY());
	//�G�`��
	for (int i = 0; i < m_Stage.GetEnemyCount(); i++)
	{
		m_EnemyArray[i].Render(m_Stage.GetScrollX(), m_Stage.GetScrollY());
	}
	//�G�e�`��
	for (int i = 0; i < ENEMYSHOT_COUNT; i++)
	{
		gShotArray[i].Render(m_Stage.GetScrollX(),m_Stage.GetScrollY());
	}
	//�A�C�e���`��
	for (int i = 0; i < m_Stage.GetItemCount(); i++)
	{
		m_ItemArray[i].Render(m_Stage.GetScrollX(), m_Stage.GetScrollY());
	}
	//�G�t�F�N�g�`��
	m_EffectManager.Render(m_Stage.GetScrollX(), m_Stage.GetScrollY());
	//�v���C���[�̏�ԕ`��
	m_Player.RenderStatus();
	//�{�X�`��
	m_Boss.Render(m_Stage.GetScrollX(),m_Stage.GetScrollY());
	//�{�X�e�`��
	for (int i = 0; i < BOSSSHOT_COUNT; i++)
	{
		gBossShotArray[i].Render(m_Stage.GetScrollX(), m_Stage.GetScrollY());
	}
	if (gStageNo == STAGENO_TUTORIAL)
	{
		CGraphicsUtilities::RenderString(10, 100, "�ړ��FA,D�A�W�����v�FW�A�U���F�}�E�X���A����F�}�E�X�E�A�K�[�h�FF", MOF_COLOR_WHITE);
		CGraphicsUtilities::RenderString(10, 130, "������m�F�o������A�C�e��������ăX�e�[�W���I�����Ă�������", MOF_COLOR_WHITE);
	}
}

/**
* �f�o�b�O�`��
*/
void CGame::RenderDebug() {

	//�X�e�[�W�f�o�b�O�`��
	m_Stage.RenderDebug();
	//�v���C���[�f�o�b�O�`��
	m_Player.RenderDebug(m_Stage.GetScrollX(),m_Stage.GetScrollY());
	//�G�f�o�b�O�`��
	for (int i = 0; i < m_Stage.GetEnemyCount(); i++)
	{
		m_EnemyArray->RenderDebug(m_Stage.GetScrollX(), m_Stage.GetScrollY());
	}
	//�A�C�e���f�o�b�O�`��
	for (int i = 0; i < m_Stage.GetItemCount(); i++)
	{
		m_ItemArray->RenderDebug(m_Stage.GetScrollX(), m_Stage.GetScrollY());
	}
	//�G�t�F�N�g�f�o�b�O�`��
	m_EffectManager.RenderDebug(m_Stage.GetScrollX(), m_Stage.GetScrollY());
	//�{�X�f�o�b�O�`��
	m_Boss.RenderDebug(m_Stage.GetScrollX(), m_Stage.GetScrollY());
}

/**
* ���
*/
void CGame::Release() {
	//�X�e�[�W���
	m_Stage.Release();
	//�v���C���[���
	m_Player.Release();
	//�G���
	if (m_EnemyArray)
	{
		delete[] m_EnemyArray;
		m_EnemyArray = NULL;
	}
	//�A�C�e�����
	if (m_ItemArray)
	{
		delete[] m_ItemArray;
		m_ItemArray = NULL;
	}
	//�G�t�F�N�g���
	m_EffectManager.Release();
	//�G�e���
	gEnemyShotTexture.Release();
	//�{�X���
	m_Boss.Release();
	gBossShotTexture.Release();
}