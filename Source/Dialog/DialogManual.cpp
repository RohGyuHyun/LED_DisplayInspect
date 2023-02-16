// DialogManual.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "typedef.h"
#include "LED_DisplayInspect.h"
#include "LED_DisplayInspectDlg.h"
#include "DialogManual.h"
#include "afxdialogex.h"

DIO_CFG g_pDI_DEV_NO[] = {
	{1, 0x000},
	{1, 0x010},
	{1, 0x020},
	{1, 0x030},
	{1, 0x040}, 
	{1, 0x050},
	{1, 0x060},
	{1, 0x070},
	{1, 0x080},
	{1, 0x090},
	{1, 0x0A0},
	{1, 0x0B0},
	{1, 0x0C0},
	{1, 0x0D0},
};

DIO_CFG g_pDO_DEV_NO[] = {
	{1, 0x000},
	{1, 0x010},
	{1, 0x020},
	{1, 0x030},
	{1, 0x040}, 
	{1, 0x050},
	{1, 0x060},
	{1, 0x070},
	{1, 0x080},
	{1, 0x090},
	{1, 0x0A0},
	{1, 0x0B0},
	{1, 0x0C0},
	{1, 0x0D0},
};

BAUD_RATE g_pBaudRate[] = {

	{BAUD_RATE_0},
	{BAUD_RATE_1},
	{BAUD_RATE_2},
	{BAUD_RATE_3},
	{BAUD_RATE_4}, 
	{BAUD_RATE_5},
	{BAUD_RATE_6},
	{BAUD_RATE_7}
};

// CDialogManual 대화 상자입니다.

IMPLEMENT_DYNAMIC(CDialogManual, CDialogEx)

CDialogManual::CDialogManual(CWnd* pParent /*=NULL*/)
	: CDialogEx(CDialogManual::IDD, pParent)
	, m_edit_nSlaveNo_0(1)
	, m_edit_dPulseToMM_0(1000)
	, m_edit_nSpeed_0(10000)
	, m_edit_nSlaveNo_1(0)
	, m_edit_dPulseToMM_1(1000)
	, m_edit_nSpeed_1(10000)
	, m_edit_nSlaveNo_2(2)
	, m_edit_dPulseToMM_2(1000)
	, m_edit_nSpeed_2(10000)
{
	memset(m_cDILabel, NULL, sizeof(char) * DI_MAX_NUM * 100);
	memset(m_cDOLabel, NULL, sizeof(char) * DO_MAX_NUM * 100);
	memset(m_bDI, NULL, sizeof(BOOL) * DI_MAX_NUM);
	memset(m_bDO, NULL, sizeof(BOOL) * DO_MAX_NUM);
	memset(m_bDI_list, NULL, sizeof(BOOL) * DI_MAX_NUM);
	memset(m_bDO_list, NULL, sizeof(BOOL) * DO_MAX_NUM);

	memset(&m_ManualPara, NULL, sizeof(typeManualPara));

	m_bStepToJog = FALSE;
	InitializeCriticalSection(m_AxisSyncIntlock);

}

CDialogManual::~CDialogManual()
{
	m_bAmpStateStart = FALSE;
	m_bAmpStateEnd = TRUE;
	CloseMotion();
	KillTimer(READ_INPUT);
	KillTimer(READ_OUTPUT);
	KillTimer(TIMER_MOTION_HOME + MOTION_X);
	KillTimer(TIMER_MOTION_HOME + MOTION_Y);
	KillTimer(TIMER_MOTION_HOME + MOTION_Z);
	KillTimer(TIMER_MOTION_MONITOR);
	DeleteCriticalSection(m_AxisSyncIntlock);
}

void CDialogManual::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST_INPUT, m_list_input);
	DDX_Control(pDX, IDC_LIST_OUTPUT, m_list_output);

	DDX_Control(pDX, IDC_BTNENHCTRL_SERVO_ORIGIN_12, m_btn_MotionX_CurPos);
	DDX_Control(pDX, IDC_BTNENHCTRL_SERVO_ORIGIN_13, m_btn_MotionX_P_Limt);
	DDX_Control(pDX, IDC_BTNENHCTRL_SERVO_ORIGIN_14, m_btn_MotionX_N_Limt);
	DDX_Control(pDX, IDC_BTNENHCTRL_SERVO_ORIGIN_0, m_btn_MotionX_Home);
	DDX_Control(pDX, IDC_BTNENHCTRL_SERVO_AMP_0, m_btn_MotionX_Amp);

	DDX_Control(pDX, IDC_BTNENHCTRL_SERVO_ORIGIN_17, m_btn_MotionY_CurPos);
	DDX_Control(pDX, IDC_BTNENHCTRL_SERVO_ORIGIN_18, m_btn_MotionY_P_Limt);
	DDX_Control(pDX, IDC_BTNENHCTRL_SERVO_ORIGIN_19, m_btn_MotionY_N_Limt);
	DDX_Control(pDX, IDC_BTNENHCTRL_SERVO_ORIGIN_1, m_btn_MotionY_Home);
	DDX_Control(pDX, IDC_BTNENHCTRL_SERVO_AMP_1, m_btn_MotionY_Amp);

	DDX_Control(pDX, IDC_BTNENHCTRL_SERVO_ORIGIN_22, m_btn_MotionZ_CurPos);
	DDX_Control(pDX, IDC_BTNENHCTRL_SERVO_ORIGIN_23, m_btn_MotionZ_P_Limt);
	DDX_Control(pDX, IDC_BTNENHCTRL_SERVO_ORIGIN_24, m_btn_MotionZ_N_Limt);
	DDX_Control(pDX, IDC_BTNENHCTRL_SERVO_ORIGIN_2, m_btn_MotionZ_Home);
	DDX_Control(pDX, IDC_BTNENHCTRL_SERVO_AMP_2, m_btn_MotionZ_Amp);

	DDX_Control(pDX, IDC_COMBO_COM_AXIS_0, m_combo_nComportNum_0);
	DDX_Control(pDX, IDC_COMBO_BAUDRATE_AXIS_0, m_combo_nBaudrate_0);
	DDX_Text(pDX, IDC_EDIT_SLAVE_NO_AXIS_0, m_edit_nSlaveNo_0);
	DDX_Text(pDX, IDC_EDIT_PULS_TO_MM_AXIS_0, m_edit_dPulseToMM_0);
	DDX_Text(pDX, IDC_EDIT_SPEED_AXIS_0, m_edit_nSpeed_0);

	DDX_Text(pDX, IDC_EDIT_SLAVE_NO_AXIS_1, m_edit_nSlaveNo_1);
	DDX_Text(pDX, IDC_EDIT_PULS_TO_MM_AXIS_1, m_edit_dPulseToMM_1);
	DDX_Text(pDX, IDC_EDIT_SPEED_AXIS_1, m_edit_nSpeed_1);

	DDX_Text(pDX, IDC_EDIT_SLAVE_NO_AXIS_2, m_edit_nSlaveNo_2);
	DDX_Text(pDX, IDC_EDIT_PULS_TO_MM_AXIS_2, m_edit_dPulseToMM_2);
	DDX_Text(pDX, IDC_EDIT_SPEED_AXIS_2, m_edit_nSpeed_2);
	DDX_Control(pDX, IDC_BTNENHCTRL_MOTION_CONNECT, m_btn_MotionConnect);
	DDX_Control(pDX, IDC_BTNENHCTRL_SERVO_ORIGIN_STATE_1, m_btn_MotionX_OriginState);
	DDX_Control(pDX, IDC_BTNENHCTRL_SERVO_ORIGIN_STATE_2, m_btn_MotionY_OriginState);
	DDX_Control(pDX, IDC_BTNENHCTRL_SERVO_ORIGIN_STATE_3, m_btn_MotionZ_OriginState);
}


BEGIN_MESSAGE_MAP(CDialogManual, CDialogEx)
	ON_WM_SHOWWINDOW()
	ON_NOTIFY(NM_DBLCLK, IDC_LIST_OUTPUT, &CDialogManual::OnNMDblclkListIoOutput)
	ON_WM_TIMER()
	ON_CBN_SELCHANGE(IDC_COMBO_COM_AXIS_0, &CDialogManual::OnCbnSelchangeComboComAxis0)
	ON_CBN_SELCHANGE(IDC_COMBO_BAUDRATE_AXIS_0, &CDialogManual::OnCbnSelchangeComboBaudrateAxis0)
END_MESSAGE_MAP()


// CDialogManual 메시지 처리기입니다.


BOOL CDialogManual::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  여기에 추가 초기화 작업을 추가합니다.
	CString strLog;
	strLog.Format(_T("CDialogManual::OnInitDialog() Start!!"));
	WriteLog(strLog);

	strLog.Format(_T("IO Label Load Comp !!"));
	if(!LoadDIOLabel())
	{
		strLog.Format(_T("IO Label Load Fail !!"));
	}
	
	WriteLog(strLog);
	
	strLog.Format(_T("Manual Para Load Comp !!"));
	if(!LoadManualPara(DB_PATH))
	{
		strLog.Format(_T("Manual Para Load Fail !!"));
	}
	
	WriteLog(strLog);

	InitCombo();

	strLog.Format(_T("IO Init Comp !!"));
	if(!InitIO())
	{
		strLog.Format(_T("IO Init Fail !!"));
	}
	
	WriteLog(strLog);

	for(int i = 0; i < MAX_MOTION_CNT; i++)
	{
		strLog.Format(_T("Motion Num %d Init Comp !!"), i);
		if(!InitMotion(i))
		{
			strLog.Format(_T("Motion Num %d Init Fail !!"), i);
		}
		
		WriteLog(strLog);
	}
	

	m_dlgJog = new CDialogJog();
	m_dlgJog->Create(IDD_DIALOG_JOG, this);

	m_bAmpStateStart = TRUE;
	m_bAmpStateEnd = FALSE;

	m_pAmpStateThread = AfxBeginThread(AmpStateThread, this, THREAD_PRIORITY_NORMAL);
	if (m_pAmpStateThread) 
		m_pAmpStateThread->m_bAutoDelete = TRUE;

	strLog.Format(_T("CDialogManual::OnInitDialog() End!!"));
	WriteLog(strLog);

	return TRUE;  // return TRUE unless you set the focus to a control
	// 예외: OCX 속성 페이지는 FALSE를 반환해야 합니다.
}


