// DlgPunchingTorque.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "RTR_Manager.h"
#include "DlgPunchingTorque.h"
#include "afxdialogex.h"

#include "safelock.h"
CCriticalSection g_LogLock;

// DlgPunchingTorque 대화 상자입니다.

IMPLEMENT_DYNAMIC(CDlgPunchingTorque, CDialog)

CDlgPunchingTorque::CDlgPunchingTorque(CWnd* pParent /*=NULL*/)
	: CDialog(IDD_DLG_PUNCHING_TORQUE, pParent)
{
	m_sPathCamSpecDir = _T("");
	m_nTotalUnit = 0;
	m_nTotalModel = 0;
	m_nTotalThick = 0;
	m_sModel = _T("");
	m_nUnit = -1; // Not Selected
	m_nThickModel = 0;
	m_nThickUnit = 0;
}

CDlgPunchingTorque::~CDlgPunchingTorque()
{
}

void CDlgPunchingTorque::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST1, m_List);
}


BEGIN_MESSAGE_MAP(CDlgPunchingTorque, CDialog)
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_CBN_SELCHANGE(IDC_COMBO_MODEL, &CDlgPunchingTorque::OnSelchangeComboModel)
	ON_CBN_SELCHANGE(IDC_COMBO_THICK_MODEL, &CDlgPunchingTorque::OnSelchangeComboThickModel)
	ON_CBN_SELCHANGE(IDC_COMBO_THICK_UNIT, &CDlgPunchingTorque::OnSelchangeComboThickUnit)
	ON_BN_CLICKED(IDC_BUTTON_SAVE_MODEL, &CDlgPunchingTorque::OnBnClickedButtonSaveModel)
	ON_BN_CLICKED(IDC_BUTTON_SAVE_UNIT, &CDlgPunchingTorque::OnBnClickedButtonSaveUnit)
	ON_EN_CHANGE(IDC_EDIT_UNIT, &CDlgPunchingTorque::OnChangeEditUnit)
	ON_BN_CLICKED(IDC_BUTTON_REFRESH_MODEL, &CDlgPunchingTorque::OnBnClickedButtonRefreshModel)
	ON_NOTIFY(NM_CLICK, IDC_LIST1, &CDlgPunchingTorque::OnClickList1)
	ON_BN_CLICKED(IDC_BUTTON_FIND_MODEL, &CDlgPunchingTorque::OnBnClickedButtonFindModel)
	ON_BN_CLICKED(IDC_BUTTON_DELETE_MODEL, &CDlgPunchingTorque::OnBnClickedButtonDeleteModel)
END_MESSAGE_MAP()


// DlgPunchingTorque 메시지 처리기입니다.

BOOL CDlgPunchingTorque::Create()
{
	return CDialog::Create(CDlgPunchingTorque::IDD);
}

BOOL CDlgPunchingTorque::OnInitDialog()
{
	CDialog::OnInitDialog();

	// TODO: 여기에 추가 초기화 작업을 추가합니다.

	LoadIni();
	InitList();
	DispList();

	ModifyModelData();
	ModifyThicknessData();

	((CComboBox*)GetDlgItem(IDC_COMBO_MODEL))->SelectString(0, m_sModel);
	((CComboBox*)GetDlgItem(IDC_COMBO_MODEL))->SetWindowText(m_sModel);

	return TRUE;  // 포커스를 컨트롤에 설정하지 않으면 TRUE를 반환합니다.
}


void CDlgPunchingTorque::InitList()
{
	// Addition ListControl
	int nIdxUnit;
	CString strItem = _T("");

	m_List.DeleteAllItems();
	Sleep(30);
	m_List.SetExtendedStyle(LVS_EX_GRIDLINES | LVS_EX_FULLROWSELECT); // 격자 추가
	m_List.InsertColumn(0, _T("Index"), LVCFMT_CENTER, 60);//LVCFMT_LEFT
	m_List.InsertColumn(1, _T("모델"), LVCFMT_CENTER, 140);//LVCFMT_LEFT
	m_List.InsertColumn(2, _T("두께구분"), LVCFMT_CENTER, 100);

	m_nTotalUnit = m_stList.nTotalMachines;
	if (m_nTotalUnit < 1)
		return;

	for (nIdxUnit = 0; nIdxUnit < m_nTotalUnit; nIdxUnit++)
	{
		strItem.Format(_T("#%d 좌"), m_stList.pUnitList[nIdxUnit]);
		m_List.InsertColumn(3 + (nIdxUnit * 2), strItem, LVCFMT_CENTER, 80);
		strItem.Format(_T("#%d 우"), m_stList.pUnitList[nIdxUnit]);
		m_List.InsertColumn(4 + (nIdxUnit * 2), strItem, LVCFMT_CENTER, 80);
	}
}


