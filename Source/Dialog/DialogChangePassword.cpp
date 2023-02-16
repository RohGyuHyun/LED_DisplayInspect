// DialogChangePassword.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "LED_DisplayInspect.h"
#include "DialogChangePassword.h"
#include "afxdialogex.h"


// CDialogChangePassword 대화 상자입니다.

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
// CDialogChangePassword 메시지 처리기입니다.






//20161014 ht 비밀번호 값 비교 후 일치 여부 및 변경 여부 반환
void CDialogChangePassword::ClickBtnCpwok()
{
	UpdateData(TRUE);

	//현재 비밀번호 체크
	if( m_str_CurrentPW.IsEmpty() )
	{
		AfxMessageBox(_T("공백이 존재합니다."));
		return;
	}

	//신규 비밀번호 체크
	if( m_str_NewPW.IsEmpty() )
	{
		AfxMessageBox(_T("공백이 존재합니다."));
		return;
	}

	if( !m_strPassCmp.Compare(m_str_CurrentPW) )
	{
		
		if( m_str_CurrentPW.Compare(m_str_NewPW) )
		{
			m_strPassCmp = m_str_NewPW;
			AfxMessageBox(_T("비밀번호가 변경되었습니다.\n(저장하려면 'Apply'버튼을 눌러주세요.)"));
			EndDialog(IDOK);
		}
		else
		{
			AfxMessageBox(_T("입력된 신규 비밀번호가 이전 비밀번호와 동일합니다."));
		}
	}
	else
	{
		AfxMessageBox(_T("입력된 현재 비밀번호가 일치하지 않습니다."));
	}	
}


void CDialogChangePassword::ClickBtnCpwcancel()
{
	EndDialog(IDCANCEL);
}
