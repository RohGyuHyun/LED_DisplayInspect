#pragma once
#include "TypeDef.h"

// CDialogChangePassword 대화 상자입니다.

class CDialogChangePassword : public CDialogEx
{
	DECLARE_DYNAMIC(CDialogChangePassword)

public:
	CDialogChangePassword(CWnd* pParent = NULL);   // 표준 생성자입니다.
	virtual ~CDialogChangePassword();


// 대화 상자 데이터입니다.
	enum { IDD = IDD_DIALOG_CHANGEPASSWORD };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.
	DECLARE_MESSAGE_MAP()
protected:
	CString m_strPassCmp;
public:
	CString m_str_CurrentPW;
	CString m_str_NewPW;
	afx_msg void OnBnClickedOk();
	afx_msg void OnBnClickedCancel();
	void SetCurrentPW(CString pw) { m_strPassCmp = pw; }
	CString GetCurrentPW() { return m_strPassCmp; }
	DECLARE_EVENTSINK_MAP()
	void ClickBtnCpwok();
	void ClickBtnCpwcancel();
};
