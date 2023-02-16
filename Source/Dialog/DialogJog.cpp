// D:\Project\����Ƽ\LED_DisplayInspect\Source\Dialog\DialogJog.cpp : ���� �����Դϴ�.
//

#include "stdafx.h"
#include "LED_DisplayInspect.h"
#include "LED_DisplayInspectDlg.h"
#include "DialogJog.h"
#include "afxdialogex.h"


// CDialogJog ��ȭ �����Դϴ�.

IMPLEMENT_DYNAMIC(CDialogJog, CDialogEx)

CDialogJog::CDialogJog(CWnd* pParent /*=NULL*/)
	: CDialogEx(CDialogJog::IDD, pParent)
	, m_edit_lSpeed(60000)
	, m_edit_dStep(1.0)
{

}

CDialogJog::~CDialogJog()
{
}

void CDialogJog::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_BTNENHCTRL_STEP_TO_JOG, m_btn_StepToJog);
	DDX_Text(pDX, IDC_EDIT_PARA_0, m_edit_lSpeed);
	DDX_Text(pDX, IDC_EDIT_PARA_1, m_edit_dStep);
}

BEGIN_MESSAGE_MAP(CDialogJog, CDialogEx)
END_MESSAGE_MAP()

BOOL CDialogJog::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  ���⿡ �߰� �ʱ�ȭ �۾��� �߰��մϴ�.
	//m_btn_StepToJog.SetValue(TRUE);
	m_bStepToJog = m_btn_StepToJog.GetValue();
	ClickBtnenhctrlStepToJog();

	return TRUE;  // return TRUE unless you set the focus to a control
	// ����: OCX �Ӽ� �������� FALSE�� ��ȯ�ؾ� �մϴ�.
}

// CDialogJog �޽��� ó�����Դϴ�.
BEGIN_EVENTSINK_MAP(CDialogJog, CDialogEx)
	ON_EVENT(CDialogJog, IDC_BTNENHCTRL_JOG_LEFT, DISPID_MOUSEDOWN, CDialogJog::MouseDownBtnenhctrlJogLeft, VTS_I2 VTS_I2 VTS_I4 VTS_I4)
	ON_EVENT(CDialogJog, IDC_BTNENHCTRL_JOG_RIGHT, DISPID_MOUSEDOWN, CDialogJog::MouseDownBtnenhctrlJogRight, VTS_I2 VTS_I2 VTS_I4 VTS_I4)
	ON_EVENT(CDialogJog, IDC_BTNENHCTRL_JOG_TOP, DISPID_MOUSEDOWN, CDialogJog::MouseDownBtnenhctrlJogTop, VTS_I2 VTS_I2 VTS_I4 VTS_I4)
	ON_EVENT(CDialogJog, IDC_BTNENHCTRL_JOG_BOTTOM, DISPID_MOUSEDOWN, CDialogJog::MouseDownBtnenhctrlJogBottom, VTS_I2 VTS_I2 VTS_I4 VTS_I4)
	ON_EVENT(CDialogJog, IDC_BTNENHCTRL_JOG_UP, DISPID_MOUSEDOWN, CDialogJog::MouseDownBtnenhctrlJogUp, VTS_I2 VTS_I2 VTS_I4 VTS_I4)
	ON_EVENT(CDialogJog, IDC_BTNENHCTRL_JOG_DOWN, DISPID_MOUSEDOWN, CDialogJog::MouseDownBtnenhctrlJogDown, VTS_I2 VTS_I2 VTS_I4 VTS_I4)
	ON_EVENT(CDialogJog, IDC_BTNENHCTRL_JOG_LEFT, DISPID_MOUSEUP, CDialogJog::MouseUpBtnenhctrlJogLeft, VTS_I2 VTS_I2 VTS_I4 VTS_I4)
	ON_EVENT(CDialogJog, IDC_BTNENHCTRL_JOG_RIGHT, DISPID_MOUSEUP, CDialogJog::MouseUpBtnenhctrlJogRight, VTS_I2 VTS_I2 VTS_I4 VTS_I4)
	ON_EVENT(CDialogJog, IDC_BTNENHCTRL_JOG_TOP, DISPID_MOUSEUP, CDialogJog::MouseUpBtnenhctrlJogTop, VTS_I2 VTS_I2 VTS_I4 VTS_I4)
	ON_EVENT(CDialogJog, IDC_BTNENHCTRL_JOG_BOTTOM, DISPID_MOUSEUP, CDialogJog::MouseUpBtnenhctrlJogBottom, VTS_I2 VTS_I2 VTS_I4 VTS_I4)
	ON_EVENT(CDialogJog, IDC_BTNENHCTRL_JOG_UP, DISPID_MOUSEUP, CDialogJog::MouseUpBtnenhctrlJogUp, VTS_I2 VTS_I2 VTS_I4 VTS_I4)
	ON_EVENT(CDialogJog, IDC_BTNENHCTRL_JOG_DOWN, DISPID_MOUSEUP, CDialogJog::MouseUpBtnenhctrlJogDown, VTS_I2 VTS_I2 VTS_I4 VTS_I4)
	ON_EVENT(CDialogJog, IDC_BTNENHCTRL_JOG_LEFT, DISPID_CLICK, CDialogJog::ClickBtnenhctrlJogLeft, VTS_NONE)
	ON_EVENT(CDialogJog, IDC_BTNENHCTRL_JOG_RIGHT, DISPID_CLICK, CDialogJog::ClickBtnenhctrlJogRight, VTS_NONE)
	ON_EVENT(CDialogJog, IDC_BTNENHCTRL_JOG_TOP, DISPID_CLICK, CDialogJog::ClickBtnenhctrlJogTop, VTS_NONE)
	ON_EVENT(CDialogJog, IDC_BTNENHCTRL_JOG_BOTTOM, DISPID_CLICK, CDialogJog::ClickBtnenhctrlJogBottom, VTS_NONE)
	ON_EVENT(CDialogJog, IDC_BTNENHCTRL_JOG_UP, DISPID_CLICK, CDialogJog::ClickBtnenhctrlJogUp, VTS_NONE)
	ON_EVENT(CDialogJog, IDC_BTNENHCTRL_JOG_DOWN, DISPID_CLICK, CDialogJog::ClickBtnenhctrlJogDown, VTS_NONE)
	ON_EVENT(CDialogJog, IDC_BTNENHCTRL_STEP_TO_JOG, DISPID_CLICK, CDialogJog::ClickBtnenhctrlStepToJog, VTS_NONE)
