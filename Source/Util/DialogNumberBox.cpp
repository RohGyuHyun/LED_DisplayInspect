// DialogNumberBox.cpp : 구현 파일입니다.
//

#include "stdafx.h"
//#include "DialogImageRect.h"
#include "DialogNumberBox.h"
//#include "LED_DisplayInspectDlg.h"

#include "afxdialogex.h"


// CDialogNumberBox 대화 상자입니다.

IMPLEMENT_DYNAMIC(CDialogNumberBox, CDialogEx)


CDialogNumberBox::CDialogNumberBox(int maxchar, char* getnum, char* title, CWnd* pParent, BOOL pwd)
	: CDialog(CDialogNumberBox::IDD, pParent)
{
	//{{AFX_DATA_INIT(CGetNumDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	m_bCalu = FALSE;
	m_nCalu = 0;

	m_FirstFlag = TRUE;
	m_intMaxChar =  maxchar;
	m_bpwd = pwd;

	/*if( m_bpwd )
		m_strGetNum = "";
	else */ //modeless
		m_strGetNum = getnum;

	if( m_strGetNum.Find('-') == -1 )
	{
		m_numFlag=TRUE;
	}
	else
	{
		m_numFlag=FALSE;
	}

	if( m_strGetNum.Find('.') == -1 )
	{
		m_nPointNum = 0;
	}
	else
	{
		m_nPointNum = 6;
	}


	m_strWndText = title;

	m_strOrg = m_strGetNum;
	m_bVkShift=FALSE;
	
	if(m_bpwd)BOOL Create(CWnd* pParentWnd) ;
}

CDialogNumberBox::~CDialogNumberBox()
{
}

void CDialogNumberBox::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_PRESENT, m_sSetValue);
	DDX_Control(pDX, IDC_MODIFY, m_sDispNum);
}


BEGIN_MESSAGE_MAP(CDialogNumberBox, CDialog)
	ON_WM_PAINT()
END_MESSAGE_MAP()


// CDialogNumberBox 메시지 처리기입니다.
BEGIN_EVENTSINK_MAP(CDialogNumberBox, CDialog)
    //{{AFX_EVENTSINK_MAP(CDialogNumberBox)
	ON_EVENT(CDialogNumberBox, IDB_BTN_PLUS, -600 /* Click */, OnBtnPlus, VTS_NONE)
	ON_EVENT(CDialogNumberBox, IDB_BTN_MINUS, -600 /* Click */, OnBtnMinus, VTS_NONE)
	ON_EVENT(CDialogNumberBox, IDB_BTN_EQUAL, -600 /* Click */, OnBtnEqual, VTS_NONE)
	ON_EVENT(CDialogNumberBox, IDB_BTN_DOT, -600 /* Click */, OnBtnDot, VTS_NONE)
	ON_EVENT(CDialogNumberBox, IDB_BTN_SIGN, -600 /* Click */, OnBtnSign, VTS_NONE)
	ON_EVENT(CDialogNumberBox, IDB_BTN_BACK, -600 /* Click */, OnBtnBack, VTS_NONE)
	ON_EVENT(CDialogNumberBox, IDB_BTN_CLEAR, -600 /* Click */, OnBtnClear, VTS_NONE)
	ON_EVENT(CDialogNumberBox, IDB_BTN_OK, -600 /* Click */, OnBtnOk, VTS_NONE)
	ON_EVENT(CDialogNumberBox, IDB_BTN_ESC, -600 /* Click */, OnBtnEsc, VTS_NONE)
	ON_EVENT(CDialogNumberBox, IDC_BTN_1, -600 /* Click */, OnBtn1, VTS_NONE)
	ON_EVENT(CDialogNumberBox, IDC_BTN_2, -600 /* Click */, OnBtn2, VTS_NONE)
	ON_EVENT(CDialogNumberBox, IDC_BTN_3, -600 /* Click */, OnBtn3, VTS_NONE)
	ON_EVENT(CDialogNumberBox, IDC_BTN_4, -600 /* Click */, OnBtn4, VTS_NONE)
	ON_EVENT(CDialogNumberBox, IDC_BTN_5, -600 /* Click */, OnBtn5, VTS_NONE)
	ON_EVENT(CDialogNumberBox, IDC_BTN_6, -600 /* Click */, OnBtn6, VTS_NONE)
	ON_EVENT(CDialogNumberBox, IDC_BTN_7, -600 /* Click */, OnBtn7, VTS_NONE)
	ON_EVENT(CDialogNumberBox, IDC_BTN_8, -600 /* Click */, OnBtn8, VTS_NONE)
	ON_EVENT(CDialogNumberBox, IDC_BTN_9, -600 /* Click */, OnBtn9, VTS_NONE)
	ON_EVENT(CDialogNumberBox, IDC_BTN_0, -600 /* Click */, OnBtn0, VTS_NONE)
	//}}AFX_EVENTSINK_MAP
