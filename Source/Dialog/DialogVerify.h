#pragma once


// CDialogVerify ��ȭ �����Դϴ�.

class CDialogVerify : public CDialogEx
{
	DECLARE_DYNAMIC(CDialogVerify)

public:
	CDialogVerify(CWnd* pParent = NULL);   // ǥ�� �������Դϴ�.
	virtual ~CDialogVerify();

// ��ȭ ���� �������Դϴ�.
	enum { IDD = IDD_DIALOG_VERIFY };

public:
	void SetIdx(int x, int y, int g, BOOL isOk);
	
protected:
	CPoint m_SelectDevice;
	int m_nGrabIdx;
	BOOL m_bIsOk;

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV �����Դϴ�.

	DECLARE_MESSAGE_MAP()
public:
	DECLARE_EVENTSINK_MAP()
	void ClickBtnenhctrlVerifyOk();
	void ClickBtnenhctrlVerifyNg();
	afx_msg void OnShowWindow(BOOL bShow, UINT nStatus);
	virtual BOOL OnInitDialog();
	CBtnEnh m_btn_SelectIdx;
	virtual BOOL PreTranslateMessage(MSG* pMsg);
};
