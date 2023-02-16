// DialogProcessBar.cpp : ���� �����Դϴ�.
//

#include "stdafx.h"
#include "LED_DisplayInspect.h"
#include "DialogProcessBar.h"
#include "afxdialogex.h"


// CDialogProcessBar ��ȭ �����Դϴ�.

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


// CDialogProcessBar �޽��� ó�����Դϴ�.


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

	// TODO: ���⿡ �޽��� ó���� �ڵ带 �߰��մϴ�.
}

void CDialogProcessBar::SetProcessPos(int pos)
{
	m_Process.SetPos(pos);
}

BOOL CDialogProcessBar::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  ���⿡ �߰� �ʱ�ȭ �۾��� �߰��մϴ�.
	m_Process.SetRange(0, 99);
	return TRUE;  // return TRUE unless you set the focus to a control
	// ����: OCX �Ӽ� �������� FALSE�� ��ȯ�ؾ� �մϴ�.
}


void CDialogProcessBar::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: ���⿡ �޽��� ó���� �ڵ带 �߰� ��/�Ǵ� �⺻���� ȣ���մϴ�.

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
