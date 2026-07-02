
// RTR_ManagerView.cpp : CRTR_ManagerView 클래스의 구현
//

#include "stdafx.h"
// SHARED_HANDLERS는 미리 보기, 축소판 그림 및 검색 필터 처리기를 구현하는 ATL 프로젝트에서 정의할 수 있으며
// 해당 프로젝트와 문서 코드를 공유하도록 해 줍니다.
#ifndef SHARED_HANDLERS
#include "RTR_Manager.h"
#endif

#include "MainFrm.h"
#include "RTR_ManagerDoc.h"
#include "RTR_ManagerView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CRTR_ManagerView

IMPLEMENT_DYNCREATE(CRTR_ManagerView, CFormView)

BEGIN_MESSAGE_MAP(CRTR_ManagerView, CFormView)
	ON_WM_SHOWWINDOW()
	ON_NOTIFY(TCN_SELCHANGE, IDC_TAB1, &CRTR_ManagerView::OnSelchangeTab1)
	ON_WM_MOUSEMOVE()
END_MESSAGE_MAP()

// CRTR_ManagerView 생성/소멸

CRTR_ManagerView::CRTR_ManagerView()
	: CFormView(IDD_RTR_MANAGER_FORM)
{
	// TODO: 여기에 생성 코드를 추가합니다.
	m_bEnableWnd = FALSE;
	m_pDlgItsOrigin = NULL;
	m_pDlgPunchingTorque = NULL;
	m_pDlgLaserSetting = NULL;
}

CRTR_ManagerView::~CRTR_ManagerView()
{
	if (m_pDlgItsOrigin != NULL)
	{
		delete m_pDlgItsOrigin;
		m_pDlgItsOrigin = NULL;
	}
	if (m_pDlgPunchingTorque != NULL)
	{
		delete m_pDlgPunchingTorque;
		m_pDlgPunchingTorque = NULL;
	}
	if (m_pDlgLaserSetting != NULL)
	{
		delete m_pDlgLaserSetting;
		m_pDlgLaserSetting = NULL;
	}
}

void CRTR_ManagerView::DoDataExchange(CDataExchange* pDX)
{
	CFormView::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_TAB1, m_TabCtrl);
}

BOOL CRTR_ManagerView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: CREATESTRUCT cs를 수정하여 여기에서
	//  Window 클래스 또는 스타일을 수정합니다.

	return CFormView::PreCreateWindow(cs);
}

void CRTR_ManagerView::OnInitialUpdate()
{
	CFormView::OnInitialUpdate();
	GetParentFrame()->RecalcLayout();
	ResizeParentToFit();

	// TODO: 여기에 추가 초기화 작업을 추가합니다.
	Init();
}


// CRTR_ManagerView 진단

#ifdef _DEBUG
void CRTR_ManagerView::AssertValid() const
{
	CFormView::AssertValid();
}

void CRTR_ManagerView::Dump(CDumpContext& dc) const
{
	CFormView::Dump(dc);
}

CRTR_ManagerDoc* CRTR_ManagerView::GetDocument() const // 디버그되지 않은 버전은 인라인으로 지정됩니다.
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CRTR_ManagerDoc)));
	return (CRTR_ManagerDoc*)m_pDocument;
}
#endif //_DEBUG


// CRTR_ManagerView 메시지 처리기
void CRTR_ManagerView::ShowDlg(int nId)
{
	HideAllDlg();
	switch (nId)
	{
	case 0:
		if (!m_pDlgItsOrigin)
		{
			m_pDlgItsOrigin = new CDlgItsOrigin(this);
			if (m_pDlgItsOrigin->GetSafeHwnd() == 0)
			{
				m_pDlgItsOrigin->ShowWindow(SW_SHOW);
			}
		}
		else
		{
			RefreshDlg();
			m_pDlgItsOrigin->ShowWindow(SW_SHOW);
		}
		break;
	case 1:
		if (!m_pDlgPunchingTorque)
		{
			m_pDlgPunchingTorque = new CDlgPunchingTorque(this);
			if (m_pDlgPunchingTorque->GetSafeHwnd() == 0)
			{
				m_pDlgPunchingTorque->Create();
				m_pDlgPunchingTorque->ShowWindow(SW_SHOW);
			}
		}
		else
		{
			RefreshDlg();
			m_pDlgPunchingTorque->ShowWindow(SW_SHOW);
		}
		break;
	case 2:
		if (!m_pDlgLaserSetting)
		{
			m_pDlgLaserSetting = new CDlgLaserSetting(this);
			if (m_pDlgLaserSetting->GetSafeHwnd() == 0)
			{
				m_pDlgLaserSetting->Create();
				m_pDlgLaserSetting->ShowWindow(SW_SHOW);
			}
		}
		else
		{
			RefreshDlg();
			m_pDlgLaserSetting->ShowWindow(SW_SHOW);
		}
		break;
	}
	RefreshDlg();
}