END_EVENTSINK_MAP()


void CDialogNumberBox::OnBtnPlus() 
{
	// TODO: Add your control notification handler code here
//	if(m_FirstFlag ) return;

	m_bCalu = TRUE;
	m_nCalu = TRUE;

	m_FirstFlag = TRUE;

	m_strOrg = m_strGetNum;
	m_strGetNum = "";
	m_sSetValue.SetCaption(m_strOrg);
	UpdateDisplay();	
}

void CDialogNumberBox::OnBtnMinus() 
{
	// TODO: Add your control notification handler code here
//	if(m_FirstFlag ) return;

	m_bCalu = TRUE;
	m_nCalu = FALSE;

	m_FirstFlag = TRUE;

	m_strOrg = m_strGetNum;
	m_strGetNum = "";
	m_sSetValue.SetCaption(m_strOrg);
	UpdateDisplay();
}

void CDialogNumberBox::OnBtnEqual() 
{
	// TODO: Add your control notification handler code here
	double fi, se, rel;
	char buf[100];

	if( !m_bCalu ) return;

	fi = _ttof(m_strOrg);
	se = _ttof(m_strGetNum);

	if( m_nCalu )
		rel = (fi) + (se);
	else
		rel = (fi) - (se);

	if( !m_nPointNum )
		sprintf(buf, "%.0f", rel);
	else
		sprintf(buf, "%.6f", rel);

	m_strGetNum = buf;
	UpdateDisplay();

	m_FirstFlag = TRUE;
	m_bCalu = FALSE;
}

void CDialogNumberBox::OnBtnDot() 
{
	// TODO: Add your control notification handler code here
	if( m_bpwd ) return;

	if( !VerifyMaxChar() ) return;

	m_strGetNum += ".";	
	UpdateDisplay();
}

void CDialogNumberBox::OnBtnSign() 
{
	// TODO: Add your control notification handler code here
	if( m_bpwd ) return;

	if( !m_strGetNum.GetLength() ) return;

	if(m_numFlag == TRUE)
	{
		m_strGetNum.Insert(0,'-');
		m_numFlag=FALSE;
	}
	else
	{
		m_strGetNum.Remove('-');
		m_numFlag=TRUE;
	}

	UpdateDisplay();
}

void CDialogNumberBox::OnBtnBack() 
{
	// TODO: Add your control notification handler code here
	m_FirstFlag = FALSE;

	int len = m_strGetNum.GetLength();
	if(len)
	{
		m_strGetNum.SetAt(len-1,' ');
		m_strGetNum.TrimRight();
		UpdateDisplay();
	}
}

void CDialogNumberBox::OnBtnClear() 
{
	// TODO: Add your control notification handler code here
	m_strGetNum = "";	
	m_numFlag=TRUE;
	UpdateDisplay();
}

void CDialogNumberBox::OnBtnOk() 
{
	// TODO: Add your control notification handler code here
	if(m_bpwd) 	{
//		GetNum_rtn = TRUE;
//		lstrcpy(codebuf, m_strGetNum);
		DestroyWindow();
	}
	else 
	{
		if(m_strGetNum.IsEmpty())
			CDialog::OnCancel();
		else
			CDialog::OnOK();
	}
}

