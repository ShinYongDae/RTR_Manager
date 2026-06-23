// DlgItsOrigin.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "RTR_Manager.h"
#include "DlgItsOrigin.h"
#include "afxdialogex.h"


// CDlgItsOrigin 대화 상자입니다.

IMPLEMENT_DYNAMIC(CDlgItsOrigin, CDialog)

CDlgItsOrigin::CDlgItsOrigin(CWnd* pParent /*=NULL*/)
	: CDialog(IDD_DLG_ITS_ORIGIN, pParent)
{
	m_sPathCamSpecDir = _T("");
	m_sModel = _T("");
	m_nCase = 0;
}

CDlgItsOrigin::~CDlgItsOrigin()
{
}

void CDlgItsOrigin::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST1, m_List);
}


BEGIN_MESSAGE_MAP(CDlgItsOrigin, CDialog)
	ON_NOTIFY(NM_CLICK, IDC_LIST1, &CDlgItsOrigin::OnClickList1)
	ON_BN_CLICKED(IDC_BUTTON_SAVE_ITS_CASE, &CDlgItsOrigin::OnBnClickedButtonSaveItsCase)
	ON_BN_CLICKED(IDC_BUTTON_REFRESH_MODEL, &CDlgItsOrigin::OnBnClickedButtonRefreshModel)
	ON_CBN_SELCHANGE(IDC_COMBO_ITS_CASE, &CDlgItsOrigin::OnSelchangeComboItsCase)
	ON_BN_CLICKED(IDC_BUTTON_FIND_MODEL, &CDlgItsOrigin::OnBnClickedButtonFindModel)
	ON_BN_CLICKED(IDC_BUTTON_DELETE_MODEL, &CDlgItsOrigin::OnBnClickedButtonDeleteModel)
	ON_CBN_SELCHANGE(IDC_COMBO_MODEL, &CDlgItsOrigin::OnSelchangeComboModel)
END_MESSAGE_MAP()


// CDlgItsOrigin 메시지 처리기입니다.

BOOL CDlgItsOrigin::Create()
{
	return CDialog::Create(CDlgItsOrigin::IDD);
}

BOOL CDlgItsOrigin::OnInitDialog()
{
	CDialog::OnInitDialog();

	// TODO: 여기에 추가 초기화 작업을 추가합니다.
	//this->SetWindowText(_T("ITS Origin Case Manager ;  Update Date(2025,10,29)"));

	LoadIni();
	InitList();
	DispList();

	ModifyModelData();
	ModifyItsOriginCaseData();

	return TRUE;  // 포커스를 컨트롤에 설정하지 않으면 TRUE를 반환합니다.
}

void CDlgItsOrigin::LoadIni()
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

void CDlgItsOrigin::InitList()
{
	// Addition ListControl
	int nIdxUnit;
	CString strItem = _T("");

	m_List.DeleteAllItems();
	Sleep(30);
	m_List.SetExtendedStyle(LVS_EX_GRIDLINES | LVS_EX_FULLROWSELECT); // 격자 추가
	m_List.InsertColumn(0, _T("Index"), LVCFMT_CENTER, 50);//LVCFMT_LEFT
	m_List.InsertColumn(1, _T("모델"), LVCFMT_CENTER, 180);//LVCFMT_LEFT
	m_List.InsertColumn(2, _T("ITS원점 CASE"), LVCFMT_CENTER, 260);
}

void CDlgItsOrigin::UpdateList()
{
	m_stListItsOrigin.ReloadList();
	DispList();
}

void CDlgItsOrigin::DispList()
{
	int nIdxModel, nIdxUnit;
	TCHAR tChar[200];
	CString sVal;

	m_List.DeleteAllItems();
	Sleep(30);

	for (nIdxModel = 0; nIdxModel < m_stListItsOrigin.nTotalModels; nIdxModel++)
	{
		LVITEM item;
		::ZeroMemory(&item, sizeof(item));
		item.mask = LVIF_TEXT | LVIF_IMAGE | LVIF_STATE;
		// Index
		item.iItem = m_List.GetItemCount();
		item.iSubItem = 0;
		item.iImage = 1;
		sVal.Format(_T("%10d"), nIdxModel);
		_stprintf(tChar, _T("%s"), sVal);
		item.pszText = tChar;
		m_List.InsertItem(&item);
		// 모델
		item.iSubItem = 1;
		_stprintf(tChar, _T("%s"), m_stListItsOrigin.pModel[nIdxModel].sModel);
		item.pszText = tChar;
		m_List.SetItem(&item);
		// ITS원점 case
		item.iSubItem = 2;
		_stprintf(tChar, _T("%s"), GetItsCaseLabel(m_stListItsOrigin.pModel[nIdxModel].nItsCase));
		item.pszText = tChar;
		m_List.SetItem(&item);
	}

	m_List.ModifyStyle(LVS_TYPEMASK, LVS_REPORT);
}

