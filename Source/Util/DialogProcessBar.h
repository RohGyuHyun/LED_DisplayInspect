#pragma once
#include "afxcmn.h"


// CDialogProcessBar ��ȭ �����Դϴ�.

class CDialogProcessBar : public CDialogEx
{
	DECLARE_DYNAMIC(CDialogProcessBar)

public:
	CDialogProcessBar(CWnd* pParent = NULL);   // ǥ�� �������Դϴ�.
	virtual ~CDialogProcessBar();

// ��ȭ ���� �������Դϴ�.
	enum { IDD = IDD_DIALOG_PROCESS_BAR };

	void SetProcessPos(int pos);
	int m_nProcessPos;
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV �����Դϴ�.

	DECLARE_MESSAGE_MAP()
public:
	CProgressCtrl m_Process;
	afx_msg void OnShowWindow(BOOL bShow, UINT nStatus);
	virtual BOOL OnInitDialog();
	afx_msg void OnTimer(UINT_PTR nIDEvent);
};
