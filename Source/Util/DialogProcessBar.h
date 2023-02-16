#pragma once
#include "afxcmn.h"


// CDialogProcessBar 대화 상자입니다.

class CDialogProcessBar : public CDialogEx
{
	DECLARE_DYNAMIC(CDialogProcessBar)

public:
	CDialogProcessBar(CWnd* pParent = NULL);   // 표준 생성자입니다.
	virtual ~CDialogProcessBar();

// 대화 상자 데이터입니다.
	enum { IDD = IDD_DIALOG_PROCESS_BAR };

	void SetProcessPos(int pos);
	int m_nProcessPos;
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()
public:
	CProgressCtrl m_Process;
	afx_msg void OnShowWindow(BOOL bShow, UINT nStatus);
	virtual BOOL OnInitDialog();
	afx_msg void OnTimer(UINT_PTR nIDEvent);
};