void CDlgPunchingTorque::DispList()
{
	int nIdxModel, nIdxUnit;
	TCHAR tChar[200];
	CString sVal;

	m_List.DeleteAllItems();
	Sleep(30);

	for (nIdxModel = 0; nIdxModel < m_stList.nTotalModels; nIdxModel++)
	{
		LVITEM item;
		::ZeroMemory(&item, sizeof(item));
		item.mask = LVIF_TEXT | LVIF_IMAGE | LVIF_STATE;
		// Index
		item.iItem = m_List.GetItemCount();
		item.iSubItem = 0;
		item.iImage = 1;
		sVal.Format(_T("%d"), nIdxModel);
		_stprintf(tChar, _T("%s"), sVal);
		item.pszText = tChar;
		m_List.InsertItem(&item);
		// 모델
		item.iSubItem = 1;
		_stprintf(tChar, _T("%s"), m_stList.pModel[nIdxModel].sModel);
		item.pszText = tChar;
		m_List.SetItem(&item);
		// 두께구분
		item.iSubItem = 2;
		_stprintf(tChar, _T("%s"), GetThicknessName(m_stList.pModel[nIdxModel].nThick));
		item.pszText = tChar;
		m_List.SetItem(&item);

		for (nIdxUnit = 0; nIdxUnit < m_nTotalUnit; nIdxUnit++)
		{
			// "#%d 좌" - Torque
			item.iSubItem = 3 + (nIdxUnit * 2);
			sVal.Format(_T("%.3f"), m_stList.pThick[m_stList.pModel[nIdxModel].nThick].pUnit[nIdxUnit].Torq.dLeft);
			_stprintf(tChar, _T("%s"), sVal);
			item.pszText = tChar;
			m_List.SetItem(&item);
			// "#%d 우" - Torque
			item.iSubItem = 4 + (nIdxUnit * 2);
			sVal.Format(_T("%.3f"), m_stList.pThick[m_stList.pModel[nIdxModel].nThick].pUnit[nIdxUnit].Torq.dRight);
			_stprintf(tChar, _T("%s"), sVal);
			item.pszText = tChar;
			m_List.SetItem(&item);
		}

		//item.state = LVIS_SELECTED | LVIS_FOCUSED;
		//m_List.InsertItem(&item);
		//m_List.InsertItem(i, m_stList.pModel[i].sModel, 0);
	}

	m_List.ModifyStyle(LVS_TYPEMASK, LVS_REPORT);
}

CString CDlgPunchingTorque::GetThicknessName(int nIdx) // 설정없음[0], 얇은 두께[1], 중간 두께[2], 두꺼운 두께[3]
{
	CString sName = _T("");
	switch (nIdx)
	{
	case 0:
		sName = THICKNESS_0;
		break;
	case 1:
		sName = THICKNESS_1;
		break;
	case 2:
		sName = THICKNESS_2;
		break;
	case 3:
		sName = THICKNESS_3;
		break;
	default:
		sName = THICKNESS_0;
		break;
	}
	return sName;
}

void CDlgPunchingTorque::LoadIni()
{
	TCHAR szData[MAX_PATH];

	if (0 < ::GetPrivateProfileString(_T("Info"), _T("CamSpecDir"), NULL, szData, sizeof(szData), LIST_PATH))
		m_sPathCamSpecDir = CString(szData);
	else
	{
		AfxMessageBox(_T("CamMaster의 Spec디렉토리 Path가 설정되어 있지 않습니다."), MB_ICONWARNING | MB_OK);
		m_sPathCamSpecDir = CString(_T(""));
	}
}

void CDlgPunchingTorque::OnSelchangeComboModel()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	CString sModel, sMsg;
	int nThickModel;
	int nIndex = ((CComboBox*)GetDlgItem(IDC_COMBO_MODEL))->GetCurSel();
	if (nIndex != LB_ERR)
	{
		((CComboBox*)GetDlgItem(IDC_COMBO_MODEL))->GetLBText(nIndex, sModel);
		m_sModel = sModel;

		nThickModel = GetModelThickness(sModel);
		if (nThickModel < 1)
		{
			sMsg.Format(_T("%s 모델은 토크값이 설정되지 않았습니다."), sModel);
			AfxMessageBox(sMsg);
			ResetComboThickModel();
			return;
		}
		ModifyComboThickModel(nThickModel);
		m_sModel = sModel;
		m_nThickModel = nThickModel;
		SelectList(sModel);
	}
}

