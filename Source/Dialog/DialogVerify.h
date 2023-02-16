#pragma once


// CDialogVerify 대화 상자입니다.

class CDialogVerify : public CDialogEx
{
	DECLARE_DYNAMIC(CDialogVerify)

public:
	CDialogVerify(CWnd* pParent = NULL);   // 표준 생성자입니다.
	virtual ~CDialogVerify();

// 대화 상자 데이터입니다.
	enum { IDD = IDD_DIALOG_VERIFY };

public:
	void SetIdx(int x, int y, int g, BOOL isOk);
	
protected:
	CPoint m_SelectDevice;
	int m_nGrabIdx;
	BOOL m_bIsOk;

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

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