CString CDlgItsOrigin::GetItsCaseLabel(int nIdx) // 좌우[0], 좌하[1], 우상[2], 우하[3]
{
	CString sName = _T("");
	switch (nIdx)
	{
	case 0:
		sName = CASE_0;
		break;
	case 1:
		sName = CASE_1;
		break;
	case 2:
		sName = CASE_2;
		break;
	case 3:
		sName = CASE_3;
		break;
	case 4:
		sName = CASE_4;
		break;
	case 5:
		sName = CASE_5;
		break;
	case 6:
		sName = CASE_6;
		break;
	case 7:
		sName = CASE_7;
		break;
	default:
		sName = CASE_0;
		break;
	}
	return sName;
}

void CDlgItsOrigin::OnClickList1(NMHDR *pNMHDR, LRESULT *pResult)
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
	int nCase = GetItsOriginCase(sModel);
	if (nCase < 0)
	{
		return;
	}
	m_sModel = sModel;
	m_nCase = nCase;
}

int CDlgItsOrigin::SearchModelInCombo(CString sModel)
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

int CDlgItsOrigin::GetItsOriginCase(CString sModel)
{
	int nModelIdx, nCase;
	CString sModelIdx, sListModel;
	TCHAR szData[MAX_PATH];
	TCHAR *token;
	TCHAR sep[] = { _T(",;\r\n\t") };

	nModelIdx = SearchModel(sModel, nCase);
	if (nModelIdx < 0)
		return -1; // Not Found

				   //sModelIdx.Format(_T("%d"), nModelIdx);
				   //if (0 < ::GetPrivateProfileString(_T("Model"), sModelIdx, NULL, szData, sizeof(szData), LIST_PATH))
				   //{
				   //	token = _tcstok(szData, sep);
				   //	sListModel = CString(token);
				   //	token = _tcstok(NULL, sep);
				   //	nThickness = _ttoi(token);
				   //}
				   //else
				   //	return -1; // Not found.

	return nCase;
}

int CDlgItsOrigin::SearchModel(CString sModel)
{
	int nModelIdx, nCase;
	CString sModelIdx, sListModel;
	TCHAR szData[MAX_PATH];
	TCHAR *token;
	TCHAR sep[] = { _T(",;\r\n\t") };

	for (nModelIdx = 0; nModelIdx < m_stListItsOrigin.nTotalModels; nModelIdx++)
	{
		sModelIdx.Format(_T("%d"), nModelIdx);
		if (0 < ::GetPrivateProfileString(_T("Model"), sModelIdx, NULL, szData, sizeof(szData), LIST_PATH))
		{
			token = _tcstok(szData, sep);
			sListModel = CString(token);
			token = _tcstok(NULL, sep);
			nCase = _ttoi(token);
		}
		else
			sListModel = _T("");

		if (sListModel == sModel)
			return nModelIdx;
	}

	return -1; // Not Found
}

int CDlgItsOrigin::SearchModel(CString sModel, int &nCase)
{
	int nModelIdx;
	CString sModelIdx, sListModel;
	TCHAR szData[MAX_PATH];
	TCHAR *token;
	TCHAR sep[] = { _T(",;\r\n\t") };

	for (nModelIdx = 0; nModelIdx < m_stListItsOrigin.nTotalModels; nModelIdx++)
	{
		sModelIdx.Format(_T("%d"), nModelIdx);
		if (0 < ::GetPrivateProfileString(_T("Model"), sModelIdx, NULL, szData, sizeof(szData), LIST_PATH))
		{
			token = _tcstok(szData, sep);
			sListModel = CString(token);
			token = _tcstok(NULL, sep);
			nCase = _ttoi(token);
		}
		else
			sListModel = _T("");

		if (sListModel == sModel)
			return nModelIdx;
	}

	return -1; // Not Found
}