void CDlgPunchingTorque::ModifyComboThickModel(int nThick)
{
	if (nThick < 1)
		return;

	int nIndex = nThick - 1;
	((CComboBox*)GetDlgItem(IDC_COMBO_THICK_MODEL))->SetCurSel(nIndex);
}

void CDlgPunchingTorque::OnSelchangeComboThickModel()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	CString sThickness = _T("");

	int nIndex = ((CComboBox*)GetDlgItem(IDC_COMBO_THICK_MODEL))->GetCurSel();
	if (nIndex != LB_ERR)
	{
		((CComboBox*)GetDlgItem(IDC_COMBO_THICK_MODEL))->GetLBText(nIndex, sThickness);
	}

	if (sThickness.IsEmpty())
		return;

	if (sThickness == THICKNESS_0)
		m_nThickModel = 0;

	if (sThickness == THICKNESS_1)
		m_nThickModel = 1;

	if (sThickness == THICKNESS_2)
		m_nThickModel = 2;

	if (sThickness == THICKNESS_3)
		m_nThickModel = 3;

	ResetEditUnit();
}

void CDlgPunchingTorque::InitModel()
{
	m_sModel = _T("");
}

void CDlgPunchingTorque::ModifyModelData()
{
	((CComboBox*)GetDlgItem(IDC_COMBO_MODEL))->ResetContent();

	TCHAR FN[100];
	_stprintf(FN, _T("%s*.*"), m_sPathCamSpecDir);

	((CComboBox*)GetDlgItem(IDC_COMBO_MODEL))->Dir(0x8010, FN);
	int t = 0;

	//	"[..]"를 제거 
	//((CComboBox*)GetDlgItem(IDC_COMBO_MODEL))->DeleteString(0);
	int nIndex = ((CComboBox*)GetDlgItem(IDC_COMBO_MODEL))->FindStringExact(-1, _T("[..]"));
	if (nIndex > -1)
		((CComboBox*)GetDlgItem(IDC_COMBO_MODEL))->DeleteString(nIndex);

	int nCount = ((CComboBox*)GetDlgItem(IDC_COMBO_MODEL))->GetCount();

	CString strBuf, strBuf2;
	for (int i = 0; i < nCount; i++)
	{
		((CComboBox*)GetDlgItem(IDC_COMBO_MODEL))->GetLBText(i, strBuf);

		if (strBuf.GetLength() < 3)
			continue;
		// 기종이름에서 "["를 제거 
		CString strBuf2 = strBuf.Mid(1, strBuf.GetLength() - 2);
		((CComboBox*)GetDlgItem(IDC_COMBO_MODEL))->InsertString(t, strBuf2);
		((CComboBox*)GetDlgItem(IDC_COMBO_MODEL))->DeleteString(t + 1);
		t++;
	}
}

void CDlgPunchingTorque::ModifyThicknessData()
{
	// Model Thickness
	((CComboBox*)GetDlgItem(IDC_COMBO_THICK_MODEL))->ResetContent();
	((CComboBox*)GetDlgItem(IDC_COMBO_THICK_MODEL))->InsertString(0, GetThicknessName(1));
	((CComboBox*)GetDlgItem(IDC_COMBO_THICK_MODEL))->InsertString(1, GetThicknessName(2));
	((CComboBox*)GetDlgItem(IDC_COMBO_THICK_MODEL))->InsertString(2, GetThicknessName(3));

	// Unit Thickness
	((CComboBox*)GetDlgItem(IDC_COMBO_THICK_UNIT))->ResetContent();
	((CComboBox*)GetDlgItem(IDC_COMBO_THICK_UNIT))->InsertString(0, GetThicknessName(1));
	((CComboBox*)GetDlgItem(IDC_COMBO_THICK_UNIT))->InsertString(1, GetThicknessName(2));
	((CComboBox*)GetDlgItem(IDC_COMBO_THICK_UNIT))->InsertString(2, GetThicknessName(3));
}



