// DialogParameterLock.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "LED_DisplayInspect.h"
#include "LED_DisplayInspectDlg.h"
#include "DialogParameterLock.h"
#include "afxdialogex.h"


// CDialogParameterLock 대화 상자입니다.

IMPLEMENT_DYNAMIC(CDialogParameterLock, CDialogEx)

CDialogParameterLock::CDialogParameterLock(CWnd* pParent /*=NULL*/)  //20160906 sc
	: CDialogEx(CDialogParameterLock::IDD, pParent)
	, m_edit_para_pass(_T(""))
{
	m_bPassRet = FALSE; //20160906 sc
}

CDialogParameterLock::~CDialogParameterLock()
{
}

void CDialogParameterLock::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT1, m_edit_para_pass);
}


BEGIN_MESSAGE_MAP(CDialogParameterLock, CDialogEx)
	
END_MESSAGE_MAP()



BEGIN_EVENTSINK_MAP(CDialogParameterLock, CDialogEx)
	ON_EVENT(CDialogParameterLock, IDC_BTN_PASSWORDOK, DISPID_CLICK, CDialogParameterLock::ClickBtnPasswordok, VTS_NONE)
	ON_EVENT(CDialogParameterLock, IDC_BTN_PASSWORDCANCEL, DISPID_CLICK, CDialogParameterLock::ClickBtnPasswordcancel, VTS_NONE)
END_EVENTSINK_MAP()
// CDialogParameterLock 메시지 처리기입니다.







BOOL CDialogParameterLock::LoadParaPassWord(CString path)
{
	char temp[DAT_STR_LENG];
	BOOL rslt = TRUE;
	CString key, dat, temp_key;
	CString file_path, strModPath;

	//file_path = path + TEACH_PARA_FILE_NAME;
	file_path.Format(_T("%s\\Data.par"), path);

	//file 존재 확인 
	HANDLE fd = CreateFile( file_path,
							GENERIC_READ|GENERIC_WRITE,
							0,
							NULL,
							OPEN_EXISTING,
							FILE_ATTRIBUTE_NORMAL,
							NULL
							);
	if(((LONG)fd)<0)
	{
		rslt = FALSE;
	}
	else
	{
		CloseHandle(fd);
		
		//Read INI
		key.Empty();
		key.Format("PASSWORD");
		GetPrivateProfileString("DATA_PARA", key, _T("0000"), temp, sizeof(temp), file_path);
		dat.Format("%s", temp);
		m_strPassCmp.Format("%s", dat);		
	}
	return TRUE;
}

BOOL CDialogParameterLock::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  여기에 추가 초기화 작업을 추가합니다.
	
	//20161014 ht 현재프로그램의 비밀번호 얻어옴
	LoadParaPassWord(DB_PATH);
	


	return TRUE;  // return TRUE unless you set the focus to a control
	// 예외: OCX 속성 페이지는 FALSE를 반환해야 합니다.
}

BOOL CDialogParameterLock::PreTranslateMessage(MSG* pMsg)
{
	// TODO: 여기에 특수화된 코드를 추가 및/또는 기본 클래스를 호출합니다.
	if(pMsg->message ==WM_KEYDOWN)
	{
		if(pMsg->wParam == VK_ESCAPE)
			return TRUE;
		else if(pMsg->wParam == VK_RETURN)
			ClickBtnPasswordok();

	}


	return CDialogEx::PreTranslateMessage(pMsg);
}


//20161014 ht 현재 비밀번호와 입력된 비밀번호 대조 후 일치여부 반환
void CDialogParameterLock::ClickBtnPasswordok()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	//CLED_DisplayInspectDlg *pdlg = ((CLED_DisplayInspectDlg *)AfxGetApp()->m_pMainWnd);
	UpdateData(TRUE);

	if( m_strPassCmp.IsEmpty() )
	{
		AfxMessageBox("패스워드를 입력 하세요");
		return;
	}


	if( !m_strPassCmp.Compare(m_edit_para_pass) )
	{
		//m_edit_para_pass.Format("");
		m_bPassRet = TRUE;
		EndDialog(IDOK);
	}
	else
	{
		m_edit_para_pass.Empty();
		MessageBox("패스워드를 다시 입력 하세요");
		m_bPassRet = FALSE;
		//pdlg->m_dlgTeach->SetParameterEnable();
	}

}



void CDialogParameterLock::ClickBtnPasswordcancel()
{
	EndDialog(IDCANCEL);
}
