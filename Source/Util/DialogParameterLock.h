#pragma once
#include "TypeDef.h"

// CDialogParameterLock ��ȭ �����Դϴ�.

class CDialogParameterLock : public CDialogEx  //20160906 sc
{
	DECLARE_DYNAMIC(CDialogParameterLock)

public:
	CDialogParameterLock(CWnd* pParent = NULL);   // ǥ�� �������Դϴ�.
	virtual ~CDialogParameterLock();

// ��ȭ ���� �������Դϴ�.
	enum { IDD = IDD_DIALOG_PARA_PASS };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV �����Դϴ�.

	DECLARE_MESSAGE_MAP()

	CString m_strPass;
	CString m_strPassCmp;
	BOOL m_bPassRet;

public:

	CString m_edit_para_pass;
	BOOL LoadParaPassWord(CString path);

	BOOL GetPassRet() { return m_bPassRet; }
	void SetCurrentPW(CString pw) { m_strPassCmp = pw; }
	virtual BOOL OnInitDialog();
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	DECLARE_EVENTSINK_MAP()
	void ClickBtnPasswordok();
	void CellClickBtnPasswordcancel(short nUniqueID);
	void ClickBtnPasswordcancel();
};
