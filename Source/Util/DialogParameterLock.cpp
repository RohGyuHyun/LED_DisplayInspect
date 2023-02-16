// DialogParameterLock.cpp : ���� �����Դϴ�.
//

#include "stdafx.h"
#include "LED_DisplayInspect.h"
#include "LED_DisplayInspectDlg.h"
#include "DialogParameterLock.h"
#include "afxdialogex.h"


// CDialogParameterLock ��ȭ �����Դϴ�.

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
// CDialogParameterLock �޽��� ó�����Դϴ�.







BOOL CDialogParameterLock::LoadParaPassWord(CString path)
{
	char temp[DAT_STR_LENG];
	BOOL rslt = TRUE;
	CString key, dat, temp_key;
	CString file_path, strModPath;

	//file_path = path + TEACH_PARA_FILE_NAME;
	file_path.Format(_T("%s\\Data.par"), path);

	//file ���� Ȯ�� 
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

	// TODO:  ���⿡ �߰� �ʱ�ȭ �۾��� �߰��մϴ�.
	
	//20161014 ht �������α׷��� ��й�ȣ ����
	LoadParaPassWord(DB_PATH);
	


	return TRUE;  // return TRUE unless you set the focus to a control
	// ����: OCX �Ӽ� �������� FALSE�� ��ȯ�ؾ� �մϴ�.
}

BOOL CDialogParameterLock::PreTranslateMessage(MSG* pMsg)
{
	// TODO: ���⿡ Ư��ȭ�� �ڵ带 �߰� ��/�Ǵ� �⺻ Ŭ������ ȣ���մϴ�.
	if(pMsg->message ==WM_KEYDOWN)
	{
		if(pMsg->wParam == VK_ESCAPE)
			return TRUE;
		else if(pMsg->wParam == VK_RETURN)
			ClickBtnPasswordok();

	}


	return CDialogEx::PreTranslateMessage(pMsg);
}


//20161014 ht ���� ��й�ȣ�� �Էµ� ��й�ȣ ���� �� ��ġ���� ��ȯ
void CDialogParameterLock::ClickBtnPasswordok()
{
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
	//CLED_DisplayInspectDlg *pdlg = ((CLED_DisplayInspectDlg *)AfxGetApp()->m_pMainWnd);
	UpdateData(TRUE);

	if( m_strPassCmp.IsEmpty() )
	{
		AfxMessageBox("�н����带 �Է� �ϼ���");
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
		MessageBox("�н����带 �ٽ� �Է� �ϼ���");
		m_bPassRet = FALSE;
		//pdlg->m_dlgTeach->SetParameterEnable();
	}

}



void CDialogParameterLock::ClickBtnPasswordcancel()
{
	EndDialog(IDCANCEL);
}
