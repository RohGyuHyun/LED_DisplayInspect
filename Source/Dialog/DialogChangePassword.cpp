// DialogChangePassword.cpp : ���� �����Դϴ�.
//

#include "stdafx.h"
#include "LED_DisplayInspect.h"
#include "DialogChangePassword.h"
#include "afxdialogex.h"


// CDialogChangePassword ��ȭ �����Դϴ�.

IMPLEMENT_DYNAMIC(CDialogChangePassword, CDialogEx)

	CDialogChangePassword::CDialogChangePassword(CWnd* pParent /*=NULL*/)
	: CDialogEx(CDialogChangePassword::IDD, pParent)
	, m_str_CurrentPW(_T(""))
	, m_str_NewPW(_T(""))
{

}

CDialogChangePassword::~CDialogChangePassword()
{
}

void CDialogChangePassword::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT_CURRENTPW, m_str_CurrentPW);
	DDX_Text(pDX, IDC_EDIT2_NEWPW, m_str_NewPW);
}


BEGIN_MESSAGE_MAP(CDialogChangePassword, CDialogEx)

END_MESSAGE_MAP()


BEGIN_EVENTSINK_MAP(CDialogChangePassword, CDialogEx)
	ON_EVENT(CDialogChangePassword, IDC_BTN_CPWOK, DISPID_CLICK, CDialogChangePassword::ClickBtnCpwok, VTS_NONE)
	ON_EVENT(CDialogChangePassword, IDC_BTN_CPWCANCEL, DISPID_CLICK, CDialogChangePassword::ClickBtnCpwcancel, VTS_NONE)
END_EVENTSINK_MAP()
// CDialogChangePassword �޽��� ó�����Դϴ�.






//20161014 ht ��й�ȣ �� �� �� ��ġ ���� �� ���� ���� ��ȯ
void CDialogChangePassword::ClickBtnCpwok()
{
	UpdateData(TRUE);

	//���� ��й�ȣ üũ
	if( m_str_CurrentPW.IsEmpty() )
	{
		AfxMessageBox(_T("������ �����մϴ�."));
		return;
	}

	//�ű� ��й�ȣ üũ
	if( m_str_NewPW.IsEmpty() )
	{
		AfxMessageBox(_T("������ �����մϴ�."));
		return;
	}

	if( !m_strPassCmp.Compare(m_str_CurrentPW) )
	{
		
		if( m_str_CurrentPW.Compare(m_str_NewPW) )
		{
			m_strPassCmp = m_str_NewPW;
			AfxMessageBox(_T("��й�ȣ�� ����Ǿ����ϴ�.\n(�����Ϸ��� 'Apply'��ư�� �����ּ���.)"));
			EndDialog(IDOK);
		}
		else
		{
			AfxMessageBox(_T("�Էµ� �ű� ��й�ȣ�� ���� ��й�ȣ�� �����մϴ�."));
		}
	}
	else
	{
		AfxMessageBox(_T("�Էµ� ���� ��й�ȣ�� ��ġ���� �ʽ��ϴ�."));
	}	
}


void CDialogChangePassword::ClickBtnCpwcancel()
{
	EndDialog(IDCANCEL);
}