void CDialogManual::OnShowWindow(BOOL bShow, UINT nStatus)
{
	CDialogEx::OnShowWindow(bShow, nStatus);
	CLED_DisplayInspectDlg *pdlg = ((CLED_DisplayInspectDlg *)AfxGetApp()->m_pMainWnd);
	CRect rect;

	if(bShow)
	{
		LoadManualPara(DB_PATH);
		Update(FALSE);

		GetDlgItem(IDC_STATIC_MANUAL_JOG)->GetClientRect(rect);
		GetDlgItem(IDC_STATIC_MANUAL_JOG)->GetWindowRect(rect);
		m_dlgJog->SetWindowPos((&CWnd::wndTopMost), rect.left, rect.top, rect.Width(), rect.Height(),
                                          SWP_NOMOVE|SWP_NOSIZE|SWP_SHOWWINDOW); 
		m_dlgJog->ShowWindow(SW_SHOW);
		m_dlgJog->MoveWindow(rect.left, rect.top, rect.Width(), rect.Height(), TRUE);
		SetTimer(READ_INPUT, 100, NULL);
		SetTimer(READ_OUTPUT, 100, NULL);
		SetTimer(TIMER_MOTION_MONITOR, 100, NULL);
		UpdateData(FALSE);
	}
	else
	{
		m_dlgJog->ShowWindow(SW_HIDE);
		KillTimer(READ_INPUT);
		KillTimer(READ_OUTPUT);
		KillTimer(TIMER_MOTION_MONITOR);
	}

	// TODO: 여기에 메시지 처리기 코드를 추가합니다.
}
BEGIN_EVENTSINK_MAP(CDialogManual, CDialogEx)
	ON_EVENT(CDialogManual, IDC_BTNENHCTRL_APPLY, DISPID_CLICK, CDialogManual::ClickBtnenhctrlApply, VTS_NONE)
	ON_EVENT(CDialogManual, IDC_BTNENHCTRL_OK, DISPID_CLICK, CDialogManual::ClickBtnenhctrlOk, VTS_NONE)
	ON_EVENT(CDialogManual, IDC_BTNENHCTRL_CANCEL, DISPID_CLICK, CDialogManual::ClickBtnenhctrlCancel, VTS_NONE)
	ON_EVENT(CDialogManual, IDC_BTNENHCTRL_SERVO_ORIGIN_0, DISPID_CLICK, CDialogManual::ClickBtnenhctrlServoOrigin0, VTS_NONE)
	ON_EVENT(CDialogManual, IDC_BTNENHCTRL_SERVO_ORIGIN_1, DISPID_CLICK, CDialogManual::ClickBtnenhctrlServoOrigin1, VTS_NONE)
	ON_EVENT(CDialogManual, IDC_BTNENHCTRL_SERVO_ORIGIN_2, DISPID_CLICK, CDialogManual::ClickBtnenhctrlServoOrigin2, VTS_NONE)
	ON_EVENT(CDialogManual, IDC_BTNENHCTRL_SERVO_AMP_0, DISPID_CLICK, CDialogManual::ClickBtnenhctrlServoAmp0, VTS_NONE)
	ON_EVENT(CDialogManual, IDC_BTNENHCTRL_SERVO_AMP_1, DISPID_CLICK, CDialogManual::ClickBtnenhctrlServoAmp1, VTS_NONE)
	ON_EVENT(CDialogManual, IDC_BTNENHCTRL_SERVO_AMP_2, DISPID_CLICK, CDialogManual::ClickBtnenhctrlServoAmp2, VTS_NONE)
	ON_EVENT(CDialogManual, IDC_BTNENHCTRL_MOTOR_STOP, DISPID_CLICK, CDialogManual::ClickBtnenhctrlMotorStop, VTS_NONE)
	ON_EVENT(CDialogManual, IDC_BTNENHCTRL_MOTOR_ALARM_RESET, DISPID_CLICK, CDialogManual::ClickBtnenhctrlMotorAlarmReset, VTS_NONE)
	ON_EVENT(CDialogManual, IDC_BTNENHCTRL_MOTION_CONNECT, DISPID_CLICK, CDialogManual::ClickBtnenhctrlMotionConnect, VTS_NONE)
END_EVENTSINK_MAP()

BOOL CDialogManual::LoadDIOLabel()
{
	BOOL rslt = TRUE;
	CString key, dat;
	int i;
	CString file_path, pat_path, extention, master_path;
	int idx = 0;

	CString strLog;
	strLog.Format(_T("LoadDIOLabel Start !!"));
	WriteLog(strLog);
	
	file_path.Format(_T("%s%s"), DB_PATH, DIO_LABEL_FILE_NAME);

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
		strLog.Format(_T("LoadDIOLabel Fail !!"));
		WriteLog(strLog);
	}
	else
	{
		CloseHandle(fd);
		
		for(i = 0; i < DI_MAX_NUM; i++)
		{
			key.Empty();
			key.Format(_T("X%03x"), g_pDI_DEV_NO[0].nDevNo + i);
			GetPrivateProfileString(_T("DIGITAL INPUT LABEL"), key, _T("0.0"), m_cDILabel[i], sizeof(m_cDILabel), file_path);
			m_strDILabel[i] = m_strDILabel[i] + m_cDILabel[i];
		}

		for(i = 0; i < DO_MAX_NUM; i++)
		{
			key.Empty();
			key.Format(_T("Y%03x"), g_pDO_DEV_NO[0].nDevNo + i);
			GetPrivateProfileString(_T("DIGITAL OUTPUT LABEL"), key, _T("0.0"), m_cDOLabel[i], sizeof(m_cDOLabel), file_path);
			m_strDOLabel[i] = m_strDOLabel[i] + m_cDOLabel[i];
		}
		
	}
	fd=0;

	if(rslt)
	{
		//List//////////////////////////////////////////////////////////////////////
		LV_COLUMN m_lvColumn;
		CImageList *img_list_slct;
		CBitmap bitmap;
		CString str;
	
		img_list_slct = new CImageList;
		img_list_slct->Create(16, 16, ILC_COLOR4, 3, 3);
	
		bitmap.LoadBitmap(IDB_BITMAP_IO_OFF);
		img_list_slct->Add(&bitmap, RGB(0, 0, 0));
		bitmap.DeleteObject();
		bitmap.LoadBitmap(IDB_BITMAP_IO_ON);
		img_list_slct->Add(&bitmap, RGB(0, 0, 0));
		bitmap.DeleteObject();
		m_list_input.SetImageList(img_list_slct, LVSIL_SMALL);
		m_list_output.SetImageList(img_list_slct, LVSIL_SMALL);

		m_lvColumn.mask= LVCF_FMT | LVCF_SUBITEM | LVCF_TEXT | LVCF_WIDTH;
		m_lvColumn.fmt=LVCFMT_LEFT;
		m_lvColumn.pszText=_T("INPUT NO.");
		m_lvColumn.iSubItem=0;
		m_lvColumn.cx=110;
		m_list_input.InsertColumn(0, &m_lvColumn);
		m_lvColumn.pszText=_T("OUTPUT NO.");
		m_list_output.InsertColumn(0, &m_lvColumn);

		m_lvColumn.mask= LVCF_FMT | LVCF_SUBITEM | LVCF_TEXT | LVCF_WIDTH;
		m_lvColumn.fmt=LVCFMT_LEFT;
		m_lvColumn.pszText=_T("NAME");
		m_lvColumn.iSubItem=0;
		m_lvColumn.cx=185;
		m_list_input.InsertColumn(1, &m_lvColumn);
		m_list_output.InsertColumn(1, &m_lvColumn);

		m_font.CreatePointFont(100, _T("굴림"));
		m_list_input.SetFont(&m_font, TRUE);
		m_list_output.SetFont(&m_font, TRUE);

		for(int i = 0; i < DI_MAX_NUM; i++)
		{
			str.Format(_T("X%03x"), g_pDI_DEV_NO[0].nDevNo + i);
			m_list_input.InsertItem(i, str);

			m_list_input.SetItemText(i, 1, m_cDILabel[i]);
		}

		for(int i = 0; i < DO_MAX_NUM; i++)
		{
			str.Format(_T("Y%03x"), g_pDO_DEV_NO[0].nDevNo + i);
			m_list_output.InsertItem(i, str);

			m_list_output.SetItemText(i, 1, m_cDOLabel[i]);
		}
	}

	strLog.Format(_T("LoadDIOLabel End !!"));
	WriteLog(strLog);

	return rslt;
}

void CDialogManual::ClickBtnenhctrlApply()
{
	// TODO: 여기에 메시지 처리기 코드를 추가합니다.
	UpdateData(TRUE);
	Update(TRUE);	
}

void CDialogManual::ClickBtnenhctrlOk()
{
	// TODO: 여기에 메시지 처리기 코드를 추가합니다.
	UpdateData(TRUE);
	Update(TRUE);
	SaveManualPara(DB_PATH);
	ShowWindow(SW_HIDE);
}

void CDialogManual::ClickBtnenhctrlCancel()
{
	// TODO: 여기에 메시지 처리기 코드를 추가합니다.
	ShowWindow(SW_HIDE);
}

void CDialogManual::OnNMDblclkListIoOutput(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMITEMACTIVATE pNMItemActivate = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.

	NM_LISTVIEW* pNMListView = (NM_LISTVIEW*)pNMHDR;

	int idx = pNMListView->iItem;

	SetOutput(MOTION_Y, m_OutputAddr[idx], !m_bDO_list[idx]);

	*pResult = 0;
}

BOOL CDialogManual::CheckInputStatus(int nMotionNum, DWORD dwAdd)
{
	BOOL bRet = TRUE;
#ifndef NOTE_BOOK
	DWORD dwIOInput;
#endif

	m_AxisSyncIntlock.Lock();	
#ifdef MOTION
	if(FAS_GetIOInput((BYTE)m_ManualPara.ManualComport, (BYTE)m_ManualPara.ManualSlaveNo[nMotionNum], &dwIOInput) == FMM_OK)
	{
		if(dwIOInput & dwAdd)
			bRet = TRUE;
		else
			bRet = FALSE;
	}
	else
	{
		bRet = FALSE;
	}
#endif
	m_AxisSyncIntlock.Unlock();			

	return bRet;
}