void CDlgPunchingTorque::OnBnClickedButtonSaveModel()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	if (m_sModel.IsEmpty())
	{
		AfxMessageBox(_T("Model이 선택되지 않았습니다."));
		return;
	}

	if (m_nThickModel < 1)
	{
		AfxMessageBox(_T("두께가 선택되지 않았습니다."));
		return;
	}

	CString sMsg;
	sMsg.Format(_T("%s 모델의 두께값을 %s로 변경하시겠습니까?"), m_sModel, GetThicknessName(m_nThickModel));
	if (IDNO == MessageBox(sMsg, _T("주의"), MB_YESNO | MB_ICONQUESTION))
	{
		return;
	}

	UpdateModel(m_sModel, m_nThickModel);
	UpdateList();
	sMsg.Format(_T("%s 모델의 두께값을 %s로 변경"), m_sModel, GetThicknessName(m_nThickModel));
	Log(sMsg);
}


void CDlgPunchingTorque::OnBnClickedButtonSaveUnit()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	CString sUnit, sTorqueLeft, sTorqueRight;
	GetDlgItem(IDC_EDIT_UNIT)->GetWindowText(sUnit);
	GetDlgItem(IDC_EDIT_TORQUE_LEFT)->GetWindowText(sTorqueLeft);
	GetDlgItem(IDC_EDIT_TORQUE_RIGHT)->GetWindowText(sTorqueRight);

	if (sUnit.IsEmpty())
	{
		AfxMessageBox(_T("호기 번호가 선택되지 않았습니다."));
		return;
	}

	if (sTorqueLeft.IsEmpty())
	{
		AfxMessageBox(_T("왼쪽 토크값이 설정되지 않았습니다."));
		return;
	}

	if (sTorqueRight.IsEmpty())
	{
		AfxMessageBox(_T("오른쪽 토크값이 설정되지 않았습니다."));
		return;
	}

	if (m_nThickUnit < 1)
	{
		AfxMessageBox(_T("두께가 선택되지 않았습니다."));
		return;
	}

	int nUnit = _tstoi(sUnit);
	double dTorqL = _tstof(sTorqueLeft);
	double dTorqR = _tstof(sTorqueRight);

	m_nUnit = nUnit;

	CString sMsg;
	sMsg.Format(_T("#%d 호기의 %s의 토크값을 좌: %.3f , 우: %.3f 으로 변경하시겠습니까?"), nUnit, GetThicknessName(m_nThickUnit), dTorqL, dTorqR);
	if (IDNO == MessageBox(sMsg, _T("주의"), MB_YESNO | MB_ICONQUESTION))
	{
		return;
	}

	UpdateUnit(nUnit, m_nThickUnit, dTorqL, dTorqR);
	UpdateList();
	sMsg.Format(_T("#%d 호기의 %s의 토크값을 좌: %.3f , 우: %.3f 으로 변경"), nUnit, GetThicknessName(m_nThickUnit), dTorqL, dTorqR);
	Log(sMsg);
}

BOOL CDlgPunchingTorque::UpdateModel(CString sModel, int nThickness)
{
	int nIdx = SearchModel(sModel);

	if (nIdx < 0) // Not Found
	{
		InsertModel(sModel, nThickness);
	}
	else
	{
		ModifyModel(sModel, nThickness);
	}

	return TRUE;
}

int CDlgPunchingTorque::SearchUnit(int nUnit)
{
	int nIdxUnit;
	CString sModelIdx, sListModel;
	TCHAR szData[MAX_PATH];
	TCHAR sep[] = { _T(",;\r\n\t") };
	TCHAR *token;
	int nListUnit;

	m_nTotalUnit = m_stList.nTotalMachines;


	if (0 < ::GetPrivateProfileString(_T("Info"), _T("Unit_Nums"), NULL, szData, sizeof(szData), LIST_PATH))
	{
		token = _tcstok(szData, sep);
		for (nIdxUnit = 0; nIdxUnit < m_stList.nTotalMachines; nIdxUnit++)
		{
			nListUnit = _ttoi(token);

			if (nListUnit == nUnit)
				return nIdxUnit;

			if (nIdxUnit < m_stList.nTotalMachines - 1)
				token = _tcstok(NULL, sep);
		}
	}

	return -1; // Not Found
}

void CDlgPunchingTorque::InsertModel(CString sModel, int nThickness)
{
	CString sModelIdx, strData;
	int nModelIdx = m_stList.nTotalModels;
	sModelIdx.Format(_T("%d"), nModelIdx);
	strData.Format(_T("%s,%d"), sModel, nThickness);
	::WritePrivateProfileString(_T("Model"), sModelIdx, strData, LIST_PATH);
	m_stList.nTotalModels++;

	strData.Format(_T("%d"), m_stList.nTotalModels);
	::WritePrivateProfileString(_T("Info"), _T("Total_Models"), strData, LIST_PATH);
}

