// DlgLaserSetting.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "RTR_Manager.h"
#include "DlgLaserSetting.h"
#include "afxdialogex.h"


// CDlgLaserSetting 대화 상자입니다.

IMPLEMENT_DYNAMIC(CDlgLaserSetting, CDialog)

CDlgLaserSetting::CDlgLaserSetting(CWnd* pParent /*=NULL*/)
	: CDialog(IDD_DLG_LASER_SETTING, pParent)
{
	m_nMachine = -1;
	m_nPreset = -1;
	m_sMachine = _T("");
	m_sPreset = _T("");

	// 인쇄 데이터
	m_nSymbolSize = 1;			// [ ]
	m_dCellSize = 0.2;			// [mm]
	m_dHeight = 5;				// [mm]
	m_dWidth = 0.2;				// [mm]

	// 인쇄 조건
	m_dLaserPower = 80.0;		// [%]
	m_nScanSpeed = 1000;		// [mm/s]
	m_nPulseFrequency = 100;	// [kHz]
	m_nSpotChange = 0;			// [ ] 
	m_nPrintRepeat = 1;			// [회]
}

CDlgLaserSetting::~CDlgLaserSetting()
{
}

void CDlgLaserSetting::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CDlgLaserSetting, CDialog)
	ON_CBN_SELCHANGE(IDC_COMBO_MACHINE, &CDlgLaserSetting::OnSelchangeComboMachine)
	ON_CBN_SELCHANGE(IDC_COMBO_PRESET, &CDlgLaserSetting::OnSelchangeComboPreset)
	ON_BN_CLICKED(IDC_BUTTON_SAVE_JOB, &CDlgLaserSetting::OnBnClickedButtonSaveJob)
END_MESSAGE_MAP()


// CDlgLaserSetting 메시지 처리기입니다.

BOOL CDlgLaserSetting::Create()
{
	return CDialog::Create(CDlgLaserSetting::IDD);
}


BOOL CDlgLaserSetting::OnInitDialog()
{
	CDialog::OnInitDialog();

	// TODO:  여기에 추가 초기화 작업을 추가합니다.
	Init();

	return TRUE;  // return TRUE unless you set the focus to a control
				  // 예외: OCX 속성 페이지는 FALSE를 반환해야 합니다.
}

void CDlgLaserSetting::Init()
{
	LoadIni();
	InitVal();
	InitComboMachine();
}

void CDlgLaserSetting::LoadIni()
{
	m_nMachine = m_stIni.LastJob.nMachine;
	m_nPreset = m_stIni.LastJob.nPreset;
}

void CDlgLaserSetting::InitVal()
{
	// 인쇄 데이터
	GetDlgItem(IDC_EDIT_SYMBOL_SIZE)->SetWindowText(_T(""));
	GetDlgItem(IDC_EDIT_CELL_SIZE)->SetWindowText(_T(""));
	GetDlgItem(IDC_EDIT_WIDTH)->SetWindowText(_T(""));
	GetDlgItem(IDC_EDIT_HEIGHT)->SetWindowText(_T(""));

	// 인쇄 조건
	GetDlgItem(IDC_EDIT_LASER_POWER)->SetWindowText(_T(""));
	GetDlgItem(IDC_EDIT_SCAN_SPEED)->SetWindowText(_T(""));
	GetDlgItem(IDC_EDIT_PULSE_FREQUENCY)->SetWindowText(_T(""));
	GetDlgItem(IDC_EDIT_SPOT_CHANGE)->SetWindowText(_T(""));
	GetDlgItem(IDC_EDIT_PRINT_NUM)->SetWindowText(_T(""));
}

void CDlgLaserSetting::InitComboMachine()
{
	if (!m_stIni.Machine) return;

	int nTot = m_stIni.nTotalMachine;
	if (nTot < 1) return;

	CComboBox* pCombo = (CComboBox*)GetDlgItem(IDC_COMBO_MACHINE);
	pCombo->ResetContent();

	CString sItem;
	for (int i = 0; i < nTot; i++)
	{
		sItem = m_stIni.Machine[i].sName;
		pCombo->InsertString(i, sItem);
	}

	if (m_nMachine > -1)
	{
		pCombo->SetCurSel(m_nMachine);
		InitComboPreset(m_nMachine);
	}
}