BOOL CDialogManual::CheckOutputStatus(int nMotionNum, DWORD dwAdd)
{
	BOOL bRet(TRUE);
#ifndef NOTE_BOOK
	DWORD dwIOOutput;
#endif

	m_AxisSyncIntlock.Lock();	
#ifdef MOTION
	if(FAS_GetIOOutput((BYTE)m_ManualPara.ManualComport, (BYTE)m_ManualPara.ManualSlaveNo[nMotionNum], &dwIOOutput) == FMM_OK)
	{
		if(dwIOOutput & dwAdd)
			bRet = TRUE;
		else
			bRet = FALSE;
	}
	else
	{
		bRet = FALSE;
	}
#endif
	m_AxisSyncIntlock.Unlock();			

	return bRet;
}

BOOL CDialogManual::GetInput(int nMotionNum, int nInputIdx)
{
	return CheckInputStatus(nMotionNum, m_InputAddr[nInputIdx]);
}

BOOL CDialogManual::GetOutput(int nMotionNum, int nOutputIdx)
{
	return CheckOutputStatus(nMotionNum, m_OutputAddr[nOutputIdx]);
}

BOOL CDialogManual::WriteOutput(int nMotionNum, int nOutputIdx, BOOL isOn)
{
	return SetOutput(nMotionNum, m_OutputAddr[nOutputIdx], isOn);
}

BOOL CDialogManual::SetOutput(int nMotionNum, DWORD nOutputIdx, BOOL isOn)
{
	int bRet(TRUE);
	DWORD dwIOOutput = 0;

	m_AxisSyncIntlock.Lock();	
#ifdef MOTION
	if(isOn)
	{
		bRet = FAS_SetIOOutput((BYTE)m_ManualPara.ManualComport, (BYTE)m_ManualPara.ManualSlaveNo[nMotionNum], nOutputIdx, dwIOOutput);
	}
	else
	{
		bRet = FAS_SetIOOutput((BYTE)m_ManualPara.ManualComport, (BYTE)m_ManualPara.ManualSlaveNo[nMotionNum], dwIOOutput, nOutputIdx);
	}
#endif
	m_AxisSyncIntlock.Unlock();			

	return bRet;
}

void CDialogManual::ReadDisplayInputStatus(BOOL disp)
{
	int i;
	LV_ITEM lvitem;

	char str[100];
	BOOL in;

	for(i = 0; i < DI_MAX_NUM; i++)
	{
		sprintf(str, "X%03x", g_pDI_DEV_NO[0].nDevNo + i);
		
		//IO reading
		in = GetInput(MOTION_Y, i);
		if(in)
		{
			if((m_bDI_list[i] != in))
			{
				if(disp)
				{
					lvitem.mask=LVIF_TEXT | LVIF_IMAGE;
					lvitem.iItem=i;
					lvitem.iSubItem=0;
					lvitem.iImage = 1;
					lvitem.pszText=str;
					m_list_input.SetItem(&lvitem);
					m_bDI_list[i] = in;
				}
			}
			m_bDI[i] = in;
		}
		else
		{
			if((m_bDI_list[i] != in))
			{
				if(disp)
				{
					lvitem.mask=LVIF_TEXT | LVIF_IMAGE;
					lvitem.iItem=i;
					lvitem.iSubItem=0;
					lvitem.iImage = 0;
					lvitem.pszText=str;
					m_list_input.SetItem(&lvitem);
					m_bDI_list[i] = in;
				}
			}
			m_bDI[i] = in;
		}
	}
}

void CDialogManual::ReadDisplayOutputStatus(BOOL disp)
{
	int i;
	LV_ITEM lvitem;

	char str[100];
	BOOL out;

	for(i = 0; i < DO_MAX_NUM; i++)
	{
		sprintf(str, "Y%03x", g_pDO_DEV_NO[0].nDevNo + i);
		
		//IO reading
		out = GetOutput(MOTION_Y, i);
		if(out)
		{
			if((m_bDO_list[i] != out))
			{
				if(disp)
				{
					lvitem.mask=LVIF_TEXT | LVIF_IMAGE;
					lvitem.iItem=i;
					lvitem.iSubItem=0;
					lvitem.iImage = 1;
					lvitem.pszText=str;
					m_list_output.SetItem(&lvitem);
					m_bDO_list[i] = out;
				}
			}
			m_bDO[i] = out;
		}
		else
		{
			if((m_bDO_list[i] != out))
			{
				if(disp)
				{
					lvitem.mask=LVIF_TEXT | LVIF_IMAGE;
					lvitem.iItem=i;
					lvitem.iSubItem=0;
					lvitem.iImage = 0;
					lvitem.pszText=str;
					m_list_output.SetItem(&lvitem);
					m_bDO_list[i] = out;
				}
			}
			m_bDO[i] = out;		
		}
	}
}

void CDialogManual::SetAllHomeFlag(BOOL isFlag)
{
	for(int i = 0; i < MAX_MOTION_CNT; i++)
	{
		m_ManualPara.ManualServoOrigin[i] = isFlag;
	}
}

void CDialogManual::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.
	CLED_DisplayInspectDlg *pdlg = ((CLED_DisplayInspectDlg *)AfxGetApp()->m_pMainWnd);
	CString strLog;
	int motion_idx = 0;
#ifdef MOTION
	long level;
#endif
	int motion_state[5];
	memset(motion_state, NULL, sizeof(int) * 5);
	switch(nIDEvent)
	{
	case 0:
		ReadDisplayInputStatus(IsWindowVisible());
		break;
	case 1:
		ReadDisplayOutputStatus(IsWindowVisible());
		break;
	case TIMER_MOTION_HOME + MOTION_X:
	case TIMER_MOTION_HOME + MOTION_Y:
	case TIMER_MOTION_HOME + MOTION_Z:
#ifdef MOTION

		if(m_bAmpState)
		{
			if(!GetInput(MOTION_Y, INPUT_AREA_SENSER))
			{
				pdlg->m_dlgProcessBar->ShowWindow(SW_HIDE);
				KillTimer(nIDEvent);
				strLog.Format(_T("Area Senser 감지"));
				pdlg->m_dlgLogAlarm->SetMessage((LPSTR)(LPCSTR)strLog);
				pdlg->m_dlgLogAlarm->ShowWindow(SW_SHOW);

				pdlg->SetTowerLamp(OUTPUT_LAM_RED, TRUE);

				for(int i = 0; i < MAX_MOTION_CNT; i++)
				{
					SetMoveStop(i);
				}
			}
		}
		else
		{
			pdlg->m_dlgProcessBar->ShowWindow(SW_HIDE);
			KillTimer(nIDEvent);
		}

		motion_idx = nIDEvent % 100;
		if(GetEZCurPos(motion_idx) == 0.0)
		{
			KillTimer(nIDEvent);
			m_ManualPara.ManualServoOrigin[motion_idx] = TRUE;
			strLog.Format(_T("Motion Num %d Origin End !!"), nIDEvent - 100);
			WriteLog(strLog);
		}

		if(m_ManualPara.ManualServoOrigin[MOTION_X] && m_ManualPara.ManualServoOrigin[MOTION_Y] && m_ManualPara.ManualServoOrigin[MOTION_Z])
		{
			pdlg->m_dlgProcessBar->ShowWindow(SW_HIDE);
			strLog.Format(_T("All Motion Origin End"));
			pdlg->WriteLog(strLog, TYPE_LOG_MAIN);
		}
#else
		pdlg->m_dlgProcessBar->ShowWindow(SW_HIDE);
		KillTimer(nIDEvent);
		m_ManualPara.ManualServoOrigin[motion_idx] = TRUE;
		strLog.Format(_T("Motion Num %d Origin End !!"), nIDEvent - 100);
		WriteLog(strLog);
#endif
		break;
	case TIMER_MOTION_MONITOR:
		for(int i = 0; i < MAX_MOTION_CNT; i++)
		{
#ifdef MOTION
			motion_state[MOTION_STATE_CUR_POS] = GetEZCurPos(i);
			motion_state[MOTION_STATE_AMP_FAULT_LEVEL] = GetAmpFaultLevel(i, level);
			motion_state[MOTION_STATE_LIMT_SENSER_P] = GetLimtSenser(i, TRUE);
			motion_state[MOTION_STATE_LIMT_SENSER_N] = GetLimtSenser(i, FALSE);
			if(i == 2)
			{
				motion_state[MOTION_STATE_HOME] = GetLimtSenser(i,FALSE);
			}
			else{
			motion_state[MOTION_STATE_HOME] = GetHomeSenser(i);
			}
#endif
			MotionStateUpdate(i, motion_state);
		}
		break;
	case TIMER_MOTION_MOVE_COMP + MOTION_X:
	case TIMER_MOTION_MOVE_COMP + MOTION_Y:
	case TIMER_MOTION_MOVE_COMP + MOTION_Z:
		motion_idx = nIDEvent % 300;

		if(!GetInput(MOTION_Y, INPUT_AREA_SENSER) && !pdlg->GetSeqStart())
		{
			KillTimer(nIDEvent);
			strLog.Format(_T("Area Senser 감지"));
			pdlg->m_dlgLogAlarm->SetMessage((LPSTR)(LPCSTR)strLog);
			pdlg->m_dlgLogAlarm->ShowWindow(SW_SHOW);

			pdlg->SetTowerLamp(OUTPUT_LAM_RED, TRUE);
			KillTimer(TIMER_MOTION_MOVE_COMP + motion_idx);
			
			for(int i = 0; i < MAX_MOTION_CNT; i++)
			{
				SetMoveStop(i);
			}
		}

		if(m_nMovePos[motion_idx] == GetEZCurPos(motion_idx))
		{
			KillTimer(TIMER_MOTION_MOVE_COMP + motion_idx);
		}
		break;
	case TIMER_MOTION_JOG:
		if(m_bAmpState)
		{
			if(!GetInput(MOTION_Y, INPUT_AREA_SENSER))
			{
				KillTimer(nIDEvent);
				strLog.Format(_T("Area Senser 감지"));
				pdlg->m_dlgLogAlarm->SetMessage((LPSTR)(LPCSTR)strLog);
				pdlg->m_dlgLogAlarm->ShowWindow(SW_SHOW);

				pdlg->SetTowerLamp(OUTPUT_LAM_RED, TRUE);
				KillTimer(TIMER_MOTION_JOG);
			
				for(int i = 0; i < MAX_MOTION_CNT; i++)
				{
					SetMoveStop(i);
				}
			}
		}
		else
		{
			KillTimer(nIDEvent);
		}

		break;
	default:
		AfxMessageBox(_T("Error : invalid index(CDialogIOCtrl::OnTimer)!"));		
		break;
	}

	CDialogEx::OnTimer(nIDEvent);
}