void CDlgItsOrigin::OnBnClickedButtonSaveItsCase()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	if (m_sModel.IsEmpty())
	{
		AfxMessageBox(_T("Model이 선택되지 않았습니다."));
		return;
	}

	if (m_nCase < 0)
	{
		AfxMessageBox(_T("ITS원점 CASE가 선택되지 않았습니다."));
		return;
	}

	CString sMsg;
	sMsg.Format(_T("%s 모델의 ITS원점 CASE를 case %s로 변경하시겠습니까?"), m_sModel, GetCaseString(m_nCase));
	if (IDNO == MessageBox(sMsg, _T("주의"), MB_YESNO | MB_ICONQUESTION))
	{
		return;
	}

	UpdateModel(m_sModel, m_nCase);
	UpdateList();
	sMsg.Format(_T("%s 모델의 ITS원점 CASE를 case %d로 변경"), m_sModel, m_nCase);
	Log(sMsg);
}

BOOL CDlgItsOrigin::UpdateModel(CString sModel, int nCase)
{
	int nIdx = SearchModel(sModel);

	if (nIdx < 0) // Not Found
	{
		InsertModel(sModel, nCase);
	}
	else
	{
		ModifyModel(sModel, nCase);
	}

	return TRUE;
}

void CDlgItsOrigin::InsertModel(CString sModel, int nCase)
{
	CString sModelIdx, strData;
	int nModelIdx = m_stListItsOrigin.nTotalModels;
	sModelIdx.Format(_T("%d"), nModelIdx);
	strData.Format(_T("%s,%d"), sModel, nCase);
	::WritePrivateProfileString(_T("Model"), sModelIdx, strData, LIST_PATH);
	m_stListItsOrigin.nTotalModels++;

	strData.Format(_T("%d"), m_stListItsOrigin.nTotalModels);
	::WritePrivateProfileString(_T("Info"), _T("Total_Models"), strData, LIST_PATH);
}

void CDlgItsOrigin::ModifyModel(CString sModel, int nCase)
{
	CString sModelIdx, strData;
	int nModelIdx = SearchModel(sModel);
	sModelIdx.Format(_T("%d"), nModelIdx);
	strData.Format(_T("%s,%d"), sModel, nCase);
	::WritePrivateProfileString(_T("Model"), sModelIdx, strData, LIST_PATH);
}

void CDlgItsOrigin::Log(CString strMsg, int nType)
{
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

void CDlgItsOrigin::StringToChar(CString str, char* pCh) // char* returned must be deleted... 
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

BOOL CDlgItsOrigin::DirectoryExists(LPCTSTR szPath)
{
	DWORD dwAttrib = GetFileAttributes(szPath);

	return (dwAttrib != INVALID_FILE_ATTRIBUTES &&
		(dwAttrib & FILE_ATTRIBUTE_DIRECTORY));
}

void CDlgItsOrigin::OnBnClickedButtonRefreshModel()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	ModifyModelData();
}

void CDlgItsOrigin::ModifyModelData()
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

void CDlgItsOrigin::ModifyItsOriginCaseData()
{
	// Model Thickness
	((CComboBox*)GetDlgItem(IDC_COMBO_ITS_CASE))->ResetContent();
	((CComboBox*)GetDlgItem(IDC_COMBO_ITS_CASE))->InsertString(0, GetItsCaseLabel(0));
	((CComboBox*)GetDlgItem(IDC_COMBO_ITS_CASE))->InsertString(1, GetItsCaseLabel(1));
	((CComboBox*)GetDlgItem(IDC_COMBO_ITS_CASE))->InsertString(2, GetItsCaseLabel(2));
	((CComboBox*)GetDlgItem(IDC_COMBO_ITS_CASE))->InsertString(3, GetItsCaseLabel(3));
	((CComboBox*)GetDlgItem(IDC_COMBO_ITS_CASE))->InsertString(4, GetItsCaseLabel(4));
	((CComboBox*)GetDlgItem(IDC_COMBO_ITS_CASE))->InsertString(5, GetItsCaseLabel(5));
	((CComboBox*)GetDlgItem(IDC_COMBO_ITS_CASE))->InsertString(6, GetItsCaseLabel(6));
	((CComboBox*)GetDlgItem(IDC_COMBO_ITS_CASE))->InsertString(7, GetItsCaseLabel(7));
}