void CDlgLaserSetting::InitComboPreset(int nMachine)
{
	ModifyPreset(nMachine);
	//if (!m_sPreset.IsEmpty())
	if(m_nPreset > -1)
	{
		//m_nPreset = GetIdxComboPreset(m_sPreset);
		CComboBox* pCombo = (CComboBox*)GetDlgItem(IDC_COMBO_PRESET);
		pCombo->SetCurSel(m_nPreset);
		InitPreset(m_nPreset);
	}
}

int CDlgLaserSetting::GetIdxComboPreset(CString sPreset)
{
	int nIdx = -1;
	CComboBox* pCombo = (CComboBox*)GetDlgItem(IDC_COMBO_PRESET);
	int nCount = pCombo->GetCount();
	CString sLine = _T("");
	for (int i = 0; i < nCount; i++)
	{
		pCombo->GetLBText(i, sLine);
		if (sLine == sPreset)
			return i;
	}
	return nIdx;
}

void CDlgLaserSetting::ModifyPreset(int nMachine)
{
	if (!m_stIni.Machine) return;

	int nTotJob = m_stIni.Machine[nMachine].nTotalJob;
	if (nTotJob < 1) return;

	CComboBox* pCombo = (CComboBox*)GetDlgItem(IDC_COMBO_PRESET);
	pCombo->ResetContent();

	CString sItem;
	for (int i = 0; i < nTotJob; i++)
	{
		sItem = m_stIni.Machine[nMachine].Job[i].sTitle;
		pCombo->InsertString(i, sItem);
	}

	if (m_nPreset > -1)	pCombo->SetCurSel(m_nPreset);
}

void CDlgLaserSetting::OnSelchangeComboMachine()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	CComboBox* pCombo = (CComboBox*)GetDlgItem(IDC_COMBO_MACHINE);
	int nIndex = pCombo->GetCurSel();
	if (nIndex != LB_ERR)
	{
		m_nMachine = nIndex;
		pCombo->GetLBText(m_nMachine, m_sMachine);
		if (m_nMachine > -1)
		{
			pCombo->SetCurSel(m_nMachine);
			InitComboPreset(m_nMachine);
		}
	}
}


void CDlgLaserSetting::OnSelchangeComboPreset()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	CComboBox* pCombo = (CComboBox*)GetDlgItem(IDC_COMBO_PRESET);
	int nIndex = pCombo->GetCurSel();
	if (nIndex != LB_ERR)
	{
		m_nPreset = nIndex;
		pCombo->GetLBText(m_nPreset, m_sPreset);
		if (m_nPreset > -1)
		{
			pCombo->SetCurSel(m_nPreset);
			InitPreset(m_nPreset);
		}
	}
}

void CDlgLaserSetting::InitPreset(int nJobIdx)
{
	CString sVal;

	// 인쇄 데이터
	m_nSymbolSize = m_stIni.Machine[m_nMachine].Job[nJobIdx].nSymbolSize;			// [ ]
	m_dCellSize = m_stIni.Machine[m_nMachine].Job[nJobIdx].dCellSize;				// [mm]
	m_dHeight = m_stIni.Machine[m_nMachine].Job[nJobIdx].dHeight;					// [mm]
	m_dWidth = m_stIni.Machine[m_nMachine].Job[nJobIdx].dWidth;						// [mm]

	// 인쇄 조건
	m_dLaserPower = m_stIni.Machine[m_nMachine].Job[nJobIdx].dLaserPower;			// [%]
	m_nScanSpeed = m_stIni.Machine[m_nMachine].Job[nJobIdx].nScanSpeed;				// [mm/s]
	m_nPulseFrequency = m_stIni.Machine[m_nMachine].Job[nJobIdx].nPulseFrequency;	// [kHz]
	m_nSpotChange = m_stIni.Machine[m_nMachine].Job[nJobIdx].nSpotChange;			// [ ] 
	m_nPrintRepeat = m_stIni.Machine[m_nMachine].Job[nJobIdx].nPrintRepeat;			// [회]

	// 인쇄 데이터	
	sVal.Format(_T("%d"), m_nSymbolSize); GetDlgItem(IDC_EDIT_SYMBOL_SIZE)->SetWindowText(sVal);
	sVal.Format(_T("%.1f"), m_dCellSize); GetDlgItem(IDC_EDIT_CELL_SIZE)->SetWindowText(sVal);
	sVal.Format(_T("%.1f"), m_dHeight); GetDlgItem(IDC_EDIT_WIDTH)->SetWindowText(sVal);
	sVal.Format(_T("%.1f"), m_dWidth); GetDlgItem(IDC_EDIT_HEIGHT)->SetWindowText(sVal);

	// 인쇄 조건
	sVal.Format(_T("%.1f"), m_dLaserPower); GetDlgItem(IDC_EDIT_LASER_POWER)->SetWindowText(sVal);
	sVal.Format(_T("%d"), m_nScanSpeed); GetDlgItem(IDC_EDIT_SCAN_SPEED)->SetWindowText(sVal);
	sVal.Format(_T("%d"), m_nPulseFrequency); GetDlgItem(IDC_EDIT_PULSE_FREQUENCY)->SetWindowText(sVal);
	sVal.Format(_T("%d"), m_nSpotChange); GetDlgItem(IDC_EDIT_SPOT_CHANGE)->SetWindowText(sVal);
	sVal.Format(_T("%d"), m_nPrintRepeat); GetDlgItem(IDC_EDIT_PRINT_NUM)->SetWindowText(sVal);
}

