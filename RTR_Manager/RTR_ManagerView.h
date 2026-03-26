
// RTR_ManagerView.h : CRTR_ManagerView 클래스의 인터페이스
//

#pragma once

#include "RTR_ManagerDoc.h"
#include "DlgItsOrigin.h"
#include "DlgPunchingTorque.h"

class CRTR_ManagerView : public CFormView
{
private:
	CDlgItsOrigin* m_pDlgItsOrigin;
	CDlgPunchingTorque* m_pDlgPunchingTorque;
	int m_nCurTab;
	CFont m_FontOfTabCtrl;
	BOOL m_bEnableWnd;

	void ShowDlg(int nId);
	void HideAllDlg();
	//void EnableWnd(BOOL bEnable);
	void InitTab();
	void Init();
	void InitDlg();

protected: // serialization에서만 만들어집니다.
	CRTR_ManagerView();
	DECLARE_DYNCREATE(CRTR_ManagerView)

public:
#ifdef AFX_DESIGN_TIME
	enum{ IDD = IDD_RTR_MANAGER_FORM };
#endif

// 특성입니다.
public:
	CRTR_ManagerDoc* GetDocument() const;

// 작업입니다.
public:
	CTabCtrl m_TabCtrl;

public:
	void RefreshDlg();

// 재정의입니다.
public:
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.
	virtual void OnInitialUpdate(); // 생성 후 처음 호출되었습니다.

// 구현입니다.
public:
	virtual ~CRTR_ManagerView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// 생성된 메시지 맵 함수
protected:
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnShowWindow(BOOL bShow, UINT nStatus);
	afx_msg void OnSelchangeTab1(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
};

#ifndef _DEBUG  // RTR_ManagerView.cpp의 디버그 버전
inline CRTR_ManagerDoc* CRTR_ManagerView::GetDocument() const
   { return reinterpret_cast<CRTR_ManagerDoc*>(m_pDocument); }
#endif