void CRTR_ManagerView::HideAllDlg()
{
	if (m_pDlgItsOrigin && m_pDlgItsOrigin->GetSafeHwnd())
	{
		if (m_pDlgItsOrigin->IsWindowVisible())
			m_pDlgItsOrigin->ShowWindow(SW_HIDE);
	}
	if (m_pDlgPunchingTorque && m_pDlgPunchingTorque->GetSafeHwnd())
	{
		if (m_pDlgPunchingTorque->IsWindowVisible())
			m_pDlgPunchingTorque->ShowWindow(SW_HIDE);
	}
	if (m_pDlgLaserSetting && m_pDlgLaserSetting->GetSafeHwnd())
	{
		if (m_pDlgLaserSetting->IsWindowVisible())
			m_pDlgLaserSetting->ShowWindow(SW_HIDE);
	}
}

void CRTR_ManagerView::RefreshDlg()
{
	CMainFrame* pFrame = (CMainFrame*)AfxGetMainWnd();

	CRect rtForm, rtTab, rtDlg;
	CRect rtDlg0, rtDlg1;
	pFrame->GetWindowRect(&rtForm);
	CRect rtFormFromView(rtForm);
	ScreenToClient(&rtFormFromView);	// View기준에서 Form의 위치
	int nHeightMenuBar = 0; // pFrame->GetMenuHeight();
	int nHeithtSystemBar = 0; // GetSystemMetrics(SM_CYCAPTION);
	int nXOffetViewForm = -rtFormFromView.left;
	int nYOffetViewForm = -rtFormFromView.top;
	int cxEdge = GetSystemMetrics(SM_CXEDGE); // 엣지의 가로 두께 (픽셀)
	int cyEdge = GetSystemMetrics(SM_CYEDGE); // 엣지의 세로 두께 (픽셀)

	GetDlgItem(IDC_STATIC_WND_POS)->GetWindowRect(&rtTab);
	ScreenToClient(&rtTab);

	if (m_pDlgItsOrigin && m_pDlgItsOrigin->GetSafeHwnd())
	{
		m_pDlgItsOrigin->GetClientRect(rtDlg0);
		rtDlg.left = rtForm.left + (nXOffetViewForm+cxEdge) + rtTab.left;
		rtDlg.top = rtForm.top + (nHeithtSystemBar+nHeightMenuBar+nYOffetViewForm+cyEdge) + rtTab.top;
		m_pDlgItsOrigin->SetWindowPos(NULL, rtDlg.left, rtDlg.top, rtDlg0.Width(), rtDlg0.Height(), SWP_NOZORDER | SWP_NOACTIVATE);
	}
	if (m_pDlgPunchingTorque && m_pDlgPunchingTorque->GetSafeHwnd())
	{
		m_pDlgPunchingTorque->GetClientRect(rtDlg1);
		rtDlg.left = rtForm.left + (nXOffetViewForm+cxEdge) + rtTab.left;
		rtDlg.top = rtForm.top + (nHeithtSystemBar+nHeightMenuBar+nYOffetViewForm+cyEdge) + rtTab.top;
		m_pDlgPunchingTorque->SetWindowPos(NULL, rtDlg.left, rtDlg.top, rtDlg1.Width(), rtDlg1.Height(), SWP_NOZORDER | SWP_NOACTIVATE);
	}
	if (m_pDlgLaserSetting && m_pDlgLaserSetting->GetSafeHwnd())
	{
		m_pDlgLaserSetting->GetClientRect(rtDlg1);
		rtDlg.left = rtForm.left + (nXOffetViewForm+cxEdge) + rtTab.left;
		rtDlg.top = rtForm.top + (nHeithtSystemBar+nHeightMenuBar+nYOffetViewForm+cyEdge) + rtTab.top;
		m_pDlgLaserSetting->SetWindowPos(NULL, rtDlg.left, rtDlg.top, rtDlg1.Width(), rtDlg1.Height(), SWP_NOZORDER | SWP_NOACTIVATE);
	}
}