void CDlgPunchingTorque::ModifyModel(CString sModel, int nThickness)
{
	CString sModelIdx, strData;
	int nModelIdx = SearchModel(sModel);
	sModelIdx.Format(_T("%d"), nModelIdx);
	strData.Format(_T("%s,%d"), sModel, nThickness);
	::WritePrivateProfileString(_T("Model"), sModelIdx, strData, LIST_PATH);
}

BOOL CDlgPunchingTorque::UpdateUnit(int nUnit, int nThickness, double dTorqL, double dTorqR)
{
	int nIdx = SearchUnit(nUnit);

	if (nIdx < 0) // Not Found
	{
		InsertUnit(nUnit, nThickness, dTorqL, dTorqR);
	}
	else
	{
		ModifyUnit(nUnit, nThickness, dTorqL, dTorqR);
	}

	return TRUE;
}

void CDlgPunchingTorque::InsertUnit(int nUnit, int nThickness, double dTorqL, double dTorqR)
{
	CString sUnits, strItem, strData, strTorq;
	m_nTotalUnit = m_stList.nTotalMachines;

	if (m_nTotalUnit < 0)
		return;

	if (nThickness < 1)
		return;

	if (dTorqL < 0.0 || dTorqR < 0.0)
		return;

	int nUnitIdx = m_nTotalUnit;

	m_nTotalUnit++;
	strData.Format(_T("%d"), m_nTotalUnit);
	::WritePrivateProfileString(_T("Info"), _T("Total_Models"), strData, LIST_PATH);

	TCHAR szData[MAX_PATH];
	if (0 < ::GetPrivateProfileString(_T("Info"), _T("Unit_Nums"), NULL, szData, sizeof(szData), LIST_PATH))
	{
		strData = CString(szData);
		if (strData.IsEmpty())
			sUnits.Format(_T("%d"), nUnit);
		else
			sUnits.Format(_T("%s,%d"), strData, nUnit);
	}
	else
		sUnits.Format(_T("%d"), nUnit);
	::WritePrivateProfileString(_T("Info"), _T("Unit_Nums"), sUnits, LIST_PATH);

	strItem.Format(_T("%d"), nThickness);
	sUnits.Format(_T("%d"), nUnit);
	strTorq.Format(_T("%.3f,%.3f"), dTorqL, dTorqR);
	::WritePrivateProfileString(strItem, sUnits, strTorq, LIST_PATH);
}

void CDlgPunchingTorque::ModifyUnit(int nUnit, int nThickness, double dTorqL, double dTorqR)
{
	CString sUnits, strItem, strTorq;

	if (nThickness < 1)
		return;

	if (dTorqL < 0.0 || dTorqR < 0.0)
		return;

	strItem.Format(_T("%d"), nThickness);
	sUnits.Format(_T("%d"), nUnit);
	strTorq.Format(_T("%.3f,%.3f"), dTorqL, dTorqR);
	::WritePrivateProfileString(strItem, sUnits, strTorq, LIST_PATH);
}

int CDlgPunchingTorque::GetModelThickness(CString sModel)
{
	int nModelIdx, nThickness;
	CString sModelIdx, sListModel;
	TCHAR szData[MAX_PATH];
	TCHAR *token;
	TCHAR sep[] = { _T(",;\r\n\t") };

	nModelIdx = SearchModel(sModel);
	if (nModelIdx < 0)
		return -1; // Not Found

	sModelIdx.Format(_T("%d"), nModelIdx);
	if (0 < ::GetPrivateProfileString(_T("Model"), sModelIdx, NULL, szData, sizeof(szData), LIST_PATH))
	{
		token = _tcstok(szData, sep);
		sListModel = CString(token);
		token = _tcstok(NULL, sep);
		nThickness = _ttoi(token);
	}
	else
		return -1; // Not found.

	return nThickness;
}

int CDlgPunchingTorque::SearchModel(CString sModel)
{
	int nModelIdx, nThickness;
	CString sModelIdx, sListModel;
	TCHAR szData[MAX_PATH];
	TCHAR *token;
	TCHAR sep[] = { _T(",;\r\n\t") };

	for (nModelIdx = 0; nModelIdx < m_stList.nTotalModels; nModelIdx++)
	{
		sModelIdx.Format(_T("%d"), nModelIdx);
		if (0 < ::GetPrivateProfileString(_T("Model"), sModelIdx, NULL, szData, sizeof(szData), LIST_PATH))
		{
			token = _tcstok(szData, sep);
			sListModel = CString(token);
			token = _tcstok(NULL, sep);
			nThickness = _ttoi(token);
		}
		else
			sListModel = _T("");

		if (sListModel == sModel)
			return nModelIdx;
	}

	return -1; // Not Found
}

