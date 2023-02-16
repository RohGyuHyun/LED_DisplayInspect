// DialogProcessBar.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "LED_DisplayInspect.h"
#include "DialogProcessBar.h"
#include "afxdialogex.h"


// CDialogProcessBar 대화 상자입니다.

IMPLEMENT_DYNAMIC(CDialogProcessBar, CDialogEx)

CDialogProcessBar::CDialogProcessBar(CWnd* pParent /*=NULL*/)
	: CDialogEx(CDialogProcessBar::IDD, pParent)
{

}

CDialogProcessBar::~CDialogProcessBar()
{
}

void CDialogProcessBar::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_PROGRESS, m_Process);
}


BEGIN_MESSAGE_MAP(CDialogProcessBar, CDialogEx)
	ON_WM_SHOWWINDOW()
	ON_WM_TIMER()
END_MESSAGE_MAP()


// CDialogProcessBar 메시지 처리기입니다.


void CDialogProcessBar::OnShowWindow(BOOL bShow, UINT nStatus)
{
	CDialogEx::OnShowWindow(bShow, nStatus);

	if(bShow)
	{
		m_nProcessPos = 0;
		SetTimer(0, 100, NULL);
	}
	else
	{
		KillTimer(0);
	}

	// TODO: 여기에 메시지 처리기 코드를 추가합니다.
}

void CDialogProcessBar::SetProcessPos(int pos)
{
	m_Process.SetPos(pos);
}

BOOL CDialogProcessBar::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  여기에 추가 초기화 작업을 추가합니다.
	m_Process.SetRange(0, 99);
	return TRUE;  // return TRUE unless you set the focus to a control
	// 예외: OCX 속성 페이지는 FALSE를 반환해야 합니다.
}


void CDialogProcessBar::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.

	switch(nIDEvent)
	{
	case 0:
		SetProcessPos(m_nProcessPos++);
		if(m_nProcessPos == 99)
		{
			m_nProcessPos = 0;
		}
		break;
	}

	CDialogEx::OnTimer(nIDEvent);
}
