#pragma once
#include "TypeDef.h"

// CDialogChangePassword ��ȭ �����Դϴ�.

class CDialogChangePassword : public CDialogEx
{
	DECLARE_DYNAMIC(CDialogChangePassword)

public:
	CDialogChangePassword(CWnd* pParent = NULL);   // ǥ�� �������Դϴ�.
	virtual ~CDialogChangePassword();


// ��ȭ ���� �������Դϴ�.
	enum { IDD = IDD_DIALOG_CHANGEPASSWORD };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV �����Դϴ�.
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