void CDlgPunchingTorque::StringToChar(CString str, char* pCh) // char* returned must be deleted... 
{
	wchar_t* wszStr;
	int	nLenth;

	USES_CONVERSION;
	//1. CString to wchar_t* conversion
	wszStr = T2W(str.GetBuffer(str.GetLength()));

	//2. wchar_t* to char* conversion
	nLenth = WideCharToMultiByte(CP_ACP, 0, wszStr, -1, NULL, 0, NULL, NULL); //char* 형에 대한길이를 구함 

																			  //3. wchar_t* to char* conversion
	WideCharToMultiByte(CP_ACP, 0, wszStr, -1, pCh, nLenth, 0, 0);
	return;
}

BOOL CDlgPunchingTorque::DirectoryExists(LPCTSTR szPath)
{
	DWORD dwAttrib = GetFileAttributes(szPath);

	return (dwAttrib != INVALID_FILE_ATTRIBUTES &&
		(dwAttrib & FILE_ATTRIBUTE_DIRECTORY));
}

// Write Log for Auto
void CDlgPunchingTorque::Log(CString strMsg, int nType)
{
	CSafeLock lock(&g_LogLock);

	TCHAR szFile[MAX_PATH] = { 0, };
	TCHAR szPath[MAX_PATH] = { 0, };
	TCHAR* pszPos = NULL;

	_stprintf(szPath, LOG_PATH);
	if (!DirectoryExists(szPath))
		CreateDirectory(szPath, NULL);

	COleDateTime time = COleDateTime::GetCurrentTime();

	switch (nType)
	{
	case 0:
		_stprintf(szFile, _T("%s\\%s.txt"), szPath, COleDateTime::GetCurrentTime().Format(_T("%Y%m%d")));
		break;
	}

	CString strDate;
	CString strContents;
	CTime now;

	strDate.Format(_T("%s - "), COleDateTime::GetCurrentTime().Format(_T("%Y/%m/%d %H:%M:%S")));
	strContents = strDate;
	strContents += strMsg;
	strContents += _T("\r");

	CFile file;
	if (file.Open(szFile, CFile::modeWrite) == 0)
	{
		if (file.Open(szFile, CFile::modeCreate | CFile::modeWrite) == 0)
			return;
	}

	char cameraKey[1024];
	StringToChar(strContents, cameraKey);

	file.SeekToEnd();
	int nLenth = strContents.GetLength();
	int nLenth2 = strlen(cameraKey);
	file.Write(cameraKey, nLenth2);
	file.Flush();
	file.Close();
}

void CDlgPunchingTorque::UpdateList()
{
	m_stList.ReloadList();
	DispList();
}

void CDlgPunchingTorque::OnChangeEditUnit()
{
	// TODO:  RICHEDIT 컨트롤인 경우, 이 컨트롤은
	// CDialog::OnInitDialog() 함수를 재지정 
	//하고 마스크에 OR 연산하여 설정된 ENM_CHANGE 플래그를 지정하여 CRichEditCtrl().SetEventMask()를 호출하지 않으면
	// 이 알림 메시지를 보내지 않습니다.

	// TODO:  여기에 컨트롤 알림 처리기 코드를 추가합니다.
	CString str;
	GetDlgItem(IDC_EDIT_UNIT)->GetWindowText(str);
	int nUnit = _tstoi(str);
	int nUnitIdx = SearchUnit(nUnit);
	if (nUnitIdx < 0) // Not Found.
		return;

	int nThick = m_nThickUnit;
	m_nUnit = nUnit;

	if (nThick < 0)
		nThick = m_nThickModel;

	if (nThick > 0)
		ModifyComboThickUnit(nUnit, nThick);
}

void CDlgPunchingTorque::ModifyComboThickUnit(int nUnit, int nThick)
{
	if (nThick < 1)
		return;

	int nIndex = nThick - 1;
	((CComboBox*)GetDlgItem(IDC_COMBO_THICK_UNIT))->SetCurSel(nIndex);

	ModifyEditTorqueUnit(nUnit, nThick);
}