END_EVENTSINK_MAP()

void CDialogJog::MouseDownBtnenhctrlJogLeft(short Button, short Shift, long x, long y)
{
	// TODO: ���⿡ �޽��� ó���� �ڵ带 �߰��մϴ�.
	CLED_DisplayInspectDlg *pdlg = ((CLED_DisplayInspectDlg *)AfxGetApp()->m_pMainWnd);
	CRect rect;
	UpdateData(TRUE);

	if(m_bStepToJog)
	{
		GetDlgItem(IDC_BTNENHCTRL_JOG_LEFT)->GetClientRect(&rect);

		pdlg->m_dlgManual->SetJogMove(MOTION_X, FALSE, m_edit_lSpeed, rect, x, y);
	}
}

void CDialogJog::MouseDownBtnenhctrlJogRight(short Button, short Shift, long x, long y)
{
	// TODO: ���⿡ �޽��� ó���� �ڵ带 �߰��մϴ�.
	CLED_DisplayInspectDlg *pdlg = ((CLED_DisplayInspectDlg *)AfxGetApp()->m_pMainWnd);
	CRect rect;
	UpdateData(TRUE);

	if(m_bStepToJog)
	{
		GetDlgItem(IDC_BTNENHCTRL_JOG_RIGHT)->GetClientRect(&rect);

		pdlg->m_dlgManual->SetJogMove(MOTION_X, TRUE, m_edit_lSpeed, rect, x, y);
	}
}

void CDialogJog::MouseDownBtnenhctrlJogTop(short Button, short Shift, long x, long y)
{
	// TODO: ���⿡ �޽��� ó���� �ڵ带 �߰��մϴ�.
	CLED_DisplayInspectDlg *pdlg = ((CLED_DisplayInspectDlg *)AfxGetApp()->m_pMainWnd);
	CRect rect;
	UpdateData(TRUE);

	if(m_bStepToJog)
	{
		GetDlgItem(IDC_BTNENHCTRL_JOG_TOP)->GetClientRect(&rect);

		pdlg->m_dlgManual->SetJogMove(MOTION_Y, FALSE, m_edit_lSpeed, rect, x, y);
	}
}

void CDialogJog::MouseDownBtnenhctrlJogBottom(short Button, short Shift, long x, long y)
{
	// TODO: ���⿡ �޽��� ó���� �ڵ带 �߰��մϴ�.
	CLED_DisplayInspectDlg *pdlg = ((CLED_DisplayInspectDlg *)AfxGetApp()->m_pMainWnd);
	CRect rect;
	UpdateData(TRUE);

	if(m_bStepToJog)
	{
		GetDlgItem(IDC_BTNENHCTRL_JOG_BOTTOM)->GetClientRect(&rect);

		pdlg->m_dlgManual->SetJogMove(MOTION_Y, TRUE, m_edit_lSpeed, rect, x, y);
	}
}

