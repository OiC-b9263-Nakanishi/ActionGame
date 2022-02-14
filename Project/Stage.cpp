#include "Stage.h"
#include "GameDefine.h"

//�X�e�[�W�ԍ�
extern int		gStageNo;

/**
* �R���X�g���N�^
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
* �f�X�g���N�^
*/
CStage::~CStage() {
}

/**
* �ǂݍ���
*
* [in]		pName		�X�e�[�W�t�@�C����
*/
bool CStage::Load(char* pName) {

	
	if(gStageNo >= STAGENO_TUTORIAL)
	{
		//�e�L�X�g�t�@�C�����J��
		FILE* fp = fopen(pName, "rt");
	
		if (fp == NULL)
		{
			return false;
		}
	
		//�t�@�C���̑S�e�ʂ𒲂ׂ�
		fseek(fp, 0, SEEK_END);
		long fSize = ftell(fp);
		fseek(fp, 0, SEEK_SET);
	
		//�t�@�C���T�C�Y�������̃��������m�ۂ���
		char* pBuffer = (char*)malloc(fSize + 1);
	
		//�t�@�C����S�ăo�b�t�@�ɓǂݍ���
		fSize = fread(pBuffer, 1, fSize, fp);
		pBuffer[fSize] = '\0';
		char* pstr;
	
		//�e�N�X�`���̓ǂݍ���
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
	
		//�`�b�v�T�C�Y
		pstr = strtok(NULL, ",");
		m_ChipSize = atof(pstr);
	
		//�}�b�v�̃`�b�v��
		pstr = strtok(NULL, ",");
		m_XCount = atoi(pstr);
		pstr = strtok(NULL, ",");
		m_YCount = atoi(pstr);
	
		//�}�b�v�`�b�v�p�̃������m��
		m_pChipData = (char*)malloc(m_XCount * m_YCount);
	
		//�`�b�v�f�[�^�̓ǂݍ���
		for (int y = 0; y < m_YCount; y++)
		{
			for (int x = 0; x < m_XCount; x++)
			{
				pstr = strtok(NULL, ",");
				m_pChipData[y * m_XCount + x] = atoi(pstr);
			}
		}
	
		//�G�e�N�X�`���ǂݍ���
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
	
		//�z�u�f�[�^�ǂݍ���
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
	
		//�A�C�e���e�N�X�`���ǂݍ���
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
	
		//�z�u�f�[�^�ǂݍ���
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

		//�t�@�C�������
		fclose(fp);
		free(pBuffer);
	}
	else
	{
		//�e�L�X�g�t�@�C�����J��
		FILE* fp = fopen(pName, "rt");

		if (fp == NULL)
		{
			return false;
		}

		//�t�@�C���̑S�e�ʂ𒲂ׂ�
		fseek(fp, 0, SEEK_END);
		long fSize = ftell(fp);
		fseek(fp, 0, SEEK_SET);
	
		//�t�@�C���T�C�Y�������̃��������m�ۂ���
		char* pBuffer = (char*)malloc(fSize + 1);
	
		//�t�@�C����S�ăo�b�t�@�ɓǂݍ���
		fSize = fread(pBuffer, 1, fSize, fp);
		pBuffer[fSize] = '\0';
		char* pstr;
	
		//�e�N�X�`���̓ǂݍ���
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
	
		//�`�b�v�T�C�Y
		pstr = strtok(NULL, ",");
		m_ChipSize = atof(pstr);
	
		//�}�b�v�̃`�b�v��
		pstr = strtok(NULL, ",");
		m_XCount = atoi(pstr);
		pstr = strtok(NULL, ",");
		m_YCount = atoi(pstr);
	
		//�}�b�v�`�b�v�p�̃������m��
		m_pChipData = (char*)malloc(m_XCount * m_YCount);
	
		//�`�b�v�f�[�^�̓ǂݍ���
		for (int y = 0; y < m_YCount; y++)
		{
			for (int x = 0; x < m_XCount; x++)
			{
				pstr = strtok(NULL, ",");
				m_pChipData[y * m_XCount + x] = atoi(pstr);
			}
		}
	
		//�G�e�N�X�`���ǂݍ���
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
	
		//�z�u�f�[�^�ǂݍ���
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
	
		//�A�C�e���e�N�X�`���ǂݍ���
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
	
		//�z�u�f�[�^�ǂݍ���
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
		//�{�X�̈ʒu��ǂݍ���
		pstr = strtok(NULL, ",");
		m_BossPosX = atof(pstr);
	
		pstr = strtok(NULL, ",");
		m_BossPosY = atof(pstr);
	
		//�t�@�C�������
		fclose(fp);
		free(pBuffer);
	}
	
	return true;
}

