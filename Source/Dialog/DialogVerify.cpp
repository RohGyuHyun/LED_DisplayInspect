// DialogVerify.cpp : ���� �����Դϴ�.
//

#include "stdafx.h"
#include "LED_DisplayInspect.h"
#include "DialogVerify.h"
#include "afxdialogex.h"
#include "LED_DisplayInspectDlg.h"


// CDialogVerify ��ȭ �����Դϴ�.

IMPLEMENT_DYNAMIC(CDialogVerify, CDialogEx)

CDialogVerify::CDialogVerify(CWnd* pParent /*=NULL*/)
	: CDialogEx(CDialogVerify::IDD, pParent)
{

}

CDialogVerify::~CDialogVerify()
{
}

void CDialogVerify::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_BTNENHCTRL_LABEL_28, m_btn_SelectIdx);
}


BEGIN_MESSAGE_MAP(CDialogVerify, CDialogEx)
	ON_WM_SHOWWINDOW()
END_MESSAGE_MAP()


// CDialogVerify �޽��� ó�����Դϴ�.
BEGIN_EVENTSINK_MAP(CDialogVerify, CDialogEx)
	ON_EVENT(CDialogVerify, IDC_BTNENHCTRL_VERIFY_OK, DISPID_CLICK, CDialogVerify::ClickBtnenhctrlVerifyOk, VTS_NONE)
	ON_EVENT(CDialogVerify, IDC_BTNENHCTRL_VERIFY_NG, DISPID_CLICK, CDialogVerify::ClickBtnenhctrlVerifyNg, VTS_NONE)
END_EVENTSINK_MAP()


void CDialogVerify::ClickBtnenhctrlVerifyOk()
{
	// TODO: ���⿡ �޽��� ó���� �ڵ带 �߰��մϴ�.
	CLED_DisplayInspectDlg *pdlg = ((CLED_DisplayInspectDlg *)AfxGetApp()->m_pMainWnd);
	pdlg->m_dlgNgView->SetGrid(7, (m_SelectDevice.x * pdlg->m_dlgTeach->GetTeachPara().TeachGrabCnt) + m_nGrabIdx, m_SelectDevice.y, m_nGrabIdx);
	pdlg->m_dlgNgView->SetVerifyRslt(FALSE, m_SelectDevice.x, m_SelectDevice.y, m_nGrabIdx);
	ShowWindow(SW_HIDE);
	pdlg->SetGridDbClick(m_SelectDevice.x + 1, m_SelectDevice.y + 1, m_nGrabIdx + 1);
}


void CDialogVerify::ClickBtnenhctrlVerifyNg()
{
	// TODO: ���⿡ �޽��� ó���� �ڵ带 �߰��մϴ�.
	CLED_DisplayInspectDlg *pdlg = ((CLED_DisplayInspectDlg *)AfxGetApp()->m_pMainWnd);
	pdlg->m_dlgNgView->SetGrid(8, (m_SelectDevice.x * pdlg->m_dlgTeach->GetTeachPara().TeachGrabCnt) + m_nGrabIdx, m_SelectDevice.y, m_nGrabIdx);
	pdlg->m_dlgNgView->SetVerifyRslt(TRUE, m_SelectDevice.x, m_SelectDevice.y, m_nGrabIdx);
	ShowWindow(SW_HIDE);
	pdlg->SetGridDbClick(m_SelectDevice.x + 1, m_SelectDevice.y + 1, m_nGrabIdx + 1);
}


void CDialogVerify::OnShowWindow(BOOL bShow, UINT nStatus)
{
	CDialogEx::OnShowWindow(bShow, nStatus);

	if(bShow)
	{

	}
	else
	{

	}

	// TODO: ���⿡ �޽��� ó���� �ڵ带 �߰��մϴ�.
}


BOOL CDialogVerify::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  ���⿡ �߰� �ʱ�ȭ �۾��� �߰��մϴ�.

	return TRUE;  // return TRUE unless you set the focus to a control
	// ����: OCX �Ӽ� �������� FALSE�� ��ȯ�ؾ� �մϴ�.
}

void CDialogVerify::SetIdx(int x, int y, int g, BOOL isOk)
{
	CString temp;
	m_SelectDevice.SetPoint(x - 1, y - 1);
	m_nGrabIdx = g - 1;
	m_bIsOk = isOk;
	temp.Format(_T("Select X : %02d Y : %02d"), x, y);
	m_btn_SelectIdx.SetWindowTextA(temp);
}

BOOL CDialogVerify::PreTranslateMessage(MSG* pMsg)
{
	// TODO: ���⿡ Ư��ȭ�� �ڵ带 �߰� ��/�Ǵ� �⺻ Ŭ������ ȣ���մϴ�.
	if(pMsg->message ==WM_KEYDOWN)
	{
		if(pMsg->wParam == VK_RETURN || pMsg->wParam == VK_ESCAPE)
			return TRUE;
	}

	return CDialogEx::PreTranslateMessage(pMsg);
}