void CDialogJog::MouseDownBtnenhctrlJogUp(short Button, short Shift, long x, long y)
{
	// TODO: ���⿡ �޽��� ó���� �ڵ带 �߰��մϴ�.
	CLED_DisplayInspectDlg *pdlg = ((CLED_DisplayInspectDlg *)AfxGetApp()->m_pMainWnd);
	CRect rect;
	UpdateData(TRUE);

	if(m_bStepToJog)
	{
		GetDlgItem(IDC_BTNENHCTRL_JOG_UP)->GetClientRect(&rect);

		pdlg->m_dlgManual->SetJogMove(MOTION_Z, FALSE, m_edit_lSpeed, rect, x, y);
	}
}

void CDialogJog::MouseDownBtnenhctrlJogDown(short Button, short Shift, long x, long y)
{
	// TODO: ���⿡ �޽��� ó���� �ڵ带 �߰��մϴ�.
	CLED_DisplayInspectDlg *pdlg = ((CLED_DisplayInspectDlg *)AfxGetApp()->m_pMainWnd);
	CRect rect;
	UpdateData(TRUE);

	if(m_bStepToJog)
	{
		GetDlgItem(IDC_BTNENHCTRL_JOG_DOWN)->GetClientRect(&rect);

		pdlg->m_dlgManual->SetJogMove(MOTION_Z, TRUE, m_edit_lSpeed, rect, x, y);
	}
}

void CDialogJog::MouseUpBtnenhctrlJogLeft(short Button, short Shift, long x, long y)
{
	// TODO: ���⿡ �޽��� ó���� �ڵ带 �߰��մϴ�.
	CLED_DisplayInspectDlg *pdlg = ((CLED_DisplayInspectDlg *)AfxGetApp()->m_pMainWnd);
	CRect rect;

	if(m_bStepToJog)
	{
		pdlg->m_dlgManual->SetMoveStop(MOTION_X);
	}
}

void CDialogJog::MouseUpBtnenhctrlJogRight(short Button, short Shift, long x, long y)
{
	// TODO: ���⿡ �޽��� ó���� �ڵ带 �߰��մϴ�.
	CLED_DisplayInspectDlg *pdlg = ((CLED_DisplayInspectDlg *)AfxGetApp()->m_pMainWnd);
	CRect rect;

	if(m_bStepToJog)
	{
		pdlg->m_dlgManual->SetMoveStop(MOTION_X);
	}
}

void CDialogJog::MouseUpBtnenhctrlJogTop(short Button, short Shift, long x, long y)
{
	// TODO: ���⿡ �޽��� ó���� �ڵ带 �߰��մϴ�.
	CLED_DisplayInspectDlg *pdlg = ((CLED_DisplayInspectDlg *)AfxGetApp()->m_pMainWnd);
	CRect rect;

	if(m_bStepToJog)
	{
		pdlg->m_dlgManual->SetMoveStop(MOTION_Y);
	}
}

void CDialogJog::MouseUpBtnenhctrlJogBottom(short Button, short Shift, long x, long y)
{
	// TODO: ���⿡ �޽��� ó���� �ڵ带 �߰��մϴ�.
	CLED_DisplayInspectDlg *pdlg = ((CLED_DisplayInspectDlg *)AfxGetApp()->m_pMainWnd);
	CRect rect;

	if(m_bStepToJog)
	{
		pdlg->m_dlgManual->SetMoveStop(MOTION_Y);
	}
}

void CDialogJog::MouseUpBtnenhctrlJogUp(short Button, short Shift, long x, long y)
{
	// TODO: ���⿡ �޽��� ó���� �ڵ带 �߰��մϴ�.
	CLED_DisplayInspectDlg *pdlg = ((CLED_DisplayInspectDlg *)AfxGetApp()->m_pMainWnd);
	CRect rect;

	if(m_bStepToJog)
	{
		pdlg->m_dlgManual->SetMoveStop(MOTION_Z);
	}
}

void CDialogJog::MouseUpBtnenhctrlJogDown(short Button, short Shift, long x, long y)
{
	// TODO: ���⿡ �޽��� ó���� �ڵ带 �߰��մϴ�.
	CLED_DisplayInspectDlg *pdlg = ((CLED_DisplayInspectDlg *)AfxGetApp()->m_pMainWnd);
	CRect rect;

	if(m_bStepToJog)
	{
		pdlg->m_dlgManual->SetMoveStop(MOTION_Z);
	}
}