void CDlgLaserSetting::UpdateVal()
{
	CString sVal;

	// 인쇄 데이터	
	GetDlgItem(IDC_EDIT_SYMBOL_SIZE)->GetWindowText(sVal); m_nSymbolSize = _ttoi(sVal);
	GetDlgItem(IDC_EDIT_CELL_SIZE)->GetWindowText(sVal); m_dCellSize = _ttof(sVal);
	GetDlgItem(IDC_EDIT_WIDTH)->GetWindowText(sVal); m_dHeight = _ttof(sVal);
	GetDlgItem(IDC_EDIT_HEIGHT)->GetWindowText(sVal); m_dWidth = _ttof(sVal);

	// 인쇄 조건
	GetDlgItem(IDC_EDIT_LASER_POWER)->GetWindowText(sVal); m_dLaserPower = _ttof(sVal);
	GetDlgItem(IDC_EDIT_SCAN_SPEED)->GetWindowText(sVal); m_nScanSpeed = _ttoi(sVal);
	GetDlgItem(IDC_EDIT_PULSE_FREQUENCY)->GetWindowText(sVal); m_nPulseFrequency = _ttoi(sVal);
	GetDlgItem(IDC_EDIT_SPOT_CHANGE)->GetWindowText(sVal); m_nSpotChange = _ttoi(sVal);
	GetDlgItem(IDC_EDIT_PRINT_NUM)->GetWindowText(sVal); m_nPrintRepeat = _ttoi(sVal);

	// 인쇄 데이터
	m_stIni.Machine[m_nMachine].Job[m_nPreset].nSymbolSize = m_nSymbolSize;				// [ ]
	m_stIni.Machine[m_nMachine].Job[m_nPreset].dCellSize = m_dCellSize;					// [mm]
	m_stIni.Machine[m_nMachine].Job[m_nPreset].dHeight = m_dHeight;						// [mm]
	m_stIni.Machine[m_nMachine].Job[m_nPreset].dWidth = m_dWidth;						// [mm]

	// 인쇄 조건
	m_stIni.Machine[m_nMachine].Job[m_nPreset].dLaserPower = m_dLaserPower;				// [%]
	m_stIni.Machine[m_nMachine].Job[m_nPreset].nScanSpeed = m_nScanSpeed;				// [mm/s]
	m_stIni.Machine[m_nMachine].Job[m_nPreset].nPulseFrequency = m_nPulseFrequency;		// [kHz]
	m_stIni.Machine[m_nMachine].Job[m_nPreset].nSpotChange = m_nSpotChange;				// [ ] 
	m_stIni.Machine[m_nMachine].Job[m_nPreset].nPrintRepeat = m_nPrintRepeat;			// [회]

	m_stIni.Machine[m_nMachine].Save(m_nPreset);
}

void CDlgLaserSetting::OnBnClickedButtonSaveJob()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	CString sMsg;
	sMsg.Format(_T("%s 의 레이저용 Preset: %s 를 위의 값으로 변경하시겠습니까?"), m_sMachine, m_sPreset);
	if (IDNO == MessageBox(sMsg, _T("주의"), MB_YESNO | MB_ICONQUESTION))
		return;

	UpdateVal();
	sMsg.Format(_T("%s 의 레이저용 Preset: %s 적용이 완료되었습니다."), m_sMachine, m_sPreset);
	AfxMessageBox(sMsg);
}


