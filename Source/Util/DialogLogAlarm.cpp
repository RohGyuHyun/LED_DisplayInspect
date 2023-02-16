// DialogLogAlarm.cpp : ���� �����Դϴ�.
//

#include "stdafx.h"
#include "LED_DisplayInspect.h"
#include "LED_DisplayInspectDlg.h"
#include "DialogLogAlarm.h"
#include "afxdialogex.h"


// CDialogLogAlarm ��ȭ �����Դϴ�.

IMPLEMENT_DYNAMIC(CDialogLogAlarm, CDialogEx)

CDialogLogAlarm::CDialogLogAlarm(CWnd* pParent /*=NULL*/)
	: CDialogEx(CDialogLogAlarm::IDD, pParent)
{
	m_sMessage.Empty();
}

CDialogLogAlarm::~CDialogLogAlarm()
{
}

void CDialogLogAlarm::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_BTNENHCTRL_LOG_ALARM_MSG, m_btn_msg);
}


BEGIN_MESSAGE_MAP(CDialogLogAlarm, CDialogEx)
	ON_WM_SHOWWINDOW()
END_MESSAGE_MAP()


// CDialogLogAlarm �޽��� ó�����Դϴ�.
void CDialogLogAlarm::SetMessage(char *msg)
{
	//TCHAR* sData = new TCHAR[512];
	//MultiByteToWideChar(CP_ACP, 0, (LPCSTR)msg, -1, sData, 512);
	m_sMessage.Format(_T("%s"), msg);
}

void CDialogLogAlarm::OnShowWindow(BOOL bShow, UINT nStatus)
{
	CDialogEx::OnShowWindow(bShow, nStatus);

	// TODO: ���⿡ �޽��� ó���� �ڵ带 �߰��մϴ�.
	if(bShow)
	{
		m_btn_msg.SetCaption(m_sMessage);
	}
	else
	{

	}
}
BEGIN_EVENTSINK_MAP(CDialogLogAlarm, CDialogEx)
	ON_EVENT(CDialogLogAlarm, IDC_BTNENHCTRL_LOG_BUZ_OFF, DISPID_CLICK, CDialogLogAlarm::ClickBtnenhctrlLogBuzOff, VTS_NONE)
	ON_EVENT(CDialogLogAlarm, IDC_BTNENHCTRL_LOG_RESET, DISPID_CLICK, CDialogLogAlarm::ClickBtnenhctrlLogReset, VTS_NONE)
END_EVENTSINK_MAP()


void CDialogLogAlarm::ClickBtnenhctrlLogBuzOff()
{
	// TODO: ���⿡ �޽��� ó���� �ڵ带 �߰��մϴ�.
	CLED_DisplayInspectDlg *pdlg = ((CLED_DisplayInspectDlg *)AfxGetApp()->m_pMainWnd);

	pdlg->SetTowerLamp(OUTPUT_LAM_RED);
}


BOOL CDialogLogAlarm::PreTranslateMessage(MSG* pMsg)
{
	// TODO: ���⿡ Ư��ȭ�� �ڵ带 �߰� ��/�Ǵ� �⺻ Ŭ������ ȣ���մϴ�.
	if(pMsg->message ==WM_KEYDOWN)
	{
		if(pMsg->wParam == VK_RETURN || pMsg->wParam == VK_ESCAPE)
			return TRUE;
	}

	return CDialogEx::PreTranslateMessage(pMsg);
}


void CDialogLogAlarm::ClickBtnenhctrlLogReset()
{
	// TODO: ���⿡ �޽��� ó���� �ڵ带 �߰��մϴ�.
	CLED_DisplayInspectDlg *pdlg = ((CLED_DisplayInspectDlg *)AfxGetApp()->m_pMainWnd);

	pdlg->SetTowerLamp(OUTPUT_LAM_YELLOW);

	ShowWindow(SW_HIDE);
}