void CDialogJog::ClickBtnenhctrlJogLeft()
{
	// TODO: ���⿡ �޽��� ó���� �ڵ带 �߰��մϴ�.
	CLED_DisplayInspectDlg *pdlg = ((CLED_DisplayInspectDlg *)AfxGetApp()->m_pMainWnd);
	CRect rect;
	UpdateData(TRUE);

	if(!m_bStepToJog)
	{
		pdlg->m_dlgManual->SetStepMove(MOTION_X, -m_edit_dStep, m_edit_lSpeed);
	}
}

void CDialogJog::ClickBtnenhctrlJogRight()
{
	// TODO: ���⿡ �޽��� ó���� �ڵ带 �߰��մϴ�.
	CLED_DisplayInspectDlg *pdlg = ((CLED_DisplayInspectDlg *)AfxGetApp()->m_pMainWnd);
	CRect rect;
	UpdateData(TRUE);

	if(!m_bStepToJog)
	{
		pdlg->m_dlgManual->SetStepMove(MOTION_X, m_edit_dStep, m_edit_lSpeed);
	}
}

void CDialogJog::ClickBtnenhctrlJogTop()
{
	// TODO: ���⿡ �޽��� ó���� �ڵ带 �߰��մϴ�.
	CLED_DisplayInspectDlg *pdlg = ((CLED_DisplayInspectDlg *)AfxGetApp()->m_pMainWnd);
	CRect rect;
	UpdateData(TRUE);

	if(!m_bStepToJog)
	{
		pdlg->m_dlgManual->SetStepMove(MOTION_Y, m_edit_dStep, m_edit_lSpeed);
	}
}

void CDialogJog::ClickBtnenhctrlJogBottom()
{
	// TODO: ���⿡ �޽��� ó���� �ڵ带 �߰��մϴ�.
	CLED_DisplayInspectDlg *pdlg = ((CLED_DisplayInspectDlg *)AfxGetApp()->m_pMainWnd);
	CRect rect;
	UpdateData(TRUE);

	if(!m_bStepToJog)
	{
		pdlg->m_dlgManual->SetStepMove(MOTION_Y, -m_edit_dStep, m_edit_lSpeed);
	}
}

void CDialogJog::ClickBtnenhctrlJogUp()
{
	// TODO: ���⿡ �޽��� ó���� �ڵ带 �߰��մϴ�.
	CLED_DisplayInspectDlg *pdlg = ((CLED_DisplayInspectDlg *)AfxGetApp()->m_pMainWnd);
	CRect rect;
	UpdateData(TRUE);

	if(!m_bStepToJog)
	{
		pdlg->m_dlgManual->SetStepMove(MOTION_Z, -m_edit_dStep, m_edit_lSpeed);
	}
}

void CDialogJog::ClickBtnenhctrlJogDown()
{
	// TODO: ���⿡ �޽��� ó���� �ڵ带 �߰��մϴ�.
	CLED_DisplayInspectDlg *pdlg = ((CLED_DisplayInspectDlg *)AfxGetApp()->m_pMainWnd);
	CRect rect;
	UpdateData(TRUE);

	if(!m_bStepToJog)
	{
		pdlg->m_dlgManual->SetStepMove(MOTION_Z, m_edit_dStep, m_edit_lSpeed);
	}
}

void CDialogJog::ClickBtnenhctrlStepToJog()
{
	// TODO: ���⿡ �޽��� ó���� �ڵ带 �߰��մϴ�.
	if(m_bStepToJog)
	{
		m_btn_StepToJog.SetWindowTextA(_T("Step"));
		m_bStepToJog = MOTION_STEP_MODE;
	}
	else
	{
		m_btn_StepToJog.SetWindowTextA(_T("Jog"));
		m_bStepToJog = MOTION_JOG_MODE;
	}
}


BOOL CDialogJog::PreTranslateMessage(MSG* pMsg)
{
	// TODO: ���⿡ Ư��ȭ�� �ڵ带 �߰� ��/�Ǵ� �⺻ Ŭ������ ȣ���մϴ�.
	if(pMsg->message ==WM_KEYDOWN)
	{
		if(pMsg->wParam == VK_RETURN || pMsg->wParam == VK_ESCAPE)
			return TRUE;
	}

	return CDialogEx::PreTranslateMessage(pMsg);
}