void CDlgItsOrigin::ModifyItsOriginCaseData(int nCase)
{
	if (nCase < 0)
		return;

	int nIndex = nCase;
	((CComboBox*)GetDlgItem(IDC_COMBO_ITS_CASE))->SetCurSel(nIndex);
}

CString CDlgItsOrigin::GetCaseString(int nCase)
{
	CString sCase = _T("");
	switch (nCase)
	{
	case 0:
		sCase = CASE_0;
		break;
	case 1:
		sCase = CASE_1;
		break;
	case 2:
		sCase = CASE_2;
		break;
	case 3:
		sCase = CASE_3;
		break;
	case 4:
		sCase = CASE_4;
		break;
	case 5:
		sCase = CASE_5;
		break;
	case 6:
		sCase = CASE_6;
		break;
	case 7:
		sCase = CASE_7;
		break;
	default:
		sCase = CASE_0;
		break;
	}

	return sCase;
}

void CDlgItsOrigin::OnSelchangeComboItsCase()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	CString sCase = _T("");

	int nIndex = ((CComboBox*)GetDlgItem(IDC_COMBO_ITS_CASE))->GetCurSel();
	if (nIndex != LB_ERR)
	{
		((CComboBox*)GetDlgItem(IDC_COMBO_ITS_CASE))->GetLBText(nIndex, sCase);
	}

	if (sCase.IsEmpty())
		return;

	if (sCase == CASE_0)
		m_nCase = 0;

	if (sCase == CASE_1)
		m_nCase = 1;

	if (sCase == CASE_2)
		m_nCase = 2;

	if (sCase == CASE_3)
		m_nCase = 3;

	if (sCase == CASE_4)
		m_nCase = 4;

	if (sCase == CASE_5)
		m_nCase = 5;

	if (sCase == CASE_6)
		m_nCase = 6;

	if (sCase == CASE_7)
		m_nCase = 7;
}

void CDlgItsOrigin::OnBnClickedButtonFindModel()
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

	int nCase = GetItsOriginCase(sModel);
	if (nCase < 0)
	{
		ResetComboItsCase();
		return;
	}
	ModifyItsOriginCaseData(nCase);

	m_sModel = sModel;
	m_nCase = nCase;

	SelectList(sModel);
}

void CDlgItsOrigin::ResetComboItsCase()
{
	((CComboBox*)GetDlgItem(IDC_COMBO_ITS_CASE))->SetWindowText(_T(""));
}

void CDlgItsOrigin::SelectList(CString sModel)
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

void CDlgItsOrigin::OnBnClickedButtonDeleteModel()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	CString sModel = m_sModel;
	int nCase = m_nCase;
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

BOOL CDlgItsOrigin::DeleteModel(CString sModel)
{
	int nDelIdx = SearchModel(sModel);
	if (nDelIdx < 0)
		return FALSE;

	int nModelTot = m_stListItsOrigin.nTotalModels;

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

	m_stListItsOrigin.nTotalModels--;

	sModelIdx.Format(_T("%d"), m_stListItsOrigin.nTotalModels);
	sData.Format(_T(""));
	::WritePrivateProfileString(_T("Model"), sModelIdx, sData, LIST_PATH);

	sData.Format(_T("%d"), m_stListItsOrigin.nTotalModels);
	::WritePrivateProfileString(_T("Info"), _T("Total_Models"), sData, LIST_PATH);

	return TRUE;
}

void CDlgItsOrigin::OnSelchangeComboModel()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	CString sModel, sMsg;
	int nCase;
	int nIndex = ((CComboBox*)GetDlgItem(IDC_COMBO_MODEL))->GetCurSel();
	if (nIndex != LB_ERR)
	{
		((CComboBox*)GetDlgItem(IDC_COMBO_MODEL))->GetLBText(nIndex, sModel);
		m_sModel = sModel;

		nCase = GetItsOriginCase(sModel);
		if (nCase < 0)
		{
			sMsg.Format(_T("%s 모델은 ITS원점 CASE가 설정되지 않았습니다."), sModel);
			AfxMessageBox(sMsg);
			ResetComboItsCase();
			return;
		}
		ModifyItsOriginCaseData(nCase);
		m_sModel = sModel;
		m_nCase = nCase;
		SelectList(sModel);
	}
}