void CDialogManual::MotionStateUpdate(int nMotionNum, int* isState)
{
	CString strPos;
	switch(nMotionNum)
	{
	case MOTION_X:
		strPos.Format(_T("%d"), isState[MOTION_STATE_CUR_POS]);
		m_btn_MotionX_CurPos.SetWindowTextA(strPos);

		if(isState[MOTION_STATE_AMP_FAULT_LEVEL])
		{
			m_btn_MotionX_Amp.SetWindowTextA(_T("Enable"));
			m_btn_MotionX_Amp.SetBackColor(BACK_COLOR_GREEN);
		}
		else
		{
			m_btn_MotionX_Amp.SetWindowTextA(_T("Disbled"));
			m_btn_MotionX_Amp.SetBackColor(BACK_COLOR_RED);
		}

		if(isState[MOTION_STATE_LIMT_SENSER_P])
		{
			m_btn_MotionX_P_Limt.SetBackColor(BACK_COLOR_GREEN);
		}
		else
		{
			m_btn_MotionX_P_Limt.SetBackColor(BACK_COLOR_RED);
		}

		if(isState[MOTION_STATE_LIMT_SENSER_N])
		{
			m_btn_MotionX_N_Limt.SetBackColor(BACK_COLOR_GREEN);
		}
		else
		{
			m_btn_MotionX_N_Limt.SetBackColor(BACK_COLOR_RED);
		}

		if(isState[MOTION_STATE_HOME])
		{
			m_btn_MotionX_Home.SetBackColor(BACK_COLOR_GREEN);
		}
		else
		{
			m_btn_MotionX_Home.SetBackColor(BACK_COLOR_RED);
		}

		if(m_ManualPara.ManualServoOrigin[MOTION_X])
		{
			m_btn_MotionX_OriginState.SetBackColor(BACK_COLOR_GREEN);
			m_btn_MotionX_OriginState.SetWindowTextA(_T("Origin Ok"));
		}
		else
		{
			m_btn_MotionX_OriginState.SetBackColor(BACK_COLOR_RED);
			m_btn_MotionX_OriginState.SetWindowTextA(_T("Origin Fail"));
		}
		break;
	case MOTION_Y:
		strPos.Format(_T("%d"), isState[MOTION_STATE_CUR_POS]);
		m_btn_MotionY_CurPos.SetWindowTextA(strPos);

		if(isState[MOTION_STATE_AMP_FAULT_LEVEL])
		{
			m_btn_MotionY_Amp.SetWindowTextA(_T("Enable"));
			m_btn_MotionY_Amp.SetBackColor(BACK_COLOR_GREEN);
		}
		else
		{
			m_btn_MotionY_Amp.SetWindowTextA(_T("Disbled"));
			m_btn_MotionY_Amp.SetBackColor(BACK_COLOR_RED);
		}

		if(isState[MOTION_STATE_LIMT_SENSER_P])
		{
			m_btn_MotionY_P_Limt.SetBackColor(BACK_COLOR_GREEN);
		}
		else
		{
			m_btn_MotionY_P_Limt.SetBackColor(BACK_COLOR_RED);
		}

		if(isState[MOTION_STATE_LIMT_SENSER_N])
		{
			m_btn_MotionY_N_Limt.SetBackColor(BACK_COLOR_GREEN);
		}
		else
		{
			m_btn_MotionY_N_Limt.SetBackColor(BACK_COLOR_RED);
		}

		if(isState[MOTION_STATE_HOME])
		{
			m_btn_MotionY_Home.SetBackColor(BACK_COLOR_GREEN);
		}
		else
		{
			m_btn_MotionY_Home.SetBackColor(BACK_COLOR_RED);
		}

		if(m_ManualPara.ManualServoOrigin[MOTION_Y])
		{
			m_btn_MotionY_OriginState.SetBackColor(BACK_COLOR_GREEN);
			m_btn_MotionY_OriginState.SetWindowTextA(_T("Origin Ok"));
		}
		else
		{
			m_btn_MotionY_OriginState.SetBackColor(BACK_COLOR_RED);
			m_btn_MotionY_OriginState.SetWindowTextA(_T("Origin Fail"));
		}
		break;
	case MOTION_Z:
		strPos.Format(_T("%d"), isState[MOTION_STATE_CUR_POS]);
		m_btn_MotionZ_CurPos.SetWindowTextA(strPos);
		if(isState[MOTION_STATE_AMP_FAULT_LEVEL])
		{
			m_btn_MotionZ_Amp.SetWindowTextA(_T("Enable"));
			m_btn_MotionZ_Amp.SetBackColor(BACK_COLOR_GREEN);
		}
		else
		{
			m_btn_MotionZ_Amp.SetWindowTextA(_T("Disabled"));
			m_btn_MotionZ_Amp.SetBackColor(BACK_COLOR_RED);
		}

		if(isState[MOTION_STATE_LIMT_SENSER_P])
		{
			m_btn_MotionZ_P_Limt.SetBackColor(BACK_COLOR_GREEN);
		}
		else
		{
			m_btn_MotionZ_P_Limt.SetBackColor(BACK_COLOR_RED);
		}

		if(isState[MOTION_STATE_LIMT_SENSER_N])
		{
			m_btn_MotionZ_N_Limt.SetBackColor(BACK_COLOR_GREEN);
			//m_btn_MotionZ_Home.SetBackColor(BACK_COLOR_GREEN);
		}
		else
		{
			m_btn_MotionZ_N_Limt.SetBackColor(BACK_COLOR_RED);
			//m_btn_MotionZ_Home.SetBackColor(BACK_COLOR_RED);
		}
		if(isState[MOTION_STATE_HOME])
		{
			m_btn_MotionZ_Home.SetBackColor(BACK_COLOR_GREEN);
		}
		else
		{
			m_btn_MotionZ_Home.SetBackColor(BACK_COLOR_RED);
		}

		if(m_ManualPara.ManualServoOrigin[MOTION_Z])
		{
			m_btn_MotionZ_OriginState.SetBackColor(BACK_COLOR_GREEN);
			m_btn_MotionZ_OriginState.SetWindowTextA(_T("Origin Ok"));
		}
		else
		{
			m_btn_MotionZ_OriginState.SetBackColor(BACK_COLOR_RED);
			m_btn_MotionZ_OriginState.SetWindowTextA(_T("Origin Fail"));
		}
		break;
	default:
		AfxMessageBox(_T("Error : MotionStateUpdate(), nMotionNum Fail!!"));
		break;
	}
}

BOOL CDialogManual::SaveManualPara(CString path)
{
	BOOL rslt = TRUE;
	CString key, temp_key, dat;
	CString pat_path, extention, master_path, strModPath;
	CString file_path;

	CString strLog;
	strLog.Format(_T("SaveManualPara Start !!"));
	WriteLog(strLog);

	file_path.Format(_T("%s%s"), path, MANUAL_PARA_FILE_NAME);

	HANDLE fd=CreateFile(   file_path,
							GENERIC_READ|GENERIC_WRITE,
							0,
							NULL,
							CREATE_ALWAYS,
							FILE_ATTRIBUTE_NORMAL,
							NULL
							);
	if(((LONG)fd)<0)
	{
		rslt = FALSE;
		strLog.Format(_T("SaveManualPara Fail !!"));
		WriteLog(strLog);
	}
	else
	{
		CloseHandle(fd);

		key.Empty();
		key.Format("COM_PORT_NUM");
		dat.Empty();
		dat.Format("%d", m_ManualPara.ManualComport);
		WritePrivateProfileString("MANUAL_PARA", key, dat, file_path);

		key.Empty();
		key.Format("BAUD_RATE");
		dat.Empty();
		dat.Format("%d", m_ManualPara.ManualBaudrate);
		WritePrivateProfileString("MANUAL_PARA", key, dat, file_path);

		for(int i = 0; i < MAX_MOTION_CNT; i++)
		{
			key.Empty();
			key.Format("SLAVE_NO_%d", i);
			dat.Empty();
			dat.Format("%d", m_ManualPara.ManualSlaveNo[i]);
			WritePrivateProfileString("MANUAL_PARA", key, dat, file_path);

			key.Empty();
			key.Format("PULSE_TO_MM_%d", i);
			dat.Empty();
			dat.Format("%d", m_ManualPara.ManualPulseToMM[i]);
			WritePrivateProfileString("MANUAL_PARA", key, dat, file_path);

			key.Empty();
			key.Format("SPEED_%d", i);
			dat.Empty();
			dat.Format("%d", m_ManualPara.ManualSpeed[i]);
			WritePrivateProfileString("MANUAL_PARA", key, dat, file_path);
		}
	}

	strLog.Format(_T("SaveManualPara End !!"));
	WriteLog(strLog);

	return rslt;
}

