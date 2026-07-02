#pragma once

#define INI_PATH			_T("C:\\R2RSet\\Keyence.ini")
#define INI_PATH_MACHINE	_T("R2RSet\\Keyence.ini")

struct stLastJob
{
	int nMachine, nPreset;

	stLastJob()
	{
		nMachine = -1;
		nPreset = -1;
	}
};

struct stJob
{
	CString sTitle;

	// 인쇄 데이터
	int nSymbolSize;
	double dCellSize, dHeight, dWidth;

	// 인쇄 조건
	double dLaserPower;
	int nScanSpeed, nPulseFrequency, nSpotChange, nPrintRepeat;

	TCHAR szData[MAX_PATH];
	CString sItem, sData;

	stJob()
	{
		sTitle = _T("");

		// 인쇄 데이터
		nSymbolSize = 1;		// [ ]
		dCellSize = 0.2;		// [mm]
		dHeight = 5;			// [mm]
		dWidth = 0.2;			// [mm]

		// 인쇄 조건
		dLaserPower = 80.0;		// [%]
		nScanSpeed = 1000;		// [mm/s]
		nPulseFrequency = 100;	// [kHz]
		nSpotChange = 0;		// [ ] 
		nPrintRepeat = 1;		// [회]
	}
};

struct stMachine
{
	CString sName, sPath, sPathJob;
	int nTotalJob;
	stJob* Job;
	int i;
	TCHAR szData[MAX_PATH];
	CString sItem, sData;

	stMachine()
	{
		sName = _T("");	sPath = _T("");	sPathJob = _T("");
		nTotalJob = 0;
		Job = NULL;
	}

	~stMachine()
	{
		if (Job)
		{
			delete[] Job;
			Job = NULL;
		}
	}

	BOOL Load()
	{
		if (sPathJob.IsEmpty())
			return FALSE;

		if (0 < ::GetPrivateProfileString(_T("Job Info"), _T("Total"), NULL, szData, sizeof(szData), sPathJob))
			nTotalJob = _ttoi(szData);
		else
			nTotalJob = 0;


		if (nTotalJob > 0)
		{
			if (Job)
				delete[] Job;

			Job = new stJob[nTotalJob];

			for (i = 0; i < nTotalJob; i++)
			{
				sItem.Format(_T("%d"), i);
				if (0 < ::GetPrivateProfileString(_T("Job Info"), sItem, NULL, szData, sizeof(szData), sPathJob))
					Job[i].sTitle.Format(_T("%s"), szData);
				else
					Job[i].sTitle = _T("");

				sItem.Format(_T("Job_%d"), i);
				if (0 < ::GetPrivateProfileString(sItem, _T("SymbolSize"), NULL, szData, sizeof(szData), sPathJob))
					Job[i].nSymbolSize = _ttoi(szData);
				else
					Job[i].nSymbolSize = 1;

				if (0 < ::GetPrivateProfileString(sItem, _T("CellSize"), NULL, szData, sizeof(szData), sPathJob))
					Job[i].dCellSize = _ttof(szData);
				else
					Job[i].dCellSize = 0.2;

				if (0 < ::GetPrivateProfileString(sItem, _T("Height"), NULL, szData, sizeof(szData), sPathJob))
					Job[i].dHeight = _ttof(szData);
				else
					Job[i].dHeight = 5.0;

				if (0 < ::GetPrivateProfileString(sItem, _T("Width"), NULL, szData, sizeof(szData), sPathJob))
					Job[i].dWidth = _ttof(szData);
				else
					Job[i].dWidth = 0.2;

				if (0 < ::GetPrivateProfileString(sItem, _T("LaserPower"), NULL, szData, sizeof(szData), sPathJob))
					Job[i].dLaserPower = _ttof(szData);
				else
					Job[i].dLaserPower = 80.0;

				if (0 < ::GetPrivateProfileString(sItem, _T("ScanSpeed"), NULL, szData, sizeof(szData), sPathJob))
					Job[i].nScanSpeed = _ttoi(szData);
				else
					Job[i].nScanSpeed = 1000;

				if (0 < ::GetPrivateProfileString(sItem, _T("PulseFrequency"), NULL, szData, sizeof(szData), sPathJob))
					Job[i].nPulseFrequency = _ttoi(szData);
				else
					Job[i].nPulseFrequency = 1;

				if (0 < ::GetPrivateProfileString(sItem, _T("SpotChange"), NULL, szData, sizeof(szData), sPathJob))
					Job[i].nSpotChange = _ttoi(szData);
				else
					Job[i].nSpotChange = 0;
			}
		}

		return TRUE;
	}

