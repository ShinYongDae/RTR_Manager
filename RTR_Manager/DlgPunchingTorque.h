#pragma once

#define LIST_PATH _T("C:\\R2RSet\\PunchingTorqueList.ini")
//#define LOG_PATH _T("C:\\R2RSet\\Log\\")

#define THICKNESS_0 _T("설정없음")
#define THICKNESS_1 _T("얇은 두께")
#define THICKNESS_2 _T("중간 두께")
#define THICKNESS_3 _T("두꺼운 두께")

struct stTorque
{
	double dLeft, dRight;

	stTorque()
	{
		dLeft = 0.0;
		dRight = 0.0;
	}
};

struct stUnit
{
	int nNum;			// 호기 번호
	stTorque Torq;
	stUnit()
	{
		nNum = 0;
	}
};

struct stThick
{
	int nTotalUnit;		// 4 : 11, 12, 13, 14
	int *pUnitList;		// 호기 번호 List
	stUnit *pUnit;		// 호기 : 11, 12, 13, 14

	int i;
	TCHAR szData[MAX_PATH];
	TCHAR sep[6]; // _T(",/;\r\n\t");
	TCHAR *token;
	CString sThickIdx, sUnitNum;

	stThick()
	{
		nTotalUnit = 0;
		pUnitList = NULL;
		pUnit = NULL;
		sep[0] = _T(',');
		sep[1] = _T('/');
		sep[2] = _T(';');
		sep[3] = _T('\r');
		sep[4] = _T('\n');
		sep[5] = _T('\t');
		token = NULL;
	}

	~stThick()
	{
		if (pUnitList)
		{
			delete[] pUnitList;
			pUnitList = NULL;
		}

		if (pUnit)
		{
			delete[] pUnit;
			pUnit = NULL;
		}
	}

	void LoadUnit(int nThickness)		// 두께별 호기의 토크값 로딩
	{
		if (0 < ::GetPrivateProfileString(_T("Info"), _T("Total_Machines"), NULL, szData, sizeof(szData), LIST_PATH))
			nTotalUnit = _ttoi(szData);
		else
			nTotalUnit = 0;

		AllocMem();

		if (nTotalUnit > 0)
		{
			if (0 < ::GetPrivateProfileString(_T("Info"), _T("Unit_Nums"), NULL, szData, sizeof(szData), LIST_PATH))
			{
				token = _tcstok(szData, sep);
				for (i = 0; i < nTotalUnit; i++)
				{
					pUnitList[i] = _ttoi(token);
					if (i < nTotalUnit - 1)
						token = _tcstok(NULL, sep);
				}
			}

			sThickIdx.Format(_T("%d"), nThickness);
			for (i = 0; i < nTotalUnit; i++)		// 4 : 11, 12, 13, 14
			{
				pUnit[i].nNum = pUnitList[i];
				sUnitNum.Format(_T("%d"), pUnit[i].nNum);
				if (0 < ::GetPrivateProfileString(sThickIdx, sUnitNum, NULL, szData, sizeof(szData), LIST_PATH))
				{
					token = _tcstok(szData, sep);
					pUnit[i].Torq.dLeft = _ttof(token);
					token = _tcstok(NULL, sep);
					pUnit[i].Torq.dRight = _ttof(token);
				}
			}
		}
	}

	void AllocMem()
	{
		if (nTotalUnit > 0)
		{
			if (pUnitList)
				delete[] pUnitList;
			pUnitList = new int[nTotalUnit];

			if (pUnit)
				delete[] pUnit;
			pUnit = new stUnit[nTotalUnit];
		}
	}
};

struct stModelTorque
{
	CString sModel;
	int nThick;			// 0 ~ 3 :  설정없음[0], 얇은 두께[1], 중간 두께[2], 두꺼운 두께[3]

	stModelTorque()
	{
		sModel = _T("");
		nThick = 0;
	}
};

struct stListTorque
{
	int nTotalMachines;
	int nTotalThicknesses;
	int *pUnitList;		// 호기 번호 List
	int nTotalModels;
	stModelTorque *pModel;
	stThick *pThick;	// 두께별 기본 토크값 (기본+3단계) : 설정없음[0], 얇은 두께[1], 중간 두께[2], 두꺼운 두께[3]

	int i, nTotUnits;
	TCHAR szData[MAX_PATH];
	TCHAR sep[6]; // _T(",/;\r\n\t");
	TCHAR *token;
	CString sModelIdx;

