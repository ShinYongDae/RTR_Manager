
// MainFrm.cpp : CMainFrame 클래스의 구현
//

#include "stdafx.h"
#include "RTR_Manager.h"

#include "MainFrm.h"
#include "RTR_ManagerView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

// CMainFrame

IMPLEMENT_DYNCREATE(CMainFrame, CFrameWnd)

BEGIN_MESSAGE_MAP(CMainFrame, CFrameWnd)
	ON_WM_CREATE()
	ON_WM_MOVING()
END_MESSAGE_MAP()

static UINT indicators[] =
{
	ID_SEPARATOR,           // 상태 줄 표시기
	ID_INDICATOR_CAPS,
	ID_INDICATOR_NUM,
	ID_INDICATOR_SCRL,
};

// CMainFrame 생성/소멸

CMainFrame::CMainFrame()
{
	// TODO: 여기에 멤버 초기화 코드를 추가합니다.
}

CMainFrame::~CMainFrame()
{
}

int CMainFrame::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CFrameWnd::OnCreate(lpCreateStruct) == -1)
		return -1;

	if (!m_wndStatusBar.Create(this))
	{
		TRACE0("상태 표시줄을 만들지 못했습니다.\n");
		return -1;      // 만들지 못했습니다.
	}
	m_wndStatusBar.SetIndicators(indicators, sizeof(indicators)/sizeof(UINT));

	// CMainFrame 내부에서 호출
	CMenu* pMenu = GetMenu();
	if (pMenu != NULL && pMenu->GetSafeHmenu() != NULL)
	{
		// 메뉴 바의 정보를 가져옴 (WIN32 API)
		MENUBARINFO mbi = { sizeof(MENUBARINFO) };
		if (::GetMenuBarInfo(m_hWnd, OBJID_MENU, 0, &mbi))
		{
			// rcBar가 메뉴바의 전체 크기 영역입니다.
			m_nMenuHeight = mbi.rcBar.bottom - mbi.rcBar.top;
			m_nMenuWidth = mbi.rcBar.right - mbi.rcBar.left;
		}
	}
	return 0;
}

BOOL CMainFrame::PreCreateWindow(CREATESTRUCT& cs)
{
	if( !CFrameWnd::PreCreateWindow(cs) )
		return FALSE;
	// TODO: CREATESTRUCT cs를 수정하여 여기에서
	//  Window 클래스 또는 스타일을 수정합니다.
	cs.style &= ~FWS_ADDTOTITLE;
	cs.lpszName = _T("해성디에스용 GigaVis RTR 통합 Manager (version: 1.1.1) ;  Update Date(2026,07,02)"); // 타이틀 설정
	cs.cx = 1350;	// 초기 가로 크기
	cs.cy = 950;	// 초기 세로 크기

	return TRUE;
}

// CMainFrame 진단

#ifdef _DEBUG
void CMainFrame::AssertValid() const
{
	CFrameWnd::AssertValid();
}

void CMainFrame::Dump(CDumpContext& dc) const
{
	CFrameWnd::Dump(dc);
}
#endif //_DEBUG


// CMainFrame 메시지 처리기

int CMainFrame::GetMenuHeight()
{
	return m_nMenuHeight;
}

int CMainFrame::GetMenuWidth()
{
	return m_nMenuWidth;
}


void CMainFrame::OnMoving(UINT fwSide, LPRECT pRect)
{
	CFrameWnd::OnMoving(fwSide, pRect);

	// TODO: 여기에 메시지 처리기 코드를 추가합니다.
	CRTR_ManagerView* pView = (CRTR_ManagerView*)GetActiveView();
	if (pView != nullptr)
	{
		pView->RefreshDlg(); // View 함수 호출
	}
}