BOOL CDialogManual::LoadManualPara(CString path)
{
	BOOL rslt = TRUE;
	CString key, temp_key, dat;
	CString pat_path, extention, master_path;
	int idx = 0;
	CString file_path;
	char temp[DAT_STR_LENG];

	CString strLog;
	strLog.Format(_T("LoadManualPara Start !!"));
	WriteLog(strLog);

	file_path.Format(_T("%s%s"), path, MANUAL_PARA_FILE_NAME);

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
		strLog.Format(_T("LoadManualPara Fail !!"));
		WriteLog(strLog);
	}
	else
	{
		CloseHandle(fd);

		key.Empty();
		key.Format("COM_PORT_NUM");
		dat.Empty();
		GetPrivateProfileString("MANUAL_PARA", key, _T("1"), temp, 256, file_path);
		dat.Format("%s", temp);
		m_ManualPara.ManualComport = atoi(dat);

		key.Empty();
		key.Format("BAUD_RATE");
		dat.Empty();
		GetPrivateProfileString("MANUAL_PARA", key, _T("0"), temp, 256, file_path);
		dat.Format("%s", temp);
		m_ManualPara.ManualBaudrate = atoi(dat);

		for(int i = 0; i < MAX_MOTION_CNT; i++)
		{
			key.Empty();
			key.Format("SLAVE_NO_%d", i);
			dat.Empty();
			GetPrivateProfileString("MANUAL_PARA", key, _T("0"), temp, 256, file_path);
			dat.Format("%s", temp);
			m_ManualPara.ManualSlaveNo[i] = atoi(dat);

			key.Empty();
			key.Format("PULSE_TO_MM_%d", i);
			dat.Empty();
			GetPrivateProfileString("MANUAL_PARA", key, _T("1000"), temp, 256, file_path);
			dat.Format("%s", temp);
			m_ManualPara.ManualPulseToMM[i] = atoi(dat);

			key.Empty();
			key.Format("SPEED_%d", i);
			dat.Empty();
			GetPrivateProfileString("MANUAL_PARA", key, _T("10000"), temp, 256, file_path);
			dat.Format("%s", temp);
			m_ManualPara.ManualSpeed[i] = atoi(dat);
		}
	}

	strLog.Format(_T("LoadManualPara End !!"));
	WriteLog(strLog);

	return rslt;
}

BOOL CDialogManual::InitIO()
{
	int i = 0;
	m_InputAddr[i++] = INPUT_START_BUTTON_1;
	m_InputAddr[i++] = INPUT_START_BUTTON_2;
	m_InputAddr[i++] = INPUT_PAUSE_BUTTON;
	m_InputAddr[i++] = INPUT_SAFETY_LIGHT_CURTAIN_DETECT;
	m_InputAddr[i++] = INPUT_SLVE0_SPARE_7;
	m_InputAddr[i++] = INPUT_SLVE0_SPARE_7;
	m_InputAddr[i++] = INPUT_SLVE0_SPARE_7;
	m_InputAddr[i++] = INPUT_SLVE0_SPARE_7;
	m_InputAddr[i++] = INPUT_SLVE0_SPARE_7;

	i = 0;
	m_OutputAddr[i++] = OUTPUT_TOWER_LAMP_RED;
	m_OutputAddr[i++] = OUTPUT_TOWER_LAMP_YELLOW;
	m_OutputAddr[i++] = OUTPUT_TOWER_LAMP_GREEN;
	m_OutputAddr[i++] = OUTPUT_TOWER_LAMP_BUZZER;
	m_OutputAddr[i++] = OUTPUT_SLVE0_SPARE_4;
	m_OutputAddr[i++] = OUTPUT_SLVE0_SPARE_5;
	m_OutputAddr[i++] = OUTPUT_SLVE0_SPARE_6;
	m_OutputAddr[i++] = OUTPUT_SLVE0_SPARE_7;
	m_OutputAddr[i++] = OUTPUT_SLVE0_SPARE_8;

	return TRUE;
}

BOOL CDialogManual::InitMotion(int nMotionNum)
{
	CLED_DisplayInspectDlg *pdlg = ((CLED_DisplayInspectDlg *)AfxGetApp()->m_pMainWnd);
	int iRet = 0;
	CString strLog;
	strLog.Format(_T("InitMotion %d Num Start !!"), nMotionNum);
	WriteLog(strLog);
	pdlg->SetMotionAmp(nMotionNum, FALSE);
#ifdef MOTION
	if(nMotionNum == 0)
	{
		iRet = FAS_Connect((BYTE)m_ManualPara.ManualComport, (DWORD)m_ManualPara.ManualBaudrate);

		if(iRet != 1)
		{
			strLog.Format(_T("InitMotion Connect Fail !!"), nMotionNum);
			WriteLog(strLog);
			AfxMessageBox(strLog);
			m_btn_MotionConnect.SetWindowTextA(_T("CONNECT"));
			return FALSE;
		}	
		m_btn_MotionConnect.SetWindowTextA(_T("CLOSE"));
	}

	iRet = FAS_ServoEnable((BYTE)m_ManualPara.ManualComport, (BYTE)nMotionNum, TRUE);

	if(iRet != 0)
	{
		strLog.Format(_T("InitMotion %d Num Enable Fail !!"), nMotionNum);
		WriteLog(strLog);
		AfxMessageBox(strLog);
		return FALSE;
	}

	m_lCurPos[nMotionNum] = GetEZCurPos(nMotionNum); // (long)m_ManualPara.ManualPulseToMM[nMotionNum];
#endif

	if(!pdlg->GetMotionAmp(nMotionNum))
	{
		pdlg->SetMotionAmp(nMotionNum, TRUE);
	}

	strLog.Format(_T("InitMotion %d Num End !!"), nMotionNum);
	WriteLog(strLog);

	return TRUE;
}

long CDialogManual::GetEZCurPos(int nMotionNum)
{
	long lActPos = 0;
#ifdef MOTION
	FAS_GetActualPos((BYTE)m_ManualPara.ManualComport, (BYTE)m_ManualPara.ManualSlaveNo[nMotionNum], &lActPos);
#endif
	return lActPos;
}

BOOL CDialogManual::GetEzPosComplete(int nMotionNum, long nPos)
{
	long pos;
	BOOL rslt = FALSE;
	pos = GetEZCurPos(nMotionNum);
	//pos = (long)((double)(pos) / m_ManualPara.ManualPulseToMM[nMotionNum]);

	if((nPos - 20) <= pos && pos <= (nPos + 20))
	{
		rslt = TRUE;
	}


	//if(pos == nPos)//if(pos == m_ManualPara.ManualMovePos[nMotionNum][nMotionMoveIdx])
	//{
	//	rslt = TRUE;
	//}

	return rslt;
}

BOOL CDialogManual::SetJogMove(int nMotionNum, BOOL isDirection, long speed, CRect rect, long x, long y)
{
	CLED_DisplayInspectDlg *pdlg = ((CLED_DisplayInspectDlg *)AfxGetApp()->m_pMainWnd);
	BOOL rslt = FALSE;
	int iRet = 0;
	CString strLog;

#ifdef MOTION
	/*if(!m_ManualPara.ManualServoOrigin[nMotionNum])
	{
		strLog.Format(_T("Servo Motion Num %d Origion Fail, Jog Move Fail!!"), nMotionNum);
		WriteLog(strLog);
		AfxMessageBox(strLog);
		return rslt;
	}*/
#endif

	if(m_bAmpState)
	{
		if(!GetInput(MOTION_Y, INPUT_AREA_SENSER))
		{
			strLog.Format(_T("Area Senser 감지"));
			pdlg->m_dlgLogAlarm->SetMessage((LPSTR)(LPCSTR)strLog);
			pdlg->m_dlgLogAlarm->ShowWindow(SW_SHOW);

			pdlg->SetTowerLamp(OUTPUT_LAM_RED, TRUE);
			
			for(int i = 0; i < MAX_MOTION_CNT; i++)
			{
				SetMoveStop(i);
			}
			return FALSE;
		}
	}
	else
	{
		return FALSE;
	}


	if(((0 < x) && (x < rect.right)) && ((0 < y) && (y < rect.bottom)))
	{
#ifdef MOTION
		iRet = FAS_MoveVelocity((BYTE)m_ManualPara.ManualComport, (BYTE)m_ManualPara.ManualSlaveNo[nMotionNum], (DWORD)speed, isDirection);
#endif
		rslt = TRUE;
		if(iRet != 1)
		{
			rslt = FALSE;
		}	
	} 

	return rslt;
}

BOOL CDialogManual::SetStepMove(int nMotionNum, double dStep, long speed)
{
	CLED_DisplayInspectDlg *pdlg = ((CLED_DisplayInspectDlg *)AfxGetApp()->m_pMainWnd);
	CString strLog;
	int m_MMC_Err = 0;
	long pos = 0;
#ifdef MOTION
	/*if(!m_ManualPara.ManualServoOrigin[nMotionNum])
	{
		strLog.Format(_T("Servo Motion Num %d Origion Fail, Step Move Fail"), nMotionNum);
		WriteLog(strLog);
		return FALSE;
	}*/
#endif
	if(m_bAmpState)
	{
		if(!GetInput(MOTION_Y, INPUT_AREA_SENSER))
		{
			strLog.Format(_T("Area Senser 감지"));
			pdlg->m_dlgLogAlarm->SetMessage((LPSTR)(LPCSTR)strLog);
			pdlg->m_dlgLogAlarm->ShowWindow(SW_SHOW);

			pdlg->SetTowerLamp(OUTPUT_LAM_RED, TRUE);
			
			for(int i = 0; i < MAX_MOTION_CNT; i++)
			{
				SetMoveStop(i);
				return FALSE;
			}
		}
	}
	else
	{
		return FALSE;
	}
	pos = (long)(dStep);// * (double)m_ManualPara.ManualPulseToMM[nMotionNum]);
	m_nMovePos[nMotionNum] = pos + GetEZCurPos(nMotionNum);;
#ifdef MOTION
	m_MMC_Err = FAS_MoveSingleAxisIncPos((BYTE)m_ManualPara.ManualComport, (BYTE)m_ManualPara.ManualSlaveNo[nMotionNum], pos, speed);
#endif
	if (m_MMC_Err == FMM_OK)
	{
		return TRUE;
	}
	else
	{
		return FALSE;
	}
}

//20161017 ngh
BOOL CDialogManual::GetSWLimtPosRslt(int nMotionNum, int nMovePos)
{
	BOOL rslt = TRUE;

	if(nMotionNum == MOTION_Z)
	{
		if(MOTION_Z_SW_LIMT < nMovePos)
		{
			rslt = FALSE;
		}
	}
	return rslt;
}