	stListTorque()
	{
		nTotalMachines = 0;
		nTotalThicknesses = 0;
		pUnitList = NULL;
		pThick = NULL;
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
	~stListTorque()
	{
		if (pUnitList)
		{
			delete[] pUnitList;
			pUnitList = NULL;
		}
		if (pThick)
		{
			delete[] pThick;
			pThick = NULL;
		}
		if (pModel)
		{
			delete[] pModel;
			pModel = NULL;
		}
	}

	void LoadList()
	{
		if (0 < ::GetPrivateProfileString(_T("Info"), _T("Total_Machines"), NULL, szData, sizeof(szData), LIST_PATH))
			nTotalMachines = _ttoi(szData);
		else
			nTotalMachines = 0;

		if (0 < ::GetPrivateProfileString(_T("Info"), _T("Total_Thicknesses"), NULL, szData, sizeof(szData), LIST_PATH))
			nTotalThicknesses = _ttoi(szData);
		else
			nTotalThicknesses = 0;

		if (0 < ::GetPrivateProfileString(_T("Info"), _T("Total_Models"), NULL, szData, sizeof(szData), LIST_PATH))
			nTotalModels = _ttoi(szData);
		else
			nTotalModels = 0;

		AllocMem();

		if (nTotalMachines > 0)
		{
			if (0 < ::GetPrivateProfileString(_T("Info"), _T("Unit_Nums"), NULL, szData, sizeof(szData), LIST_PATH))
			{
				token = _tcstok(szData, sep);
				for (i = 0; i < nTotalMachines; i++)
				{
					pUnitList[i] = _ttoi(token);
					if (i < nTotalMachines - 1)
						token = _tcstok(NULL, sep);
				}
			}
		}

		if (nTotalThicknesses > 0)
		{
			for (i = 0; i < nTotalThicknesses; i++)
			{
				pThick[i].LoadUnit(i); //  설정없음[0], 얇은 두께[1], 중간 두께[2], 두꺼운 두께[3] : 두께별로 모든 Unit(11 ~ 14호기)에 대한 좌/우 토크값을 로딩함.
			}
		}

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
					pModel[i].nThick = _ttoi(token);
				}
			}
		}
	}

	void ReloadList()
	{
		if (pUnitList)
		{
			delete[] pUnitList;
			pUnitList = NULL;
		}

		if (pThick)
		{
			delete[] pThick;
			pThick = NULL;
		}

		if (pModel)
		{
			delete[] pModel;
			pModel = NULL;
		}

		LoadList();
	}

	void AllocMem()
	{
		if (nTotalMachines > 0)
		{
			if (pUnitList)
				delete[] pUnitList;

			pUnitList = new int[nTotalMachines];
		}

		if (nTotalThicknesses > 0)	// 두께별 기본 토크값 (기본+3단계) : 설정없음[0], 얇은 두께[1], 중간 두께[2], 두꺼운 두께[3]
		{
			if (pThick)
				delete[] pThick;

			pThick = new stThick[nTotalThicknesses];
		}

		if (nTotalModels > 0)
		{
			if (pModel)
				delete[] pModel;

			pModel = new stModelTorque[nTotalModels];
		}
	}
};


// Color constants.
const COLORREF rgbRed = 0x000000FF;
const COLORREF rgbGreen = 0x0000FF00;
const COLORREF rgbBlue = 0x00FF0000;
const COLORREF rgbBlack = 0x00000000;
const COLORREF rgbWhite = 0x00FFFFFF;


// DlgPunchingTorque 대화 상자입니다.

class CDlgPunchingTorque : public CDialog
{
	DECLARE_DYNAMIC(CDlgPunchingTorque)
private:
	stListTorque m_stListTorque;
	int m_nTotalUnit, m_nTotalModel, m_nTotalThick;
	CString m_sPathCamSpecDir, m_sModel;
	int m_nUnit;
	int m_nThickModel, m_nThickUnit;

	void LoadIni();
	void InitList();
	void InitModel();
	void ModifyModelData();
	void ModifyThicknessData();
	BOOL DirectoryExists(LPCTSTR szPath);
	void StringToChar(CString str, char* pCh); // char* returned must be deleted... 
	void ModifyComboThickModel(int nThick);
	void ModifyComboThickUnit(int nUnit, int nThick);
	void ModifyEditTorqueUnit(int nUnit, int nThick);
	BOOL DeleteModel(CString sModel);

public:
	CDlgPunchingTorque(CWnd* pParent = NULL);   // 표준 생성자입니다.
	virtual ~CDlgPunchingTorque();

// 대화 상자 데이터입니다.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DLG_PUNCHING_TORQUE };
#else
	enum { IDD = IDD_DLG_PUNCHING_TORQUE };
#endif

public:
	BOOL Create();
	void Log(CString strMsg, int nType = 0);

protected:
	CListCtrl m_List;

	int GetModelThickness(CString sModel);
	CString GetThicknessName(int nIdx); // 설정없음[0], 얇은 두께[1], 중간 두께[2], 두꺼운 두께[3]
	BOOL UpdateModel(CString sModel, int nThickness);
	BOOL UpdateUnit(int nUnit, int nThickness, double dTorqL, double dTorqR);
	int SearchModel(CString sModel);
	void InsertModel(CString sModel, int nThickness);
	void ModifyModel(CString sModel, int nThickness);
	int SearchUnit(int nUnit);
	void InsertUnit(int nUnit, int nThickness, double dTorqL, double dTorqR);
	void ModifyUnit(int nUnit, int nThickness, double dTorqL, double dTorqR);
	void UpdateList();
	BOOL GetTorque(int nUnit, int nThick, double &dTorqL, double &dTorqR);
	void ResetComboThickModel();
	void ResetEditUnit();
	void DispList();
	int SearchModelInCombo(CString sModel);
	void SelectList(CString sModel);

protected:
	virtual BOOL OnInitDialog();
	afx_msg void OnSelchangeComboModel();
	afx_msg void OnSelchangeComboThickModel();
	afx_msg void OnSelchangeComboThickUnit();
	afx_msg void OnBnClickedButtonSaveModel();
	afx_msg void OnBnClickedButtonSaveUnit();
	afx_msg void OnChangeEditUnit();
	afx_msg void OnBnClickedButtonRefreshModel();
	afx_msg void OnClickList1(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnBnClickedButtonFindModel();
	afx_msg void OnBnClickedButtonDeleteModel();

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()
};