	BOOL Save(int nJobIdx)
	{
		if (sPathJob.IsEmpty())
			return FALSE;

		sItem.Format(_T("Job_%d"), nJobIdx);
		sData.Format(_T("%d"), Job[nJobIdx].nSymbolSize);
		::WritePrivateProfileString(sItem, _T("SymbolSize"), sData, sPathJob);

		sData.Format(_T("%.1f"), Job[nJobIdx].dCellSize);
		::WritePrivateProfileString(sItem, _T("CellSize"), sData, sPathJob);

		sData.Format(_T("%.1f"), Job[nJobIdx].dHeight);
		::WritePrivateProfileString(sItem, _T("Height"), sData, sPathJob);

		sData.Format(_T("%.1f"), Job[nJobIdx].dWidth);
		::WritePrivateProfileString(sItem, _T("Width"), sData, sPathJob);

		sData.Format(_T("%.1f"), Job[nJobIdx].dLaserPower);
		::WritePrivateProfileString(sItem, _T("LaserPower"), sData, sPathJob);

		sData.Format(_T("%d"), Job[nJobIdx].nScanSpeed);
		::WritePrivateProfileString(sItem, _T("ScanSpeed"), sData, sPathJob);

		sData.Format(_T("%d"), Job[nJobIdx].nPulseFrequency);
		::WritePrivateProfileString(sItem, _T("PulseFrequency"), sData, sPathJob);

		sData.Format(_T("%d"), Job[nJobIdx].nSpotChange);
		::WritePrivateProfileString(sItem, _T("SpotChange"), sData, sPathJob);

		return TRUE;
	}

};

struct stIniLaserSetting
{
	stLastJob LastJob;
	stMachine* Machine;
	int nTotalMachine;

	int i;
	TCHAR szData[MAX_PATH];
	TCHAR sep[6]; // _T(",/;\r\n\t");
	TCHAR *token;
	CString sItem, sData;

	stIniLaserSetting()
	{
		nTotalMachine = 0;
		Machine = NULL;

		sep[0] = _T(',');
		sep[1] = _T('/');
		sep[2] = _T(';');
		sep[3] = _T('\r');
		sep[4] = _T('\n');
		sep[5] = _T('\t');
		token = NULL;
		sItem = _T("");

		Load();
	}

	~stIniLaserSetting()
	{
		if (Machine)
		{
			delete[] Machine;
			Machine = NULL;
		}
	}

	void Load()
	{
		if (0 < ::GetPrivateProfileString(_T("Machine Info"), _T("Total"), NULL, szData, sizeof(szData), INI_PATH))
			nTotalMachine = _ttoi(szData);
		else
			nTotalMachine = 0;

		if (nTotalMachine > 0)
		{
			if (Machine)
				delete[] Machine;

			Machine = new stMachine[nTotalMachine];

			for (i = 0; i < nTotalMachine; i++)
			{
				sItem.Format(_T("Machine_%d"), i);
				if (0 < ::GetPrivateProfileString(sItem, _T("Name"), NULL, szData, sizeof(szData), INI_PATH))
					Machine[i].sName = CString(szData);
				else
					Machine[i].sName = _T("");

				if (0 < ::GetPrivateProfileString(sItem, _T("Address"), NULL, szData, sizeof(szData), INI_PATH))
					Machine[i].sPath = CString(szData);
				else
					Machine[i].sPath = _T("");

				if (!Machine[i].sPath.IsEmpty())
					Machine[i].sPathJob = Machine[i].sPath + INI_PATH_MACHINE;
				else
					Machine[i].sPathJob = _T("");

				Machine[i].Load();
			}
		}
		
		if (0 < ::GetPrivateProfileString(_T("Last Job"), _T("Machine No"), NULL, szData, sizeof(szData), INI_PATH))
			LastJob.nMachine = _ttoi(szData);
		else
			LastJob.nMachine = -1;

		if (0 < ::GetPrivateProfileString(_T("Last Job"), _T("Preset No"), NULL, szData, sizeof(szData), INI_PATH))
			LastJob.nPreset = _ttoi(szData);
		else
			LastJob.nPreset = -1;
	}
};


// CDlgLaserSetting 대화 상자입니다.

class CDlgLaserSetting : public CDialog
{
	DECLARE_DYNAMIC(CDlgLaserSetting)

	int m_nMachine, m_nPreset;
	stIniLaserSetting m_stIni;
	CString m_sMachine, m_sPreset;

	// 인쇄 데이터
	int m_nSymbolSize;		// [ ]
	double m_dCellSize;		// [mm]
	double m_dHeight;		// [mm]
	double m_dWidth;		// [mm]

	// 인쇄 조건
	double m_dLaserPower;	// [%]
	int m_nScanSpeed;		// [mm/s]
	int m_nPulseFrequency;	// [kHz]
	int m_nSpotChange;		// [ ] 
	int m_nPrintRepeat;		// [회]

	void LoadIni();
	void Init();
	void InitVal();
	void InitComboMachine();
	void InitComboPreset(int nMachine);
	void ModifyPreset(int nMachine);
	int GetIdxComboPreset(CString sPreset);
	void InitPreset(int nJobIdx);
	void UpdateVal();

public:
	CDlgLaserSetting(CWnd* pParent = NULL);   // 표준 생성자입니다.
	virtual ~CDlgLaserSetting();

// 대화 상자 데이터입니다.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DLG_LASER_SETTING };
#else
	enum { IDD = IDD_DLG_LASER_SETTING };
#endif

public:
	BOOL Create();

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	afx_msg void OnSelchangeComboMachine();
	afx_msg void OnSelchangeComboPreset();
	afx_msg void OnBnClickedButtonSaveJob();
};