/**
* ������
*/
void CStage::Initialize(CEnemy* pEnemy,CItem* pItem) {
	m_Scroll.x = 0;
	m_Scroll.y = 0;
	int n = 0;
	for (int y = 0; y < m_YCount; y++)
	{
		for (int x = 0; x < m_XCount; x++)
		{
			//�z�u�ԍ�
			//�ԍ�0�͔z�u���Ȃ�
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
			//�z�u�ԍ�
			//�ԍ��O�͔z�u���Ȃ�
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
* �X�V
*
* ����
* [in]		pl		�v���C���[�A�X�N���[������Ɏg�p
*/
void CStage::Update(CPlayer& pl) {

	//�v���C���[��`�擾
	CRectangle prec = pl.GetRect();

	//�X�N���[����
	float sw = CGraphicsUtilities::GetGraphics()->GetTargetWidth();

	//�X�e�[�W�S�̕�
	float stgw = m_ChipSize * m_XCount;

	//���W����ʒ[�̏ꍇ(�e�[����220pixel)�X�N���[�����s���ĕ␳����
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
* �����蔻��
*
* [in]		r		�����`
* [out]		ox		X���܂�
* [out]		oy		Y���܂�
*/
bool CStage::Colision(CRectangle r,float& ox,float& oy) {
	bool re = false;

	//�����蔻�肷���`�̍���ƉE���̃`�b�v�ʒu�����߂�
	int lc = r.Left / m_ChipSize;
	int rc = r.Right / m_ChipSize;
	int tc = r.Top / m_ChipSize;
	int bc = r.Bottom / m_ChipSize;

	//�X�e�[�W�͈̔͊O�ɂ͂Ȃ�Ȃ��悤�ɂ���
	if (lc < 0) { lc = 0; }
	if (tc < 0) { tc = 0; }
	if (rc >= m_XCount) { rc = m_XCount - 1; }
	if (bc >= m_YCount) { bc = m_YCount - 1; }

	//�����蔻��������`�̍��ォ��E���͈̔͂̂ݓ����蔻��������Ȃ�
	//����ȊO�̔ԍ��͓����邱�Ƃ͂Ȃ��̂Ŕ��肪�K�v�Ȃ�
	for (int y = tc; y <= bc; y++)
	{
		for (int x = lc; x <= rc; x++)
		{
			//�`�悷��`�b�v�ԍ�
			//�`�b�v�ԍ��O�͓����蔻�肵�Ȃ�
			char cn = m_pChipData[y * m_XCount + x] - 1;
			if (cn < 0 || cn == 7 || cn == 1)
			{
				continue;
			}

			//�}�b�v�`�b�v��`
			CRectangle cr(x * m_ChipSize, y * m_ChipSize,
				x * m_ChipSize + m_ChipSize, y * m_ChipSize + m_ChipSize);

			//�����蔻��p�L�����N�^��`
			CRectangle brec = r;
			brec.Top = brec.Bottom - 1; //���̋�`�͏㑤�����Ɠ����l�ɂ���
			brec.Expansion(-6, 0);      //���͈̔͂��������߂�

			//�����蔻��(��)
			if (cr.CollisionRect(brec))
			{
				re = true;
				//�`�b�v���Ζʂ̏ꍇ�����̗����ʒu���獂���̊��������߂�
				//�Ζ�(��)
				if (cn == LEFTSLOPE)
				{
					float sp = (brec.Right - cr.Left) / cr.GetWidth();
					if (sp < 0.0f) { sp = 0.0f; }
					else if (sp > 1.0f) { sp = 1.0f; }

					//�Ζʂ̏�����߂�
					float cTop = cr.Bottom - cr.GetHeight() * sp;
					if (brec.Bottom < cTop) { continue; }

					//�����܂�Ȃ̂ō��[�����`�̉��[���������l�����܂�l
					oy += cTop - brec.Bottom;
					r.Top += cTop - brec.Bottom;
					r.Bottom += cTop - brec.Bottom;
				}
				//�Ζ�(�E)
				else if (cn == RIGHTSLOPE)
				{
					float sp = (cr.Right - brec.Left) / cr.GetWidth();
					if (sp > 1.0f) { sp = 1.0f; }
					else if (sp < 0.0f) { sp = 0.0f; }

					//�Ζʂ̏�����߂�
					float cTop = cr.Bottom - cr.GetHeight() * sp;
					if (brec.Bottom  < cTop) { continue; }

					//�����܂�Ȃ̂ō��[�����`�̉������������l�����܂�l
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
			//�ΖʈȊO�̎�
			if (cn != LEFTSLOPE && cn != RIGHTSLOPE) 
			{
				CRectangle lrec = r;
				lrec.Right = lrec.Left + 1;   //���̋�`�͉E�������Ɠ����l�ɂ���
				lrec.Expansion(0, -6);        //�c�͈̔͂��������߂�
				CRectangle rrec = r;
				rrec.Left = rrec.Right - 1;   //�E�̋�`�͍������E�Ɠ����l�ɂ���
				rrec.Expansion(0, -6);        //�c�͈̔͂��������߂�

				//�����蔻��(��)
				if (cr.CollisionRect(lrec))
				{
					re = true;

					ox += cr.Right - lrec.Left;
					r.Left += cr.Right - lrec.Left;
					r.Right += cr.Right - lrec.Left;
				}
				//�����蔻��(�E)
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

				//�����蔻��(��)
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

	//�����蔻�肷���`�̍���ƉE���̃`�b�v�ʒu�����߂�
	int x = px / m_ChipSize;
	int y = py / m_ChipSize;

	//�X�e�[�W�͈̔͊O�ɂȂ�Ȃ��悤�ɂ���
	if (x < 0 || x >= m_XCount || y < 0 || y >= m_YCount)
	{
		return false;
	}

	//�`�悷��`�b�v�ԍ�
	//�`�b�v�ԍ��O�͓����蔻�肵�Ȃ�
	char cn = m_pChipData[y * m_XCount + x] - 1;
	if (cn < 0)
	{
		return false;
	}

	return true;
}

/**
* �`��
*/
void CStage::Render() {

	//���i�̕`��
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

	//�e�N�X�`���̉�������}�b�v�`�b�v�̏c�I�t�Z�b�g�����߂�
	int tcx = m_ChipTexture.GetWidth() / m_ChipSize;

	//�}�b�v�`�b�v�`��
	for (int y = 0; y < m_YCount; y++)
	{
		for (int x = 0; x < m_XCount; x++)
		{
			//�`�悷��`�b�v�ԍ�
			//�`�b�v�ԍ��O�͕`�悵�Ȃ�
			char cn = m_pChipData[y * m_XCount + x] - 1;
			if (cn < 0)
			{
				continue;
			}

			//�}�b�v�`�b�v��`
			CRectangle cr(m_ChipSize * (cn % tcx), m_ChipSize * (cn / tcx),
				m_ChipSize * (cn % tcx + 1), m_ChipSize * (cn / tcx + 1));

			//�}�b�v�`�b�v�`��
			m_ChipTexture.Render(-m_Scroll.x + x * m_ChipSize,
								 -m_Scroll.y + y * m_ChipSize, cr);
		}
	}
}

/**
* �f�o�b�O�`��
*/
void CStage::RenderDebug() {
	//�ʒu�̕`��
	CGraphicsUtilities::RenderString(10, 100, "�X�N���[�� X : %.0f,Y : %.0f", m_Scroll.x, m_Scroll.y);
}

/**
* ���
*/
void CStage::Release() {
	m_ChipTexture.Release();
	m_BackTexture.Release();
	if (m_pChipData)
	{
		free(m_pChipData);
		m_pChipData = NULL;
	}
	//�G�p�e�N�X�`���������
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
	//�A�C�e���p�e�N�X�`���������
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