//void CRTR_ManagerView::EnableWnd(BOOL bEnable)
//{
//	m_bEnableWnd = bEnable;
//}

//void CRTR_ManagerView::Init()
//{
	//LoadIni();
	//InitTestMode();
	//InitComboMachine();
	//InitComboModel();
	//InitListLot();
	//InitListLayer();
	//InitProcessCode();
	//InitItsCode();
//}

void CRTR_ManagerView::Init()
{
	InitTab();
	InitDlg();

	m_nCurTab = 0;
	m_TabCtrl.HighlightItem(m_nCurTab, TRUE);
	ShowDlg(m_nCurTab);
}

void CRTR_ManagerView::InitDlg()
{
	if (!m_pDlgItsOrigin)
	{
		m_pDlgItsOrigin = new CDlgItsOrigin(this);
		if (m_pDlgItsOrigin->GetSafeHwnd() == 0)
		{
			m_pDlgItsOrigin->Create();
			m_pDlgItsOrigin->ShowWindow(SW_HIDE);
		}
	}
	if (!m_pDlgPunchingTorque)
	{
		m_pDlgPunchingTorque = new CDlgPunchingTorque(this);
		if (m_pDlgPunchingTorque->GetSafeHwnd() == 0)
		{
			m_pDlgPunchingTorque->Create();
			m_pDlgPunchingTorque->ShowWindow(SW_HIDE);
		}
	}
	if (!m_pDlgLaserSetting)
	{
		m_pDlgLaserSetting = new CDlgLaserSetting(this);
		if (m_pDlgLaserSetting->GetSafeHwnd() == 0)
		{
			m_pDlgLaserSetting->Create();
			m_pDlgLaserSetting->ShowWindow(SW_HIDE);
		}
	}
}

void CRTR_ManagerView::OnShowWindow(BOOL bShow, UINT nStatus)
{
	CFormView::OnShowWindow(bShow, nStatus);

	// TODO: 여기에 메시지 처리기 코드를 추가합니다.
	if (bShow)
	{
	}
}

void CRTR_ManagerView::InitTab()	// Tab Control 초기화
{
	CString sTabCaption[3];
	sTabCaption[0] = _T("ITS Origin");
	sTabCaption[1] = _T("Torque");
	sTabCaption[2] = _T("Laser");

	// Tab Control의 폰트 변경.
	LOGFONT lf;
	memset(&lf, 0, sizeof(LOGFONT));
	lf.lfHeight = 15; // point 크기
	lf.lfWeight = FW_EXTRABOLD;
	_tcscpy(lf.lfFaceName, _T("굴림체"));

	m_FontOfTabCtrl.CreateFontIndirect(&lf);
	CTabCtrl* pCtlTab = (CTabCtrl*)GetDlgItem(IDC_TAB1); // 탭 콘트롤	
	pCtlTab->SetFont((CFont*)&m_FontOfTabCtrl, TRUE);

	TC_ITEM item;
	item.mask = TCIF_TEXT;

	item.pszText = (LPWSTR)(LPCWSTR)sTabCaption[0];
	m_TabCtrl.InsertItem(0, &item);

	item.pszText = (LPWSTR)(LPCWSTR)sTabCaption[1];
	m_TabCtrl.InsertItem(1, &item);

	item.pszText = (LPWSTR)(LPCWSTR)sTabCaption[2];
	m_TabCtrl.InsertItem(2, &item);
}


void CRTR_ManagerView::OnSelchangeTab1(NMHDR *pNMHDR, LRESULT *pResult)
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	*pResult = 0;

	// 현재 설정된 SUB UNIT의 인덱스값(0기준)을 얻는다.
	m_TabCtrl.HighlightItem(m_nCurTab, FALSE);

	m_nCurTab = m_TabCtrl.GetCurSel();
	m_TabCtrl.HighlightItem(m_nCurTab, TRUE);
	ShowDlg(m_nCurTab);
}


void CRTR_ManagerView::OnMouseMove(UINT nFlags, CPoint point)
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.
	CFormView::OnMouseMove(nFlags, point);
	RefreshDlg();
}