BOOL CDialogManual::SetMotorOrigin(BOOL isAll, int nMotionNum)
{
	CLED_DisplayInspectDlg *pdlg = ((CLED_DisplayInspectDlg *)AfxGetApp()->m_pMainWnd);
	BOOL rslt = TRUE;
	CString strLog;
	if(isAll)
	{
		strLog.Format(_T("All Motion Origin Start"));
		pdlg->WriteLog(strLog, TYPE_LOG_MAIN);
		pdlg->m_dlgProcessBar->ShowWindow(SW_SHOW);
		for(int i = 0; i < MAX_MOTION_CNT; i++)
		{
			rslt = MotionOrigin(i);
			if(!rslt)
			{
				return FALSE;
			}
		}
	}
	else
	{
		rslt = MotionOrigin(nMotionNum);
	}

	return rslt;
}

BOOL CDialogManual::MotionOrigin(int nMotionNum)
{
	CString strLog;
	int iRet = 0;

	strLog.Format(_T("Motion Num %d Origin Start !!"), nMotionNum);
	WriteLog(strLog);
	m_ManualPara.ManualServoOrigin[nMotionNum] = FALSE;
#ifdef MOTION
	iRet = FAS_MoveOriginSingleAxis((BYTE)m_ManualPara.ManualComport, (BYTE)m_ManualPara.ManualSlaveNo[nMotionNum]);//FAS_AllMoveOriginSingleAxis((BYTE)m_ManualPara.ManualComport);

	if(iRet != 0)
	{
		strLog.Format(_T("Motion Num %d Origin Fail!!"), nMotionNum);
		WriteLog(strLog);
		return FALSE;
	}
#endif
	SetTimer(TIMER_MOTION_HOME + nMotionNum, 1000, NULL);

	return TRUE;
}

BOOL CDialogManual::SetEzSpeed(int nMotionNum, int nSpeed)
{
	CString strLog;
	if(nSpeed <= 0 || nSpeed <= 200000)
	{
		strLog.Format(_T("Servo Motion Num %d Min Speed 0, Max Speed 100000, Cur Speed Fail !!"), nMotionNum, nSpeed);
		return FALSE;
	}

	strLog.Format(_T("Servo Motion Num %d Speed Set %d!!"), nMotionNum, nSpeed);
	WriteLog(strLog);

	m_ManualPara.ManualSpeed[nMotionNum] = nSpeed;
	return TRUE;
}

BOOL CDialogManual::SetEzPosMove(int nMotionNum, int nPos)
{
	CLED_DisplayInspectDlg *pdlg = ((CLED_DisplayInspectDlg *)AfxGetApp()->m_pMainWnd);
	CString strLog;
	int iRet = 0;
#ifdef MOTION
	if(!m_ManualPara.ManualServoOrigin[nMotionNum])
	{
		strLog.Format(_T("Servo Motion Num %d Origion Fail, Pos Move Fail"), nMotionNum);
		WriteLog(strLog);
		AfxMessageBox(_T(strLog));
		return FALSE;
	}
#endif

	long pos = 0;
	pos = (long)nPos;//(long)((double)nPos * (double)m_ManualPara.ManualPulseToMM[nMotionNum]);

#ifdef MOTION
	iRet = FAS_MoveSingleAxisAbsPos((BYTE)m_ManualPara.ManualComport, (BYTE)m_ManualPara.ManualSlaveNo[nMotionNum], pos,
									(DWORD)m_ManualPara.ManualSpeed[nMotionNum]);	

	if (iRet != FMM_OK)
	{
		strLog.Format(_T("Error happended : %d"), iRet);
		WriteLog(strLog);
		return FALSE;
	}
#endif

	strLog.Format(_T("Servo Motion Num %d Move Pos %d"), nMotionNum, pos);
	WriteLog(strLog);

	m_nMovePos[nMotionNum] = pos;
	if(!pdlg->GetInspectionStart())
	{
		SetTimer(TIMER_MOTION_MOVE_COMP + nMotionNum, 100, NULL);
	}

	return TRUE;
}

void CDialogManual::CloseMotion()
{
	CString strLog;
#ifdef MOTION
	for(int i = 0; i < MAX_MOTION_CNT; i++)
	{
		FAS_ServoEnable((BYTE)m_ManualPara.ManualComport, (BYTE)i, FALSE);
	}
	FAS_Close(m_ManualPara.ManualComport);
#endif
	m_ManualPara.ManualServoOrigin[MOTION_X] = FALSE;
	m_ManualPara.ManualServoOrigin[MOTION_Y] = FALSE;
	m_ManualPara.ManualServoOrigin[MOTION_Z] = FALSE;
	strLog.Format(_T("Servo Motion Num Close"));
	WriteLog(strLog);
}

int CDialogManual::GetServoAmp(int nMotionNum)
{
	DWORD dwAxisStatus=0;
	BOOL rslt = -1;
	int MMC_Err;
	EZISERVO_AXISSTATUS state;
		
	MMC_Err = FAS_GetAxisStatus((BYTE)m_ManualPara.ManualComport, (BYTE)m_ManualPara.ManualSlaveNo[nMotionNum], &dwAxisStatus);

	state.dwValue = dwAxisStatus;

	if(MMC_Err == FMM_OK)
	{
		if(state.FFLAG_SERVOON == 1) 
		{
			rslt = TRUE;
		}
		else 
		{
			rslt = FALSE;
		}
	}

	return rslt;
}

BOOL CDialogManual::GetLimtSenser(int nMotionNum, BOOL isDirection)
{
	BOOL rslt = FALSE;
	int MMC_Err;
	DWORD dwInput=0;
	
	m_AxisSyncIntlock.Lock();
	MMC_Err = FAS_GetIOInput((BYTE)m_ManualPara.ManualComport, (BYTE)m_ManualPara.ManualSlaveNo[nMotionNum], &dwInput);
	m_AxisSyncIntlock.Unlock();
	if(MMC_Err == FMM_OK)
	{
		if(isDirection)
		{
			if(dwInput & SERVO_IN_BITMASK_LIMITP)
			{
				rslt = TRUE;
			}
			else
			{
				rslt = FALSE;
			}
		}
		else
		{
			if(dwInput & SERVO_IN_BITMASK_LIMITN)
			{
				rslt = TRUE;
			}
			else
			{
				rslt = FALSE;
			}
		}
	}

	return rslt;
}

BOOL CDialogManual::GetHomeSenser(int nMotionNum)
{
	BOOL rslt = FALSE;
	int MMC_Err;
	DWORD dwInput=0;
		
	m_AxisSyncIntlock.Lock();
	MMC_Err = FAS_GetIOInput((BYTE)m_ManualPara.ManualComport, (BYTE)m_ManualPara.ManualSlaveNo[nMotionNum], &dwInput);
	//z축은 -limt 정보 가져오기
	m_AxisSyncIntlock.Unlock();
	if(MMC_Err == FMM_OK)
	{
		if(dwInput & SERVO_IN_BITMASK_ORIGIN)
			rslt =  TRUE;
		else
			rslt =  FALSE;
	}
	
	return rslt;
}

BOOL CDialogManual::GetAmpFaultLevel(int nMotionNum, long& level)
{
	BOOL rslt = FALSE;
	int MMC_Err;
	BYTE PinNo; 

	BYTE Level = FALSE;
		
	PinNo = 0; //초기0번 출력을 Alarm으로 한다. 
	level = FALSE;
		
	DWORD LogicNo;
	m_AxisSyncIntlock.Lock();
	MMC_Err = FAS_GetIOAssignMap((BYTE)m_ManualPara.ManualComport, (BYTE)m_ManualPara.ManualSlaveNo[nMotionNum], 12 + PinNo, &LogicNo,&Level);
	m_AxisSyncIntlock.Unlock();

	if(MMC_Err == FMM_OK)
	{
		level = Level;
		rslt = TRUE;
	}
	
	return rslt;
}

BOOL CDialogManual::SetMoveStop(int nMotionNum)
{
	int iRet = TRUE;
	CString strLog;
#ifdef MOTION
	iRet = FAS_MoveStop((BYTE)m_ManualPara.ManualComport, (BYTE)m_ManualPara.ManualSlaveNo[nMotionNum]);
#endif
	strLog.Format(_T("Servo Motion Num %d Stop"), nMotionNum);
	WriteLog(strLog);
	return iRet;
}

void CDialogManual::Update(BOOL isUpdate)
{
	if(!isUpdate)
	{
		m_nComportNum_0 = m_ManualPara.ManualComport;
		m_nBaudrate_0 = m_ManualPara.ManualBaudrate;
		m_edit_nSlaveNo_0 = m_ManualPara.ManualSlaveNo[MOTION_X];
		m_edit_dPulseToMM_0 = m_ManualPara.ManualPulseToMM[MOTION_X];
		m_edit_nSpeed_0 = m_ManualPara.ManualSpeed[MOTION_X];

		m_edit_nSlaveNo_1 = m_ManualPara.ManualSlaveNo[MOTION_Y];
		m_edit_dPulseToMM_1 = m_ManualPara.ManualPulseToMM[MOTION_Y];
		m_edit_nSpeed_1 = m_ManualPara.ManualSpeed[MOTION_Y];

		m_edit_nSlaveNo_2 = m_ManualPara.ManualSlaveNo[MOTION_Z];
		m_edit_dPulseToMM_2 = m_ManualPara.ManualPulseToMM[MOTION_Z];
		m_edit_nSpeed_2 = m_ManualPara.ManualSpeed[MOTION_Z];
	}
	else
	{
		m_ManualPara.ManualComport = m_nComportNum_0;
		m_ManualPara.ManualBaudrate = m_nBaudrate_0 = GetIdxBaudrate(m_combo_nBaudrate_0.GetCurSel());
		m_ManualPara.ManualSlaveNo[MOTION_X] = m_edit_nSlaveNo_0;
		m_ManualPara.ManualPulseToMM[MOTION_X] = m_edit_dPulseToMM_0;
		m_ManualPara.ManualSpeed[MOTION_X] = m_edit_nSpeed_0;

		m_ManualPara.ManualSlaveNo[MOTION_Y] = m_edit_nSlaveNo_1;
		m_ManualPara.ManualPulseToMM[MOTION_Y] = m_edit_dPulseToMM_1;
		m_ManualPara.ManualSpeed[MOTION_Y] = m_edit_nSpeed_1;

		m_ManualPara.ManualSlaveNo[MOTION_Z] = m_edit_nSlaveNo_2;
		m_ManualPara.ManualPulseToMM[MOTION_Z] = m_edit_dPulseToMM_2;
		m_ManualPara.ManualSpeed[MOTION_Z] = m_edit_nSpeed_2;
	}
}