void CDlgPunchingTorque::OnSelchangeComboThickUnit()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	CString sThickness = _T("");

	int nIndex = ((CComboBox*)GetDlgItem(IDC_COMBO_THICK_UNIT))->GetCurSel();
	if (nIndex != LB_ERR)
	{
		((CComboBox*)GetDlgItem(IDC_COMBO_THICK_UNIT))->GetLBText(nIndex, sThickness);
	}

	if (sThickness.IsEmpty())
		return;

	if (sThickness == THICKNESS_0)
		m_nThickUnit = 0;

	if (sThickness == THICKNESS_1)
		m_nThickUnit = 1;

	if (sThickness == THICKNESS_2)
		m_nThickUnit = 2;

	if (sThickness == THICKNESS_3)
		m_nThickUnit = 3;

	ModifyEditTorqueUnit(m_nUnit, m_nThickUnit);
}

void CDlgPunchingTorque::ModifyEditTorqueUnit(int nUnit, int nThick)
{
	double dTorqL, dTorqR;
	CString sVal;
	if (!GetTorque(nUnit, nThick, dTorqL, dTorqR))
		return;

	sVal.Format(_T("%.3f"), dTorqL);
	GetDlgItem(IDC_EDIT_TORQUE_LEFT)->SetWindowText(sVal);
	sVal.Format(_T("%.3f"), dTorqR);
	GetDlgItem(IDC_EDIT_TORQUE_RIGHT)->SetWindowText(sVal);
}

BOOL CDlgPunchingTorque::GetTorque(int nUnit, int nThick, double &dTorqL, double &dTorqR)
{
	int nTotalUnit;
	TCHAR szData[MAX_PATH];
	TCHAR sep[] = { _T(",;\r\n\t") };
	TCHAR *token;

	if (0 < ::GetPrivateProfileString(_T("Info"), _T("Total_Machines"), NULL, szData, sizeof(szData), LIST_PATH))
		m_nTotalUnit = nTotalUnit = _ttoi(szData);
	else
	{
		m_nTotalUnit = nTotalUnit = 0;
		return FALSE;
	}

	CString sThickIdx, sUnitNum;
	sThickIdx.Format(_T("%d"), nThick);

	sUnitNum.Format(_T("%d"), nUnit);
	if (0 < ::GetPrivateProfileString(sThickIdx, sUnitNum, NULL, szData, sizeof(szData), LIST_PATH))
	{
		token = _tcstok(szData, sep);
		dTorqL = _ttof(token);
		token = _tcstok(NULL, sep);
		dTorqR = _ttof(token);
	}
	else
		return FALSE;

	return TRUE;;
}

void CDlgPunchingTorque::ResetComboThickModel()
{
	((CComboBox*)GetDlgItem(IDC_COMBO_THICK_MODEL))->SetWindowText(_T(""));
	ResetEditUnit();
}

void CDlgPunchingTorque::ResetEditUnit()
{
	GetDlgItem(IDC_EDIT_UNIT)->SetWindowText(_T(""));
	((CComboBox*)GetDlgItem(IDC_COMBO_THICK_UNIT))->SetWindowText(_T(""));
	GetDlgItem(IDC_EDIT_TORQUE_LEFT)->SetWindowText(_T(""));
	GetDlgItem(IDC_EDIT_TORQUE_RIGHT)->SetWindowText(_T(""));
}

void CDlgPunchingTorque::OnBnClickedButtonRefreshModel()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	ModifyModelData();
}


void CDlgPunchingTorque::OnClickList1(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMITEMACTIVATE pNMItemActivate = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	*pResult = 0;

	// 행 클릭시 행 넘버값 받아오기
	NM_LISTVIEW* pNMListView = (NM_LISTVIEW*)pNMHDR;
	int idx = pNMListView->iItem;

	// 선택된 아이템값(idx)의 열에서 [1]번째 col의 아이템을 가져온다.
	CString sModel = m_List.GetItemText(idx, 1);

	// Model ComboBox에서 선택한 모델을 표시한다.
	int nSel = SearchModelInCombo(sModel);
	((CComboBox*)GetDlgItem(IDC_COMBO_MODEL))->SetCurSel(nSel);
	int nThickModel = GetModelThickness(sModel);
	if (nThickModel < 1)
	{
		ResetComboThickModel();
		return;
	}
	ModifyComboThickModel(nThickModel);

	m_sModel = sModel;
	m_nThickModel = nThickModel;
}