void CDialogNumberBox::OnBtnEsc() 
{
	// TODO: Add your control notification handler code here
	if(m_bpwd) {	
//		GetNum_rtn = TRUE;
//		lstrcpy(codebuf, "0");
		DestroyWindow();

	}
	else CDialog::OnCancel();
}

void CDialogNumberBox::OnBtn1() 
{
	// TODO: Add your control notification handler code here
	if( m_FirstFlag )
	{
		m_FirstFlag = FALSE;
		m_numFlag=TRUE;
		m_strGetNum = "";
	}

	if( !VerifyMaxChar() ) return;

	m_strGetNum += "1";
	UpdateDisplay();
}

void CDialogNumberBox::OnBtn2() 
{
	// TODO: Add your control notification handler code here
	if( m_FirstFlag )
	{
		m_FirstFlag = FALSE;
		m_numFlag=TRUE;
		m_strGetNum = "";
	}

	if( !VerifyMaxChar() ) return;

	m_strGetNum += "2";
	UpdateDisplay();
}

void CDialogNumberBox::OnBtn3() 
{
	// TODO: Add your control notification handler code here
	if( m_FirstFlag )
	{
		m_FirstFlag = FALSE;
		m_numFlag=TRUE;
		m_strGetNum = "";
	}

	if( !VerifyMaxChar() ) return;

	m_strGetNum += "3";
	UpdateDisplay();
}

void CDialogNumberBox::OnBtn4() 
{
	// TODO: Add your control notification handler code here
	if( m_FirstFlag )
	{
		m_FirstFlag = FALSE;
		m_numFlag=TRUE;
		m_strGetNum = "";
	}

	if( !VerifyMaxChar() ) return;

	m_strGetNum += "4";
	UpdateDisplay();
}

void CDialogNumberBox::OnBtn5() 
{
	// TODO: Add your control notification handler code here
	if( m_FirstFlag )
	{
		m_FirstFlag = FALSE;
		m_numFlag=TRUE;
		m_strGetNum = "";
	}

	if( !VerifyMaxChar() ) return;

	m_strGetNum += "5";
	UpdateDisplay();
}

void CDialogNumberBox::OnBtn6() 
{
	// TODO: Add your control notification handler code here
	if( m_FirstFlag )
	{
		m_FirstFlag = FALSE;
		m_numFlag=TRUE;
		m_strGetNum = "";
	}

	if( !VerifyMaxChar() ) return;

	m_strGetNum += "6";
	UpdateDisplay();
}

void CDialogNumberBox::OnBtn7() 
{
	// TODO: Add your control notification handler code here
	if( m_FirstFlag )
	{
		m_FirstFlag = FALSE;
		m_numFlag=TRUE;
		m_strGetNum = "";
	}

	if( !VerifyMaxChar() ) return;

	m_strGetNum += "7";
	UpdateDisplay();
}

void CDialogNumberBox::OnBtn8() 
{
	// TODO: Add your control notification handler code here
	if( m_FirstFlag )
	{
		m_FirstFlag = FALSE;
		m_numFlag=TRUE;
		m_strGetNum = "";
	}

	if( !VerifyMaxChar() ) return;

	m_strGetNum += "8";
	UpdateDisplay();
}

void CDialogNumberBox::OnBtn9() 
{
	// TODO: Add your control notification handler code here
	if( m_FirstFlag )
	{
		m_FirstFlag = FALSE;
		m_numFlag=TRUE;
		m_strGetNum = "";
	}

	if( !VerifyMaxChar() ) return;

	m_strGetNum += "9";
	UpdateDisplay();
}

void CDialogNumberBox::OnBtn0() 
{
	// TODO: Add your control notification handler code here
	if( m_FirstFlag )
	{
		m_FirstFlag = FALSE;
		m_numFlag=TRUE;
		m_strGetNum = "";
	}

	if( !VerifyMaxChar() ) return;

	m_strGetNum += "0";
	UpdateDisplay();
}

