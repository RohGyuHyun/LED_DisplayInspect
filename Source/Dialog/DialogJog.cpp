// D:\Project\디지티\LED_DisplayInspect\Source\Dialog\DialogJog.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "LED_DisplayInspect.h"
#include "LED_DisplayInspectDlg.h"
#include "DialogJog.h"
#include "afxdialogex.h"


// CDialogJog 대화 상자입니다.

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

	// TODO:  여기에 추가 초기화 작업을 추가합니다.
	//m_btn_StepToJog.SetValue(TRUE);
	m_bStepToJog = m_btn_StepToJog.GetValue();
	ClickBtnenhctrlStepToJog();

	return TRUE;  // return TRUE unless you set the focus to a control
	// 예외: OCX 속성 페이지는 FALSE를 반환해야 합니다.
}

// CDialogJog 메시지 처리기입니다.
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
	// TODO: 여기에 메시지 처리기 코드를 추가합니다.
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
	// TODO: 여기에 메시지 처리기 코드를 추가합니다.
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
	// TODO: 여기에 메시지 처리기 코드를 추가합니다.
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
	// TODO: 여기에 메시지 처리기 코드를 추가합니다.
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
	// TODO: 여기에 메시지 처리기 코드를 추가합니다.
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
	// TODO: 여기에 메시지 처리기 코드를 추가합니다.
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
	// TODO: 여기에 메시지 처리기 코드를 추가합니다.
	CLED_DisplayInspectDlg *pdlg = ((CLED_DisplayInspectDlg *)AfxGetApp()->m_pMainWnd);
	CRect rect;

	if(m_bStepToJog)
	{
		pdlg->m_dlgManual->SetMoveStop(MOTION_X);
	}
}

void CDialogJog::MouseUpBtnenhctrlJogRight(short Button, short Shift, long x, long y)
{
	// TODO: 여기에 메시지 처리기 코드를 추가합니다.
	CLED_DisplayInspectDlg *pdlg = ((CLED_DisplayInspectDlg *)AfxGetApp()->m_pMainWnd);
	CRect rect;

	if(m_bStepToJog)
	{
		pdlg->m_dlgManual->SetMoveStop(MOTION_X);
	}
}

void CDialogJog::MouseUpBtnenhctrlJogTop(short Button, short Shift, long x, long y)
{
	// TODO: 여기에 메시지 처리기 코드를 추가합니다.
	CLED_DisplayInspectDlg *pdlg = ((CLED_DisplayInspectDlg *)AfxGetApp()->m_pMainWnd);
	CRect rect;

	if(m_bStepToJog)
	{
		pdlg->m_dlgManual->SetMoveStop(MOTION_Y);
	}
}

void CDialogJog::MouseUpBtnenhctrlJogBottom(short Button, short Shift, long x, long y)
{
	// TODO: 여기에 메시지 처리기 코드를 추가합니다.
	CLED_DisplayInspectDlg *pdlg = ((CLED_DisplayInspectDlg *)AfxGetApp()->m_pMainWnd);
	CRect rect;

	if(m_bStepToJog)
	{
		pdlg->m_dlgManual->SetMoveStop(MOTION_Y);
	}
}

void CDialogJog::MouseUpBtnenhctrlJogUp(short Button, short Shift, long x, long y)
{
	// TODO: 여기에 메시지 처리기 코드를 추가합니다.
	CLED_DisplayInspectDlg *pdlg = ((CLED_DisplayInspectDlg *)AfxGetApp()->m_pMainWnd);
	CRect rect;

	if(m_bStepToJog)
	{
		pdlg->m_dlgManual->SetMoveStop(MOTION_Z);
	}
}

void CDialogJog::MouseUpBtnenhctrlJogDown(short Button, short Shift, long x, long y)
{
	// TODO: 여기에 메시지 처리기 코드를 추가합니다.
	CLED_DisplayInspectDlg *pdlg = ((CLED_DisplayInspectDlg *)AfxGetApp()->m_pMainWnd);
	CRect rect;

	if(m_bStepToJog)
	{
		pdlg->m_dlgManual->SetMoveStop(MOTION_Z);
	}
}

void CDialogJog::ClickBtnenhctrlJogLeft()
{
	// TODO: 여기에 메시지 처리기 코드를 추가합니다.
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
	// TODO: 여기에 메시지 처리기 코드를 추가합니다.
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
	// TODO: 여기에 메시지 처리기 코드를 추가합니다.
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
	// TODO: 여기에 메시지 처리기 코드를 추가합니다.
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
	// TODO: 여기에 메시지 처리기 코드를 추가합니다.
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
	// TODO: 여기에 메시지 처리기 코드를 추가합니다.
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
	// TODO: 여기에 메시지 처리기 코드를 추가합니다.
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
	// TODO: 여기에 특수화된 코드를 추가 및/또는 기본 클래스를 호출합니다.
	if(pMsg->message ==WM_KEYDOWN)
	{
		if(pMsg->wParam == VK_RETURN || pMsg->wParam == VK_ESCAPE)
			return TRUE;
	}

	return CDialogEx::PreTranslateMessage(pMsg);
}
