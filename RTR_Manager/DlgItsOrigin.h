#pragma once


#define LIST_PATH _T("C:\\R2RSet\\ItsOriginCaseManager.ini")
//#define LOG_PATH _T("C:\\R2RSet\\Log\\")


#define CASE_0 _T("1-0 [몰상,좌상]")
#define CASE_1 _T("2-0 [몰상,좌하]")
#define CASE_2 _T("3-0 [몰하,우상]")
#define CASE_3 _T("4-0 [몰하,우하]")
#define CASE_4 _T("1-1 [몰상,우상]")
#define CASE_5 _T("2-1 [몰상,우하]")
#define CASE_6 _T("3-1 [몰하,좌상]")
#define CASE_7 _T("4-1 [몰하,좌하]")

struct stModelItsOrigin
{
	CString sModel;
	int nItsCase;			// 0 ~ 3 :  좌우[0], 좌하[1], 우상[2], 우하[3]

	stModelItsOrigin()
	{
		sModel = _T("");
		nItsCase = 0;
	}
};

struct stListItsOrigin
{
	int nTotalModels;
	stModelItsOrigin *pModel;

	TCHAR szData[MAX_PATH];
	TCHAR sep[6]; // _T(",/;\r\n\t");
	TCHAR *token;
	CString sModelIdx;
	int i;

	stListItsOrigin()
	{
		nTotalModels = 0;
		pModel = NULL;
		sep[0] = _T(',');
		sep[1] = _T('/');
		sep[2] = _T(';');
		sep[3] = _T('\r');
		sep[4] = _T('\n');
		sep[5] = _T('\t');
		token = NULL;

		LoadList();
	}

	~stListItsOrigin()
	{
		if (pModel)
		{
			delete[] pModel;
			pModel = NULL;
		}
	}


	void LoadList()
	{
		if (0 < ::GetPrivateProfileString(_T("Info"), _T("Total_Models"), NULL, szData, sizeof(szData), LIST_PATH))
			nTotalModels = _ttoi(szData);
		else
			nTotalModels = 0;

		AllocMem();

		if (nTotalModels > 0)
		{
			for (i = 0; i < nTotalModels; i++)
			{
				sModelIdx.Format(_T("%d"), i);
				if (0 < ::GetPrivateProfileString(_T("Model"), sModelIdx, NULL, szData, sizeof(szData), LIST_PATH))
				{
					token = _tcstok(szData, sep);
					pModel[i].sModel = CString(token);
					token = _tcstok(NULL, sep);
					pModel[i].nItsCase = _ttoi(token);
				}
			}
		}
	}

	void ReloadList()
	{
		if (pModel)
		{
			delete[] pModel;
			pModel = NULL;
		}

		LoadList();
	}

	void AllocMem()
	{
		if (nTotalModels > 0)
		{
			if (pModel)
				delete[] pModel;

			pModel = new stModelItsOrigin[nTotalModels];
		}
	}
};


// CDlgItsOrigin 대화 상자입니다.

class CDlgItsOrigin : public CDialog
{
	DECLARE_DYNAMIC(CDlgItsOrigin)

private:
	CString m_sPathCamSpecDir, m_sModel;
	stListItsOrigin m_stListItsOrigin;
	int m_nCase;

	void LoadIni();
	void InitList();
	void UpdateList();
	void DispList();

	int SearchModelInCombo(CString sModel);
	int SearchModel(CString sModel);
	int SearchModel(CString sModel, int &nCase);
	BOOL UpdateModel(CString sModel, int nCase);
	void InsertModel(CString sModel, int nCase);
	void ModifyModel(CString sModel, int nCase);
	void ModifyModelData();
	void SelectList(CString sModel);
	BOOL DeleteModel(CString sModel);

	int GetItsOriginCase(CString sModel);
	void ResetComboItsCase();
	CString GetItsCaseLabel(int nIdx);
	void ModifyItsOriginCaseData();
	void ModifyItsOriginCaseData(int nCase);

	void Log(CString strMsg, int nType = 0);
	BOOL DirectoryExists(LPCTSTR szPath);
	void StringToChar(CString str, char* pCh); // char* returned must be deleted... 

	CString GetCaseString(int nCase);

public:
	CDlgItsOrigin(CWnd* pParent = NULL);   // 표준 생성자입니다.
	virtual ~CDlgItsOrigin();

// 대화 상자 데이터입니다.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DLG_ITS_ORIGIN };
#else
	enum { IDD = IDD_DLG_ITS_ORIGIN };
#endif

public:
	BOOL Create();


protected:
	CListCtrl m_List;
	virtual BOOL OnInitDialog();
	afx_msg void OnClickList1(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnBnClickedButtonSaveItsCase();
	afx_msg void OnBnClickedButtonRefreshModel();
	afx_msg void OnSelchangeComboItsCase();
	afx_msg void OnBnClickedButtonFindModel();
	afx_msg void OnBnClickedButtonDeleteModel();
	afx_msg void OnSelchangeComboModel();

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()
};