void CDialogNumberBox::InitStaticDispWnd()
{
    CStatic* pRect = (CStatic*)GetDlgItem(IDC_MODIFY);
    pRect->GetWindowRect(&m_rect);
    pRect->DestroyWindow();
    ScreenToClient(&m_rect);

    TEXTMETRIC tm;
    CClientDC dc(this);
    dc.GetTextMetrics(&tm);
    m_cxChar = tm.tmAveCharWidth;
    m_cyChar = tm.tmHeight - tm.tmDescent;
}

BOOL CDialogNumberBox::VerifyMaxChar()
{
	if( m_strGetNum.GetLength() >= m_intMaxChar ) return FALSE;
	else return TRUE;
}

void CDialogNumberBox::UpdateDisplay(COLORREF bkcolor)
{
	int len, i;
	CString bufstr;

	i = 0; len = 0;
	m_sDispNum.SetCaption(m_strGetNum);
}

void CDialogNumberBox::SetWindowTitle(LPCTSTR title)
{
	m_strWndText = title;
}

void CDialogNumberBox::OnPaint() 
{
	CPaintDC dc(this); // device context for painting
	
	// TODO: Add your message handler code here
	UpdateDisplay();
	SetWindowText(m_strWndText);	
	
	// Do not call CDialog::OnPaint() for painting messages
}

CString CDialogNumberBox::GetstrNum()
{
	return m_strGetNum;
}

void CDialogNumberBox::SetstrNum(CString strNum)
{
	m_strGetNum = strNum;
}

void CDialogNumberBox::SetstrNum(double dNum)
{
	m_strGetNum.Format(_T("%3.3f"),dNum);
}

void CDialogNumberBox::SetstrNum(int nNum)
{
	m_strGetNum.Format(_T("%d"),nNum);
}

BOOL CDialogNumberBox::PreTranslateMessage(MSG* pMsg) 
{
	if(pMsg->wParam!=0)
	{
		if(pMsg->wParam == VK_SHIFT)
		{
			if(pMsg->message == WM_KEYDOWN)
			{
				m_bVkShift = TRUE;
			}
			else if(pMsg->message == WM_KEYUP)
			{
				m_bVkShift = FALSE;
			}
		}

		if(pMsg->message == WM_KEYDOWN)
		{
			if(pMsg->wParam==VK_RETURN)
			{
				OnBtnOk();
				return true;
			}
			if(pMsg->wParam==VK_BACK)
			{
				OnBtnBack();
				return true;
			}
			if(pMsg->wParam==0xBD)
			{
				OnBtnMinus();
				return true;
			}
			if(pMsg->wParam==0xBE)
			{
				OnBtnDot();
				return true;
			}
			if(pMsg->wParam==0xBB)
			{
				if(m_bVkShift)
					OnBtnPlus();
				else
					OnBtnEqual();
				return true;
			}

			if(pMsg->wParam=='0')
			{
				OnBtn0();
				return true;
			}
			if(pMsg->wParam=='1')
			{
				OnBtn1();
				return true;
			}
			if(pMsg->wParam=='2')
			{
				OnBtn2();
				return true;
			}
			if(pMsg->wParam=='3')
			{
				OnBtn3();
				return true;
			}
			if(pMsg->wParam=='4')
			{
				OnBtn4();
				return true;
			}
			if(pMsg->wParam=='5')
			{
				OnBtn5();
				return true;
			}
			if(pMsg->wParam=='6')
			{
				OnBtn6();
				return true;
			}
			if(pMsg->wParam=='7')
			{
				OnBtn7();
				return true;
			}
			if(pMsg->wParam=='8')
			{
				OnBtn8();
				return true;
			}
			if(pMsg->wParam=='9')
			{
				OnBtn9();
				return true;
			}
		}
	}

	return CDialog::PreTranslateMessage(pMsg);
}
