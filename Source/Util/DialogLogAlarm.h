#pragma once

#include "btnenh.h"
// CDialogLogAlarm 대화 상자입니다.

class CDialogLogAlarm : public CDialogEx
{
	DECLARE_DYNAMIC(CDialogLogAlarm)

public:
	CDialogLogAlarm(CWnd* pParent = NULL);   // 표준 생성자입니다.
	virtual ~CDialogLogAlarm();

// 대화 상자 데이터입니다.
	enum { IDD = IDD_DIALOG_LOG_ALARM };

	void SetMessage(char *msg);
protected:
	CString m_sMessage;

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnShowWindow(BOOL bShow, UINT nStatus);
	CBtnEnh m_btn_msg;
	DECLARE_EVENTSINK_MAP()
	void ClickBtnenhctrlLogBuzOff();
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	void ClickBtnenhctrlLogReset();
};