int CDlgPunchingTorque::SearchModelInCombo(CString sModel)
{
	int i, idx = -1, nTotalModel = 0;
	CString sModelInCombo;

	nTotalModel = ((CComboBox*)GetDlgItem(IDC_COMBO_MODEL))->GetCount();

	for (i = 0; i < nTotalModel; i++)
	{
		((CComboBox*)GetDlgItem(IDC_COMBO_MODEL))->GetLBText(i, sModelInCombo);

		if (sModelInCombo == sModel)
			return i;
	}

	return idx;
}

void CDlgPunchingTorque::SelectList(CString sModel)
{
	int i = SearchModel(sModel);

	// 먼저 현재 선택상태를 해제합니다
	m_List.SetItemState(-1, 0, LVIS_SELECTED | LVIS_FOCUSED);
	// 원하는 아이템을 선택합니다
	m_List.SetItemState(i, LVIS_FOCUSED | LVIS_SELECTED, LVIS_FOCUSED | LVIS_SELECTED);
	// 선택된 아이템을 표시합니다
	m_List.EnsureVisible(i, false);
	// 리스트 컨트롤에 포커스를 맞춥니다
	m_List.SetFocus();

	//특정위치 하이라이트
	m_List.SetSelectionMark(i);
	m_List.EnsureVisible(i, TRUE); //스크롤
	m_List.SetItemState(m_List.GetSelectionMark(), LVIS_SELECTED | LVIS_FOCUSED, LVIS_SELECTED | LVIS_FOCUSED);
	m_List.SetFocus();
}

void CDlgPunchingTorque::OnBnClickedButtonFindModel()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	CString sModel, sMsg;
	GetDlgItem(IDC_EDIT_MODEL)->GetWindowText(sModel);
	sModel.MakeUpper();

	int nSel = SearchModelInCombo(sModel);
	if (nSel < 0)
	{
		sMsg.Format(_T("%s 모델은 토크값이 설정되지 않았습니다."), sModel);
		AfxMessageBox(sMsg);
		return;
	}

	((CComboBox*)GetDlgItem(IDC_COMBO_MODEL))->SetCurSel(nSel);
	m_sModel = sModel;

	int nThickModel = GetModelThickness(sModel);
	if (nThickModel < 1)
	{
		ResetComboThickModel();
		return;
	}
	ModifyComboThickModel(nThickModel);

	m_sModel = sModel;
	m_nThickModel = nThickModel;

	SelectList(sModel);
}


void CDlgPunchingTorque::OnBnClickedButtonDeleteModel()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	CString sModel = m_sModel;
	int nThickModel = m_nThickModel;
	CString sMsg;
	sMsg.Format(_T("모델 : %s 에 대한 토크설정 정보를 삭제하시겠습니까?"), sModel);
	if (IDNO == MessageBox(sMsg, _T("주의"), MB_YESNO | MB_ICONQUESTION))
	{
		return;
	}

	BOOL bRtn = DeleteModel(sModel);

	if (bRtn)
		UpdateList();
}

BOOL CDlgPunchingTorque::DeleteModel(CString sModel)
{
	int nDelIdx = SearchModel(sModel);
	if (nDelIdx < 0)
		return FALSE;

	int nModelTot = m_stList.nTotalModels;

	TCHAR szData[MAX_PATH];
	TCHAR *token;
	TCHAR sep[] = { _T(",;\r\n\t") };

	CString sModelIdx, sData;
	int nThickness;
	CString sListModel;

	for (int i = nDelIdx + 1; i < nModelTot; i++)
	{
		sModelIdx.Format(_T("%d"), i);
		if (0 < ::GetPrivateProfileString(_T("Model"), sModelIdx, NULL, szData, sizeof(szData), LIST_PATH))
		{
			token = _tcstok(szData, sep);
			sListModel = CString(token);
			token = _tcstok(NULL, sep);
			nThickness = _ttoi(token);

			sModelIdx.Format(_T("%d"), i - 1);
			sData.Format(_T("%s,%d"), sListModel, nThickness);
			::WritePrivateProfileString(_T("Model"), sModelIdx, sData, LIST_PATH);
		}
	}

	m_stList.nTotalModels--;

	sModelIdx.Format(_T("%d"), m_stList.nTotalModels);
	sData.Format(_T(""));
	::WritePrivateProfileString(_T("Model"), sModelIdx, sData, LIST_PATH);

	sData.Format(_T("%d"), m_stList.nTotalModels);
	::WritePrivateProfileString(_T("Info"), _T("Total_Models"), sData, LIST_PATH);

	return TRUE;
}