BOOL CDialogManual::WriteLog(CString strLog)
{
	CLED_DisplayInspectDlg *pdlg = ((CLED_DisplayInspectDlg *)AfxGetApp()->m_pMainWnd);
	BOOL rslt = FALSE;
	rslt = pdlg->WriteLog(strLog, TYPE_LOG_MANUAL);

	return rslt;
}

void CDialogManual::InitCombo()
{
	//COM5;COM6;COM7;COM8;COM9;
	CString strTemp;

	for(int j = 0; j < MAX_COMPORT_NUM; j++)
	{
		strTemp.Format(_T("COM_%d"), j);
		m_combo_nComportNum_0.InsertString(j, strTemp);
	}

	SetComboComPort(m_ManualPara.ManualComport);

	for(int j = 0; j < MAX_BAUD_RATE_NUM; j++)
	{
		strTemp.Format(_T("%d"), g_pBaudRate[j]);
		m_combo_nBaudrate_0.InsertString(j, strTemp);
	}

	SetComboBaudrate(m_ManualPara.ManualBaudrate);
}

void CDialogManual::SetComboComPort(int nIdx)
{
	if(nIdx < MOTION_DEFAULT_COMPORT)
	{
		nIdx = 0;
	}

	m_combo_nComportNum_0.SetCurSel(nIdx);
	m_nComportNum_0 = nIdx;
}

void CDialogManual::SetComboBaudrate(int nBaudrate)
{
	int nIdx = 0;
	for(int i = 0; i < MAX_BAUD_RATE_NUM; i++)
	{
		if(nBaudrate == g_pBaudRate[i].nBaudRate)
		{
			break;
		}
		nIdx++;
	}

	if(nIdx == MAX_BAUD_RATE_NUM)
	{
		nIdx = 0;
	}

	m_combo_nBaudrate_0.SetCurSel(nIdx);
	m_nBaudrate_0 = nBaudrate;
}

int CDialogManual::GetIdxBaudrate(int nIdx)
{
	int rslt = 0;
	switch(nIdx)
	{
	case COMBO_BAUD_RATE_0:
		rslt = BAUD_RATE_0;
		break;
	case COMBO_BAUD_RATE_1:
		rslt = BAUD_RATE_1;
		break;
	case COMBO_BAUD_RATE_2:
		rslt = BAUD_RATE_2;
		break;
	case COMBO_BAUD_RATE_3:
		rslt = BAUD_RATE_3;
		break;
	case COMBO_BAUD_RATE_4:
		rslt = BAUD_RATE_4;
		break;
	case COMBO_BAUD_RATE_5:
		rslt = BAUD_RATE_5;
		break;
	case COMBO_BAUD_RATE_6:
		rslt = BAUD_RATE_6;
		break;
	case COMBO_BAUD_RATE_7:
		rslt = BAUD_RATE_7;
		break;
	default:

		break;
	}

	return rslt;
}

void CDialogManual::OnCbnSelchangeComboComAxis0()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	m_nComportNum_0 = m_combo_nComportNum_0.GetCurSel();
}

void CDialogManual::OnCbnSelchangeComboBaudrateAxis0()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	m_nBaudrate_0 = GetIdxBaudrate(m_combo_nBaudrate_0.GetCurSel());
}

typeManualPara CDialogManual::GetManualPara()
{
	return m_ManualPara;
}

void CDialogManual::ClickBtnenhctrlServoOrigin0()
{
	// TODO: 여기에 메시지 처리기 코드를 추가합니다.
	MotionOrigin(MOTION_X);
}

void CDialogManual::ClickBtnenhctrlServoOrigin1()
{
	// TODO: 여기에 메시지 처리기 코드를 추가합니다.
	MotionOrigin(MOTION_Y);
}

void CDialogManual::ClickBtnenhctrlServoOrigin2()
{
	// TODO: 여기에 메시지 처리기 코드를 추가합니다.
	MotionOrigin(MOTION_Z);
}

BOOL CDialogManual::MotionAmp(int nMotionNum, BOOL isAmp)
{
	CString strLog;
#ifndef NOTE_BOOK
	int iRet;
#endif
#ifdef MOTION
	iRet = FAS_ServoEnable((BYTE)m_ManualPara.ManualComport, (BYTE)m_ManualPara.ManualSlaveNo[nMotionNum], isAmp);

	if(iRet != 1)
	{
		strLog.Format(_T("Motion %d Num Enable Fail !!"), nMotionNum);
		WriteLog(strLog);
		AfxMessageBox(strLog);
		return FALSE;
	}
#endif
	return TRUE;
}

BOOL CDialogManual::SetMotorAmp(BOOL isAll, int nMotionNum, BOOL isAmp)
{
	BOOL rslt = TRUE;
	if(isAll)
	{
		for(int i = 0; i < MAX_MOTION_CNT; i++)
		{
			rslt = MotionAmp(i, isAmp);
			if(!rslt)
			{
				return FALSE;
			}
		}
	}
	else
	{
		rslt = MotionAmp(nMotionNum, isAmp);
	}

	return rslt;
}

BOOL CDialogManual::MotionAlarmReset(int nMotionNum)
{
	int iRet = 0;
	CString strLog;
	BYTE alarm = 0;
	
#ifdef MOTION

	iRet = FAS_GetAlarmType((BYTE)m_ManualPara.ManualComport, (BYTE)m_ManualPara.ManualSlaveNo[nMotionNum], &alarm);
#endif
	if(iRet != 1)
	{
		strLog.Format(_T("Motion %d Num Get Alarm Type Error!!"), nMotionNum);
		WriteLog(strLog);
		AfxMessageBox(strLog);
		return FALSE;
	}

	if(alarm == 0)
	{
		return TRUE;
	}

#ifdef MOTION
	iRet = FAS_ServoAlarmReset((BYTE)m_ManualPara.ManualComport, (BYTE)m_ManualPara.ManualSlaveNo[nMotionNum]);
#endif
	if(iRet != 1)
	{
		strLog.Format(_T("Motion %d Num Alarm Reset Fail!!"), nMotionNum);
		WriteLog(strLog);
		AfxMessageBox(strLog);
		return FALSE;
	}

	return TRUE;
}

BOOL CDialogManual::SetMotorAlarmReset(BOOL isAll, int nMotionNum)
{
	BOOL rslt = TRUE;
	if(isAll)
	{
		for(int i = 0; i < MAX_MOTION_CNT; i++)
		{
			rslt = MotionAlarmReset(i);
			if(!rslt)
			{
				return FALSE;
			}
		}
	}
	else
	{
		rslt = MotionAlarmReset(nMotionNum);
	}

	return rslt;
}


void CDialogManual::ClickBtnenhctrlServoAmp0()
{
	// TODO: 여기에 메시지 처리기 코드를 추가합니다.
	CString strAmp;
	BOOL bAmp;
	m_btn_MotionX_Amp.GetWindowTextA(strAmp);
	if(!strAmp.Compare("Enable"))
	{
		bAmp = FALSE;
	}
	else
	{
		bAmp = TRUE;
	}
	MotionAmp(MOTION_X, bAmp);
}


void CDialogManual::ClickBtnenhctrlServoAmp1()
{
	// TODO: 여기에 메시지 처리기 코드를 추가합니다.
	CString strAmp;
	BOOL bAmp;
	m_btn_MotionY_Amp.GetWindowTextA(strAmp);
	if(!strAmp.Compare("Enable"))
	{
		bAmp = FALSE;
	}
	else
	{
		bAmp = TRUE;
	}
	MotionAmp(MOTION_Y, bAmp);
}


void CDialogManual::ClickBtnenhctrlServoAmp2()
{
	// TODO: 여기에 메시지 처리기 코드를 추가합니다.
	CString strAmp;
	BOOL bAmp;
	m_btn_MotionZ_Amp.GetWindowTextA(strAmp);
	if(!strAmp.Compare("Enable"))
	{
		bAmp = FALSE;
	}
	else
	{
		bAmp = TRUE;
	}
	MotionAmp(MOTION_Z, bAmp);
}


void CDialogManual::ClickBtnenhctrlMotorStop()
{
	// TODO: 여기에 메시지 처리기 코드를 추가합니다.
	KillTimer(TIMER_MOTION_HOME + MOTION_X);
	KillTimer(TIMER_MOTION_HOME + MOTION_Y);
	KillTimer(TIMER_MOTION_HOME + MOTION_Z);
	FAS_AllMoveStop((BYTE)m_ManualPara.ManualComport);
}

void CDialogManual::ClickBtnenhctrlMotorAlarmReset()
{
	// TODO: 여기에 메시지 처리기 코드를 추가합니다.
	BOOL bRslt;

	bRslt = SetMotorAlarmReset();
	AfxMessageBox(_T("Alarm Reset Comp"));
}


void CDialogManual::ClickBtnenhctrlMotionConnect()
{
	// TODO: 여기에 메시지 처리기 코드를 추가합니다.
	CString strTemp, strLog;

	m_btn_MotionConnect.GetWindowTextA(strTemp);

	if(!strTemp.Compare(_T("CONNECT")))
	{
		m_btn_MotionConnect.SetWindowTextA(_T("CLOSE"));
		for(int i = 0; i < MAX_MOTION_CNT; i++)
		{
			strLog.Format(_T("Motion Num %d Init Comp !!"), i);

			if(!InitMotion(i))
			{
				strLog.Format(_T("Motion Num %d Init Fail !!"), i);
			}
		
			WriteLog(strLog);
		}
	}
	else if(!strTemp.Compare(_T("CLOSE")))
	{
		m_btn_MotionConnect.SetWindowTextA(_T("CONNECT"));
		CloseMotion();
	}
}


BOOL CDialogManual::PreTranslateMessage(MSG* pMsg)
{
	// TODO: 여기에 특수화된 코드를 추가 및/또는 기본 클래스를 호출합니다.
	if(pMsg->message ==WM_KEYDOWN)
	{
		if(pMsg->wParam == VK_RETURN || pMsg->wParam == VK_ESCAPE)
			return TRUE;
	}

	return CDialogEx::PreTranslateMessage(pMsg);
}

