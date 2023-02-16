
// LED_DisplayInspectDlg.cpp : 구현 파일
//
#include "stdafx.h"
#include "afxwin.h"
#include "LED_DisplayInspect.h"
#include "LED_DisplayInspectDlg.h"
#include "afxdialogex.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

//메모리 릭 체크 start
#ifdef _DEBUG
#define new DEBUG_NEW

#undef THIS_FILE
static char THIS_FILE[] = __FILE__;

#endif
//메모리 릭 체크 end

#define KERNEL_WIDTH     19L
#define KERNEL_HEIGHT    19L
#define KERNEL_DEPTH     8L
unsigned char  KernelData[KERNEL_HEIGHT][KERNEL_WIDTH] ={
				{1, 1, 1, 1, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 1, 1, 1, 1},
				{1, 1, 1, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 1, 1, 1},
				{1, 1, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 1, 1},
				{1, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 1},
				{2, 2, 2, 2, 2, 2, 2, 2, 2, 4, 2, 2, 2, 2, 2, 2, 2, 2, 2},
				{2, 2, 2, 2, 2, 2, 2, 2, 4, 4, 4, 2, 2, 2, 2, 2, 2, 2, 2},
				{2, 2, 2, 2, 2, 2, 2, 4, 4, 4, 4, 4, 2, 2, 2, 2, 2, 2, 2},
				{2, 2, 2, 2, 2, 2, 4, 4, 4, 4, 4, 4, 4, 2, 2, 2, 2, 2, 2},
				{2, 2, 2, 2, 2, 4, 4, 4, 4, 4, 4, 4, 4, 4, 2, 2, 2, 2, 2},
				{2, 2, 2, 2, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 2, 2, 2, 2},
				{2, 2, 2, 2, 2, 4, 4, 4, 4, 4, 4, 4, 4, 4, 2, 2, 2, 2, 2},
				{2, 2, 2, 2, 2, 2, 4, 4, 4, 4, 4, 4, 4, 2, 2, 2, 2, 2, 2},
				{2, 2, 2, 2, 2, 2, 2, 4, 4, 4, 4, 4, 2, 2, 2, 2, 2, 2, 2},
				{2, 2, 2, 2, 2, 2, 2, 2, 4, 4, 4, 2, 2, 2, 2, 2, 2, 2, 2},
				{2, 2, 2, 2, 2, 2, 2, 2, 2, 4, 2, 2, 2, 2, 2, 2, 2, 2, 2},
				{1, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 1},
				{1, 1, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 1, 1},
				{1, 1, 1, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 1, 1, 1},
				{1, 1, 1, 1, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 1, 1, 1, 1},
               };

DWORD _TimerCounter;

//Time
UINT _TimerID;
TIMECAPS _Caps;
BOOL _TimerPost;
void CALLBACK TimerReadFunc(UINT wID,UINT wUser,DWORD dwUser,DWORD dw1,DWORD dw2);

static void CALLBACK TimerReadFunc(UINT wID,UINT wUser,DWORD dwUser,DWORD dw1,DWORD dw2)
{
	if(_TimerPost==FALSE)
	{
		_TimerCounter++;
	}
	else
	{
		AfxMessageBox(_T("Error : high resolution timer event loss!"));
	}	
}


//////////////////////////////////////////////////////////////////////
//Motion Data
//////////////////////////////////////////////////////////////////////
CMotionData::CMotionData()
{
	for(int i = 0; i < MOT_STATUS_MAX_NUM; i++)
	{
		m_ProSts[i] = new typeMotionProcessSts[MOT_STATUS_SUB_MAX_NUM];
		memset(m_ProSts[i], NULL, sizeof(typeMotionProcessSts) * MOT_STATUS_SUB_MAX_NUM);
	}

	memset(&m_Seq, NULL, sizeof(typeMotionSeq) * MOT_STATUS_MAX_NUM);
}

CMotionData::~CMotionData()
{
	for(int i = 0; i < MOT_STATUS_MAX_NUM; i++)
	{
		delete m_ProSts[i];
	}
}

void CMotionData::ResetSeq()
{
	memset(&m_Seq, NULL, sizeof(typeMotionSeq) * MOT_STATUS_MAX_NUM);
}

//file search
BOOL CLED_DisplayInspectDlg::FileSearch(CString FilePath, CString FileName)
{
	CFileFind ff;
	CString strTarget = FilePath;
	CStringArray strFileArray;
	BOOL rslt = FALSE;

	if(strTarget.Right(1) != '\\')
	{
		strTarget += '\\';
	}

	strTarget += "*.*";
	if(ff.FindFile(strTarget)==TRUE)
	{
		CString Filename;
		BOOL bFlag = true;
		while(bFlag == TRUE)
		{
			bFlag = ff.FindNextFile();
			Filename = ff.GetFilePath();
			if(ff.IsDots())
			{
				continue;
			}
			if(ff.IsDirectory())
			{
				continue;
			}
			if(ff.GetFileName() == FileName)
			{
				rslt = TRUE;
				break;
			}
		}
	}
	ff.Close();

	return rslt;
}

//Make directories
void _MakeDirectories(CString path)
{
	char tmp_path[MAX_PATH];
	char sub[MAX_PATH];
	long size;
	size = sizeof(path);

	long index = 0;

	wsprintf(tmp_path, _T("%s"), (LPCSTR)path);
	memset(sub, NULL, sizeof(sub));

	while(1)
	{
		if(tmp_path[index] == '\\')
		{
			CreateDirectory(sub, NULL);
		}

		sub[index] = tmp_path[index];

		if(tmp_path[index] == '\0')
		{
			CreateDirectory(sub, NULL);
			break;
		}

		//base_index++;
		index++;

		if( (index > MAX_PATH) )
		{
			AfxMessageBox(_T("Error : index overflow(_MakeDirectories)!"));
			break;
		}

	}
}


// 응용 프로그램 정보에 사용되는 CAboutDlg 대화 상자입니다.

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

	// 대화 상자 데이터입니다.
	enum { IDD = IDD_ABOUTBOX };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	// 구현입니다.
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialogEx(CAboutDlg::IDD)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
END_MESSAGE_MAP()


// CLED_DisplayInspectDlg 대화 상자

CLED_DisplayInspectDlg::CLED_DisplayInspectDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CLED_DisplayInspectDlg::IDD, pParent)
	, m_edit_nRepeatMaxCnt(1)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);

	memset(&m_RsltCnt, NULL, sizeof(typeInsCntPara));
	memset(&m_SelectModel, NULL, sizeof(typedefModelPara));
	memset(&m_RsltPara, NULL, sizeof(typeInsRsltPara));
	memset(&m_BeforeRsltPara, NULL, sizeof(typeInsRsltPara));
	memset(&m_InsRsltPara[0], NULL, sizeof(typeInsRsltPara));
	memset(&m_InsRsltPara[1], NULL, sizeof(typeInsRsltPara));

	m_bInspThreadStart = FALSE;
	m_bInspThreadEnd = FALSE;
	m_bAutoDeleteStart = FALSE;
	m_bAutoDeleteEnd = FALSE;
	m_bDelete = FALSE;
	m_iDel_Year = 0;
	m_iDel_Month = 0;
	m_iDel_Day = 0;
	m_iDel_RecentDay = 0;

	m_iFocus_Precision = 0; //20160906 sc

	m_nRepeatCnt = 0;
	m_bRepeat = FALSE;
	m_bInspectionOKNG = TRUE;
	m_bMouseWheel = FALSE;
	for(int i = 0; i < MAX_MOTION_CNT; i++)
	{
		m_bMotionAmp[i] = FALSE;
	}
#ifdef BASLER
	m_BaslerManage = new CBaslerManage(CAMERA_WIDTH,CAMERA_HEIGHT);
#endif
	InitializeCriticalSection(&m_CSInline);
}

void CLED_DisplayInspectDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_SCROLLBAR_NG_DISP, m_scrol_NgDisp);
	DDX_Control(pDX, IDC_BTNENHCTRL_LIVE, m_btnLive);
	DDX_Control(pDX, IDC_MAIN_PRODUCT_TOTAL, m_btn_Total);
	DDX_Control(pDX, IDC_MAIN_PRODUCT_PASS, m_btn_Pass);
	DDX_Control(pDX, IDC_MAIN_PRODUCT_FAIL, m_btn_Fail);
	DDX_Control(pDX, IDC_MAIN_PRODUCT_FAIL_RATIO, m_btn_FailRat);
	DDX_Control(pDX, IDC_DATE_LABEL, m_btn_TimerData);
	DDX_Control(pDX, IDC_STATIC_LOGO, m_static_Logo);
	DDX_Control(pDX, IDC_WORK_MODEL_NAME_LABEL, m_btn_SelectModelName);
	DDX_Control(pDX, IDC_MAINVIEW_BTN, m_btn_Auto);
	DDX_Control(pDX, IDB_MAIN_OP_START, m_btn_Start);
	DDX_Control(pDX, IDB_MAIN_OP_STOP, m_btn_End);
	DDX_Control(pDX, IDC_FOCUS_PRECISION, m_btn_focus_precision);
	DDX_Control(pDX, IDC_LIST_MAIN_LOG, m_list_MainLog);
	DDX_Control(pDX, IDC_BRIGHT, m_btn_Bright);
	DDX_Text(pDX, IDC_EDIT_REPEAT_CNT, m_edit_nRepeatMaxCnt);
	DDX_Control(pDX, IDC_MAIN_GRAB_TIME, m_btn_GrabTime);
	DDX_Control(pDX, IDC_MAIN_INSP_TIME, m_btn_InspTime);
	DDX_Control(pDX, IDC_MAIN_OP_STATE, m_btn_main_op_state);
	DDX_Control(pDX, IDC_MANUALMVIEW_BTN, m_btn_Manual);
	DDX_Control(pDX, IDC_TEACHVIEW_BTN, m_btn_Teach);
	DDX_Control(pDX, IDC_DATAVIEW_BTN, m_btn_Model);
	DDX_Control(pDX, IDB_MAIN_INSPECTION_RESET, m_btn_InspReset);
	DDX_Control(pDX, IDB_MAIN_OP_INIT, m_btn_UnitInit);
}

BEGIN_MESSAGE_MAP(CLED_DisplayInspectDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_BUTTON_TEST, &CLED_DisplayInspectDlg::OnBnClickedButtonTest)
	ON_WM_VSCROLL()
	ON_WM_TIMER()
	ON_MESSAGE(USER_MSG_MOT, OnMotMessage)
	ON_WM_MOUSEWHEEL()
	ON_BN_CLICKED(IDC_BUTTON_IMG_TEST, &CLED_DisplayInspectDlg::OnBnClickedButtonImgTest)
	ON_BN_CLICKED(IDC_BUTTON_REGRID, &CLED_DisplayInspectDlg::OnBnClickedButtonRegrid)
END_MESSAGE_MAP()


// CLED_DisplayInspectDlg 메시지 처리기

BOOL CLED_DisplayInspectDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// 시스템 메뉴에 "정보..." 메뉴 항목을 추가합니다.

	// IDM_ABOUTBOX는 시스템 명령 범위에 있어야 합니다.
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		BOOL bNameValid;
		CString strAboutMenu;
		bNameValid = strAboutMenu.LoadString(IDS_ABOUTBOX);
		ASSERT(bNameValid);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// 이 대화 상자의 아이콘을 설정합니다. 응용 프로그램의 주 창이 대화 상자가 아닐 경우에는
	//  프레임워크가 이 작업을 자동으로 수행합니다.
	SetIcon(m_hIcon, TRUE);			// 큰 아이콘을 설정합니다.
	SetIcon(m_hIcon, FALSE);		// 작은 아이콘을 설정합니다.


	//20161014 ht 작업표시줄에서 프로그램 이름 보이게끔 해주는 부분
	SetWindowText("LED_DisplayInspect"); 
	ModifyStyle(0, WS_SYSMENU,0); // WS_SYSMENU 스타일을 추가한다. 


	// TODO: 여기에 추가 초기화 작업을 추가합니다.
	m_bInspThreadTest = FALSE;
	if(m_bInspThreadTest)
	{
		srand((unsigned int)(time(NULL))); 
	}
	m_bInspectionImageSave = FALSE;
#ifdef CAMERA
#ifdef BASLER
#ifdef _DEBUG

	//20161019 ht 카메라 상태에 따라 프로그램이 종료될수 있게끔 수정
	switch(m_BaslerManage->InitCamera(1200000))
	{
	case SUCCESS_CAMERA:
		break;
	case ERROR_CAMERA_NOT_OPEN:
		AfxMessageBox(_T("카메라 연결 실패!\n프로그램을 종료합니다."));
		exit(0);
		break;
	case ERROR_CAMERA_AVAILABLE:
		AfxMessageBox(_T("카메라 연결 실패!\n프로그램을 종료합니다."));
		exit(0);
		break;
	}
#else
	switch(m_BaslerManage->InitCamera(3000))
	{
	case SUCCESS_CAMERA:
		break;
	case ERROR_CAMERA_NOT_OPEN:
		AfxMessageBox(_T("카메라 연결 실패!\n프로그램을 종료합니다."));
		exit(0);
		break;
	case ERROR_CAMERA_AVAILABLE:
		AfxMessageBox(_T("카메라 연결 실패!\n프로그램을 종료합니다."));
		exit(0);
		break;
	}
#endif
#endif
#endif

	m_nBeforeRsltFileCnt = 0;
	MoveWindow(0, 0, GetSystemMetrics(SM_CXSCREEN), GetSystemMetrics(SM_CYSCREEN) - 40); //화면 최대 해상도로 맞추기
	initProductInfoCnt();

	HWND _hwnd[MAX_DISP];
	CRect rect, rect1, rect2, rect3, rect4;
	double ratio[2], ratiow;
	CString temp;

	m_dlgProcessBar = new CDialogProcessBar();
	m_dlgProcessBar->Create(IDD_DIALOG_PROCESS_BAR, this);


	m_dlgData = new CDialogData();
	m_dlgData->Create(IDD_DIALOG_DATA, this);
	GetDlgItem(IDC_MAIN_FRM_DISP)->GetWindowRect(rect);
	GetDlgItem(IDC_STATIC_GRID)->GetWindowRect(rect3);
	GetWindowRect(rect1);
	GetDlgItem(IDC_BTNENHCTRL4)->GetWindowRect(rect2);
	GetDlgItem(IDC_LIST_MAIN_LOG)->GetWindowRect(rect4);
	m_dlgData->MoveWindow(0, rect3.top, rect1.Width() - (rect2.Width() + rect4.Width()), rect.bottom - rect3.top, TRUE);

	m_dlgManual = new CDialogManual();
	m_dlgManual->Create(IDD_DIALOG_MANUAL, this);
	GetDlgItem(IDC_MAIN_FRM_DISP)->GetWindowRect(rect);
	GetDlgItem(IDC_STATIC_GRID)->GetWindowRect(rect3);
	GetWindowRect(rect1);
	GetDlgItem(IDC_BTNENHCTRL4)->GetWindowRect(rect2);
	m_dlgManual->MoveWindow(0, rect3.top, rect1.Width() - rect2.Width(), rect.bottom - rect3.top, TRUE);


	m_dlgTeach = new CDialogTeach();
	m_dlgTeach->Create(IDD_DIALOG_TEACH, this);
	GetDlgItem(IDC_BTNENHCTRL4)->GetWindowRect(rect);
	GetDlgItem(IDC_MAIN_FRM_DISP)->GetWindowRect(rect1);
	m_dlgTeach->MoveWindow(rect.left - rect.Width(), 0, rect.Width() * 2, rect1.bottom, TRUE);

	m_dlgImageRect = new CDialogImageRect();
	m_dlgImageRect->Create(IDD_DIALOG_IMAGE_RECT, this);
	GetDlgItem(IDC_MAIN_FRM_DISP)->GetWindowRect(rect);
	m_dlgImageRect->MoveWindow(rect.left, rect.top + 60, rect.Width(), rect.Height() - 60, TRUE);

	m_dlgNgView = new CDialogNgView();
	m_dlgNgView->Create(IDD_DIALOG_NG_VIEW, this);
	m_dlgNgView->GetWindowRect(rect);
	GetDlgItem(IDC_LIST_MAIN_LOG)->GetWindowRect(rect1);
	GetDlgItem(IDC_BTNENHCTRL_LOGO)->GetWindowRect(rect2);
	m_dlgNgView->MoveWindow(0, rect2.bottom, rect.Width() - 10, rect.Height() - 7, TRUE);

	m_dlgLogAlarm = new CDialogLogAlarm();
	m_dlgLogAlarm->Create(IDD_DIALOG_LOG_ALARM, this);


	GetDlgItem(IDC_MAIN_FRM_DISP)->GetWindowRect(rect);
	ratiow = (double)rect.Height() / CAMERA_HEIGHT;
	ratio[0] = (double)rect.Width() / CAMERA_WIDTH;
	m_dlgImageRect->m_dZoomDefault = ratio[0];
	m_dZoomRatio = ratio[0];

	_hwnd[0] = m_dlgImageRect->GetDispHwnd();
	_hwnd[1] = GetDlgItem(IDC_STATIC_NG_IMAGE_0)->m_hWnd;
	_hwnd[2] = GetDlgItem(IDC_STATIC_NG_IMAGE_1)->m_hWnd;
	_hwnd[3] = GetDlgItem(IDC_STATIC_NG_IMAGE_2)->m_hWnd;
	_hwnd[4] = m_dlgNgView->GetDispHwnd();
	//20161010 ngh
	////////////////////////////////////////////
	HWND t_hwnd[3];
	m_dlgNgView->GetNgDispHwnd(t_hwnd);
	//t_hwnd = m_dlgNgView->GetNgDispHwnd();	
	////////////////////////////////////////////
	_hwnd[5] = t_hwnd[0];
	_hwnd[6] = t_hwnd[1];
	_hwnd[7] = t_hwnd[2];
	ratio[1] = m_dlgNgView->GetZoomRatio();
#ifdef CAMERA
	m_MilManage.Open(_hwnd, ratio);
#endif
	m_dlgImageRect->SetZoom(ratio[0], CAMERA_WIDTH, CAMERA_HEIGHT);
	rect.SetRect(0, 0, (int)(CAMERA_WIDTH * ratio[0]), (int)(CAMERA_HEIGHT * ratio[0]));
	m_dlgImageRect->SetDlgRect(rect);
	m_dlgImageRect->InitTracker();
	m_dlgImageRect->ShowWindow(SW_SHOW);

	SCROLLINFO scinfo;
	scinfo.cbSize = sizeof(scinfo);
	scinfo.fMask = SIF_ALL;
	scinfo.nMin = 0;
	scinfo.nMax = 0;
	scinfo.nPage = 1;
	scinfo.nTrackPos = 0;
	scinfo.nPos = 0;
	m_scrol_NgDisp.SetScrollInfo(&scinfo);



	//high resolution timer for IO reading & PLC init
	//(timer)
	_TimerPost=FALSE;
	timeGetDevCaps(&_Caps,sizeof(_Caps));
	_Caps.wPeriodMin = 1;
	timeBeginPeriod(_Caps.wPeriodMin);
	_TimerID = timeSetEvent(_Caps.wPeriodMin, _Caps.wPeriodMin, (LPTIMECALLBACK)&TimerReadFunc, 0, (UINT)TIME_PERIODIC);
	if(!_TimerID)
	{
		AfxMessageBox(_T("Error : high resolution timer set fail!"));
	}

	m_Flag.m_bProcessThread = TRUE;
	for(int i = 0; i < MOT_PROCESS_MAX_NUM; i++)
	{
		for(int j = 0; j < MOT_STATUS_MAX_NUM; j++)
		{
			m_MDat[i].m_Seq[j].m_bAutoStart = FALSE;
		}
	}
	//Thread
	m_pMotionThread = AfxBeginThread(MotionThread, this, THREAD_PRIORITY_NORMAL);
	if (m_pMotionThread) 
		m_pMotionThread->m_bAutoDelete = TRUE;

	m_bMotionProcessStart = FALSE;
	m_bMotionProcessEnd = FALSE;

	m_pAutoDeleteThread = AfxBeginThread(AutoDeleteThread, this, THREAD_PRIORITY_NORMAL);
	if(m_pAutoDeleteThread)
		m_pAutoDeleteThread->m_bAutoDelete = TRUE;

	m_bAutoDeleteStart = TRUE;

	m_pInspThread = AfxBeginThread(InspThread, this, THREAD_PRIORITY_NORMAL);
	if (m_pInspThread) 
		m_pInspThread->m_bAutoDelete = TRUE;

	CString rslt_path;
	rslt_path.Format(_T("%s%s"), RESULT_PATH, GRAB_IMAGE_SAVE_PATH);
	_MakeDirectories(rslt_path);
	rslt_path.Format(_T("%s%s"), RESULT_PATH, OK_IMAGE_SAVE_PATH);
	_MakeDirectories(rslt_path);
	rslt_path.Format(_T("%s%s"), RESULT_PATH, NG_IMAGE_SAVE_PATH);
	_MakeDirectories(rslt_path);

	m_btn_Start.EnableWindow(TRUE);
	m_btn_End.EnableWindow(FALSE);

	m_list_MainLog.DeleteAllItems();

	// 리스트 스타일 설정
	m_list_MainLog.SetExtendedStyle(LVS_EX_GRIDLINES);
	m_list_MainLog.InsertColumn(0, _T("Log"), LVCFMT_LEFT, 800, -1);

	InitGrid();

	m_Rslt.m_bInspectionStart = FALSE;

	ClickMainInspectionReset();
	//m_Flag.m_bAutoStart = FALSE;

	m_nViewMode = AUTO_WINDOW;

	m_btnLive.SetValue(FALSE); //20161006 sc
	GetDlgItem(IDB_MAIN_MOTION_STOP)->EnableWindow(FALSE);

	ChangeLanguage(m_dlgData->GetCurrentLanguage()); //20161011 ht 어떤언어가 저장되어있는지 확인후 선택해서 변경시켜줌 

	//SetTimer(TIMER_MOTION_AMP, 100, NULL);
	SetTimer(TIMER_TIME_SHOW, 1000, NULL);
	SetTimer(TIMER_ALL_ORIGIN_START, 100, NULL);
	SetTowerLamp(OUTPUT_LAM_YELLOW);

	return TRUE;  // 포커스를 컨트롤에 설정하지 않으면 TRUE를 반환합니다.
}

void CLED_DisplayInspectDlg::SetMotionAmp(int nMotionIdx, BOOL isAmp)
{
	m_bMotionAmp[nMotionIdx] = isAmp;
}

BOOL CLED_DisplayInspectDlg::GetMotionAmp(int nMotionIdx)
{
	return m_bMotionAmp[nMotionIdx];
}

void CLED_DisplayInspectDlg::InitGrid()
{
	m_grid.AttachGrid(this, IDC_STATIC_GRID, m_dlgTeach->GetTeachPara().TeachDeviceCntX, m_dlgTeach->GetTeachPara().TeachDeviceCntY, m_dlgTeach->GetTeachPara().TeachGrabCnt);
	m_grid.RedrawAll();
}

void CLED_DisplayInspectDlg::DeleteGrid()
{
	for(int i = 0 ; i < m_grid.m_GI->m_numberCols; i++)
	{
		for(int j = 0; j < (m_grid.m_GI->m_numberRows); j++)
		{
			m_grid.DeleteCell(i, j);
		}
	}
}

void CLED_DisplayInspectDlg::SetGrid()
{
	DeleteGrid();
	m_grid.RedrawAll();

	int cnt = 0;
	if(m_dlgTeach->GetTeachPara().TeachArrayCntY % m_dlgTeach->GetTeachPara().TeachOneGrabInspCnt != 0.)
	{
		cnt = (m_dlgTeach->GetTeachPara().TeachArrayCntY % m_dlgTeach->GetTeachPara().TeachOneGrabInspCnt);
	}

	//m_grid.SetGrid_LED(m_dlgTeach->GetTeachPara().TeachDeviceCntX, m_dlgTeach->GetTeachPara().TeachDeviceCntY * m_dlgTeach->GetTeachPara().TeachOneGrabInspCnt, m_dlgTeach->GetTeachPara().TeachGrabCnt,m_dlgTeach->GetTeachPara().TeachInspGrabCnt);
	//20161221 수식때문에 변경한 부분입니다. 넘겨주는 인자값을 함수에 하나 더 추가 하였습니다 (1번촬상당 촬영하는 제품의수)
	m_grid.SetGrid_LED(m_dlgTeach->GetTeachPara().TeachDeviceCntX, m_dlgTeach->GetTeachPara().TeachDeviceCntY, m_dlgTeach->GetTeachPara().TeachGrabCnt,m_dlgTeach->GetTeachPara().TeachInspGrabCnt);
	m_grid.RedrawAll();

	//20161017 ngh
	for(int i = 0; i < (m_dlgTeach->GetTeachPara().TeachArrayCntY + cnt); i++)
	{
		for(int j = 0; j < m_dlgTeach->GetTeachPara().TeachDeviceCntX; j++)
		{
			for(int k = 0; k < m_dlgTeach->GetTeachPara().TeachGrabCnt; k++)
			{
				if(i < m_dlgTeach->GetTeachPara().TeachArrayCntY)
				{
					GridDrow(TRUE, j, i, k);
				}
				else
				{
					m_grid.SetGridOkNg_LED(5, (j * m_dlgTeach->GetTeachPara().TeachGrabCnt) + k, i, k);
				}
			}
		}
	}
	m_grid.RedrawAll();
}

void CLED_DisplayInspectDlg::SetGrid(int isOK, int nDeviceIdxX, int nDeviceIdxY, int nGrabIdx)
{
	GridDrow(isOK, nDeviceIdxX, nDeviceIdxY, nGrabIdx);
}

void CLED_DisplayInspectDlg::GridDrow(int isOK, int nDeviceIdxX, int nDeviceIdxY, int nGrabIdx)
{
	if(isOK == 5)
	{
		m_grid.SetGridOkNg_LED(5, (nDeviceIdxX * m_dlgTeach->GetTeachPara().TeachGrabCnt) + nGrabIdx, nDeviceIdxY, nGrabIdx);
	}
	else if(isOK == 6)
	{
		m_grid.SetGridOkNg_LED(6, (nDeviceIdxX * m_dlgTeach->GetTeachPara().TeachGrabCnt) + nGrabIdx, nDeviceIdxY, nGrabIdx);
	}
	else
	{
		m_grid.SetGridOkNg_LED(isOK, (nDeviceIdxX * m_dlgTeach->GetTeachPara().TeachGrabCnt) + nGrabIdx, nDeviceIdxY, nGrabIdx);
	}
	
	//20161213 nht Debug로그기록과 Grid그리는 방식 수정
	m_grid.RedrawCell((nDeviceIdxX * m_dlgTeach->GetTeachPara().TeachGrabCnt) + nGrabIdx, nDeviceIdxY);
	CString strLog;
	strLog.Format(_T("Grid Num (X : %d   /   Y : %d), X : %d   /   Y : %d"),m_grid.m_nDeviceX[(nDeviceIdxX * m_dlgTeach->GetTeachPara().TeachGrabCnt) + nGrabIdx],
		m_grid.m_nDeviceY[nDeviceIdxY], (nDeviceIdxX * m_dlgTeach->GetTeachPara().TeachGrabCnt) + nGrabIdx,nDeviceIdxY);
	WriteLog(strLog,TYPE_LOG_DEBUG,FALSE);
	//m_grid.RedrawCell((nDeviceIdxX * m_dlgTeach->GetTeachPara().TeachGrabCnt) + nGrabIdx, nDeviceIdxY);


	//m_grid.RedrawAll();
}

//20170104
void CLED_DisplayInspectDlg::SetGridDbClick(int col, int row, int grab_idx)
{
	int d_x, d_y, g_i, ig_i;
	int idx = 2, g_idx;

	d_x = col;
	d_y = row;
	g_i = grab_idx;
	ig_i = 0;
	if(m_dlgTeach->GetTeachPara().TeachRegionCnt[0][0] > 1)
	{
		g_idx = row % m_dlgTeach->GetTeachPara().TeachRegionCnt[0][0];
		if(g_idx == 0)
		{
			idx = (1 * 3) + 2;
		}
		else
		{
			idx = (0 * 3) + 2;
		}
	}

	if((m_dlgNgView->IsWindowVisible() || m_bBeforeView) && !m_dlgNgView->m_dlgVerify->IsWindowVisible())
	{
		m_dlgNgView->SetRsltPara(d_x - 1, d_y - 1, g_i - 1, ig_i, m_BeforeRsltPara);
		m_dlgNgView->SetGridDbClick(d_x - 1, d_y - 1, g_i - 1);

		for(int k = 0; k < 5; k++)
		{
			MgraRect(M_DEFAULT, *m_MilManage.GetMilOverlay(NG_VIEW_DISP), 
				m_dlgTeach->GetTeachPara().TeachRegion[0][grab_idx - 1][idx].GetLeft() - k,
				m_dlgTeach->GetTeachPara().TeachRegion[0][grab_idx - 1][idx].GetTop() - k,
				m_dlgTeach->GetTeachPara().TeachRegion[0][grab_idx - 1][idx].GetRight() + k,
				m_dlgTeach->GetTeachPara().TeachRegion[0][grab_idx - 1][idx].GetBottom() + k);
		}
	}
	else if((m_dlgNgView->IsWindowVisible() || m_bBeforeView) && m_dlgNgView->m_dlgVerify->IsWindowVisible())
	{
		m_dlgNgView->SetRsltPara(m_BeforeRsltPara.nDeviceIdx[0].x, m_BeforeRsltPara.nDeviceIdx[0].y, m_BeforeRsltPara.nGrabIdx[0], ig_i, m_BeforeRsltPara);
		m_dlgNgView->SetGridDbClick(m_BeforeRsltPara.nDeviceIdx[0].x, m_BeforeRsltPara.nDeviceIdx[0].y, m_BeforeRsltPara.nGrabIdx[0]);
	}

}

void CLED_DisplayInspectDlg::VerifyRsltCal()
{
	vector<CString> temp;
	CString temp1, temp2;
	int idx = 0, step = 0;

	for(int i = 0; i < m_BeforeRsltPara.nNgCnt; i++)
	{
		if(m_BeforeRsltPara.bNg[i])
		{
			temp2.Format(_T("%d,%d,%d,%d,%f,%f,%f,%f,%f,%f"), m_BeforeRsltPara.nDeviceIdx[i].x, m_BeforeRsltPara.nDeviceIdx[i].y, m_BeforeRsltPara.nGrabIdx[i], m_BeforeRsltPara.nInspGrabIdx[i],
				m_BeforeRsltPara.dCentX[i], m_BeforeRsltPara.dCentY[i], m_BeforeRsltPara.dWidth[i], m_BeforeRsltPara.dHeight[i], m_BeforeRsltPara.dLong[i], m_BeforeRsltPara.dShort[i]);
			temp.push_back(temp2);
			idx++;
		}
	}
	InitBeforeRsltPara();
	for(int i = 0; idx; i++)
	{
		if(temp.size() == i)
		{
			break;
		}
		step = 0;
		AfxExtractSubString(temp1, temp[i], step++, ',');
		m_BeforeRsltPara.nDeviceIdx[i].x = atoi(temp1);
		AfxExtractSubString(temp1, temp[i], step++, ',');
		m_BeforeRsltPara.nDeviceIdx[i].y = atoi(temp1);
		AfxExtractSubString(temp1, temp[i], step++, ',');
		m_BeforeRsltPara.nGrabIdx[i] = atoi(temp1);
		AfxExtractSubString(temp1, temp[i], step++, ',');
		m_BeforeRsltPara.nInspGrabIdx[i] = atoi(temp1);
		AfxExtractSubString(temp1, temp[i], step++, ',');
		m_BeforeRsltPara.dCentX[i] = atof(temp1);
		AfxExtractSubString(temp1, temp[i], step++, ',');
		m_BeforeRsltPara.dCentY[i] = atof(temp1);
		AfxExtractSubString(temp1, temp[i], step++, ',');
		m_BeforeRsltPara.dWidth[i] = atof(temp1);
		AfxExtractSubString(temp1, temp[i], step++, ',');
		m_BeforeRsltPara.dHeight[i] = atof(temp1);
		AfxExtractSubString(temp1, temp[i], step++, ',');
		m_BeforeRsltPara.dLong[i] = atof(temp1);
		AfxExtractSubString(temp1, temp[i], step++, ',');
		m_BeforeRsltPara.dShort[i] = atof(temp1);
		m_BeforeRsltPara.bNg[i] = TRUE;
		m_BeforeRsltPara.nNgCnt++;
	}
}

void CLED_DisplayInspectDlg::SetVerifyRslt(BOOL isNg, int nDeviceIdxX, int nDeviceIdxY, int nGrabIdx, int file_idx)
{
	int cnt = 0;
	int col = 0, row = 0;
	BOOL search = FALSE;
	if(!isNg)
	{
		for(int i = 0; i < m_BeforeRsltPara.nNgCnt; i++)
		{
			if((m_BeforeRsltPara.nDeviceIdx[i].x == nDeviceIdxX) && (m_BeforeRsltPara.nDeviceIdx[i].y == nDeviceIdxY))
			{
				if((m_BeforeRsltPara.nGrabIdx[i] == nGrabIdx))
				{
					m_BeforeRsltPara.bNg[i] = FALSE;
					search = TRUE;
					cnt++;
				}
			}
		}

		CString data, file_path, path, name, temp;
		BOOL rslt;

		if(search)
		{
			VerifyRsltCal();

			file_path.Format(_T("%s"), m_cBeforeRsltFilePath[file_idx]);

			path = _T("");
			name = _T("");

			for(int i = 0; i < 6; i++)
			{
				AfxExtractSubString(temp, file_path, i, '\\');
				if(!path.IsEmpty())
				{
					path = path + _T("\\") + temp;
				}
				else
				{
					path+=temp;
				}
			}

			_DeleteFile(path, file_path);

			data.Format(_T("Array_X,Array_Y,GrabIdx,InspIdx,Cent_X,Cent_Y,Width,Height,Logn,Short,Motion_Z"));
			rslt = SaveRsltFile(file_path, data);
			if(!rslt)
			{
				m_RsltIdx = 0;
				return;
			}
			for(int i = 0; i < m_BeforeRsltPara.nNgCnt; i++)
			{
				data.Format(_T("%d,%d,%d,%d,%f,%f,%f,%f,%f,%f,%d"), m_BeforeRsltPara.nDeviceIdx[i].x, m_BeforeRsltPara.nDeviceIdx[i].y, m_BeforeRsltPara.nGrabIdx[i], m_BeforeRsltPara.nInspGrabIdx[i],
					m_BeforeRsltPara.dCentX[i], m_BeforeRsltPara.dCentY[i], m_BeforeRsltPara.dWidth[i], m_BeforeRsltPara.dHeight[i], m_BeforeRsltPara.dLong[i], m_BeforeRsltPara.dShort[i],m_dlgManual->GetEZCurPos(2));
				rslt = SaveRsltFile(file_path, data);
				if(!rslt)
				{
					break;
				}
			}

			BOOL isNg = FALSE;
			for(int i = 0; i < m_BeforeRsltPara.nNgCnt; i++)
			{
				if((m_BeforeRsltPara.nDeviceIdx[i].x == nDeviceIdxX) && (m_BeforeRsltPara.nDeviceIdx[i].y == nDeviceIdxY))
				{
					isNg = TRUE;
					break;
				}
			}

			if(!isNg)
			{
				m_RsltCnt.nNgCnt--;
				m_RsltCnt.nOk1Cnt++;
				SetCapa(isNg, 1);
				SetAllCapa(isNg, 1);
			}

			m_dlgNgView->SetRsltPara(m_BeforeRsltPara.nDeviceIdx[0].x, m_BeforeRsltPara.nDeviceIdx[0].y, m_BeforeRsltPara.nGrabIdx[0], 0, m_BeforeRsltPara);
			if(m_BeforeRsltPara.nNgCnt > 0)
			{
				m_dlgNgView->SetGridDbClick(m_BeforeRsltPara.nDeviceIdx[0].x, m_BeforeRsltPara.nDeviceIdx[0].y, m_BeforeRsltPara.nGrabIdx[0]);
			}
		}
	}
	else
	{
		m_BeforeRsltPara.nDeviceIdx[m_BeforeRsltPara.nNgCnt].SetPoint(nDeviceIdxX, nDeviceIdxY);
		m_BeforeRsltPara.nGrabIdx[m_BeforeRsltPara.nNgCnt] = nGrabIdx;
		m_BeforeRsltPara.bNg[m_BeforeRsltPara.nNgCnt] = TRUE;
		m_BeforeRsltPara.bAddNg[m_BeforeRsltPara.nNgCnt] = TRUE;
		m_BeforeRsltPara.nNgCnt++;

		m_RsltCnt.nNgCnt++;
		m_RsltCnt.nOk1Cnt--;
		SetCapa(TRUE, 1);
		SetAllCapa(TRUE, 1);
	}

	ProductInfoView();
	m_RsltIdx = 0;
}


void CLED_DisplayInspectDlg::SetSelectGrid(int type, int nDeviceIdxX, int nDeviceIdxY, int nGrabIdx)
{
	m_grid.SetGridOkNg_LED(type + 3, (nDeviceIdxX * m_dlgTeach->GetTeachPara().TeachGrabCnt) + nGrabIdx, nDeviceIdxY, nGrabIdx);
	m_grid.RedrawAll();
}

void CLED_DisplayInspectDlg::NGAllDisplay()
{
	NGDisplay(m_poDeviceIdx[m_Rslt.m_bInspectionComp].x, m_poDeviceIdx[m_Rslt.m_bInspectionComp].y, m_nGrabIdx[m_Rslt.m_bInspectionComp], m_nInspGrabIdx[m_Rslt.m_bInspectionComp], MAIN_DISP, m_RsltPara);
}

void CLED_DisplayInspectDlg::NGView(int nDeviceIdxX, int nDeviceIdxY, int nGrabIdx, int nInspGrabIdx)
{

}

typeInsRsltPara CLED_DisplayInspectDlg::GetBeforeRslt()
{
	return m_BeforeRsltPara;
}

void CLED_DisplayInspectDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialogEx::OnSysCommand(nID, lParam);
	}
}

// 대화 상자에 최소화 단추를 추가할 경우 아이콘을 그리려면
//  아래 코드가 필요합니다. 문서/뷰 모델을 사용하는 MFC 응용 프로그램의 경우에는
//  프레임워크에서 이 작업을 자동으로 수행합니다.

void CLED_DisplayInspectDlg::OnPaint()
{
	CRect rect1;
	GetDlgItem(IDC_BTNENHCTRL_LOGO)->GetWindowRect(rect1);
	//m_static_Logo.MoveWindow(rect.left, rect.top, rect.Width(), rect.Height(), TRUE);

	CDC memDc;
	memDc.CreateCompatibleDC(m_static_Logo.GetDC());
	CBitmap bitmap;
	bitmap.LoadBitmapA(IDB_BITMAP_LOGO1);
	memDc.SelectObject(&bitmap);

	m_static_Logo.GetDC()->StretchBlt(0,0,rect1.Width(), rect1.Height(),&memDc, 0,0,177,46,SRCCOPY);

	if (IsIconic())
	{
		CPaintDC dc(this); // 그리기를 위한 디바이스 컨텍스트입니다.

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// 클라이언트 사각형에서 아이콘을 가운데에 맞춥니다.
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// 아이콘을 그립니다.
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{

		CDialogEx::OnPaint();
	}
}

// 사용자가 최소화된 창을 끄는 동안에 커서가 표시되도록 시스템에서
//  이 함수를 호출합니다.
HCURSOR CLED_DisplayInspectDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

void CLED_DisplayInspectDlg::OnBnClickedButtonTest()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	m_bRepeat = TRUE;
	if(m_btn_End.GetValue())
	{
		m_nRepeatCnt = 0;
	}
	UpdateData(TRUE);
	/*
	// 가상 키보드 사용 예제

	CGetTextDlg dlg;
	CString strTemp;

	if(dlg.DoModal() != IDOK)
	return;

	strTemp = dlg.GetStringValue();

	AfxMessageBox(strTemp);
	*/
}

BEGIN_EVENTSINK_MAP(CLED_DisplayInspectDlg, CDialogEx)
	ON_EVENT(CLED_DisplayInspectDlg, IDC_EXITVIEW_BTN, DISPID_CLICK, CLED_DisplayInspectDlg::ClickExitviewBtn, VTS_NONE)
	ON_EVENT(CLED_DisplayInspectDlg, IDC_TEACHVIEW_BTN, DISPID_CLICK, CLED_DisplayInspectDlg::ClickTeachviewBtn, VTS_NONE)
	ON_EVENT(CLED_DisplayInspectDlg, IDC_BTNENHCTRL_LIVE, DISPID_CLICK, CLED_DisplayInspectDlg::ClickBtnenhctrlLive, VTS_NONE)
	ON_EVENT(CLED_DisplayInspectDlg, IDC_BTNENHCTRL_SAVE, DISPID_CLICK, CLED_DisplayInspectDlg::ClickBtnenhctrlSave, VTS_NONE)
	ON_EVENT(CLED_DisplayInspectDlg, IDC_BTNENHCTRL_LOAD, DISPID_CLICK, CLED_DisplayInspectDlg::ClickBtnenhctrlLoad, VTS_NONE)
	ON_EVENT(CLED_DisplayInspectDlg, IDC_BTNENHCTRL_ZOOMIN, DISPID_CLICK, CLED_DisplayInspectDlg::ClickBtnenhctrlZoomin, VTS_NONE)
	ON_EVENT(CLED_DisplayInspectDlg, IDC_BTNENHCTRL_ZOOMOUT, DISPID_CLICK, CLED_DisplayInspectDlg::ClickBtnenhctrlZoomout, VTS_NONE)
	ON_EVENT(CLED_DisplayInspectDlg, IDC_BTNENHCTRL_ONE, DISPID_CLICK, CLED_DisplayInspectDlg::ClickBtnenhctrlOne, VTS_NONE)
	ON_EVENT(CLED_DisplayInspectDlg, IDC_BTNENHCTRL_FIT, DISPID_CLICK, CLED_DisplayInspectDlg::ClickBtnenhctrlFit, VTS_NONE)
	ON_EVENT(CLED_DisplayInspectDlg, IDC_BTNENHCTRL_TESTRUN, DISPID_CLICK, CLED_DisplayInspectDlg::ClickBtnenhctrlTestrun, VTS_NONE)
	ON_EVENT(CLED_DisplayInspectDlg, IDC_MANUALMVIEW_BTN, DISPID_CLICK, CLED_DisplayInspectDlg::ClickManualmviewBtn, VTS_NONE)
	ON_EVENT(CLED_DisplayInspectDlg, IDC_MAINVIEW_BTN, DISPID_CLICK, CLED_DisplayInspectDlg::ClickMainviewBtn, VTS_NONE)
	ON_EVENT(CLED_DisplayInspectDlg, IDC_DATAVIEW_BTN, DISPID_CLICK, CLED_DisplayInspectDlg::ClickDataviewBtn, VTS_NONE)
	ON_EVENT(CLED_DisplayInspectDlg, IDB_MAIN_OP_START, DISPID_CLICK, CLED_DisplayInspectDlg::ClickMainOpStart, VTS_NONE)
	ON_EVENT(CLED_DisplayInspectDlg, IDB_MAIN_OP_STOP, DISPID_CLICK, CLED_DisplayInspectDlg::ClickMainOpStop, VTS_NONE)
	ON_EVENT(CLED_DisplayInspectDlg, IDB_MAIN_OP_INIT, DISPID_CLICK, CLED_DisplayInspectDlg::ClickMainOpInit, VTS_NONE)
	ON_EVENT(CLED_DisplayInspectDlg, IDB_MAIN_INSPECTION_RESET, DISPID_CLICK, CLED_DisplayInspectDlg::ClickMainInspectionReset, VTS_NONE)
	ON_EVENT(CLED_DisplayInspectDlg, IDB_MAIN_BEFORE_RSLT_VIEW, DISPID_CLICK, CLED_DisplayInspectDlg::ClickMainBeforeRsltView, VTS_NONE)
	ON_EVENT(CLED_DisplayInspectDlg, IDB_MAIN_PRODUCT_RESET, DISPID_CLICK, CLED_DisplayInspectDlg::ClickMainProductReset, VTS_NONE)
	ON_EVENT(CLED_DisplayInspectDlg, IDB_MAIN_MOTION_STOP, DISPID_CLICK, CLED_DisplayInspectDlg::ClickMainMotionStop, VTS_NONE)
END_EVENTSINK_MAP()


void CLED_DisplayInspectDlg::ClickExitviewBtn()
{
	// TODO: 여기에 메시지 처리기 코드를 추가합니다.

	if(AfxMessageBox(_T("Program Exit?"), MB_YESNO) == IDNO)
	{
		return;
	}

	KillTimer(TIMER_GRAB_LIVE);
	KillTimer(TIMER_TIME_SHOW);
	KillTimer(TIMER_ALL_ORIGIN_START);
	//KillTimer(TIMER_MOTION_AMP);

	m_Flag.m_bProcessThread = FALSE;

	m_bInspThreadEnd = TRUE;

	m_bAutoDeleteStart = FALSE;
	m_bAutoDeleteEnd = TRUE;

	if(!m_dlgData->GetNonModel())
	{
		//save
		CString strPath;
		strPath.Format(_T("%s"), m_dlgData->GetSelectModel().cPath);
		_MakeDirectories(strPath);
		m_dlgTeach->SaveTeachPara(strPath);
		m_dlgManual->SaveManualPara(DB_PATH);
	}

	delete m_dlgProcessBar;
	delete m_dlgNgView;
	delete m_dlgManual;
	delete m_dlgTeach;
	delete m_dlgImageRect;
	delete m_dlgLogAlarm;
	delete m_dlgData;
#ifdef CAMERA
	m_MilManage.Close();
#ifdef BASLER
	m_BaslerManage->Release();
	delete m_BaslerManage;
#endif
#endif

	DeleteCriticalSection(&m_CSInline);

	EndDialog(0);
}


void CLED_DisplayInspectDlg::ClickTeachviewBtn()
{
	// TODO: 여기에 메시지 처리기 코드를 추가합니다.
	if(m_dlgData->GetNonModel())
	{
		AfxMessageBox(_T("모델이 없습니다. 모델을 먼저 생성 하십시오."));
		WindowVisible(AUTO_WINDOW);
		return;
	}

#ifdef _RELEASE
	CDialogParameterLock dlgParaLock;  //20161006 sc

	if(dlgParaLock.DoModal())
	{
		if(dlgParaLock.GetPassRet())
		{
			WindowVisible(TEACH_WINDOW);
		}
		else
		{
			return;
		}
	}
#else
	WindowVisible(TEACH_WINDOW);
#endif
}


void CLED_DisplayInspectDlg::ClickBtnenhctrlLive()
{

	//20161116 nht 검사 시퀀스 도중 Live영상 촬영 방지
	if(m_btn_Start.IsWindowEnabled() == 0)
	{
		AfxMessageBox(_T("검사가 진행중입니다."));
		m_btnLive.SetValue(FALSE);
		return;
	}

	// TODO: 여기에 메시지 처리기 코드를 추가합니다.
	CString text;
	m_btnLive.GetWindowTextA(text);
	UpdateData(FALSE);
	//	if(!text.Compare(_T("LIVE")))
	if(m_btnLive.GetValue())
	{
		
#ifdef CAMERA
			MdispControl(*m_MilManage.GetMilDisplay(MAIN_DISP), M_OVERLAY_CLEAR, M_DEFAULT);
#endif
			//	m_btnLive.SetWindowTextA(_T("STOP"));
			SetTimer(TIMER_GRAB_LIVE, 100, NULL);
			//Light on
			m_dlgTeach->SetLight(LIGHT_1_CH, m_dlgTeach->GetTeachPara().TeachLight[m_dlgTeach->GetInspGrabIdx()][0], m_dlgTeach->GetInspGrabIdx());
			Sleep(200);
			m_dlgTeach->SetLight(LIGHT_2_CH, m_dlgTeach->GetTeachPara().TeachLight[m_dlgTeach->GetInspGrabIdx()][1], m_dlgTeach->GetInspGrabIdx());
		}
		else
		{
			//	m_btnLive.SetWindowTextA(_T("LIVE"));
			KillTimer(TIMER_GRAB_LIVE);
			//Light off
			m_dlgTeach->OnOffLight(FALSE, m_dlgTeach->GetInspGrabIdx());
		}
	
	
	
}


void CLED_DisplayInspectDlg::ClickBtnenhctrlSave()
{
	// TODO: 여기에 메시지 처리기 코드를 추가합니다.
	CString tmpPath_Name;
	CFileDialog dlgFile(FALSE,"Image","*.bmp",
		OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT,
		"Bitmap Image(*.bmp)|*.bmp|");

	/*
	const int c_cMaxFiles = 10;  // 메모리 부족현상으로 확장 안해주면 몇개 못씀
	const int c_cbBuffSize = (c_cMaxFiles * (MAX_PATH + 1)) + 1;
	dlg.GetOFN().lpstrFile = strFileList.GetBuffer(c_cbBuffSize);
	dlg.GetOFN().nMaxFile = c_cbBuffSize;
	*/

	if(dlgFile.DoModal() == IDOK)
	{
		tmpPath_Name = dlgFile.GetPathName();
#ifdef CAMERA
		MbufExport((char *)(LPCTSTR)tmpPath_Name, M_BMP, *m_MilManage.GetMilDispBuf(MAIN_DISP));
#endif
	}

	delete dlgFile;	
}


void CLED_DisplayInspectDlg::ClickBtnenhctrlLoad()
{
	// TODO: 여기에 메시지 처리기 코드를 추가합니다.
	CString tmpPath_Name;
	CFileDialog dlgFile(TRUE,"Image","*.bmp",
		OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT,
		"Bitmap Image(*.bmp)|*.bmp|");
	if(dlgFile.DoModal() == IDOK)
	{ 
		tmpPath_Name = dlgFile.GetPathName();
#ifdef CAMERA
		MbufImport((char *)(LPCTSTR)tmpPath_Name, M_BMP, M_LOAD, *m_MilManage.GetMilSystem(), m_MilManage.GetMilGrabBuf());
		MbufCopy(*m_MilManage.GetMilGrabBuf(), *m_MilManage.GetMilDispBuf(MAIN_DISP));
		MdispControl(*m_MilManage.GetMilDisplay(MAIN_DISP), M_OVERLAY_CLEAR, M_DEFAULT);
#endif
	}

	delete dlgFile;	
}


void CLED_DisplayInspectDlg::ClickBtnenhctrlZoomin()
{
	// TODO: 여기에 메시지 처리기 코드를 추가합니다.
	double win_left, win_top, win_right, win_bottom;
	double sx, sy, ex, ey, o_rat, rat, rrat;
	double cx, cy;
	BOOL over_disp = FALSE;;

	CdRect dWheelRect, temp_rect;
	CRect img_rect;
	o_rat = m_dZoomRatio;
	m_dZoomRatio = m_dZoomRatio + 0.2;
	m_dlgImageRect->m_dZoomRatio = m_dZoomRatio;
	int ins_idx = m_dlgImageRect->GetInsIdx();
	int grab_idx = m_dlgImageRect->GetGrabIdx();

	for(int k = 0; k < MAX_GRAB_CNT; k++)
	{
		for(int j = 0; j < MAX_INSP_GRAB_CNT; j++)
		{
			for(int i = 0; i < TRACKER_MAX_NUM; i++)
			{
				m_dlgImageRect->m_Tracker[k][j][i].GetRect(&win_left, &win_top, &win_right, &win_bottom);	
				m_dlgImageRect->m_Tracker[k][j][i].GetZoom(&sx, &sy, &ex, &ey);
				rat = m_dZoomRatio / o_rat;
				rrat = (o_rat / m_dZoomRatio);

				if(m_bMouseWheel)
				{

				}

				//Tracker zoom///////////////////////////
				cx = (sx + ex) * 0.5;
				cy = (sy + ey) * 0.5;

				sx = sx - cx;
				sy = sy - cy;
				ex = ex - cx;
				ey = ey - cy;

				sx = sx * rrat + cx;
				sy = sy * rrat + cy;
				ex = ex * rrat + cx;
				ey = ey * rrat + cy;

				m_dlgImageRect->m_Tracker[k][j][i].SetZoom(sx, sy, ex, ey);

				//Tracker rect///////////////////////////
				cx = m_dlgImageRect->m_rectDlg.Width() * 0.5;
				cy = m_dlgImageRect->m_rectDlg.Height() * 0.5;

				win_left = win_left - cx;
				win_top = win_top - cy;
				win_right = win_right - cx;
				win_bottom = win_bottom - cy;

				win_left = win_left * rat + cx;
				win_top = win_top * rat + cy;
				win_right = win_right * rat + cx;
				win_bottom = win_bottom * rat + cy;

				m_dlgImageRect->m_Tracker[k][j][i].SetRect(win_left, win_top, win_right, win_bottom);
			}
		}
	}

	m_dlgImageRect->SetZoom(m_dZoomRatio, (long)(ex - sx), (long)(ey - sy));
#ifdef CAMERA
	MdispZoom(*m_MilManage.GetMilDisplay(0), m_dlgImageRect->GetZoom(), m_dlgImageRect->GetZoom());
	MdispPan(*m_MilManage.GetMilDisplay(0), sx, sy);
#endif
}


void CLED_DisplayInspectDlg::ClickBtnenhctrlZoomout()
{
	// TODO: 여기에 메시지 처리기 코드를 추가합니다.
	double win_left, win_top, win_right, win_bottom;
	double sx, sy, ex, ey, o_rat, rat, rrat;
	double cx, cy;

	CdRect dWheelRect, temp_rect;
	CRect img_rect;

	o_rat = m_dZoomRatio;
	m_dZoomRatio = m_dZoomRatio - 0.2;
	m_dlgImageRect->m_dZoomRatio = m_dZoomRatio;
	int ins_idx = m_dlgImageRect->GetInsIdx();
	int grab_idx = m_dlgImageRect->GetGrabIdx();

	if(m_dlgImageRect->m_dZoomDefault > m_dZoomRatio)
	{
		m_dZoomRatio = m_dlgImageRect->m_dZoomDefault;
	}

	for(int k = 0; k < MAX_GRAB_CNT; k++)
	{
		for(int j = 0; j < MAX_INSP_GRAB_CNT; j++)
		{
			for(int i = 0; i < TRACKER_MAX_NUM; i++)
			{
				m_dlgImageRect->m_Tracker[k][j][i].GetRect(&win_left, &win_top, &win_right, &win_bottom);	
				m_dlgImageRect->m_Tracker[k][j][i].GetZoom(&sx, &sy, &ex, &ey);
				rat = m_dZoomRatio / o_rat;
				rrat = (o_rat / m_dZoomRatio);

				if(m_bMouseWheel)
				{

				}

				//Tracker zoom///////////////////////////
				cx = (sx + ex) * 0.5;
				cy = (sy + ey) * 0.5;

				sx = sx - cx;
				sy = sy - cy;
				ex = ex - cx;
				ey = ey - cy;

				sx = sx * rrat + cx;
				sy = sy * rrat + cy;
				ex = ex * rrat + cx;
				ey = ey * rrat + cy;

				m_dlgImageRect->m_Tracker[k][j][i].SetZoom(sx, sy, ex, ey);

				//Tracker rect///////////////////////////
				cx = m_dlgImageRect->m_rectDlg.Width() * 0.5;
				cy = m_dlgImageRect->m_rectDlg.Height() * 0.5;

				win_left = win_left - cx;
				win_top = win_top - cy;
				win_right = win_right - cx;
				win_bottom = win_bottom - cy;

				win_left = win_left * rat + cx;
				win_top = win_top * rat + cy;
				win_right = win_right * rat + cx;
				win_bottom = win_bottom * rat + cy;

				m_dlgImageRect->m_Tracker[k][j][i].SetRect(win_left, win_top, win_right, win_bottom);
			}
		}
	}
	m_dlgImageRect->SetZoom(m_dZoomRatio, (long)(ex - sx), (long)(ey - sy));
#ifdef CAMERA
	MdispZoom(*m_MilManage.GetMilDisplay(MAIN_DISP), m_dlgImageRect->GetZoom(), m_dlgImageRect->GetZoom());
	MdispPan(*m_MilManage.GetMilDisplay(MAIN_DISP), sx, sy);
#endif
}


void CLED_DisplayInspectDlg::ClickBtnenhctrlOne()
{
	// TODO: 여기에 메시지 처리기 코드를 추가합니다.
	double win_left, win_top, win_right, win_bottom;
	double sx, sy, ex, ey, o_rat, rat, rrat;
	double cx, cy;

	o_rat = m_dZoomRatio;
	m_dZoomRatio = 1.0;
	m_dlgImageRect->m_dZoomRatio = m_dZoomRatio;
	int ins_idx = m_dlgImageRect->GetInsIdx();
	int grab_idx = m_dlgImageRect->GetGrabIdx();

	for(int k = 0; k < MAX_GRAB_CNT; k++)
	{
		for(int j = 0; j < MAX_INSP_GRAB_CNT; j++)
		{
			for(int i = 0; i < TRACKER_MAX_NUM; i++)
			{
				m_dlgImageRect->m_Tracker[k][j][i].GetRect(&win_left, &win_top, &win_right, &win_bottom);	
				m_dlgImageRect->m_Tracker[k][j][i].GetZoom(&sx, &sy, &ex, &ey);
				rat = m_dZoomRatio / o_rat;
				rrat = (o_rat / m_dZoomRatio);

				//Tracker zoom///////////////////////////
				cx = (sx + ex) * 0.5;
				cy = (sy + ey) * 0.5;

				sx = sx - cx;
				sy = sy - cy;
				ex = ex - cx;
				ey = ey - cy;

				sx = sx * rrat + cx;
				sy = sy * rrat + cy;
				ex = ex * rrat + cx;
				ey = ey * rrat + cy;

				m_dlgImageRect->m_Tracker[k][j][i].SetZoom(sx, sy, ex, ey);

				//Tracker rect///////////////////////////
				cx = m_dlgImageRect->m_rectDlg.Width() * 0.5;
				cy = m_dlgImageRect->m_rectDlg.Height() * 0.5;

				win_left = win_left - cx;
				win_top = win_top - cy;
				win_right = win_right - cx;
				win_bottom = win_bottom - cy;

				win_left = win_left * rat + cx;
				win_top = win_top * rat + cy;
				win_right = win_right * rat + cx;
				win_bottom = win_bottom * rat + cy;

				m_dlgImageRect->m_Tracker[k][j][i].SetRect(win_left, win_top, win_right, win_bottom);
			}
		}
	}

	m_dlgImageRect->SetZoom(m_dZoomRatio, (long)(ex - sx), (long)(ey - sy));
#ifdef CAMERA
	MdispZoom(*m_MilManage.GetMilDisplay(MAIN_DISP), m_dlgImageRect->GetZoom(), m_dlgImageRect->GetZoom());
	MdispPan(*m_MilManage.GetMilDisplay(MAIN_DISP), sx, sy);
#endif
}


void CLED_DisplayInspectDlg::ClickBtnenhctrlFit()
{
	// TODO: 여기에 메시지 처리기 코드를 추가합니다.
	double win_left, win_top, win_right, win_bottom;
	double sx, sy, ex, ey, o_rat, rat, rrat;
	double cx, cy;

	o_rat = m_dZoomRatio;
	m_dZoomRatio = m_dlgImageRect->m_dZoomDefault;
	m_dlgImageRect->m_dZoomRatio = m_dZoomRatio;
	int ins_idx = m_dlgImageRect->GetInsIdx();
	int grab_idx = m_dlgImageRect->GetGrabIdx();

	for(int k = 0; k < MAX_GRAB_CNT; k++)
	{
		for(int j = 0; j < MAX_INSP_GRAB_CNT; j++)
		{
			for(int i = 0; i < TRACKER_MAX_NUM; i++)
			{
				m_dlgImageRect->m_Tracker[k][j][i].GetRect(&win_left, &win_top, &win_right, &win_bottom);	
				m_dlgImageRect->m_Tracker[k][j][i].GetZoom(&sx, &sy, &ex, &ey);
				rat = m_dZoomRatio / o_rat;
				rrat = (o_rat / m_dZoomRatio);

				//Tracker zoom///////////////////////////
				cx = (sx + ex) * 0.5;
				cy = (sy + ey) * 0.5;

				sx = sx - cx;
				sy = sy - cy;
				ex = ex - cx;
				ey = ey - cy;

				sx = sx * rrat + cx;
				sy = sy * rrat + cy;
				ex = ex * rrat + cx;
				ey = ey * rrat + cy;

				m_dlgImageRect->m_Tracker[k][j][i].SetZoom(sx, sy, ex, ey);

				//Tracker rect///////////////////////////
				cx = m_dlgImageRect->m_rectDlg.Width() * 0.5;
				cy = m_dlgImageRect->m_rectDlg.Height() * 0.5;

				win_left = win_left - cx;
				win_top = win_top - cy;
				win_right = win_right - cx;
				win_bottom = win_bottom - cy;

				win_left = win_left * rat + cx;
				win_top = win_top * rat + cy;
				win_right = win_right * rat + cx;
				win_bottom = win_bottom * rat + cy;

				m_dlgImageRect->m_Tracker[k][j][i].SetRect(win_left, win_top, win_right, win_bottom);
			}
		}
	}

	m_dlgImageRect->SetZoom(m_dZoomRatio, (long)(ex - sx), (long)(ey - sy));
#ifdef CAMERA
	MdispZoom(*m_MilManage.GetMilDisplay(MAIN_DISP), m_dlgImageRect->GetZoom(), m_dlgImageRect->GetZoom());
	MdispPan(*m_MilManage.GetMilDisplay(MAIN_DISP), 0, 0);
#endif
	m_dlgImageRect->SetReDraw();
}


void CLED_DisplayInspectDlg::ClickBtnenhctrlTestrun()
{

	//20161116 nht 검사도중 Test버튼 방지 
	if(m_btn_Start.IsWindowEnabled() == 0)
	{
		AfxMessageBox(_T("검사가 진행중입니다."));
		return;
	}

	// TODO: 여기에 메시지 처리기 코드를 추가합니다.
	if(m_dlgData->GetNonModel())
	{
		AfxMessageBox(_T("모델이 없습니다. 모델을 먼저 생성 하십시오."));
		WindowVisible(AUTO_WINDOW);
		return;
	}

	//검사중 테스트 못하게 추가
	if(!m_btn_Start.GetEnabled()) //20161014 sc 
	{
		return;
	}

	int rslt = 0;
	CString strLog, strScreenFilePath;
	SYSTEMTIME	lpSystemTime;
	BOOL isNG = FALSE;
	GetLocalTime(&lpSystemTime);

	InitRsltPara();
	InitInsRsltPara(0);
	InitInsRsltPara(1);
	InitBeforeRsltPara();
	m_RsltIdx = 0;
	m_Rslt.m_nInspGrabIdx = 0;
	m_Rslt.m_bGrabComp = 0;
	m_Rslt.m_nImageSel = 0;
	m_Rslt.m_bInspectionComp = 1;
	m_Rslt.m_bInspComp = FALSE;

	for(int i = 0; i < 3; i++)
	{
		for(int j = 0; j < 3; j++)
		{
			m_Rslt.m_bInspRslt[i][j] = TRUE;
			m_Rslt.m_bInspectionRslt[i][j] = FALSE;
			m_Rslt.m_bAlignInsp[i] = TRUE;
		}
	}
	if(m_dlgTeach->IsWindowVisible())
	{
		m_poDeviceIdx[m_Rslt.m_bGrabComp].x = m_dlgTeach->GetDeviceIdxX();
		m_poDeviceIdx[m_Rslt.m_bGrabComp].y = m_dlgTeach->GetDeviceIdxY() * m_dlgTeach->GetTeachPara().TeachOneGrabInspCnt;
		m_nGrabIdx[m_Rslt.m_bGrabComp] = m_dlgTeach->GetGrabIdx();
		m_nInspGrabIdx[m_Rslt.m_bGrabComp] = m_dlgTeach->GetInspGrabIdx();

		sprintf(m_Rslt.m_cStartDate, "%04d%02d%02d", lpSystemTime.wYear, lpSystemTime.wMonth, lpSystemTime.wDay);
		sprintf(m_Rslt.m_cStartTime, "%02d%02d%02d", lpSystemTime.wHour, lpSystemTime.wMinute, lpSystemTime.wSecond);
		for(m_Rslt.m_nInspGrabIdx = 0; m_Rslt.m_nInspGrabIdx < m_dlgTeach->GetTeachPara().TeachInspGrabCnt; m_Rslt.m_nInspGrabIdx++)
		{
			strLog.Format(_T("Device Idx X : %d, Device Idx Y : %d, Grab Idx : %d, Insp Grab Idx %d, Grab & Inspection Test Start"), m_dlgTeach->GetDeviceIdxX(), m_dlgTeach->GetDeviceIdxY(), m_dlgTeach->GetGrabIdx(), m_Rslt.m_nInspGrabIdx);
			WriteLog(strLog, TYPE_LOG_MAIN, FALSE);

			for(int i = 0; i < MAX_LIGHT; i++)
			{
				m_Rslt.m_nLightVal[m_Rslt.m_nInspGrabIdx][i] = m_dlgTeach->GetTeachPara().TeachLight[m_Rslt.m_nInspGrabIdx][i];
			}

			//Light On
			m_dlgTeach->OnOffLight(TRUE, m_Rslt.m_nInspGrabIdx);
			strLog.Format(_T("Light On, Ch1 : %d, Ch2 : %d"), m_Rslt.m_nLightVal[m_Rslt.m_nInspGrabIdx][LIGHT_1_CH - 1], m_Rslt.m_nLightVal[m_Rslt.m_nInspGrabIdx][LIGHT_2_CH - 1]);
			WriteLog(strLog, TYPE_LOG_MAIN, FALSE);
			Sleep(100);

#ifdef BASLER
			Grab();
			MbufCopy(*m_MilManage.GetMilGrabBuf(), *m_MilManage.GetMilInspBuf(m_Rslt.m_bGrabComp, m_Rslt.m_nInspGrabIdx));
#else
			//strLog.Format(_T("D:\\DGT_TEST_IMAGE\\0_%d.bmp"), m_Rslt.m_nInspGrabIdx + 1);
			strLog.Format(_T("D:\\DGT_TEST_IMAGE\\TEST\\2Grab\\X_%02d_Y_%02d_G_%02d_I_%d.bmp"), m_poDeviceIdx[m_Rslt.m_bGrabComp].x, m_poDeviceIdx[m_Rslt.m_bGrabComp].y, m_nGrabIdx[m_Rslt.m_bGrabComp], m_Rslt.m_nInspGrabIdx);
#ifdef CAMERA
			MbufImport((char *)(LPCTSTR)strLog, M_BMP, M_LOAD, *m_MilManage.GetMilSystem(), m_MilManage.GetMilGrabBuf());
			MbufCopy(*m_MilManage.GetMilGrabBuf(), *m_MilManage.GetMilDispBuf(MAIN_DISP));
			MbufCopy(*m_MilManage.GetMilGrabBuf(), *m_MilManage.GetMilInspBuf(m_Rslt.m_bGrabComp, m_Rslt.m_nInspGrabIdx));
			//MbufCopy(*m_MilManage.GetMilDispBuf(MAIN_DISP), *m_MilManage.GetMilGrabBuf());
			//MbufCopy(*m_MilManage.GetMilDispBuf(MAIN_DISP), *m_MilManage.GetMilInspBuf(m_Rslt.m_bGrabComp, m_Rslt.m_nInspGrabIdx));
#else
			MbufCopy(*m_MilManage.GetMilDispBuf(MAIN_DISP), *m_MilManage.GetMilGrabBuf());
			MbufCopy(*m_MilManage.GetMilDispBuf(MAIN_DISP), *m_MilManage.GetMilInspBuf(m_Rslt.m_bGrabComp, m_Rslt.m_nInspGrabIdx));
#endif
#endif
			m_dGrabInspStartTime = clock();

		}

		if(m_Rslt.m_bInspectionComp)
		{
			m_bInspThreadStart = TRUE;
		}
	}
}

BOOL CLED_DisplayInspectDlg::InspectionAlignTeaching(int nDeviceIdxX, int nDeviceIdxY, int nGrabIdx, int nInspGrabIdx)
{

	BOOL rslt = TRUE;
#ifdef CAMERA
	UpdateData(TRUE);
	int ins_idx = 0;
	BOOL search_rslt = FALSE;
	MIL_ID image;;
	CString strModPath, strTeachPath, strTeachFilePath, strTeachImagePath;
	typeTeachPara teachPara;
	teachPara = m_dlgTeach->GetTeachPara();

	MbufAlloc2d(*m_MilManage.GetMilSystem(), CAMERA_WIDTH, CAMERA_HEIGHT, 8+M_UNSIGNED, M_IMAGE + M_PROC, &image);
	MbufCopy(*m_MilManage.GetMilDispBuf(MAIN_DISP), image);
	MdispControl(*m_MilManage.GetMilDisplay(MAIN_DISP), M_OVERLAY_CLEAR, M_DEFAULT);

	strTeachPath.Format("%s\\TeachImg", GetSelectModel().cPath);
	_MakeDirectories(strTeachPath);
	strTeachImagePath.Format("%s\\Align_%d_%d.bmp", strTeachPath, nGrabIdx, nInspGrabIdx);

	MgraControl(M_DEFAULT, M_FONT_X_SCALE , 4);
	MgraControl(M_DEFAULT, M_FONT_Y_SCALE , 4);

	int align_idx = 0;
	int align_search_idx = 0;

	for(int i = 0; i < teachPara.TeachRegionCnt[nGrabIdx][nInspGrabIdx]; i++)
	{
		align_idx = 1 + (i * 3);
		align_search_idx = (i * 3);
		strTeachFilePath.Format("%s\\Align_%d_%d_%d.mod", strTeachPath, nGrabIdx, nInspGrabIdx, i);
		//MbufExport(MIL_TEXT((char *)(LPCTSTR)strTeachImagePath), M_BMP, image);

		teachPara.AlignPara.bCreatMdID[nGrabIdx][nInspGrabIdx][i] = 0;
		if(teachPara.AlignPara.bCreatMdID[nGrabIdx][nInspGrabIdx][i])
		{
			if(teachPara.AlignPara.MilSearchContext[nGrabIdx][nInspGrabIdx][i])
			{
				MmodFree(teachPara.AlignPara.MilSearchContext[nGrabIdx][nInspGrabIdx][i]);
				teachPara.AlignPara.MilSearchContext[nGrabIdx][nInspGrabIdx][i] = NULL;
			}
		}

		MmodAlloc(*m_MilManage.GetMilSystem(), M_GEOMETRIC, M_DEFAULT, &teachPara.AlignPara.MilSearchContext[nGrabIdx][nInspGrabIdx][i]);

		CdRect rect(teachPara.TeachRegion[nGrabIdx][nInspGrabIdx][align_idx]);
		CdRect search(teachPara.TeachRegion[nGrabIdx][nInspGrabIdx][align_search_idx]);
		CdPoint cent = rect.GetCenter();
		MappControl(M_ERROR, M_PRINT_DISABLE);
		MmodDefine(teachPara.AlignPara.MilSearchContext[nGrabIdx][nInspGrabIdx][i], M_IMAGE, image, rect.GetLeft(), rect.GetTop(), rect.GetWidth(), rect.GetHeight());

		if((rect.GetLeft() >= search.GetLeft()) && (rect.GetTop() >= search.GetTop()) && (rect.GetRight() <= search.GetRight()) && (rect.GetBottom() <= search.GetBottom()))
		{
			search_rslt = TRUE;
		}

		if(!MappGetError(M_GLOBAL, M_NULL) && search_rslt)
		{
			teachPara.AlignPara.bCreatMdID[nGrabIdx][nInspGrabIdx][i] = TRUE;


			MmodControl(teachPara.AlignPara.MilSearchContext[nGrabIdx][nInspGrabIdx][i], M_CONTEXT, M_SPEED, M_VERY_HIGH);

			MmodControl(teachPara.AlignPara.MilSearchContext[nGrabIdx][nInspGrabIdx][i], M_DEFAULT, M_ANGLE_DELTA_NEG, 5.0);
			MmodControl(teachPara.AlignPara.MilSearchContext[nGrabIdx][nInspGrabIdx][i], M_DEFAULT, M_ANGLE_DELTA_POS, 5.0);


			MmodPreprocess(teachPara.AlignPara.MilSearchContext[nGrabIdx][nInspGrabIdx][i], M_DEFAULT);


			MgraColor(M_DEFAULT, M_COLOR_RED);
			MmodDraw(M_DEFAULT, teachPara.AlignPara.MilSearchContext[nGrabIdx][nInspGrabIdx][i], *m_MilManage.GetMilOverlay(MAIN_DISP), M_DRAW_POSITION, 0, M_ORIGINAL);

			MgraControl(M_DEFAULT, M_BACKGROUND_MODE, M_TRANSPARENT);
			MgraColor(M_DEFAULT, M_COLOR_GREEN);
			MgraText(M_DEFAULT, *m_MilManage.GetMilOverlay(MAIN_DISP), rect.GetLeft() + 20, rect.GetTop() + 20, MIL_TEXT("Align Teaching Success...!"));
			WriteLog("Align Teaching Success...!", TYPE_LOG_MAIN);
			//strModPath.Format("%s\\Align_Teaching", pdlg->m_ModelInfo.strModelPath);
			MmodSave(MIL_TEXT((char *)(LPCTSTR)strTeachFilePath), teachPara.AlignPara.MilSearchContext[nGrabIdx][nInspGrabIdx][i], M_DEFAULT);
		}
		else
		{
			MgraControl(M_DEFAULT, M_BACKGROUND_MODE, M_TRANSPARENT);
			MgraColor(M_DEFAULT, M_COLOR_RED);
			MgraText(M_DEFAULT, *m_MilManage.GetMilOverlay(MAIN_DISP), rect.GetLeft() + 20, rect.GetTop() + 20, MIL_TEXT("Align Teaching Fail...!"));
			teachPara.AlignPara.bCreatMdID[nGrabIdx][nInspGrabIdx][i] = FALSE;
			WriteLog("Align Teaching Fail...!", TYPE_LOG_MAIN);
			rslt = FALSE;
		}

		MmodFree(teachPara.AlignPara.MilSearchContext[nGrabIdx][nInspGrabIdx][i]);
	}

	if(rslt)
	{
		teachPara.AlignPara.bAlign = FALSE;
	
		m_dlgTeach->SetTeachPara(teachPara);

		InspectionAlign(nDeviceIdxX, nDeviceIdxY, nGrabIdx, nInspGrabIdx, 0);

		teachPara = m_dlgTeach->GetTeachPara();

		teachPara.AlignPara.bAlign = TRUE;
		m_dlgTeach->SetTeachPara(teachPara);
	}
	MappControl(M_ERROR, M_PRINT_ENABLE);

	MbufFree(image);
#endif
	return rslt;
}

BOOL CLED_DisplayInspectDlg::InspectionAlign(int nDeviceIdxX, int nDeviceIdxY, int nGrabIdx, int nInspGrabIdx, BOOL isGrabComp)
{
	BOOL rslt = TRUE;
#ifdef CAMERA
	CString temp, strModPath;
	long count;
	BOOL search_rslt = FALSE;
	double x[PATTERN_FIND_MAX_NUM], y[PATTERN_FIND_MAX_NUM], ang[PATTERN_FIND_MAX_NUM], score[PATTERN_FIND_MAX_NUM];
	typeTeachPara teachPara;
	teachPara = m_dlgTeach->GetTeachPara();
	CString tmpPath_Name, strLog;

	int ins_idx = 0;
	int search_idx = 1;
	int align_idx = 0;
	int align_search_idx = 0;
	MIL_ID image, image2;
	MIL_ID MilChildImage, MilTransImage;//20161121
	MIL_ID pat_rslt = NULL;

	MbufAlloc2d(*m_MilManage.GetMilSystem(), CAMERA_WIDTH, CAMERA_HEIGHT, 8+M_UNSIGNED,M_IMAGE+M_PROC, &image);
	MbufAlloc2d(*m_MilManage.GetMilSystem(), CAMERA_WIDTH, CAMERA_HEIGHT, 8+M_UNSIGNED,M_IMAGE+M_PROC, &MilTransImage);
	MbufCopy(*m_MilManage.GetMilDispBuf(MAIN_DISP), image);
	MdispControl(*m_MilManage.GetMilDisplay(MAIN_DISP), M_OVERLAY_CLEAR, M_DEFAULT);

	MgraControl(M_DEFAULT, M_FONT_X_SCALE , 4);
	MgraControl(M_DEFAULT, M_FONT_Y_SCALE , 4);

	for(int j = 0; j < teachPara.TeachRegionCnt[nGrabIdx][nInspGrabIdx]; j++)
	{
		align_idx = 1 + (j * 3);
		align_search_idx = (j * 3);

		if((nDeviceIdxY + j) == teachPara.TeachArrayCntY)
		{
			continue;
		}

		//Test///////////////////////////////////////
		CdRect rect;
		rect.SetdRectLTRB(teachPara.TeachRegion[nGrabIdx][nInspGrabIdx][align_idx].GetLeft(),
			teachPara.TeachRegion[nGrabIdx][nInspGrabIdx][align_idx].GetTop(),
			teachPara.TeachRegion[nGrabIdx][nInspGrabIdx][align_idx].GetRight(),
			teachPara.TeachRegion[nGrabIdx][nInspGrabIdx][align_idx].GetBottom());
		CdRect search;
		search.SetdRectLTRB(teachPara.TeachRegion[nGrabIdx][nInspGrabIdx][align_search_idx].GetLeft(),
			teachPara.TeachRegion[nGrabIdx][nInspGrabIdx][align_search_idx].GetTop(),
			teachPara.TeachRegion[nGrabIdx][nInspGrabIdx][align_search_idx].GetRight(),
			teachPara.TeachRegion[nGrabIdx][nInspGrabIdx][align_search_idx].GetBottom());
		CdPoint cent = search.GetCenter();
		double ndx, pdx, ndy, pdy;
		ndx = cent.GetX() - search.GetLeft();
		pdx = search.GetRight() - cent.GetX();
		ndy = cent.GetY() - search.GetTop();
		pdy = search.GetBottom() - cent.GetY();
		/////////////////////////////////////////////

		CString strTeachPath, strTeachImagePath, strTeachFilePath;

		strTeachPath.Format("%s\\TeachImg", GetSelectModel().cPath);
		strTeachImagePath.Format("%s\\Align_%d_%d.bmp", strTeachPath, nGrabIdx, nInspGrabIdx);
		strTeachFilePath.Format("%s\\Align_%d_%d_%d.mod", strTeachPath, nGrabIdx, nInspGrabIdx, j);
		search_rslt = FALSE;
		if((rect.GetLeft() >= search.GetLeft()) && (rect.GetTop() >= search.GetTop()) && (rect.GetRight() <= search.GetRight()) && (rect.GetBottom() <= search.GetBottom()))
		{
			search_rslt = TRUE;
		}

		if(search_rslt)
		{
			tmpPath_Name.Format(_T("Align_%d_%d_%d.mod"), nGrabIdx, nInspGrabIdx, j);
			if(!FileSearch(strTeachPath, tmpPath_Name))
			{
				MgraColor(M_DEFAULT, M_COLOR_RED);
				temp.Format(_T("Not Align Teaching"));
				MgraText(M_DEFAULT, *m_MilManage.GetMilOverlay(MAIN_DISP), rect.GetLeft() + 20, rect.GetTop() + 20, MIL_TEXT((char *)(LPCTSTR)temp));
				strLog.Format(_T("AlignImageSearchFail_%d"), j);
				WriteLog(strLog, TYPE_LOG_MAIN);
				MbufFree(image);
				return FALSE;
			}

			if(teachPara.AlignPara.MilSearchContext[nGrabIdx][nInspGrabIdx] == NULL)
			{
				MmodAlloc(*m_MilManage.GetMilSystem(), M_GEOMETRIC, M_DEFAULT, &teachPara.AlignPara.MilSearchContext[nGrabIdx][nInspGrabIdx][j]);
			}

			if(teachPara.AlignPara.bCreatMdID[nGrabIdx][nInspGrabIdx][j])
			{
				//strModPath.Format("%s\\Align_Teaching", pdlg->m_ModelInfo.strModelPath);
				MmodRestore(MIL_TEXT((char *)(LPCTSTR)strTeachFilePath), *m_MilManage.GetMilSystem(), M_DEFAULT , &teachPara.AlignPara.MilSearchContext[nGrabIdx][nInspGrabIdx][j]);
				MmodPreprocess(teachPara.AlignPara.MilSearchContext[nGrabIdx][nInspGrabIdx][j], M_DEFAULT);
				/* Allocate a result buffer. */
				MmodAllocResult(*m_MilManage.GetMilSystem(), M_DEFAULT, &pat_rslt);

				MmodControl(teachPara.AlignPara.MilSearchContext[nGrabIdx][nInspGrabIdx][j], M_DEFAULT, M_ACCEPTANCE, teachPara.TeachAlignScore);

				//Test 0820//////////////////////////////
				//Ins region
				MmodControl(teachPara.AlignPara.MilSearchContext[nGrabIdx][nInspGrabIdx][j], M_DEFAULT,  M_POSITION_X , cent.GetX());
				MmodControl(teachPara.AlignPara.MilSearchContext[nGrabIdx][nInspGrabIdx][j], M_DEFAULT,  M_POSITION_Y , cent.GetY());

				MmodControl(teachPara.AlignPara.MilSearchContext[nGrabIdx][nInspGrabIdx][j], M_DEFAULT,  M_POSITION_DELTA_NEG_X , ndx-10);
				MmodControl(teachPara.AlignPara.MilSearchContext[nGrabIdx][nInspGrabIdx][j], M_DEFAULT,  M_POSITION_DELTA_POS_X , pdx+10);
				MmodControl(teachPara.AlignPara.MilSearchContext[nGrabIdx][nInspGrabIdx][j], M_DEFAULT,  M_POSITION_DELTA_NEG_Y , ndy-10);
				MmodControl(teachPara.AlignPara.MilSearchContext[nGrabIdx][nInspGrabIdx][j], M_DEFAULT,  M_POSITION_DELTA_POS_Y , pdy+10);

				/////////////////////////////////////////

				//20161121
				/////////////////////////
				MbufClear(MilTransImage, 0x00);
				MbufChild2d(image, (MIL_INT)search.GetLeft(), (MIL_INT)search.GetTop(), (MIL_INT)search.GetWidth(), (MIL_INT)search.GetHeight(), &MilChildImage);
				MbufAlloc2d(*m_MilManage.GetMilSystem(), (MIL_INT)search.GetWidth(), (MIL_INT)search.GetHeight(), 8+M_UNSIGNED, M_IMAGE+M_PROC, &image2);
				MbufCopy(MilChildImage, image2),
				MimTranslate(image2, MilTransImage, search.GetLeft(), search.GetTop(), M_DEFAULT);

				/* Dummy first find operation for better function timing accuracy (model cache effet,...). */
				MmodFind(teachPara.AlignPara.MilSearchContext[nGrabIdx][nInspGrabIdx][j], MilTransImage, pat_rslt);//image -> MilTransImage


				/////////////////////////
				/* Get the number of models found. */
				MmodGetResult(pat_rslt, M_DEFAULT, M_NUMBER+M_TYPE_LONG, &count);

				MmodGetResult(pat_rslt, M_DEFAULT, M_POSITION_X, x);
				MmodGetResult(pat_rslt, M_DEFAULT, M_POSITION_Y, y);
				MmodGetResult(pat_rslt, M_DEFAULT, M_ANGLE, ang);
				MmodGetResult(pat_rslt, M_DEFAULT, M_SCORE, score);

				teachPara.AlignPara.dX[align_idx + nGrabIdx + j] = x[0];
				teachPara.AlignPara.dY[align_idx + nGrabIdx + j] = y[0];
				teachPara.AlignPara.dAng[align_idx + nGrabIdx + j] = ang[0];

				if(!teachPara.AlignPara.bAlign)
				{
					teachPara.AlignPara.dCentX[j + (m_dlgTeach->GetTeachPara().TeachGrabCnt * nGrabIdx)] = x[0];
					teachPara.AlignPara.dCentY[j + (m_dlgTeach->GetTeachPara().TeachGrabCnt * nGrabIdx)] = y[0];
				}

				for (int i=0; i<count; i++)
				{
					MgraColor(M_DEFAULT, M_COLOR_GREEN);
					MmodDraw(M_DEFAULT,  pat_rslt, *m_MilManage.GetMilOverlay(MAIN_DISP), M_DRAW_EDGES+M_DRAW_POSITION, i, M_DEFAULT);

					MgraControl(M_DEFAULT, M_BACKGROUND_MODE, M_TRANSPARENT);
					MgraColor(M_DEFAULT, M_COLOR_GREEN);
					temp.Format("X : %f, Y : %f", x[0], y[0]);
					MgraText(M_DEFAULT, *m_MilManage.GetMilOverlay(MAIN_DISP), rect.GetLeft() + 20, rect.GetTop() + 20, MIL_TEXT((char *)(LPCTSTR)temp));
					temp.Format("Ang : %f, Score : %f", ang[0], score[0]);
					MgraText(M_DEFAULT, *m_MilManage.GetMilOverlay(MAIN_DISP), rect.GetLeft() + 20, rect.GetTop() + 80, MIL_TEXT((char *)(LPCTSTR)temp));
					m_Rslt.m_bAlignInsp[j] = TRUE;

				}

				if(0 == count)
				{
					MgraControl(M_DEFAULT, M_BACKGROUND_MODE, M_TRANSPARENT);
					MgraColor(M_DEFAULT, M_COLOR_RED);
					MgraText(M_DEFAULT, *m_MilManage.GetMilOverlay(MAIN_DISP), rect.GetLeft() + 20, rect.GetTop() + 20, MIL_TEXT("Error : can't find pattern!"));
					temp.Format("Error : can't find pattern!");
					WriteLog(temp, TYPE_LOG_MAIN);
				}

				if(pat_rslt)
				{
					MmodFree(pat_rslt);
				}

				MmodFree(teachPara.AlignPara.MilSearchContext[nGrabIdx][nInspGrabIdx][j]);
				teachPara.AlignPara.MilSearchContext[nGrabIdx][nInspGrabIdx][j] = NULL;

				MbufFree(MilChildImage);
				MbufFree(image2);
			}
			else
			{
				MgraControl(M_DEFAULT, M_BACKGROUND_MODE, M_TRANSPARENT);
				MgraColor(M_DEFAULT, M_COLOR_RED);
				MgraText(M_DEFAULT, *m_MilManage.GetMilOverlay(MAIN_DISP), rect.GetLeft() + 20, rect.GetTop() + 20, MIL_TEXT("Error : no pattern!"));
				temp.Format("Error : no pattern!");
				WriteLog(temp, TYPE_LOG_MAIN);
				m_Rslt.m_bAlignInsp[j] = FALSE;
				rslt = FALSE;
			}
		}
		else
		{
			MgraControl(M_DEFAULT, M_BACKGROUND_MODE, M_TRANSPARENT);
			MgraColor(M_DEFAULT, M_COLOR_RED);
			MgraText(M_DEFAULT, *m_MilManage.GetMilOverlay(MAIN_DISP), rect.GetLeft() + 20, rect.GetTop() + 20, MIL_TEXT("Error : Align Search fail!"));
			temp.Format("Error : Align Search fail!");
			WriteLog(temp, TYPE_LOG_MAIN);
			m_Rslt.m_bAlignInsp[j] = FALSE;
			rslt = FALSE;
		}

		if(!m_Rslt.m_bAlignInsp[j])
		{
			m_InsRsltPara[isGrabComp].dCentX[m_InsRsltPara[isGrabComp].nNgCnt] = rect.GetCenter().GetX();
			m_InsRsltPara[isGrabComp].dCentY[m_InsRsltPara[isGrabComp].nNgCnt] = rect.GetCenter().GetY();
			m_InsRsltPara[isGrabComp].dWidth[m_InsRsltPara[isGrabComp].nNgCnt] = 50;
			m_InsRsltPara[isGrabComp].dHeight[m_InsRsltPara[isGrabComp].nNgCnt] = 50;
			m_InsRsltPara[isGrabComp].dLong[m_InsRsltPara[isGrabComp].nNgCnt] = 50;
			m_InsRsltPara[isGrabComp].dShort[m_InsRsltPara[isGrabComp].nNgCnt] = 50;
			m_InsRsltPara[isGrabComp].dBreadth[m_InsRsltPara[isGrabComp].nNgCnt] = 50;
			m_InsRsltPara[isGrabComp].dAng[m_InsRsltPara[isGrabComp].nNgCnt] = 0;//20161017 ngh

			m_InsRsltPara[isGrabComp].nDeviceIdx[m_InsRsltPara[isGrabComp].nNgCnt].SetPoint(nDeviceIdxX, (nDeviceIdxY + j));
			m_InsRsltPara[isGrabComp].nGrabIdx[m_InsRsltPara[isGrabComp].nNgCnt] = nGrabIdx;
			m_InsRsltPara[isGrabComp].nInspGrabIdx[m_InsRsltPara[isGrabComp].nNgCnt] = nInspGrabIdx;

			m_InsRsltPara[isGrabComp].bNg[m_InsRsltPara[isGrabComp].nNgCnt] = TRUE;
			m_InsRsltPara[isGrabComp].bAlign[m_InsRsltPara[isGrabComp].nNgCnt] = TRUE;

			m_InsRsltPara[isGrabComp].nNgCnt++;
		}
	}



	m_dlgTeach->SetTeachPara(teachPara);

	MbufFree(MilTransImage);//20161121
	MbufFree(image);
#endif
	return rslt;
}

BOOL CLED_DisplayInspectDlg::InspectionTeach(int nGrabIdx, int nInspGrabIdx)
{
	BOOL rslt = TRUE;
#ifdef CAMERA
	typeTeachPara teachPara;
	int ins_idx = nGrabIdx, idx = 2;
	int ins_grab_idx = nInspGrabIdx;
	MIL_ID	image, MilMaskImage, MilTransImage, MilDilate, MilBinImage, MilChildImage, MilBlobResult, MilBlobFeatureList;
	MIL_INT TotalBlobs;
	CString tmpPath_Name, strTeachPath;
	BYTE *by_source;
	long pitch;
	int cicle = 0;

	teachPara = m_dlgTeach->GetTeachPara();

	strTeachPath.Format(_T("%s\\TeachImg\\"), m_SelectModel.cPath);
	_MakeDirectories(strTeachPath);

	MdispControl(*m_MilManage.GetMilDisplay(MAIN_DISP), M_OVERLAY_CLEAR, M_DEFAULT);

	MbufAlloc2d(*m_MilManage.GetMilSystem(), CAMERA_WIDTH, CAMERA_HEIGHT, 8+M_UNSIGNED, M_IMAGE+M_PROC, &image);
	MbufCopy(*m_MilManage.GetMilDispBuf(MAIN_DISP), image);	

	MbufAlloc2d(*m_MilManage.GetMilSystem(), CAMERA_WIDTH, CAMERA_HEIGHT, 8+M_UNSIGNED, M_IMAGE+M_PROC, &MilTransImage);	
	MbufClear(MilTransImage, 0x00);

	MbufAlloc2d(*m_MilManage.GetMilSystem(), CAMERA_WIDTH, CAMERA_HEIGHT, 8+M_UNSIGNED, M_IMAGE+M_PROC, &MilDilate);
	MbufClear(MilDilate, 0x00);

	MbufAlloc2d(*m_MilManage.GetMilSystem(), CAMERA_WIDTH, CAMERA_HEIGHT, 8+M_UNSIGNED, M_IMAGE+M_PROC, &MilMaskImage);
	MbufInquire(MilMaskImage, M_HOST_ADDRESS, &by_source);
	MbufInquire(MilMaskImage, M_PITCH, &pitch);

	m_RsltIdx = 0;

	//Inspection Teach
	for(int i = 0; i < teachPara.TeachRegionCnt[ins_idx][ins_grab_idx]; i++)
	{
		idx  = (i * 3) + 2;
		CRect rect((int)teachPara.TeachRegion[ins_idx][ins_grab_idx][idx].GetLeft(),
			(int)teachPara.TeachRegion[ins_idx][ins_grab_idx][idx].GetTop(),
			(int)teachPara.TeachRegion[ins_idx][ins_grab_idx][idx].GetRight(),
			(int)teachPara.TeachRegion[ins_idx][ins_grab_idx][idx].GetBottom());
		MbufClear(MilTransImage, 0x00);

		MbufAlloc2d(*m_MilManage.GetMilSystem(), rect.Width(), rect.Height(), 8+M_UNSIGNED, M_IMAGE+M_PROC, &MilBinImage);
		MbufClear(MilBinImage, 0x00);

		MbufChild2d(image, rect.left, rect.top, rect.Width(), rect.Height(), &MilChildImage);

		MimBinarize(MilChildImage, MilBinImage, M_GREATER_OR_EQUAL, teachPara.TeachThreshHold[ins_grab_idx], M_NULL);

		////Transform
		MimTranslate(MilBinImage, MilTransImage, rect.left, rect.top, M_DEFAULT);

		MblobReconstruct(MilTransImage, M_NULL, MilTransImage, M_FILL_HOLES, M_BINARY);

		/* Allocate a blob result buffer. */
		MblobAllocResult(*m_MilManage.GetMilSystem(), &MilBlobResult); 

		/* Allocate a feature list. */ 
		MblobAllocFeatureList(*m_MilManage.GetMilSystem(), &MilBlobFeatureList); 

		MblobSelectFeature(MilBlobFeatureList, M_AREA); 
		MblobSelectFeature(MilBlobFeatureList, M_CENTER_OF_GRAVITY); 
		MblobSelectFeature(MilBlobFeatureList, M_FERET_X); 
		MblobSelectFeature(MilBlobFeatureList, M_FERET_Y);
		MblobSelectFeature(MilBlobFeatureList, M_BOX_X_MAX);
		MblobSelectFeature(MilBlobFeatureList, M_BOX_Y_MAX);
		MblobSelectFeature(MilBlobFeatureList, M_BOX_X_MIN);
		MblobSelectFeature(MilBlobFeatureList, M_BOX_Y_MIN);

		//MimDilate(MilTransImage, MilDilate, DILATE_PIXEL - 3, M_BINARY);
		if(m_dlgTeach->GetEdgeInsp())
		{
			MimErode(MilTransImage, MilDilate, DILATE_PIXEL + 1, M_BINARY);
			cicle = 2;
		}
		else
		{
			//20161122 NHT Mask사이즈 조절
			if(m_dlgTeach->GetAlignSize(nInspGrabIdx) != 0)
			{
				MimErode(MilTransImage, MilDilate, DILATE_PIXEL + m_dlgTeach->GetAlignSize(nInspGrabIdx), M_BINARY);
				cicle = 2;
			}
			else if(m_dlgTeach->GetAlignSize(nInspGrabIdx) == 0)
			{
				MimErode(MilTransImage, MilDilate, DILATE_PIXEL, M_BINARY);
				cicle = 2;
			}
		}

#ifdef INSPECTION_MASK_TEST
		for(int j = 0; j < cicle; j++)
		{
			MblobCalculate(MilDilate, M_NULL, MilBlobFeatureList, MilBlobResult); 
			if(j == 0)
			{
				MblobSelect(MilBlobResult, M_EXCLUDE, M_AREA, M_LESS_OR_EQUAL, teachPara.TeachCircleArea[nInspGrabIdx], M_NULL);
			}
			else
			{
				MblobSelect(MilBlobResult, M_EXCLUDE, M_AREA, M_GREATER, teachPara.TeachCircleArea[nInspGrabIdx], M_NULL);
			}

#else
		{
			int j = 0;
#endif
			MblobGetNumber(MilBlobResult, &TotalBlobs); 

			MbufClear(MilMaskImage, 0xFF);
			MgraColor(M_DEFAULT, M_COLOR_BLACK);

			MblobDraw(M_DEFAULT, MilBlobResult, MilMaskImage, M_DRAW_BLOBS , M_INCLUDED_BLOBS, M_DEFAULT);
			//MimDilate(MilMaskImage, MilMaskImage, DILATE_PIXEL, M_BINARY);
			if(m_dlgTeach->GetEdgeInsp())
			{
				tmpPath_Name.Format(_T("%sMask_%d_%d_%d_%d.bmp"), strTeachPath, ins_idx, nInspGrabIdx, i, j);
			}
			else
			{
				tmpPath_Name.Format(_T("%sBlobMask_%d_%d_%d_%d.bmp"), strTeachPath, ins_idx, nInspGrabIdx, i, j);
			}
			MbufExport((char *)(LPCTSTR)tmpPath_Name, M_BMP, MilMaskImage);

			for(int h = 0; h < CAMERA_HEIGHT; h++)
			{
				for(int w = 0; w < CAMERA_WIDTH; w++)
				{
					if(by_source[w + (h * pitch)] == 0)
					{
						by_source[w + (h * pitch)] = 255;
					}
					else
					{
						by_source[w + (h * pitch)] = 0;
					}
				}
			}

			MblobCalculate(MilMaskImage, M_NULL, MilBlobFeatureList, MilBlobResult); 
			MgraColor(M_DEFAULT, M_COLOR_GREEN);
			MblobDraw(M_DEFAULT, MilBlobResult, *m_MilManage.GetMilOverlay(MAIN_DISP), M_DRAW_BLOBS , M_INCLUDED_BLOBS, M_DEFAULT);
		}

		MblobFree(MilBlobResult); 
		MblobFree(MilBlobFeatureList); 
		MbufFree(MilChildImage);
		MbufFree(MilBinImage);
	}

	MbufFree(MilDilate);
	MbufFree(MilMaskImage);
	MbufFree(MilTransImage);
	MbufFree(image);
#endif
	return rslt;
}

int CLED_DisplayInspectDlg::Inspection(int nDeviceIdxX, int nDeviceIdxY, int nGrabIdx, int nInspGrabIdx, int ins_type, BOOL isGrabComp)
{
	int rslt = INSPECTION_OK;
	BOOL bRslt = FALSE;
	int nRslt = 0;
	switch(ins_type)
	{
	case INSPECTION_ALIGN_TEACH:
		bRslt = InspectionAlignTeaching(nDeviceIdxX, nDeviceIdxY, nGrabIdx, nInspGrabIdx);
		rslt = bRslt;
		break;
	case INSPECTION_TEACH:
		bRslt = InspectionTeach(nGrabIdx, nInspGrabIdx);
		rslt = bRslt;
		break;
	case INSPECTION_ALIGN:
		bRslt = InspectionAlign(nDeviceIdxX, nDeviceIdxY, nGrabIdx, nInspGrabIdx, isGrabComp);
		if(bRslt == 0)
		{
			rslt = ALIGN_NG;
		}
		break;
	case INSPECTION_EDGE:
		nRslt = InspectionEdge(nDeviceIdxX, nDeviceIdxY, nGrabIdx, nInspGrabIdx, isGrabComp);
		if(nRslt == NON_TEACHING_IAMGE)
		{
			rslt = NON_TEACHING_IAMGE;
		}
		else if(nRslt == INSPECTION_NG)
		{
			rslt = INSPECTION_NG;
		}
		break;
	case INSPECTION_MASK_SET:
		bRslt = InspectionMaskSet(nDeviceIdxX, nDeviceIdxY, nGrabIdx, nInspGrabIdx, isGrabComp);
		rslt = bRslt;
		break;
	case INSPECTION_EDGE2:
		nRslt = InspectionEdge2(nDeviceIdxX, nDeviceIdxY, nGrabIdx, nInspGrabIdx, isGrabComp);
		break;
	case INSPECTION_EDGE3:
		nRslt = InspectionEdge3(nDeviceIdxX, nDeviceIdxY, nGrabIdx, nInspGrabIdx, isGrabComp);
		break;
	case INSPECTION_BLOB:
		nRslt = InspectionBlobs(nDeviceIdxX, nDeviceIdxY, nGrabIdx, nInspGrabIdx, isGrabComp);
		break;
	case INSPECTION_BLOB2:
		nRslt = InspectionBlobs2(nDeviceIdxX, nDeviceIdxY, nGrabIdx, nInspGrabIdx, isGrabComp);
		break;
	case INSPECTION_BLOB3:
		nRslt = InspectionBlobs3(nDeviceIdxX, nDeviceIdxY, nGrabIdx, nInspGrabIdx, isGrabComp);
		break;
	case INSPECTION_TEST:
		nRslt = InspectionTest(nDeviceIdxX, nDeviceIdxY, nGrabIdx, nInspGrabIdx, isGrabComp);
		break;
	}

	//20161024 ngh
	if(ins_type > INSPECTION_MASK_SET)
	{
		if(nRslt == NON_TEACHING_IAMGE)
		{
			rslt = NON_TEACHING_IAMGE;
		}
		else if(nRslt == INSPECTION_NG)
		{
			rslt = INSPECTION_NG;
		}
	}

	return rslt;
}

BOOL CLED_DisplayInspectDlg::InspectionMaskSet(int nDeviceIdxX, int nDeviceIdx, int nGrabIdx, int nInspGrabIdx, BOOL isGrabComp)
{
	BOOL rslt = TRUE;
	CString strLog;
	strLog.Format(_T("InspectionSetMask Start"));
	WriteLog(strLog, TYPE_LOG_MAIN, FALSE);
#ifdef CAMERA
	typeTeachPara teachPara;
	int ins_idx = nGrabIdx, idx = 2, align_idx;
	int ins_grab_idx = nInspGrabIdx;
	MIL_ID	image, MilMaskImage, MilTransImage, MilBinImage, MilChildImage, MilBlobResult, MilBlobFeatureList;
	MIL_INT TotalBlobs;
	CString tmpPath_Name, strTeachPath;
	BYTE *by_source;
	long pitch;
	double align_dist_x, align_dist_y;

	teachPara = m_dlgTeach->GetTeachPara();

	strTeachPath.Format(_T("%s\\TeachImg\\"), m_SelectModel.cPath);
	_MakeDirectories(strTeachPath);

	MbufAlloc2d(*m_MilManage.GetMilSystem(), CAMERA_WIDTH, CAMERA_HEIGHT, 8+M_UNSIGNED, M_IMAGE+M_PROC, &image);
	MbufAlloc2d(*m_MilManage.GetMilSystem(), CAMERA_WIDTH, CAMERA_HEIGHT, 8+M_UNSIGNED, M_IMAGE+M_PROC, &MilMaskImage);
	MbufInquire(MilMaskImage, M_HOST_ADDRESS, &by_source);
	MbufInquire(MilMaskImage, M_PITCH, &pitch);

	MbufAlloc2d(*m_MilManage.GetMilSystem(), CAMERA_WIDTH, CAMERA_HEIGHT, 8+M_UNSIGNED, M_IMAGE+M_PROC, &MilTransImage);	

	m_RsltIdx = 0;

	//Inspection Teach
	for(int i = 0; i < teachPara.TeachRegionCnt[ins_idx][ins_grab_idx]; i++)
	{
		if(!m_Rslt.m_bAlignInsp[i])
		{
			continue;
		}
		idx  = (i * 3) + 2;
		align_idx = 1 + (i * 3); 

		double temp_x, temp_y;

		temp_x = teachPara.AlignPara.dCentX[i + (m_dlgTeach->GetTeachPara().TeachGrabCnt * nGrabIdx)] - teachPara.AlignPara.dX[align_idx + nGrabIdx];
		temp_y = teachPara.AlignPara.dCentY[i + (m_dlgTeach->GetTeachPara().TeachGrabCnt * nGrabIdx)] - teachPara.AlignPara.dY[align_idx + nGrabIdx];
		//MimRotate(MilMaskImage, MilAlignImage, teachPara.AlignPara.dAng[i], teachPara.AlignPara.dX[i], teachPara.AlignPara.dY[i],
		//																	teachPara.TeachRegion[ins_idx][ins_grab_idx][align_idx].GetCenter().GetX(),
		//																	teachPara.TeachRegion[ins_idx][ins_grab_idx][align_idx].GetCenter().GetY(),
		//																	M_DEFAULT);
		//MimRotate(*m_MilManage.GetMilInspBuf(isGrabComp, nInspGrabIdx), image, teachPara.AlignPara.dAng[align_idx], teachPara.AlignPara.dCentX[i + (m_dlgTeach->GetTeachPara().TeachGrabCnt * nGrabIdx)],
		//																	teachPara.AlignPara.dCentY[i + (m_dlgTeach->GetTeachPara().TeachGrabCnt * nGrabIdx)],
		//																	teachPara.AlignPara.dX[align_idx + nGrabIdx], teachPara.AlignPara.dY[align_idx + nGrabIdx], M_DEFAULT);
		MbufCopy(*m_MilManage.GetMilInspBuf(isGrabComp, nInspGrabIdx), image);

		align_dist_x = teachPara.AlignPara.dCentX[i + (m_dlgTeach->GetTeachPara().TeachGrabCnt * nGrabIdx)] - teachPara.AlignPara.dX[align_idx + nGrabIdx];
		align_dist_y = teachPara.AlignPara.dCentY[i + (m_dlgTeach->GetTeachPara().TeachGrabCnt * nGrabIdx)] - teachPara.AlignPara.dY[align_idx + nGrabIdx];

		CRect rect((int)(teachPara.TeachRegion[ins_idx][ins_grab_idx][idx].GetLeft() - align_dist_x),
			(int)(teachPara.TeachRegion[ins_idx][ins_grab_idx][idx].GetTop() - align_dist_y),
			(int)(teachPara.TeachRegion[ins_idx][ins_grab_idx][idx].GetRight() - align_dist_x),
			(int)(teachPara.TeachRegion[ins_idx][ins_grab_idx][idx].GetBottom() - align_dist_y));
		MbufClear(MilTransImage, 0x00);

		MgraColor(M_DEFAULT, M_COLOR_RED);

		MgraLine(M_DEFAULT, *m_MilManage.GetMilOverlay(MAIN_DISP), rect.left, rect.top, rect.right, rect.top);
		MgraLine(M_DEFAULT, *m_MilManage.GetMilOverlay(MAIN_DISP), rect.left, rect.bottom, rect.right, rect.bottom);
		MgraLine(M_DEFAULT, *m_MilManage.GetMilOverlay(MAIN_DISP), rect.left, rect.top, rect.left, rect.bottom);
		MgraLine(M_DEFAULT, *m_MilManage.GetMilOverlay(MAIN_DISP), rect.right, rect.top, rect.right, rect.bottom);
		//MblobDraw(M_DEFAULT, MilBlobResult, *m_MilManage.GetMilOverlay(MAIN_DISP), M_DRAW_BLOBS_CONTOUR , M_INCLUDED_BLOBS, M_DEFAULT);

		MbufAlloc2d(*m_MilManage.GetMilSystem(), rect.Width(), rect.Height(), 8+M_UNSIGNED, M_IMAGE+M_PROC, &MilBinImage);
		MbufClear(MilBinImage, 0x00);

		MbufChild2d(image, rect.left, rect.top, rect.Width(), rect.Height(), &MilChildImage);

		MimBinarize(MilChildImage, MilBinImage, M_GREATER_OR_EQUAL, teachPara.TeachThreshHold[ins_grab_idx], M_NULL);

		////Transform
		MimTranslate(MilBinImage, MilTransImage, rect.left, rect.top, M_DEFAULT);

		MblobReconstruct(MilTransImage, M_NULL, MilTransImage, M_FILL_HOLES, M_BINARY);

		/* Allocate a blob result buffer. */
		MblobAllocResult(*m_MilManage.GetMilSystem(), &MilBlobResult); 

		/* Allocate a feature list. */ 
		MblobAllocFeatureList(*m_MilManage.GetMilSystem(), &MilBlobFeatureList); 

		MblobSelectFeature(MilBlobFeatureList, M_AREA); 
		MblobSelectFeature(MilBlobFeatureList, M_CENTER_OF_GRAVITY); 
		MblobSelectFeature(MilBlobFeatureList, M_FERET_X); 
		MblobSelectFeature(MilBlobFeatureList, M_FERET_Y);
		MblobSelectFeature(MilBlobFeatureList, M_BOX_X_MAX);
		MblobSelectFeature(MilBlobFeatureList, M_BOX_Y_MAX);
		MblobSelectFeature(MilBlobFeatureList, M_BOX_X_MIN);
		MblobSelectFeature(MilBlobFeatureList, M_BOX_Y_MIN);

#ifdef INSPECTION_MASK_TEST
		for(int j = 0; j < 2; j++)
		{
			MblobCalculate(MilTransImage, M_NULL, MilBlobFeatureList, MilBlobResult); 
			if(j == 0)
			{
				MblobSelect(MilBlobResult, M_EXCLUDE, M_AREA, M_LESS_OR_EQUAL, teachPara.TeachCircleArea[nInspGrabIdx], M_NULL);
			}
			else
			{
				MblobSelect(MilBlobResult, M_EXCLUDE, M_AREA, M_GREATER, teachPara.TeachCircleArea[nInspGrabIdx], M_NULL);
			}

#else
		{
			int j = 0;
#endif
			MblobGetNumber(MilBlobResult, &TotalBlobs); 

			MbufClear(MilMaskImage, 0xFF);
			MgraColor(M_DEFAULT, M_COLOR_BLACK);

			MblobDraw(M_DEFAULT, MilBlobResult, MilMaskImage, M_DRAW_BLOBS , M_INCLUDED_BLOBS, M_DEFAULT);
			//MimDilate(MilMaskImage, MilMaskImage, DILATE_PIXEL, M_BINARY);
			tmpPath_Name.Format(_T("%sSetMask_%d_%d_%d_%d.bmp"), strTeachPath, ins_idx, ins_grab_idx - 1, i, j);
			MbufExport((char *)(LPCTSTR)tmpPath_Name, M_BMP, MilMaskImage);

			for(int h = 0; h < CAMERA_HEIGHT; h++)
			{
				for(int w = 0; w < CAMERA_WIDTH; w++)
				{
					if(by_source[w + (h * pitch)] == 0)
					{
						by_source[w + (h * pitch)] = 255;
					}
					else
					{
						by_source[w + (h * pitch)] = 0;
					}
				}
			}

			MblobCalculate(MilMaskImage, M_NULL, MilBlobFeatureList, MilBlobResult); 
			MgraColor(M_DEFAULT, M_COLOR_RED);
			//MblobDraw(M_DEFAULT, MilBlobResult, *m_MilManage.GetMilOverlay(MAIN_DISP), M_DRAW_BLOBS_CONTOUR , M_INCLUDED_BLOBS, M_DEFAULT);
		}

		MblobFree(MilBlobResult); 
		MblobFree(MilBlobFeatureList); 
		MbufFree(MilChildImage);
		MbufFree(MilBinImage);
	}

	//MbufFree(MilDilate);
	MbufFree(MilMaskImage);
	MbufFree(MilTransImage);
	MbufFree(image);
#endif

	strLog.Format(_T("InspectionSetMask End"));
	WriteLog(strLog, TYPE_LOG_MAIN, FALSE);

	return rslt;
}

BOOL CLED_DisplayInspectDlg::EdgeCompare(int nDeviceIdxX, int nDeviceIdxY, int nGrabIdx, double cent_x, double cent_y, double width, double height, double distance)
{
	BOOL rslt = FALSE;
	CdRect ng_rect1, ng_rect2, ng_rect3;

	ng_rect1.SetdRectCWH(cent_x, cent_y, width, height, distance);

	for(int i = 0; i < m_InsRsltPara[m_Rslt.m_bGrabComp].nNgCnt; i++)
	{
		//for(int j = 0; j < m_dlgTeach->GetTeachPara().TeachOneGrabInspCnt; j++)
		{
			if((m_InsRsltPara[m_Rslt.m_bGrabComp].nDeviceIdx[i].x == nDeviceIdxX) && (m_InsRsltPara[m_Rslt.m_bGrabComp].nDeviceIdx[i].y == nDeviceIdxY))//(j + (nDeviceIdxY * m_dlgTeach->GetTeachPara().TeachOneGrabInspCnt))))//nDeviceIdxY + j + 1))
			{
				if((m_InsRsltPara[m_Rslt.m_bGrabComp].nGrabIdx[i] == nGrabIdx))
				{
					ng_rect2.SetdRectCWH(m_InsRsltPara[m_Rslt.m_bGrabComp].dCentX[i], m_InsRsltPara[m_Rslt.m_bGrabComp].dCentY[i], m_InsRsltPara[m_Rslt.m_bGrabComp].dWidth[i], m_InsRsltPara[m_Rslt.m_bGrabComp].dHeight[i], distance);
					if(ng_rect2.IntersectRect(ng_rect1) || ng_rect1.IntersectRect(ng_rect2))
					{
						rslt = TRUE;
						break;
					}
				}
			}
		}
		if(rslt)
		{
			break;
		}
	}

	return rslt;
}

int CLED_DisplayInspectDlg::InspectionEdge3(int nDeviceIdxX, int nDeviceIdxY, int nGrabIdx, int nInspGrabIdx, BOOL isGrabComp)
{
	int nNG = 0;
#ifdef CAMERA
	MIL_ID image;
	MIL_ID MilMaskImage, MilMaskImage2, MilMaskImage3;
	MIL_ID MilEdgeContext, MilEdgeResult;
	MIL_ID MilAlignImage;

	MIL_ID MilTransImage, MilBinImage, MilChildImage, MilBlobResult, MilBlobFeatureList;
	MIL_INT tot_blob;
	long NumEdgeFound;
	CString strTeachPath;
	typeTeachPara teachPara;

	int idx = 2;
	int ins_idx = nGrabIdx;
	int ins_grab_idx = nInspGrabIdx;
	long NumResults = 0;
	double high_thresh, low_thresh;
	CString tmpPath_Name, image_file_name;
	SYSTEMTIME	lpSystemTime;
	GetLocalTime(&lpSystemTime);

	double *CogX, *CogY, *Width, *Height, *Major, *Minor, *Ang;//20161017 ngh
	double align_dist_x, align_dist_y;//201601017 ngh

	teachPara = m_dlgTeach->GetTeachPara();
	double spec = (double)teachPara.TeachSpec / (double)teachPara.TeachPixelSize;
	m_RsltIdx = 0;
	double fun_time_s, fun_time_e, fun_time;
	CString strLog;

	//InitInsRsltPara(isGrabComp);
	strLog.Format(_T("InspectionEdge1 Start"));
	//WriteLog(strLog, TYPE_LOG_MAIN);

	strTeachPath.Format(_T("%s\\TeachImg\\"), m_SelectModel.cPath);

	//MdispControl(*m_MilManage.GetMilDisplay(MAIN_DISP), M_OVERLAY_CLEAR, M_DEFAULT);

	MbufAlloc2d(*m_MilManage.GetMilSystem(), CAMERA_WIDTH, CAMERA_HEIGHT, 8+M_UNSIGNED, M_IMAGE+M_PROC, &image);
	MbufAlloc2d(*m_MilManage.GetMilSystem(), CAMERA_WIDTH, CAMERA_HEIGHT, 8+M_UNSIGNED, M_IMAGE+M_PROC, &MilTransImage);
	MbufAlloc2d(*m_MilManage.GetMilSystem(), CAMERA_WIDTH, CAMERA_HEIGHT, 8+M_UNSIGNED, M_IMAGE+M_PROC, &MilMaskImage);
	MbufAlloc2d(*m_MilManage.GetMilSystem(), CAMERA_WIDTH, CAMERA_HEIGHT, 8+M_UNSIGNED, M_IMAGE+M_PROC, &MilMaskImage2);
	MbufAlloc2d(*m_MilManage.GetMilSystem(), CAMERA_WIDTH, CAMERA_HEIGHT, 8+M_UNSIGNED, M_IMAGE+M_PROC, &MilMaskImage3);
	MbufAlloc2d(*m_MilManage.GetMilSystem(), CAMERA_WIDTH, CAMERA_HEIGHT, 8+M_UNSIGNED, M_IMAGE+M_PROC, &MilAlignImage);

	MbufClear(MilAlignImage, 0xFF);
	int align_idx = 0;
	//Inspection
	for(int i = 0; i < teachPara.TeachRegionCnt[ins_idx][ins_grab_idx]; i++)
	{
		if(nDeviceIdxY + i == teachPara.TeachArrayCntY)
		{
			m_Rslt.m_bAlignInsp[i] = FALSE;
			nNG = INSPECTION_NG;
			m_Rslt.m_bInspectionRslt[0][i] = TRUE;
			m_Rslt.m_bInspectionRslt[1][i] = TRUE;
			continue;
		}
		if(!m_Rslt.m_bAlignInsp[i])
		{
			nNG = INSPECTION_NG;
			m_Rslt.m_bInspectionRslt[0][i] = TRUE;
			m_Rslt.m_bInspectionRslt[1][i] = TRUE;
			continue;
		}
#ifdef TEST
		strLog.Format(_T("InspectionEdge Start_%d"), i);
		WriteLog(strLog, TYPE_LOG_MAIN);
#endif
		idx  = (i * 3) + 2;
		align_idx = (i * 3) + 1;
		CRect ng_rect;

		int k = 0;//20161017 ngh
#ifdef INSPECTION_MASK_TEST
		for(k = 0; k < 2; k++)
		{
			tmpPath_Name.Format(_T("Mask_%d_%d_%d.bmp"), 0, i, k);
#else
		tmpPath_Name.Format(_T("Mask_%d_%d_0.bmp"), 0, i);
		{
#endif
			if(!FileSearch(strTeachPath, tmpPath_Name))
			{
				MgraColor(M_DEFAULT, M_COLOR_RED);
				strLog.Format(_T("Not Model Teaching"));
				CdRect rect;
				rect.SetdRectLTRB((int)teachPara.TeachRegion[ins_idx][ins_grab_idx][idx].GetLeft(),
					(int)teachPara.TeachRegion[ins_idx][ins_grab_idx][idx].GetTop(),
					(int)teachPara.TeachRegion[ins_idx][ins_grab_idx][idx].GetRight(),
					(int)teachPara.TeachRegion[ins_idx][ins_grab_idx][idx].GetBottom());
				MgraText(M_DEFAULT, *m_MilManage.GetMilOverlay(MAIN_DISP), rect.GetLeft() + 20, rect.GetTop() + 20, MIL_TEXT((char *)(LPCTSTR)strLog));
				strLog.Format(_T("Edge3,MaskImageSearchFail_%d"), i);
				WriteLog(strLog, TYPE_LOG_MAIN);
				continue;
			}

			MbufClear(MilMaskImage, 0xFF);
			MbufClear(MilMaskImage2, 0xFF);
			MbufClear(MilMaskImage3, 0xFF);

#ifdef INSPECTION_MASK_TEST
			tmpPath_Name.Format(_T("%sMask_%d_%d_%d.bmp"), strTeachPath, 0, i, k);
#else
			tmpPath_Name.Format(_T("%sMask_%d_%d_0.bmp"), strTeachPath, 0, i);
#endif

			MbufImport((char *)(LPCTSTR)tmpPath_Name, M_BMP, M_LOAD, *m_MilManage.GetMilSystem(), &MilMaskImage2);	

			double temp_x, temp_y;

			temp_x = teachPara.AlignPara.dCentX[i + (m_dlgTeach->GetTeachPara().TeachGrabCnt * nGrabIdx)] - teachPara.AlignPara.dX[align_idx + nGrabIdx];
			temp_y = teachPara.AlignPara.dCentY[i + (m_dlgTeach->GetTeachPara().TeachGrabCnt * nGrabIdx)] - teachPara.AlignPara.dY[align_idx + nGrabIdx];

			fun_time_s = clock();
			//MimRotate(MilMaskImage, MilAlignImage, teachPara.AlignPara.dAng[i], teachPara.AlignPara.dX[i], teachPara.AlignPara.dY[i],
			//																	teachPara.TeachRegion[ins_idx][ins_grab_idx][align_idx].GetCenter().GetX(),
			//																	teachPara.TeachRegion[ins_idx][ins_grab_idx][align_idx].GetCenter().GetY(),
			//																	M_DEFAULT);
			MimRotate(MilMaskImage2, MilAlignImage, teachPara.AlignPara.dAng[align_idx], teachPara.AlignPara.dCentX[i + (m_dlgTeach->GetTeachPara().TeachGrabCnt * nGrabIdx)],
				teachPara.AlignPara.dCentY[i + (m_dlgTeach->GetTeachPara().TeachGrabCnt * nGrabIdx)],
				teachPara.AlignPara.dX[align_idx + nGrabIdx], teachPara.AlignPara.dY[align_idx + nGrabIdx], M_DEFAULT);
			fun_time_e = clock();
			fun_time = (fun_time_e - fun_time_s) / 1000.0;
#ifdef TEST
			strLog.Format(_T("MimRotate() %f"), fun_time);
			WriteLog(strLog, TYPE_LOG_MAIN);


			tmpPath_Name.Format(_T("%sMilAlignImage_%d_%d_0.bmp"), strTeachPath, nGrabIdx, nInspGrabIdx);
			MbufExport((char *)(LPCTSTR)tmpPath_Name, M_BMP, MilAlignImage);
#endif
			if(k == 0)
			{
				MimErode(MilAlignImage, MilMaskImage2, DILATE_PIXEL - 1, M_BINARY);
			}
			else
			{
				//MimErode(MilAlignImage, MilMaskImage2, DILATE_PIXEL - 1 + teachPara.TeachCircleErodeDistance[nInspGrabIdx], M_BINARY);
				MimDilate(MilMaskImage3, MilMaskImage3, teachPara.TeachCircleErodeDistance[nInspGrabIdx], M_BINARY);
			}
			//MbufCopy(MilAlignImage, MilMaskImage);

#ifdef INSPECTION_MASK_TEST
			tmpPath_Name.Format(_T("SetMask_%d_%d_%d_%d.bmp"), ins_idx, ins_grab_idx - 1, i, k);
#else
			tmpPath_Name.Format(_T("SetMask_%d_%d_%d_0.bmp"), ins_idx, ins_grab_idx - 1, i);
#endif
			if(!FileSearch(strTeachPath, tmpPath_Name))
			{
				MgraColor(M_DEFAULT, M_COLOR_RED);
				strLog.Format(_T("Not Model Teaching"));
				CdRect rect;
				rect.SetdRectLTRB((int)teachPara.TeachRegion[ins_idx][ins_grab_idx][idx].GetLeft(),
					(int)teachPara.TeachRegion[ins_idx][ins_grab_idx][idx].GetTop(),
					(int)teachPara.TeachRegion[ins_idx][ins_grab_idx][idx].GetRight(),
					(int)teachPara.TeachRegion[ins_idx][ins_grab_idx][idx].GetBottom());
				MgraText(M_DEFAULT, *m_MilManage.GetMilOverlay(MAIN_DISP), rect.GetLeft() + 20, rect.GetTop() + 20, MIL_TEXT((char *)(LPCTSTR)strLog));
				strLog.Format(_T("Edge3,SetMaskImageSearchFail_%d"), i);
				WriteLog(strLog, TYPE_LOG_MAIN);
				continue;
			}

			MbufClear(MilMaskImage, 0xFF);
#ifdef INSPECTION_MASK_TEST
			tmpPath_Name.Format(_T("%sSetMask_%d_%d_%d_%d.bmp"), strTeachPath, ins_idx, ins_grab_idx - 1, i, k);
#else
			tmpPath_Name.Format(_T("%sSetMask_%d_%d_%d_0.bmp"), strTeachPath, ins_idx, ins_grab_idx - 1, i);
#endif
			MbufImport((char *)(LPCTSTR)tmpPath_Name, M_BMP, M_LOAD, *m_MilManage.GetMilSystem(), &MilMaskImage3);	

			if(k == 1)
			{
				MimDilate(MilMaskImage3, MilMaskImage3, teachPara.TeachCircleErodeDistance[nInspGrabIdx], M_BINARY);
			}

			//image and cal
			MimArith(MilMaskImage2, MilMaskImage3, MilMaskImage, M_AND);

			if(k == 0)
			{
				MimDilate(MilMaskImage, MilMaskImage, 1, M_BINARY);
			}

#ifdef TEST
			tmpPath_Name.Format(_T("%sMilMaskImage%d_%d_%d.bmp"), strTeachPath, nGrabIdx, nInspGrabIdx, i);
			MbufExport((char *)(LPCTSTR)tmpPath_Name, M_BMP, MilMaskImage);
#endif

			//20161017 ngh
			align_dist_x = teachPara.AlignPara.dCentX[i + (m_dlgTeach->GetTeachPara().TeachGrabCnt * nGrabIdx)] - teachPara.AlignPara.dX[align_idx + nGrabIdx];
			align_dist_y = teachPara.AlignPara.dCentY[i + (m_dlgTeach->GetTeachPara().TeachGrabCnt * nGrabIdx)] - teachPara.AlignPara.dY[align_idx + nGrabIdx];

			CRect rect((int)(teachPara.TeachRegion[ins_idx][ins_grab_idx][idx].GetLeft() - align_dist_x),
				(int)(teachPara.TeachRegion[ins_idx][ins_grab_idx][idx].GetTop() - align_dist_y),
				(int)(teachPara.TeachRegion[ins_idx][ins_grab_idx][idx].GetRight() - align_dist_x),
				(int)(teachPara.TeachRegion[ins_idx][ins_grab_idx][idx].GetBottom() - align_dist_y));

			//CRect rect((int)teachPara.TeachRegion[ins_idx][ins_grab_idx][idx].GetLeft(),// - ((teachPara.TeachRegion[ins_idx][ins_grab_idx][align_idx].GetCenter().GetX() - teachPara.AlignPara.dX[i]) / 2),
			//			(int)teachPara.TeachRegion[ins_idx][ins_grab_idx][idx].GetTop(),// - ((teachPara.TeachRegion[ins_idx][ins_grab_idx][align_idx].GetCenter().GetY() - teachPara.AlignPara.dY[i]) / 2),
			//			(int)teachPara.TeachRegion[ins_idx][ins_grab_idx][idx].GetRight(),// + ((teachPara.TeachRegion[ins_idx][ins_grab_idx][align_idx].GetCenter().GetX() - teachPara.AlignPara.dX[i]) / 2),
			//			(int)teachPara.TeachRegion[ins_idx][ins_grab_idx][idx].GetBottom());// + ((teachPara.TeachRegion[ins_idx][ins_grab_idx][align_idx].GetCenter().GetY() - teachPara.AlignPara.dY[i]) / 2));
			MbufClear(MilTransImage, 0x00);

			MbufAlloc2d(*m_MilManage.GetMilSystem(), rect.Width(), rect.Height(), 8+M_UNSIGNED, M_IMAGE+M_PROC, &MilBinImage);
			MbufClear(MilBinImage, 0x00);

			MbufChild2d(MilMaskImage, rect.left, rect.top, rect.Width(), rect.Height(), &MilChildImage);
#ifdef TEST
			tmpPath_Name.Format(_T("%sMilChildImage_%d_%d_0.bmp"), strTeachPath, nGrabIdx, nInspGrabIdx);
			MbufExport((char *)(LPCTSTR)tmpPath_Name, M_BMP, MilChildImage);
#endif
			fun_time_s = clock();
			MimBinarize(MilChildImage, MilBinImage, M_LESS_OR_EQUAL, 100, M_NULL);
			fun_time_e = clock();
			fun_time = (fun_time_e - fun_time_s) / 1000.0;
#ifdef TEST
			strLog.Format(_T("MimBinarize() %f"), fun_time);
			WriteLog(strLog, TYPE_LOG_MAIN);

			tmpPath_Name.Format(_T("%sMilBinImage_%d_%d_0.bmp"), strTeachPath, nGrabIdx, nInspGrabIdx);
			MbufExport((char *)(LPCTSTR)tmpPath_Name, M_BMP, MilBinImage);
#endif

			////Transform
			fun_time_s = clock();
			MimTranslate(MilBinImage, MilTransImage, rect.left, rect.top, M_DEFAULT);
			fun_time_e = clock();
			fun_time = (fun_time_e - fun_time_s) / 1000.0;
#ifdef TEST
			strLog.Format(_T("MimTranslate() %f"), fun_time);
			WriteLog(strLog, TYPE_LOG_MAIN);

			tmpPath_Name.Format(_T("%sMilTransImage_%d_%d_0.bmp"), strTeachPath, nGrabIdx, nInspGrabIdx);
			MbufExport((char *)(LPCTSTR)tmpPath_Name, M_BMP, MilTransImage);
#endif

			/* Allocate a blob result buffer. */
			MblobAllocResult(*m_MilManage.GetMilSystem(), &MilBlobResult); 

			/* Allocate a feature list. */ 
			MblobAllocFeatureList(*m_MilManage.GetMilSystem(), &MilBlobFeatureList); 

			MblobSelectFeature(MilBlobFeatureList, M_BOX_X_MAX);
			MblobSelectFeature(MilBlobFeatureList, M_BOX_Y_MAX);
			MblobSelectFeature(MilBlobFeatureList, M_BOX_X_MIN);
			MblobSelectFeature(MilBlobFeatureList, M_BOX_Y_MIN);
			MblobSelectFeature(MilBlobFeatureList, M_MEAN_PIXEL);
			MblobSelectFeature(MilBlobFeatureList, M_FERET_X); 
			MblobSelectFeature(MilBlobFeatureList, M_FERET_Y); 
			MblobSelectFeature(MilBlobFeatureList, M_CENTER_OF_GRAVITY);
			MblobSelectFeature(MilBlobFeatureList, M_MEAN_PIXEL);
			MblobSelectFeature(MilBlobFeatureList, M_SUM_PIXEL);
			MblobSelectFeature(MilBlobFeatureList, M_AREA );

			/* Allocate a Edge Finder context. */
			MedgeAlloc(*m_MilManage.GetMilSystem(), M_CONTOUR, M_DEFAULT, &MilEdgeContext); 

			/* Allocate a result buffer. */
			MedgeAllocResult(*m_MilManage.GetMilSystem(), M_DEFAULT, &MilEdgeResult);

			/* Enable features to compute. */
			//MedgeControl(MilEdgeContext, M_MOMENT_ELONGATION,                M_ENABLE);
			//MedgeControl(MilEdgeContext, M_FERET_MEAN_DIAMETER+M_SORT1_DOWN, M_ENABLE);
			MedgeControl(MilEdgeContext, M_FAST_LENGTH,						 M_ENABLE);
			MedgeControl(MilEdgeContext, M_ACCURACY,						 M_DISABLE);
			MedgeControl(MilEdgeContext, M_THRESHOLD_MODE,					 M_USER_DEFINED);
			MedgeControl(MilEdgeContext, M_BOX_X_MAX, M_ENABLE);
			MedgeControl(MilEdgeContext, M_BOX_Y_MAX, M_ENABLE);
			MedgeControl(MilEdgeContext, M_BOX_X_MIN, M_ENABLE);
			MedgeControl(MilEdgeContext, M_BOX_Y_MIN, M_ENABLE);
			MedgeControl(MilEdgeContext, M_FERET_X, M_ENABLE); 
			MedgeControl(MilEdgeContext, M_FERET_Y, M_ENABLE); 
			MedgeControl(MilEdgeContext, M_CENTER_OF_GRAVITY, M_ENABLE);
			MedgeControl(MilEdgeContext, M_ELLIPSE_FIT_MAJOR_AXIS, M_ENABLE);
			MedgeControl(MilEdgeContext, M_ELLIPSE_FIT_MINOR_AXIS, M_ENABLE);
			MedgeControl(MilEdgeContext, M_ELLIPSE_FIT_ANGLE, M_ENABLE);//20161017 ngh
			MedgeControl(MilEdgeContext, M_ELLIPSE_FIT_ERROR , M_ENABLE);//20161017 ngh
			MedgeControl(MilEdgeContext, M_FILL_GAP_DISTANCE, spec);// / (double)m_dlgTeach->GetTeachPara().TeachPixelSize);

			fun_time_s = clock();
			//double temp_x, temp_y;

			MimHistogramEqualize(*m_MilManage.GetMilInspBuf(isGrabComp, 0), image, M_RAYLEIGH, 1, teachPara.TeachInspThreshHold[ins_grab_idx][0], 255);

			temp_x = teachPara.AlignPara.dCentX[i + (m_dlgTeach->GetTeachPara().TeachGrabCnt * nGrabIdx)] - teachPara.AlignPara.dX[align_idx + nGrabIdx];
			temp_y = teachPara.AlignPara.dCentY[i + (m_dlgTeach->GetTeachPara().TeachGrabCnt * nGrabIdx)] - teachPara.AlignPara.dY[align_idx + nGrabIdx];
			//MimRotate(MilMaskImage, MilAlignImage, teachPara.AlignPara.dAng[i], teachPara.AlignPara.dX[i], teachPara.AlignPara.dY[i],
			//																	teachPara.TeachRegion[ins_idx][ins_grab_idx][align_idx].GetCenter().GetX(),
			//																	teachPara.TeachRegion[ins_idx][ins_grab_idx][align_idx].GetCenter().GetY(),
			//																	M_DEFAULT);
			//MimRotate(image, image, teachPara.AlignPara.dAng[align_idx], teachPara.AlignPara.dCentX[i + (m_dlgTeach->GetTeachPara().TeachGrabCnt * nGrabIdx)],
			//																	teachPara.AlignPara.dCentY[i + (m_dlgTeach->GetTeachPara().TeachGrabCnt * nGrabIdx)],
			//																	teachPara.AlignPara.dX[align_idx + nGrabIdx], teachPara.AlignPara.dY[align_idx + nGrabIdx], M_DEFAULT);


			//MbufCopy(*m_MilManage.GetMilInspBuf(isGrabComp, nInspGrabIdx), image);
			fun_time_e = clock();
			fun_time = (fun_time_e - fun_time_s) / 1000.0;
#ifdef TEST
			strLog.Format(_T("MimHistogramEqualize() %f"), fun_time);
			WriteLog(strLog, TYPE_LOG_MAIN);

			tmpPath_Name.Format(_T("%sMimHistogramEqualize_%d_%d_0.bmp"), strTeachPath, nGrabIdx, nInspGrabIdx);
			MbufExport((char *)(LPCTSTR)tmpPath_Name, M_BMP, image);
#endif
			fun_time_s = clock();
			//MblobCalculate(MilTransImage, M_NULL, MilBlobFeatureList, MilBlobResult); 
			MblobCalculate(MilTransImage, image, MilBlobFeatureList, MilBlobResult); 
			fun_time_e = clock();
			fun_time = (fun_time_e - fun_time_s) / 1000.0;
#ifdef TEST
			strLog.Format(_T("MblobCalculate() %f"), fun_time);
			WriteLog(strLog, TYPE_LOG_MAIN);
#endif
			MblobGetNumber(MilBlobResult, &tot_blob);
			double *mean;
			mean = (double *)malloc(tot_blob*sizeof(double));

			MblobGetResult(MilBlobResult, M_MEAN_PIXEL, mean);

			free(mean);

			MgraColor(M_DEFAULT, M_COLOR_DARK_RED);
			MblobDraw(M_DEFAULT, MilBlobResult, *m_MilManage.GetMilOverlay(MAIN_DISP), M_DRAW_BLOBS_CONTOUR , M_INCLUDED_BLOBS, M_DEFAULT);

			high_thresh = teachPara.TeachInspThreshHold[ins_grab_idx][1];
			low_thresh = teachPara.TeachInspThreshHold[ins_grab_idx][2];

			MedgeControl(MilEdgeContext, M_THRESHOLD_HIGH,					 high_thresh);
			MedgeControl(MilEdgeContext, M_THRESHOLD_LOW,					 low_thresh);

			MedgeMask(MilEdgeContext, MilMaskImage, M_DEFAULT);

			fun_time_s = clock();
			/* Calculate edges and features. */
			MedgeCalculate(MilEdgeContext, image, M_NULL, M_NULL, M_NULL, MilEdgeResult, M_DEFAULT);
			fun_time_e = clock();
			fun_time = (fun_time_e - fun_time_s) / 1000.0;
#ifdef TEST
			strLog.Format(_T("MedgeCalculate() %f"), fun_time);
			WriteLog(strLog, TYPE_LOG_MAIN);
#endif
			MedgeSelect(MilEdgeResult, M_EXCLUDE, M_BOX_X_MIN, M_LESS_OR_EQUAL, rect.left, M_NULL);
			MedgeSelect(MilEdgeResult, M_EXCLUDE, M_BOX_Y_MIN, M_LESS_OR_EQUAL, rect.top, M_NULL);
			MedgeSelect(MilEdgeResult, M_EXCLUDE, M_BOX_X_MAX, M_GREATER_OR_EQUAL, rect.right, M_NULL);
			MedgeSelect(MilEdgeResult, M_EXCLUDE, M_BOX_Y_MAX, M_GREATER_OR_EQUAL, rect.bottom, M_NULL);
			//MedgeSelect(MilEdgeResult, M_EXCLUDE, M_FERET_X, M_LESS, spec, M_NULL); 
			//MedgeSelect(MilEdgeResult, M_EXCLUDE, M_FERET_Y, M_LESS, spec, M_NULL); 
			MedgeSelect(MilEdgeResult, M_EXCLUDE, M_ELLIPSE_FIT_MAJOR_AXIS , M_LESS, spec / 2.0, M_NULL);
			MedgeSelect(MilEdgeResult, M_EXCLUDE, M_ELLIPSE_FIT_MINOR_AXIS , M_LESS, spec / 2.0, M_NULL);
			MedgeSelect(MilEdgeResult, M_EXCLUDE, M_ELLIPSE_FIT_ERROR , M_GREATER_OR_EQUAL , 1.0, M_NULL);//20161017 ngh

			/* Get the number of edges found. */
			MedgeGetResult(MilEdgeResult, M_DEFAULT, M_NUMBER_OF_CHAINS+M_TYPE_LONG, &NumEdgeFound, M_NULL);

			//result
			if(NumEdgeFound > 0)
			{
				if ((CogX = (double *)malloc(NumEdgeFound*sizeof(double))) && 
					(CogY = (double *)malloc(NumEdgeFound*sizeof(double))) && 
					(Major = (double *)malloc(NumEdgeFound*sizeof(double))) &&
					(Minor = (double *)malloc(NumEdgeFound*sizeof(double))) &&
					(Ang = (double *)malloc(NumEdgeFound*sizeof(double))) &&
					(Width = (double *)malloc(NumEdgeFound*sizeof(double))) &&
					(Height = (double *)malloc(NumEdgeFound*sizeof(double))))
				{
					memset(CogX, NULL, NumEdgeFound*sizeof(double));
					memset(CogY, NULL, NumEdgeFound*sizeof(double));
					memset(Major, NULL, NumEdgeFound*sizeof(double));
					memset(Minor, NULL, NumEdgeFound*sizeof(double));
					memset(Ang, NULL, NumEdgeFound*sizeof(double));
					memset(Width, NULL, NumEdgeFound*sizeof(double));
					memset(CogX, NULL, NumEdgeFound*sizeof(double));

					MedgeGetResult(MilEdgeResult, M_DEFAULT, M_CENTER_OF_GRAVITY_X, CogX, M_NULL); 
					MedgeGetResult(MilEdgeResult, M_DEFAULT, M_CENTER_OF_GRAVITY_Y, CogY, M_NULL); 
					MedgeGetResult(MilEdgeResult, M_DEFAULT, M_FERET_X, Width, M_NULL); 
					MedgeGetResult(MilEdgeResult, M_DEFAULT, M_FERET_Y, Height, M_NULL); 
					MedgeGetResult(MilEdgeResult, M_DEFAULT, M_ELLIPSE_FIT_MAJOR_AXIS, Major, M_NULL);
					MedgeGetResult(MilEdgeResult, M_DEFAULT, M_ELLIPSE_FIT_MINOR_AXIS, Minor, M_NULL);
					MedgeGetResult(MilEdgeResult, M_DEFAULT, M_ELLIPSE_FIT_ANGLE, Ang, M_NULL);//20161017 ngh

					MgraColor(M_DEFAULT, M_COLOR_LIGHT_GREEN);
					MedgeDraw(M_DEFAULT, MilEdgeResult, *m_MilManage.GetMilOverlay(MAIN_DISP), M_DRAW_EDGES, M_DEFAULT, M_DEFAULT);

					CdRect def_rect, temp_rect;


					for(int def_idx = 0; def_idx < NumEdgeFound; def_idx++)
					{
						BOOL isCompare = FALSE;
						def_rect.SetdRectCWH(CogX[def_idx], CogY[def_idx], Width[def_idx], Height[def_idx]);

						for(int j = 0; j < m_InsRsltPara[isGrabComp].nNgCnt; j++)
						{
							temp_rect.SetdRectCWH(m_InsRsltPara[isGrabComp].dCentX[j], m_InsRsltPara[isGrabComp].dCentY[j],
								m_InsRsltPara[isGrabComp].dWidth[j], m_InsRsltPara[isGrabComp].dHeight[j]);
							if(def_rect.IntersectRect(temp_rect))
							{
								isCompare = TRUE;
								def_rect.UnionRect(def_rect, temp_rect);
								m_InsRsltPara[isGrabComp].dCentX[j] = def_rect.GetCenter().GetX();
								m_InsRsltPara[isGrabComp].dCentY[j] = def_rect.GetCenter().GetY();
								m_InsRsltPara[isGrabComp].dWidth[j] = def_rect.GetWidth();
								m_InsRsltPara[isGrabComp].dHeight[j] = def_rect.GetHeight();
								m_InsRsltPara[isGrabComp].dLong[j] = Major[def_idx] * 2;
								m_InsRsltPara[isGrabComp].dShort[j] = Minor[def_idx] * 2;
								m_InsRsltPara[isGrabComp].dAng[j] = Ang[def_idx];//20161017 ngh
							}
						}

						if(!isCompare)
						{
							m_InsRsltPara[isGrabComp].dCentX[m_InsRsltPara[isGrabComp].nNgCnt] = CogX[def_idx];
							m_InsRsltPara[isGrabComp].dCentY[m_InsRsltPara[isGrabComp].nNgCnt] = CogY[def_idx];
							m_InsRsltPara[isGrabComp].dWidth[m_InsRsltPara[isGrabComp].nNgCnt] = Width[def_idx];
							m_InsRsltPara[isGrabComp].dHeight[m_InsRsltPara[isGrabComp].nNgCnt] = Height[def_idx];
							m_InsRsltPara[isGrabComp].dLong[m_InsRsltPara[isGrabComp].nNgCnt] = Major[def_idx] * 2;
							m_InsRsltPara[isGrabComp].dShort[m_InsRsltPara[isGrabComp].nNgCnt] = Minor[def_idx] * 2;
							m_InsRsltPara[isGrabComp].dAng[m_InsRsltPara[isGrabComp].nNgCnt] = Ang[def_idx];//20161017 ngh

							m_InsRsltPara[isGrabComp].nDeviceIdx[m_InsRsltPara[isGrabComp].nNgCnt].SetPoint(nDeviceIdxX, (nDeviceIdxY + i));
							m_InsRsltPara[isGrabComp].nGrabIdx[m_InsRsltPara[isGrabComp].nNgCnt] = nGrabIdx;
							m_InsRsltPara[isGrabComp].nInspGrabIdx[m_InsRsltPara[isGrabComp].nNgCnt] = nInspGrabIdx;

							m_InsRsltPara[isGrabComp].bNg[m_InsRsltPara[isGrabComp].nNgCnt] = TRUE;

							m_InsRsltPara[isGrabComp].nNgCnt++;
						}
					}
				}
				else
				{

				}

				nNG = INSPECTION_NG;
				if(nInspGrabIdx == 0)
				{
					m_Rslt.m_bInspectionRslt[0][i] = TRUE;
				}
				else
				{
					m_Rslt.m_bInspectionRslt[1][i] = TRUE;
				}

				free(CogX);
				free(CogY);
				free(Major);
				free(Minor);
				free(Ang);//20161017 ngh
				free(Width);
				free(Height);
			}

			/* Draw edges in the source image to show the result. */
			MgraColor(M_DEFAULT, M_COLOR_LIGHT_GREEN);
			MedgeDraw(M_DEFAULT, MilEdgeResult, *m_MilManage.GetMilOverlay(MAIN_DISP), M_DRAW_EDGES, M_DEFAULT, M_DEFAULT);

			MedgeFree(MilEdgeContext);
			MedgeFree(MilEdgeResult);

			MbufFree(MilBinImage);
			MbufFree(MilChildImage);

			MblobFree(MilBlobResult);
			MblobFree(MilBlobFeatureList);
#ifdef TEST
			strLog.Format(_T("InspectionEdge End_%d"), i);
			WriteLog(strLog, TYPE_LOG_MAIN);
#endif
		}
	}

	MbufFree(MilTransImage);
	MbufFree(MilAlignImage);
	MbufFree(MilMaskImage3);
	MbufFree(MilMaskImage2);
	MbufFree(MilMaskImage);
	MbufFree(image);


	strLog.Format(_T("InspectionEdge3 End"));
	//WriteLog(strLog, TYPE_LOG_MAIN);
#endif	
	return nNG;
}

int CLED_DisplayInspectDlg::InspectionEdge2(int nDeviceIdxX, int nDeviceIdxY, int nGrabIdx, int nInspGrabIdx, BOOL isGrabComp)
{
	int nNG = 0;
	int idx = 2;
	int ins_idx = nGrabIdx;
	int ins_grab_idx = nInspGrabIdx;
	int align_idx;
#ifdef CAMERA
	double *CogX, *CogY, *Width, *Height, *Major, *Minor, *Ang;//20161017 ngh
	double high_thresh, low_thresh;
#endif
	CString tmpPath_Name, strTeachPath, strLog;
	typeTeachPara teachPara;

	teachPara = m_dlgTeach->GetTeachPara();
	double spec = (double)teachPara.TeachSpec / (double)teachPara.TeachPixelSize;
	strTeachPath.Format(_T("%s\\TeachImg\\"), m_SelectModel.cPath);
#ifdef CAMERA
	MIL_ID image, MilMaskImage, MilTransImage;
	MIL_ID MilBinImage, MilChildImage, MilBlobFeatureList, MilBlobResult;
	MIL_ID MilEdgeContext, MilEdgeResult;
	MIL_INT NumEdgeFound;
	SYSTEMTIME	lpSystemTime;
	GetLocalTime(&lpSystemTime);

	strLog.Format(_T("InspectionEdge2 Start"));
	//WriteLog(strLog, TYPE_LOG_MAIN);

	MbufAlloc2d(*m_MilManage.GetMilSystem(), CAMERA_WIDTH, CAMERA_HEIGHT, 8+M_UNSIGNED, M_IMAGE+M_PROC, &image);
	MbufAlloc2d(*m_MilManage.GetMilSystem(), CAMERA_WIDTH, CAMERA_HEIGHT, 8+M_UNSIGNED, M_IMAGE+M_PROC, &MilTransImage);
	MbufAlloc2d(*m_MilManage.GetMilSystem(), CAMERA_WIDTH, CAMERA_HEIGHT, 8+M_UNSIGNED, M_IMAGE+M_PROC, &MilMaskImage);

	//MbufCopy(*m_MilManage.GetMilInspBuf(isGrabComp, nInspGrabIdx), image);
	//MimHistogramEqualize(*m_MilManage.GetMilGrabBuf(), image, M_UNIFORM, 5, 20, 100);

	for(int i = 0; i < teachPara.TeachRegionCnt[ins_idx][ins_grab_idx]; i++)
	{
		if(nDeviceIdxY + i == teachPara.TeachArrayCntY)
		{
			m_Rslt.m_bAlignInsp[i] = FALSE;
			nNG = INSPECTION_NG;
			m_Rslt.m_bInspectionRslt[0][i] = TRUE;
			m_Rslt.m_bInspectionRslt[1][i] = TRUE;
			continue;
		}
		if(!m_Rslt.m_bAlignInsp[i])
		{
			nNG = INSPECTION_NG;
			m_Rslt.m_bInspectionRslt[0][i] = TRUE;
			m_Rslt.m_bInspectionRslt[1][i] = TRUE;
			continue;
		}

		int k = 0;

#ifdef INSPECTION_MASK_TEST
		tmpPath_Name.Format(_T("SetMask_%d_%d_%d_%d.bmp"), ins_idx, ins_grab_idx - 2, i, k);
#else
		tmpPath_Name.Format(_T("SetMask_%d_%d_%d_0.bmp"), ins_idx, ins_grab_idx - 2, i);
#endif
		if(!FileSearch(strTeachPath, tmpPath_Name))
		{
			strLog.Format(_T("Edge2,SetMaskImageSearchFail_%d"), i);
			WriteLog(strLog, TYPE_LOG_MAIN);
			continue;
		}

		MbufClear(MilMaskImage, 0xFF);
#ifdef INSPECTION_MASK_TEST
		tmpPath_Name.Format(_T("%sSetMask_%d_%d_%d_%d.bmp"), strTeachPath, ins_idx, ins_grab_idx - 2, i, k);
#else
		tmpPath_Name.Format(_T("%sSetMask_%d_%d_%d_0.bmp"), strTeachPath, ins_idx, ins_grab_idx - 2, i);
#endif
		MbufImport((char *)(LPCTSTR)tmpPath_Name, M_BMP, M_LOAD, *m_MilManage.GetMilSystem(), &MilMaskImage);	

		MimDilate(MilMaskImage, MilMaskImage, 3, M_BINARY);

		idx  = (i * 3) + 2;
		align_idx = (i * 3) + 1;

		CRect rect((int)teachPara.TeachRegion[ins_idx][ins_grab_idx][idx].GetLeft(),// - ((teachPara.TeachRegion[ins_idx][ins_grab_idx][align_idx].GetCenter().GetX() - teachPara.AlignPara.dX[i]) / 2),
			(int)teachPara.TeachRegion[ins_idx][ins_grab_idx][idx].GetTop(),// - ((teachPara.TeachRegion[ins_idx][ins_grab_idx][align_idx].GetCenter().GetY() - teachPara.AlignPara.dY[i]) / 2),
			(int)teachPara.TeachRegion[ins_idx][ins_grab_idx][idx].GetRight(),// + ((teachPara.TeachRegion[ins_idx][ins_grab_idx][align_idx].GetCenter().GetX() - teachPara.AlignPara.dX[i]) / 2),
			(int)teachPara.TeachRegion[ins_idx][ins_grab_idx][idx].GetBottom());// + ((teachPara.TeachRegion[ins_idx][ins_grab_idx][align_idx].GetCenter().GetY() - teachPara.AlignPara.dY[i]) / 2));
		MbufClear(MilTransImage, 0x00);

		MbufAlloc2d(*m_MilManage.GetMilSystem(), rect.Width(), rect.Height(), 8+M_UNSIGNED, M_IMAGE+M_PROC, &MilBinImage);
		MbufClear(MilBinImage, 0x00);

		MbufChild2d(MilMaskImage, rect.left, rect.top, rect.Width(), rect.Height(), &MilChildImage);

		MimBinarize(MilChildImage, MilBinImage, M_LESS_OR_EQUAL, 100, M_NULL);

		////Transform
		MimTranslate(MilBinImage, MilTransImage, rect.left, rect.top, M_DEFAULT);

		/* Allocate a blob result buffer. */
		MblobAllocResult(*m_MilManage.GetMilSystem(), &MilBlobResult); 

		/* Allocate a feature list. */ 
		MblobAllocFeatureList(*m_MilManage.GetMilSystem(), &MilBlobFeatureList); 

		MblobSelectFeature(MilBlobFeatureList, M_BOX_X_MAX);
		MblobSelectFeature(MilBlobFeatureList, M_BOX_Y_MAX);
		MblobSelectFeature(MilBlobFeatureList, M_BOX_X_MIN);
		MblobSelectFeature(MilBlobFeatureList, M_BOX_Y_MIN);

		MblobCalculate(MilTransImage, M_NULL, MilBlobFeatureList, MilBlobResult); 

		MgraColor(M_DEFAULT, M_COLOR_YELLOW);
		MblobDraw(M_DEFAULT, MilBlobResult, *m_MilManage.GetMilOverlay(MAIN_DISP), M_DRAW_BLOBS_CONTOUR , M_INCLUDED_BLOBS, M_DEFAULT);

		/* Allocate a Edge Finder context. */
		MedgeAlloc(*m_MilManage.GetMilSystem(), M_CONTOUR, M_DEFAULT, &MilEdgeContext); 

		/* Allocate a result buffer. */
		MedgeAllocResult(*m_MilManage.GetMilSystem(), M_DEFAULT, &MilEdgeResult);

		/* Enable features to compute. */
		//MedgeControl(MilEdgeContext, M_MOMENT_ELONGATION,                M_ENABLE);
		//MedgeControl(MilEdgeContext, M_FERET_MEAN_DIAMETER+M_SORT1_DOWN, M_ENABLE);
		MedgeControl(MilEdgeContext, M_FAST_LENGTH,						 M_ENABLE);
		MedgeControl(MilEdgeContext, M_ACCURACY,						 M_DISABLE);
		MedgeControl(MilEdgeContext, M_THRESHOLD_MODE,					 M_USER_DEFINED);
		MedgeControl(MilEdgeContext, M_BOX_X_MAX, M_ENABLE);
		MedgeControl(MilEdgeContext, M_BOX_Y_MAX, M_ENABLE);
		MedgeControl(MilEdgeContext, M_BOX_X_MIN, M_ENABLE);
		MedgeControl(MilEdgeContext, M_BOX_Y_MIN, M_ENABLE);
		MedgeControl(MilEdgeContext, M_FERET_X, M_ENABLE); 
		MedgeControl(MilEdgeContext, M_FERET_Y, M_ENABLE); 
		MedgeControl(MilEdgeContext, M_CENTER_OF_GRAVITY, M_ENABLE);
		MedgeControl(MilEdgeContext, M_ELLIPSE_FIT_MAJOR_AXIS, M_ENABLE);
		MedgeControl(MilEdgeContext, M_ELLIPSE_FIT_MINOR_AXIS, M_ENABLE);
		MedgeControl(MilEdgeContext, M_ELLIPSE_FIT_ANGLE, M_ENABLE);//20161017 ngh
		MedgeControl(MilEdgeContext, M_ELLIPSE_FIT_ERROR , M_ENABLE);//20161017 ngh
		MedgeControl(MilEdgeContext, M_FILL_GAP_DISTANCE, spec);// / (double)m_dlgTeach->GetTeachPara().TeachPixelSize);

		high_thresh = teachPara.TeachInspThreshHold[ins_grab_idx][1];
		low_thresh = teachPara.TeachInspThreshHold[ins_grab_idx][2];

		MedgeControl(MilEdgeContext, M_THRESHOLD_HIGH,					 high_thresh);
		MedgeControl(MilEdgeContext, M_THRESHOLD_LOW,					 low_thresh);

		MedgeMask(MilEdgeContext, MilMaskImage, M_DEFAULT);

		double temp_x, temp_y;

		temp_x = teachPara.AlignPara.dCentX[i + (m_dlgTeach->GetTeachPara().TeachGrabCnt * nGrabIdx)] - teachPara.AlignPara.dX[align_idx + nGrabIdx];
		temp_y = teachPara.AlignPara.dCentY[i + (m_dlgTeach->GetTeachPara().TeachGrabCnt * nGrabIdx)] - teachPara.AlignPara.dY[align_idx + nGrabIdx];
		//MimRotate(MilMaskImage, MilAlignImage, teachPara.AlignPara.dAng[i], teachPara.AlignPara.dX[i], teachPara.AlignPara.dY[i],
		//																	teachPara.TeachRegion[ins_idx][ins_grab_idx][align_idx].GetCenter().GetX(),
		//																	teachPara.TeachRegion[ins_idx][ins_grab_idx][align_idx].GetCenter().GetY(),
		//																	M_DEFAULT);
		//MimRotate(*m_MilManage.GetMilInspBuf(isGrabComp, nInspGrabIdx), image, teachPara.AlignPara.dAng[align_idx], teachPara.AlignPara.dCentX[i + (m_dlgTeach->GetTeachPara().TeachGrabCnt * nGrabIdx)],
		//																	teachPara.AlignPara.dCentY[i + (m_dlgTeach->GetTeachPara().TeachGrabCnt * nGrabIdx)],
		//																	teachPara.AlignPara.dX[align_idx + nGrabIdx], teachPara.AlignPara.dY[align_idx + nGrabIdx], M_DEFAULT);

		//MimHistogramEqualize(*m_MilManage.GetMilInspBuf(isGrabComp, nInspGrabIdx), image, M_HYPER_LOG, 1, teachPara.TeachInspThreshHold[ins_grab_idx][0], 200);
		MbufCopy(*m_MilManage.GetMilInspBuf(isGrabComp, nInspGrabIdx), image);

		/* Calculate edges and features. */
		MedgeCalculate(MilEdgeContext, image, M_NULL, M_NULL, M_NULL, MilEdgeResult, M_DEFAULT);

		MedgeSelect(MilEdgeResult, M_EXCLUDE, M_BOX_X_MIN, M_LESS_OR_EQUAL, rect.left, M_NULL);
		MedgeSelect(MilEdgeResult, M_EXCLUDE, M_BOX_Y_MIN, M_LESS_OR_EQUAL, rect.top, M_NULL);
		MedgeSelect(MilEdgeResult, M_EXCLUDE, M_BOX_X_MAX, M_GREATER_OR_EQUAL, rect.right, M_NULL);
		MedgeSelect(MilEdgeResult, M_EXCLUDE, M_BOX_Y_MAX, M_GREATER_OR_EQUAL, rect.bottom, M_NULL);
		//MedgeSelect(MilEdgeResult, M_EXCLUDE, M_FERET_X, M_LESS, spec, M_NULL); 
		//MedgeSelect(MilEdgeResult, M_EXCLUDE, M_FERET_Y, M_LESS, spec, M_NULL); 
		MedgeSelect(MilEdgeResult, M_EXCLUDE, M_ELLIPSE_FIT_MAJOR_AXIS , M_LESS, spec / 2.0, M_NULL);
		MedgeSelect(MilEdgeResult, M_EXCLUDE, M_ELLIPSE_FIT_MINOR_AXIS , M_LESS, spec / 2.0, M_NULL);
		MedgeSelect(MilEdgeResult, M_EXCLUDE, M_ELLIPSE_FIT_ERROR , M_GREATER_OR_EQUAL , 1.0, M_NULL);//20161017 ngh

		/* Get the number of edges found. */
		MedgeGetResult(MilEdgeResult, M_DEFAULT, M_NUMBER_OF_CHAINS+M_TYPE_LONG, &NumEdgeFound, M_NULL);

		//result
		if(NumEdgeFound > 0)
		{
			if ((CogX = (double *)malloc(NumEdgeFound*sizeof(double))) && 
				(CogY = (double *)malloc(NumEdgeFound*sizeof(double))) && 
				(Major = (double *)malloc(NumEdgeFound*sizeof(double))) &&
				(Minor = (double *)malloc(NumEdgeFound*sizeof(double))) &&
				(Ang = (double *)malloc(NumEdgeFound*sizeof(double))) &&
				(Width = (double *)malloc(NumEdgeFound*sizeof(double))) &&
				(Height = (double *)malloc(NumEdgeFound*sizeof(double))))
			{
				memset(CogX, NULL, NumEdgeFound*sizeof(double));
				memset(CogY, NULL, NumEdgeFound*sizeof(double));
				memset(Major, NULL, NumEdgeFound*sizeof(double));
				memset(Minor, NULL, NumEdgeFound*sizeof(double));
				memset(Ang, NULL, NumEdgeFound*sizeof(double));
				memset(Width, NULL, NumEdgeFound*sizeof(double));
				memset(CogX, NULL, NumEdgeFound*sizeof(double));

				MedgeGetResult(MilEdgeResult, M_DEFAULT, M_CENTER_OF_GRAVITY_X, CogX, M_NULL); 
				MedgeGetResult(MilEdgeResult, M_DEFAULT, M_CENTER_OF_GRAVITY_Y, CogY, M_NULL); 
				MedgeGetResult(MilEdgeResult, M_DEFAULT, M_FERET_X, Width, M_NULL); 
				MedgeGetResult(MilEdgeResult, M_DEFAULT, M_FERET_Y, Height, M_NULL); 
				MedgeGetResult(MilEdgeResult, M_DEFAULT, M_ELLIPSE_FIT_MAJOR_AXIS, Major, M_NULL);
				MedgeGetResult(MilEdgeResult, M_DEFAULT, M_ELLIPSE_FIT_MINOR_AXIS, Minor, M_NULL);
				MedgeGetResult(MilEdgeResult, M_DEFAULT, M_ELLIPSE_FIT_ANGLE, Ang, M_NULL);//20161017 ngh

				//MgraColor(M_DEFAULT, M_COLOR_GREEN);
				//MedgeDraw(M_DEFAULT, MilEdgeResult, *m_MilManage.GetMilOverlay(MAIN_DISP), M_DRAW_EDGES, M_DEFAULT, M_DEFAULT);

				for(int def_idx = 0; def_idx < NumEdgeFound; def_idx++)
				{
					if(!EdgeCompare(nDeviceIdxX, (nDeviceIdxY * m_dlgTeach->GetTeachPara().TeachOneGrabInspCnt) + i, nGrabIdx, CogX[def_idx], CogY[def_idx], Width[def_idx], Height[def_idx], 2))
					{
						m_InsRsltPara[isGrabComp].dCentX[m_InsRsltPara[isGrabComp].nNgCnt] = CogX[def_idx];
						m_InsRsltPara[isGrabComp].dCentY[m_InsRsltPara[isGrabComp].nNgCnt] = CogY[def_idx];
						m_InsRsltPara[isGrabComp].dWidth[m_InsRsltPara[isGrabComp].nNgCnt] = Width[def_idx];
						m_InsRsltPara[isGrabComp].dHeight[m_InsRsltPara[isGrabComp].nNgCnt] = Height[def_idx];
						m_InsRsltPara[isGrabComp].dLong[m_InsRsltPara[isGrabComp].nNgCnt] = Major[def_idx] * 2;
						m_InsRsltPara[isGrabComp].dShort[m_InsRsltPara[isGrabComp].nNgCnt] = Minor[def_idx] * 2;

						m_InsRsltPara[isGrabComp].dAng[m_InsRsltPara[isGrabComp].nNgCnt] = Ang[def_idx];//20161017 ngh

						m_InsRsltPara[isGrabComp].nDeviceIdx[m_InsRsltPara[isGrabComp].nNgCnt].SetPoint(nDeviceIdxX, (nDeviceIdxY + i));
						m_InsRsltPara[isGrabComp].nGrabIdx[m_InsRsltPara[isGrabComp].nNgCnt] = nGrabIdx;
						m_InsRsltPara[isGrabComp].nInspGrabIdx[m_InsRsltPara[isGrabComp].nNgCnt] = nInspGrabIdx;

						m_InsRsltPara[isGrabComp].bNg[m_InsRsltPara[isGrabComp].nNgCnt] = TRUE;

						m_InsRsltPara[isGrabComp].nNgCnt++;
					}
				}
			}
			else
			{

			}

			nNG = INSPECTION_NG;
			if(nInspGrabIdx == 0)
			{
				m_Rslt.m_bInspectionRslt[0][i] = TRUE;
			}
			else
			{
				m_Rslt.m_bInspectionRslt[1][i] = TRUE;
			}

			free(CogX);
			free(CogY);
			free(Major);
			free(Minor);
			free(Ang);//20161017 ngh
			free(Width);
			free(Height);
		}

		MgraColor(M_DEFAULT, M_COLOR_CYAN);
		MedgeDraw(M_DEFAULT, MilEdgeResult, *m_MilManage.GetMilOverlay(MAIN_DISP), M_DRAW_EDGES, M_DEFAULT, M_DEFAULT);

		MedgeFree(MilEdgeContext);
		MedgeFree(MilEdgeResult);
		MblobFree(MilBlobFeatureList);
		MblobFree(MilBlobResult);
		MbufFree(MilChildImage);
		MbufFree(MilBinImage);
	}

	MbufFree(MilMaskImage);
	MbufFree(MilTransImage);
	MbufFree(image);
#endif

	strLog.Format(_T("InspectionEdge2 End"));
	//WriteLog(strLog, TYPE_LOG_MAIN);

	return nNG;
}

int CLED_DisplayInspectDlg::InspectionTest(int nDeviceIdxX, int nDeviceIdxY, int nGrabIdx, int nInspGrabIdx, BOOL isGrabComp)
{
	int nNG = 0;
#ifdef CAMERA
	MIL_ID image;
	MIL_ID MilMaskImage;//, MilMaskImage2;
	MIL_ID MilEdgeContext, MilEdgeResult;
	MIL_ID MilAlignImage;

	MIL_ID MilTransImage, MilBinImage, MilChildImage, MilBlobResult, MilBlobFeatureList;
	MIL_INT tot_blob;
	long NumEdgeFound;
	CString strTeachPath;
	typeTeachPara teachPara;

	int idx = 2;
	int ins_idx = nGrabIdx;
	int ins_grab_idx = nInspGrabIdx;
	long NumResults = 0;
	double high_thresh, low_thresh;
	CString tmpPath_Name, image_file_name;
	SYSTEMTIME	lpSystemTime;
	GetLocalTime(&lpSystemTime);

	double *CogX, *CogY, *Width, *Height, *Major, *Minor, *Ang;//20161017 ngh

	teachPara = m_dlgTeach->GetTeachPara();
	double spec = (double)teachPara.TeachSpec / (double)teachPara.TeachPixelSize;
	m_RsltIdx = 0;
	CString strLog;

	InitInsRsltPara(isGrabComp);
	strLog.Format(_T("InspectionTest Start"));
	//WriteLog(strLog, TYPE_LOG_MAIN);

	strTeachPath.Format(_T("%s\\TeachImg\\"), m_SelectModel.cPath);

	//MdispControl(*m_MilManage.GetMilDisplay(MAIN_DISP), M_OVERLAY_CLEAR, M_DEFAULT);

	MbufAlloc2d(*m_MilManage.GetMilSystem(), CAMERA_WIDTH, CAMERA_HEIGHT, 8+M_UNSIGNED, M_IMAGE+M_PROC, &image);
	MbufAlloc2d(*m_MilManage.GetMilSystem(), CAMERA_WIDTH, CAMERA_HEIGHT, 8+M_UNSIGNED, M_IMAGE+M_PROC, &MilTransImage);
	MbufAlloc2d(*m_MilManage.GetMilSystem(), CAMERA_WIDTH, CAMERA_HEIGHT, 8+M_UNSIGNED, M_IMAGE+M_PROC, &MilMaskImage);
	MbufAlloc2d(*m_MilManage.GetMilSystem(), CAMERA_WIDTH, CAMERA_HEIGHT, 8+M_UNSIGNED, M_IMAGE+M_PROC, &MilAlignImage);

	MbufClear(MilAlignImage, 0xFF);
	int align_idx = 0;
	//Inspection
	for(int i = 0; i < teachPara.TeachRegionCnt[ins_idx][ins_grab_idx]; i++)
	{
		if(nDeviceIdxY + i == teachPara.TeachArrayCntY)
		{
			nNG = INSPECTION_NG;
			m_Rslt.m_bInspectionRslt[0][i] = TRUE;
			m_Rslt.m_bInspectionRslt[1][i] = TRUE;
			continue;
		}
		if(!m_Rslt.m_bAlignInsp[i])
		{
			nNG = INSPECTION_NG;
			m_Rslt.m_bInspectionRslt[0][i] = TRUE;
			m_Rslt.m_bInspectionRslt[1][i] = TRUE;
			continue;
		}
#ifdef TEST
		strLog.Format(_T("InspectionEdge Start_%d"), i);
		WriteLog(strLog, TYPE_LOG_MAIN);
#endif
		idx  = (i * 3) + 2;
		align_idx = (i * 3) + 1;
		CRect ng_rect;

		for(int k = 0; k < 2; k++)
		{
			tmpPath_Name.Format(_T("Mask_%d_%d_%d.bmp"), ins_idx, i, k);
			if(!FileSearch(strTeachPath, tmpPath_Name))
			{
				MgraColor(M_DEFAULT, M_COLOR_RED);
				strLog.Format(_T("Not Model Teaching"));
				CdRect rect;
				rect.SetdRectLTRB((int)teachPara.TeachRegion[ins_idx][ins_grab_idx][idx].GetLeft(),
					(int)teachPara.TeachRegion[ins_idx][ins_grab_idx][idx].GetTop(),
					(int)teachPara.TeachRegion[ins_idx][ins_grab_idx][idx].GetRight(),
					(int)teachPara.TeachRegion[ins_idx][ins_grab_idx][idx].GetBottom());
				MgraText(M_DEFAULT, *m_MilManage.GetMilOverlay(MAIN_DISP), rect.GetLeft() + 20, rect.GetTop() + 20, MIL_TEXT((char *)(LPCTSTR)strLog));
				strLog.Format(_T("Test,MaskImageSearchFail_%d"), i);
				WriteLog(strLog, TYPE_LOG_MAIN);
				continue;
			}

			MbufClear(MilMaskImage, 0xFF);

			tmpPath_Name.Format(_T("%sMask_%d_%d_%d.bmp"), strTeachPath, ins_idx, i, k);

			MbufImport((char *)(LPCTSTR)tmpPath_Name, M_BMP, M_LOAD, *m_MilManage.GetMilSystem(), &MilMaskImage);	

			double temp_x, temp_y;

			temp_x = teachPara.AlignPara.dCentX[i + (m_dlgTeach->GetTeachPara().TeachGrabCnt * nGrabIdx)] - teachPara.AlignPara.dX[align_idx + nGrabIdx];
			temp_y = teachPara.AlignPara.dCentY[i + (m_dlgTeach->GetTeachPara().TeachGrabCnt * nGrabIdx)] - teachPara.AlignPara.dY[align_idx + nGrabIdx];

			//MimRotate(MilMaskImage, MilAlignImage, teachPara.AlignPara.dAng[i], teachPara.AlignPara.dX[i], teachPara.AlignPara.dY[i],
			//																	teachPara.TeachRegion[ins_idx][ins_grab_idx][align_idx].GetCenter().GetX(),
			//																	teachPara.TeachRegion[ins_idx][ins_grab_idx][align_idx].GetCenter().GetY(),
			//																	M_DEFAULT);
			MimRotate(MilMaskImage, MilAlignImage, teachPara.AlignPara.dAng[align_idx], teachPara.AlignPara.dCentX[i + (m_dlgTeach->GetTeachPara().TeachGrabCnt * nGrabIdx)],
				teachPara.AlignPara.dCentY[i + (m_dlgTeach->GetTeachPara().TeachGrabCnt * nGrabIdx)],
				teachPara.AlignPara.dX[align_idx + nGrabIdx], teachPara.AlignPara.dY[align_idx + nGrabIdx], M_DEFAULT);

			if(k == 0)
			{
				MbufCopy(MilAlignImage, MilMaskImage);
			}
			else
			{
				MimDilate(MilAlignImage, MilMaskImage, teachPara.TeachCircleErodeDistance[nInspGrabIdx], M_BINARY);
			}

			CRect rect((int)teachPara.TeachRegion[ins_idx][ins_grab_idx][idx].GetLeft(),// - ((teachPara.TeachRegion[ins_idx][ins_grab_idx][align_idx].GetCenter().GetX() - teachPara.AlignPara.dX[i]) / 2),
				(int)teachPara.TeachRegion[ins_idx][ins_grab_idx][idx].GetTop(),// - ((teachPara.TeachRegion[ins_idx][ins_grab_idx][align_idx].GetCenter().GetY() - teachPara.AlignPara.dY[i]) / 2),
				(int)teachPara.TeachRegion[ins_idx][ins_grab_idx][idx].GetRight(),// + ((teachPara.TeachRegion[ins_idx][ins_grab_idx][align_idx].GetCenter().GetX() - teachPara.AlignPara.dX[i]) / 2),
				(int)teachPara.TeachRegion[ins_idx][ins_grab_idx][idx].GetBottom());// + ((teachPara.TeachRegion[ins_idx][ins_grab_idx][align_idx].GetCenter().GetY() - teachPara.AlignPara.dY[i]) / 2));
			MbufClear(MilTransImage, 0x00);

			MbufAlloc2d(*m_MilManage.GetMilSystem(), rect.Width(), rect.Height(), 8+M_UNSIGNED, M_IMAGE+M_PROC, &MilBinImage);
			MbufClear(MilBinImage, 0x00);

			MbufChild2d(MilMaskImage, rect.left, rect.top, rect.Width(), rect.Height(), &MilChildImage);
			MimBinarize(MilChildImage, MilBinImage, M_LESS_OR_EQUAL, 100, M_NULL);
			MimTranslate(MilBinImage, MilTransImage, rect.left, rect.top, M_DEFAULT);

			/* Allocate a blob result buffer. */
			MblobAllocResult(*m_MilManage.GetMilSystem(), &MilBlobResult); 

			/* Allocate a feature list. */ 
			MblobAllocFeatureList(*m_MilManage.GetMilSystem(), &MilBlobFeatureList); 

			MblobSelectFeature(MilBlobFeatureList, M_BOX_X_MAX);
			MblobSelectFeature(MilBlobFeatureList, M_BOX_Y_MAX);
			MblobSelectFeature(MilBlobFeatureList, M_BOX_X_MIN);
			MblobSelectFeature(MilBlobFeatureList, M_BOX_Y_MIN);
			MblobSelectFeature(MilBlobFeatureList, M_MEAN_PIXEL);
			MblobSelectFeature(MilBlobFeatureList, M_FERET_X); 
			MblobSelectFeature(MilBlobFeatureList, M_FERET_Y); 
			MblobSelectFeature(MilBlobFeatureList, M_CENTER_OF_GRAVITY);
			MblobSelectFeature(MilBlobFeatureList, M_MEAN_PIXEL);
			MblobSelectFeature(MilBlobFeatureList, M_SUM_PIXEL);
			MblobSelectFeature(MilBlobFeatureList, M_AREA );

			/* Allocate a Edge Finder context. */
			MedgeAlloc(*m_MilManage.GetMilSystem(), M_CONTOUR, M_DEFAULT, &MilEdgeContext); 

			/* Allocate a result buffer. */
			MedgeAllocResult(*m_MilManage.GetMilSystem(), M_DEFAULT, &MilEdgeResult);

			/* Enable features to compute. */
			//MedgeControl(MilEdgeContext, M_MOMENT_ELONGATION,                M_ENABLE);
			//MedgeControl(MilEdgeContext, M_FERET_MEAN_DIAMETER+M_SORT1_DOWN, M_ENABLE);
			MedgeControl(MilEdgeContext, M_FAST_LENGTH,						 M_ENABLE);
			MedgeControl(MilEdgeContext, M_ACCURACY,						 M_DISABLE);
			MedgeControl(MilEdgeContext, M_THRESHOLD_MODE,					 M_USER_DEFINED);
			MedgeControl(MilEdgeContext, M_BOX_X_MAX, M_ENABLE);
			MedgeControl(MilEdgeContext, M_BOX_Y_MAX, M_ENABLE);
			MedgeControl(MilEdgeContext, M_BOX_X_MIN, M_ENABLE);
			MedgeControl(MilEdgeContext, M_BOX_Y_MIN, M_ENABLE);
			MedgeControl(MilEdgeContext, M_FERET_X, M_ENABLE); 
			MedgeControl(MilEdgeContext, M_FERET_Y, M_ENABLE); 
			MedgeControl(MilEdgeContext, M_CENTER_OF_GRAVITY, M_ENABLE);
			MedgeControl(MilEdgeContext, M_ELLIPSE_FIT_MAJOR_AXIS, M_ENABLE);
			MedgeControl(MilEdgeContext, M_ELLIPSE_FIT_MINOR_AXIS, M_ENABLE);
			MedgeControl(MilEdgeContext, M_ELLIPSE_FIT_ANGLE, M_ENABLE);//20161017 ngh
			MedgeControl(MilEdgeContext, M_ELLIPSE_FIT_ERROR , M_ENABLE);//20161017 ngh
			MedgeControl(MilEdgeContext, M_FILL_GAP_DISTANCE, spec);// / (double)m_dlgTeach->GetTeachPara().TeachPixelSize);
			MedgeControl(MilEdgeContext, M_MAGNITUDE_TYPE, M_SQR_NORM);
			MedgeControl(MilEdgeContext, M_SAVE_CHAIN_MAGNITUDE, M_ENABLE);
			MedgeControl(MilEdgeContext, M_SAVE_CHAIN_ANGLE, M_ENABLE);
			MedgeControl(MilEdgeContext, M_SAVE_ANGLE, M_ENABLE);

			MimHistogramEqualize(*m_MilManage.GetMilInspBuf(isGrabComp, nInspGrabIdx), image, M_RAYLEIGH, 1, teachPara.TeachInspThreshHold[ins_grab_idx][0], 255);
			MblobCalculate(MilTransImage, image, MilBlobFeatureList, MilBlobResult); 
			MblobGetNumber(MilBlobResult, &tot_blob);

			MgraColor(M_DEFAULT, M_COLOR_BLUE);
			MblobDraw(M_DEFAULT, MilBlobResult, *m_MilManage.GetMilOverlay(MAIN_DISP), M_DRAW_BLOBS_CONTOUR , M_INCLUDED_BLOBS, M_DEFAULT);

			high_thresh = teachPara.TeachInspThreshHold[ins_grab_idx][1];
			low_thresh = teachPara.TeachInspThreshHold[ins_grab_idx][2];

			MedgeControl(MilEdgeContext, M_THRESHOLD_HIGH,					 high_thresh);
			MedgeControl(MilEdgeContext, M_THRESHOLD_LOW,					 low_thresh);

			MedgeMask(MilEdgeContext, MilMaskImage, M_DEFAULT);

			/* Calculate edges and features. */
			MedgeCalculate(MilEdgeContext, image, M_NULL, M_NULL, M_NULL, MilEdgeResult, M_DEFAULT);

			MedgeSelect(MilEdgeResult, M_EXCLUDE, M_BOX_X_MIN, M_LESS_OR_EQUAL, rect.left, M_NULL);
			MedgeSelect(MilEdgeResult, M_EXCLUDE, M_BOX_Y_MIN, M_LESS_OR_EQUAL, rect.top, M_NULL);
			MedgeSelect(MilEdgeResult, M_EXCLUDE, M_BOX_X_MAX, M_GREATER_OR_EQUAL, rect.right, M_NULL);
			MedgeSelect(MilEdgeResult, M_EXCLUDE, M_BOX_Y_MAX, M_GREATER_OR_EQUAL, rect.bottom, M_NULL);
			//MedgeSelect(MilEdgeResult, M_EXCLUDE, M_FERET_X, M_LESS, spec, M_NULL); 
			//MedgeSelect(MilEdgeResult, M_EXCLUDE, M_FERET_Y, M_LESS, spec, M_NULL); 
			MedgeSelect(MilEdgeResult, M_EXCLUDE, M_ELLIPSE_FIT_MAJOR_AXIS , M_LESS, spec / 2.0, M_NULL);
			MedgeSelect(MilEdgeResult, M_EXCLUDE, M_ELLIPSE_FIT_MINOR_AXIS , M_LESS, spec / 2.0, M_NULL);
			MedgeSelect(MilEdgeResult, M_EXCLUDE, M_ELLIPSE_FIT_ERROR , M_GREATER_OR_EQUAL , 1.0, M_NULL);//20161017 ngh

			/* Get the number of edges found. */
			MedgeGetResult(MilEdgeResult, M_DEFAULT, M_NUMBER_OF_CHAINS+M_TYPE_LONG, &NumEdgeFound, M_NULL);
			//result

			if(NumEdgeFound > 0)
			{
				if ((CogX = (double *)malloc(NumEdgeFound*sizeof(double))) && 
					(CogY = (double *)malloc(NumEdgeFound*sizeof(double))) && 
					(Major = (double *)malloc(NumEdgeFound*sizeof(double))) &&
					(Minor = (double *)malloc(NumEdgeFound*sizeof(double))) &&
					(Ang = (double *)malloc(NumEdgeFound*sizeof(double))) &&
					(Width = (double *)malloc(NumEdgeFound*sizeof(double))) &&
					(Height = (double *)malloc(NumEdgeFound*sizeof(double))))
				{
					memset(CogX, NULL, NumEdgeFound*sizeof(double));
					memset(CogY, NULL, NumEdgeFound*sizeof(double));
					memset(Major, NULL, NumEdgeFound*sizeof(double));
					memset(Minor, NULL, NumEdgeFound*sizeof(double));
					memset(Ang, NULL, NumEdgeFound*sizeof(double));
					memset(Width, NULL, NumEdgeFound*sizeof(double));
					memset(CogX, NULL, NumEdgeFound*sizeof(double));

					MedgeGetResult(MilEdgeResult, M_DEFAULT, M_CENTER_OF_GRAVITY_X, CogX, M_NULL); 
					MedgeGetResult(MilEdgeResult, M_DEFAULT, M_CENTER_OF_GRAVITY_Y, CogY, M_NULL); 
					MedgeGetResult(MilEdgeResult, M_DEFAULT, M_FERET_X, Width, M_NULL); 
					MedgeGetResult(MilEdgeResult, M_DEFAULT, M_FERET_Y, Height, M_NULL); 
					MedgeGetResult(MilEdgeResult, M_DEFAULT, M_ELLIPSE_FIT_MAJOR_AXIS, Major, M_NULL);
					MedgeGetResult(MilEdgeResult, M_DEFAULT, M_ELLIPSE_FIT_MINOR_AXIS, Minor, M_NULL);
					MedgeGetResult(MilEdgeResult, M_DEFAULT, M_ELLIPSE_FIT_ANGLE, Ang, M_NULL);//20161017 ngh

					MgraColor(M_DEFAULT, M_COLOR_GREEN);
					MedgeDraw(M_DEFAULT, MilEdgeResult, *m_MilManage.GetMilOverlay(MAIN_DISP), M_DRAW_EDGES, M_DEFAULT, M_DEFAULT);

					for(int def_idx = 0; def_idx < NumEdgeFound; def_idx++)
					{
						m_InsRsltPara[isGrabComp].dCentX[m_InsRsltPara[isGrabComp].nNgCnt] = CogX[def_idx];
						m_InsRsltPara[isGrabComp].dCentY[m_InsRsltPara[isGrabComp].nNgCnt] = CogY[def_idx];
						m_InsRsltPara[isGrabComp].dWidth[m_InsRsltPara[isGrabComp].nNgCnt] = Width[def_idx];
						m_InsRsltPara[isGrabComp].dHeight[m_InsRsltPara[isGrabComp].nNgCnt] = Height[def_idx];
						m_InsRsltPara[isGrabComp].dLong[m_InsRsltPara[isGrabComp].nNgCnt] = Major[def_idx] * 2;
						m_InsRsltPara[isGrabComp].dShort[m_InsRsltPara[isGrabComp].nNgCnt] = Minor[def_idx] * 2;
						m_InsRsltPara[isGrabComp].dAng[m_InsRsltPara[isGrabComp].nNgCnt] = Ang[def_idx];//20161017 ngh

						m_InsRsltPara[isGrabComp].nDeviceIdx[m_InsRsltPara[isGrabComp].nNgCnt].SetPoint(nDeviceIdxX, (nDeviceIdxY + i));
						m_InsRsltPara[isGrabComp].nGrabIdx[m_InsRsltPara[isGrabComp].nNgCnt] = nGrabIdx;
						m_InsRsltPara[isGrabComp].nInspGrabIdx[m_InsRsltPara[isGrabComp].nNgCnt] = nInspGrabIdx;

						m_InsRsltPara[isGrabComp].bNg[m_InsRsltPara[isGrabComp].nNgCnt] = TRUE;

						m_InsRsltPara[isGrabComp].nNgCnt++;
					}
				}
				else
				{

				}

				nNG = INSPECTION_NG;
				if(nInspGrabIdx == 0)
				{
					m_Rslt.m_bInspectionRslt[0][i] = TRUE;
				}
				else
				{
					m_Rslt.m_bInspectionRslt[1][i] = TRUE;
				}

				free(CogX);
				free(CogY);
				free(Major);
				free(Minor);
				free(Ang);//20161017 ngh
				free(Width);
				free(Height);
			}

			/* Draw edges in the source image to show the result. */
			MgraColor(M_DEFAULT, M_COLOR_GREEN);
			MedgeDraw(M_DEFAULT, MilEdgeResult, *m_MilManage.GetMilOverlay(MAIN_DISP), M_DRAW_EDGES, M_DEFAULT, M_DEFAULT);

			MedgeFree(MilEdgeContext);
			MedgeFree(MilEdgeResult);

			MbufFree(MilBinImage);
			MbufFree(MilChildImage);

			MblobFree(MilBlobResult);
			MblobFree(MilBlobFeatureList);
#ifdef TEST
			strLog.Format(_T("InspectionTest End_%d"), i);
			WriteLog(strLog, TYPE_LOG_MAIN);
#endif
		}
	}

	MbufFree(MilTransImage);
	MbufFree(MilAlignImage);
	MbufFree(MilMaskImage);
	MbufFree(image);


	strLog.Format(_T("InspectionEdge1 End"));
	//WriteLog(strLog, TYPE_LOG_MAIN);
#endif	
	return nNG;
}


int CLED_DisplayInspectDlg::InspectionEdge(int nDeviceIdxX, int nDeviceIdxY, int nGrabIdx, int nInspGrabIdx, BOOL isGrabComp)
{
	int nNG = 0;
#ifdef CAMERA
	MIL_ID image;
	MIL_ID MilMaskImage;//, MilMaskImage2;
	MIL_ID MilEdgeContext, MilEdgeResult;
	MIL_ID MilAlignImage;

	MIL_ID MilTransImage, MilBinImage, MilChildImage, MilBlobResult, MilBlobFeatureList;
	MIL_INT tot_blob;
	long NumEdgeFound;
	CString strTeachPath;
	typeTeachPara teachPara;

	int idx = 2;
	int ins_idx = nGrabIdx;
	int ins_grab_idx = nInspGrabIdx;
	long NumResults = 0;
	double high_thresh, low_thresh;
	CString tmpPath_Name, image_file_name;
	SYSTEMTIME	lpSystemTime;
	GetLocalTime(&lpSystemTime);

	double *CogX, *CogY, *Width, *Height, *Major, *Minor, *Ang;//20161017 ngh

	teachPara = m_dlgTeach->GetTeachPara();
	double spec = (double)teachPara.TeachSpec / (double)teachPara.TeachPixelSize;
	m_RsltIdx = 0;
	double fun_time_s, fun_time_e, fun_time;
	CString strLog;

	InitInsRsltPara(isGrabComp);
	strLog.Format(_T("InspectionEdge1 Start"));
	//WriteLog(strLog, TYPE_LOG_MAIN);

	strTeachPath.Format(_T("%s\\TeachImg\\"), m_SelectModel.cPath);

	//MdispControl(*m_MilManage.GetMilDisplay(MAIN_DISP), M_OVERLAY_CLEAR, M_DEFAULT);

	MbufAlloc2d(*m_MilManage.GetMilSystem(), CAMERA_WIDTH, CAMERA_HEIGHT, 8+M_UNSIGNED, M_IMAGE+M_PROC, &image);
	MbufAlloc2d(*m_MilManage.GetMilSystem(), CAMERA_WIDTH, CAMERA_HEIGHT, 8+M_UNSIGNED, M_IMAGE+M_PROC, &MilTransImage);
	MbufAlloc2d(*m_MilManage.GetMilSystem(), CAMERA_WIDTH, CAMERA_HEIGHT, 8+M_UNSIGNED, M_IMAGE+M_PROC, &MilMaskImage);
	MbufAlloc2d(*m_MilManage.GetMilSystem(), CAMERA_WIDTH, CAMERA_HEIGHT, 8+M_UNSIGNED, M_IMAGE+M_PROC, &MilAlignImage);

	MbufClear(MilAlignImage, 0xFF);
	int align_idx = 0;
	//Inspection
	for(int i = 0; i < teachPara.TeachRegionCnt[ins_idx][ins_grab_idx]; i++)
	{
		if(nDeviceIdxY + i == teachPara.TeachArrayCntY)
		{
			nNG = INSPECTION_NG;
			m_Rslt.m_bInspectionRslt[0][i] = TRUE;
			m_Rslt.m_bInspectionRslt[1][i] = TRUE;
			continue;
		}
		if(!m_Rslt.m_bAlignInsp[i])
		{
			nNG = INSPECTION_NG;
			m_Rslt.m_bInspectionRslt[0][i] = TRUE;
			m_Rslt.m_bInspectionRslt[1][i] = TRUE;
			continue;
		}
#ifdef TEST
		strLog.Format(_T("InspectionEdge Start_%d"), i);
		WriteLog(strLog, TYPE_LOG_MAIN);
#endif
		idx  = (i * 3) + 2;
		align_idx = (i * 3) + 1;
		CRect ng_rect;

		tmpPath_Name.Format(_T("Mask_%d_%d_0.bmp"), ins_idx, i);
		if(!FileSearch(strTeachPath, tmpPath_Name))
		{
			MgraColor(M_DEFAULT, M_COLOR_RED);
			strLog.Format(_T("Not Model Teaching"));
			CdRect rect;
			rect.SetdRectLTRB((int)teachPara.TeachRegion[ins_idx][ins_grab_idx][idx].GetLeft(),
				(int)teachPara.TeachRegion[ins_idx][ins_grab_idx][idx].GetTop(),
				(int)teachPara.TeachRegion[ins_idx][ins_grab_idx][idx].GetRight(),
				(int)teachPara.TeachRegion[ins_idx][ins_grab_idx][idx].GetBottom());
			MgraText(M_DEFAULT, *m_MilManage.GetMilOverlay(MAIN_DISP), rect.GetLeft() + 20, rect.GetTop() + 20, MIL_TEXT((char *)(LPCTSTR)strLog));
			strLog.Format(_T("MaskImageSearchFail_%d"), i);
			WriteLog(strLog, TYPE_LOG_MAIN);
			continue;
		}

		MbufClear(MilMaskImage, 0xFF);

		tmpPath_Name.Format(_T("%sMask_%d_%d_0.bmp"), strTeachPath, ins_idx, i);

		MbufImport((char *)(LPCTSTR)tmpPath_Name, M_BMP, M_LOAD, *m_MilManage.GetMilSystem(), &MilMaskImage);	

		double temp_x, temp_y;

		temp_x = teachPara.AlignPara.dCentX[i + (m_dlgTeach->GetTeachPara().TeachGrabCnt * nGrabIdx)] - teachPara.AlignPara.dX[align_idx + nGrabIdx];
		temp_y = teachPara.AlignPara.dCentY[i + (m_dlgTeach->GetTeachPara().TeachGrabCnt * nGrabIdx)] - teachPara.AlignPara.dY[align_idx + nGrabIdx];

		fun_time_s = clock();
		//MimRotate(MilMaskImage, MilAlignImage, teachPara.AlignPara.dAng[i], teachPara.AlignPara.dX[i], teachPara.AlignPara.dY[i],
		//																	teachPara.TeachRegion[ins_idx][ins_grab_idx][align_idx].GetCenter().GetX(),
		//																	teachPara.TeachRegion[ins_idx][ins_grab_idx][align_idx].GetCenter().GetY(),
		//																	M_DEFAULT);
		MimRotate(MilMaskImage, MilAlignImage, teachPara.AlignPara.dAng[align_idx], teachPara.AlignPara.dCentX[i + (m_dlgTeach->GetTeachPara().TeachGrabCnt * nGrabIdx)],
			teachPara.AlignPara.dCentY[i + (m_dlgTeach->GetTeachPara().TeachGrabCnt * nGrabIdx)],
			teachPara.AlignPara.dX[align_idx + nGrabIdx], teachPara.AlignPara.dY[align_idx + nGrabIdx], M_DEFAULT);
		fun_time_e = clock();
		fun_time = (fun_time_e - fun_time_s) / 1000.0;
#ifdef TEST
		strLog.Format(_T("MimRotate() %f"), fun_time);
		WriteLog(strLog, TYPE_LOG_MAIN);


		tmpPath_Name.Format(_T("%sMilAlignImage_%d_%d_0.bmp"), strTeachPath, nGrabIdx, nInspGrabIdx);
		MbufExport((char *)(LPCTSTR)tmpPath_Name, M_BMP, MilAlignImage);
#endif
		MbufCopy(MilAlignImage, MilMaskImage);

#ifdef TEST
		tmpPath_Name.Format(_T("%sMilOrgAlignImage_%d_%d_0.bmp"), strTeachPath, nGrabIdx, nInspGrabIdx);
		MbufExport((char *)(LPCTSTR)tmpPath_Name, M_BMP, MilAlignImage);
#endif

		CRect rect((int)teachPara.TeachRegion[ins_idx][ins_grab_idx][idx].GetLeft(),// - ((teachPara.TeachRegion[ins_idx][ins_grab_idx][align_idx].GetCenter().GetX() - teachPara.AlignPara.dX[i]) / 2),
			(int)teachPara.TeachRegion[ins_idx][ins_grab_idx][idx].GetTop(),// - ((teachPara.TeachRegion[ins_idx][ins_grab_idx][align_idx].GetCenter().GetY() - teachPara.AlignPara.dY[i]) / 2),
			(int)teachPara.TeachRegion[ins_idx][ins_grab_idx][idx].GetRight(),// + ((teachPara.TeachRegion[ins_idx][ins_grab_idx][align_idx].GetCenter().GetX() - teachPara.AlignPara.dX[i]) / 2),
			(int)teachPara.TeachRegion[ins_idx][ins_grab_idx][idx].GetBottom());// + ((teachPara.TeachRegion[ins_idx][ins_grab_idx][align_idx].GetCenter().GetY() - teachPara.AlignPara.dY[i]) / 2));
		MbufClear(MilTransImage, 0x00);

		MbufAlloc2d(*m_MilManage.GetMilSystem(), rect.Width(), rect.Height(), 8+M_UNSIGNED, M_IMAGE+M_PROC, &MilBinImage);
		MbufClear(MilBinImage, 0x00);

		MbufChild2d(MilMaskImage, rect.left, rect.top, rect.Width(), rect.Height(), &MilChildImage);
#ifdef TEST
		tmpPath_Name.Format(_T("%sMilChildImage_%d_%d_0.bmp"), strTeachPath, nGrabIdx, nInspGrabIdx);
		MbufExport((char *)(LPCTSTR)tmpPath_Name, M_BMP, MilChildImage);
#endif
		fun_time_s = clock();
		MimBinarize(MilChildImage, MilBinImage, M_LESS_OR_EQUAL, 100, M_NULL);
		fun_time_e = clock();
		fun_time = (fun_time_e - fun_time_s) / 1000.0;
#ifdef TEST
		strLog.Format(_T("MimBinarize() %f"), fun_time);
		WriteLog(strLog, TYPE_LOG_MAIN);

		tmpPath_Name.Format(_T("%sMilBinImage_%d_%d_0.bmp"), strTeachPath, nGrabIdx, nInspGrabIdx);
		MbufExport((char *)(LPCTSTR)tmpPath_Name, M_BMP, MilBinImage);
#endif

		////Transform
		fun_time_s = clock();
		MimTranslate(MilBinImage, MilTransImage, rect.left, rect.top, M_DEFAULT);
		fun_time_e = clock();
		fun_time = (fun_time_e - fun_time_s) / 1000.0;
#ifdef TEST
		strLog.Format(_T("MimTranslate() %f"), fun_time);
		WriteLog(strLog, TYPE_LOG_MAIN);

		tmpPath_Name.Format(_T("%sMilTransImage_%d_%d_0.bmp"), strTeachPath, nGrabIdx, nInspGrabIdx);
		MbufExport((char *)(LPCTSTR)tmpPath_Name, M_BMP, MilTransImage);
#endif

		/* Allocate a blob result buffer. */
		MblobAllocResult(*m_MilManage.GetMilSystem(), &MilBlobResult); 

		/* Allocate a feature list. */ 
		MblobAllocFeatureList(*m_MilManage.GetMilSystem(), &MilBlobFeatureList); 

		MblobSelectFeature(MilBlobFeatureList, M_BOX_X_MAX);
		MblobSelectFeature(MilBlobFeatureList, M_BOX_Y_MAX);
		MblobSelectFeature(MilBlobFeatureList, M_BOX_X_MIN);
		MblobSelectFeature(MilBlobFeatureList, M_BOX_Y_MIN);
		MblobSelectFeature(MilBlobFeatureList, M_MEAN_PIXEL);
		MblobSelectFeature(MilBlobFeatureList, M_FERET_X); 
		MblobSelectFeature(MilBlobFeatureList, M_FERET_Y); 
		MblobSelectFeature(MilBlobFeatureList, M_CENTER_OF_GRAVITY);
		MblobSelectFeature(MilBlobFeatureList, M_MEAN_PIXEL);
		MblobSelectFeature(MilBlobFeatureList, M_SUM_PIXEL);
		MblobSelectFeature(MilBlobFeatureList, M_AREA );

		/* Allocate a Edge Finder context. */
		MedgeAlloc(*m_MilManage.GetMilSystem(), M_CONTOUR, M_DEFAULT, &MilEdgeContext); 

		/* Allocate a result buffer. */
		MedgeAllocResult(*m_MilManage.GetMilSystem(), M_DEFAULT, &MilEdgeResult);

		/* Enable features to compute. */
		//MedgeControl(MilEdgeContext, M_MOMENT_ELONGATION,                M_ENABLE);
		//MedgeControl(MilEdgeContext, M_FERET_MEAN_DIAMETER+M_SORT1_DOWN, M_ENABLE);
		MedgeControl(MilEdgeContext, M_FAST_LENGTH,						 M_ENABLE);
		MedgeControl(MilEdgeContext, M_ACCURACY,						 M_DISABLE);
		MedgeControl(MilEdgeContext, M_THRESHOLD_MODE,					 M_USER_DEFINED);
		MedgeControl(MilEdgeContext, M_BOX_X_MAX, M_ENABLE);
		MedgeControl(MilEdgeContext, M_BOX_Y_MAX, M_ENABLE);
		MedgeControl(MilEdgeContext, M_BOX_X_MIN, M_ENABLE);
		MedgeControl(MilEdgeContext, M_BOX_Y_MIN, M_ENABLE);
		MedgeControl(MilEdgeContext, M_FERET_X, M_ENABLE); 
		MedgeControl(MilEdgeContext, M_FERET_Y, M_ENABLE); 
		MedgeControl(MilEdgeContext, M_CENTER_OF_GRAVITY, M_ENABLE);
		MedgeControl(MilEdgeContext, M_ELLIPSE_FIT_MAJOR_AXIS, M_ENABLE);
		MedgeControl(MilEdgeContext, M_ELLIPSE_FIT_MINOR_AXIS, M_ENABLE);
		MedgeControl(MilEdgeContext, M_ELLIPSE_FIT_ANGLE, M_ENABLE);//20161017 ngh
		MedgeControl(MilEdgeContext, M_ELLIPSE_FIT_ERROR , M_ENABLE);//20161017 ngh
		MedgeControl(MilEdgeContext, M_FILL_GAP_DISTANCE, spec);// / (double)m_dlgTeach->GetTeachPara().TeachPixelSize);
		MedgeControl(MilEdgeContext, M_MAGNITUDE_TYPE, M_SQR_NORM);
		MedgeControl(MilEdgeContext, M_SAVE_CHAIN_MAGNITUDE, M_ENABLE);
		MedgeControl(MilEdgeContext, M_SAVE_CHAIN_ANGLE, M_ENABLE);
		MedgeControl(MilEdgeContext, M_SAVE_ANGLE, M_ENABLE);

		fun_time_s = clock();
		MimHistogramEqualize(*m_MilManage.GetMilInspBuf(isGrabComp, nInspGrabIdx), image, M_RAYLEIGH, 1, teachPara.TeachInspThreshHold[ins_grab_idx][0], 255);
		fun_time_e = clock();
		fun_time = (fun_time_e - fun_time_s) / 1000.0;
#ifdef TEST
		strLog.Format(_T("MimHistogramEqualize() %f"), fun_time);
		WriteLog(strLog, TYPE_LOG_MAIN);

		tmpPath_Name.Format(_T("%sMimHistogramEqualize_%d_%d_0.bmp"), strTeachPath, nGrabIdx, nInspGrabIdx);
		MbufExport((char *)(LPCTSTR)tmpPath_Name, M_BMP, image);
#endif
		fun_time_s = clock();
		//MblobCalculate(MilTransImage, M_NULL, MilBlobFeatureList, MilBlobResult); 
		MblobCalculate(MilTransImage, image, MilBlobFeatureList, MilBlobResult); 
		fun_time_e = clock();
		fun_time = (fun_time_e - fun_time_s) / 1000.0;
#ifdef TEST
		strLog.Format(_T("MblobCalculate() %f"), fun_time);
		WriteLog(strLog, TYPE_LOG_MAIN);
#endif
		MblobGetNumber(MilBlobResult, &tot_blob);
		double *mean;
		mean = (double *)malloc(tot_blob*sizeof(double));

		MblobGetResult(MilBlobResult, M_MEAN_PIXEL, mean);

		free(mean);

		MgraColor(M_DEFAULT, M_COLOR_BLUE);
		MblobDraw(M_DEFAULT, MilBlobResult, *m_MilManage.GetMilOverlay(MAIN_DISP), M_DRAW_BLOBS_CONTOUR , M_INCLUDED_BLOBS, M_DEFAULT);

		high_thresh = teachPara.TeachInspThreshHold[ins_grab_idx][1];
		low_thresh = teachPara.TeachInspThreshHold[ins_grab_idx][2];

		MedgeControl(MilEdgeContext, M_THRESHOLD_HIGH,					 high_thresh);
		MedgeControl(MilEdgeContext, M_THRESHOLD_LOW,					 low_thresh);

		MedgeMask(MilEdgeContext, MilMaskImage, M_DEFAULT);

		fun_time_s = clock();
		/* Calculate edges and features. */
		MedgeCalculate(MilEdgeContext, image, M_NULL, M_NULL, M_NULL, MilEdgeResult, M_DEFAULT);
		fun_time_e = clock();
		fun_time = (fun_time_e - fun_time_s) / 1000.0;
#ifdef TEST
		strLog.Format(_T("MedgeCalculate() %f"), fun_time);
		WriteLog(strLog, TYPE_LOG_MAIN);
#endif
		MedgeSelect(MilEdgeResult, M_EXCLUDE, M_BOX_X_MIN, M_LESS_OR_EQUAL, rect.left, M_NULL);
		MedgeSelect(MilEdgeResult, M_EXCLUDE, M_BOX_Y_MIN, M_LESS_OR_EQUAL, rect.top, M_NULL);
		MedgeSelect(MilEdgeResult, M_EXCLUDE, M_BOX_X_MAX, M_GREATER_OR_EQUAL, rect.right, M_NULL);
		MedgeSelect(MilEdgeResult, M_EXCLUDE, M_BOX_Y_MAX, M_GREATER_OR_EQUAL, rect.bottom, M_NULL);
		//MedgeSelect(MilEdgeResult, M_EXCLUDE, M_FERET_X, M_LESS, spec, M_NULL); 
		//MedgeSelect(MilEdgeResult, M_EXCLUDE, M_FERET_Y, M_LESS, spec, M_NULL); 
		MedgeSelect(MilEdgeResult, M_EXCLUDE, M_ELLIPSE_FIT_MAJOR_AXIS , M_LESS, spec / 2.0, M_NULL);
		MedgeSelect(MilEdgeResult, M_EXCLUDE, M_ELLIPSE_FIT_MINOR_AXIS , M_LESS, spec / 2.0, M_NULL);
		MedgeSelect(MilEdgeResult, M_EXCLUDE, M_ELLIPSE_FIT_ERROR , M_GREATER_OR_EQUAL , 1.0, M_NULL);//20161017 ngh

		/* Get the number of edges found. */
		MedgeGetResult(MilEdgeResult, M_DEFAULT, M_NUMBER_OF_CHAINS+M_TYPE_LONG, &NumEdgeFound, M_NULL);
		//result

		if(NumEdgeFound > 0)
		{
			if ((CogX = (double *)malloc(NumEdgeFound*sizeof(double))) && 
				(CogY = (double *)malloc(NumEdgeFound*sizeof(double))) && 
				(Major = (double *)malloc(NumEdgeFound*sizeof(double))) &&
				(Minor = (double *)malloc(NumEdgeFound*sizeof(double))) &&
				(Ang = (double *)malloc(NumEdgeFound*sizeof(double))) &&
				(Width = (double *)malloc(NumEdgeFound*sizeof(double))) &&
				(Height = (double *)malloc(NumEdgeFound*sizeof(double))))
			{
				memset(CogX, NULL, NumEdgeFound*sizeof(double));
				memset(CogY, NULL, NumEdgeFound*sizeof(double));
				memset(Major, NULL, NumEdgeFound*sizeof(double));
				memset(Minor, NULL, NumEdgeFound*sizeof(double));
				memset(Ang, NULL, NumEdgeFound*sizeof(double));
				memset(Width, NULL, NumEdgeFound*sizeof(double));
				memset(CogX, NULL, NumEdgeFound*sizeof(double));

				MedgeGetResult(MilEdgeResult, M_DEFAULT, M_CENTER_OF_GRAVITY_X, CogX, M_NULL); 
				MedgeGetResult(MilEdgeResult, M_DEFAULT, M_CENTER_OF_GRAVITY_Y, CogY, M_NULL); 
				MedgeGetResult(MilEdgeResult, M_DEFAULT, M_FERET_X, Width, M_NULL); 
				MedgeGetResult(MilEdgeResult, M_DEFAULT, M_FERET_Y, Height, M_NULL); 
				MedgeGetResult(MilEdgeResult, M_DEFAULT, M_ELLIPSE_FIT_MAJOR_AXIS, Major, M_NULL);
				MedgeGetResult(MilEdgeResult, M_DEFAULT, M_ELLIPSE_FIT_MINOR_AXIS, Minor, M_NULL);
				MedgeGetResult(MilEdgeResult, M_DEFAULT, M_ELLIPSE_FIT_ANGLE, Ang, M_NULL);//20161017 ngh

				MgraColor(M_DEFAULT, M_COLOR_GREEN);
				MedgeDraw(M_DEFAULT, MilEdgeResult, *m_MilManage.GetMilOverlay(MAIN_DISP), M_DRAW_EDGES, M_DEFAULT, M_DEFAULT);

				for(int def_idx = 0; def_idx < NumEdgeFound; def_idx++)
				{
					m_InsRsltPara[isGrabComp].dCentX[m_InsRsltPara[isGrabComp].nNgCnt] = CogX[def_idx];
					m_InsRsltPara[isGrabComp].dCentY[m_InsRsltPara[isGrabComp].nNgCnt] = CogY[def_idx];
					m_InsRsltPara[isGrabComp].dWidth[m_InsRsltPara[isGrabComp].nNgCnt] = Width[def_idx];
					m_InsRsltPara[isGrabComp].dHeight[m_InsRsltPara[isGrabComp].nNgCnt] = Height[def_idx];
					m_InsRsltPara[isGrabComp].dLong[m_InsRsltPara[isGrabComp].nNgCnt] = Major[def_idx] * 2;
					m_InsRsltPara[isGrabComp].dShort[m_InsRsltPara[isGrabComp].nNgCnt] = Minor[def_idx] * 2;
					m_InsRsltPara[isGrabComp].dAng[m_InsRsltPara[isGrabComp].nNgCnt] = Ang[def_idx];//20161017 ngh

					m_InsRsltPara[isGrabComp].nDeviceIdx[m_InsRsltPara[isGrabComp].nNgCnt].SetPoint(nDeviceIdxX, (nDeviceIdxY + i));
					m_InsRsltPara[isGrabComp].nGrabIdx[m_InsRsltPara[isGrabComp].nNgCnt] = nGrabIdx;
					m_InsRsltPara[isGrabComp].nInspGrabIdx[m_InsRsltPara[isGrabComp].nNgCnt] = nInspGrabIdx;

					m_InsRsltPara[isGrabComp].bNg[m_InsRsltPara[isGrabComp].nNgCnt] = TRUE;

					m_InsRsltPara[isGrabComp].nNgCnt++;
				}
			}
			else
			{

			}

			nNG = INSPECTION_NG;
			if(nInspGrabIdx == 0)
			{
				m_Rslt.m_bInspectionRslt[0][i] = TRUE;
			}
			else
			{
				m_Rslt.m_bInspectionRslt[1][i] = TRUE;
			}

			free(CogX);
			free(CogY);
			free(Major);
			free(Minor);
			free(Ang);//20161017 ngh
			free(Width);
			free(Height);
		}

		/* Draw edges in the source image to show the result. */
		MgraColor(M_DEFAULT, M_COLOR_GREEN);
		MedgeDraw(M_DEFAULT, MilEdgeResult, *m_MilManage.GetMilOverlay(MAIN_DISP), M_DRAW_EDGES, M_DEFAULT, M_DEFAULT);

		MedgeFree(MilEdgeContext);
		MedgeFree(MilEdgeResult);

		MbufFree(MilBinImage);
		MbufFree(MilChildImage);

		MblobFree(MilBlobResult);
		MblobFree(MilBlobFeatureList);
#ifdef TEST
		strLog.Format(_T("InspectionEdge End_%d"), i);
		WriteLog(strLog, TYPE_LOG_MAIN);
#endif

	}

	MbufFree(MilTransImage);
	MbufFree(MilAlignImage);
	MbufFree(MilMaskImage);
	MbufFree(image);


	strLog.Format(_T("InspectionEdge1 End"));
	//WriteLog(strLog, TYPE_LOG_MAIN);
#endif	
	return nNG;
}

BOOL CLED_DisplayInspectDlg::BlobsCompare(int nDeviceIdxX, int nDeviceIdxY, int nInspGrabIdx, int nGrabIdx, double cent_x, double cent_y, double width, double height, double Long, double Short, double Breadth, double Ang, double distance, BOOL isGrabComp)
{
	BOOL rslt = FALSE;
	CdRect ng_rect1, ng_rect2, ng_rect3;

	ng_rect1.SetdRectCWH(cent_x, cent_y, width, height, distance);

	for(int i = 0; i < m_InsRsltPara[isGrabComp].nNgCnt; i++)
	{
		if((m_InsRsltPara[isGrabComp].nDeviceIdx[i].x == nDeviceIdxX) && (m_InsRsltPara[isGrabComp].nDeviceIdx[i].y == nDeviceIdxY))
		{
			if((m_InsRsltPara[isGrabComp].nGrabIdx[i] == nGrabIdx))
			{
				ng_rect2.SetdRectCWH(m_InsRsltPara[isGrabComp].dCentX[i], m_InsRsltPara[isGrabComp].dCentY[i], m_InsRsltPara[isGrabComp].dWidth[i], m_InsRsltPara[isGrabComp].dHeight[i], distance);
				if(ng_rect2.IntersectRect(ng_rect1) || ng_rect1.IntersectRect(ng_rect2))
				{
					m_InsRsltPara[isGrabComp].dCentX[i] = cent_x;
					m_InsRsltPara[isGrabComp].dCentY[i] = cent_y;
					m_InsRsltPara[isGrabComp].dWidth[i] = width;
					m_InsRsltPara[isGrabComp].dHeight[i] = height;
					m_InsRsltPara[isGrabComp].dLong[i] = Long;
					m_InsRsltPara[isGrabComp].dShort[i] = Short;
					m_InsRsltPara[isGrabComp].dBreadth[i] = Breadth;
					m_InsRsltPara[isGrabComp].dAng[i] = Ang;

					m_InsRsltPara[isGrabComp].nGrabIdx[i] = nGrabIdx;
					m_InsRsltPara[isGrabComp].nInspGrabIdx[i] = nInspGrabIdx;

					m_InsRsltPara[isGrabComp].bAlign[i] = FALSE;
					m_InsRsltPara[isGrabComp].bNg[i] = TRUE;
					rslt = TRUE;
					break;
				}
			}
		}
		if(rslt)
		{
			break;
		}
	}

	return rslt;
}

CRect CLED_DisplayInspectDlg::SetMoveRect(CRect *rect, int distance, int direction)//0 : orgin, 1 : left, 2 : top, 3 : right, 4 : bottom
{
	CRect _rect;
	switch(direction)
	{
	case 0:
		_rect.SetRect(rect->left, rect->top, rect->right, rect->bottom);
		break;
	case 1:
		_rect.SetRect(rect->left - distance, rect->top, rect->right - distance, rect->bottom);
		break;
	case 2:
		_rect.SetRect(rect->left, rect->top - distance, rect->right, rect->bottom - distance);
		break;
	case 3:
		_rect.SetRect(rect->left + distance, rect->top, rect->right + distance, rect->bottom);
		break;
	case 4:
		_rect.SetRect(rect->left, rect->top + distance, rect->right, rect->bottom + distance);
		break;
	}
	return _rect;
}

//20170102 ngh
int CLED_DisplayInspectDlg::InspectionBlobs3(int nDeviceIdxX, int nDeviceIdxY, int nGrabIdx, int nInspGrabIdx, BOOL isGrabComp)
{
	int nNG = 0;
	CString strLog;

	strLog.Format(_T("InspectionBlob3 Start"));
	WriteLog(strLog, TYPE_LOG_MAIN, FALSE);
#ifdef CAMERA
	CString tmpPath_Name, strTeachPath;
	MIL_ID MilMaskImage, MilAlignImage, MilTransImage, MilKernel;
	MIL_ID MilBlobResult, MilBlobFeatureList, MilBlobResult2, MilBlobFeatureList2;
	MIL_ID MilBinImage, MilChildImage;
	MIL_ID image1, image2, image3, image4, image5;
	MIL_ID rslt_image1, rslt_image2, rslt_image3, rslt_image4, rslt_image5;
	MIL_INT tot_tblob, tot_blob;
	long pitch;
	int threshhold = 0, idx = 2, ins_idx = nGrabIdx, ins_grab_idx = nInspGrabIdx, align_idx = 0;
	typeTeachPara teachPara;
	BYTE *by_source1, *by_source2, *by_source3, *by_source4;
	double *left, *right, *top, *bottom, *mean;
	int temp = 0;
	BOOL isImageSave = FALSE;

#ifdef TEST_INSP_DISP
	MbufCopy(*m_MilManage.GetMilInspBuf(isGrabComp, nInspGrabIdx), *m_MilManage.GetMilDispBuf(MAIN_DISP));
#endif

	MbufAlloc2d(*m_MilManage.GetMilSystem(), CAMERA_WIDTH, CAMERA_HEIGHT, 8+M_UNSIGNED, M_IMAGE+M_PROC, &image1);
	MbufAlloc2d(*m_MilManage.GetMilSystem(), CAMERA_WIDTH, CAMERA_HEIGHT, 8+M_UNSIGNED, M_IMAGE+M_PROC, &image2);
	MbufAlloc2d(*m_MilManage.GetMilSystem(), CAMERA_WIDTH, CAMERA_HEIGHT, 8+M_UNSIGNED, M_IMAGE+M_PROC, &image3);
	MbufAlloc2d(*m_MilManage.GetMilSystem(), CAMERA_WIDTH, CAMERA_HEIGHT, 8+M_UNSIGNED, M_IMAGE+M_PROC, &image4);
	MbufAlloc2d(*m_MilManage.GetMilSystem(), CAMERA_WIDTH, CAMERA_HEIGHT, 8+M_UNSIGNED, M_IMAGE+M_PROC, &image5);
	MbufAlloc2d(*m_MilManage.GetMilSystem(), CAMERA_WIDTH, CAMERA_HEIGHT, 8+M_UNSIGNED, M_IMAGE+M_PROC, &MilTransImage);
	MbufAlloc2d(*m_MilManage.GetMilSystem(), CAMERA_WIDTH, CAMERA_HEIGHT, 8+M_UNSIGNED, M_IMAGE+M_PROC, &MilMaskImage);
	MbufAlloc2d(*m_MilManage.GetMilSystem(), CAMERA_WIDTH, CAMERA_HEIGHT, 8+M_UNSIGNED, M_IMAGE+M_PROC, &MilAlignImage);
	MbufAlloc2d(*m_MilManage.GetMilSystem(), CAMERA_WIDTH, CAMERA_HEIGHT, 8+M_UNSIGNED, M_IMAGE+M_PROC, &rslt_image1);
	MbufAlloc2d(*m_MilManage.GetMilSystem(), CAMERA_WIDTH, CAMERA_HEIGHT, 8+M_UNSIGNED, M_IMAGE+M_PROC, &rslt_image2);
	MbufAlloc2d(*m_MilManage.GetMilSystem(), CAMERA_WIDTH, CAMERA_HEIGHT, 8+M_UNSIGNED, M_IMAGE+M_PROC, &rslt_image3);
	MbufAlloc2d(*m_MilManage.GetMilSystem(), CAMERA_WIDTH, CAMERA_HEIGHT, 8+M_UNSIGNED, M_IMAGE+M_PROC, &rslt_image4);
	MbufAlloc2d(*m_MilManage.GetMilSystem(), CAMERA_WIDTH, CAMERA_HEIGHT, 8+M_UNSIGNED, M_IMAGE+M_PROC, &rslt_image5);

	MbufInquire(image1, M_PITCH, &pitch);
	MbufInquire(image1, M_HOST_ADDRESS, &by_source1);
	MbufInquire(MilAlignImage, M_HOST_ADDRESS, &by_source2);
	MbufInquire(image3, M_HOST_ADDRESS, &by_source3);
	MbufInquire(image4, M_HOST_ADDRESS, &by_source4);

	//strLog.Format(_T("D:\\DGT_TEST_IMAGE\\0_1.bmp"));
	//MbufImport((char *)(LPCTSTR)strLog, M_BMP, M_LOAD, *m_MilManage.GetMilSystem(), &image1);
	MbufCopy(*m_MilManage.GetMilInspBuf(isGrabComp, nInspGrabIdx), image1);
	//MbufCopy(image1, image2);
	teachPara = m_dlgTeach->GetTeachPara();
	strTeachPath.Format("%s\\TeachImg", GetSelectModel().cPath);
	//불량 블롭 분리
	///////////////////////////////////
	for(int i = 0; i < teachPara.TeachRegionCnt[ins_idx][ins_grab_idx]; i++)
	{
		if((nDeviceIdxY + i) == teachPara.TeachArrayCntY)
		{
			continue;
		}

		if(nDeviceIdxY + i == teachPara.TeachArrayCntY)
		{
			nNG = INSPECTION_NG;
			m_Rslt.m_bInspectionRslt[0][i] = TRUE;
			m_Rslt.m_bInspectionRslt[1][i] = TRUE;
			continue;
		}

		if(!m_Rslt.m_bAlignInsp[i])//align 검사 확인
		{
			nNG = INSPECTION_NG;
			m_Rslt.m_bInspectionRslt[0][i] = TRUE;
			m_Rslt.m_bInspectionRslt[1][i] = TRUE;
			continue;
		}

		idx  = (i * 3) + 2;
		align_idx = (i * 3) + 1;
		CRect ng_rect;

		for(int k = 0; k < 2; k++)//원형 분리 시에 for(int k = 0; k < 2; k++) 로 변경
		{
			tmpPath_Name.Format(_T("BlobMask_%d_%d_%d_%d.bmp"), ins_idx, nInspGrabIdx, i, k);
			if(!FileSearch(strTeachPath, tmpPath_Name))//mask 이미지 검색
			{
				MgraColor(M_DEFAULT, M_COLOR_RED);
				strLog.Format(_T("Not Model Teaching"));
				CdRect rect;
				rect.SetdRectLTRB((int)teachPara.TeachRegion[ins_idx][ins_grab_idx][idx].GetLeft(),
					(int)teachPara.TeachRegion[ins_idx][ins_grab_idx][idx].GetTop(),
					(int)teachPara.TeachRegion[ins_idx][ins_grab_idx][idx].GetRight(),
					(int)teachPara.TeachRegion[ins_idx][ins_grab_idx][idx].GetBottom());
				MgraText(M_DEFAULT, *m_MilManage.GetMilOverlay(MAIN_DISP), rect.GetLeft() + 20, rect.GetTop() + 20, MIL_TEXT((char *)(LPCTSTR)strLog));
				strLog.Format(_T("Test,BlobMaskImageSearchFail_%d"), i);
				WriteLog(strLog, TYPE_LOG_MAIN);
				continue;
			}

			MbufClear(MilMaskImage, 0xFF);
			MbufClear(MilAlignImage, 0xFF);

			tmpPath_Name.Format(_T("%s\\BlobMask_%d_%d_%d_%d.bmp"), strTeachPath, ins_idx, nInspGrabIdx, i, k);
			MbufImport((char *)(LPCTSTR)tmpPath_Name, M_BMP, M_LOAD, *m_MilManage.GetMilSystem(), &MilMaskImage);	

			double temp_x, temp_y;

			temp_x = teachPara.AlignPara.dCentX[i + (m_dlgTeach->GetTeachPara().TeachGrabCnt * nGrabIdx)] - teachPara.AlignPara.dX[align_idx + nGrabIdx];
			temp_y = teachPara.AlignPara.dCentY[i + (m_dlgTeach->GetTeachPara().TeachGrabCnt * nGrabIdx)] - teachPara.AlignPara.dY[align_idx + nGrabIdx];

			//mask image align 값으로 보정
			MimRotate(MilMaskImage, MilAlignImage, teachPara.AlignPara.dAng[align_idx + nGrabIdx + i], 
				teachPara.AlignPara.dCentX[i + (m_dlgTeach->GetTeachPara().TeachGrabCnt * nGrabIdx)],
				teachPara.AlignPara.dCentY[i + (m_dlgTeach->GetTeachPara().TeachGrabCnt * nGrabIdx)],
				teachPara.AlignPara.dX[align_idx + nGrabIdx + i], teachPara.AlignPara.dY[align_idx + nGrabIdx + i], M_DEFAULT);
			strLog.Format(_T("AlignImage Rotate"));
			WriteLog(strLog, TYPE_LOG_MAIN, FALSE);

			if(k == 1)
			{
				if(teachPara.TeachCircleErodeDistance[nInspGrabIdx] > 0.)
				{
					MimDilate(MilAlignImage, MilAlignImage, teachPara.TeachCircleErodeDistance[nInspGrabIdx], M_BINARY);
				}
				else
				{
					MimDilate(MilAlignImage, MilAlignImage, 5, M_BINARY);
				}
			}

			CRect rect((int)teachPara.TeachRegion[ins_idx][ins_grab_idx][idx].GetLeft(),
				(int)teachPara.TeachRegion[ins_idx][ins_grab_idx][idx].GetTop(),
				(int)teachPara.TeachRegion[ins_idx][ins_grab_idx][idx].GetRight(),
				(int)teachPara.TeachRegion[ins_idx][ins_grab_idx][idx].GetBottom());
			MbufClear(MilTransImage, 0x00);

			/* Allocate a blob result buffer. */
			MblobAllocResult(*m_MilManage.GetMilSystem(), &MilBlobResult); 

			/* Allocate a feature list. */ 
			MblobAllocFeatureList(*m_MilManage.GetMilSystem(), &MilBlobFeatureList); 

			MblobSelectFeature(MilBlobFeatureList, M_MEAN_PIXEL);
			MblobSelectFeature(MilBlobFeatureList, M_BOX_X_MAX);
			MblobSelectFeature(MilBlobFeatureList, M_BOX_Y_MAX);
			MblobSelectFeature(MilBlobFeatureList, M_BOX_X_MIN);
			MblobSelectFeature(MilBlobFeatureList, M_BOX_Y_MIN);

			//align image 반전
			for(int h = 0; h < CAMERA_HEIGHT; h++)
			{
				for(int w = 0; w < CAMERA_WIDTH; w++)
				{
					if(by_source2[w + (h * pitch)] == 0)
					{
						by_source2[w + (h * pitch)] = 255;
					}
					else
					{
						by_source2[w + (h * pitch)] = 0;
					}
				}
			}

			/*
			MimConvolve(*m_MilManage.GetMilInspBuf(isGrabComp, nInspGrabIdx), image5, M_SHEN_FILTER(M_SHARPEN, 30));

			if(isImageSave)
			{
				tmpPath_Name.Format(_T("%s\\Insp_Rslt3_%d_%d_%d.bmp"), strTeachPath, ins_idx, i, k);
				MbufExport((char *)(LPCTSTR)tmpPath_Name, M_BMP, image5);
			}
			*/

			MblobCalculate(MilAlignImage, *m_MilManage.GetMilInspBuf(isGrabComp, nInspGrabIdx), MilBlobFeatureList, MilBlobResult); 

			MblobSelect(MilBlobResult, M_EXCLUDE, M_BOX_X_MIN, M_LESS_OR_EQUAL, rect.left, M_NULL);
			MblobSelect(MilBlobResult, M_EXCLUDE, M_BOX_Y_MIN, M_LESS_OR_EQUAL, rect.top, M_NULL);
			MblobSelect(MilBlobResult, M_EXCLUDE, M_BOX_X_MAX, M_GREATER_OR_EQUAL, rect.right, M_NULL);
			MblobSelect(MilBlobResult, M_EXCLUDE, M_BOX_Y_MAX, M_GREATER_OR_EQUAL, rect.bottom, M_NULL);

			MblobGetNumber(MilBlobResult, &tot_tblob);

			if(tot_tblob < 1)
			{
				MblobFree(MilBlobResult);
				MblobFree(MilBlobFeatureList);
				continue;
			}
			strLog.Format(_T("TotBlob = %d, Blobs Insp_%d_%d"), tot_tblob, i, k);
			WriteLog(strLog, TYPE_LOG_MAIN, FALSE);

			MgraColor(M_DEFAULT, M_COLOR_BLUE);
			MblobDraw(M_DEFAULT, MilBlobResult, *m_MilManage.GetMilOverlay(MAIN_DISP), M_DRAW_BLOBS_CONTOUR, M_DEFAULT, M_DEFAULT);

			//blob 별로 평균 밝기값, left, top, right, bottom 추출
			mean = (double *)malloc(tot_tblob*sizeof(double));
			left = (double *)malloc(tot_tblob*sizeof(double));
			top = (double *)malloc(tot_tblob*sizeof(double));
			right = (double *)malloc(tot_tblob*sizeof(double));
			bottom = (double *)malloc(tot_tblob*sizeof(double));

			MblobGetResult(MilBlobResult, M_MEAN_PIXEL, mean);
			MblobGetResult(MilBlobResult, M_BOX_X_MIN, left);
			MblobGetResult(MilBlobResult, M_BOX_Y_MIN, top);
			MblobGetResult(MilBlobResult, M_BOX_X_MAX, right);
			MblobGetResult(MilBlobResult, M_BOX_Y_MAX, bottom);

			double dMean = 0., dSum = 0.;

			for(int j = 0; j < tot_tblob; j++)
			{
				dSum+=mean[j];
			}

			MbufCopy(*m_MilManage.GetMilInspBuf(isGrabComp, nInspGrabIdx), image1);
			MimConvolve(image1, image1, M_SHEN_FILTER(M_SHARPEN, 30));

			dMean = dSum / tot_tblob;

			for(int h = 0; h < CAMERA_HEIGHT; h++)
			{
				for(int w = 0; w < CAMERA_WIDTH; w++)
				{
					if(by_source2[w + (h * pitch)] == 0)
					{
						by_source1[w + (h * pitch)] = (BYTE)dMean;
					}
				}
			}

			if(isImageSave)
			{
				tmpPath_Name.Format(_T("%s\\Insp_Rslt1_%d_%d_%d.bmp"), strTeachPath, ins_idx, i, k);
				MbufExport((char *)(LPCTSTR)tmpPath_Name, M_BMP, image1);
			}

			MbufAlloc2d(*m_MilManage.GetMilSystem(), KERNEL_WIDTH, KERNEL_HEIGHT, KERNEL_DEPTH+M_UNSIGNED, M_KERNEL, &MilKernel);
	
			MbufPut(MilKernel,KernelData);
			MbufControlNeighborhood(MilKernel, M_NORMALIZATION_FACTOR, 800);
			MimConvolve(image1, image2, MilKernel);

			MbufControlNeighborhood(MilKernel, M_OVERSCAN, M_DISABLE);
			MimConvolve(image2, image3, MilKernel);  

			for(int h = 0; h < CAMERA_HEIGHT; h++)
			{
				for(int w = 0; w < CAMERA_WIDTH; w++)
				{
					temp = (int)by_source1[w + (h * pitch)] - (int)by_source3[w + (h * pitch)];
					temp = (int)(((double)temp * 0.8) + 128);
		
					by_source4[w + (h * pitch)] = temp;
				}
			}

			if(isImageSave)
			{
				tmpPath_Name.Format(_T("%s\\Insp_Rslt2_%d_%d_%d.bmp"), strTeachPath, ins_idx, i, k);
				MbufExport((char *)(LPCTSTR)tmpPath_Name, M_BMP, image4);
			}

			MblobFree(MilBlobResult);
			MblobFree(MilBlobFeatureList);

			for(int l = 0; l < 2; l++)
			{
				MbufAlloc2d(*m_MilManage.GetMilSystem(), rect.Width(), rect.Height(), 8+M_UNSIGNED, M_IMAGE+M_PROC, &MilBinImage);
				MbufClear(MilBinImage, 0x00);

				MbufChild2d(image4, rect.left, rect.top, rect.Width(), rect.Height(), &MilChildImage);

				if(l == 1)
				{
					threshhold = /*(int)dMean*/128 - (int)teachPara.TeachInspThreshHold[ins_grab_idx][l + 1];
					if(threshhold < 20)
					{
						threshhold = 20;
					}
					MimBinarize(MilChildImage, MilBinImage, M_FIXED + M_IN_RANGE, 0.0, threshhold);
				}
				else
				{
					threshhold = /*(int)dMean*/128 + (int)teachPara.TeachInspThreshHold[ins_grab_idx][l + 1];
					if(threshhold > 240)
					{
						threshhold = 240;
					}
					MimBinarize(MilChildImage, MilBinImage, M_FIXED + M_IN_RANGE, threshhold, 255);
				}

				//MimTranslate(MilBinImage, MilTransImage, _rect.left, _rect.top, M_DEFAULT);
				MimTranslate(MilBinImage, MilTransImage, rect.left, rect.top, M_DEFAULT);

				/* Allocate a blob result buffer. */
				MblobAllocResult(*m_MilManage.GetMilSystem(), &MilBlobResult2); 

				/* Allocate a feature list. */ 
				MblobAllocFeatureList(*m_MilManage.GetMilSystem(), &MilBlobFeatureList2); 

				MblobSelectFeature(MilBlobFeatureList2, M_AREA); 
				MblobSelectFeature(MilBlobFeatureList2, M_CENTER_OF_GRAVITY); 
				MblobSelectFeature(MilBlobFeatureList2, M_FERET_X); 
				MblobSelectFeature(MilBlobFeatureList2, M_FERET_Y);
				MblobSelectFeature(MilBlobFeatureList2, M_BOX_X_MAX);
				MblobSelectFeature(MilBlobFeatureList2, M_BOX_Y_MAX);
				MblobSelectFeature(MilBlobFeatureList2, M_BOX_X_MIN);
				MblobSelectFeature(MilBlobFeatureList2, M_BOX_Y_MIN);
				MblobSelectFeature(MilBlobFeatureList2, M_FERET_MIN_DIAMETER);
				MblobSelectFeature(MilBlobFeatureList2, M_FERET_MAX_DIAMETER);
				MblobSelectFeature(MilBlobFeatureList2, M_FERET_MIN_ANGLE);
				MblobSelectFeature(MilBlobFeatureList2, M_FERET_MAX_ANGLE);
				MblobSelectFeature(MilBlobFeatureList2, M_BREADTH);
				

				MblobCalculate(MilTransImage, image4, MilBlobFeatureList2, MilBlobResult2); 

				//검사영역 외부는 제외
				//////////////////////////////
				MblobSelect(MilBlobResult2, M_DELETE, M_BOX_X_MIN, M_LESS_OR_EQUAL, rect.left, M_NULL);
				MblobSelect(MilBlobResult2, M_DELETE, M_BOX_Y_MIN, M_LESS_OR_EQUAL, rect.top, M_NULL);
				MblobSelect(MilBlobResult2, M_DELETE, M_BOX_X_MAX, M_GREATER_OR_EQUAL, rect.right, M_NULL);
				MblobSelect(MilBlobResult2, M_DELETE, M_BOX_Y_MAX, M_GREATER_OR_EQUAL, rect.bottom, M_NULL);
				//////////////////////////////

				//검사 spec보다 작은 것 제외
				//////////////////////////////
				//MblobSelect(MilBlobResult2, M_DELETE, M_AREA, M_LESS_OR_EQUAL, 1, M_NULL);//area
				//MblobSelect(MilBlobResult2, M_DELETE, M_FERET_MIN_DIAMETER, M_LESS_OR_EQUAL, 2.0, M_NULL);//smallist rect short
				//MblobSelect(MilBlobResult2, M_DELETE, M_FERET_MAX_DIAMETER, M_LESS_OR_EQUAL, 1, M_NULL);//smallist rect long
				//MblobSelect(MilBlobResult2, M_DELETE, M_BREADTH, M_LESS_OR_EQUAL, 1.9, M_NULL);//breadth
				//////////////////////////////

				MblobGetNumber(MilBlobResult2, &tot_blob);

				if(tot_blob == 0)
				{
					MblobFree(MilBlobResult2);
					MblobFree(MilBlobFeatureList2);
					MbufFree(MilChildImage);
					MbufFree(MilBinImage);
					continue;
				}

				MbufClear(MilMaskImage, 0x00);
				MgraColor(M_DEFAULT, M_COLOR_WHITE);
				MblobDraw(M_DEFAULT, MilBlobResult2, MilMaskImage, M_DRAW_BLOBS , M_INCLUDED_BLOBS, M_DEFAULT);

				if(l == 0)
				{
					MbufClear(rslt_image1, 0x00);
					MgraColor(M_DEFAULT, M_COLOR_WHITE);
					MblobDraw(M_DEFAULT, MilBlobResult2, rslt_image1, M_DRAW_BLOBS , M_INCLUDED_BLOBS, M_DEFAULT);
				}
				else
				{
					MbufClear(rslt_image2, 0x00);
					MgraColor(M_DEFAULT, M_COLOR_WHITE);
					MblobDraw(M_DEFAULT, MilBlobResult2, rslt_image2, M_DRAW_BLOBS , M_INCLUDED_BLOBS, M_DEFAULT);
				}

				//MgraColor(M_DEFAULT, M_COLOR_RED);
				//MblobDraw(M_DEFAULT, MilBlobResult2, *m_MilManage.GetMilOverlay(MAIN_DISP), M_DRAW_BLOBS , M_INCLUDED_BLOBS, M_DEFAULT);

				MbufFree(MilChildImage);
				MbufFree(MilBinImage);
				MblobFree(MilBlobResult2);
				MblobFree(MilBlobFeatureList2);
			}

			MbufFree(MilKernel);

			free(mean);
			free(left);
			free(top);
			free(right);
			free(bottom);			
		}

		if(i == 0)
		{
			MbufClear(rslt_image3, 0x00);
			MimArith(rslt_image1, rslt_image2, rslt_image3, M_OR);
		}
		else
		{
			MbufClear(rslt_image4, 0x00);
			MimArith(rslt_image1, rslt_image2, rslt_image4, M_OR);
		}

	}

	MimArith(rslt_image3, rslt_image4, rslt_image5, M_OR);

	//tmpPath_Name.Format(_T("%s\\Insp_Rslt3_%d.bmp"), strTeachPath, ins_idx);
	//MbufExport((char *)(LPCTSTR)tmpPath_Name, M_BMP, rslt_image5);

	//불량 이미지 저장
	tmpPath_Name.Format(_T("%s\\InspectionBlob_Rslt.bmp"), strTeachPath);
	MbufExport((char *)(LPCTSTR)tmpPath_Name, M_BMP, rslt_image5);

	MbufFree(MilMaskImage);
	MbufFree(MilAlignImage);
	MbufFree(MilTransImage);
	MbufFree(image5);
	MbufFree(image4);
	MbufFree(image3);
	MbufFree(image2);
	MbufFree(image1);
	MbufFree(rslt_image1);
	MbufFree(rslt_image2);
	MbufFree(rslt_image3);
	MbufFree(rslt_image4);
	MbufFree(rslt_image5);
#endif

	strLog.Format(_T("InspectionBlob3 End"));
	WriteLog(strLog, TYPE_LOG_MAIN, FALSE);

	return nNG;
}


//20170102 ngh
int CLED_DisplayInspectDlg::InspectionBlobs2(int nDeviceIdxX, int nDeviceIdxY, int nGrabIdx, int nInspGrabIdx, BOOL isGrabComp)
{
	int nNG = 0;
	CString strLog;

	strLog.Format(_T("InspectionBlob2 Start"));
	WriteLog(strLog, TYPE_LOG_MAIN, FALSE);
#ifdef CAMERA
	MIL_ID image, image1, image2, image3, image4, image5;
	MIL_ID MilMaskImage, MilMaskImage2, MilMaskImage3, MilMaskImage4;
	MIL_ID MilAlignImage, MilAndImage;
	MIL_ID MilWhiteImage, MilBlackImage, MilDilImage, MilLineImage;
	MIL_ID MilRsltImage, MilRsltImage2, MilRsltImage3, MilRsltImage4;
	MIL_ID MilTransImage, MilBinImage, MilChildImage, MilTransImage2, MilBinImage2, MilChildImage2;
	MIL_ID MilBlobResult, MilBlobFeatureList, MilBlobResult2, MilBlobFeatureList2, MilBlobResult3, MilBlobFeatureList3;
	MIL_INT tot_blob, tot_tblob;

	CString strTeachPath, tmpPath_Name, image_file_name;
	typeTeachPara teachPara;
	BYTE *by_source, *by_source2, *by_source3, *by_source4, *by_source5, *by_source6, *by_source7, *by_source8, *by_source9, *by_source10, *by_source11, *by_source12;
	long pitch, NumResults = 0;	
	int threshhold, idx = 2, ins_idx = nGrabIdx, ins_grab_idx = nInspGrabIdx, align_idx = 0;
	double  spec, short_spec;
	double offset;
	double *CogX, *CogY, *Width, *Height, *Short, *Long, *Ang, *Breadth;//20161017 ngh
	double *left, *right, *top, *bottom, *mean;
	BOOL isImageSave = FALSE;
#ifdef TEST_INSP_DISP
	MbufCopy(*m_MilManage.GetMilInspBuf(isGrabComp, nInspGrabIdx), *m_MilManage.GetMilDispBuf(MAIN_DISP));
#endif

	SYSTEMTIME	lpSystemTime;
	GetLocalTime(&lpSystemTime);

	teachPara = m_dlgTeach->GetTeachPara();
	isImageSave = m_dlgData->GetDataPara().DataInspRsltImageSave;
	spec = (double)teachPara.TeachSpec / (double)teachPara.TeachPixelSize;
	short_spec = (double)teachPara.TeachShortSpec / (double)teachPara.TeachPixelSize;;
	offset = (double)teachPara.TeachRsltOffSet / (double)teachPara.TeachPixelSize;
	m_RsltIdx = 0;

	strTeachPath.Format(_T("%s\\TeachImg\\"), m_SelectModel.cPath);

	//image buffer init
	MbufAlloc2d(*m_MilManage.GetMilSystem(), CAMERA_WIDTH, CAMERA_HEIGHT, 8+M_UNSIGNED, M_IMAGE+M_PROC, &image);
	MbufAlloc2d(*m_MilManage.GetMilSystem(), CAMERA_WIDTH, CAMERA_HEIGHT, 8+M_UNSIGNED, M_IMAGE+M_PROC, &image1);
	MbufAlloc2d(*m_MilManage.GetMilSystem(), CAMERA_WIDTH, CAMERA_HEIGHT, 8+M_UNSIGNED, M_IMAGE+M_PROC, &image2);
	MbufAlloc2d(*m_MilManage.GetMilSystem(), CAMERA_WIDTH, CAMERA_HEIGHT, 8+M_UNSIGNED, M_IMAGE+M_PROC, &image3);
	MbufAlloc2d(*m_MilManage.GetMilSystem(), CAMERA_WIDTH, CAMERA_HEIGHT, 8+M_UNSIGNED, M_IMAGE+M_PROC, &image4);
	MbufAlloc2d(*m_MilManage.GetMilSystem(), CAMERA_WIDTH, CAMERA_HEIGHT, 8+M_UNSIGNED, M_IMAGE+M_PROC, &image5);
	MbufAlloc2d(*m_MilManage.GetMilSystem(), CAMERA_WIDTH, CAMERA_HEIGHT, 8+M_UNSIGNED, M_IMAGE+M_PROC, &MilTransImage);
	MbufAlloc2d(*m_MilManage.GetMilSystem(), CAMERA_WIDTH, CAMERA_HEIGHT, 8+M_UNSIGNED, M_IMAGE+M_PROC, &MilTransImage2);
	MbufAlloc2d(*m_MilManage.GetMilSystem(), CAMERA_WIDTH, CAMERA_HEIGHT, 8+M_UNSIGNED, M_IMAGE+M_PROC, &MilMaskImage);
	MbufAlloc2d(*m_MilManage.GetMilSystem(), CAMERA_WIDTH, CAMERA_HEIGHT, 8+M_UNSIGNED, M_IMAGE+M_PROC, &MilMaskImage2);
	MbufAlloc2d(*m_MilManage.GetMilSystem(), CAMERA_WIDTH, CAMERA_HEIGHT, 8+M_UNSIGNED, M_IMAGE+M_PROC, &MilMaskImage3);
	MbufAlloc2d(*m_MilManage.GetMilSystem(), CAMERA_WIDTH, CAMERA_HEIGHT, 8+M_UNSIGNED, M_IMAGE+M_PROC, &MilMaskImage4);
	MbufAlloc2d(*m_MilManage.GetMilSystem(), CAMERA_WIDTH, CAMERA_HEIGHT, 8+M_UNSIGNED, M_IMAGE+M_PROC, &MilAlignImage);
	MbufAlloc2d(*m_MilManage.GetMilSystem(), CAMERA_WIDTH, CAMERA_HEIGHT, 8+M_UNSIGNED, M_IMAGE+M_PROC, &MilAndImage);
	MbufAlloc2d(*m_MilManage.GetMilSystem(), CAMERA_WIDTH, CAMERA_HEIGHT, 8+M_UNSIGNED, M_IMAGE+M_PROC, &MilRsltImage);
	MbufAlloc2d(*m_MilManage.GetMilSystem(), CAMERA_WIDTH, CAMERA_HEIGHT, 8+M_UNSIGNED, M_IMAGE+M_PROC, &MilRsltImage2);
	MbufAlloc2d(*m_MilManage.GetMilSystem(), CAMERA_WIDTH, CAMERA_HEIGHT, 8+M_UNSIGNED, M_IMAGE+M_PROC, &MilRsltImage3);
	MbufAlloc2d(*m_MilManage.GetMilSystem(), CAMERA_WIDTH, CAMERA_HEIGHT, 8+M_UNSIGNED, M_IMAGE+M_PROC, &MilRsltImage4);
	MbufAlloc2d(*m_MilManage.GetMilSystem(), CAMERA_WIDTH, CAMERA_HEIGHT, 8+M_UNSIGNED, M_IMAGE+M_PROC, &MilWhiteImage);
	MbufAlloc2d(*m_MilManage.GetMilSystem(), CAMERA_WIDTH, CAMERA_HEIGHT, 8+M_UNSIGNED, M_IMAGE+M_PROC, &MilBlackImage);
	MbufAlloc2d(*m_MilManage.GetMilSystem(), CAMERA_WIDTH, CAMERA_HEIGHT, 8+M_UNSIGNED, M_IMAGE+M_PROC, &MilDilImage);
	MbufAlloc2d(*m_MilManage.GetMilSystem(), CAMERA_WIDTH, CAMERA_HEIGHT, 8+M_UNSIGNED, M_IMAGE+M_PROC, &MilLineImage);

	MbufClear(MilRsltImage, 0xFF);
	MbufClear(MilRsltImage2, 0xFF);
	MbufClear(MilAlignImage, 0xFF);
	MbufCopy(*m_MilManage.GetMilInspBuf(isGrabComp, nInspGrabIdx), image);
	MbufCopy(*m_MilManage.GetMilInspBuf(isGrabComp, nInspGrabIdx), image3);


	//image buffer inquire
	MbufInquire(MilMaskImage, M_HOST_ADDRESS, &by_source);
	MbufInquire(MilAlignImage, M_HOST_ADDRESS, &by_source2);
	MbufInquire(MilRsltImage2, M_HOST_ADDRESS, &by_source3);
	MbufInquire(MilRsltImage4, M_HOST_ADDRESS, &by_source4);
	MbufInquire(image, M_HOST_ADDRESS, &by_source5);
	MbufInquire(image2, M_HOST_ADDRESS, &by_source7);
	MbufInquire(MilWhiteImage, M_HOST_ADDRESS, &by_source8);
	MbufInquire(MilBlackImage, M_HOST_ADDRESS, &by_source9);
	MbufInquire(MilDilImage, M_HOST_ADDRESS, &by_source10);
	MbufInquire(MilLineImage, M_HOST_ADDRESS, &by_source11);
	MbufInquire(image5, M_HOST_ADDRESS, &by_source12);
	MbufInquire(MilMaskImage, M_PITCH, &pitch);


	MbufInquire(MilMaskImage, M_PITCH, &pitch);

	MbufInquire(image, M_HOST_ADDRESS, &by_source6);

	for(int h = 0; h < CAMERA_HEIGHT; h++)
	{
		for(int w = 0; w < CAMERA_WIDTH; w++)
		{
			by_source6[w + (h * pitch)] = 255 - by_source6[w + (h * pitch)];
		}
	}

	MimArith(image, image3, image1, M_SUB_CONST);
	MimArith(image1, image3, image2, M_ADD_CONST);
	MbufClear(image, 0x00);
	MbufCopy(image2, image);
	MbufCopy(image, MilWhiteImage);
	MbufCopy(image, MilBlackImage);

	//불량 블롭 분리
	///////////////////////////////////
	for(int i = 0; i < teachPara.TeachRegionCnt[ins_idx][ins_grab_idx]; i++)
	{
		if((nDeviceIdxY + i) == teachPara.TeachArrayCntY)
		{
			continue;
		}

		if(nDeviceIdxY + i == teachPara.TeachArrayCntY)
		{
			nNG = INSPECTION_NG;
			m_Rslt.m_bInspectionRslt[0][i] = TRUE;
			m_Rslt.m_bInspectionRslt[1][i] = TRUE;
			continue;
		}

		if(!m_Rslt.m_bAlignInsp[i])//align 검사 확인
		{
			nNG = INSPECTION_NG;
			m_Rslt.m_bInspectionRslt[0][i] = TRUE;
			m_Rslt.m_bInspectionRslt[1][i] = TRUE;
			continue;
		}

		idx  = (i * 3) + 2;
		align_idx = (i * 3) + 1;
		CRect ng_rect;

		for(int k = 0; k < 1; k++)//원형 분리 시에 for(int k = 0; k < 2; k++) 로 변경
		{
			tmpPath_Name.Format(_T("BlobMask_%d_%d_%d_%d.bmp"), ins_idx, nInspGrabIdx, i, k);
			if(!FileSearch(strTeachPath, tmpPath_Name))//mask 이미지 검색
			{
				MgraColor(M_DEFAULT, M_COLOR_RED);
				strLog.Format(_T("Not Model Teaching"));
				CdRect rect;
				rect.SetdRectLTRB((int)teachPara.TeachRegion[ins_idx][ins_grab_idx][idx].GetLeft(),
					(int)teachPara.TeachRegion[ins_idx][ins_grab_idx][idx].GetTop(),
					(int)teachPara.TeachRegion[ins_idx][ins_grab_idx][idx].GetRight(),
					(int)teachPara.TeachRegion[ins_idx][ins_grab_idx][idx].GetBottom());
				MgraText(M_DEFAULT, *m_MilManage.GetMilOverlay(MAIN_DISP), rect.GetLeft() + 20, rect.GetTop() + 20, MIL_TEXT((char *)(LPCTSTR)strLog));
				strLog.Format(_T("Test,BlobMaskImageSearchFail_%d"), i);
				WriteLog(strLog, TYPE_LOG_MAIN);
				continue;
			}

			MbufClear(MilMaskImage, 0xFF);
			MbufClear(MilAlignImage, 0xFF);

			tmpPath_Name.Format(_T("%sBlobMask_%d_%d_%d_%d.bmp"), strTeachPath, ins_idx, nInspGrabIdx, i, k);
			MbufImport((char *)(LPCTSTR)tmpPath_Name, M_BMP, M_LOAD, *m_MilManage.GetMilSystem(), &MilMaskImage);	

			double temp_x, temp_y;

			temp_x = teachPara.AlignPara.dCentX[i + (m_dlgTeach->GetTeachPara().TeachGrabCnt * nGrabIdx)] - teachPara.AlignPara.dX[align_idx + nGrabIdx];
			temp_y = teachPara.AlignPara.dCentY[i + (m_dlgTeach->GetTeachPara().TeachGrabCnt * nGrabIdx)] - teachPara.AlignPara.dY[align_idx + nGrabIdx];

			//mask image align 값으로 보정
			MimRotate(MilMaskImage, MilAlignImage, teachPara.AlignPara.dAng[align_idx + nGrabIdx + i], 
				teachPara.AlignPara.dCentX[i + (m_dlgTeach->GetTeachPara().TeachGrabCnt * nGrabIdx)],
				teachPara.AlignPara.dCentY[i + (m_dlgTeach->GetTeachPara().TeachGrabCnt * nGrabIdx)],
				teachPara.AlignPara.dX[align_idx + nGrabIdx + i], teachPara.AlignPara.dY[align_idx + nGrabIdx + i], M_DEFAULT);

			if(teachPara.TeachCircleErodeDistance[nInspGrabIdx] > 0.)
			{
				MimDilate(MilAlignImage, MilAlignImage, teachPara.TeachCircleErodeDistance[nInspGrabIdx], M_BINARY);
				//MimErode(MilAlignImage, MilAlignImage, teachPara.TeachCircleErodeDistance[nInspGrabIdx], M_BINARY);
			}

			CRect rect((int)teachPara.TeachRegion[ins_idx][ins_grab_idx][idx].GetLeft(),
				(int)teachPara.TeachRegion[ins_idx][ins_grab_idx][idx].GetTop(),
				(int)teachPara.TeachRegion[ins_idx][ins_grab_idx][idx].GetRight(),
				(int)teachPara.TeachRegion[ins_idx][ins_grab_idx][idx].GetBottom());
			MbufClear(MilTransImage, 0x00);

			/* Allocate a blob result buffer. */
			MblobAllocResult(*m_MilManage.GetMilSystem(), &MilBlobResult2); 

			/* Allocate a feature list. */ 
			MblobAllocFeatureList(*m_MilManage.GetMilSystem(), &MilBlobFeatureList2); 

			MblobSelectFeature(MilBlobFeatureList2, M_MEAN_PIXEL);
			MblobSelectFeature(MilBlobFeatureList2, M_BOX_X_MAX);
			MblobSelectFeature(MilBlobFeatureList2, M_BOX_Y_MAX);
			MblobSelectFeature(MilBlobFeatureList2, M_BOX_X_MIN);
			MblobSelectFeature(MilBlobFeatureList2, M_BOX_Y_MIN);

			MimDilate(MilAlignImage, MilDilImage, 2, M_BINARY);
			MbufCopy(MilAlignImage, MilLineImage);
			for(int h = 0; h < CAMERA_HEIGHT; h++)
			{
				for(int w = 0; w < CAMERA_WIDTH; w++)
				{
					if(by_source10[w + (h * pitch)] == 0)
					{
						by_source11[w + (h * pitch)] = 255;
					}
				}
			}

			for(int h = 0; h < CAMERA_HEIGHT; h++)
			{
				for(int w = 0; w < CAMERA_WIDTH; w++)
				{
					if(by_source11[w + (h * pitch)] == 0)
					{
						by_source8[w + (h * pitch)]+=40;
						by_source9[w + (h * pitch)]-=40;
					}
				}
			}

			//align image 반전
			for(int h = 0; h < CAMERA_HEIGHT; h++)
			{
				for(int w = 0; w < CAMERA_WIDTH; w++)
				{
					if(by_source2[w + (h * pitch)] == 0)
					{
						by_source2[w + (h * pitch)] = 255;
					}
					else
					{
						by_source2[w + (h * pitch)] = 0;
					}
				}
			}

			//MimConvolve(image2, image2, M_SHEN_FILTER(M_SHARPEN, 30));

			MblobCalculate(MilAlignImage, image2, MilBlobFeatureList2, MilBlobResult2); 

			//MblobSelect(MilBlobResult2, M_EXCLUDE, M_BOX_X_MIN, M_LESS_OR_EQUAL, rect.left, M_NULL);
			//MblobSelect(MilBlobResult2, M_EXCLUDE, M_BOX_Y_MIN, M_LESS_OR_EQUAL, rect.top, M_NULL);
			//MblobSelect(MilBlobResult2, M_EXCLUDE, M_BOX_X_MAX, M_GREATER_OR_EQUAL, rect.right, M_NULL);
			//MblobSelect(MilBlobResult2, M_EXCLUDE, M_BOX_Y_MAX, M_GREATER_OR_EQUAL, rect.bottom, M_NULL);

			MblobGetNumber(MilBlobResult2, &tot_tblob);

			if(tot_tblob < 1)
			{
				MblobFree(MilBlobResult2);
				MblobFree(MilBlobFeatureList2);
				continue;
			}

			MgraColor(M_DEFAULT, M_COLOR_CYAN);
			MblobDraw(M_DEFAULT, MilBlobResult2, *m_MilManage.GetMilOverlay(MAIN_DISP), M_DRAW_BLOBS_CONTOUR, M_DEFAULT, M_DEFAULT);

			//blob 별로 평균 밝기값, left, top, right, bottom 추출
			mean = (double *)malloc(tot_tblob*sizeof(double));
			left = (double *)malloc(tot_tblob*sizeof(double));
			top = (double *)malloc(tot_tblob*sizeof(double));
			right = (double *)malloc(tot_tblob*sizeof(double));
			bottom = (double *)malloc(tot_tblob*sizeof(double));

			MblobGetResult(MilBlobResult2, M_MEAN_PIXEL, mean);
			MblobGetResult(MilBlobResult2, M_BOX_X_MIN, left);
			MblobGetResult(MilBlobResult2, M_BOX_Y_MIN, top);
			MblobGetResult(MilBlobResult2, M_BOX_X_MAX, right);
			MblobGetResult(MilBlobResult2, M_BOX_Y_MAX, bottom);

			double dMean = 0., dSum = 0.;

			for(int j = 0; j < tot_tblob; j++)
			{
				dSum+=mean[j];
			}

			dMean = dSum / tot_tblob;

			for(int j = 0; j < tot_tblob; j++)
			{
				for(int l = 0; l < 2; l++)//i = 0 -> 밝은 불량, i = 1 -> 어두운 불량
				{
					//MbufCopy(image, image2);
					CRect _rect;
					_rect.SetRect((int)left[j], (int)top[j], (int)right[j], (int)bottom[j]);

					if(_rect.Width() < 1 || _rect.Height() < 1)
					{
						continue;
					}

					/*for(int h = _rect.top; h < _rect.bottom; h++)
					{
						for(int w = _rect.left; w < _rect.right; w++)
						{
							if((by_source7[w + (h * pitch)] < (mean[j] + teachPara.TeachInspThreshHold[ins_grab_idx][1]))
								&& by_source7[w + (h * pitch)] > (mean[j] - teachPara.TeachInspThreshHold[ins_grab_idx][2]))
							{
								by_source7[w + (h * pitch)] = (BYTE)mean[j];
							}
						}
					}*/

					if(l == 0)
					{
						MbufCopy(MilWhiteImage, image2);
					}
					else
					{
						MbufCopy(MilBlackImage, image2);
					}

					int nTemp1 = 0, nTemp2 = 0;
					CRect TempRect;
#ifdef SHIFT_INSPECTION
					int nBlobCnt[MAX_SHIFT_CNT];
					for(int z = 0; z < MAX_SHIFT_CNT; z++)//0 : orgin, 1 : left, 2 : top, 3 : right, 4 : bottom
					{
						MbufAlloc2d(*m_MilManage.GetMilSystem(), _rect.Width(), _rect.Height(), 8+M_UNSIGNED, M_IMAGE+M_PROC, &MilBinImage);
						MbufClear(MilBinImage, 0x00);

						MbufChild2d(image2, _rect.left, _rect.top, _rect.Width(), _rect.Height(), &MilChildImage);

						if(l == 0)
						{
							threshhold = (int)dMean - (int)teachPara.TeachInspThreshHold[ins_grab_idx][l + 1];
							if(threshhold < 20)
							{
								threshhold = 20;
							}
							MimBinarize(MilChildImage, MilBinImage, M_FIXED + M_IN_RANGE, 0.0, threshhold);
						}
						else
						{
							threshhold = (int)dMean + (int)teachPara.TeachInspThreshHold[ins_grab_idx][l + 1];
							if(threshhold > 240)
							{
								threshhold = 240;
							}
							MimBinarize(MilChildImage, MilBinImage, M_FIXED + M_IN_RANGE, threshhold, 255);
						}

						TempRect = SetMoveRect(&_rect, 2, z);
						MimTranslate(MilBinImage, MilTransImage, TempRect.left, TempRect.top, M_DEFAULT);

						/* Allocate a blob result buffer. */
						MblobAllocResult(*m_MilManage.GetMilSystem(), &MilBlobResult); 

						/* Allocate a feature list. */ 
						MblobAllocFeatureList(*m_MilManage.GetMilSystem(), &MilBlobFeatureList); 

						MblobSelectFeature(MilBlobFeatureList, M_AREA); 
						MblobSelectFeature(MilBlobFeatureList, M_CENTER_OF_GRAVITY); 
						MblobSelectFeature(MilBlobFeatureList, M_FERET_X); 
						MblobSelectFeature(MilBlobFeatureList, M_FERET_Y);
						MblobSelectFeature(MilBlobFeatureList, M_BOX_X_MAX);
						MblobSelectFeature(MilBlobFeatureList, M_BOX_Y_MAX);
						MblobSelectFeature(MilBlobFeatureList, M_BOX_X_MIN);
						MblobSelectFeature(MilBlobFeatureList, M_BOX_Y_MIN);
						MblobSelectFeature(MilBlobFeatureList, M_FERET_MIN_DIAMETER);
						MblobSelectFeature(MilBlobFeatureList, M_FERET_MAX_DIAMETER);
						MblobSelectFeature(MilBlobFeatureList, M_FERET_MIN_ANGLE);
						MblobSelectFeature(MilBlobFeatureList, M_FERET_MAX_ANGLE);
						MblobSelectFeature(MilBlobFeatureList, M_BREADTH);

						MblobCalculate(MilTransImage, image2, MilBlobFeatureList, MilBlobResult); 

						//검사영역 외부는 제외
						//////////////////////////////
						MblobSelect(MilBlobResult, M_DELETE, M_BOX_X_MIN, M_LESS_OR_EQUAL, rect.left, M_NULL);
						MblobSelect(MilBlobResult, M_DELETE, M_BOX_Y_MIN, M_LESS_OR_EQUAL, rect.top, M_NULL);
						MblobSelect(MilBlobResult, M_DELETE, M_BOX_X_MAX, M_GREATER_OR_EQUAL, rect.right, M_NULL);
						MblobSelect(MilBlobResult, M_DELETE, M_BOX_Y_MAX, M_GREATER_OR_EQUAL, rect.bottom, M_NULL);
						//////////////////////////////

						//검사 spec보다 작은 것 제외
						//////////////////////////////
						MblobSelect(MilBlobResult, M_DELETE, M_AREA, M_LESS_OR_EQUAL, spec, M_NULL);//area
						//MblobSelect(MilBlobResult, M_DELETE, M_FERET_MIN_DIAMETER, M_LESS_OR_EQUAL, short_spec, M_NULL);//smallist rect short
						MblobSelect(MilBlobResult, M_DELETE, M_FERET_MAX_DIAMETER, M_LESS_OR_EQUAL, spec, M_NULL);//smallist rect long
						MblobSelect(MilBlobResult, M_DELETE, M_BREADTH, M_LESS_OR_EQUAL, short_spec, M_NULL);//smallist rect long
						//////////////////////////////

						MblobGetNumber(MilBlobResult, &tot_blob);
						nBlobCnt[z] = tot_blob;		
						MblobFree(MilBlobFeatureList);
						MblobFree(MilBlobResult);
						MbufFree(MilChildImage);
						MbufFree(MilBinImage);
						//strLog.Format(_T("%d_BlobCnt_%d_%d_%d_%d"), z, i, j, l, tot_blob);
						//WriteLog(strLog, TYPE_LOG_MAIN);
					}

					//0 : orgin, 1 : left, 2 : top, 3 : right, 4 : bottom
					nTemp1 = min(nBlobCnt[0], nBlobCnt[1]);
					for(int z = 2; z < 5; z++)
					{
						nTemp2 = min(nTemp1, nBlobCnt[z]);
						nTemp1 = nTemp2;
					}

					for(int z = 0; z < 4; z++)
					{
						if(nTemp2 == nBlobCnt[z])
						{
							nTemp1 = z;
							break;
						}
					}
#endif

					_rect.SetRect((int)left[j] - 20, (int)top[j] - 20, (int)right[j] + 20, (int)bottom[j] + 20);
					
					MbufAlloc2d(*m_MilManage.GetMilSystem(), _rect.Width(), _rect.Height(), 8+M_UNSIGNED, M_IMAGE+M_PROC, &MilBinImage2);
					
					MbufClear(MilBinImage2, 0x00);
					MbufClear(MilTransImage2, 0x00);

					
					MbufChild2d(MilAlignImage, _rect.left, _rect.top, _rect.Width(), _rect.Height(), &MilChildImage2);
					MimBinarize(MilChildImage2, MilBinImage2, M_FIXED + M_IN_RANGE, 100, 255);
					MimTranslate(MilBinImage2, MilTransImage2, _rect.left, _rect.top, M_DEFAULT);

					/* Allocate a blob result buffer. */
					MblobAllocResult(*m_MilManage.GetMilSystem(), &MilBlobResult3); 
					/* Allocate a feature list. */ 
					MblobAllocFeatureList(*m_MilManage.GetMilSystem(), &MilBlobFeatureList3); 
					MblobSelectFeature(MilBlobFeatureList3, M_AREA); 
					MblobCalculate(MilTransImage2, MilAlignImage, MilBlobFeatureList3, MilBlobResult3); 
					MblobGetNumber(MilBlobResult3, &tot_blob);

					double *area;
					double temp_area, max_area;
					area = (double *)malloc(tot_blob*sizeof(double));
					MblobGetResult(MilBlobResult3, M_AREA, area);

					if(tot_blob > 1)
					{
						temp_area = area[1];
						for(int z = 0; z < tot_blob - 1; z++)
						{
							max_area = max(area[z], temp_area);
							temp_area = max_area;
						}
						MblobSelect(MilBlobResult3, M_DELETE, M_AREA, M_LESS_OR_EQUAL, (max_area - 10), M_NULL);//area
					}
					free(area);

					MblobGetNumber(MilBlobResult3, &tot_blob);

					MbufCopy(image2, image5);
					MbufClear(MilMaskImage, 0x00);
					MgraColor(M_DEFAULT, M_COLOR_WHITE);
					MblobDraw(M_DEFAULT, MilBlobResult3, MilMaskImage, M_DRAW_BLOBS , M_INCLUDED_BLOBS, M_DEFAULT);

					for(int h = _rect.top; h < _rect.bottom; h++)
					{
						for(int w = _rect.left; w < _rect.right; w++)
						{
							if(by_source[w + (h * pitch)] == 0)
							{
								by_source12[w + (h * pitch)] = (BYTE)mean[j];
							}
						}
					}

					MblobFree(MilBlobFeatureList3);
					MblobFree(MilBlobResult3);

					MbufFree(MilChildImage2);
					MbufFree(MilBinImage2);

					for(int z = 0; z < 2; z++)
					{
						MbufAlloc2d(*m_MilManage.GetMilSystem(), _rect.Width(), _rect.Height(), 8+M_UNSIGNED, M_IMAGE+M_PROC, &MilBinImage);
						MbufChild2d(image5, _rect.left, _rect.top, _rect.Width(), _rect.Height(), &MilChildImage);
						MbufClear(MilBinImage, 0x00);

						if(l == 0)
						{
							threshhold = (int)mean[j] - (int)teachPara.TeachInspThreshHold[ins_grab_idx][l + 1];
							if(z == 1)
							{
								//20161202nht
								threshhold = (int)mean[j] - (int)(teachPara.TeachInspThreshHold[ins_grab_idx][l + 1] / teachPara.TeachSecondInspThreshHold[ins_grab_idx]);
								//threshhold = (int)mean[j] - (int)(teachPara.TeachInspThreshHold[ins_grab_idx][l + 1] / 1.5);
							}
							if(threshhold < 20)
							{
								threshhold = 20;
							}
							MimBinarize(MilChildImage, MilBinImage, M_FIXED + M_IN_RANGE, 0.0, threshhold);
						}
						else
						{
							threshhold = (int)mean[j] + (int)teachPara.TeachInspThreshHold[ins_grab_idx][l + 1];
							if(z == 1)
							{
								//20161202nht
								threshhold = (int)mean[j] + (int)(teachPara.TeachInspThreshHold[ins_grab_idx][l + 1] / teachPara.TeachSecondInspThreshHold[ins_grab_idx]);
								//threshhold = (int)mean[j] + (int)(teachPara.TeachInspThreshHold[ins_grab_idx][l + 1] / 1.5);
							}
							if(threshhold > 240)
							{
								threshhold = 240;
							}
							MimBinarize(MilChildImage, MilBinImage, M_FIXED + M_IN_RANGE, threshhold, 255);
						}

						//MimTranslate(MilBinImage, MilTransImage, _rect.left, _rect.top, M_DEFAULT);
						TempRect = SetMoveRect(&_rect, 2, nTemp1);
						MimTranslate(MilBinImage, MilTransImage, TempRect.left, TempRect.top, M_DEFAULT);

						/* Allocate a blob result buffer. */
						MblobAllocResult(*m_MilManage.GetMilSystem(), &MilBlobResult); 

						/* Allocate a feature list. */ 
						MblobAllocFeatureList(*m_MilManage.GetMilSystem(), &MilBlobFeatureList); 

						MblobSelectFeature(MilBlobFeatureList, M_AREA); 
						MblobSelectFeature(MilBlobFeatureList, M_CENTER_OF_GRAVITY); 
						MblobSelectFeature(MilBlobFeatureList, M_FERET_X); 
						MblobSelectFeature(MilBlobFeatureList, M_FERET_Y);
						MblobSelectFeature(MilBlobFeatureList, M_BOX_X_MAX);
						MblobSelectFeature(MilBlobFeatureList, M_BOX_Y_MAX);
						MblobSelectFeature(MilBlobFeatureList, M_BOX_X_MIN);
						MblobSelectFeature(MilBlobFeatureList, M_BOX_Y_MIN);
						MblobSelectFeature(MilBlobFeatureList, M_FERET_MIN_DIAMETER);
						MblobSelectFeature(MilBlobFeatureList, M_FERET_MAX_DIAMETER);
						MblobSelectFeature(MilBlobFeatureList, M_FERET_MIN_ANGLE);
						MblobSelectFeature(MilBlobFeatureList, M_FERET_MAX_ANGLE);
						MblobSelectFeature(MilBlobFeatureList, M_BREADTH);

						MblobCalculate(MilTransImage, image2, MilBlobFeatureList, MilBlobResult); 

						//검사영역 외부는 제외
						//////////////////////////////
						//MblobSelect(MilBlobResult, M_DELETE, M_BOX_X_MIN, M_LESS_OR_EQUAL, rect.left, M_NULL);
						//MblobSelect(MilBlobResult, M_DELETE, M_BOX_Y_MIN, M_LESS_OR_EQUAL, rect.top, M_NULL);
						//MblobSelect(MilBlobResult, M_DELETE, M_BOX_X_MAX, M_GREATER_OR_EQUAL, rect.right, M_NULL);
						//MblobSelect(MilBlobResult, M_DELETE, M_BOX_Y_MAX, M_GREATER_OR_EQUAL, rect.bottom, M_NULL);
						//////////////////////////////

						//검사 spec보다 작은 것 제외
						//////////////////////////////
						//MblobSelect(MilBlobResult, M_DELETE, M_AREA, M_LESS_OR_EQUAL, spec, M_NULL);//area
						//MblobSelect(MilBlobResult, M_DELETE, M_FERET_MIN_DIAMETER, M_LESS_OR_EQUAL, short_spec, M_NULL);//smallist rect short
						//MblobSelect(MilBlobResult, M_DELETE, M_FERET_MAX_DIAMETER, M_LESS_OR_EQUAL, spec, M_NULL);//smallist rect long
						//MblobSelect(MilBlobResult, M_DELETE, M_BREADTH, M_LESS_OR_EQUAL, short_spec, M_NULL);//smallist rect long
						//////////////////////////////

						MblobGetNumber(MilBlobResult, &tot_blob);

						if(tot_blob < 1)
						{
							MblobFree(MilBlobFeatureList);
							MblobFree(MilBlobResult);
							MbufFree(MilChildImage);
							MbufFree(MilBinImage);
							continue;
						}

						MgraColor(M_DEFAULT, M_COLOR_WHITE);

						if(z == 0)
						{
							MbufClear(MilMaskImage, 0x00);
							MblobDraw(M_DEFAULT, MilBlobResult, MilMaskImage, M_DRAW_BLOBS , M_INCLUDED_BLOBS, M_DEFAULT);
						}
						else
						{
							MbufClear(MilMaskImage2, 0x00);
							MblobDraw(M_DEFAULT, MilBlobResult, MilMaskImage2, M_DRAW_BLOBS , M_INCLUDED_BLOBS, M_DEFAULT);
						}

						MblobFree(MilBlobFeatureList);
						MblobFree(MilBlobResult);

						MbufFree(MilChildImage);
						MbufFree(MilBinImage);

						if(z == 0)
						{
							MbufClear(MilMaskImage3, 0xFF);
							MimArith(MilAlignImage, MilMaskImage, MilMaskImage3, M_AND);
						}
						else
						{
							MbufClear(MilMaskImage4, 0xFF);
							MimArith(MilAlignImage, MilMaskImage2, MilMaskImage4, M_AND);
						}
					}
					MbufClear(MilRsltImage4, 0x00);
					MblobReconstruct(MilMaskImage4, MilMaskImage3, MilRsltImage4, M_RECONSTRUCT_FROM_SEED, M_DEFAULT);

					//찾은 불량 만 MilRsltImage2에 그리기
					for(int h = 0; h < CAMERA_HEIGHT; h++)
					{
						for(int w = 0; w < CAMERA_WIDTH; w++)
						{
							if(by_source4[w + (h * pitch)] == 255)
							{
								by_source3[w + (h * pitch)] = 0;
							}
						}
					}
				}
			}

			free(mean);
			free(left);
			free(top);
			free(right);
			free(bottom);

			MblobFree(MilBlobResult2);
			MblobFree(MilBlobFeatureList2);
		}
	}

		
	///////////////////////////////////
	MbufClear(MilRsltImage, 0xFF);
	MbufClear(MilRsltImage3, 0xFF);

	//불량 이미지 반전
	MimArith(MilRsltImage3, MilRsltImage2, MilRsltImage4, M_NAND);
	
	MbufClear(MilRsltImage3, 0x00);
	MbufClear(MilRsltImage2, 0x00);

	tmpPath_Name.Format(_T("InspectionBlob_Rslt.bmp"));
	if(!FileSearch(strTeachPath, tmpPath_Name))//mask 이미지 검색
	{
		MbufCopy(MilRsltImage4, MilRsltImage);
	}
	else
	{
		MbufClear(MilRsltImage3, 0x00);
		tmpPath_Name.Format(_T("%sInspectionBlob_Rslt.bmp"), strTeachPath);
		MbufImport((char *)(LPCTSTR)tmpPath_Name, M_BMP, M_LOAD, *m_MilManage.GetMilSystem(), &MilRsltImage3);
		MimArith(MilRsltImage3, MilRsltImage4, MilRsltImage, M_OR);
	}

	tmpPath_Name.Format(_T("%sIspectionBlobs2_Rslt.bmp"), strTeachPath);
	MbufExport((char *)(LPCTSTR)tmpPath_Name, M_BMP, MilRsltImage);

	double temp_x, temp_y;
	//처리된 blob 정보 저장
	///////////////////////////////////
	for(int i = 0; i < teachPara.TeachRegionCnt[ins_idx][ins_grab_idx]; i++)
	{
		if((nDeviceIdxY + i) == teachPara.TeachArrayCntY)
		{
			continue;
		}

		if(nDeviceIdxY + i == teachPara.TeachArrayCntY)
		{
			nNG = INSPECTION_NG;
			m_Rslt.m_bInspectionRslt[0][i] = TRUE;
			m_Rslt.m_bInspectionRslt[1][i] = TRUE;
			continue;
		}

		if(!m_Rslt.m_bAlignInsp[i])//align 검사 확인
		{
			nNG = INSPECTION_NG;
			m_Rslt.m_bInspectionRslt[0][i] = TRUE;
			m_Rslt.m_bInspectionRslt[1][i] = TRUE;
			continue;
		}

		idx  = (i * 3) + 2;
		align_idx = (i * 3) + 1;

		temp_x = teachPara.AlignPara.dCentX[i + (m_dlgTeach->GetTeachPara().TeachGrabCnt * 0)] - teachPara.AlignPara.dX[align_idx + 0 + i];
		temp_y = teachPara.AlignPara.dCentY[i + (m_dlgTeach->GetTeachPara().TeachGrabCnt * 0)] - teachPara.AlignPara.dY[align_idx + 0 + i];

		CRect rect((int)teachPara.TeachRegion[ins_idx][ins_grab_idx][idx].GetLeft() - (int)temp_x,
			(int)teachPara.TeachRegion[ins_idx][ins_grab_idx][idx].GetTop() - (int)temp_y,
			(int)teachPara.TeachRegion[ins_idx][ins_grab_idx][idx].GetRight() - (int)temp_x,
			(int)teachPara.TeachRegion[ins_idx][ins_grab_idx][idx].GetBottom() - (int)temp_y);

		MbufAlloc2d(*m_MilManage.GetMilSystem(), rect.Width(), rect.Height(), 8+M_UNSIGNED, M_IMAGE+M_PROC, &MilBinImage);
		MbufClear(MilBinImage, 0x00);
		MbufChild2d(MilRsltImage, rect.left, rect.top, rect.Width(), rect.Height(), &MilChildImage);
		MimBinarize(MilChildImage, MilBinImage, M_GREATER_OR_EQUAL, 100.0, M_NULL);
		MbufClear(MilTransImage, 0x00);
		MimTranslate(MilBinImage, MilTransImage, rect.left, rect.top, M_DEFAULT);
	
		/* Allocate a blob result buffer. */
		MblobAllocResult(*m_MilManage.GetMilSystem(), &MilBlobResult); 

		/* Allocate a feature list. */ 
		MblobAllocFeatureList(*m_MilManage.GetMilSystem(), &MilBlobFeatureList); 

		MblobSelectFeature(MilBlobFeatureList, M_AREA); 
		MblobSelectFeature(MilBlobFeatureList, M_CENTER_OF_GRAVITY); 
		MblobSelectFeature(MilBlobFeatureList, M_FERET_X); 
		MblobSelectFeature(MilBlobFeatureList, M_FERET_Y);
		MblobSelectFeature(MilBlobFeatureList, M_BOX_X_MAX);
		MblobSelectFeature(MilBlobFeatureList, M_BOX_Y_MAX);
		MblobSelectFeature(MilBlobFeatureList, M_BOX_X_MIN);
		MblobSelectFeature(MilBlobFeatureList, M_BOX_Y_MIN);
		MblobSelectFeature(MilBlobFeatureList, M_FERET_MIN_DIAMETER);
		MblobSelectFeature(MilBlobFeatureList, M_FERET_MAX_DIAMETER);
		MblobSelectFeature(MilBlobFeatureList, M_FERET_MIN_ANGLE);
		MblobSelectFeature(MilBlobFeatureList, M_FERET_MAX_ANGLE);
		MblobSelectFeature(MilBlobFeatureList, M_BREADTH);
		MblobSelectFeature(MilBlobFeatureList, M_LENGTH);

		MblobCalculate(MilTransImage, M_NULL, MilBlobFeatureList, MilBlobResult); 

		MblobGetNumber(MilBlobResult, &tot_blob);

		//검사영역 외부는 제외
		//////////////////////////////
		//MblobSelect(MilBlobResult, M_DELETE , M_BOX_X_MIN, M_LESS_OR_EQUAL, rect.left, M_NULL);
		//MblobSelect(MilBlobResult, M_DELETE , M_BOX_Y_MIN, M_LESS_OR_EQUAL, rect.top, M_NULL);
		//MblobSelect(MilBlobResult, M_DELETE , M_BOX_X_MAX, M_GREATER_OR_EQUAL, rect.right, M_NULL);
		//MblobSelect(MilBlobResult, M_DELETE , M_BOX_Y_MAX, M_GREATER_OR_EQUAL, rect.bottom, M_NULL);
		//////////////////////////////

		//검사 spec보다 작은 것 제외
		//////////////////////////////
		MblobSelect(MilBlobResult, M_DELETE, M_AREA, M_LESS_OR_EQUAL, spec, M_NULL);//area
		MblobSelect(MilBlobResult, M_DELETE, M_FERET_MIN_DIAMETER, M_LESS_OR_EQUAL, short_spec, M_NULL);//smallist rect short
		MblobSelect(MilBlobResult, M_DELETE, M_FERET_MAX_DIAMETER, M_LESS_OR_EQUAL, spec, M_NULL);//smallist rect long
		MblobSelect(MilBlobResult, M_DELETE, M_BREADTH, M_LESS_OR_EQUAL, short_spec, M_NULL);//smallist rect long
		//MblobSelect(MilBlobResult, M_DELETE, M_LENGTH, M_GREATER_OR_EQUAL, 20, M_NULL);//
		//////////////////////////////

		MblobGetNumber(MilBlobResult, &tot_blob);

		if(tot_blob < 1)
		{
			MblobFree(MilBlobFeatureList);
			MblobFree(MilBlobResult);

			MbufFree(MilChildImage);
			MbufFree(MilBinImage);
			continue;
		}

		nNG = INSPECTION_NG;
		if(nInspGrabIdx == 0)
		{
			m_Rslt.m_bInspectionRslt[0][i] = TRUE;
		}
		else
		{
			m_Rslt.m_bInspectionRslt[1][i] = TRUE;
		}

		MgraColor(M_DEFAULT, M_COLOR_YELLOW);
		MblobDraw(M_DEFAULT, MilBlobResult, *m_MilManage.GetMilOverlay(MAIN_DISP), M_DRAW_BLOBS, M_DEFAULT, M_DEFAULT);

		//불량 정보 저장
		if ((CogX = (double *)malloc(tot_blob*sizeof(double))) && 
			(CogY = (double *)malloc(tot_blob*sizeof(double))) && 
			(Short = (double *)malloc(tot_blob*sizeof(double))) &&
			(Long = (double *)malloc(tot_blob*sizeof(double))) &&
			(Ang = (double *)malloc(tot_blob*sizeof(double))) &&
			(Width = (double *)malloc(tot_blob*sizeof(double))) &&
			(Height = (double *)malloc(tot_blob*sizeof(double))) &&
			(Breadth = (double *)malloc(tot_blob*sizeof(double))))
		{
			MblobGetResult(MilBlobResult, M_BREADTH, Breadth);
			MblobGetResult(MilBlobResult, M_FERET_MIN_DIAMETER, Short);
			MblobGetResult(MilBlobResult, M_FERET_MAX_DIAMETER, Long);
			MblobGetResult(MilBlobResult, M_FERET_MAX_ANGLE, Ang);
			MblobGetResult(MilBlobResult, M_FERET_X, Width);
			MblobGetResult(MilBlobResult, M_FERET_Y, Height);
			MblobGetResult(MilBlobResult, M_CENTER_OF_GRAVITY_X, CogX);
			MblobGetResult(MilBlobResult, M_CENTER_OF_GRAVITY_Y, CogY);

			MgraColor(M_DEFAULT, M_COLOR_BLACK);
			MbufClear(MilRsltImage2, 0xFF);
			MblobDraw(M_DEFAULT, MilBlobResult, MilRsltImage2, M_DRAW_BLOBS, M_DEFAULT, M_DEFAULT);

			if(m_dlgData->GetDataPara().DataInspRsltImageSave)
			{
				m_strGrabImagePath.Format(_T("%s%s\\%s\\%s\\%s\\"), RESULT_PATH, GRAB_IMAGE_SAVE_PATH, m_Rslt.m_cStartDate, m_SelectModel.cName, m_Rslt.m_cStartTime);
				_MakeDirectories(m_strGrabImagePath);

				image_file_name.Format(_T("Rslt_X_%02d_Y_%02d_G_%02d_I_%d.bmp"), nDeviceIdxX, nDeviceIdxY + i, nGrabIdx, nInspGrabIdx);

				tmpPath_Name.Format(_T("%s%s"), m_strGrabImagePath, image_file_name);
				MbufExport((char *)(LPCTSTR)tmpPath_Name, M_BMP, MilRsltImage2);
			}

			for(int def_idx = 0; def_idx < tot_blob; def_idx++)
			{
				//if(!BlobsCompare(nDeviceIdxX, (nDeviceIdxY * m_dlgTeach->GetTeachPara().TeachOneGrabInspCnt) + i, nInspGrabIdx,  nGrabIdx,
				//	CogX[def_idx], CogY[def_idx], Width[def_idx], Height[def_idx], Long[def_idx], Short[def_idx], Breadth[def_idx], Ang[def_idx], 0, isGrabComp))
				{ 
					m_InsRsltPara[isGrabComp].dCentX[m_InsRsltPara[isGrabComp].nNgCnt] = CogX[def_idx];
					m_InsRsltPara[isGrabComp].dCentY[m_InsRsltPara[isGrabComp].nNgCnt] = CogY[def_idx];
					m_InsRsltPara[isGrabComp].dWidth[m_InsRsltPara[isGrabComp].nNgCnt] = Rounding(Width[def_idx], 0);
					m_InsRsltPara[isGrabComp].dHeight[m_InsRsltPara[isGrabComp].nNgCnt] = Rounding(Height[def_idx], 0);
					m_InsRsltPara[isGrabComp].dLong[m_InsRsltPara[isGrabComp].nNgCnt] = Rounding(Long[def_idx] + offset, 0);
					m_InsRsltPara[isGrabComp].dShort[m_InsRsltPara[isGrabComp].nNgCnt] = Rounding(Short[def_idx], 0);
					m_InsRsltPara[isGrabComp].dBreadth[m_InsRsltPara[isGrabComp].nNgCnt] = Rounding(Breadth[def_idx], 0);
					m_InsRsltPara[isGrabComp].dAng[m_InsRsltPara[isGrabComp].nNgCnt] = Ang[def_idx];//20161017 ngh

					m_InsRsltPara[isGrabComp].nDeviceIdx[m_InsRsltPara[isGrabComp].nNgCnt].SetPoint(nDeviceIdxX, (nDeviceIdxY + i));
					m_InsRsltPara[isGrabComp].nGrabIdx[m_InsRsltPara[isGrabComp].nNgCnt] = nGrabIdx;
					m_InsRsltPara[isGrabComp].nInspGrabIdx[m_InsRsltPara[isGrabComp].nNgCnt] = nInspGrabIdx;

					m_InsRsltPara[isGrabComp].bAlign[m_InsRsltPara[isGrabComp].nNgCnt] = FALSE;
					m_InsRsltPara[isGrabComp].bNg[m_InsRsltPara[isGrabComp].nNgCnt] = TRUE;

					m_InsRsltPara[isGrabComp].nNgCnt++;
				}
			}
		}

		free(CogX);
		free(CogY);
		free(Short);
		free(Long);
		free(Ang);
		free(Width);
		free(Height);
		free(Breadth);

		MblobFree(MilBlobFeatureList);
		MblobFree(MilBlobResult);

		MbufFree(MilChildImage);
		MbufFree(MilBinImage);
	}

	///////////////////////////////////

	MbufFree(MilAndImage);
	MbufFree(MilMaskImage);
	MbufFree(MilMaskImage2);
	MbufFree(MilMaskImage3);
	MbufFree(MilMaskImage4);
	MbufFree(MilAlignImage);
	MbufFree(MilLineImage);
	MbufFree(MilDilImage);
	MbufFree(MilWhiteImage);
	MbufFree(MilBlackImage);
	MbufFree(MilTransImage2);
	MbufFree(MilTransImage);
	MbufFree(MilRsltImage4);
	MbufFree(MilRsltImage3);
	MbufFree(MilRsltImage2);
	MbufFree(MilRsltImage);
	MbufFree(image5);
	MbufFree(image4);
	MbufFree(image3);
	MbufFree(image2);
	MbufFree(image1);
	MbufFree(image);
#endif

	strLog.Format(_T("InspectionBlob2 End"));
	WriteLog(strLog, TYPE_LOG_MAIN, FALSE);

	return nNG;
}

//20161116
int CLED_DisplayInspectDlg::InspectionBlobs(int nDeviceIdxX, int nDeviceIdxY, int nGrabIdx, int nInspGrabIdx, BOOL isGrabComp)
{
	int nNG = 0;
	CString strLog;

	strLog.Format(_T("InspectionBlob Start"));
	WriteLog(strLog, TYPE_LOG_MAIN, FALSE);
#ifdef CAMERA
	MIL_ID image, image1, image2, image3, image4, image5;
	MIL_ID MilMaskImage, MilMaskImage2, MilMaskImage3, MilMaskImage4;
	MIL_ID MilAlignImage;
	MIL_ID MilWhiteImage, MilBlackImage, MilDilImage, MilLineImage;
	MIL_ID MilRsltImage, MilRsltImage2, MilRsltImage3, MilRsltImage4;
	MIL_ID MilTransImage, MilBinImage, MilChildImage, MilTransImage2, MilBinImage2, MilChildImage2;
	MIL_ID MilBlobResult, MilBlobFeatureList, MilBlobResult2, MilBlobFeatureList2, MilBlobResult3, MilBlobFeatureList3;
	MIL_INT tot_blob, tot_tblob;

	CString strTeachPath, tmpPath_Name, image_file_name;
	typeTeachPara teachPara;
	BYTE *by_source, *by_source2, *by_source3, *by_source4, *by_source5, *by_source6, *by_source7, *by_source8, *by_source9, *by_source10, *by_source11, *by_source12;
	long pitch, NumResults = 0;	
	int threshhold, idx = 2, ins_idx = nGrabIdx, ins_grab_idx = nInspGrabIdx, align_idx = 0;
	double  spec, short_spec;
	double *left, *right, *top, *bottom, *mean;

	SYSTEMTIME	lpSystemTime;
	GetLocalTime(&lpSystemTime);

	teachPara = m_dlgTeach->GetTeachPara();
	spec = (double)teachPara.TeachSpec / (double)teachPara.TeachPixelSize;
	short_spec = 0.2;//(double)teachPara.TeachSpec / 100.0;
	if(short_spec < 1.0)
	{
		short_spec = 1.0;
	}
	m_RsltIdx = 0;

	strTeachPath.Format(_T("%s\\TeachImg\\"), m_SelectModel.cPath);

	//image buffer init
	MbufAlloc2d(*m_MilManage.GetMilSystem(), CAMERA_WIDTH, CAMERA_HEIGHT, 8+M_UNSIGNED, M_IMAGE+M_PROC, &image);
	MbufAlloc2d(*m_MilManage.GetMilSystem(), CAMERA_WIDTH, CAMERA_HEIGHT, 8+M_UNSIGNED, M_IMAGE+M_PROC, &image1);
	MbufAlloc2d(*m_MilManage.GetMilSystem(), CAMERA_WIDTH, CAMERA_HEIGHT, 8+M_UNSIGNED, M_IMAGE+M_PROC, &image2);
	MbufAlloc2d(*m_MilManage.GetMilSystem(), CAMERA_WIDTH, CAMERA_HEIGHT, 8+M_UNSIGNED, M_IMAGE+M_PROC, &image3);
	MbufAlloc2d(*m_MilManage.GetMilSystem(), CAMERA_WIDTH, CAMERA_HEIGHT, 8+M_UNSIGNED, M_IMAGE+M_PROC, &image4);
	MbufAlloc2d(*m_MilManage.GetMilSystem(), CAMERA_WIDTH, CAMERA_HEIGHT, 8+M_UNSIGNED, M_IMAGE+M_PROC, &image5);
	MbufAlloc2d(*m_MilManage.GetMilSystem(), CAMERA_WIDTH, CAMERA_HEIGHT, 8+M_UNSIGNED, M_IMAGE+M_PROC, &MilTransImage);
	MbufAlloc2d(*m_MilManage.GetMilSystem(), CAMERA_WIDTH, CAMERA_HEIGHT, 8+M_UNSIGNED, M_IMAGE+M_PROC, &MilTransImage2);
	MbufAlloc2d(*m_MilManage.GetMilSystem(), CAMERA_WIDTH, CAMERA_HEIGHT, 8+M_UNSIGNED, M_IMAGE+M_PROC, &MilMaskImage);
	MbufAlloc2d(*m_MilManage.GetMilSystem(), CAMERA_WIDTH, CAMERA_HEIGHT, 8+M_UNSIGNED, M_IMAGE+M_PROC, &MilMaskImage2);
	MbufAlloc2d(*m_MilManage.GetMilSystem(), CAMERA_WIDTH, CAMERA_HEIGHT, 8+M_UNSIGNED, M_IMAGE+M_PROC, &MilMaskImage3);
	MbufAlloc2d(*m_MilManage.GetMilSystem(), CAMERA_WIDTH, CAMERA_HEIGHT, 8+M_UNSIGNED, M_IMAGE+M_PROC, &MilMaskImage4);
	MbufAlloc2d(*m_MilManage.GetMilSystem(), CAMERA_WIDTH, CAMERA_HEIGHT, 8+M_UNSIGNED, M_IMAGE+M_PROC, &MilAlignImage);
	MbufAlloc2d(*m_MilManage.GetMilSystem(), CAMERA_WIDTH, CAMERA_HEIGHT, 8+M_UNSIGNED, M_IMAGE+M_PROC, &MilRsltImage);
	MbufAlloc2d(*m_MilManage.GetMilSystem(), CAMERA_WIDTH, CAMERA_HEIGHT, 8+M_UNSIGNED, M_IMAGE+M_PROC, &MilRsltImage2);
	MbufAlloc2d(*m_MilManage.GetMilSystem(), CAMERA_WIDTH, CAMERA_HEIGHT, 8+M_UNSIGNED, M_IMAGE+M_PROC, &MilRsltImage3);
	MbufAlloc2d(*m_MilManage.GetMilSystem(), CAMERA_WIDTH, CAMERA_HEIGHT, 8+M_UNSIGNED, M_IMAGE+M_PROC, &MilRsltImage4);
	MbufAlloc2d(*m_MilManage.GetMilSystem(), CAMERA_WIDTH, CAMERA_HEIGHT, 8+M_UNSIGNED, M_IMAGE+M_PROC, &MilWhiteImage);
	MbufAlloc2d(*m_MilManage.GetMilSystem(), CAMERA_WIDTH, CAMERA_HEIGHT, 8+M_UNSIGNED, M_IMAGE+M_PROC, &MilBlackImage);
	MbufAlloc2d(*m_MilManage.GetMilSystem(), CAMERA_WIDTH, CAMERA_HEIGHT, 8+M_UNSIGNED, M_IMAGE+M_PROC, &MilDilImage);
	MbufAlloc2d(*m_MilManage.GetMilSystem(), CAMERA_WIDTH, CAMERA_HEIGHT, 8+M_UNSIGNED, M_IMAGE+M_PROC, &MilLineImage);

	MbufClear(MilRsltImage, 0xFF);
	MbufClear(MilRsltImage2, 0xFF);
	MbufClear(MilAlignImage, 0xFF);


	//image buffer inquire
	MbufInquire(MilMaskImage, M_HOST_ADDRESS, &by_source);
	MbufInquire(MilAlignImage, M_HOST_ADDRESS, &by_source2);
	MbufInquire(MilRsltImage2, M_HOST_ADDRESS, &by_source3);
	MbufInquire(MilRsltImage4, M_HOST_ADDRESS, &by_source4);
	MbufInquire(image3, M_HOST_ADDRESS, &by_source5);
	MbufInquire(image2, M_HOST_ADDRESS, &by_source7);
	MbufInquire(MilWhiteImage, M_HOST_ADDRESS, &by_source8);
	MbufInquire(MilBlackImage, M_HOST_ADDRESS, &by_source9);
	MbufInquire(MilDilImage, M_HOST_ADDRESS, &by_source10);
	MbufInquire(MilLineImage, M_HOST_ADDRESS, &by_source11);
	MbufInquire(image5, M_HOST_ADDRESS, &by_source12);
	MbufInquire(MilMaskImage, M_PITCH, &pitch);

	MbufInquire(image, M_HOST_ADDRESS, &by_source6);

	//불량 블롭 분리
	///////////////////////////////////
	for(int i = 0; i < teachPara.TeachRegionCnt[ins_idx][ins_grab_idx]; i++)
	{
		m_Rslt.m_bInspectionRslt[0][i] = FALSE;
		m_Rslt.m_bInspectionRslt[1][i] = FALSE;
		if((nDeviceIdxY + i) == teachPara.TeachArrayCntY)
		{
			continue;
		}

		if(nDeviceIdxY + i == teachPara.TeachArrayCntY)
		{
			nNG = INSPECTION_NG;
			m_Rslt.m_bInspectionRslt[0][i] = TRUE;
			m_Rslt.m_bInspectionRslt[1][i] = TRUE;
			continue;
		}

		if(!m_Rslt.m_bAlignInsp[i])//align 검사 확인
		{
			nNG = INSPECTION_NG;
			m_Rslt.m_bInspectionRslt[0][i] = TRUE;
			m_Rslt.m_bInspectionRslt[1][i] = TRUE;
			continue;
		}

		MbufCopy(*m_MilManage.GetMilInspBuf(isGrabComp, nInspGrabIdx), image);
		MbufCopy(*m_MilManage.GetMilInspBuf(isGrabComp, nInspGrabIdx), image3);

		for(int h = 0; h < CAMERA_HEIGHT; h++)
		{
			for(int w = 0; w < CAMERA_WIDTH; w++)
			{
				by_source6[w + (h * pitch)] = 255 - by_source6[w + (h * pitch)];
			}
		}

		MimArith(image, image3, image1, M_SUB_CONST);
		MimArith(image1, image3, image2, M_ADD_CONST);
		MbufClear(image, 0x00);
		MbufCopy(image2, image);
		MbufCopy(image, MilWhiteImage);
		MbufCopy(image, MilBlackImage);

		idx  = (i * 3) + 2;
		align_idx = (i * 3) + 1;
		CRect ng_rect;

		for(int k = 0; k < 2; k++)//원형 분리 시에 for(int k = 0; k < 2; k++) 로 변경
		{
			tmpPath_Name.Format(_T("BlobMask_%d_%d_%d.bmp"), ins_idx, i, k);
			if(!FileSearch(strTeachPath, tmpPath_Name))//mask 이미지 검색
			{
				MgraColor(M_DEFAULT, M_COLOR_RED);
				strLog.Format(_T("Not Model Teaching"));
				CdRect rect;
				rect.SetdRectLTRB((int)teachPara.TeachRegion[ins_idx][ins_grab_idx][idx].GetLeft(),
					(int)teachPara.TeachRegion[ins_idx][ins_grab_idx][idx].GetTop(),
					(int)teachPara.TeachRegion[ins_idx][ins_grab_idx][idx].GetRight(),
					(int)teachPara.TeachRegion[ins_idx][ins_grab_idx][idx].GetBottom());
				MgraText(M_DEFAULT, *m_MilManage.GetMilOverlay(MAIN_DISP), rect.GetLeft() + 20, rect.GetTop() + 20, MIL_TEXT((char *)(LPCTSTR)strLog));
				strLog.Format(_T("Test,BlobMaskImageSearchFail_%d"), i);
				WriteLog(strLog, TYPE_LOG_MAIN);
				continue;
			}

			MbufClear(MilMaskImage, 0xFF);
			MbufClear(MilAlignImage, 0xFF);

			tmpPath_Name.Format(_T("%sBlobMask_%d_%d_%d.bmp"), strTeachPath, ins_idx, i, k);
			MbufImport((char *)(LPCTSTR)tmpPath_Name, M_BMP, M_LOAD, *m_MilManage.GetMilSystem(), &MilMaskImage);	

			double temp_x, temp_y;

			temp_x = teachPara.AlignPara.dCentX[i + (m_dlgTeach->GetTeachPara().TeachGrabCnt * nGrabIdx)] - teachPara.AlignPara.dX[align_idx + nGrabIdx + i];
			temp_y = teachPara.AlignPara.dCentY[i + (m_dlgTeach->GetTeachPara().TeachGrabCnt * nGrabIdx)] - teachPara.AlignPara.dY[align_idx + nGrabIdx + i];

			//mask image align 값으로 보정
			MimRotate(MilMaskImage, MilAlignImage, teachPara.AlignPara.dAng[align_idx + nGrabIdx + i], 
				teachPara.AlignPara.dCentX[i + (m_dlgTeach->GetTeachPara().TeachGrabCnt * nGrabIdx)],
				teachPara.AlignPara.dCentY[i + (m_dlgTeach->GetTeachPara().TeachGrabCnt * nGrabIdx)],
				teachPara.AlignPara.dX[align_idx + nGrabIdx + i], teachPara.AlignPara.dY[align_idx + nGrabIdx + i], M_DEFAULT);
			strLog.Format(_T("AlignImage Rotate"));
			WriteLog(strLog, TYPE_LOG_MAIN, FALSE);

			tmpPath_Name.Format(_T("%sAlign_rslt_%d_%d_%d.bmp"), strTeachPath, ins_idx, i, k);
			MbufExport((char *)(LPCTSTR)tmpPath_Name, M_BMP, MilAlignImage);

			if(k == 1)//원형일 경우에 검사영역 축소
			{
				if(teachPara.TeachCircleErodeDistance[nInspGrabIdx] > 0.)
				{
					MimDilate(MilAlignImage, MilAlignImage, teachPara.TeachCircleErodeDistance[nInspGrabIdx], M_BINARY);
				}
			}
			else
			{
				MimDilate(MilAlignImage, MilAlignImage, 1, M_BINARY);
			}

			CRect rect((int)teachPara.TeachRegion[ins_idx][ins_grab_idx][idx].GetLeft(),
				(int)teachPara.TeachRegion[ins_idx][ins_grab_idx][idx].GetTop(),
				(int)teachPara.TeachRegion[ins_idx][ins_grab_idx][idx].GetRight(),
				(int)teachPara.TeachRegion[ins_idx][ins_grab_idx][idx].GetBottom());
			MbufClear(MilTransImage, 0x00);

			/* Allocate a blob result buffer. */
			MblobAllocResult(*m_MilManage.GetMilSystem(), &MilBlobResult2); 

			/* Allocate a feature list. */ 
			MblobAllocFeatureList(*m_MilManage.GetMilSystem(), &MilBlobFeatureList2); 

			MblobSelectFeature(MilBlobFeatureList2, M_MEAN_PIXEL);
			MblobSelectFeature(MilBlobFeatureList2, M_BOX_X_MAX);
			MblobSelectFeature(MilBlobFeatureList2, M_BOX_Y_MAX);
			MblobSelectFeature(MilBlobFeatureList2, M_BOX_X_MIN);
			MblobSelectFeature(MilBlobFeatureList2, M_BOX_Y_MIN);

			MimDilate(MilAlignImage, MilDilImage, 2, M_BINARY);
			MbufCopy(MilAlignImage, MilLineImage);
			for(int h = 0; h < CAMERA_HEIGHT; h++)
			{
				for(int w = 0; w < CAMERA_WIDTH; w++)
				{
					if(by_source10[w + (h * pitch)] == 0)
					{
						by_source11[w + (h * pitch)] = 255;
					}
				}
			}

			for(int h = 0; h < CAMERA_HEIGHT; h++)
			{
				for(int w = 0; w < CAMERA_WIDTH; w++)
				{
					if(by_source11[w + (h * pitch)] == 0)
					{
						by_source8[w + (h * pitch)]+=40;
						by_source9[w + (h * pitch)]-=40;
					}
				}
			}

			//align image 반전
			for(int h = 0; h < CAMERA_HEIGHT; h++)
			{
				for(int w = 0; w < CAMERA_WIDTH; w++)
				{
					if(by_source2[w + (h * pitch)] == 0)
					{
						by_source2[w + (h * pitch)] = 255;
					}
					else
					{
						by_source2[w + (h * pitch)] = 0;
					}
				}
			}

			MimConvolve(image, image, M_SHEN_FILTER(M_SHARPEN, m_edit_nRepeatMaxCnt));
			MimConvolve(image, image, M_SHEN_FILTER(M_SHARPEN, m_edit_nRepeatMaxCnt)); 

			MblobCalculate(MilAlignImage, image, MilBlobFeatureList2, MilBlobResult2); 

			//MblobSelect(MilBlobResult2, M_EXCLUDE, M_BOX_X_MIN, M_LESS_OR_EQUAL, rect.left, M_NULL);
			//MblobSelect(MilBlobResult2, M_EXCLUDE, M_BOX_Y_MIN, M_LESS_OR_EQUAL, rect.top, M_NULL);
			//MblobSelect(MilBlobResult2, M_EXCLUDE, M_BOX_X_MAX, M_GREATER_OR_EQUAL, rect.right, M_NULL);
			//MblobSelect(MilBlobResult2, M_EXCLUDE, M_BOX_Y_MAX, M_GREATER_OR_EQUAL, rect.bottom, M_NULL);

			MblobGetNumber(MilBlobResult2, &tot_tblob);

			if(tot_tblob < 1)
			{
				MblobFree(MilBlobResult2);
				MblobFree(MilBlobFeatureList2);
				continue;
			}

			MgraColor(M_DEFAULT, M_COLOR_BLUE);
			MblobDraw(M_DEFAULT, MilBlobResult2, *m_MilManage.GetMilOverlay(MAIN_DISP), M_DRAW_BLOBS_CONTOUR, M_DEFAULT, M_DEFAULT);

			//blob 별로 평균 밝기값, left, top, right, bottom 추출
			mean = (double *)malloc(tot_tblob*sizeof(double));
			left = (double *)malloc(tot_tblob*sizeof(double));
			top = (double *)malloc(tot_tblob*sizeof(double));
			right = (double *)malloc(tot_tblob*sizeof(double));
			bottom = (double *)malloc(tot_tblob*sizeof(double));

			MblobGetResult(MilBlobResult2, M_MEAN_PIXEL, mean);
			MblobGetResult(MilBlobResult2, M_BOX_X_MIN, left);
			MblobGetResult(MilBlobResult2, M_BOX_Y_MIN, top);
			MblobGetResult(MilBlobResult2, M_BOX_X_MAX, right);
			MblobGetResult(MilBlobResult2, M_BOX_Y_MAX, bottom);

			double dMean = 0., dSum = 0.;

			for(int j = 0; j < tot_tblob; j++)
			{
				dSum+=mean[j];
			}

			dMean = dSum / tot_tblob;

			strLog.Format(_T("TotBlob = %d, Blobs Insp_%d_%d"), tot_tblob, i, k);
			WriteLog(strLog, TYPE_LOG_MAIN, FALSE);
			for(int j = 0; j < tot_tblob; j++)
			{
				for(int l = 0; l < 2; l++)//i = 0 -> 밝은 불량, i = 1 -> 어두운 불량
				{
					CRect _rect;
					_rect.SetRect((int)left[j], (int)top[j], (int)right[j], (int)bottom[j]);

					if(_rect.Width() < 1 || _rect.Height() < 1)
					{
						continue;
					}

					if(l == 0)
					{
						MbufCopy(MilWhiteImage, image);
					}
					else
					{
						MbufCopy(MilBlackImage, image);
					}

					int nTemp1 = 0, nTemp2 = 0;
					CRect TempRect;
	
					_rect.SetRect((int)left[j] - 20, (int)top[j] - 20, (int)right[j] + 20, (int)bottom[j] + 20);
					MbufAlloc2d(*m_MilManage.GetMilSystem(), _rect.Width(), _rect.Height(), 8+M_UNSIGNED, M_IMAGE+M_PROC, &MilBinImage2);
					MbufClear(MilBinImage2, 0x00);
					MbufClear(MilTransImage2, 0x00);

					
					MbufChild2d(MilAlignImage, _rect.left, _rect.top, _rect.Width(), _rect.Height(), &MilChildImage2);
					MimBinarize(MilChildImage2, MilBinImage2, M_FIXED + M_IN_RANGE, 100, 255);
					MimTranslate(MilBinImage2, MilTransImage2, _rect.left, _rect.top, M_DEFAULT);

					/* Allocate a blob result buffer. */
					MblobAllocResult(*m_MilManage.GetMilSystem(), &MilBlobResult3); 
					/* Allocate a feature list. */ 
					MblobAllocFeatureList(*m_MilManage.GetMilSystem(), &MilBlobFeatureList3); 
					MblobSelectFeature(MilBlobFeatureList3, M_AREA); 
					MblobCalculate(MilTransImage2, MilAlignImage, MilBlobFeatureList3, MilBlobResult3); 
					MblobGetNumber(MilBlobResult3, &tot_blob);

					double *area;
					double temp_area, max_area;
					area = (double *)malloc(tot_blob*sizeof(double));
					MblobGetResult(MilBlobResult3, M_AREA, area);

					if(tot_blob > 1)
					{
						temp_area = area[1];
						for(int z = 0; z < tot_blob - 1; z++)
						{
							max_area = max(area[z], temp_area);
							temp_area = max_area;
						}
						MblobSelect(MilBlobResult3, M_DELETE, M_AREA, M_LESS_OR_EQUAL, (max_area - 10), M_NULL);//area
					}
					free(area);

					MblobGetNumber(MilBlobResult3, &tot_blob);

					MbufCopy(image, image5);
					MbufClear(MilMaskImage, 0x00);
					MgraColor(M_DEFAULT, M_COLOR_WHITE);
					MblobDraw(M_DEFAULT, MilBlobResult3, MilMaskImage, M_DRAW_BLOBS , M_INCLUDED_BLOBS, M_DEFAULT);

					for(int h = _rect.top; h < _rect.bottom; h++)
					{
						for(int w = _rect.left; w < _rect.right; w++)
						{
							if(by_source[w + (h * pitch)] == 0)
							{
								by_source12[w + (h * pitch)] = (BYTE)mean[j];
							}
						}
					}

					MblobFree(MilBlobFeatureList3);
					MblobFree(MilBlobResult3);

					MbufFree(MilChildImage2);
					MbufFree(MilBinImage2);
					for(int z = 0; z < 2; z++)
					{
						MbufAlloc2d(*m_MilManage.GetMilSystem(), _rect.Width(), _rect.Height(), 8+M_UNSIGNED, M_IMAGE+M_PROC, &MilBinImage);
						MbufClear(MilBinImage, 0x00);
						MbufChild2d(image5, _rect.left, _rect.top, _rect.Width(), _rect.Height(), &MilChildImage);
						if(l == 0)
						{
							threshhold = (int)mean[j] - (int)teachPara.TeachInspThreshHold[ins_grab_idx][l + 1];
							if(z == 1)
							{
								//20161202nht
								threshhold = (int)mean[j] - (int)(teachPara.TeachInspThreshHold[ins_grab_idx][l + 1] / teachPara.TeachSecondInspThreshHold[ins_grab_idx]);
								//threshhold = (int)mean[j] - (int)(teachPara.TeachInspThreshHold[ins_grab_idx][l + 1] / 1.5);
							}
							if(threshhold < 20)
							{
								threshhold = 20;
							}
							MimBinarize(MilChildImage, MilBinImage, M_FIXED + M_IN_RANGE, 0.0, threshhold);
						}
						else
						{
							threshhold = (int)mean[j] + (int)teachPara.TeachInspThreshHold[ins_grab_idx][l + 1];
							if(z == 1)
							{
								//20161202nht
								threshhold = (int)mean[j] + (int)(teachPara.TeachInspThreshHold[ins_grab_idx][l + 1] / teachPara.TeachSecondInspThreshHold[ins_grab_idx]);
								//threshhold = (int)mean[j] + (int)(teachPara.TeachInspThreshHold[ins_grab_idx][l + 1] / 1.5);
							}
							if(threshhold > 240)
							{
								threshhold = 240;
							}
							MimBinarize(MilChildImage, MilBinImage, M_FIXED + M_IN_RANGE, threshhold, 255);
						}

						TempRect = SetMoveRect(&_rect, 2, nTemp1);
						MimTranslate(MilBinImage, MilTransImage, TempRect.left, TempRect.top, M_DEFAULT);

						/* Allocate a blob result buffer. */
						MblobAllocResult(*m_MilManage.GetMilSystem(), &MilBlobResult); 

						/* Allocate a feature list. */ 
						MblobAllocFeatureList(*m_MilManage.GetMilSystem(), &MilBlobFeatureList); 

						MblobSelectFeature(MilBlobFeatureList, M_AREA); 
						MblobSelectFeature(MilBlobFeatureList, M_CENTER_OF_GRAVITY); 
						MblobSelectFeature(MilBlobFeatureList, M_FERET_X); 
						MblobSelectFeature(MilBlobFeatureList, M_FERET_Y);
						MblobSelectFeature(MilBlobFeatureList, M_BOX_X_MAX);
						MblobSelectFeature(MilBlobFeatureList, M_BOX_Y_MAX);
						MblobSelectFeature(MilBlobFeatureList, M_BOX_X_MIN);
						MblobSelectFeature(MilBlobFeatureList, M_BOX_Y_MIN);
						MblobSelectFeature(MilBlobFeatureList, M_FERET_MIN_DIAMETER);
						MblobSelectFeature(MilBlobFeatureList, M_FERET_MAX_DIAMETER);
						MblobSelectFeature(MilBlobFeatureList, M_FERET_MIN_ANGLE);
						MblobSelectFeature(MilBlobFeatureList, M_FERET_MAX_ANGLE);
						MblobSelectFeature(MilBlobFeatureList, M_BREADTH);

						MblobCalculate(MilTransImage, image5, MilBlobFeatureList, MilBlobResult); 

						//검사영역 외부는 제외
						//////////////////////////////
						//MblobSelect(MilBlobResult, M_DELETE, M_BOX_X_MIN, M_LESS_OR_EQUAL, rect.left, M_NULL);
						//MblobSelect(MilBlobResult, M_DELETE, M_BOX_Y_MIN, M_LESS_OR_EQUAL, rect.top, M_NULL);
						//MblobSelect(MilBlobResult, M_DELETE, M_BOX_X_MAX, M_GREATER_OR_EQUAL, rect.right, M_NULL);
						//MblobSelect(MilBlobResult, M_DELETE, M_BOX_Y_MAX, M_GREATER_OR_EQUAL, rect.bottom, M_NULL);
						//////////////////////////////

						//검사 spec보다 작은 것 제외
						//////////////////////////////
						MblobSelect(MilBlobResult, M_DELETE, M_AREA, M_LESS_OR_EQUAL, spec, M_NULL);//area
						MblobSelect(MilBlobResult, M_DELETE, M_FERET_MAX_DIAMETER, M_LESS_OR_EQUAL, spec, M_NULL);//smallist rect long
						MblobSelect(MilBlobResult, M_DELETE, M_BREADTH, M_LESS_OR_EQUAL, short_spec, M_NULL);//smallist rect long
						//////////////////////////////

						MblobGetNumber(MilBlobResult, &tot_blob);

						if(tot_blob < 1)
						{
							MblobFree(MilBlobFeatureList);
							MblobFree(MilBlobResult);
							MbufFree(MilChildImage);
							MbufFree(MilBinImage);
							continue;
						}

						//MbufClear(MilMaskImage, 0x00);
						MgraColor(M_DEFAULT, M_COLOR_WHITE);
						
						if(z == 0)
						{
							MbufClear(MilMaskImage, 0x00);
							MblobDraw(M_DEFAULT, MilBlobResult, MilMaskImage, M_DRAW_BLOBS , M_INCLUDED_BLOBS, M_DEFAULT);
						}
						else
						{
							MbufClear(MilMaskImage2, 0x00);
							MblobDraw(M_DEFAULT, MilBlobResult, MilMaskImage2, M_DRAW_BLOBS , M_INCLUDED_BLOBS, M_DEFAULT);
						}
						MblobFree(MilBlobFeatureList);
						MblobFree(MilBlobResult);

						MbufFree(MilChildImage);
						MbufFree(MilBinImage);
					
						if(z == 0)
						{
							MbufClear(MilMaskImage3, 0xFF);
							MimArith(MilAlignImage, MilMaskImage, MilMaskImage3, M_AND);
						}
						else
						{
							MbufClear(MilMaskImage4, 0xFF);
							MimArith(MilAlignImage, MilMaskImage2, MilMaskImage4, M_AND);
						}
					}
					MbufClear(MilRsltImage4, 0x00);
					MblobReconstruct(MilMaskImage4, MilMaskImage3, MilRsltImage4, M_RECONSTRUCT_FROM_SEED, M_DEFAULT);


					//찾은 불량 만 MilRsltImage2에 그리기
					for(int h = 0; h < CAMERA_HEIGHT; h++)
					{
						for(int w = 0; w < CAMERA_WIDTH; w++)
						{
							if(by_source4[w + (h * pitch)] == 255)
							{
								by_source3[w + (h * pitch)] = 0;
							}
						}
					}
				}
			}

			free(mean);
			free(left);
			free(top);
			free(right);
			free(bottom);

			MblobFree(MilBlobResult2);
			MblobFree(MilBlobFeatureList2);
		}
	}	
	///////////////////////////////////
	MbufClear(MilRsltImage, 0xFF);
	MbufClear(MilRsltImage3, 0xFF);

	//불량 이미지 반전
	MimArith(MilRsltImage3, MilRsltImage2, MilRsltImage, M_NAND);

	//MblobReconstruct(MilRsltImage4, MilRsltImage, MilRsltImage3, M_RECONSTRUCT_FROM_SEED, M_DEFAULT);

	//불량 이미지 저장
	tmpPath_Name.Format(_T("%sInspectionBlob_Rslt.bmp"), strTeachPath);
	MbufExport((char *)(LPCTSTR)tmpPath_Name, M_BMP, MilRsltImage);
	///////////////////////////////////

	MbufFree(MilAlignImage);
	MbufFree(MilMaskImage);
	MbufFree(MilMaskImage2);
	MbufFree(MilMaskImage3);
	MbufFree(MilMaskImage4);
	MbufFree(MilRsltImage4);
	MbufFree(MilLineImage);
	MbufFree(MilDilImage);
	MbufFree(MilWhiteImage);
	MbufFree(MilBlackImage);
	MbufFree(MilTransImage2);
	MbufFree(MilTransImage);
	MbufFree(MilRsltImage3);
	MbufFree(MilRsltImage2);
	MbufFree(MilRsltImage);
	MbufFree(image5);
	MbufFree(image4);
	MbufFree(image3);
	MbufFree(image2);
	MbufFree(image1);
	MbufFree(image);
#endif

	strLog.Format(_T("InspectionBlob End"));
	WriteLog(strLog, TYPE_LOG_MAIN, FALSE);

	return nNG;
}

BOOL CLED_DisplayInspectDlg::InspectionImageSave(int nDeviceIdxX, int nDeviceIdxY, int nGrabIdx, int nInspGrabIdx, int nOk, BOOL isGrabComp)
{
	BOOL rslt = TRUE;
	CString image_file_name, tmpPath_Name;
	SYSTEMTIME	lpSystemTime;
	GetLocalTime(&lpSystemTime);
	typedefDataPara dataPara;
	int y_idx = 0;

	dataPara = m_dlgData->GetDataPara();

	m_strGrabImagePath.Format(_T("%s%s\\%s\\%s\\%s\\"), RESULT_PATH, GRAB_IMAGE_SAVE_PATH, m_Rslt.m_cStartDate, m_SelectModel.cName, m_Rslt.m_cStartTime);
	_MakeDirectories(m_strGrabImagePath);

	m_strOKImagePath.Format(_T("%s%s%s\\%s\\%s\\"), RESULT_PATH, OK_IMAGE_SAVE_PATH, m_Rslt.m_cStartDate, m_SelectModel.cName, m_Rslt.m_cStartTime);
	_MakeDirectories(m_strOKImagePath);

	m_strNGImagePath.Format(_T("%s%s%s\\%s\\%s\\"), RESULT_PATH, NG_IMAGE_SAVE_PATH, m_Rslt.m_cStartDate, m_SelectModel.cName, m_Rslt.m_cStartTime);
	_MakeDirectories(m_strNGImagePath);

	image_file_name.Format(_T("X_%02d_Y_%02d_G_%02d_I_%d.bmp"), nDeviceIdxX, nDeviceIdxY, nGrabIdx, nInspGrabIdx);

	if(nOk == 0)
	{
		if(dataPara.DataGrabImageSave)
		{
			tmpPath_Name.Format(_T("%s%s"), m_strGrabImagePath, image_file_name);
#ifdef CAMERA
			MbufExport((char *)(LPCTSTR)tmpPath_Name, M_BMP, *m_MilManage.GetMilInspBuf(m_Rslt.m_bGrabComp, nInspGrabIdx));
#endif
		}
	}

	if(nOk == INSPECTION_NG || nOk == ALIGN_NG)
	{
		m_bInspectionOKNG = FALSE;
		if(dataPara.DataNGImageSave)
		{
			tmpPath_Name.Format(_T("%s%s"), m_strNGImagePath, image_file_name);
#ifdef CAMERA
			MbufExport((char *)(LPCTSTR)tmpPath_Name, M_BMP, *m_MilManage.GetMilInspBuf(m_Rslt.m_bGrabComp, nInspGrabIdx));
#endif
		}
	}
	else if(nOk == INSPECTION_OK)
	{
		if(dataPara.DataOKImageSave)
		{
			tmpPath_Name.Format(_T("%s%s"), m_strOKImagePath, image_file_name);
#ifdef CAMERA
			MbufExport((char *)(LPCTSTR)tmpPath_Name, M_BMP, *m_MilManage.GetMilInspBuf(m_Rslt.m_bGrabComp, nInspGrabIdx));
#endif
		}
	}
	return rslt;
}

void CLED_DisplayInspectDlg::NGDiaplayClear(int nDisplayNum)
{
	if(nDisplayNum == MAIN_DISP)
	{
#ifdef CAMERA
		for(int i = 0; i < MAX_NG_DISP; i++)
		{
			MdispControl(*m_MilManage.GetMilDisplay(NG_DISP_1 + i), M_OVERLAY_CLEAR, M_DEFAULT);
		}
		for(int i = 0; i < MAX_NG_DISP; i++)
		{
			MbufClear(*m_MilManage.GetMilDispBuf(NG_DISP_1 + i), 0x00);
		}
		//MbufCopy(*m_MilManage.GetMilGrabBuf(), *m_MilManage.GetMilDispBuf(MAIN_DISP));
#endif
	}
	else if(nDisplayNum == NG_VIEW_DISP)
	{
#ifdef CAMERA
		for(int i = 0; i < MAX_NG_DISP; i++)
		{
			MdispControl(*m_MilManage.GetMilDisplay(NG_DISP_1 + i + NG_VIEW_DISP), M_OVERLAY_CLEAR, M_DEFAULT);
		}
		for(int i = 0; i < MAX_NG_DISP; i++)
		{
			MbufClear(*m_MilManage.GetMilDispBuf(NG_DISP_1 + i + NG_VIEW_DISP), 0x00);
		}
		//MbufCopy(*m_MilManage.GetMilGrabBuf(), *m_MilManage.GetMilDispBuf(NG_VIEW_DISP));
#endif
	}
}

void CLED_DisplayInspectDlg::SetInspRslt(typeInsRsltPara rsltPara, int nDeviceIdxX, int nDeviceIdxY, int nGrabIdx)
{
	m_RsltPara;
}

//20161201 ngh
void CLED_DisplayInspectDlg::NGToMainDisplay(int nDeviceIdxX, int nDeviceIdxY, int nGrabIdx, int nInspGrabIdx, int nDisplayNum, typeInsRsltPara rsltPara)
{
	CString temp;
	CdRect rect;
	double ng_left, ng_top;
	int nAddNg = 0;
	CPoint poStart;
#ifdef CAMERA
	MgraControl(M_DEFAULT, M_FONT_X_SCALE , 4);
	MgraControl(M_DEFAULT, M_FONT_Y_SCALE , 4);
#endif
	if(nDisplayNum == NG_VIEW_DISP)
	{
		for(int i = 0; i < rsltPara.nNgCnt; i++)
		{
			for(int j = 0; j < m_dlgTeach->GetTeachPara().TeachOneGrabInspCnt; j++)
			{
				if((rsltPara.nDeviceIdx[i].x == nDeviceIdxX) && (rsltPara.nDeviceIdx[i].y == nDeviceIdxY + j))
				{
					if((rsltPara.nGrabIdx[i] == nGrabIdx))// && (rsltPara.nInspGrabIdx[i] == nInspGrabIdx))
					{
						if(rsltPara.bAddNg[i])
						{
							ng_left = nAddNg * 30 + 20;
							ng_top = nAddNg + 20;
							nAddNg++;
							temp.Format("NG %d = Add Defect", i + 1);
#ifdef CAMERA
							MgraColor(M_DEFAULT, M_COLOR_RED);
							MgraText(M_DEFAULT, *m_MilManage.GetMilOverlay(NG_VIEW_DISP), ng_left, ng_top, MIL_TEXT(temp));
#endif
						}
						else
						{
							//m_RsltIdx = i;
							//불량 위치 표시
							rsltPara.dCentX[i];
							rsltPara.dCentY[i];
							rsltPara.dWidth[i];
							rsltPara.dHeight[i];
							ng_left = rsltPara.dCentX[i] + (rsltPara.dWidth[i] / 2.0) + 10;
							ng_top = rsltPara.dCentY[i] - (rsltPara.dHeight[i] / 2.0) - 10;
							temp.Format("NG %d", i + 1);
#ifdef CAMERA
							MgraColor(M_DEFAULT, M_COLOR_RED);
							poStart.SetPoint((int)ng_left, (int)ng_top);
							DrowArrow(NG_VIEW_DISP, poStart);

							MgraText(M_DEFAULT, *m_MilManage.GetMilOverlay(NG_VIEW_DISP), ng_left + 60, ng_top - 60, MIL_TEXT(temp));

							rect.SetdRectLTRB(rsltPara.dCentX[i] - (rsltPara.dWidth[i] / 2), rsltPara.dCentY[i] - (rsltPara.dHeight[i] / 2), rsltPara.dCentX[i] + (rsltPara.dWidth[i] / 2), rsltPara.dCentY[i] + (rsltPara.dHeight[i] / 2), 5);

							//20161017 ngh
							MgraArcAngle(M_DEFAULT, *m_MilManage.GetMilOverlay(NG_VIEW_DISP), rsltPara.dCentX[i], rsltPara.dCentY[i], 
								rsltPara.dLong[i], rsltPara.dShort[i], 0.1, 0, rsltPara.dAng[i], M_DEFAULT);
#endif
						}
					}
				}
			}
		}
	}
	else if(nDisplayNum == MAIN_DISP)
	{
		for(int i = 0; i < rsltPara.nNgCnt; i++)
		{
			for(int j = 0; j < m_dlgTeach->GetTeachPara().TeachOneGrabInspCnt; j++)
			{
				if((rsltPara.nDeviceIdx[i].x == nDeviceIdxX) && (rsltPara.nDeviceIdx[i].y == nDeviceIdxY + j))
				{
					if((rsltPara.nGrabIdx[i] == nGrabIdx))// && (rsltPara.nInspGrabIdx[i] == nInspGrabIdx))
					{
						if(rsltPara.bAddNg[i])
						{
							ng_left = nAddNg * 30 + 20;
							ng_top = nAddNg + 20;
							nAddNg++;
							temp.Format("NG %d = Add Defect", i + 1);
#ifdef CAMERA
							MgraColor(M_DEFAULT, M_COLOR_RED);
							MgraText(M_DEFAULT, *m_MilManage.GetMilOverlay(nDisplayNum), ng_left, ng_top, MIL_TEXT(temp));
#endif
						}
						else
						{
							//m_RsltIdx = i;
							//불량 위치 표시
							rsltPara.dCentX[i];
							rsltPara.dCentY[i];
							rsltPara.dWidth[i];
							rsltPara.dHeight[i];
							ng_left = rsltPara.dCentX[i] + (rsltPara.dWidth[i] / 2.0) + 10;
							ng_top = rsltPara.dCentY[i] - (rsltPara.dHeight[i] / 2.0) - 10;
							temp.Format("NG %d", i + 1);
#ifdef CAMERA
							MgraColor(M_DEFAULT, M_COLOR_RED);
							poStart.SetPoint((int)ng_left, (int)ng_top);
							//DrowArrow(MAIN_DISP, poStart);

							MgraText(M_DEFAULT, *m_MilManage.GetMilOverlay(nDisplayNum), ng_left + 10, ng_top - 30, MIL_TEXT(temp));

							rect.SetdRectLTRB(rsltPara.dCentX[i] - (rsltPara.dWidth[i] / 2.0), rsltPara.dCentY[i] - (rsltPara.dHeight[i] / 2.0), rsltPara.dCentX[i] + (rsltPara.dWidth[i] / 2.0), rsltPara.dCentY[i] + (rsltPara.dHeight[i] / 2.0), 5.0);

							//20161017 ngh
							MgraArcAngle(M_DEFAULT, *m_MilManage.GetMilOverlay(nDisplayNum), rsltPara.dCentX[i], rsltPara.dCentY[i], 
								rsltPara.dLong[i], rsltPara.dShort[i], 0.1, 0, rsltPara.dAng[i], M_DEFAULT);
#endif
						}
					}
				}
			}
		}
	}
}

typeInsRsltPara CLED_DisplayInspectDlg::GetRsltPara()
{
	return m_RsltPara;
}

typeInsRsltPara CLED_DisplayInspectDlg::GetRsltPara(int nDeviceIdxX, int nDeviceIdxY, int nGrabIdx, int nInspGrabIdx)
{
	typeInsRsltPara rsltPara;
	rsltPara.nNgCnt = 0;
	for(int i = 0; i < MAX_RSLT_NUM; i++)
	{
		rsltPara.nDeviceIdx[i].SetPoint(0, 0);
		rsltPara.dArea[i] = 0.;
		rsltPara.dCentX[i] = 0.;
		rsltPara.dCentY[i] = 0.;
		rsltPara.dHeight[i] = 0.;
		rsltPara.dAng[i] = 0.;//20161017 ngh
		rsltPara.dWidth[i] = 0.;
		rsltPara.dLong[i] = 0.;
		rsltPara.dShort[i] = 0.;
		rsltPara.dAng[i] = 0.;
		rsltPara.dBreadth[i] = 0.;
		rsltPara.nGrabIdx[i] = 0;
		rsltPara.nInspGrabIdx[i] = 0;
		rsltPara.bAddNg[i] = FALSE;
		rsltPara.bNg[i] = FALSE;
	}
	//불량을 찾았지만 표시가 안되던 버그를 수정했는데 임시방편으로 수정해서 확인이 필요
	/*
	int nDeviceIdxYTemp = 0; //20161014 sc 확인 필요

	if(m_dlgTeach->GetTeachPara().TeachOneGrabInspCnt > 1) //20161014 sc 확인 필요
	{
	nDeviceIdxY = nDeviceIdxY - m_RsltPara.nDeviceIdx[0].y ;

	}
	*/
	///////////////////////////////////////////////////////////////////////
	for(int i = 0; i < m_RsltPara.nNgCnt; i++)
	{
		for(int j = 0; j < m_dlgTeach->GetTeachPara().TeachOneGrabInspCnt; j++)
		{
			if((m_RsltPara.nDeviceIdx[i].x == nDeviceIdxX) && (m_RsltPara.nDeviceIdx[i].y == nDeviceIdxY + j))
			{
				if((m_RsltPara.nGrabIdx[i] == nGrabIdx))// && (rsltPara.nInspGrabIdx[i] == nInspGrabIdx))
				{
					rsltPara.nDeviceIdx[rsltPara.nNgCnt] = m_RsltPara.nDeviceIdx[i];
					rsltPara.dArea[rsltPara.nNgCnt] = m_RsltPara.dArea[i];
					rsltPara.dCentX[rsltPara.nNgCnt] = m_RsltPara.dCentX[i];
					rsltPara.dCentY[rsltPara.nNgCnt] = m_RsltPara.dCentY[i];
					rsltPara.dHeight[rsltPara.nNgCnt] = m_RsltPara.dHeight[i];
					rsltPara.dWidth[rsltPara.nNgCnt] = m_RsltPara.dWidth[i];
					rsltPara.dLong[rsltPara.nNgCnt] = m_RsltPara.dLong[i];
					rsltPara.dShort[rsltPara.nNgCnt] = m_RsltPara.dShort[i];
					rsltPara.dAng[rsltPara.nNgCnt] = m_RsltPara.dAng[i];
					rsltPara.dBreadth[rsltPara.nNgCnt] = m_RsltPara.dBreadth[i];
					rsltPara.nGrabIdx[rsltPara.nNgCnt] = m_RsltPara.nGrabIdx[i];
					rsltPara.nInspGrabIdx[rsltPara.nNgCnt] = m_RsltPara.nInspGrabIdx[i];
					rsltPara.bAddNg[rsltPara.nNgCnt] = m_RsltPara.bAddNg[i];
					rsltPara.bNg[rsltPara.nNgCnt] = m_RsltPara.bNg[i];
					rsltPara.bAlign[rsltPara.nNgCnt] = m_RsltPara.bAlign[i];
					rsltPara.nNgCnt++;
				}
			}
		}
	}

	return rsltPara;
}

void CLED_DisplayInspectDlg::NGDisplay(int nDeviceIdxX, int nDeviceIdxY, int nGrabIdx, int nInspGrabIdx, int nDisplayNum, typeInsRsltPara rsltPara)
{
	CString text, temp;
	CdRect rect;
	int nAddNg = 0;
	NGDiaplayClear(nDisplayNum);

	if(nDisplayNum == MAIN_DISP)
	{
		rsltPara = GetRsltPara(nDeviceIdxX, nDeviceIdxY, nGrabIdx, nInspGrabIdx);
		//Inspection Ng
		if(rsltPara.nNgCnt > 3)
		{
			m_scrol_NgDisp.SetScrollRange(0, rsltPara.nNgCnt - 3);
		}
		else
		{
			m_scrol_NgDisp.SetScrollRange(0, 0);
		}
	}

	NGToMainDisplay(nDeviceIdxX, nDeviceIdxY, nGrabIdx, nInspGrabIdx, nDisplayNum, rsltPara);
	int nNGinfoIdx = 0;
	double pixel_size = (double)m_dlgTeach->GetTeachPara().TeachPixelSize;

#ifdef CAMERA
	MgraControl(M_DEFAULT, M_FONT_X_SCALE , 1);
	MgraControl(M_DEFAULT, M_FONT_Y_SCALE , 1);
#endif
	if(nDisplayNum == NG_VIEW_DISP)
	{
		for(int i = 0; i < MAX_NG_DISP; i++)
		{
			if(rsltPara.nNgCnt <= i)
			{
				break;
			}
#ifdef CAMERA

			if(rsltPara.bAddNg[i])
			{
				MdispPan(*m_MilManage.GetMilDisplay(NG_DISP_1 + i + NG_VIEW_DISP), 200, 200);
				//20161014 sc
				//	temp.Format("Device X = %d, Device Y = %d, GrabIdx = %d", rsltPara.nDeviceIdx[m_dlgNgView->m_RsltIdx + i].x, rsltPara.nDeviceIdx[m_dlgNgView->m_RsltIdx + i].y, rsltPara.nGrabIdx[m_dlgNgView->m_RsltIdx + i]);
				//	MgraText(M_DEFAULT, *m_MilManage.GetMilOverlay(NG_DISP_1 + i + NG_VIEW_DISP), 200 - NG_INFO_PRINT_X, 200 - NG_INFO_PRINT_Y + (nNGinfoIdx++ * NG_INFO_PRINT_GAP), MIL_TEXT(temp));

				//	temp.Format("Add Defect", rsltPara.dCentX[m_dlgNgView->m_RsltIdx + i], rsltPara.dCentY[m_dlgNgView->m_RsltIdx + i]);
				//	MgraText(M_DEFAULT, *m_MilManage.GetMilOverlay(NG_DISP_1 + i + NG_VIEW_DISP), 200 - NG_INFO_PRINT_X, 200 - NG_INFO_PRINT_Y + (nNGinfoIdx++ * NG_INFO_PRINT_GAP), MIL_TEXT(temp));
			}
			else
			{
				MgraColor(M_DEFAULT, M_COLOR_RED);
				MbufCopy(*m_MilManage.GetMilDispBuf(nDisplayNum), *m_MilManage.GetMilDispBuf(NG_DISP_1 + i + NG_VIEW_DISP));
				MdispPan(*m_MilManage.GetMilDisplay(NG_DISP_1 + i + NG_VIEW_DISP), rsltPara.dCentX[m_dlgNgView->m_RsltIdx + i] - NG_DISPLAY_IAMGE_WIDTH, rsltPara.dCentY[m_dlgNgView->m_RsltIdx + i] - NG_DISPLAY_IAMGE_HEIGHT);
				//20161014 sc
				//	temp.Format("Device X = %d, Device Y = %d, GrabIdx = %d", rsltPara.nDeviceIdx[m_dlgNgView->m_RsltIdx + i].x, rsltPara.nDeviceIdx[m_dlgNgView->m_RsltIdx + i].y, rsltPara.nGrabIdx[m_dlgNgView->m_RsltIdx + i]);
				//	MgraText(M_DEFAULT, *m_MilManage.GetMilOverlay(NG_DISP_1 + i + NG_VIEW_DISP), rsltPara.dCentX[m_dlgNgView->m_RsltIdx + i] - NG_INFO_PRINT_X, rsltPara.dCentY[m_dlgNgView->m_RsltIdx + i] - NG_INFO_PRINT_Y + (nNGinfoIdx++ * NG_INFO_PRINT_GAP), MIL_TEXT(temp));

				//	temp.Format("Center X = %.1f, Center Y = %.1f", rsltPara.dCentX[m_dlgNgView->m_RsltIdx + i], rsltPara.dCentY[m_dlgNgView->m_RsltIdx + i]);
				//	MgraText(M_DEFAULT, *m_MilManage.GetMilOverlay(NG_DISP_1 + i + NG_VIEW_DISP), rsltPara.dCentX[m_dlgNgView->m_RsltIdx + i] - NG_INFO_PRINT_X, rsltPara.dCentY[m_dlgNgView->m_RsltIdx + i] - NG_INFO_PRINT_Y + (nNGinfoIdx++ * NG_INFO_PRINT_GAP), MIL_TEXT(temp));

				if(rsltPara.bAlign[i])
				{
					temp.Format("Align Search Fail !!");
				}
				else
				{
					temp.Format("Long = %.1f(um), Short = %.1f(um)", rsltPara.dLong[m_dlgNgView->m_RsltIdx + i] * pixel_size, rsltPara.dBreadth[m_dlgNgView->m_RsltIdx + i] * pixel_size);
				}
				MgraText(M_DEFAULT, *m_MilManage.GetMilOverlay(NG_DISP_1 + i + NG_VIEW_DISP), rsltPara.dCentX[m_dlgNgView->m_RsltIdx + i] - NG_INFO_PRINT_X, rsltPara.dCentY[m_dlgNgView->m_RsltIdx + i] - NG_INFO_PRINT_Y + (nNGinfoIdx++ * NG_INFO_PRINT_GAP), MIL_TEXT(temp));

				rect.SetdRectLTRB(rsltPara.dCentX[m_dlgNgView->m_RsltIdx + i] - (rsltPara.dWidth[m_dlgNgView->m_RsltIdx + i] / 2), rsltPara.dCentY[m_dlgNgView->m_RsltIdx + i] - (rsltPara.dHeight[m_dlgNgView->m_RsltIdx + i] / 2), rsltPara.dCentX[m_dlgNgView->m_RsltIdx + i] + (rsltPara.dWidth[m_dlgNgView->m_RsltIdx + i] / 2), rsltPara.dCentY[m_dlgNgView->m_RsltIdx + i] + (rsltPara.dHeight[m_dlgNgView->m_RsltIdx + i] / 2), 5);
				//MgraRect(M_DEFAULT, *m_MilManage.GetMilOverlay(NG_DISP_1 + i + NG_VIEW_DISP), rect.GetLeft(), rect.GetTop(), rect.GetRight(), rect.GetBottom());

				//20161017 ngh
				MgraArcAngle(M_DEFAULT, *m_MilManage.GetMilOverlay(NG_DISP_1 + i + NG_VIEW_DISP), rsltPara.dCentX[m_dlgNgView->m_RsltIdx + i], rsltPara.dCentY[m_dlgNgView->m_RsltIdx + i], 
					rsltPara.dLong[m_dlgNgView->m_RsltIdx + i], rsltPara.dShort[m_dlgNgView->m_RsltIdx + i], 0.1, 0, rsltPara.dAng[m_dlgNgView->m_RsltIdx + i], M_DEFAULT);
			}
#endif
			nNGinfoIdx = 0;
			text.Format(_T("NG%d"), m_dlgNgView->m_RsltIdx + i + 1);
			m_dlgNgView->GetDlgItem(IDC_LABLE_PRODUCT_NG1 + i)->SetWindowTextA(text);
		}
	}
	else if(nDisplayNum == MAIN_DISP)
	{
		for(int i = 0; i < MAX_NG_DISP; i++)
		{
			if(rsltPara.nNgCnt <= i)
			{
				break;
			}
#ifdef CAMERA
			if(rsltPara.bAddNg[i])
			{
				MdispPan(*m_MilManage.GetMilDisplay(NG_DISP_1 + i), 200, 200);
				//20161014 sc
				//temp.Format("Device X = %d, Device Y = %d, GrabIdx = %d", rsltPara.nDeviceIdx[m_dlgNgView->m_RsltIdx + i].x, rsltPara.nDeviceIdx[m_dlgNgView->m_RsltIdx + i].y, rsltPara.nGrabIdx[m_dlgNgView->m_RsltIdx + i]);
				//MgraText(M_DEFAULT, *m_MilManage.GetMilOverlay(NG_DISP_1 + i), 200 - NG_INFO_PRINT_X, 200 - NG_INFO_PRINT_Y + (nNGinfoIdx++ * NG_INFO_PRINT_GAP), MIL_TEXT(temp));

				//temp.Format("Add Defect", rsltPara.dCentX[m_dlgNgView->m_RsltIdx + i], rsltPara.dCentY[m_dlgNgView->m_RsltIdx + i]);
				//MgraText(M_DEFAULT, *m_MilManage.GetMilOverlay(NG_DISP_1 + i), 200 - NG_INFO_PRINT_X, 200 - NG_INFO_PRINT_Y + (nNGinfoIdx++ * NG_INFO_PRINT_GAP), MIL_TEXT(temp));
			}
			else
			{
				MgraColor(M_DEFAULT, M_COLOR_RED);
				MbufCopy(*m_MilManage.GetMilInspBuf(m_Rslt.m_bGrabComp, 0), *m_MilManage.GetMilDispBuf(NG_DISP_1 + i));
				MdispPan(*m_MilManage.GetMilDisplay(NG_DISP_1 + i), rsltPara.dCentX[m_RsltIdx + i] - NG_DISPLAY_IAMGE_WIDTH, rsltPara.dCentY[m_RsltIdx + i] - NG_DISPLAY_IAMGE_HEIGHT);
				//20161014 sc
				//	temp.Format("Device X = %d, Device Y = %d, GrabIdx = %d", rsltPara.nDeviceIdx[m_RsltIdx + i].x, rsltPara.nDeviceIdx[m_RsltIdx + i].y, rsltPara.nGrabIdx[m_RsltIdx + i]);
				//	MgraText(M_DEFAULT, *m_MilManage.GetMilOverlay(NG_DISP_1 + i), rsltPara.dCentX[m_RsltIdx + i] - NG_INFO_PRINT_X, rsltPara.dCentY[m_RsltIdx + i] - NG_INFO_PRINT_Y + (nNGinfoIdx++ * NG_INFO_PRINT_GAP), MIL_TEXT(temp));
				//	temp.Format("Center X = %.1f, Center Y = %.1f", rsltPara.dCentX[m_RsltIdx + i], rsltPara.dCentY[m_RsltIdx + i]);
				//	MgraText(M_DEFAULT, *m_MilManage.GetMilOverlay(NG_DISP_1 + i), rsltPara.dCentX[m_RsltIdx + i] - NG_INFO_PRINT_X, rsltPara.dCentY[m_RsltIdx + i] - NG_INFO_PRINT_Y + (nNGinfoIdx++ * NG_INFO_PRINT_GAP), MIL_TEXT(temp));

				if(rsltPara.bAlign[i])
				{
					temp.Format("Align Search Fail !!");
				}
				else
				{
					temp.Format("Long = %.1f(um), Short = %.1f(um)", rsltPara.dLong[m_RsltIdx + i] * pixel_size, rsltPara.dBreadth[m_RsltIdx + i] * pixel_size);
				}
				MgraText(M_DEFAULT, *m_MilManage.GetMilOverlay(NG_DISP_1 + i), rsltPara.dCentX[m_RsltIdx + i] - NG_INFO_PRINT_X, rsltPara.dCentY[m_RsltIdx + i] - NG_INFO_PRINT_Y + (nNGinfoIdx++ * NG_INFO_PRINT_GAP), MIL_TEXT(temp));

				rect.SetdRectLTRB(rsltPara.dCentX[m_RsltIdx + i] - (rsltPara.dWidth[m_RsltIdx + i] / 2), rsltPara.dCentY[m_RsltIdx + i] - (rsltPara.dHeight[m_RsltIdx + i] / 2), rsltPara.dCentX[m_RsltIdx + i] + (rsltPara.dWidth[m_RsltIdx + i] / 2), rsltPara.dCentY[m_RsltIdx + i] + (rsltPara.dHeight[m_RsltIdx + i] / 2), 5);
				//MgraRect(M_DEFAULT, *m_MilManage.GetMilOverlay(NG_DISP_1 + i), rect.GetLeft(), rect.GetTop(), rect.GetRight(), rect.GetBottom());	

				//20161017 ngh
				MgraArcAngle(M_DEFAULT, *m_MilManage.GetMilOverlay(NG_DISP_1 + i), rsltPara.dCentX[m_RsltIdx + i], rsltPara.dCentY[m_RsltIdx + i], 
					rsltPara.dLong[m_RsltIdx + i], rsltPara.dShort[m_RsltIdx + i], 0.1, 0, rsltPara.dAng[m_RsltIdx + i], M_DEFAULT);

			}
#endif
			nNGinfoIdx = 0;
			text.Format(_T("NG%d"), m_RsltIdx + i + 1);
			GetDlgItem(IDC_LABLE_PRODUCT_NG1 + i)->SetWindowTextA(text);
		}
	}
}

void CLED_DisplayInspectDlg::OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar)
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.
	SCROLLINFO  scrinfo;

	if(pScrollBar)
	{
		// 스크롤 바 검사
		if(pScrollBar == (CScrollBar*)&m_scrol_NgDisp)
		{
			// 스크롤바 정보를 가져온다.
			if(pScrollBar->GetScrollInfo(&scrinfo))
			{
				if(scrinfo.nMax > 0)
				{
					switch(nSBCode)
					{
					case SB_PAGEUP:   // 스크롤 바의 위쪽 바를 클릭
					case SB_LINEUP:   // 스크롤 바의 위쪽 화살표를 클릭
						scrinfo.nPos -= scrinfo.nPage;
						break;
					case SB_PAGEDOWN:  // 스크롤 바의 아래쪽 바를 클릭
					case SB_LINEDOWN:  // 스크롤 바의 아래쪽 화살표를 클릭
						scrinfo.nPos += scrinfo.nPage;
						break;
					case SB_THUMBPOSITION: // 스크롤바의 트랙이 움직이고 나서
					case SB_THUMBTRACK:  // 스크롤바의 트랙이 움직이는 동안
						scrinfo.nPos = scrinfo.nTrackPos;   // 16bit값 이상을 사용
						break;
					}

					// 스크롤바의 위치를 변경한다.
					//if(scrinfo.nPos < (GetRsltPara(m_poDeviceIdx[m_Rslt.m_bInspectionComp].x, m_poDeviceIdx[m_Rslt.m_bInspectionComp].y, m_nGrabIdx[m_Rslt.m_bInspectionComp], m_nInspGrabIdx[m_Rslt.m_bInspectionComp]).nNgCnt - 2) && scrinfo.nPos > -1)
					//{
					//	m_RsltIdx = scrinfo.nPos;
					//	pScrollBar->SetScrollPos(m_RsltIdx);
					//	NGDisplay(m_poDeviceIdx[m_Rslt.m_bInspectionComp].x, m_poDeviceIdx[m_Rslt.m_bInspectionComp].y, m_nGrabIdx[m_Rslt.m_bInspectionComp], m_nInspGrabIdx[m_Rslt.m_bInspectionComp], MAIN_DISP, m_RsltPara);
					//}
					//20170103 ngh
					NGDisplay(m_poDeviceIdx[m_Rslt.m_bGrabComp].x, m_poDeviceIdx[m_Rslt.m_bGrabComp].y, m_nGrabIdx[m_Rslt.m_bGrabComp], m_nInspGrabIdx[m_Rslt.m_bGrabComp], MAIN_DISP, m_RsltPara);
					m_RsltIdx = scrinfo.nPos;
					pScrollBar->SetScrollPos(m_RsltIdx);
				}
			}
		}
	}
	CDialogEx::OnVScroll(nSBCode, nPos, pScrollBar);
}

void CLED_DisplayInspectDlg::Grab()
{
#ifdef CAMERA
#ifdef BASLER
	BYTE *temp;
	int nStart = 0;
	CString strLog;
	//20161205 nht 카메라 다운현상 확인위해 로그 남김
	strLog.Format(_T("Grab start"));
	WriteLog(strLog, TYPE_LOG_MAIN, FALSE);
	
	//20161116 nht 라이브버튼으로 촬영시 카메라가 다운되던 현상 수정
	if(m_BaslerManage->Grab()){
		temp = m_BaslerManage->GetGrabImgBuf();
		nStart = m_btn_Start.IsWindowEnabled();
		if(nStart == 0)
		{
			MbufPut2d(*m_MilManage.GetMilGrabBuf(), 0, 0, CAMERA_WIDTH, CAMERA_HEIGHT, temp);
		}
		else
		{
			//20161117 nht

			MbufPut2d(*m_MilManage.GetMilDispBuf(MAIN_DISP), 0, 0, CAMERA_WIDTH, CAMERA_HEIGHT, temp);
			MbufCopy(*m_MilManage.GetMilDispBuf(MAIN_DISP), *m_MilManage.GetMilGrabBuf());


		}
		//20161205 nht 카메라 다운현상 확인위해 로그 남김
		strLog.Format(_T("Grab End"));
		WriteLog(strLog, TYPE_LOG_MAIN, FALSE);
	}
#else
	MbufCopy(*m_MilManage.GetMilDispBuf(MAIN_DISP), *m_MilManage.GetMilGrabBuf());
#endif
#endif
}

void CLED_DisplayInspectDlg::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.
	SYSTEMTIME	lpSystemTime;
	CString str, strLog, strFocus;

#ifndef NOTE_BOOK
	int org_rslt;
#endif

	int amp_stat = 0;

	switch(nIDEvent)
	{
	case TIMER_GRAB_LIVE:
		Grab();
		m_iFocus_Precision = (int)FocusValue();  //20160906 sc
		strFocus.Format(("FOCUS : %03d"),m_iFocus_Precision);
		m_btn_focus_precision.SetWindowTextA(strFocus);
		break;
	case TIMER_TIME_SHOW:
		GetLocalTime(&lpSystemTime);
		str.Format(_T("%04d:%02d:%02d\n%02d:%02d:%02d"), lpSystemTime.wYear, lpSystemTime.wMonth, lpSystemTime.wDay, lpSystemTime.wHour, lpSystemTime.wMinute, lpSystemTime.wSecond);
		m_btn_TimerData.SetCaption(str);
		break;
	case TIMER_ALL_ORIGIN_START:
		if(this->IsWindowVisible())
		{
			KillTimer(TIMER_ALL_ORIGIN_START);
			SetGrid();
#ifdef MOTION
			org_rslt= AfxMessageBox(_T("All Origin Start?\nCancel Btn Press Program Exit."), MB_YESNOCANCEL);

			if(org_rslt == IDYES)
			{
				//전체 원점 작업 yes
				strLog.Format(_T("All Origin Yes"));
				WriteLog(strLog, TYPE_LOG_MAIN);
				m_dlgManual->SetMotorOrigin();
			}
			else if(org_rslt == IDNO)
			{
				//전체 원점 작업 no
				strLog.Format(_T("All Origin No"));
				WriteLog(strLog, TYPE_LOG_MAIN);
			}
			else
			{
				//프로그램 종료
				strLog.Format(_T("Program Exit"));
				WriteLog(strLog, TYPE_LOG_MAIN);
				ClickExitviewBtn();
			}
#endif
		}
		break;
	case TIMER_MOTION_AMP:
		for(int i = 0; i < MAX_MOTION_CNT; i++)
		{
			amp_stat = m_dlgManual->GetServoAmp(i);
			if(amp_stat == -1)
			{
#ifdef MOTION
				m_bMotionAmp[i] = FALSE;
#endif
			}
		}
		break;
	}

	CDialogEx::OnTimer(nIDEvent);
}


void CLED_DisplayInspectDlg::ClickManualmviewBtn()
{
	// TODO: 여기에 메시지 처리기 코드를 추가합니다.
#ifdef _RELEASE
	CDialogParameterLock dlgParaLock;  //20161006 sc

	if(dlgParaLock.DoModal())
	{
		if(dlgParaLock.GetPassRet())
		{
			WindowVisible(MANUAL_WINDOW);
		}
		else
		{
			return;
		}
	}
#else
	WindowVisible(MANUAL_WINDOW);
#endif
}

void CLED_DisplayInspectDlg::ProductInfoView()
{
	CString text;
	text.Format(_T("%d"), m_RsltCnt.nTotalCnt);
	m_btn_Total.SetWindowTextA(text);

	text.Format(_T("%d"), m_RsltCnt.nOk1Cnt);
	m_btn_Pass.SetWindowTextA(text);

	text.Format(_T("%d"), m_RsltCnt.nNgCnt);
	m_btn_Fail.SetWindowTextA(text);

	if(m_RsltCnt.nTotalCnt > 0 && m_RsltCnt.nNgCnt > 0)
	{
		m_RsltCnt.nNgRat = (int)(((double)(m_RsltCnt.nNgCnt)/(double)(m_RsltCnt.nTotalCnt)) * 100.0);
	}

	text.Format(_T("%d %%"), m_RsltCnt.nNgRat);
	m_btn_FailRat.SetWindowTextA(text);
}

void CLED_DisplayInspectDlg::initProductInfoCnt()
{
	memset(&m_RsltCnt, NULL, sizeof(typeInsCntPara));
	ProductInfoView();
}

BOOL CLED_DisplayInspectDlg::WriteLog(CString text, int log_type, BOOL isList)
{
	EnterCriticalSection(&m_CSInline);

	FILE*	stream;
	char	strFile[MAX_PATH], strToFile[MAX_PATH];
	long	result, line;
	char	szTime[DAT_STR_LENG*3]; 
	CString bak_file;
	int limit_line = 500000;
	CString path;
	CString strTime;
	CString log, strLogType;
	char buffer[DAT_STR_LENG];
	CString strLogPath;

	switch(log_type)
	{
	case TYPE_LOG_MAIN:
		strLogType = MAIN_LOG_FILE_NAME;
		break;
	case TYPE_LOG_MANUAL:
		strLogType = MANUAL_LOG_FILE_NAME;
		break;
	case TYPE_LOG_TEACH:
		strLogType = TEACH_LOG_FILE_NAME;
		break;
	case TYPE_LOG_DATA:
		strLogType = DATA_LOG_FILE_NAME;
		break;
	case TYPE_LOG_DEBUG:
		strLogType = DEBUG_LOG_FILE_NAME;
		break;
	}


	SYSTEMTIME	lpSystemTime;
	GetLocalTime(&lpSystemTime);
	strTime.Format(_T("%04d%02d%02d"), lpSystemTime.wYear, lpSystemTime.wMonth, lpSystemTime.wDay);
	log.Format(_T("%04d-%02d-%02d-%02d:%02d:%02d:%03d->%s"), lpSystemTime.wYear, lpSystemTime.wMonth, lpSystemTime.wDay, lpSystemTime.wHour, lpSystemTime.wMinute, lpSystemTime.wSecond, lpSystemTime.wMilliseconds, text);

	if(log_type == TYPE_LOG_MAIN)
	{
		if(isList)
		{
			m_list_MainLog.InsertItem(0, log);
		}
	}

	sprintf(strFile, "%s\\%s\\%s_%s", LOG_PATH, strTime, strTime, strLogType);
	strLogPath.Format(_T("%s\\%s"), LOG_PATH, strTime);
	_MakeDirectories(strLogPath);

	if((stream = _tfopen(strFile, "a+")) == NULL)
	{
		HANDLE fd = CreateFile( strFile,
			GENERIC_READ|GENERIC_WRITE,
			//GENERIC_WRITE,
			FILE_SHARE_READ|FILE_SHARE_WRITE,
			NULL,
			OPEN_ALWAYS,
			FILE_ATTRIBUTE_NORMAL,
			NULL
			);

		stream = _tfopen(strFile, "a+");
		if(stream == NULL)
		{
			CloseHandle(fd);
			LeaveCriticalSection(&m_CSInline);
			return FALSE;
		}

		CloseHandle(fd);
	}

	//Check first time
	result = fseek(stream, 0L, SEEK_SET);
	if(result)
	{
		fclose(stream);
		LeaveCriticalSection(&m_CSInline);
		return FALSE;
	}
	line = 0;
	while(fgets(buffer, DAT_STR_LENG, stream)) 
	{
		line++;
		if(1 < line)
		{
			break;
		}
	}

	result = fseek(stream, 0L, SEEK_END);
	if(result)
	{
		fclose(stream);
		LeaveCriticalSection(&m_CSInline);
		return FALSE;
	}

	memset(szTime, NULL, sizeof(char) * 512 * 3);
	sprintf(szTime, "%s", log);
	fputs(szTime, stream);
	fputs("\n", stream);

	result = fseek(stream, 0L, SEEK_SET);
	if(result)
	{
		fclose(stream);
		LeaveCriticalSection(&m_CSInline);
		return FALSE;
	}

	//Check limit line
	line = 0;
	if(limit_line)
	{
		while(fgets(buffer, DAT_STR_LENG, stream)) line++;
		if(line > limit_line)
		{
			sprintf_s(strToFile, path.GetLength() + bak_file.GetLength() + 2, "%S\\%S", path, bak_file); 
			CopyFile((LPCSTR)strFile, (LPCSTR)strToFile, FALSE);
			fclose(stream);
			DeleteFile((LPCSTR)strFile);
		}
		else
		{
			fclose(stream);
		}
	}
	else
	{
		fclose(stream);
	}

	LeaveCriticalSection(&m_CSInline);

	return TRUE;
}

void CLED_DisplayInspectDlg::AllHide()
{
	m_dlgTeach->ShowWindow(SW_HIDE);
	m_dlgManual->ShowWindow(SW_HIDE);
	m_dlgData->ShowWindow(SW_HIDE);
}

void CLED_DisplayInspectDlg::WindowVisible(int win_type)
{
	//20161019 ht 메인 화면에서 메뉴 선택시 재선택 불가능하게 수정
	/*if(m_nViewMode == win_type)
	{
	return;
	}
	else
	{
	m_nViewMode = win_type;
	}*/



	switch(win_type)
	{
	case AUTO_WINDOW:
		AutoVisible();
		m_btn_Auto.SetValue(TRUE);
		break;
	case MANUAL_WINDOW:
		if(!m_dlgManual->IsWindowVisible()){
			ManualVisible();
		}
		else
		{
			return;
		}
		break;
	case TEACH_WINDOW:
		if(!m_dlgTeach->IsWindowVisible()){
			TeachVisible();
		}
		else
		{
			return;
		}
		break;
	case DATA_WINDOW:
		if(!m_dlgData->IsWindowVisible())
		{
			DataVisible();
		}
		else{
			return;
		}
		break;
	}

}

void CLED_DisplayInspectDlg::AutoVisible()
{
	AllHide();
}

void CLED_DisplayInspectDlg::ManualVisible()
{


	AllHide();
	m_dlgManual->ShowWindow(SW_SHOW);

}

void CLED_DisplayInspectDlg::TeachVisible()
{


	AllHide();
	m_dlgImageRect->ShowWindow(SW_SHOW);
	m_dlgTeach->ShowWindow(SW_SHOW);

}

void CLED_DisplayInspectDlg::DataVisible()
{


	AllHide();
	m_dlgData->ShowWindow(SW_SHOW);

}

void CLED_DisplayInspectDlg::ClickMainviewBtn()
{
	// TODO: 여기에 메시지 처리기 코드를 추가합니다.
	WindowVisible(AUTO_WINDOW);
}

void CLED_DisplayInspectDlg::ClickDataviewBtn()
{

	WindowVisible(DATA_WINDOW);


}

void CLED_DisplayInspectDlg::SetSelectModel(typedefModelPara select_model)
{
	CString name, path;
	m_SelectModel = select_model;
	name.Format(_T("%s"), m_SelectModel.cName);
	path.Format(_T("%s"), m_SelectModel.cPath);
	m_btn_SelectModelName.SetWindowTextA(name);
	m_dlgTeach->SetSavePath(path);
	::_MakeDirectories(path);
}

typedefModelPara CLED_DisplayInspectDlg::GetSelectModel()
{
	return m_SelectModel;
}

void CLED_DisplayInspectDlg::SetSeqParaReset(BOOL isReset)
{
	m_Flag.m_bFirstStart = FALSE;
	SeqReset(isReset);
}

void CLED_DisplayInspectDlg::SeqReset(BOOL isReset)
{
	for(int i = 0; i < MOT_PROCESS_MAX_NUM; i++)
	{
		for(int j = 0; j < MOT_STATUS_MAX_NUM; j++)
		{
			m_MDat[i].m_Seq[j].m_bAutoStart = isReset;
			m_MDat[i].m_Seq[j].m_iSubSeqIdx = 0;
		}
	}

	m_Flag.m_bProcessThread = TRUE;

	if(m_btn_Start.IsWindowEnabled() == 0)	//20161103 nht 센서팝업 플래그값 설정 
	{
		m_Flag.m_bAutoStart = TRUE;
	}
	else
	{
		m_Flag.m_bAutoStart = FALSE;
	}

}

int CLED_DisplayInspectDlg::AutoStart()
{
	CString strLog;
#ifdef MOTION
	BOOL isOrigin[MAX_MOTION_CNT];

	for(int i = 0; i < MAX_MOTION_CNT; i++)
	{
		isOrigin[i] = m_dlgManual->GetManualPara().ManualServoOrigin[i];
		if(!isOrigin[i])
		{
			strLog.Format(_T("Motion Num %d, Origin Error!!"), i);
			AfxMessageBox(_T(strLog));
			return 1;
		}
	}
#endif

	//SetTimer(TIMER_MOTION_AMP, 1000, NULL);

	if(m_Flag.m_bReset)
	{
		m_Flag.m_bFirstStart = FALSE;
	}
	else
	{
		m_Flag.m_bFirstStart = TRUE;
	}

	//First start
	if(!m_Flag.m_bFirstStart)
	{
		m_Flag.m_bReset = FALSE;
		m_Flag.m_bFirstStart = TRUE;
		SetTowerLamp(OUTPUT_LAM_YELLOW);
		SeqReset(TRUE);
		m_MDat[0].m_Seq[2].m_iSubSeqIdx = 100;
	}
	else
	{
		m_MDat[0].m_Seq[2].m_iSubSeqIdx = m_nBeforeSeq;
		SetTowerLamp(OUTPUT_LAM_GREEN);
	}

	for(int i = 0; i < MOT_PROCESS_MAX_NUM; i++)
	{
		for(int j = 0; j < MOT_STATUS_MAX_NUM; j++)
		{
			m_MDat[i].m_Seq[j].m_bAutoStart = TRUE;
		}
	}

	//CString path;
	//path.Format(_T("%s"), m_dlgData->GetSelectModel().cPath);
	//m_dlgTeach->LoadTeachPara(path);
	
	return 0;
}

BOOL CLED_DisplayInspectDlg::GetSeqStart()
{
	return m_MDat[0].m_Seq[2].m_bAutoStart;
}

int CLED_DisplayInspectDlg::AutoStop()
{
	int i, j;	
	//m_nBeforeSeq = m_MDat[0].m_Seq[2].m_iSubSeqIdx;

	for(i = 0; i < MOT_PROCESS_MAX_NUM; i++)
	{
		for(j = 0; j < MOT_STATUS_MAX_NUM; j++)
		{
			//m_Flag.m_bAutoStart = FALSE;
			m_MDat[i].m_Seq[j].m_bAutoStart = FALSE;
			m_MDat[0].m_Seq[2].m_iSubSeqIdx = 0;
		}
	}



	//m_dlgTeach->OnOffLight(FALSE, 0);

	SetTowerLamp(OUTPUT_LAM_YELLOW);


	m_btn_Manual.EnableWindow(TRUE);//20161103 nht 입구 센서 작동시 버튼 비활성화가 풀리게 설정
	m_btn_Teach.EnableWindow(TRUE);
	m_btn_Model.EnableWindow(TRUE);
	m_btn_UnitInit.EnableWindow(TRUE);
	m_btn_InspReset.EnableWindow(TRUE);

	//KillTimer(TIMER_MOTION_AMP);

	return 0;
}

LRESULT CLED_DisplayInspectDlg::OnMotMessage(WPARAM para0, LPARAM para1)
{
	// TODO: Add your control notification handler code here
	int i, j;
	CString temp;

	switch(para0)
	{
		//Alarm
	case 0:
		for(i = 0; i < MOT_PROCESS_MAX_NUM; i++)
		{
			for(j = 0; j < MOT_STATUS_MAX_NUM; j++)
			{
				if(m_MDat[i].m_Seq[j].m_bAlarm)
				{
					m_dlgLogAlarm->SetMessage(m_MDat[i].m_ProSts[j][m_MDat[i].m_Seq[j].m_iSubSeqIdx].m_cNGStatus);
					m_dlgLogAlarm->ShowWindow(SW_SHOW);

					if(m_bMotionAmp[1])
					{
						SetTowerLamp(OUTPUT_LAM_RED, TRUE);
					}

					//button
					m_btn_Start.EnableWindow(TRUE);
					m_btn_End.EnableWindow(FALSE);
					break;
				}
			}
		}
		break;
	default:
		AfxMessageBox(_T("Error : invalid index!(CLED_DisplayInspectDlg::OnMotMessage)"));
		break;
	}

	return 0;
}

void CLED_DisplayInspectDlg::AlarmOperation(int pro_idx, int sts_idx, int sub_seq_idx)
{

}

UINT CLED_DisplayInspectDlg::MotionThread(LPVOID pParam)
{
	CLED_DisplayInspectDlg *pdlg = (CLED_DisplayInspectDlg *)pParam;

	pdlg->MotionThread();

	return 0;
}

void CLED_DisplayInspectDlg::MotionThread()
{
	int rslt[MOT_PROCESS_MAX_NUM][MOT_STATUS_MAX_NUM];
	int i, j, prc_idx, seq_idx, film_idx;
	CString log;
	BOOL chk = FALSE;
	film_idx = 0;
	while(m_Flag.m_bProcessThread)
	{ 
		//Read digital input
		//m_Motion.getDigitalInput(IO_IN_MAX_NUM, m_bIOIn);

		chk = TRUE;
		for(i = 0; i < MOT_PROCESS_MAX_NUM; i++)
		{
			for(j = 0; j < MOT_STATUS_MAX_NUM; j++)
			{
				if(!m_MDat[i].m_Seq[j].m_bAutoStart)
				{
					chk = FALSE;
				}
			}
		}

		if(chk)
			//Auto mode이면
		{
			//Proccessing
			for(i = 0; i < MOT_PROCESS_MAX_NUM; i++)
			{
				for(j = 0; j < MOT_STATUS_MAX_NUM; j++)
				{
					rslt[i][j] = MotionProcess(i, j);

					if(1 == rslt[i][j])
					{
						//Auto status

					}
					else if(0 == rslt[i][j])
					{

						//Error

						//정지
						//AutoStop();

						//Alarm동작

						m_MDat[i].m_Seq[j].m_bAlarm = TRUE;

						//먼저 에러가 나는쪽 먼져 처리한다.
						::SendMessage(this->m_hWnd, USER_MSG_MOT, 0, 0);

						//리셋시 다음번 동작예약
						//m_MDat[i].m_Seq[j].m_iSubSeqIdx = m_MDat[i].m_Seq[j].m_iNextSubSeqIdx;

						//Buzzer on

						break;
					}
				}
			}
		}
		else
		{
			if( 1 == StopModeOperation(&prc_idx, &seq_idx) )
			{

			}
			else
			{
				//정지
				AutoStop();

				//Alarm동작

				//Error
				m_MDat[prc_idx].m_Seq[seq_idx].m_bAlarm = TRUE;

				//먼져 에러가 나는쪽 먼져 처리한다.
				::SendMessage(this->m_hWnd, USER_MSG_MOT, 0, 0);

				//Buzzer on

			}		
			Sleep(1);
		}
		Sleep(1);
	}

	AfxEndThread(0);
}

int CLED_DisplayInspectDlg::StopModeOperation(int *prc_idx, int *seq_idx)
{
	*prc_idx = 0;
	*seq_idx = 0;

	if(m_MDat[*prc_idx].m_Seq[*seq_idx].m_bAlarm)
	{
		return 1;
	}

	return 1;
}

BOOL CLED_DisplayInspectDlg::MotionProcess(int prc_idx, int seq_idx)
{
	BOOL rslt = TRUE;
	int insp_rslt = 0;
	int sub_seq_idx = 0;
	CString strLog, strScreenFilePath;
	BOOL chk_timeout = TRUE;
	char status_type[20];
	memset(status_type, NULL, sizeof(status_type));
	double p_time;
	SYSTEMTIME	lpSystemTime;

	switch(seq_idx)
	{
	case 0:
		chk_timeout = FALSE;
		break;
	case 1:
		//safe and limit sensor
		sub_seq_idx = m_MDat[prc_idx].m_Seq[seq_idx].m_iSubSeqIdx;
		switch(sub_seq_idx)
		{
		case 0:
			m_MDat[prc_idx].m_ProSts[seq_idx][sub_seq_idx].m_dTimeout = 10;//ms
			sprintf(m_MDat[prc_idx].m_ProSts[seq_idx][sub_seq_idx].m_cNGStatus, "EMO 감지 & 전원 확인 필요 -000-");
#ifdef MOTION
			if(m_MDat[prc_idx].m_Seq[seq_idx].m_bAutoStart)
			{
				if(!m_bMotionAmp[0] || !m_bMotionAmp[1] || !m_bMotionAmp[2])
				{
					AutoStop();
					m_btn_Start.EnableWindow(TRUE);	
					m_btn_End.EnableWindow(FALSE);
					m_dlgManual->SetAllHomeFlag(FALSE);
				}
				else if(m_dlgManual->GetInput(MOTION_Y, INPUT_PAUSE))
				{
					if(m_bMotionAmp[0] && m_bMotionAmp[1] && m_bMotionAmp[2])
					{	
						sprintf(m_MDat[prc_idx].m_ProSts[seq_idx][sub_seq_idx].m_cNGStatus, "Pause  Button 누름 -001-");
						AutoStop();
						m_btn_Start.EnableWindow(TRUE);	
						m_btn_End.EnableWindow(FALSE);
					}
				}
				else if(!m_dlgManual->GetInput(MOTION_Y, INPUT_AREA_SENSER))
				{
					if(m_bMotionAmp[0] && m_bMotionAmp[1] && m_bMotionAmp[2])
					{
						sprintf(m_MDat[prc_idx].m_ProSts[seq_idx][sub_seq_idx].m_cNGStatus, "Area Senser 감지 -002-");
						for(int i = 0; i < MAX_MOTION_CNT; i++)
						{
							m_dlgManual->SetMoveStop(i);
						}
						AutoStop();
						m_btn_Start.EnableWindow(TRUE);	
						m_btn_End.EnableWindow(FALSE);
					}
				}
				else
				{
					SetSeqIdx(prc_idx, seq_idx, 0);
				}
			}
#else
			SetSeqIdx(prc_idx, seq_idx, 0);
#endif

			/*if(m_MDat[prc_idx].m_Seq[seq_idx].m_bAutoStart)
			{
				if(!m_bMotionAmp[0] || !m_bMotionAmp[1] || !m_bMotionAmp[2])
				{
					AutoStop();
					m_btn_Start.EnableWindow(TRUE);	
					m_btn_End.EnableWindow(FALSE);
					m_dlgManual->SetAllHomeFlag(FALSE);
					break;
				}
				else
				{
					SetSeqIdx(prc_idx, seq_idx, 1);
				}
			}
			else
			{
				SetSeqIdx(prc_idx, seq_idx, 1);
			}*/
			break;
		case 1:
			m_MDat[prc_idx].m_ProSts[seq_idx][sub_seq_idx].m_dTimeout = 10;//ms
			sprintf(m_MDat[prc_idx].m_ProSts[seq_idx][sub_seq_idx].m_cNGStatus, "Pause  Button 누름 -001-");
			if(m_MDat[prc_idx].m_Seq[seq_idx].m_bAutoStart)
			{
				if(m_bMotionAmp[0] && m_bMotionAmp[1] && m_bMotionAmp[2])
				{
					if(!m_dlgManual->GetInput(MOTION_Y, INPUT_PAUSE))// && m_bMotionAmp[0] && m_bMotionAmp[1] && m_bMotionAmp[2])
					{
						SetSeqIdx(prc_idx, seq_idx, 2);
					}
					else// if(m_bMotionAmp[0] && m_bMotionAmp[1] && m_bMotionAmp[2])
					{
						if(m_bMotionAmp[0] && m_bMotionAmp[1] && m_bMotionAmp[2])
						{
							AutoStop();
							m_btn_Start.EnableWindow(TRUE);	
							m_btn_End.EnableWindow(FALSE);
							break;
						}
						else
						{
							SetSeqIdx(prc_idx, seq_idx, 2);
						}
					}
				}
				else
				{
					SetSeqIdx(prc_idx, seq_idx, 2);
				}
			}
			else
			{
				SetSeqIdx(prc_idx, seq_idx, 2);
			}
			break;
//#ifdef MOTION
			/*if(m_MDat[prc_idx].m_Seq[seq_idx].m_bAutoStart)
			{
				if(!m_dlgManual->GetInput(MOTION_Y, INPUT_PAUSE) && m_bMotionAmp[0] && m_bMotionAmp[1] && m_bMotionAmp[2])
				{
					SetSeqIdx(prc_idx, seq_idx, 2);
				}
				else
				{
					AutoStop();
					m_btn_Start.EnableWindow(TRUE);
					m_btn_End.EnableWindow(FALSE);
				}
#else
			if(1)
			{
				SetSeqIdx(prc_idx, seq_idx, 2);
#endif
			}
			else
			{
				SetSeqIdx(prc_idx, seq_idx, 2);
			}
			break;*/
		case 2:
			m_MDat[prc_idx].m_ProSts[seq_idx][sub_seq_idx].m_dTimeout = 10;//ms
			sprintf(m_MDat[prc_idx].m_ProSts[seq_idx][sub_seq_idx].m_cNGStatus, "Area Senser 감지 -002-");

			if(m_MDat[prc_idx].m_Seq[seq_idx].m_bAutoStart)
			{
				//AfxMessageBox(_T("Area Senser 1"));
				//20161118 nht &&에서 ||로
				if(m_bMotionAmp[0] || m_bMotionAmp[1] || m_bMotionAmp[2])
				{
					//AfxMessageBox(_T("Area Senser 2"));
					if(m_dlgManual->GetInput(MOTION_Y, INPUT_AREA_SENSER))// && m_bMotionAmp[0] && m_bMotionAmp[1] && m_bMotionAmp[2])
					{	
						//AfxMessageBox(_T("Area Senser 3"));
						SetSeqIdx(prc_idx, seq_idx, 0);
					}
					else// if(m_bMotionAmp[0] && m_bMotionAmp[1] && m_bMotionAmp[2])
					{
						//AfxMessageBox(_T("Area Senser 4"));
						if(m_bMotionAmp[0] && m_bMotionAmp[1] && m_bMotionAmp[2])
						{
							//AfxMessageBox(_T("Area Senser 5"));
							for(int i = 0; i < MAX_MOTION_CNT; i++)
							{
								m_dlgManual->SetMoveStop(i);
							}
							AutoStop();
							m_btn_Start.EnableWindow(TRUE);	
							m_btn_End.EnableWindow(FALSE);
							break;
						}
						else
						{
							SetSeqIdx(prc_idx, seq_idx, 0);
						}
					}
				}
				else
				{
					SetSeqIdx(prc_idx, seq_idx, 0);
				}
			}
			else
			{
				SetSeqIdx(prc_idx, seq_idx, 0);
			}
			break;
		case 999:

			break;

		}
		break;
	case 2:
		/////////////////////////////////////
		//Main sequence
		//
		sub_seq_idx = m_MDat[prc_idx].m_Seq[seq_idx].m_iSubSeqIdx;
		switch(sub_seq_idx)
		{
		case 0:

			break;
		case 100://파라메터 초기화
			if(m_btnLive.GetValue()) //20161014 sc
			{
				m_btnLive.SetValue(FALSE);
				KillTimer(TIMER_GRAB_LIVE);
			}
			m_btn_main_op_state.SetCaption("검사 대기중"); //20161006 sc
			m_Rslt.m_bInspectionStart = FALSE;
			strLog.Format(_T("Parameter Init"));
			WriteLog(strLog, TYPE_LOG_MAIN, FALSE);

			SetSeqIdx(prc_idx, seq_idx, 110);
			break;
		case 110://default Move
			m_nBeforeSeq = m_MDat[prc_idx].m_Seq[seq_idx].m_iSubSeqIdx;
			m_dlgManual->SetEzPosMove(MOTION_X, m_Rslt.m_lDefaultPos[MOTION_X]);
			m_dlgManual->SetEzPosMove(MOTION_Y, m_Rslt.m_lDefaultPos[MOTION_Y]);
			m_dlgManual->SetEzPosMove(MOTION_Z, m_Rslt.m_lDefaultPos[MOTION_Z]);

			strLog.Format(_T("Default Pos Move Start (Motion_X, Motion_Y, Motion_Z)"));
			WriteLog(strLog, TYPE_LOG_MAIN);

			SetSeqIdx(prc_idx, seq_idx, 120);
			break;
		case 120://default X Move Cmop
			m_MDat[prc_idx].m_ProSts[seq_idx][sub_seq_idx].m_dTimeout = TIMEOUT_MOTOR_MOVE;//ms
			sprintf(m_MDat[prc_idx].m_ProSts[seq_idx][sub_seq_idx].m_cNGStatus, "Ez Move Time Over Error=X 축동작 타임아웃(축 동작 실패)!-120-");

#ifdef MOTION
			if(m_dlgManual->GetEzPosComplete(MOTION_X, m_Rslt.m_lDefaultPos[MOTION_X]))
			{
				strLog.Format(_T("Motion_X Move Complete"));
				WriteLog(strLog, TYPE_LOG_MAIN, FALSE);
				SetSeqIdx(prc_idx, seq_idx, 130);
			}
#else
			strLog.Format(_T("Motion_X Move Complete"));
			WriteLog(strLog, TYPE_LOG_MAIN, FALSE);
			SetSeqIdx(prc_idx, seq_idx, 130);
#endif
			break;
		case 130://default Y Move Cmop
			m_MDat[prc_idx].m_ProSts[seq_idx][sub_seq_idx].m_dTimeout = TIMEOUT_MOTOR_MOVE;//ms
			sprintf(m_MDat[prc_idx].m_ProSts[seq_idx][sub_seq_idx].m_cNGStatus, "Ez Move Time Over Error=Y 축동작 타임아웃(축 동작 실패)!-130-");
#ifdef MOTION
			if(m_dlgManual->GetEzPosComplete(MOTION_Y, m_Rslt.m_lDefaultPos[MOTION_Y]))
			{
				strLog.Format(_T("Motion_Y Move Complete"));
				WriteLog(strLog, TYPE_LOG_MAIN, FALSE);
				SetSeqIdx(prc_idx, seq_idx, 140);
			}
#else
			strLog.Format(_T("Motion_Y Move Complete"));
			WriteLog(strLog, TYPE_LOG_MAIN, FALSE);
			SetSeqIdx(prc_idx, seq_idx, 140);
#endif
			break;
		case 140://default Z Move Cmop
			m_MDat[prc_idx].m_ProSts[seq_idx][sub_seq_idx].m_dTimeout = TIMEOUT_MOTOR_MOVE;//ms
			sprintf(m_MDat[prc_idx].m_ProSts[seq_idx][sub_seq_idx].m_cNGStatus, "Ez Move Time Over Error=Z 축동작 타임아웃(축 동작 실패)!-140-");
#ifdef MOTION
			if(m_dlgManual->GetEzPosComplete(MOTION_Z, m_Rslt.m_lDefaultPos[MOTION_Z]))
			{
				strLog.Format(_T("Motion_Z Move Complete"));
				WriteLog(strLog, TYPE_LOG_MAIN, FALSE);
				strLog.Format(_T("Inspection Start Wait(Left, Right Start Button Press)"));
				WriteLog(strLog, TYPE_LOG_MAIN);
				SetSeqIdx(prc_idx, seq_idx, 150);
			}
#else
			strLog.Format(_T("Motion_Z Move Complete"));
			WriteLog(strLog, TYPE_LOG_MAIN, FALSE);
			strLog.Format(_T("Inspection Start Wait(Left, Right Start Button Press)"));
			WriteLog(strLog, TYPE_LOG_MAIN);
			SetSeqIdx(prc_idx, seq_idx, 150);
#endif
			break;
		case 150://left, right start button(output on check)
			if(m_bRepeat)
			{
				if(m_edit_nRepeatMaxCnt > m_nRepeatCnt)
				{
					//strLog.Format(_T("Grid Drow Start"));
					//WriteLog(strLog, TYPE_LOG_MAIN);
					SetGrid();
					//strLog.Format(_T("Grid Drow End"));
					//WriteLog(strLog, TYPE_LOG_MAIN);
					m_nRepeatCnt++;
					SetTowerLamp(OUTPUT_LAM_GREEN);
					strLog.Format(_T("Repeat Inspection Start_%d"), m_nRepeatCnt);
					WriteLog(strLog, TYPE_LOG_MAIN);

					InspectionParaReset(TRUE);
					m_Rslt.m_bInspectionStart = TRUE;
					m_RsltPara.nNgCnt = 0;

					GetLocalTime(&lpSystemTime);

					sprintf(m_Rslt.m_cStartDate, "%04d%02d%02d", lpSystemTime.wYear, lpSystemTime.wMonth, lpSystemTime.wDay);
					sprintf(m_Rslt.m_cStartTime, "%02d%02d%02d", lpSystemTime.wHour, lpSystemTime.wMinute, lpSystemTime.wSecond);

					//memset(&m_RsltPara, NULL, sizeof(typeInsRsltPara));
					SetSeqIdx(prc_idx, seq_idx, 151);
				}
				else
				{
					m_nRepeatCnt = 0;
					m_bRepeat = FALSE;
				}
			}
#ifdef MOTION
			if(!m_bMotionAmp[0] || !m_bMotionAmp[1] || !m_bMotionAmp[2])
			{
				break;
			}

			if(m_dlgManual->GetInput(MOTION_Y, Di_START_BUTTON_1) && m_dlgManual->GetInput(MOTION_Y, Di_START_BUTTON_2))
			{
				SetGrid();
				SetTowerLamp(OUTPUT_LAM_GREEN);
				strLog.Format(_T("Inspection Start"));
				WriteLog(strLog, TYPE_LOG_MAIN);

				InspectionParaReset(TRUE);
				m_Rslt.m_bInspectionStart = TRUE;
				m_RsltPara.nNgCnt = 0;

				GetLocalTime(&lpSystemTime);

				sprintf(m_Rslt.m_cStartDate, "%04d%02d%02d", lpSystemTime.wYear, lpSystemTime.wMonth, lpSystemTime.wDay);
				sprintf(m_Rslt.m_cStartTime, "%02d%02d%02d", lpSystemTime.wHour, lpSystemTime.wMinute, lpSystemTime.wSecond);
				//memset(&m_RsltPara, NULL, sizeof(typeInsRsltPara));
				SetSeqIdx(prc_idx, seq_idx, 151);
			}
#endif
			break;
			//20161031 ngh
			//기존에는 wait pos 와 first device pos를 같은 위치라 보고 코드가 짜여짐. 이에 아래와 같이 변경
			//wait pos 와 first device pos 가 다른 경우 -> wait pos로 이동 후 first device pos 로 이동
		case 151:
			m_nBeforeSeq = m_MDat[prc_idx].m_Seq[seq_idx].m_iSubSeqIdx;
			m_Rslt.m_lMotPos[MOTION_X] = m_dlgTeach->GetTeachMovePos(MOTION_X, 0, 0, 0);
			m_Rslt.m_lMotPos[MOTION_Y] = m_dlgTeach->GetTeachMovePos(MOTION_Y, 0, 0, 0);
			m_Rslt.m_lMotPos[MOTION_Z] = m_dlgTeach->GetTeachMovePos(MOTION_Z, 0, 0, 0);

			m_dlgManual->SetEzPosMove(MOTION_X, m_Rslt.m_lMotPos[MOTION_X]);
			m_dlgManual->SetEzPosMove(MOTION_Y, m_Rslt.m_lMotPos[MOTION_Y]);
			m_dlgManual->SetEzPosMove(MOTION_Z, m_Rslt.m_lMotPos[MOTION_Z]);
			SetSeqIdx(prc_idx, seq_idx, 152);
			break;
		case 152:
			m_MDat[prc_idx].m_ProSts[seq_idx][sub_seq_idx].m_dTimeout = TIMEOUT_MOTOR_MOVE;//ms
			sprintf(m_MDat[prc_idx].m_ProSts[seq_idx][sub_seq_idx].m_cNGStatus, "Ez Move Time Over Error=X 축동작 타임아웃(축 동작 실패)!-152-");
#ifdef MOTION
			if(m_dlgManual->GetEzPosComplete(MOTION_X, m_Rslt.m_lMotPos[MOTION_X]))
			{
				strLog.Format(_T("Motion_X Move Complete"));
				WriteLog(strLog, TYPE_LOG_MAIN, FALSE);
				SetSeqIdx(prc_idx, seq_idx, 153);
			}
#else
			strLog.Format(_T("Motion_X Move Complete"));
			WriteLog(strLog, TYPE_LOG_MAIN, FALSE);
			SetSeqIdx(prc_idx, seq_idx, 153);
#endif
			break;
		case 153:
			m_MDat[prc_idx].m_ProSts[seq_idx][sub_seq_idx].m_dTimeout = TIMEOUT_MOTOR_MOVE;//ms
			sprintf(m_MDat[prc_idx].m_ProSts[seq_idx][sub_seq_idx].m_cNGStatus, "Ez Move Time Over Error=Y 축동작 타임아웃(축 동작 실패)!-153-");
#ifdef MOTION
			if(m_dlgManual->GetEzPosComplete(MOTION_Y, m_Rslt.m_lMotPos[MOTION_Y]))
			{
				strLog.Format(_T("Motion_Y Move Complete"));
				WriteLog(strLog, TYPE_LOG_MAIN, FALSE);
				SetSeqIdx(prc_idx, seq_idx, 154);
			}
#else
			strLog.Format(_T("Motion_Y Move Complete"));
			WriteLog(strLog, TYPE_LOG_MAIN, FALSE);
			SetSeqIdx(prc_idx, seq_idx, 154);
#endif
			break;
		case 154:
			m_MDat[prc_idx].m_ProSts[seq_idx][sub_seq_idx].m_dTimeout = TIMEOUT_MOTOR_MOVE;//ms
			sprintf(m_MDat[prc_idx].m_ProSts[seq_idx][sub_seq_idx].m_cNGStatus, "Ez Move Time Over Error=Z 축동작 타임아웃(축 동작 실패)!-154-");
#ifdef MOTION
			if(m_dlgManual->GetEzPosComplete(MOTION_Z, m_Rslt.m_lMotPos[MOTION_Z]))
			{
				strLog.Format(_T("Motion_Z Move Complete"));
				WriteLog(strLog, TYPE_LOG_MAIN, FALSE);
				SetSeqIdx(prc_idx, seq_idx, 160);
			}
#else
			strLog.Format(_T("Motion_Z Move Complete"));
			WriteLog(strLog, TYPE_LOG_MAIN, FALSE);
			SetSeqIdx(prc_idx, seq_idx, 160);
#endif
			break;
		case 160://x, y, z Teach Move
			m_nBeforeSeq = m_MDat[prc_idx].m_Seq[seq_idx].m_iSubSeqIdx;
			if(m_Rslt.m_nDeviceIdxY < m_Rslt.m_nMaxDeviceCntY)
			{
				m_btn_main_op_state.SetCaption("검사중"); //20161006 sc
				if((m_Rslt.m_nDeviceIdxY % 2) == 0)
				{
					m_Rslt.m_bGrabDirection = FALSE;
				}
				else
				{
					m_Rslt.m_bGrabDirection = TRUE;
				}

				if(m_Rslt.m_nDeviceIdxX < m_Rslt.m_nMaxDeviceCntX && !m_Rslt.m_bGrabDirection)
				{
					if(m_Rslt.m_nGrabIdx < m_Rslt.m_nMaxGrabCnt)
					{
						m_dlgTeach->OnOffLight(TRUE, 0);
						//Sleep(200);

						strLog.Format(_T("Device Idx X : %d, Device Idx Y : %d, Grab Idx : %d Move Start (Motion_X, Motion_Y)"), m_Rslt.m_nDeviceIdxX, m_Rslt.m_nDeviceIdxY, m_Rslt.m_nGrabIdx);
						WriteLog(strLog, TYPE_LOG_MAIN, FALSE);

						m_Rslt.m_lMotPos[MOTION_X] = m_dlgTeach->GetTeachMovePos(MOTION_X, m_Rslt.m_nDeviceIdxX, m_Rslt.m_nDeviceIdxY, m_Rslt.m_nGrabIdx);
						m_Rslt.m_lMotPos[MOTION_Y] = m_dlgTeach->GetTeachMovePos(MOTION_Y, m_Rslt.m_nDeviceIdxX, m_Rslt.m_nDeviceIdxY, m_Rslt.m_nGrabIdx);
						m_Rslt.m_lMotPos[MOTION_Z] = m_dlgTeach->GetTeachMovePos(MOTION_Z, m_Rslt.m_nDeviceIdxX, m_Rslt.m_nDeviceIdxY, m_Rslt.m_nGrabIdx);

						m_dlgManual->SetEzPosMove(MOTION_X, m_Rslt.m_lMotPos[MOTION_X]);
						m_dlgManual->SetEzPosMove(MOTION_Y, m_Rslt.m_lMotPos[MOTION_Y]);
						m_dlgManual->SetEzPosMove(MOTION_Z, m_Rslt.m_lMotPos[MOTION_Z]);

						for(int j = 0; j < m_dlgTeach->GetTeachPara().TeachOneGrabInspCnt; j++)
						{
							SetSelectGrid(1, m_Rslt.m_nDeviceIdxX, (m_Rslt.m_nDeviceIdxY * m_dlgTeach->GetTeachPara().TeachOneGrabInspCnt) + j, m_Rslt.m_nGrabIdx);
						}
						m_Rslt.m_nImageSel++;
						SetSeqIdx(prc_idx, seq_idx, 170);
					}
					else
					{
						if(m_Rslt.m_bGrabDirection)
						{
							m_Rslt.m_nGrabIdx = m_Rslt.m_nMaxGrabCnt - 1;
						}
						else
						{
							m_Rslt.m_nGrabIdx = 0;
						}

						//제품 cnt 증가
						//20161001 ngh

						SetSeqIdx(prc_idx, seq_idx, 160);
					}
				}
				else if(m_Rslt.m_nDeviceIdxX > -1 && m_Rslt.m_bGrabDirection)
				{
					if(m_Rslt.m_nGrabIdx > -1)
					{
						m_dlgTeach->OnOffLight(TRUE, 0);
						//Sleep(200);
						strLog.Format(_T("Device Idx X : %d, Device Idx Y : %d, Grab Idx : %d Move Start (Motion_X, Motion_Y)"), m_Rslt.m_nDeviceIdxX, m_Rslt.m_nDeviceIdxY, m_Rslt.m_nGrabIdx);
						WriteLog(strLog, TYPE_LOG_MAIN, FALSE);

						m_Rslt.m_lMotPos[MOTION_X] = m_dlgTeach->GetTeachMovePos(MOTION_X, m_Rslt.m_nDeviceIdxX, m_Rslt.m_nDeviceIdxY, m_Rslt.m_nGrabIdx);
						m_Rslt.m_lMotPos[MOTION_Y] = m_dlgTeach->GetTeachMovePos(MOTION_Y, m_Rslt.m_nDeviceIdxX, m_Rslt.m_nDeviceIdxY, m_Rslt.m_nGrabIdx);
						m_Rslt.m_lMotPos[MOTION_Z] = m_dlgTeach->GetTeachMovePos(MOTION_Z, m_Rslt.m_nDeviceIdxX, m_Rslt.m_nDeviceIdxY, m_Rslt.m_nGrabIdx);

						m_dlgManual->SetEzPosMove(MOTION_X, m_Rslt.m_lMotPos[MOTION_X]);
						m_dlgManual->SetEzPosMove(MOTION_Y, m_Rslt.m_lMotPos[MOTION_Y]);
						m_dlgManual->SetEzPosMove(MOTION_Z, m_Rslt.m_lMotPos[MOTION_Z]);

						for(int j = 0; j < m_dlgTeach->GetTeachPara().TeachOneGrabInspCnt; j++)
						{
							SetSelectGrid(1, m_Rslt.m_nDeviceIdxX, (m_Rslt.m_nDeviceIdxY * m_dlgTeach->GetTeachPara().TeachOneGrabInspCnt) + j, m_Rslt.m_nGrabIdx);
						}
						m_Rslt.m_nImageSel++;
						SetSeqIdx(prc_idx, seq_idx, 170);
					}
					else
					{
						m_Rslt.m_nGrabIdx = m_Rslt.m_nMaxGrabCnt - 1;
						SetSeqIdx(prc_idx, seq_idx, 160);
					}
				}
				else
				{
					if(m_Rslt.m_bGrabDirection)
					{
						m_Rslt.m_nDeviceIdxX = m_Rslt.m_nMaxDeviceCntX - 1;
					}
					else
					{
						m_Rslt.m_nDeviceIdxX = 0;
					}

					SetSeqIdx(prc_idx, seq_idx, 160);
				}
			}
			else
			{
				SetSeqIdx(prc_idx, seq_idx, 240);
			}
			break;
		case 170://default X Move Cmop
			m_MDat[prc_idx].m_ProSts[seq_idx][sub_seq_idx].m_dTimeout = TIMEOUT_MOTOR_MOVE;//ms
			sprintf(m_MDat[prc_idx].m_ProSts[seq_idx][sub_seq_idx].m_cNGStatus, "Ez Move Time Over Error=X 축동작 타임아웃(축 동작 실패)!-170-");
#ifdef MOTION
			if(m_dlgManual->GetEzPosComplete(MOTION_X, m_Rslt.m_lMotPos[MOTION_X]))
			{
				strLog.Format(_T("Motion_X Move Complete"));
				WriteLog(strLog, TYPE_LOG_MAIN, FALSE);
				SetSeqIdx(prc_idx, seq_idx, 180);
			}
#else
			strLog.Format(_T("Motion_X Move Complete"));
			WriteLog(strLog, TYPE_LOG_MAIN, FALSE);
			SetSeqIdx(prc_idx, seq_idx, 180);
#endif
			break;
		case 180://default Y Move Cmop
			m_MDat[prc_idx].m_ProSts[seq_idx][sub_seq_idx].m_dTimeout = TIMEOUT_MOTOR_MOVE;//ms
			sprintf(m_MDat[prc_idx].m_ProSts[seq_idx][sub_seq_idx].m_cNGStatus, "Ez Move Time Over Error=Y 축동작 타임아웃(축 동작 실패)!-180-");
#ifdef MOTION
			if(m_dlgManual->GetEzPosComplete(MOTION_Y, m_Rslt.m_lMotPos[MOTION_Y]))
			{
				strLog.Format(_T("Motion_Y Move Complete"));
				WriteLog(strLog, TYPE_LOG_MAIN, FALSE);
				SetSeqIdx(prc_idx, seq_idx, 190);
			}
#else
			strLog.Format(_T("Motion_Y Move Complete"));
			WriteLog(strLog, TYPE_LOG_MAIN, FALSE);
			SetSeqIdx(prc_idx, seq_idx, 190);
#endif
			break;
		case 190://grab
			if(m_Rslt.m_nImageSel == 0)
			{
				m_Rslt.m_bInspComp = FALSE;
			}
			else if((m_Rslt.m_nImageSel % 2) == 0)
			{
				m_Rslt.m_bInspComp = FALSE;
			}
			else
			{
				m_Rslt.m_bInspComp = TRUE;
			}

			strLog.Format(_T("Device Idx X : %d, Device Idx Y : %d, Grab  & Inspection Start"), m_Rslt.m_nDeviceIdxX, m_Rslt.m_nDeviceIdxY, m_Rslt.m_nGrabIdx, m_Rslt.m_nInspGrabIdx);
			WriteLog(strLog, TYPE_LOG_MAIN, FALSE);
			m_dGrabStartTime = clock();
			for(m_Rslt.m_nInspGrabIdx = 0; m_Rslt.m_nInspGrabIdx < m_Rslt.m_nMaxInspGrabCnt; m_Rslt.m_nInspGrabIdx++)
			{
				Sleep(200);
#ifdef CAMERA
#ifndef NOTE_BOOK
				Grab();
				MbufCopy(*m_MilManage.GetMilGrabBuf(), *m_MilManage.GetMilInspBuf(m_Rslt.m_bInspComp, m_Rslt.m_nInspGrabIdx));
#else
				Sleep(200);
				strLog.Format(_T("D:\\DGT_TEST_IMAGE\\TEST\\2Grab\\X_%02d_Y_%02d_G_%02d_I_%d.bmp"), m_Rslt.m_nDeviceIdxX, m_Rslt.m_nDeviceIdxY * m_dlgTeach->GetTeachPara().TeachOneGrabInspCnt, m_Rslt.m_nGrabIdx, m_Rslt.m_nInspGrabIdx);
				MbufImport((char *)(LPCTSTR)strLog, M_BMP, M_LOAD, *m_MilManage.GetMilSystem(), m_MilManage.GetMilGrabBuf());
				MbufCopy(*m_MilManage.GetMilGrabBuf(), *m_MilManage.GetMilInspBuf(m_Rslt.m_bInspComp, m_Rslt.m_nInspGrabIdx));
#endif
#endif
				//Light On
				if(m_Rslt.m_nInspGrabIdx == m_dlgTeach->GetTeachPara().TeachInspGrabCnt)
				{
					m_dlgTeach->OnOffLight(TRUE, 0);
				}
				else
				{
					m_dlgTeach->OnOffLight(TRUE, m_Rslt.m_nInspGrabIdx + 1);
				}

				strLog.Format(_T("Light On, Ch1 : %d, Ch2 : %d"), m_Rslt.m_nLightVal[m_Rslt.m_nInspGrabIdx][LIGHT_1_CH - 1], m_Rslt.m_nLightVal[m_Rslt.m_nInspGrabIdx][LIGHT_2_CH - 1]);
				WriteLog(strLog, TYPE_LOG_MAIN, FALSE);
			}
			m_poDeviceIdx[m_Rslt.m_bInspComp].x = m_Rslt.m_nDeviceIdxX;
			m_poDeviceIdx[m_Rslt.m_bInspComp].y = m_Rslt.m_nDeviceIdxY * m_dlgTeach->GetTeachPara().TeachOneGrabInspCnt;
			m_nGrabIdx[m_Rslt.m_bInspComp] = m_Rslt.m_nGrabIdx;
			m_dGrabEndTime = clock();
			p_time = (m_dGrabEndTime - m_dGrabStartTime) / 1000.0;
			strLog.Format(_T("Device Idx X : %d, Device Idx Y : %d, Grab Time %.3f"), m_poDeviceIdx[m_Rslt.m_bInspComp].x, m_poDeviceIdx[m_Rslt.m_bInspComp].y, p_time);
			WriteLog(strLog, TYPE_LOG_MAIN, FALSE);
			strLog.Format(_T("%.3f sec"), p_time);
			m_btn_GrabTime.SetWindowTextA(strLog);

			if(m_MDat[0].m_Seq[2].m_iSubSeqIdx)
			{
				SetSeqIdx(prc_idx, seq_idx, 200);
			}
			break;
		case 200://검사 스레드 시작
			m_btn_main_op_state.SetCaption("검사중"); //20161006 sc
			if(m_Rslt.m_bInspectionComp)
			{
				m_bInspThreadStart = TRUE;
				SetSeqIdx(prc_idx, seq_idx, 210);
			}
			break;
		case 210:
			if(m_Rslt.m_bGrabDirection)
			{
				m_Rslt.m_nGrabIdx--;
				if(m_Rslt.m_nGrabIdx > -1)
				{
					SetSeqIdx(prc_idx, seq_idx, 160);
				}
				else
				{
					SetSeqIdx(prc_idx, seq_idx, 220);
				}
			}
			else
			{
				m_Rslt.m_nGrabIdx++;
				if(m_Rslt.m_nGrabIdx < m_Rslt.m_nMaxGrabCnt)
				{
					SetSeqIdx(prc_idx, seq_idx, 160);
				}
				else
				{
					SetSeqIdx(prc_idx, seq_idx, 220);
				}
			}
			break;
		case 220:
			if(m_Rslt.m_bGrabDirection)
			{
				m_Rslt.m_nDeviceIdxX--;
				if(m_Rslt.m_nDeviceIdxX > -1)
				{
					m_Rslt.m_nGrabIdx = m_Rslt.m_nMaxGrabCnt - 1;;
					SetSeqIdx(prc_idx, seq_idx, 160);
				}
				else
				{
					SetSeqIdx(prc_idx, seq_idx, 230);
				}
			}
			else
			{
				m_Rslt.m_nDeviceIdxX++;
				if(m_Rslt.m_nDeviceIdxX < m_Rslt.m_nMaxDeviceCntX)
				{
					m_Rslt.m_nGrabIdx = 0;
					SetSeqIdx(prc_idx, seq_idx, 160);
				}
				else
				{
					SetSeqIdx(prc_idx, seq_idx, 230);
				}
			}
			break;
		case 230:
			m_Rslt.m_nDeviceIdxY++;
			if(m_Rslt.m_nDeviceIdxY < m_Rslt.m_nMaxDeviceCntY)
			{
				if(m_Rslt.m_bGrabDirection)
				{
					m_Rslt.m_nGrabIdx = 0;
					m_Rslt.m_nDeviceIdxX = 0;
				}
				else
				{
					m_Rslt.m_nGrabIdx = m_Rslt.m_nMaxGrabCnt - 1;
					m_Rslt.m_nDeviceIdxX = m_Rslt.m_nMaxDeviceCntX - 1;
				}
				SetSeqIdx(prc_idx, seq_idx, 160);
			}
			else
			{
				SetSeqIdx(prc_idx, seq_idx, 240);
			}
			break;
		case 240://default Move
			m_nBeforeSeq = m_MDat[prc_idx].m_Seq[seq_idx].m_iSubSeqIdx;
			m_dlgManual->SetEzPosMove(MOTION_X, m_Rslt.m_lDefaultPos[MOTION_X]);
			m_dlgManual->SetEzPosMove(MOTION_Y, m_Rslt.m_lDefaultPos[MOTION_Y]);

			strLog.Format(_T("Default Pos Move Start (Motion_X, Motion_Y, Motion_Z)"));
			WriteLog(strLog, TYPE_LOG_MAIN);

			//Light Off
			m_dlgTeach->OnOffLight(FALSE, LIGHT_COM_PORT);
			strLog.Format(_T("Light Off"));
			WriteLog(strLog, TYPE_LOG_MAIN, FALSE);

			SetSeqIdx(prc_idx, seq_idx, 250);
			break;
		case 250://default X Move Cmop
			m_MDat[prc_idx].m_ProSts[seq_idx][sub_seq_idx].m_dTimeout = TIMEOUT_MOTOR_MOVE;//ms
			sprintf(m_MDat[prc_idx].m_ProSts[seq_idx][sub_seq_idx].m_cNGStatus, "Ez Move Time Over Error=X 축동작 타임아웃(축 동작 실패)!-250-");
#ifdef MOTION
			if(m_dlgManual->GetEzPosComplete(MOTION_X, m_Rslt.m_lDefaultPos[MOTION_X]))
			{
				strLog.Format(_T("Motion_X Move Complete"));
				WriteLog(strLog, TYPE_LOG_MAIN, FALSE);
				SetSeqIdx(prc_idx, seq_idx, 260);
			}
#else
			strLog.Format(_T("Motion_X Move Complete"));
			WriteLog(strLog, TYPE_LOG_MAIN, FALSE);
			SetSeqIdx(prc_idx, seq_idx, 260);
#endif
			break;
		case 260://default Y Move Cmop
			m_MDat[prc_idx].m_ProSts[seq_idx][sub_seq_idx].m_dTimeout = TIMEOUT_MOTOR_MOVE;//ms
			sprintf(m_MDat[prc_idx].m_ProSts[seq_idx][sub_seq_idx].m_cNGStatus, "Ez Move Time Over Error=Y 축동작 타임아웃(축 동작 실패)!-260-");
#ifdef MOTION
			if(m_dlgManual->GetEzPosComplete(MOTION_Y, m_Rslt.m_lDefaultPos[MOTION_Y]))
			{
				strLog.Format(_T("Motion_Y Move Complete"));
				WriteLog(strLog, TYPE_LOG_MAIN, FALSE);
				SetSeqIdx(prc_idx, seq_idx, 270);
			}
#else
			strLog.Format(_T("Motion_Y Move Complete"));
			WriteLog(strLog, TYPE_LOG_MAIN, FALSE);
			SetSeqIdx(prc_idx, seq_idx, 270);
#endif
			break;
		case 270:
			if(!m_bInspThreadStart && m_Rslt.m_bInspectionComp)
			{
				CString time_temp, date_temp;;
				time_temp.Format(_T("%s"), m_Rslt.m_cStartTime);
				date_temp.Format(_T("%s"), m_Rslt.m_cStartDate);
				m_cStartTime.push_back(time_temp);
				m_cStartDate.push_back(date_temp);

				m_BeforeRsltPara = m_RsltPara;
				if(m_Rslt.m_nDeviceIdxX < 0)
				{
					m_Rslt.m_nDeviceIdxX = 0;
				}
				else if(m_Rslt.m_nDeviceIdxX > (m_dlgTeach->GetTeachPara().TeachArrayCntX - 1))
				{
					m_Rslt.m_nDeviceIdxX--;
				}

				if(m_Rslt.m_nGrabIdx < 0)
				{
					m_Rslt.m_nGrabIdx = 0;
				}
				else if(m_Rslt.m_nGrabIdx > (m_dlgTeach->GetTeachPara().TeachGrabCnt - 1))
				{
					m_Rslt.m_nGrabIdx--;
				}

				for(int j = 0; j < m_dlgTeach->GetTeachPara().TeachOneGrabInspCnt; j++)
				{
					GridDrow(6, m_Rslt.m_nDeviceIdxX, ((m_Rslt.m_nDeviceIdxY * m_dlgTeach->GetTeachPara().TeachOneGrabInspCnt) + j - m_dlgTeach->GetTeachPara().TeachOneGrabInspCnt), m_Rslt.m_nGrabIdx);
				}

				for(int i = 0; i < m_Rslt.m_nMaxInspGrabCnt; i++)
				{
					for(int j = 0; j < m_dlgTeach->GetTeachPara().TeachOneGrabInspCnt; j++)
					{
						m_Rslt.m_bInspRslt[i][j] = TRUE;
						m_Rslt.m_bInspectionRslt[i][j] = FALSE;
					}
				}

				strLog.Format(_T("%s\\%s\\%s\\"), CAPA_PATH, date_temp, m_SelectModel.cName);

				SetCapa(strLog);
				SetAllCapa();

				SetRsltFile();

				/*GetLocalTime(&lpSystemTime);
				::_MakeDirectories(_T("D:\\ScreenFile\\"));
				strScreenFilePath.Format(_T("D:\\ScreenFile\\%04d%02d%02d_%02d%02d%02d.bmp"), lpSystemTime.wYear, lpSystemTime.wMonth, lpSystemTime.wDay, lpSystemTime.wHour, lpSystemTime.wMinute, lpSystemTime.wSecond);
				ScreenCapture(strScreenFilePath);*/

				strLog.Format(_T("Inspection End"));
				WriteLog(strLog, TYPE_LOG_MAIN);
				strLog.Format(_T("\n\n"));
				WriteLog(strLog,TYPE_LOG_DEBUG,FALSE);
				

				SetSeqIdx(prc_idx, seq_idx, 100);
			}
			break;
		}
	}

	if(chk_timeout)
	{
		rslt = CheckTimeout(prc_idx, seq_idx, m_MDat[prc_idx].m_Seq[seq_idx].m_iSubSeqIdx, status_type);
	}

	return rslt;
}

void CLED_DisplayInspectDlg::SetSeq(int prc_idx, int seq_idx, int seq_sub_idx)
{
	m_MDat[prc_idx].m_ProSts[seq_idx][seq_sub_idx].m_iNGSeqIdx = 0;
	m_MDat[prc_idx].m_Seq[seq_idx].m_iSubSeqIdx = seq_sub_idx;
	m_MDat[prc_idx].m_Seq[seq_idx].m_iDispSubSeqIdx = seq_sub_idx;
	m_MDat[prc_idx].m_Seq[seq_idx].m_iNextSubSeqIdx = seq_sub_idx;
	m_MDat[prc_idx].m_Seq[seq_idx].m_bOpSts = 0;
	m_MDat[prc_idx].m_Seq[seq_idx].m_iDeviceIdx = 0;
}

//시퀀스 인덱스 넣기(리셋후 다음동작 않함)
void CLED_DisplayInspectDlg::SetSeqIdx(int prc_idx, int seq_idx, int seq_sub_idx)
{
	m_MDat[prc_idx].m_ProSts[seq_idx][seq_sub_idx].m_iNGSeqIdx = 0;
	m_MDat[prc_idx].m_Seq[seq_idx].m_iSubSeqIdx = seq_sub_idx;
	m_MDat[prc_idx].m_Seq[seq_idx].m_iDispSubSeqIdx = seq_sub_idx;
	m_MDat[prc_idx].m_Seq[seq_idx].m_iNextSubSeqIdx = seq_sub_idx;
	m_MDat[prc_idx].m_Seq[seq_idx].m_bOpSts = 0;
	m_MDat[prc_idx].m_Seq[seq_idx].m_iDeviceIdx = 0;
}

//시퀀스 인덱스 넣기(리셋하며 다음동작으로 넘어감)
void CLED_DisplayInspectDlg::SetSeqIdx(int prc_idx, int seq_idx, int seq_sub_idx, int next_seq_sub_idx, int type)
{
	m_MDat[prc_idx].m_ProSts[seq_idx][seq_sub_idx].m_iNGSeqIdx = 0;
	m_MDat[prc_idx].m_Seq[seq_idx].m_iSubSeqIdx = seq_sub_idx;
	m_MDat[prc_idx].m_Seq[seq_idx].m_iDispSubSeqIdx = seq_sub_idx;
	m_MDat[prc_idx].m_Seq[seq_idx].m_iNextSubSeqIdx = next_seq_sub_idx;
	m_MDat[prc_idx].m_Seq[seq_idx].m_bOpSts = type;
	m_MDat[prc_idx].m_Seq[seq_idx].m_iDeviceIdx = 0;
}

void CLED_DisplayInspectDlg::SetSeqIdx(int prc_idx, int seq_idx, int seq_sub_idx, int next_seq_sub_idx, int type, int device_idx)
{
	m_MDat[prc_idx].m_ProSts[seq_idx][seq_sub_idx].m_iNGSeqIdx = 0;
	m_MDat[prc_idx].m_Seq[seq_idx].m_iSubSeqIdx = seq_sub_idx;
	m_MDat[prc_idx].m_Seq[seq_idx].m_iDispSubSeqIdx = seq_sub_idx;
	m_MDat[prc_idx].m_Seq[seq_idx].m_iNextSubSeqIdx = next_seq_sub_idx;
	m_MDat[prc_idx].m_Seq[seq_idx].m_bOpSts = type;
	m_MDat[prc_idx].m_Seq[seq_idx].m_iDeviceIdx = device_idx;
}

//Check timeout
int CLED_DisplayInspectDlg::CheckTimeout(int prc_idx, int seq_idx, int seq_sub_idx, char* status_type)
{

	if(0 == m_MDat[prc_idx].m_ProSts[seq_idx][seq_sub_idx].m_iNGSeqIdx)
	{
		m_MDat[prc_idx].m_ProSts[seq_idx][seq_sub_idx].m_iNGSeqIdx++;
		m_MDat[prc_idx].m_ProSts[seq_idx][seq_sub_idx].m_dTimeoutS = _TimerCounter;
	}
	else
	{
		//Compare timeout
		if(0 <m_MDat[prc_idx].m_ProSts[seq_idx][seq_sub_idx].m_dTimeout)
		{
			m_MDat[prc_idx].m_ProSts[seq_idx][seq_sub_idx].m_dTimeoutE = _TimerCounter;
			if( m_MDat[prc_idx].m_ProSts[seq_idx][seq_sub_idx].m_dTimeout < (m_MDat[prc_idx].m_ProSts[seq_idx][seq_sub_idx].m_dTimeoutE - m_MDat[prc_idx].m_ProSts[seq_idx][seq_sub_idx].m_dTimeoutS) )
			{
				return 0;
			}
		}
	}

	sprintf(m_MDat[prc_idx].m_ProSts[seq_idx][seq_sub_idx].m_iTypeStatus, "%s", status_type);//축번호

	return 1;
}

//Check timeout
int CLED_DisplayInspectDlg::CheckDelay(int prc_idx, int seq_idx, int delay)
{
	if(0 == m_MDat[prc_idx].m_ProSts[seq_idx][m_MDat[prc_idx].m_Seq[seq_idx].m_iSubSeqIdx].m_iNGSeqIdx)
	{
		m_MDat[prc_idx].m_ProSts[seq_idx][m_MDat[prc_idx].m_Seq[seq_idx].m_iSubSeqIdx].m_iNGSeqIdx++;
		m_MDat[prc_idx].m_ProSts[seq_idx][m_MDat[prc_idx].m_Seq[seq_idx].m_iSubSeqIdx].m_dTimeoutS = _TimerCounter;
	}
	else
	{
		//Compare timeout
		if(0 < delay)
		{
			m_MDat[prc_idx].m_ProSts[seq_idx][m_MDat[prc_idx].m_Seq[seq_idx].m_iSubSeqIdx].m_dTimeoutE = _TimerCounter;
			if( delay < (m_MDat[prc_idx].m_ProSts[seq_idx][m_MDat[prc_idx].m_Seq[seq_idx].m_iSubSeqIdx].m_dTimeoutE - m_MDat[prc_idx].m_ProSts[seq_idx][m_MDat[prc_idx].m_Seq[seq_idx].m_iSubSeqIdx].m_dTimeoutS) )
			{
				return 1;
			}
		}
		else
		{
			return 1;
		}
	}

	return 0;
}

UINT CLED_DisplayInspectDlg::AutoDeleteThread(LPVOID pParam)
{
	CLED_DisplayInspectDlg *pdlg = (CLED_DisplayInspectDlg *)pParam;

	while(TRUE)
	{
		if(pdlg->m_bAutoDeleteStart)
		{
			pdlg->AutoDelete();
		}

		if(pdlg->m_bAutoDeleteEnd)
		{
			pdlg->m_bAutoDeleteStart = FALSE;
			pdlg->m_bAutoDeleteEnd = FALSE;
			break;
		}
		Sleep(1);
	}

	return 0;
}

void CLED_DisplayInspectDlg::AutoDelete()
{
	SYSTEMTIME	lpSystemTime;
	typedefDataPara dataPara;
	dataPara = m_dlgData->GetDataPara();

	GetLocalTime(&lpSystemTime);
	if((dataPara.DataDeleteHour == lpSystemTime.wHour) && (dataPara.DataDeleteMinute == lpSystemTime.wMinute) && !m_bDelete)
	{
		StartAutoDelete_Img(dataPara);
		m_bDelete = TRUE;
	}
	else if((dataPara.DataDeleteMinute != lpSystemTime.wMinute) && m_bDelete)
	{
		m_bDelete = FALSE;
	}
}

BOOL CLED_DisplayInspectDlg::StartAutoDelete_Img(typedefDataPara dataPara)
{
	CString Delete_Img_dir,Delete_Data_dir,Delete_Ng_Data_dir,Delete_Tab_Data_dir;
	CString Delete_Product_dir,Delete_Spc_dir;
	CTime now_time = CTime::GetCurrentTime();
	BOOL bDelFolder[31];

	for(int j = 0; j < 4; j++)
	{
		switch(j)
		{
		case 0:
			m_strImgPath.Format("%s\\GrabImage", RESULT_PATH);
			break;
		case 1:
			m_strImgPath.Format("%s\\NGImage", RESULT_PATH);
			break;
		case 2:
			m_strImgPath.Format("%s\\OKImage", RESULT_PATH);
			break;
		case 3:
			m_strImgPath.Format("%s\\CAPA", RESULT_PATH);
			break;
		}
		CheckDeleteDate(dataPara);

		int nYear = m_iDel_Year;
		int nDelYear = now_time.GetYear();

		if(1 == now_time.GetMonth() && now_time.GetDay() < m_iDel_RecentDay)
		{
			nDelYear = m_iDel_Year;
		}
		else if(1 == now_time.GetMonth() && now_time.GetDay() > m_iDel_RecentDay)
		{
			nDelYear = m_iDel_Year - 1;
		}

		for(int nYear = m_iDel_Year; nYear >= nDelYear; nYear--)		// 110930 ytlee
		{
			if(nYear != now_time.GetYear())
			{
				m_iDel_Month = 12;
				m_iDel_Day = m_iDel_RecentDay;		// 120104 ytlee
			}

			for(int nMonth = m_iDel_Month; nMonth > 0; nMonth--)
			{
				if(nMonth == 1 || nMonth == 3 || nMonth == 5 ||
					nMonth == 7 || nMonth == 8 || nMonth == 10 || nMonth == 12)		// 120104 ytlee
				{
					m_iDel_Day = 31;
				}
				else if(nMonth == 4 || nMonth == 6 || nMonth == 9 || nMonth == 11)
				{
					m_iDel_Day = 30;
				}
				else if(nMonth == 2)
				{
					m_iDel_Day = 28;
				}
				if(nYear == now_time.GetYear() && (nMonth == now_time.GetMonth() || dataPara.DataDeleteDay > now_time.GetDay()))		// 110901 ytlee
				{
					m_iDel_Day = m_iDel_RecentDay;
				}
				else
				{
					m_iDel_Day = 31;
				}

				for(int i = 0; i < 31; i++)
				{
					bDelFolder[i] = FALSE;
				}


				for(int nDay = 1; nDay <= m_iDel_Day; nDay++)
				{
					if(nMonth>9 && nDay>9)
						Delete_Img_dir.Format("%s\\%d%d%d",m_strImgPath, nYear, nMonth, nDay);
					else if(nMonth<10 && nDay>9)
						Delete_Img_dir.Format("%s\\%d0%d%d",m_strImgPath, nYear, nMonth, nDay);
					else if(nMonth>9 && nDay<10)
						Delete_Img_dir.Format("%s\\%d%d0%d",m_strImgPath, nYear, nMonth, nDay);
					else if(nMonth<10 && nDay<10)
						Delete_Img_dir.Format("%s\\%d0%d0%d",m_strImgPath, nYear, nMonth, nDay);

					CFileFind dbfile;

					if(dbfile.FindFile(Delete_Img_dir,0))
					{
						if(DeleteFolder(Delete_Img_dir))
							RemoveDirectory(Delete_Img_dir);
					}

					if(DeleteFolderSearch(Delete_Img_dir))
					{
						bDelFolder[nDay - 1] = TRUE;
					}
					else
					{
						bDelFolder[nDay - 1] = FALSE;
					}
				}

				for(int i = 0; i < m_iDel_Day; i++)
				{
					if(!bDelFolder[i])
					{
						return FALSE;
					}
				}
			}
		}
	}

	return TRUE;
}

BOOL CLED_DisplayInspectDlg::CheckDeleteDate(typedefDataPara dataPara)
{
	CString str;
	CTime now_time = CTime::GetCurrentTime();
	int temp_year, temp_month, temp_day;
	CTime delete_time;
	int TempDeleteDay=0;
	int i=0;
	temp_year=0;
	temp_month=0;
	temp_day=0;

	temp_day = now_time.GetDay();
	temp_month = now_time.GetMonth();
	temp_year = now_time.GetYear();

	int save_day = dataPara.DataDeleteDay;
	if( (temp_day - save_day) < 0)
	{
		if(now_time.GetMonth() == 2 || now_time.GetMonth() == 4 || now_time.GetMonth() == 6 ||
			now_time.GetMonth() == 8 || now_time.GetMonth() == 9 || now_time.GetMonth() == 11)
		{
			m_iDel_Year = now_time.GetYear();
			m_iDel_Month = now_time.GetMonth() - 1;
			m_iDel_RecentDay = 31 - (save_day - temp_day);
		}
		else if(now_time.GetMonth() == 5 || now_time.GetMonth() == 7 || now_time.GetMonth() == 10 || now_time.GetMonth() == 12)
		{
			m_iDel_Year = now_time.GetYear();
			m_iDel_Month = now_time.GetMonth() - 1;
			m_iDel_RecentDay = 30 - (save_day - temp_day);
		}
		else if(now_time.GetMonth() == 3)
		{
			m_iDel_Year = now_time.GetYear();
			m_iDel_Month = now_time.GetMonth() - 1;
			m_iDel_RecentDay = 28 - (save_day - temp_day);
		}
		else if(now_time.GetMonth() == 1)
		{
			m_iDel_Year = now_time.GetYear() - 1;
			m_iDel_Month = 12;
			m_iDel_RecentDay = 31 - (save_day - temp_day);
		}
	}
	else if( (temp_day - save_day) == 0)
	{
		if(now_time.GetMonth() == 1)
		{
			m_iDel_Year = now_time.GetYear() - 1;
			m_iDel_Month = 12;
			m_iDel_RecentDay = 0;
		}
		else
		{
			m_iDel_Year = now_time.GetYear();
			m_iDel_Month = now_time.GetMonth() - 1;
			m_iDel_RecentDay = 0;
		}
	}
	else
	{
		m_iDel_Year = now_time.GetYear();
		m_iDel_Month = now_time.GetMonth();
		m_iDel_RecentDay = temp_day - save_day;
	}
	/* >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>  저장 기간을 개월로 했을 경우
	int save_month = m_dlgSetup->m_SysPara.m_iPara[0];
	if( (temp_month - save_month) < 0)
	{
	m_iDel_Year = now_time.GetYear() - 1;

	for(i = 0; i <= (save_month - temp_month); i++)
	{
	if(temp_month == i+1)
	{
	m_iDel_Month = 12 - (save_month - temp_month);
	}
	}
	}
	else if( (temp_month - save_month) == 0)
	{
	m_iDel_Year = now_time.GetYear() - 1;

	m_iDel_Month = 12;
	}
	else
	{
	m_iDel_Year = now_time.GetYear();

	m_iDel_Month = temp_month - save_month;
	}
	*/// <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<
	/*  //저장 기간 3개월 일 경우
	if( (temp_month - 2) <= 0)
	{
	m_iDel_Year = now_time.GetYear() - 1;

	if(temp_month == 1)
	m_iDel_Month = 10;
	else if(temp_month == 2)
	m_iDel_Month = 11;
	}
	else
	{
	m_iDel_Year = now_time.GetYear();

	if(temp_month == 3)
	{
	m_iDel_Year = now_time.GetYear() - 1;
	m_iDel_Month = 12;
	}
	else
	m_iDel_Month = temp_month - 3;
	}
	*/
	return TRUE;
}

BOOL CLED_DisplayInspectDlg::DeleteFolderSearch(CString strFolder)
{
	CFileFind ff;
	CString strTarget = strFolder;
	CStringArray strFileArray;

	if(strTarget.Right(1) != '\\')
	{
		strTarget += '\\';
	}

	strTarget += "*.*";
	if(ff.FindFile(strTarget)==TRUE)
	{
		CString Filename;
		BOOL bFlag = true;
		while(bFlag == TRUE)
		{
			bFlag = ff.FindNextFile();
			Filename = ff.GetFilePath();
			if(ff.IsDots())
			{
				continue;
			}
			if(ff.IsDirectory())
			{
				return FALSE;
			}
		}
	}
	ff.Close();

	return TRUE;

}


BOOL CLED_DisplayInspectDlg::DeleteFolder(CString strFolder)
{
	CFileFind ff;
	CString strTarget = strFolder;
	CStringArray strFileArray;

	if(strTarget.Right(1) != '\\')
	{
		strTarget += '\\';
	}

	strTarget += "*.*";
	if(ff.FindFile(strTarget)==TRUE)
	{
		CString Filename;
		BOOL bFlag = true;
		while(bFlag == TRUE)
		{
			bFlag = ff.FindNextFile();
			Filename = ff.GetFilePath();
			if(ff.IsDots())
			{
				continue;
			}
			if(ff.IsDirectory())
			{
				DeleteFolder(Filename);
				RemoveDirectory(Filename);
			}
			else
			{
				DeleteFile(Filename);
			}
		}
	}
	ff.Close();

	return TRUE;
}

void CLED_DisplayInspectDlg::ClickMainOpStart()
{
	// TODO: 여기에 메시지 처리기 코드를 추가합니다.

	if(0 == AutoStart())
	{
		m_btn_Start.SetValue(TRUE);
		m_btn_Start.EnableWindow(FALSE);
		m_btn_End.EnableWindow(TRUE);

		m_btn_Manual.EnableWindow(FALSE);	//20161102 nht  검사 진행중 일 때 메뉴버튼 비활성화
		m_btn_Teach.EnableWindow(FALSE);
		m_btn_Model.EnableWindow(FALSE);
		m_btn_InspReset.EnableWindow(FALSE);
		m_btn_UnitInit.EnableWindow(FALSE);

		GetDlgItem(IDB_MAIN_MOTION_STOP)->EnableWindow(TRUE);
		m_btn_main_op_state.SetCaption("검사 대기중"); //20161006 sc
	}
	else
	{
		m_btn_Start.SetValue(FALSE);
		m_btn_Start.EnableWindow(TRUE);
		m_btn_End.EnableWindow(FALSE);
		GetDlgItem(IDB_MAIN_MOTION_STOP)->EnableWindow(FALSE);
	}

	CString strLog;
	strLog.Format(_T("Auto Inspection Start"));
	WriteLog(strLog, TYPE_LOG_MAIN);

	m_bRepeat = FALSE;
}


void CLED_DisplayInspectDlg::ClickMainOpStop()
{
	// TODO: 여기에 메시지 처리기 코드를 추가합니다.
	AutoStop();

	m_BeforeRsltPara = m_RsltPara;
	m_btn_Start.EnableWindow(TRUE);
	m_btn_End.EnableWindow(FALSE);

	m_btn_Start.SetValue(FALSE);
	m_btn_Manual.EnableWindow(TRUE);	//20161102 nht  검사중이 아닐때 메뉴버튼 활성화
	m_btn_Teach.EnableWindow(TRUE);
	m_btn_Model.EnableWindow(TRUE);
	m_btn_InspReset.EnableWindow(TRUE);
	m_btn_UnitInit.EnableWindow(TRUE);

	for(int i = 0; i < MAX_MOTION_CNT; i++)
	{
		//m_dlgManual->SetMoveStop(i);
	}
	CString strLog;
	strLog.Format(_T("Auto Inspection Stop"));
	WriteLog(strLog, TYPE_LOG_MAIN);
	m_btn_main_op_state.SetCaption("검사 정지"); //20161006 sc
}

void CLED_DisplayInspectDlg::ClickMainOpInit()
{
	// TODO: 여기에 메시지 처리기 코드를 추가합니다.
	CString strLog;
	int Motion_StateFlag = 0;

	for(int i = 0; i < MAX_MOTION_CNT; i++)		//20161104 NHT Emergency Stop을 눌렀을때 모터를 초기화시켜 다시 재설정해주는 부분
	{
		Motion_StateFlag += m_dlgManual->GetServoAmp(i);
	}

	if((0 <= Motion_StateFlag ) && (Motion_StateFlag < MAX_MOTION_CNT))
	{
		for(int i = 0; i < MAX_MOTION_CNT; i++)
		{
			strLog.Format(_T("Motion Num %d Init Comp !!"), i);
			if(!m_dlgManual->InitMotion(i))
			{
				strLog.Format(_T("Motion Num %d Init Fail !!"), i);

			}
			WriteLog(strLog, TYPE_LOG_MANUAL);
		}
		Sleep(1000);
		m_dlgManual->SetMotorOrigin();
	}
	else if(Motion_StateFlag == -3)
	{
		AfxMessageBox(_T("EMO Button!!"));
		strLog.Format(_T("EMO Button!!"));
		WriteLog(strLog, TYPE_LOG_MANUAL);
	}
	else
	{
		m_dlgManual->SetMotorOrigin();
	}	

	ClickMainInspectionReset();
}

double CLED_DisplayInspectDlg::FocusValue() //20160906 sc
{
	double var = 0.;
#ifdef CAMERA
	BYTE *by_source_orgin = NULL, *by_source;

	long pitch = 0, pitch2 = 0;
	long width = 0, height = 0;

	CString temp = _T("");
	double m_edit_multi = 1.0;
	double *by_source_dest = new double[CAMERA_WIDTH * CAMERA_HEIGHT];

	MIL_ID monobuf;
	long left, right, top, bottom, wd, ht;
	double l = 0,t = 0,r = 0,b = 0;

	left = (long)m_dlgTeach->GetTeachPara().TeachRegion[0][0][1].GetLeft();
	top = (long)m_dlgTeach->GetTeachPara().TeachRegion[0][0][1].GetTop();
	right = (long)m_dlgTeach->GetTeachPara().TeachRegion[0][0][1].GetRight();
	bottom = (long)m_dlgTeach->GetTeachPara().TeachRegion[0][0][1].GetBottom();

	wd = right - left;
	ht = bottom - top;

	//Edge Rect 
	MbufAlloc2d(*m_MilManage.GetMilSystem(), wd, ht, 8L+M_UNSIGNED,M_IMAGE+M_DISP+M_PROC+M_GRAB, &monobuf);
	MbufClear(monobuf, 0L);

	MbufInquire(monobuf, M_HOST_ADDRESS, &by_source);

	MbufInquire(*m_MilManage.GetMilGrabBuf(), M_HOST_ADDRESS, &by_source_orgin);
	MbufInquire(monobuf, M_PITCH, &pitch);
	MbufInquire(monobuf, M_SIZE_X, &width);
	MbufInquire(monobuf, M_SIZE_Y, &height);
	MbufInquire(*m_MilManage.GetMilGrabBuf(), M_PITCH, &pitch2);

	RGBtoGrayF((const long)(width), (const long)(height), (const long)(pitch), (const long)(pitch2), left, top, by_source_orgin, by_source);

	var = EdgeRoberts_D((long)(width), (long)(height), (long)(pitch), by_source, by_source_dest, m_edit_multi);

	MbufFree(monobuf);
	delete [] by_source_dest;
#endif
	return var;
}

int CLED_DisplayInspectDlg::RGBtoGrayF(const long width, const long height, const long dest_pitch, const long src_pitch, const long src_left, const long src_top, 
	BYTE* img_source_r, BYTE* img_dest)  //20160906 sc
{
	register long i, j;

	long idx, idx2;
	long start;

	int val = 0;
	int cnt = 0;

	//원본 이미지의 시작점. 
	//열 * 피치 + 행
	start = src_top * src_pitch + src_left;

	for(j = 0 ; j < height ; j++)
	{
		for(i = 0 ; i < width ; i++)
		{
			idx = j * dest_pitch + i;
			idx2 = start + (src_pitch * j + i);

			img_dest[idx] = img_source_r[idx2];
			val = val + img_dest[idx];
			cnt++;
		}
	}

	val = val / cnt;	//평균밝기

	return val;
}

double CLED_DisplayInspectDlg::EdgeRoberts_D(long width, long height, long pitch, BYTE* img_source, double* img_dest, double multi)  //20160906 sc
{
	double dev = 0;
	register long i, j;

	long w = width;
	long h = height;

	long h1, h2;
	long idx, cnt;
	double max;
	double hval, diff;
	max = 0;
	if(1 > multi)
	{
		multi = 1;
	}

	for(j = 10 ; j < h - 10 ; j++)
	{
		for(i = 10 ; i < w - 10 ; i++)
		{
			idx = j * pitch + i;
			h1 = img_source[idx] - img_source[(j - 10) * pitch + (i - 10)];
			h2 = img_source[idx] - img_source[(j - 10) * pitch + (i + 10)];

			hval = sqrt( (double)h1 * h1 + h2 * h2 );

			//Multi
			hval = hval * multi;

			img_dest[idx] = hval;

			if(max < hval)
			{
				max = hval;
			}
		}
	}

	max = max * 0.8;

	//Average difference
	diff = 0;
	cnt = 0;
	for(j = 1 ; j < h - 1 ; j++)
	{
		for(i = 1 ; i < w - 1 ; i++)
		{
			idx = j * pitch + i;

			if(max < img_dest[idx])
			{
				diff = diff + img_dest[idx];
				cnt++;
			}
		}
	}

	diff = diff / cnt;

	return diff;
}

void CLED_DisplayInspectDlg::SetTowerLamp(int nLampColor, BOOL bBuzzerOn)
{
	if(m_bMotionAmp[1])
	{
		for(int i = 0; i < MAX_OUTPUT_NUM; i++)
		{
			m_dlgManual->WriteOutput(MOTION_Y, i, FALSE);
		}

		m_dlgManual->WriteOutput(MOTION_Y, nLampColor, TRUE);
		m_dlgManual->WriteOutput(MOTION_Y, OUTPUT_LAM_BUZZER, bBuzzerOn);
	}
}

void CLED_DisplayInspectDlg::SetInspectionReset(BOOL isReset)
{
	m_nRepeatCnt = 0;
	m_Flag.m_bReset = TRUE;
	InspectionParaReset(isReset);
}

void CLED_DisplayInspectDlg::ClickMainInspectionReset()
{
	// TODO: 여기에 메시지 처리기 코드를 추가합니다.
	CString strLog;
	strLog.Format(_T("Inspection Reset"));
	WriteLog(strLog, TYPE_LOG_MAIN);
	SetInspectionReset(FALSE);
}

//20161017 ngh
void CLED_DisplayInspectDlg::InitRsltPara()
{
	m_RsltPara.nNgCnt = 0;
	for(int i = 0; i < MAX_RSLT_NUM; i++)
	{
		m_RsltPara.nDeviceIdx[i].SetPoint(0, 0);
		m_RsltPara.dArea[i] = 0.;
		m_RsltPara.dCentX[i] = 0.;
		m_RsltPara.dCentY[i] = 0.;
		m_RsltPara.dHeight[i] = 0.;
		m_RsltPara.dWidth[i] = 0.;
		m_RsltPara.dAng[i] = 0.;
		m_RsltPara.nGrabIdx[i] = 0;
		m_RsltPara.nInspGrabIdx[i] = 0;
		m_RsltPara.bAddNg[i] = FALSE;
		m_RsltPara.bNg[i] = FALSE;
		m_RsltPara.bAlign[i] = FALSE;
	}
}

//20161017 ngh
void CLED_DisplayInspectDlg::InitBeforeRsltPara()
{
	m_BeforeRsltPara.nNgCnt = 0;
	for(int i = 0; i < MAX_RSLT_NUM; i++)
	{
		m_BeforeRsltPara.nDeviceIdx[i].SetPoint(0, 0);
		m_BeforeRsltPara.dArea[i] = 0.;
		m_BeforeRsltPara.dCentX[i] = 0.;
		m_BeforeRsltPara.dCentY[i] = 0.;
		m_BeforeRsltPara.dHeight[i] = 0.;
		m_BeforeRsltPara.dWidth[i] = 0.;
		m_BeforeRsltPara.dAng[i] = 0.;
		m_BeforeRsltPara.nGrabIdx[i] = 0;
		m_BeforeRsltPara.nInspGrabIdx[i] = 0;
		m_BeforeRsltPara.bAddNg[i] = FALSE;
		m_BeforeRsltPara.bNg[i] = FALSE;
		m_BeforeRsltPara.bAlign[i] = FALSE;
	}
}

//20161017 ngh
void CLED_DisplayInspectDlg::InitInsRsltPara(int idx)
{
	m_InsRsltPara[idx].nNgCnt = 0;
	for(int i = 0; i < MAX_RSLT_NUM; i++)
	{
		m_InsRsltPara[idx].nDeviceIdx[i].SetPoint(0, 0);
		m_InsRsltPara[idx].dArea[i] = 0.;
		m_InsRsltPara[idx].dCentX[i] = 0.;
		m_InsRsltPara[idx].dCentY[i] = 0.;
		m_InsRsltPara[idx].dHeight[i] = 0.;
		m_InsRsltPara[idx].dWidth[i] = 0.;
		m_InsRsltPara[idx].dAng[i] = 0.;
		m_InsRsltPara[idx].nGrabIdx[i] = 0;
		m_InsRsltPara[idx].nInspGrabIdx[i] = 0;
		m_InsRsltPara[idx].bAddNg[i] = FALSE;
		m_InsRsltPara[idx].bNg[i] = FALSE;
		m_InsRsltPara[idx].bAlign[i] = FALSE;
	}
}

void CLED_DisplayInspectDlg::InspectionParaReset(BOOL isReset)
{
	InitRsltPara();
	InitInsRsltPara(0);
	InitInsRsltPara(1);

	for(int i = 0; i < 3; i++)
	{
		for(int j = 0; i < 3; i++)
		{
			m_Rslt.m_bInspRslt[i][j] = TRUE;
			m_Rslt.m_bInspectionRslt[i][j] = FALSE;
			m_Rslt.m_bAlignInsp[i] = FALSE;
		}
	}
	m_Rslt.m_nImageSel = 0;
	m_Rslt.m_bGrabComp = FALSE;
	m_Rslt.m_bInspectionComp = TRUE;
	m_Rslt.m_bInspectionStart = FALSE;
	m_Rslt.m_nDeviceIdxX = 0;
	m_Rslt.m_nDeviceIdxY = 0;
	m_Rslt.m_nGrabIdx = 0;
	m_Rslt.m_nOneGrabInspCnt = m_dlgTeach->GetTeachPara().TeachOneGrabInspCnt;

	m_Rslt.m_nMaxInspGrabCnt = m_dlgTeach->GetTeachPara().TeachInspGrabCnt;

	m_Rslt.m_nMaxDeviceCntX = m_dlgTeach->GetTeachPara().TeachDeviceCntX;
	m_Rslt.m_nMaxDeviceCntY = m_dlgTeach->GetTeachPara().TeachDeviceCntY;
	m_Rslt.m_nMaxGrabCnt = m_dlgTeach->GetTeachPara().TeachGrabCnt;
	for(int i = 0; i < MAX_INSP_GRAB_CNT; i++)
	{
		m_Rslt.m_nLightVal[i][LIGHT_1_CH - 1] = m_dlgTeach->GetTeachPara().TeachLight[i][LIGHT_1_CH - 1];
		m_Rslt.m_nLightVal[i][LIGHT_2_CH - 1] = m_dlgTeach->GetTeachPara().TeachLight[i][LIGHT_2_CH - 1];
	}

	for(int i = 0; i < MAX_MOTION_CNT; i++)
	{
		m_Rslt.m_lMotPos[i] = 0;
		m_Rslt.m_lDefaultPos[i] = 0;
	}
	m_Rslt.m_nInspGrabIdx = 0;
	m_Rslt.m_lDefaultPos[MOTION_X] = m_dlgTeach->GetWaitMovePos(MOTION_X);
	m_Rslt.m_lDefaultPos[MOTION_Y] = m_dlgTeach->GetWaitMovePos(MOTION_Y);
	m_Rslt.m_lDefaultPos[MOTION_Z] = m_dlgTeach->GetWaitMovePos(MOTION_Z);

	m_nInspCnt = 0;
	m_nCapaCnt = 0;
	m_nAllCapaCnt = 0;
	//MinusProductInfoCnt();

	SeqReset(isReset);
}

void CLED_DisplayInspectDlg::ClickMainBeforeRsltView()
{
	// TODO: 여기에 메시지 처리기 코드를 추가합니다.
	WindowVisible(NG_WINDOW);
	m_bBeforeView = TRUE;
	m_dlgNgView->SetListIdxInit();
	SetLoadRsltPara(0);
	m_dlgNgView->ShowWindow(SW_SHOW);
}

void CLED_DisplayInspectDlg::SetBright(int nBright, int x, int y)
{
	CString strTemp;

	strTemp.Format(_T("X : %d, Y : %d, BRIGHT : %d"), x, y, nBright);
	m_btn_Bright.SetWindowTextA(strTemp);
}

CPoint CLED_DisplayInspectDlg::GetDeviceIdx(int nIdx)
{
	return m_poDeviceIdx[nIdx];
}

int CLED_DisplayInspectDlg::GetGrabIdx(int nIdx)
{
	return m_nGrabIdx[nIdx];
}

int CLED_DisplayInspectDlg::GetInspGrabIdx(int nIdx)
{
	return m_nInspGrabIdx[nIdx];
}

BOOL CLED_DisplayInspectDlg::GetInspectionStart()
{
	return m_Rslt.m_bInspectionStart;
}

UINT CLED_DisplayInspectDlg::InspThread(LPVOID pParam)
{
	CLED_DisplayInspectDlg *pdlg = (CLED_DisplayInspectDlg *)pParam;

	while(TRUE)
	{
		if(pdlg->m_bInspThreadStart)
		{
			pdlg->m_bInspThreadStart = FALSE;
			pdlg->InspThread();
		}

		if(pdlg->m_bInspThreadEnd)
		{
			break;
		}
		Sleep(1);
	}

	return 0;
}

void CLED_DisplayInspectDlg::InspThread()
{
	int insp_rslt = 0, max_grab_cnt;
	double p_time, ins_time_s, ins_time_e;;
	CString strLog;
	BOOL grab_comp;
	BOOL ins_rslt[3];
	ins_rslt[0] = FALSE;
	ins_rslt[1] = FALSE;
	ins_rslt[2] = FALSE;

	SYSTEMTIME	lpSystemTime;
	GetLocalTime(&lpSystemTime);

	m_Rslt.m_bInspectionComp = FALSE;
	m_Rslt.m_bGrabComp = m_Rslt.m_bInspComp;
	grab_comp = m_Rslt.m_bGrabComp;
	InitInsRsltPara(grab_comp);

#ifdef TEST
	m_bInspectionImageSave = TRUE;
#endif

	for(int j = 0; j < m_dlgTeach->GetTeachPara().TeachOneGrabInspCnt; j++)
	{
		//SetSelectGrid(1, m_Rslt.m_nDeviceIdxX, (m_Rslt.m_nDeviceIdxY * m_dlgTeach->GetTeachPara().TeachOneGrabInspCnt) + j, m_Rslt.m_nGrabIdx);
		SetSelectGrid(0, m_poDeviceIdx[grab_comp].x, (m_poDeviceIdx[grab_comp].y) + j, m_nGrabIdx[grab_comp]);
		m_Rslt.m_bAlignInsp[j] = FALSE;
	}

	max_grab_cnt = m_dlgTeach->GetTeachPara().TeachInspGrabCnt;
	if(!m_dlgTeach->GetEdgeInsp())
	{
		//max_grab_cnt = 1;
	}
	//max_grab_cnt = 1;
	for(int i = 0; i < max_grab_cnt; i++)
	{
		MbufCopy(*m_MilManage.GetMilInspBuf(grab_comp, i), *m_MilManage.GetMilDispBuf(MAIN_DISP));
		m_Rslt.m_bInspectionRslt[0][i] = FALSE;
		m_Rslt.m_bInspectionRslt[1][i] = FALSE;
		insp_rslt = INSPECTION_OK;
		if(!m_bInspThreadTest)  
		{
			m_dGrabInspStartTime = clock();

			if(m_dlgTeach->GetEdgeInsp())
			{
				switch(i)
				{
				case 0:
					ins_time_s = clock();
					insp_rslt = Inspection(m_poDeviceIdx[grab_comp].x, m_poDeviceIdx[grab_comp].y, m_nGrabIdx[grab_comp], i, INSPECTION_ALIGN, grab_comp);
					//insp_rslt = Inspection(m_poDeviceIdx[grab_comp].x, m_poDeviceIdx[grab_comp].y, m_nGrabIdx[grab_comp], i, INSPECTION_EDGE, grab_comp);
					insp_rslt = Inspection(m_poDeviceIdx[grab_comp].x, m_poDeviceIdx[grab_comp].y, m_nGrabIdx[grab_comp], i, INSPECTION_TEST, grab_comp);
					break;
				case 1:
					insp_rslt = Inspection(m_poDeviceIdx[grab_comp].x, m_poDeviceIdx[grab_comp].y, m_nGrabIdx[grab_comp], i, INSPECTION_MASK_SET, grab_comp);
#ifdef INSPECTION_TEST
					insp_rslt = Inspection(m_poDeviceIdx[grab_comp].x, m_poDeviceIdx[grab_comp].y, m_nGrabIdx[grab_comp], i, INSPECTION_EDGE3, grab_comp);
#endif
					break;
				case 2:
					insp_rslt = Inspection(m_poDeviceIdx[grab_comp].x, m_poDeviceIdx[grab_comp].y, m_nGrabIdx[grab_comp], i, INSPECTION_EDGE2, grab_comp);
					insp_rslt = 1;
					ins_time_e = clock();
					p_time = (ins_time_e - ins_time_s) / 1000.0;
					strLog.Format(_T("%.3f sec"), p_time);
					m_btn_InspTime.SetWindowTextA(strLog);
					break;
				}
			}
			else
			{
				switch(i)
				{
				case 0:
					ins_time_s = clock();
					insp_rslt = Inspection(m_poDeviceIdx[grab_comp].x, m_poDeviceIdx[grab_comp].y, m_nGrabIdx[grab_comp], i, INSPECTION_ALIGN, grab_comp);
//#ifndef BLOB_TEST
					//if(!m_dlgTeach->GetTeachPara().TeachNewInsp)
					{
						//insp_rslt = Inspection(m_poDeviceIdx[grab_comp].x, m_poDeviceIdx[grab_comp].y, m_nGrabIdx[grab_comp], i, INSPECTION_BLOB, grab_comp);
					}
					//else
					{
						insp_rslt = Inspection(m_poDeviceIdx[grab_comp].x, m_poDeviceIdx[grab_comp].y, m_nGrabIdx[grab_comp], i, INSPECTION_BLOB3, grab_comp);
						//insp_rslt = Inspection(m_poDeviceIdx[grab_comp].x, m_poDeviceIdx[grab_comp].y, m_nGrabIdx[grab_comp], i, INSPECTION_BLOB2, grab_comp);
					}
//#endif
					//insp_rslt = Inspection(m_poDeviceIdx[grab_comp].x, m_poDeviceIdx[grab_comp].y, m_nGrabIdx[grab_comp], i + 1, INSPECTION_BLOB2, grab_comp);
					//ins_time_e = clock();
					//p_time = (ins_time_e - ins_time_s) / 1000.0;
					//strLog.Format(_T("%.3f sec"), p_time);
					//m_btn_InspTime.SetWindowTextA(strLog);
					break;
				case 1:
					//ins_time_s = clock();
					insp_rslt = Inspection(m_poDeviceIdx[grab_comp].x, m_poDeviceIdx[grab_comp].y, m_nGrabIdx[grab_comp], i, INSPECTION_BLOB2, grab_comp);
					ins_time_e = clock();
					p_time = (ins_time_e - ins_time_s) / 1000.0;
					strLog.Format(_T("%.3f sec"), p_time);
					m_btn_InspTime.SetWindowTextA(strLog);
					break;
				}
			}

			m_dGrabInspEndTime = clock();
			p_time = (m_dGrabInspEndTime - m_dGrabInspStartTime) / 1000.0;
			strLog.Format(_T("Device Idx X : %d, Device Idx Y : %d, Grab Idx : %d, Insp Grab Idx %d, Insp Time %.3f"), m_poDeviceIdx[grab_comp].x, m_poDeviceIdx[grab_comp].y, m_nGrabIdx[grab_comp], i, p_time);
			WriteLog(strLog, TYPE_LOG_MAIN, FALSE);

			strLog.Format(_T("Device Idx X : %d, Device Idx Y : %d, Grab Idx : %d, Insp Grab Idx %d, Grab & Inspection End"), m_poDeviceIdx[grab_comp].x, m_poDeviceIdx[grab_comp].y, m_nGrabIdx[grab_comp], i);
			WriteLog(strLog, TYPE_LOG_MAIN, FALSE);
		}
		else
		{
			Sleep(200);
			int startNum = 0;
			int endNum = 5;
			int randomNum;

			for(int j = 0; j < m_dlgTeach->GetTeachPara().TeachOneGrabInspCnt; j++)
			{
				randomNum = (rand() % (endNum-startNum+1)) + startNum;
				switch(randomNum)
				{
				case 0:
					m_Rslt.m_bAlignInsp[j] = FALSE;
					break;
				case 1:
					m_Rslt.m_bInspectionRslt[0][j] = TRUE;
					break;
				case 2:
					m_Rslt.m_bInspectionRslt[1][j] = TRUE;
				default:
					m_Rslt.m_bAlignInsp[j] = TRUE;
					m_Rslt.m_bInspectionRslt[0][j] = FALSE;
					m_Rslt.m_bInspectionRslt[1][j] = FALSE;
					break;
				}
			}
		}

#ifdef CAMERA
		//MbufCopy(*m_MilManage.GetMilInspBuf(grab_comp, i), *m_MilManage.GetMilDispBuf(MAIN_DISP));
#endif
		int nMaxInspGrabCnt = max_grab_cnt - 1;
		for(int j = 0; j < m_dlgTeach->GetTeachPara().TeachOneGrabInspCnt; j++)
		{
			InspectionImageSave(m_poDeviceIdx[grab_comp].x, m_poDeviceIdx[grab_comp].y + j, m_nGrabIdx[grab_comp], i, 0, grab_comp);
		}

		if(insp_rslt != INSPECTION_OK)
		{
			for(int j = 0; j < m_dlgTeach->GetTeachPara().TeachOneGrabInspCnt; j++)
			{
				if(!m_Rslt.m_bAlignInsp[j])
				{
					m_Rslt.m_bInspectionRslt[0][j] = TRUE;
					m_Rslt.m_bInspectionRslt[1][j] = TRUE;
				}

				if(m_Rslt.m_bInspectionRslt[0][j] != 0)
				{
					m_Rslt.m_bInspRslt[0][j] = INSPECTION_NG;
					m_Rslt.m_bInspectionRslt[1][j] = TRUE;
				}
				else if(m_Rslt.m_bInspectionRslt[1][j] != 0)
				{
					m_Rslt.m_bInspRslt[0][j] = INSPECTION_NG;
				}
				else if((m_Rslt.m_bInspectionRslt[0][j] == 0) && (m_Rslt.m_bInspectionRslt[1][j] == 0))
				{
					m_Rslt.m_bInspRslt[0][j] = INSPECTION_OK;
				}

				InspectionImageSave(m_poDeviceIdx[grab_comp].x, m_poDeviceIdx[grab_comp].y + j, m_nGrabIdx[grab_comp], i, m_Rslt.m_bInspRslt[0][j], grab_comp);

				if(m_Rslt.m_bInspRslt[0][j])
				{
					ins_rslt[j] = INSPECTION_NG;
				}
			}
		}

		if(i == nMaxInspGrabCnt)
		{
			for(int j = 0; j < m_dlgTeach->GetTeachPara().TeachOneGrabInspCnt; j++)
			{
				if((m_poDeviceIdx[grab_comp].y + j) == m_dlgTeach->GetTeachPara().TeachArrayCntY)
				{
					GridDrow(5, m_poDeviceIdx[grab_comp].x, (m_poDeviceIdx[grab_comp].y) + j, m_nGrabIdx[grab_comp]);
				}
				else
				{
					GridDrow(!ins_rslt[j], m_poDeviceIdx[grab_comp].x, (m_poDeviceIdx[grab_comp].y) + j, m_nGrabIdx[grab_comp]);
				}
				if((m_poDeviceIdx[grab_comp].y) + j == m_dlgTeach->GetTeachPara().TeachArrayCntY)
				{
					continue;
				}

				if(ins_rslt[j])
				{
					m_bInspOKNG[j][m_nGrabIdx[grab_comp]] = FALSE;
				}
				else
				{
					m_bInspOKNG[j][m_nGrabIdx[grab_comp]] = TRUE;;
				}

				m_Rslt.m_bInspectionRslt[0][j] = FALSE;
				m_Rslt.m_bInspectionRslt[1][j] = FALSE;
			}
			m_nInspGrabIdx[grab_comp] = i;
		}
	}
	SetRsltPara(m_InsRsltPara[grab_comp]);
	//20161017 ngh
	NGDisplay(m_poDeviceIdx[grab_comp].x, m_poDeviceIdx[grab_comp].y, m_nGrabIdx[grab_comp], 0, MAIN_DISP, m_InsRsltPara[grab_comp]);
	int cnt = 0;;

	if(m_nGrabIdx[grab_comp] == (m_dlgTeach->GetTeachPara().TeachGrabCnt - 1))
	{
		if(m_poDeviceIdx[grab_comp].y == m_dlgTeach->GetTeachPara().TeachArrayCntY - 1)
		{
			cnt = m_dlgTeach->GetTeachPara().TeachArrayCntY % m_dlgTeach->GetTeachPara().TeachOneGrabInspCnt;
		}

		for(int j = 0; j < m_dlgTeach->GetTeachPara().TeachOneGrabInspCnt - cnt; j++)
		{
			BOOL isNG = FALSE;
			for(int i = 0; i < m_dlgTeach->GetTeachPara().TeachGrabCnt; i++)
			{
				if(!m_bInspOKNG[j][i])
				{
					isNG = TRUE;
					m_RsltCnt.nNgCnt++;				
					m_nCapaCnt++;
					m_nAllCapaCnt++;
					break;
				}
			}
			if(!isNG)
			{
				m_RsltCnt.nOk1Cnt++;
			}

			m_nInspCnt++;

			m_RsltCnt.nTotalCnt++;

			ProductInfoView();
		}
	}

	m_Rslt.m_bInspectionComp = TRUE;
}

void CLED_DisplayInspectDlg::SetRsltPara(typeInsRsltPara rsltPara)
{
	int start_idx = m_RsltPara.nNgCnt;
	BOOL image_save = FALSE;

	for(int i = 0; i < rsltPara.nNgCnt; i++)
	{
		m_RsltPara.nDeviceIdx[start_idx + i] = rsltPara.nDeviceIdx[i];
		m_RsltPara.dArea[start_idx + i] = rsltPara.dArea[i];
		m_RsltPara.dCentX[start_idx + i] = rsltPara.dCentX[i];
		m_RsltPara.dCentY[start_idx + i] = rsltPara.dCentY[i];
		m_RsltPara.dHeight[start_idx + i] = rsltPara.dHeight[i];
		m_RsltPara.dWidth[start_idx + i] = rsltPara.dWidth[i];
		m_RsltPara.dLong[start_idx + i] = rsltPara.dLong[i];
		m_RsltPara.dShort[start_idx + i] = rsltPara.dShort[i];
		m_RsltPara.dBreadth[start_idx + i] = rsltPara.dBreadth[i];
		m_RsltPara.dAng[start_idx + i] = rsltPara.dAng[i];
		m_RsltPara.nGrabIdx[start_idx + i] = rsltPara.nGrabIdx[i];
		m_RsltPara.nInspGrabIdx[start_idx + i] = rsltPara.nInspGrabIdx[i];
		m_RsltPara.bAlign[start_idx + i] = rsltPara.bAlign[i];


		m_RsltPara.nNgCnt++;
	}
}

void CLED_DisplayInspectDlg::ScreenCapture(CString strPath)
{
	CRect rect;
	HDC h_screen_dc;
	m_hWnd = GetSafeHwnd();
	h_screen_dc = ::GetDC(m_hWnd);

	GetWindowRect(&rect);

	int width = 1920;
	int height = 1080;

	// DIB의 형식을 정의한다. 
	BITMAPINFO dib_define;
	dib_define.bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
	dib_define.bmiHeader.biWidth = width;
	dib_define.bmiHeader.biHeight = height;
	dib_define.bmiHeader.biPlanes = 1;
	dib_define.bmiHeader.biBitCount = 24;
	dib_define.bmiHeader.biCompression = BI_RGB;
	dib_define.bmiHeader.biSizeImage = (((width * 24 + 31) & ~31) >> 3) * height;
	dib_define.bmiHeader.biXPelsPerMeter = 0;
	dib_define.bmiHeader.biYPelsPerMeter = 0;
	dib_define.bmiHeader.biClrImportant = 0;
	dib_define.bmiHeader.biClrUsed = 0;

	// DIB의 내부 이미지 비트 패턴을 참조할 포인터 변수
	BYTE *p_image_data = NULL;
	// dib_define에 정의된 내용으로 DIB를 생성한다.
	HBITMAP h_bitmap = ::CreateDIBSection(h_screen_dc, &dib_define, DIB_RGB_COLORS, (void **)&p_image_data, 0, 0);
	// 이미지를 추출하기 위해서 가상 DC를 생성한다. 메인 DC에서는 직접적으로 비트맵에 접근하여
	// 이미지 패턴을 얻을 수 없기 때문이다.
	HDC h_memory_dc = ::CreateCompatibleDC(h_screen_dc);
	// 가상 DC에 이미지를 추출할 비트맵을 연결한다.
	HBITMAP h_old_bitmap = (HBITMAP)::SelectObject(h_memory_dc, h_bitmap);
	// 현재 스크린 화면을 캡쳐한다.
	::BitBlt(h_memory_dc, 0, 0, width, height, h_screen_dc, 0, 0, SRCCOPY);
	// 본래의 비트맵으로 복구한다.
	::SelectObject(h_memory_dc, h_old_bitmap); 
	// 가상 DC를 제거한다.
	DeleteDC(h_memory_dc);

	// DIB 파일의 내용을 구성한다.
	BITMAPFILEHEADER dib_format_layout;
	ZeroMemory(&dib_format_layout, sizeof(BITMAPFILEHEADER));
	dib_format_layout.bfType = *(WORD*)"BM";
	dib_format_layout.bfSize = sizeof(BITMAPFILEHEADER) + sizeof(BITMAPINFOHEADER) + dib_define.bmiHeader.biSizeImage;
	dib_format_layout.bfOffBits = sizeof(BITMAPFILEHEADER) + sizeof(BITMAPINFOHEADER);

	// DIB 파일을 생성한다.
	char file_name[512];
	sprintf(file_name, "%s", strPath);
	FILE *p_file = fopen(file_name, "wb");
	//AfxMessageBox(strPath);
	if(p_file != NULL){
		fwrite(&dib_format_layout, 1, sizeof(BITMAPFILEHEADER), p_file);
		fwrite(&dib_define, 1, sizeof(BITMAPINFOHEADER), p_file);
		fwrite(p_image_data, 1, dib_define.bmiHeader.biSizeImage, p_file);
		fclose(p_file);
	}

	if(NULL != h_bitmap) DeleteObject(h_bitmap);
	// if(NULL != h_screen_dc) ::ReleaseDC(NULL, h_screen_dc); 
	if(NULL != h_screen_dc) ::ReleaseDC(m_hWnd, h_screen_dc); 
	// 캡쳐를 하기 위해서 감춘 화면을 다시 보여준다.
	fclose(p_file);
	//ShowWindow(SW_SHOW);
}

void CLED_DisplayInspectDlg::ClickMainProductReset()
{
	// TODO: 여기에 메시지 처리기 코드를 추가합니다.
	initProductInfoCnt();
}


void CLED_DisplayInspectDlg::ClickMainMotionStop()
{
	// TODO: 여기에 메시지 처리기 코드를 추가합니다.
	CString strLog;
	strLog.Format(_T("Motion All Stop"));
	WriteLog(strLog, TYPE_LOG_MAIN);
	for(int i = 0; i < MAX_MOTION_CNT; i++)
	{
		m_dlgManual->SetMoveStop(i);
	}
	ClickMainOpStop();
}


BOOL CLED_DisplayInspectDlg::OnMouseWheel(UINT nFlags, short zDelta, CPoint pt)
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.
	CRect rect;
	if(m_nViewMode == AUTO_WINDOW || m_nViewMode == TEACH_WINDOW)
	{
		GetDlgItem(IDC_MAIN_FRM_DISP)->GetWindowRect(rect);

		if((rect.top + 60) < pt.y && pt.y < rect.bottom && rect.left < pt.x && pt.x < rect.right)
		{
			m_bMouseWheel = TRUE;
			m_MouseWheelPoint = pt;
			if(zDelta > 0)
			{
				//zoom in
				ClickBtnenhctrlZoomin();
			}
			else
			{
				//zoom out
				ClickBtnenhctrlZoomout();
			}
		}
		m_bMouseWheel = FALSE;
	}
	else if(m_nViewMode == NG_WINDOW)
	{

	}

	CRect rect2; //20161006 sc
	GetDlgItem(IDC_SCROLLBAR_NG_DISP)->GetWindowRect(rect2);
	typeInsRsltPara rsltPara;//20161017 ngh
	rsltPara = GetRsltPara(m_poDeviceIdx[m_Rslt.m_bGrabComp].x, m_poDeviceIdx[m_Rslt.m_bGrabComp].y, m_nGrabIdx[m_Rslt.m_bGrabComp], m_nInspGrabIdx[m_Rslt.m_bGrabComp]);

	if(rect2.right > pt.x && rect2.top < pt.y && rect2.bottom > pt.y)
	{
		if(zDelta > 0)
		{
			if(m_RsltIdx > 0)
			{
				m_RsltIdx--;
				NGDisplay(m_poDeviceIdx[m_Rslt.m_bGrabComp].x, m_poDeviceIdx[m_Rslt.m_bGrabComp].y, m_nGrabIdx[m_Rslt.m_bGrabComp], m_nInspGrabIdx[m_Rslt.m_bGrabComp], MAIN_DISP, m_RsltPara);
			}
		}
		else
		{
			if(rsltPara.nNgCnt > m_RsltIdx + 3)//20161017 ngh
			{
				m_RsltIdx++;
				NGDisplay(m_poDeviceIdx[m_Rslt.m_bGrabComp].x, m_poDeviceIdx[m_Rslt.m_bGrabComp].y, m_nGrabIdx[m_Rslt.m_bGrabComp], m_nInspGrabIdx[m_Rslt.m_bGrabComp], MAIN_DISP, m_RsltPara);
			}
		}
		m_scrol_NgDisp.SetScrollPos(m_RsltIdx);

		if((0 <= m_RsltIdx) && ((m_RsltIdx + 3) <= m_RsltPara.nNgCnt))
		{


		}
	}
	//20161006 sc
	return CDialogEx::OnMouseWheel(nFlags, zDelta, pt);
}

//void CLED_DisplayInspectDlg::MinusProductInfoCnt()
//{
/*int cnt[3];
for(int i = 0; i < 3; i++)
{
cnt[i] = 0;
}

for(int j = 0; j < m_dlgTeach->GetTeachPara().TeachOneGrabInspCnt; j++)
{
for(int i = 0; i < m_nInspCnt; i++)
{
if(m_nCapaCnt[j][i] == 0)
{
cnt[0]++;
}
else if(m_nCapaCnt[j][i] == 1)
{
cnt[1]++;
}
else
{
break;
}
}

cnt[2] = cnt[0] + cnt[1];

m_RsltCnt.nOk1Cnt-=cnt[0];
m_RsltCnt.nNgCnt-=cnt[1];
m_RsltCnt.nTotalCnt-=cnt[2];
}*/
//}

void CLED_DisplayInspectDlg::SetCapa(BOOL isNg, int nCnt)
{
	CString path;
	//path.Format(_T("%s"), m_SelectModel.cPath);
	path.Format(_T("%s\\%s\\%s\\"), CAPA_PATH, m_cStartDate[m_cStartDate.size() - 1], m_SelectModel.cName);

	int cnt[3];
	for(int i = 0; i < 3; i++)
	{
		cnt[i] = 0;
	}

	for(int j = 0; j < m_dlgTeach->GetTeachPara().TeachGrabCnt; j++)
	{
		CapaLoad(path, cnt);

		if(isNg)
		{
			cnt[0]+=nCnt;
			cnt[1]-=nCnt;
		}
		else
		{
			cnt[0]-=nCnt;
			cnt[1]+=nCnt;
		}

		cnt[2] = cnt[0] + cnt[1];

		CapaSave(path, cnt);
	}
}

void CLED_DisplayInspectDlg::SetCapa(CString path)
{
	int cnt[3];
	int temp_cnt[3];
	for(int i = 0; i < 3; i++)
	{
		cnt[i] = 0;
		temp_cnt[i] = 0;
	}

	for(int j = 0; j < m_dlgTeach->GetTeachPara().TeachGrabCnt; j++)
	{
		CapaLoad(path, temp_cnt);

		cnt[0] = m_nCapaCnt;
		cnt[1] = m_nInspCnt - cnt[0];
		cnt[2] = cnt[0] + cnt[1];

		cnt[0]+=temp_cnt[0];
		cnt[1]+=temp_cnt[1];
		cnt[2]+=temp_cnt[2];

		CapaSave(path, cnt);
	}
}

void CLED_DisplayInspectDlg::SetAllCapa(BOOL isNg, int nCnt)
{
	int cnt[3];
	for(int i = 0; i < 3; i++)
	{
		cnt[i] = 0;
	}

	for(int j = 0; j < m_dlgTeach->GetTeachPara().TeachGrabCnt; j++)
	{
		AllCapaLoad(cnt);

		if(isNg)
		{
			cnt[0]+=nCnt;
			cnt[1]-=nCnt;
		}
		else
		{
			cnt[0]-=nCnt;
			cnt[1]+=nCnt;
		}

		cnt[2] = cnt[0] + cnt[1];

		AllCapaSave(cnt);
	}
}

void CLED_DisplayInspectDlg::SetAllCapa()
{
	CString path, temp;
	int cnt[3];
	int temp_cnt[3];
	for(int i = 0; i < 3; i++)
	{
		cnt[i] = 0;
		temp_cnt[i] = 0;
	}

	for(int j = 0; j < m_dlgTeach->GetTeachPara().TeachGrabCnt; j++)
	{
		AllCapaLoad(temp_cnt);

		cnt[0] = m_nAllCapaCnt;
		cnt[1] = m_nInspCnt - cnt[0];
		cnt[2] = cnt[0] + cnt[1];

		cnt[0]+=temp_cnt[0];
		cnt[1]+=temp_cnt[1];
		cnt[2]+=temp_cnt[2];

		AllCapaSave(cnt);
	}
}

BOOL CLED_DisplayInspectDlg::CapaSave(CString path, int *cnt)
{
	BOOL rslt = TRUE;
	CString file_path, temp_path;
	SYSTEMTIME	lpSystemTime;
	CString key, dat;
	CString strLog;
	strLog.Format(_T("CapaSave Start!!"));
	WriteLog(strLog, TYPE_LOG_MAIN, FALSE);
	GetLocalTime(&lpSystemTime);

	//temp_path.Format(_T("%s\\CAPA\\%04d%02d%02d"), path, lpSystemTime.wYear, lpSystemTime.wMonth, lpSystemTime.wDay);
	_MakeDirectories(path);

	file_path.Format(_T("%s\\Capa.ini"), path);

	//file 존재 확인 
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
	}
	else
	{
		CloseHandle(fd);

		key.Empty();
		key.Format(_T("INSPECTION_OK_CNT"));
		dat.Empty();
		dat.Format(_T("%d"), cnt[0]);
		WritePrivateProfileString(_T("CAPA"), key, dat, file_path);	

		key.Empty();
		key.Format(_T("INSPECTION_NG_CNT"));
		dat.Empty();
		dat.Format(_T("%d"), cnt[1]);
		WritePrivateProfileString(_T("CAPA"), key, dat, file_path);	

		key.Empty();
		key.Format(_T("INSPECTION_TOTAL_CNT"));
		dat.Empty();
		dat.Format(_T("%d"), cnt[2]);
		WritePrivateProfileString(_T("CAPA"), key, dat, file_path);	
	}

	fd = 0;


	strLog.Format(_T("CapaSave End!!"));
	WriteLog(strLog, TYPE_LOG_MAIN, FALSE);
	return rslt;
}

BOOL CLED_DisplayInspectDlg::CapaLoad(CString path, int *cnt)
{
	BOOL rslt = TRUE;
	CString file_path, temp_path;
	CString key, dat;
	char temp[DAT_STR_LENG];
	SYSTEMTIME	lpSystemTime;

	CString strLog;
	strLog.Format(_T("CapaLoad Start!!"));
	WriteLog(strLog, TYPE_LOG_MAIN, FALSE);
	GetLocalTime(&lpSystemTime);

	//temp_path.Format(_T("%s\\CAPA\\%04d%02d%02d"), path, lpSystemTime.wYear, lpSystemTime.wMonth, lpSystemTime.wDay);
	file_path.Format(_T("%s\\Capa.ini"), path);


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

		key.Empty();
		key.Format(_T("INSPECTION_OK_CNT"));
		GetPrivateProfileString(_T("CAPA"), key, _T("0"), temp, sizeof(temp), file_path);
		dat.Format(_T("%s"), temp);
		cnt[0] = atoi(dat);

		key.Empty();
		key.Format(_T("INSPECTION_NG_CNT"));
		GetPrivateProfileString(_T("CAPA"), key, _T("0"), temp, sizeof(temp), file_path);
		dat.Format(_T("%s"), temp);
		cnt[1] = atoi(dat);

		key.Empty();
		key.Format(_T("INSPECTION_TOTAL_CNT"));
		GetPrivateProfileString(_T("CAPA"), key, _T("0"), temp, sizeof(temp), file_path);
		dat.Format(_T("%s"), temp);
		cnt[2] = atoi(dat);
	}

	fd = 0;

	strLog.Format(_T("CapaLoad End!!"));
	WriteLog(strLog, TYPE_LOG_MAIN, FALSE);

	return rslt;
}

BOOL CLED_DisplayInspectDlg::AllCapaSave(int *cnt)
{
	BOOL rslt = TRUE;
	CString file_path, temp_path;
	CString key, dat;
	SYSTEMTIME	lpSystemTime;

	CString strLog;
	strLog.Format(_T("AllCapaSave Start!!"));
	WriteLog(strLog, TYPE_LOG_MAIN, FALSE);

	GetLocalTime(&lpSystemTime);
	temp_path.Format(_T("%s\\%s\\"), CAPA_PATH, m_cStartDate[m_cStartDate.size() - 1]);
	//temp_path.Format(_T("%s\\%04d%02d%02d"), CAPA_PATH, lpSystemTime.wYear, lpSystemTime.wMonth, lpSystemTime.wDay);
	_MakeDirectories(temp_path);

	file_path.Format(_T("%s\\Capa.ini"), temp_path);

	//file 존재 확인 
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
	}
	else
	{
		CloseHandle(fd);

		key.Empty();
		key.Format(_T("INSPECTION_OK_CNT"));
		dat.Empty();
		dat.Format(_T("%d"), cnt[0]);
		WritePrivateProfileString(_T("CAPA"), key, dat, file_path);	

		key.Empty();
		key.Format(_T("INSPECTION_NG_CNT"));
		dat.Empty();
		dat.Format(_T("%d"), cnt[1]);
		WritePrivateProfileString(_T("CAPA"), key, dat, file_path);	

		key.Empty();
		key.Format(_T("INSPECTION_TOTAL_CNT"));
		dat.Empty();
		dat.Format(_T("%d"), cnt[2]);
		WritePrivateProfileString(_T("CAPA"), key, dat, file_path);	
	}

	fd = 0;

	strLog.Format(_T("AllCapaSave End!!"));
	WriteLog(strLog, TYPE_LOG_MAIN, FALSE);

	return rslt;
}

BOOL CLED_DisplayInspectDlg::AllCapaLoad(int *cnt)
{
	BOOL rslt = TRUE;
	CString file_path, temp_path;
	CString key, dat;
	char temp[DAT_STR_LENG];
	SYSTEMTIME	lpSystemTime;

	CString strLog;
	strLog.Format(_T("AllCapaLoad Start!!"));
	WriteLog(strLog, TYPE_LOG_MAIN, FALSE);

	GetLocalTime(&lpSystemTime);
	temp_path.Format(_T("%s\\%s\\"), CAPA_PATH, m_cStartDate[m_cStartDate.size() - 1]);
	file_path.Format(_T("%s\\Capa.ini"), temp_path);

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

		key.Empty();
		key.Format(_T("INSPECTION_OK_CNT"));
		GetPrivateProfileString(_T("CAPA"), key, _T("0"), temp, sizeof(temp), file_path);
		dat.Format(_T("%s"), temp);
		cnt[0] = atoi(dat);

		key.Empty();
		key.Format(_T("INSPECTION_NG_CNT"));
		GetPrivateProfileString(_T("CAPA"), key, _T("0"), temp, sizeof(temp), file_path);
		dat.Format(_T("%s"), temp);
		cnt[1] = atoi(dat);

		key.Empty();
		key.Format(_T("INSPECTION_TOTAL_CNT"));
		GetPrivateProfileString(_T("CAPA"), key, _T("0"), temp, sizeof(temp), file_path);
		dat.Format(_T("%s"), temp);
		cnt[2] = atoi(dat);
	}

	fd = 0;

	strLog.Format(_T("AllCapaLoad End!!"));
	WriteLog(strLog, TYPE_LOG_MAIN, FALSE);

	return rslt;
}

CString CLED_DisplayInspectDlg::GetStartTime(int idx)
{
	if(m_cStartTime.size() == 0)
	{
		return "";
	}
	return m_cStartTime[idx];
}

CString CLED_DisplayInspectDlg::GetStartDate(int idx)
{
	if(m_cStartDate.size() == 0)
	{
		return "";
	}
	return m_cStartDate[idx];
}

BOOL CLED_DisplayInspectDlg::DeleteRsltFilePath(int idx)
{
	if((m_cBeforeRsltFilePath.size() > 0) && (m_cStartDate.size() > 0) && (m_cStartTime.size() > 0))
	{
		m_cBeforeRsltFilePath.erase(m_cBeforeRsltFilePath.begin() + idx);
		m_cStartDate.erase(m_cStartDate.begin() + idx);
		m_cStartTime.erase(m_cStartTime.begin() + idx);
		m_nBeforeRsltFileCnt = m_cBeforeRsltFilePath.size();
		return TRUE;
	}

	return FALSE;
}

int CLED_DisplayInspectDlg::GetBeforeRsltCnt()
{
	return m_cBeforeRsltFilePath.size();//20161111 ngh
}

BOOL CLED_DisplayInspectDlg::SetLoadRsltPara(int idx)
{
	BOOL rslt = FALSE;

	InitBeforeRsltPara();
	NGDiaplayClear(NG_VIEW_DISP);
	MbufClear(*m_MilManage.GetMilDispBuf(NG_VIEW_DISP), 0x00);

	if(m_cBeforeRsltFilePath.size() > 0)
	{
		rslt = LoadRsltFile(m_cBeforeRsltFilePath[idx]);
		SetGridDbClick(m_BeforeRsltPara.nDeviceIdx[0].x + 1, m_BeforeRsltPara.nDeviceIdx[0].y + 1, m_BeforeRsltPara.nGrabIdx[0] + 1);
	}

	return rslt;
}

BOOL CLED_DisplayInspectDlg::LoadRsltFile(CString file_path)
{
	BOOL rslt = TRUE;
	char strFile[DAT_STR_LENG], buffer[DAT_STR_LENG], *ps;
	int line_cnt = 0, step = 0;
	FILE *stream;
	CString line_rslt, temp;

	sprintf(strFile, "%s", file_path);

	if((stream = _tfopen( strFile, "rb")) == NULL)
	{
		for(int i = 0; i < 3; i++)
		{
			stream = _tfopen( strFile, "rb" );
			if(stream != NULL)
			{
				break;
			}
			Sleep(500);
			if(i == 2)
			{
				return FALSE;
			}
		}
	}

	while (fgets(buffer, DAT_STR_LENG, stream) != NULL) // 텍스트파일을 한줄씩 읽습니다. 
	{
		ps = strchr(buffer, '\n'); // 제일 뒤쪽의 new line의 char을 찿아.
		if (ps != NULL) *ps = '\0';// new line의 char을 null문자로 바꿉니다. 

		if(line_cnt == 0)
		{
			InitBeforeRsltPara();
		}
		//data.Format(_T("Array_X,Array_Y,GrabIdx,InspIdx,Cent_X,Cent_Y,Width,Height,Logn,Short"));
		if(line_cnt != 0)
		{
			step = 0;
			line_rslt.Format(_T("%s"), buffer);

			AfxExtractSubString(temp, line_rslt, step++, ',');
			m_BeforeRsltPara.nDeviceIdx[line_cnt - 1].x = atoi(temp);
			AfxExtractSubString(temp, line_rslt, step++, ',');
			m_BeforeRsltPara.nDeviceIdx[line_cnt - 1].y = atoi(temp);
			AfxExtractSubString(temp, line_rslt, step++, ',');
			m_BeforeRsltPara.nGrabIdx[line_cnt - 1] = atoi(temp);
			AfxExtractSubString(temp, line_rslt, step++, ',');
			m_BeforeRsltPara.nInspGrabIdx[line_cnt - 1] = atoi(temp);
			AfxExtractSubString(temp, line_rslt, step++, ',');
			m_BeforeRsltPara.dCentX[line_cnt - 1] = atof(temp);
			AfxExtractSubString(temp, line_rslt, step++, ',');
			m_BeforeRsltPara.dCentY[line_cnt - 1] = atof(temp);
			AfxExtractSubString(temp, line_rslt, step++, ',');
			m_BeforeRsltPara.dWidth[line_cnt - 1] = atof(temp);
			AfxExtractSubString(temp, line_rslt, step++, ',');
			m_BeforeRsltPara.dHeight[line_cnt - 1] = atof(temp);
			AfxExtractSubString(temp, line_rslt, step++, ',');
			m_BeforeRsltPara.dLong[line_cnt - 1] = atof(temp);
			AfxExtractSubString(temp, line_rslt, step++, ',');
			m_BeforeRsltPara.dShort[line_cnt - 1] = atof(temp);
			AfxExtractSubString(temp, line_rslt, step++, ',');
			m_BeforeRsltPara.dBreadth[line_cnt - 1] = atof(temp);
			AfxExtractSubString(temp, line_rslt, step++, ',');
			m_BeforeRsltPara.dAng[line_cnt - 1] = atof(temp);
			AfxExtractSubString(temp, line_rslt, step++, ',');
			m_BeforeRsltPara.bAlign[line_cnt - 1] = atoi(temp);
			m_BeforeRsltPara.bNg[line_cnt - 1] = TRUE;
			m_BeforeRsltPara.nNgCnt++;
		}

		line_cnt++;
	}

	fclose(stream);

	return rslt;
}

BOOL CLED_DisplayInspectDlg::SetRsltFile()
{
	BOOL rslt = TRUE;;
	CString file_path, data;
	data.Format(_T(""));
	file_path.Format(_T("%s\\%s\\%s\\"), CAPA_PATH, m_cStartDate[m_cStartDate.size() - 1], m_SelectModel.cName);
	::_MakeDirectories(file_path);
	file_path.Format(_T("%s\\%s\\%s\\InspRslt_%s.csv"), CAPA_PATH, m_cStartDate[m_cStartDate.size() - 1], m_SelectModel.cName, m_cStartTime[m_cStartTime.size() - 1]);
	m_cBeforeRsltFilePath.push_back(file_path);

	//if(m_RsltPara.nNgCnt == 0)
	{
		//if((m_cBeforeRsltFilePath.size() > 0) && (m_cStartDate.size() > 0) && (m_cStartTime.size() > 0))
		{
			//m_cBeforeRsltFilePath.erase(m_cBeforeRsltFilePath.begin());
			//m_cStartDate.erase(m_cStartDate.begin());
			//m_cStartTime.erase(m_cStartTime.begin());
			//return TRUE;
		}
	}

	if(m_cBeforeRsltFilePath.size() == MAX_BEFORE_RSLT_CNT + 1)
	{
		m_cBeforeRsltFilePath.erase(m_cBeforeRsltFilePath.begin());
		m_cStartDate.erase(m_cStartDate.begin());
		m_cStartTime.erase(m_cStartTime.begin());
	}

	m_nBeforeRsltFileCnt = m_cBeforeRsltFilePath.size();

	data.Format(_T("Array_X,Array_Y,GrabIdx,InspIdx,Cent_X,Cent_Y,Width,Height,Logn,Short"));
	rslt = SaveRsltFile(file_path, data);
	if(!rslt)
	{
		return FALSE;
	}
	for(int i = 0; i < m_RsltPara.nNgCnt; i++)
	{
		data.Format(_T("%d,%d,%d,%d,%f,%f,%f,%f,%f,%f,%f,%f,%d,%d"), m_RsltPara.nDeviceIdx[i].x, m_RsltPara.nDeviceIdx[i].y, m_RsltPara.nGrabIdx[i], m_RsltPara.nInspGrabIdx[i],
			m_RsltPara.dCentX[i], m_RsltPara.dCentY[i], m_RsltPara.dWidth[i], m_RsltPara.dHeight[i], m_RsltPara.dLong[i], m_RsltPara.dShort[i], m_RsltPara.dBreadth[i], m_RsltPara.dAng[i],
			m_RsltPara.bAlign[i],m_dlgManual->GetEZCurPos(2));
		rslt = SaveRsltFile(file_path, data);
		if(!rslt)
		{
			break;
		}
	}

	return rslt;
}

BOOL CLED_DisplayInspectDlg::SaveRsltFile(CString file_path, CString data)
{
	FILE*	stream;
	char	strFile[DAT_STR_LENG], strToFile[MAX_PATH];
	long	result, line;
	char	szTime[DAT_STR_LENG]; 
	CString bak_file, path, filePath;
	char buffer[DAT_STR_LENG];
	CTime time;
	time = CTime::GetCurrentTime();
	int limit_line = 0;

	path.Empty();	

	//MakeDirectories(path);
	sprintf(strFile, "%s", file_path);

	if((stream = _tfopen(strFile, "a+")) == NULL)
	{
		HANDLE fd = CreateFile( strFile,
			GENERIC_READ|GENERIC_WRITE,
			//GENERIC_WRITE,
			FILE_SHARE_READ|FILE_SHARE_WRITE,
			NULL,
			OPEN_ALWAYS,
			FILE_ATTRIBUTE_NORMAL,
			NULL
			);

		stream = _tfopen(strFile, "a+");
		if(stream == NULL)
		{
			CloseHandle(fd);
			return FALSE;
		}

		CloseHandle(fd);
	}

	//Check first time
	result = fseek(stream, 0L, SEEK_SET);
	if(result)
	{
		fclose(stream);
		return FALSE;
	}
	line = 0;
	while(fgets(buffer, 999999, stream)) 
	{
		line++;
		if(1 < line)
		{
			break;
		}
	}

	result = fseek(stream, 0L, SEEK_END);
	if(result)
	{
		fclose(stream);
		return FALSE;
	}

	memset(szTime, NULL, sizeof(szTime));
	sprintf_s(szTime, data.GetLength() + 1, "%s", data);
	fputs(szTime, stream);
	fputs("\n", stream);

	result = fseek(stream, 0L, SEEK_SET);
	if(result)
	{
		fclose(stream);
		return FALSE;
	}

	//Check limit line
	line = 0;
	if(limit_line)
	{
		while(fgets(buffer, 999999, stream)) line++;
		if(line > limit_line)
		{
			sprintf_s(strToFile, filePath.GetLength() + bak_file.GetLength() + 2 + 1, "%s\\%s", path, bak_file);
			CopyFile((LPCSTR)strFile, (LPCSTR)strToFile, FALSE);
			fclose(stream);
			DeleteFile((LPCSTR)strFile);
		}
		else
		{
			fclose(stream);
		}
	}
	else
	{
		fclose(stream);
	}

	return TRUE;
}

CString CLED_DisplayInspectDlg::GetBeforeRsltFilePath(int idx)
{
	if(m_cBeforeRsltFilePath.size() == 0)
	{
		return _T("");
	}
	return m_cBeforeRsltFilePath[idx];
}

void CLED_DisplayInspectDlg::SetMainState(LPCTSTR text)
{
	m_btn_main_op_state.SetCaption(text);  //20161006 sc
}

BOOL CLED_DisplayInspectDlg::PreTranslateMessage(MSG* pMsg)
{
	// TODO: 여기에 특수화된 코드를 추가 및/또는 기본 클래스를 호출합니다.
	if(pMsg->message ==WM_KEYDOWN)
	{
		if(pMsg->wParam == VK_RETURN || pMsg->wParam == VK_ESCAPE)
			return TRUE;
	}

	return CDialogEx::PreTranslateMessage(pMsg);
}

//20161011 ht 메인다이얼로그 언어 설정
void CLED_DisplayInspectDlg::ChangeLanguage(BOOL StateLanguage)
{
	switch(StateLanguage)
	{
	case 0:
		GetDlgItem(IDC_SUB_MENU)->SetWindowText(_T("MAIN STATE"));
		GetDlgItem(IDC_LABLE_EQP_ID)->SetWindowText(_T("EQP ID"));
		GetDlgItem(IDC_LABLE_OP_STATE)->SetWindowText(_T("MODE"));
		GetDlgItem(IDC_LABLE_OP_STATE2)->SetWindowText(_T("STATE"));

		GetDlgItem(IDC_OP_MENU2)->SetWindowText(_T("PRODUCT INFORMATION"));
		GetDlgItem(IDC_LABLE_PRODUCT_TOTAL)->SetWindowText(_T("TOTAL"));
		GetDlgItem(IDC_LABLE_PRODUCT_PASS)->SetWindowText(_T("PASS"));
		GetDlgItem(IDC_LABLE_PRODUCT_FAIL)->SetWindowText(_T("FAIL"));
		GetDlgItem(IDC_LABLE_PRODUCT_FAIL_RATIO)->SetWindowText(_T("FAIL RATIO"));
		GetDlgItem(IDB_MAIN_PRODUCT_RESET)->SetWindowText(_T("RESET"));

		GetDlgItem(IDC_OP_MENU)->SetWindowText(_T("OP MENU"));		
		GetDlgItem(IDB_MAIN_OP_INIT)->SetWindowText(_T("UNIT INIT"));
		GetDlgItem(IDB_MAIN_MOTION_STOP)->SetWindowText(_T("Motion All Stop"));
		GetDlgItem(IDB_MAIN_BEFORE_RSLT_VIEW)->SetWindowText(_T("Before Rslt View"));
		GetDlgItem(IDC_LABLE_PRODUCT_PASS2)->SetWindowText(_T("Grab Time"));
		GetDlgItem(IDC_LABLE_PRODUCT_FAIL2)->SetWindowText(_T("Insp Time"));
		GetDlgItem(IDB_MAIN_INSPECTION_RESET)->SetWindowText(_T("Inspection Reset"));
		//GetDlgItem(IDB_MAIN_OP_START)->SetWindowText(_T("START"));
		//GetDlgItem(IDB_MAIN_OP_STOP)->SetWindowText(_T("STOP"));
		//GetDlgItem(IDC_EXITVIEW_BTN)->SetWindowText(_T("EXIT"));
		break;
	case 1:
		GetDlgItem(IDC_SUB_MENU)->SetWindowText(_T("MAIN STATE"));
		GetDlgItem(IDC_LABLE_EQP_ID)->SetWindowText(_T("설비 ID"));
		GetDlgItem(IDC_LABLE_OP_STATE)->SetWindowText(_T("운전 모드"));
		GetDlgItem(IDC_LABLE_OP_STATE2)->SetWindowText(_T("설비 상태"));

		GetDlgItem(IDC_OP_MENU2)->SetWindowText(_T("PRODUCT INFORMATION"));
		GetDlgItem(IDC_LABLE_PRODUCT_TOTAL)->SetWindowText(_T("총수량"));
		GetDlgItem(IDC_LABLE_PRODUCT_PASS)->SetWindowText(_T("양품"));
		GetDlgItem(IDC_LABLE_PRODUCT_FAIL)->SetWindowText(_T("불량"));
		GetDlgItem(IDC_LABLE_PRODUCT_FAIL_RATIO)->SetWindowText(_T("불량률"));
		GetDlgItem(IDB_MAIN_PRODUCT_RESET)->SetWindowText(_T("초기화"));

		GetDlgItem(IDC_OP_MENU)->SetWindowText(_T("OP MENU"));		
		GetDlgItem(IDB_MAIN_OP_INIT)->SetWindowText(_T("모터 초기화"));
		GetDlgItem(IDB_MAIN_MOTION_STOP)->SetWindowText(_T("전체 정지"));
		GetDlgItem(IDB_MAIN_BEFORE_RSLT_VIEW)->SetWindowText(_T("이전 결과 보기"));
		GetDlgItem(IDC_LABLE_PRODUCT_PASS2)->SetWindowText(_T("촬상 시간"));
		GetDlgItem(IDC_LABLE_PRODUCT_FAIL2)->SetWindowText(_T("검사 시간"));
		GetDlgItem(IDB_MAIN_INSPECTION_RESET)->SetWindowText(_T("검사 초기화"));
		//GetDlgItem(IDB_MAIN_OP_START)->SetWindowText(_T("시작"));
		//GetDlgItem(IDB_MAIN_OP_STOP)->SetWindowText(_T("정지"));
		//GetDlgItem(IDC_EXITVIEW_BTN)->SetWindowText(_T("종료"));
		break;
	}

	m_dlgNgView->ChangeLanguage(StateLanguage);
	m_dlgTeach->ChangeLanguage(StateLanguage);
	m_dlgManual->ChangeLanguage(StateLanguage);
	m_dlgData->ChangeLanguage(StateLanguage);	

}

void CLED_DisplayInspectDlg::OnBnClickedButtonImgTest()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	CString strLog, tmpPath_Name;
	MIL_ID image1, image2, image3, image4, image5;
	int i = 0;

	MbufAlloc2d(*m_MilManage.GetMilSystem(), CAMERA_WIDTH, CAMERA_HEIGHT, 8+M_UNSIGNED, M_IMAGE+M_PROC, &image1);
	MbufAlloc2d(*m_MilManage.GetMilSystem(), CAMERA_WIDTH, CAMERA_HEIGHT, 8+M_UNSIGNED, M_IMAGE+M_PROC, &image2);
	MbufAlloc2d(*m_MilManage.GetMilSystem(), CAMERA_WIDTH, CAMERA_HEIGHT, 8+M_UNSIGNED, M_IMAGE+M_PROC, &image3);
	MbufAlloc2d(*m_MilManage.GetMilSystem(), CAMERA_WIDTH, CAMERA_HEIGHT, 8+M_UNSIGNED, M_IMAGE+M_PROC, &image4);
	MbufAlloc2d(*m_MilManage.GetMilSystem(), CAMERA_WIDTH, CAMERA_HEIGHT, 8+M_UNSIGNED, M_IMAGE+M_PROC, &image5);

	strLog.Format(_T("D:\\DGT_TEST_IMAGE\\1_1.bmp"));
	MbufImport((char *)(LPCTSTR)strLog, M_BMP, M_LOAD, *m_MilManage.GetMilSystem(), &image1);
	strLog.Format(_T("D:\\DGT_TEST_IMAGE\\1_2.bmp"));
	MbufImport((char *)(LPCTSTR)strLog, M_BMP, M_LOAD, *m_MilManage.GetMilSystem(), &image2);
	
	MimConvolve(image1, image1, M_SHEN_FILTER(M_SHARPEN, 30));

	MblobReconstruct(image2, image1, image3, M_RECONSTRUCT_FROM_SEED, M_DEFAULT);


	strLog.Format(_T("D:\\DGT_TEST_IMAGE\\TEST123_0.bmp"));
	MbufExport((char *)(LPCTSTR)strLog, M_BMP, image1);

	MbufFree(image1);
	MbufFree(image2);
	MbufFree(image3);
	MbufFree(image4);
	MbufFree(image5);
}

//20161201 ngh
void CLED_DisplayInspectDlg::DrowArrow(int nDisplayType, CPoint poStart, int nArrowLength, int nArrowBreadth)
{
	CPoint poEnd;
	CPoint poArrowLeft, poArrowRight;
	int nBreadth1 = 0, nBreadth2 = 0;

	poEnd.x = poStart.x + nArrowLength;
	poEnd.y = poStart.y - nArrowLength;

	poArrowLeft.x = poStart.x;
	poArrowLeft.y = poStart.y - nArrowBreadth;

	poArrowRight.x = poStart.x + nArrowBreadth;
	poArrowRight.y = poStart.y;

	for(int i = 0; i < nArrowBreadth; i++)
	{
		if(i == 0)
		{
			MgraLine(M_DEFAULT, *m_MilManage.GetMilOverlay(nDisplayType), poStart.x, poStart.y, poEnd.x, poEnd.y);
		}
		else if((i % 2) == 0)
		{
			MgraLine(M_DEFAULT, *m_MilManage.GetMilOverlay(nDisplayType), poStart.x, poStart.y - nBreadth2, poEnd.x, poEnd.y - nBreadth2);
			nBreadth2++;
		}
		else
		{
			MgraLine(M_DEFAULT, *m_MilManage.GetMilOverlay(nDisplayType), poStart.x + nBreadth1, poStart.y, poEnd.x + nBreadth1, poEnd.y);
			nBreadth1++;
		}
			
	}

	for(int i = 0; i < (nArrowBreadth / 2); i++)
	{
		MgraLine(M_DEFAULT, *m_MilManage.GetMilOverlay(nDisplayType), poStart.x + i, poStart.y - i, poArrowLeft.x + i, poArrowLeft.y - i);
		MgraLine(M_DEFAULT, *m_MilManage.GetMilOverlay(nDisplayType), poStart.x + i, poStart.y - i, poArrowRight.x + i, poArrowRight.y - i);
	}
}

void CLED_DisplayInspectDlg::OnBnClickedButtonRegrid()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	m_grid.RedrawAll();
	SetGrid();
}