//20161011 ht Manual다이얼로그 언어 설정
void CDialogManual::ChangeLanguage(BOOL StateLanguage)
{

	switch(StateLanguage)
	
	{
	case 0:
		//Axis Monitor
		GetDlgItem(IDC_BTNENHCTRL_AXIX_NAME)->SetWindowText(_T("Axis Name"));
		GetDlgItem(IDC_BTNENHCTRL_AXIX_NAME2)->SetWindowText(_T("Stage X"));
		GetDlgItem(IDC_BTNENHCTRL_AXIX_NAME3)->SetWindowText(_T("Stage Y"));
		GetDlgItem(IDC_BTNENHCTRL_AXIX_NAME4)->SetWindowText(_T("Stage Z"));
		GetDlgItem(IDC_BTNENHCTRL_AXIX_NAME5)->SetWindowText(_T("Current Pos"));
		GetDlgItem(IDC_BTNENHCTRL_SERVO_ORIGIN_12)->SetWindowText(_T("Current Pos"));
		GetDlgItem(IDC_BTNENHCTRL_SERVO_ORIGIN_17)->SetWindowText(_T("Current Pos"));
		GetDlgItem(IDC_BTNENHCTRL_SERVO_ORIGIN_22)->SetWindowText(_T("Current Pos"));
		GetDlgItem(IDC_BTNENHCTRL_AXIX_NAME6)->SetWindowText(_T("P Limit"));
		GetDlgItem(IDC_BTNENHCTRL_AXIX_NAME7)->SetWindowText(_T("N Limit"));
		GetDlgItem(IDC_BTNENHCTRL_AXIX_NAME8)->SetWindowText(_T("Home"));
		GetDlgItem(IDC_BTNENHCTRL_AXIX_NAME9)->SetWindowText(_T("Amp State"));

		GetDlgItem(IDC_BTNENHCTRL_SERVO_AMP_0)->SetWindowText(_T("Amp State"));
		GetDlgItem(IDC_BTNENHCTRL_SERVO_AMP_1)->SetWindowText(_T("Amp State"));
		GetDlgItem(IDC_BTNENHCTRL_SERVO_AMP_2)->SetWindowText(_T("Amp State"));
		
		GetDlgItem(IDC_BTNENHCTRL_AXIX_NAME10)->SetWindowText(_T("Origin State"));

		GetDlgItem(IDC_BTNENHCTRL_SERVO_ORIGIN_STATE_1)->SetWindowText(_T("Origin TRUE")); //-----> 이부분을 한영으로 표시 하려면 상태를 표시해주는 함수에서 변경후 다시 표시하는 방법으로 해야할것 같음.
		GetDlgItem(IDC_BTNENHCTRL_SERVO_ORIGIN_STATE_2)->SetWindowText(_T("Origin TRUE"));
		GetDlgItem(IDC_BTNENHCTRL_SERVO_ORIGIN_STATE_3)->SetWindowText(_T("Origin TRUE"));
		
		//Axis Connect
		GetDlgItem(IDC_BTNENHCTRL_AXIX_NAME11)->SetWindowText(_T("Comport"));
		GetDlgItem(IDC_BTNENHCTRL_AXIX_NAME12)->SetWindowText(_T("Baudrate"));
		GetDlgItem(IDC_BTNENHCTRL_MOTION_CONNECT)->SetWindowText(_T("Connect"));
		
		//Axis Parameter
		GetDlgItem(IDC_BTNENHCTRL_AXIX_NAME13)->SetWindowText(_T("Slave No"));
		GetDlgItem(IDC_BTNENHCTRL_AXIX_NAME15)->SetWindowText(_T("Speed"));
		GetDlgItem(IDC_BTNENHCTRL_AXIX_NAME16)->SetWindowText(_T("Stage X"));
		GetDlgItem(IDC_BTNENHCTRL_AXIX_NAME17)->SetWindowText(_T("Stage Y"));
		GetDlgItem(IDC_BTNENHCTRL_AXIX_NAME18)->SetWindowText(_T("Stage Z"));
		
		//I/O
		GetDlgItem(IDC_BTNENHCTRL_AXIX_NAME19)->SetWindowText(_T("Input"));
		GetDlgItem(IDC_BTNENHCTRL_AXIX_NAME20)->SetWindowText(_T("Output"));

		GetDlgItem(IDC_BTNENHCTRL_APPLY)->SetWindowText(_T("Apply"));
		GetDlgItem(IDC_BTNENHCTRL_OK)->SetWindowText(_T("Save"));
		GetDlgItem(IDC_BTNENHCTRL_CANCEL)->SetWindowText(_T("Cancel"));
		break;
	case 1:
		//Axis Monitor
		GetDlgItem(IDC_BTNENHCTRL_AXIX_NAME)->SetWindowText(_T("축 명칭"));
		GetDlgItem(IDC_BTNENHCTRL_AXIX_NAME2)->SetWindowText(_T("X축"));
		GetDlgItem(IDC_BTNENHCTRL_AXIX_NAME3)->SetWindowText(_T("Y축"));
		GetDlgItem(IDC_BTNENHCTRL_AXIX_NAME4)->SetWindowText(_T("Z축"));
		GetDlgItem(IDC_BTNENHCTRL_AXIX_NAME5)->SetWindowText(_T("현재 위치"));
		GetDlgItem(IDC_BTNENHCTRL_SERVO_ORIGIN_12)->SetWindowText(_T("Current Pos"));
		GetDlgItem(IDC_BTNENHCTRL_SERVO_ORIGIN_17)->SetWindowText(_T("Current Pos"));
		GetDlgItem(IDC_BTNENHCTRL_SERVO_ORIGIN_22)->SetWindowText(_T("Current Pos"));
		GetDlgItem(IDC_BTNENHCTRL_AXIX_NAME6)->SetWindowText(_T("P Limit"));
		GetDlgItem(IDC_BTNENHCTRL_AXIX_NAME7)->SetWindowText(_T("N Limit"));
		GetDlgItem(IDC_BTNENHCTRL_AXIX_NAME8)->SetWindowText(_T("원점"));
		GetDlgItem(IDC_BTNENHCTRL_AXIX_NAME9)->SetWindowText(_T("앰프 상태"));

		GetDlgItem(IDC_BTNENHCTRL_SERVO_AMP_0)->SetWindowText(_T("Amp State"));
		GetDlgItem(IDC_BTNENHCTRL_SERVO_AMP_1)->SetWindowText(_T("Amp State"));
		GetDlgItem(IDC_BTNENHCTRL_SERVO_AMP_2)->SetWindowText(_T("Amp State"));
		
		GetDlgItem(IDC_BTNENHCTRL_AXIX_NAME10)->SetWindowText(_T("원점 상태"));
		
		GetDlgItem(IDC_BTNENHCTRL_SERVO_ORIGIN_STATE_1)->SetWindowText(_T("Origin TRUE"));
		GetDlgItem(IDC_BTNENHCTRL_SERVO_ORIGIN_STATE_2)->SetWindowText(_T("Origin TRUE"));
		GetDlgItem(IDC_BTNENHCTRL_SERVO_ORIGIN_STATE_3)->SetWindowText(_T("Origin TRUE"));
		
		//Axis Connect
		GetDlgItem(IDC_BTNENHCTRL_AXIX_NAME11)->SetWindowText(_T("통신 포트"));
		GetDlgItem(IDC_BTNENHCTRL_AXIX_NAME12)->SetWindowText(_T("변조 속도"));
		GetDlgItem(IDC_BTNENHCTRL_MOTION_CONNECT)->SetWindowText(_T("연결"));
		
		//Axis Parameter
		GetDlgItem(IDC_BTNENHCTRL_AXIX_NAME13)->SetWindowText(_T("슬레이브 번호"));
		GetDlgItem(IDC_BTNENHCTRL_AXIX_NAME15)->SetWindowText(_T("속도"));
		GetDlgItem(IDC_BTNENHCTRL_AXIX_NAME16)->SetWindowText(_T("Stage X"));
		GetDlgItem(IDC_BTNENHCTRL_AXIX_NAME17)->SetWindowText(_T("Stage Y"));
		GetDlgItem(IDC_BTNENHCTRL_AXIX_NAME18)->SetWindowText(_T("Stage Z"));
		
		//I/O
		GetDlgItem(IDC_BTNENHCTRL_AXIX_NAME19)->SetWindowText(_T("입력"));
		GetDlgItem(IDC_BTNENHCTRL_AXIX_NAME20)->SetWindowText(_T("출력"));

		GetDlgItem(IDC_BTNENHCTRL_APPLY)->SetWindowText(_T("적용"));
		GetDlgItem(IDC_BTNENHCTRL_OK)->SetWindowText(_T("저장"));
		GetDlgItem(IDC_BTNENHCTRL_CANCEL)->SetWindowText(_T("Cancel"));
		break;
	}

}

UINT CDialogManual::AmpStateThread(LPVOID pParam)
{
	CDialogManual *pdlg = (CDialogManual *)pParam;

	while(TRUE)
	{
		if(pdlg->m_bAmpStateStart)
		{
			//pdlg->m_bAmpStateStart = FALSE;
			pdlg->AmpState();
		}

		if(pdlg->m_bAmpStateEnd)
		{
			break;
		}
		Sleep(1);
	}

	return 0;
}


void CDialogManual::AmpState()
{
	CLED_DisplayInspectDlg *pdlg = ((CLED_DisplayInspectDlg *)AfxGetApp()->m_pMainWnd);
	int amp_stat = 0;
	for(int i = 0; i < MAX_MOTION_CNT; i++)
	{
		if(pdlg->GetMotionAmp(MOTION_X) && pdlg->GetMotionAmp(MOTION_Y) && pdlg->GetMotionAmp(MOTION_Z))
		{
			m_bAmpState = TRUE;
			amp_stat = GetServoAmp(i);
			if(amp_stat == -1)
			{
#ifdef MOTION
				m_bAmpState = FALSE;
				pdlg->SetMotionAmp(MOTION_X, FALSE);
				pdlg->SetMotionAmp(MOTION_Y, FALSE);
				pdlg->SetMotionAmp(MOTION_Z, FALSE);
				break;
#endif
			}
		}
	}
}