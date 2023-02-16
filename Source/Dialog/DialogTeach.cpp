// \source\dialog\DialogTeach.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "LED_DisplayInspect.h"
#include "LED_DisplayInspectDlg.h"
#include "DialogTeach.h"
#include "afxdialogex.h"


// DialogTeach 대화 상자입니다.

IMPLEMENT_DYNAMIC(CDialogTeach, CDialogEx)

CDialogTeach::CDialogTeach(CWnd* pParent /*=NULL*/)
	: CDialogEx(CDialogTeach::IDD, pParent)
	, m_edit_nInspRegionCnt(0)
	, m_edit_nLight_0(0)
	, m_edit_nLight_1(0)
	, m_edit_nImageGrabCnt(0)
	, m_edit_nSelectImageIdx(1)
	, m_edit_nMoveCntX(1)
	, m_edit_nMoveCntY(1)
	, m_edit_nDeviceCntX(0)
	, m_edit_nDeviceCntY(0)
	, m_edit_dGrabDistanceX(0)
	, m_edit_dGrabDistanceY(0)
	, m_edit_dFirstDevicePosX(0)
	, m_edit_dFirstDevicePosY(0)
	, m_edit_dFirstDevicePosZ(0)
	, m_edit_nDeviceDistanceX(0)
	, m_edit_nDeviceDistanceY(0)
	, m_edit_dWaitPosX(0)
	, m_edit_dWaitPosY(0)
	, m_edit_dWaitPosZ(0)
	, m_edit_nThreshHold1(0)
	, m_edit_nInspThreshHold3(0)
	, m_edit_nSpec(0)
	, m_edit_nPixelSize(0)
	, m_edit_nInspThreshHold1(0)
	, m_edit_nInspThreshHold2(0)
	, m_edit_nInspGrabIdx(0)
	, m_edit_nSelectInspGrabIdx(1)
	, m_edit_nOneGrabInspCnt(1)
	, m_edit_dCircleArea(0)
	, m_edit_dCircleErodeDistance(0)
	, m_edit_nAlignScore(0)
	, m_edit_nAlignSize(0)
	, m_edit_nSecondInspThreshHold(0)
	, m_edit_nShortSpec(25)
{

	m_bParaLock = TRUE;
	memset(&m_TeachPara, NULL, sizeof(typeTeachPara));

	memset(&m_TeachMovePos, NULL, sizeof(typedefMovePosPara));

	
	m_TeachPara.TeachGrabCnt = 1;
	m_TeachPara.TeachDeviceCntX = 1;
	m_TeachPara.TeachDeviceCntY = 1;
	m_TeachPara.TeachArrayCntX = 1;
	m_TeachPara.TeachArrayCntY = 1;
	m_TeachPara.TeachOneGrabInspCnt = 1;
	m_TeachPara.TeachInspGrabCnt = 1;
	
	m_TeachPara.TeachDeviceMotionDistanceX = 0;
	m_TeachPara.TeachDeviceMotionDistanceY = 0;
	m_TeachPara.TeachGrabMotionDistanceX = 0;
	m_TeachPara.TeachGrabMotionDistanceY = 0;

	m_TeachPara.TeachInspType = 0;

	m_iBeforeidxX = 0;
	m_iBeforeidxY = 0;
	m_iBeforeidxX1 = 0;
	m_iBeforeidxY1 = 0;
	
	for(int i = 0; i < MAX_GRAB_CNT; i++)
	{
		for(int j = 0; j < MAX_INSP_GRAB_CNT; j++)
		{
			m_TeachPara.TeachWaitPos[j] = 0;
			m_TeachPara.TeachFirstDevicePos[j] = 0;
			m_TeachPara.TeachCircleArea[j] = 100;
			m_TeachPara.TeachCircleErodeDistance[j] = 2;
			m_TeachPara.TeachRegionCnt[i][j] = 1;
			
			for(int k = 0; k < MAX_INS_PARA; k++)
			{
				m_TeachPara.TeachRegion[i][j][k].SetdRectLTRB(100., 100., 200., 200.);
			}
		}
	}

	m_nBeforeX = 0;
	m_nBeforeY = 0;
	m_nBdforeImgIdx = 0;
}

CDialogTeach::~CDialogTeach()
{
	for(int i = 0; i < MAX_GRAB_CNT; i++)
	{
		for(int j = 0; j < MAX_INSP_GRAB_CNT; j++)
		{
			for(int k = 0; k < TRACKER_MAX_NUM; k++)
			{
				if(m_TeachPara.AlignPara.MilSearchContext[i][j][k])
				{
					MmodFree(m_TeachPara.AlignPara.MilSearchContext[i][j][k]);
				}
			}
		}
	}

}

void CDialogTeach::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT_PARA_0, m_edit_nInspRegionCnt);
	DDX_Control(pDX, IDC_SLIDER_LIGHT_0, m_slider_Light_0);
	DDX_Control(pDX, IDC_SLIDER_LIGHT_1, m_slider_Light_1);
	DDX_Text(pDX, IDC_EDIT_PARA_1, m_edit_nLight_0);
	DDX_Text(pDX, IDC_EDIT_PARA_2, m_edit_nLight_1);
	DDX_Text(pDX, IDC_EDIT_PARA_3, m_edit_nImageGrabCnt);
	DDX_Text(pDX, IDC_EDIT_PARA_4, m_edit_nSelectImageIdx);
	DDX_Text(pDX, IDC_EDIT_PARA_5, m_edit_nMoveCntX);
	DDX_Text(pDX, IDC_EDIT_PARA_10, m_edit_nMoveCntY);
	DDX_Text(pDX, IDC_EDIT_PARA_6, m_edit_nDeviceCntX);
	DDX_Text(pDX, IDC_EDIT_PARA_7, m_edit_nDeviceCntY);
	DDX_Text(pDX, IDC_EDIT_PARA_8, m_edit_dGrabDistanceX);
	DDX_Text(pDX, IDC_EDIT_PARA_9, m_edit_dGrabDistanceY);
	DDX_Text(pDX, IDC_EDIT_PARA_11, m_edit_dFirstDevicePosX);
	DDX_Text(pDX, IDC_EDIT_PARA_12, m_edit_dFirstDevicePosY);
	DDX_Text(pDX, IDC_EDIT_PARA_13, m_edit_dFirstDevicePosZ);
	DDX_Text(pDX, IDC_EDIT_PARA_14, m_edit_nDeviceDistanceX);
	DDX_Text(pDX, IDC_EDIT_PARA_15, m_edit_nDeviceDistanceY);
	DDX_Text(pDX, IDC_EDIT_PARA_17, m_edit_dWaitPosX);
	DDX_Text(pDX, IDC_EDIT_PARA_18, m_edit_dWaitPosY);
	DDX_Text(pDX, IDC_EDIT_PARA_19, m_edit_dWaitPosZ);
	DDX_Text(pDX, IDC_EDIT_PARA_20, m_edit_nThreshHold1);
	DDX_Text(pDX, IDC_EDIT_PARA_21, m_edit_nInspThreshHold1);
	DDX_Text(pDX, IDC_EDIT_PARA_22, m_edit_nSpec);
	DDX_Text(pDX, IDC_EDIT_PARA_23, m_edit_nPixelSize);
	DDX_Control(pDX, IDC_BTNENHCTRL_PARA_LOCK, m_btn_para_lock);  //20160906 sc
	DDX_Control(pDX, IDC_EDIT_PARA_3, m_edit_ctrl_para3); 
	DDX_Control(pDX, IDC_EDIT_PARA_0, m_edit_ctrl_para0); 
	DDX_Control(pDX, IDC_EDIT_PARA_6, m_edit_ctrl_para6); 
	DDX_Control(pDX, IDC_EDIT_PARA_7, m_edit_ctrl_para7); 
	DDX_Control(pDX, IDC_EDIT_PARA_8, m_edit_ctrl_para8);
	DDX_Control(pDX, IDC_EDIT_PARA_9, m_edit_ctrl_para9);
	DDX_Control(pDX, IDC_EDIT_PARA_14, m_edit_ctrl_para14);
	DDX_Control(pDX, IDC_EDIT_PARA_15, m_edit_ctrl_para15);
	DDX_Control(pDX, IDC_EDIT_PARA_17, m_edit_ctrl_para17);
	DDX_Control(pDX, IDC_EDIT_PARA_18, m_edit_ctrl_para18);
	DDX_Control(pDX, IDC_EDIT_PARA_19, m_edit_ctrl_para19);
	DDX_Control(pDX, IDC_EDIT_PARA_11, m_edit_ctrl_para11);
	DDX_Control(pDX, IDC_EDIT_PARA_12, m_edit_ctrl_para12);
	DDX_Control(pDX, IDC_EDIT_PARA_13, m_edit_ctrl_para13);
	DDX_Control(pDX, IDC_EDIT_PARA_20, m_edit_ctrl_para20);
	DDX_Control(pDX, IDC_EDIT_PARA_21, m_edit_ctrl_para21);
	DDX_Control(pDX, IDC_EDIT_PARA_22, m_edit_ctrl_para22);  //20160906 sc
	DDX_Text(pDX, IDC_EDIT_PARA_24, m_edit_nInspThreshHold2);
	DDX_Text(pDX, IDC_EDIT_PARA_25, m_edit_nInspThreshHold3);
	DDX_Text(pDX, IDC_EDIT_PARA_26, m_edit_nInspGrabIdx);
	DDX_Text(pDX, IDC_EDIT_PARA_27, m_edit_nSelectInspGrabIdx);
	DDX_Text(pDX, IDC_EDIT_PARA_28, m_edit_nOneGrabInspCnt);
	DDX_Text(pDX, IDC_EDIT_PARA_29, m_edit_dCircleArea);
	DDX_Text(pDX, IDC_EDIT_PARA_30, m_edit_dCircleErodeDistance);
	DDX_Control(pDX, IDC_TEACH_EDGE_INSP, m_btn_InspEdge);
	DDX_Control(pDX, IDC_TEACH_BLOB_INSP, m_btn_InspBlob);
	DDV_MinMaxInt(pDX, m_edit_dCircleErodeDistance, 1, 10);
	DDX_Text(pDX, IDC_EDIT_PARA_31, m_edit_nAlignScore);
	DDX_Control(pDX, IDC_TEACH_NEW_INSP, m_btn_NewInspection);
	DDX_Text(pDX, IDC_EDIT_ALIGNSIZE, m_edit_nAlignSize);
	DDX_Text(pDX, IDC_EDIT_SECONDINSPTHRESHHOLD, m_edit_nSecondInspThreshHold);
	DDX_Control(pDX, IDC_EDIT_PARA_32, m_edit_ctrl_para32);
	DDX_Text(pDX, IDC_EDIT_PARA_32, m_edit_nShortSpec);
	DDX_Control(pDX, IDC_EDIT_PARA_28, m_edit_ctrl_para28);
	DDX_Control(pDX, IDC_EDIT_PARA_26, m_edit_ctrl_para26);
	DDX_Control(pDX, IDC_EDIT_PARA_24, m_edit_ctrl_para24);
	DDX_Control(pDX, IDC_EDIT_PARA_25, m_edit_ctrl_para25);
	DDX_Control(pDX, IDC_EDIT_PARA_29, m_edit_ctrl_para29);
	DDX_Control(pDX, IDC_EDIT_PARA_30, m_edit_ctrl_para30);
	DDX_Control(pDX, IDC_EDIT_PARA_31, m_edit_ctrl_para31);
	DDX_Control(pDX, IDC_EDIT_PARA_23, m_edit_ctrl_para23);
	DDX_Control(pDX, IDC_EDIT_ALIGNSIZE, m_edit_ctrl_para00);
}


BEGIN_MESSAGE_MAP(CDialogTeach, CDialogEx)
	ON_WM_SHOWWINDOW()
	ON_NOTIFY(NM_CUSTOMDRAW, IDC_SLIDER_LIGHT_0, &CDialogTeach::OnNMCustomdrawSliderLight0)
	ON_NOTIFY(NM_CUSTOMDRAW, IDC_SLIDER_LIGHT_1, &CDialogTeach::OnNMCustomdrawSliderLight1)
END_MESSAGE_MAP()


// DialogTeach 메시지 처리기입니다.


BOOL CDialogTeach::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  여기에 추가 초기화 작업을 추가합니다.
	CLED_DisplayInspectDlg *pdlg = ((CLED_DisplayInspectDlg *)AfxGetApp()->m_pMainWnd);

	CString strLog;
	strLog.Format(_T("CDialogTeach::OnInitDialog() Start!!"));
	WriteLog(strLog);

	pdlg->SetSelectModel(pdlg->m_dlgData->GetSelectModel());

	m_SavePath.Format(_T("%s"), pdlg->GetSelectModel().cPath);
	if(!LoadTeachPara(m_SavePath))
	{
		AfxMessageBox(_T("TeachPara.ini No File"));
	}

	m_slider_Light_0.SetRange(0,255);
	if(0 > m_edit_nLight_0)
	{
		m_edit_nLight_0 = 0;
	}
	if(255 < m_edit_nLight_0)
	{
		m_edit_nLight_0 = 255;
	}
	m_slider_Light_0.SetPos(m_edit_nLight_0);

	m_slider_Light_1.SetRange(0,255);
	if(0 > m_edit_nLight_1)
	{
		m_edit_nLight_1 = 0;
	}
	if(100 < m_edit_nLight_1)
	{
		m_edit_nLight_1 = 255;
	}
	m_slider_Light_1.SetPos(m_edit_nLight_1);

	strLog.Format(_T("Light Open Comp!!"));
#ifdef LIGHT
	m_cSerial.Open(LIGHT_COM_PORT, LIGHT_COM_BAUDRATE);

	if(m_cSerial.IsOpen())
	{
		OnOffLight(FALSE, 0);
	}
	else
	{
		strLog.Format(_T("Light Open Fail!!"));
		
	}
#endif
	WriteLog(strLog);

	m_dlgJog = new CDialogJog();
	m_dlgJog->Create(IDD_DIALOG_JOG, this);

	Update(FALSE);
	UpdateData(FALSE);

	CalTeachMovePos();

	
#ifdef INSPECTION_MAST_TEST
	GetDlgItem(IDC_BTNENHCTRL_LABEL_30)->ShowWindow(SW_HIDE);
	GetDlgItem(IDC_EDIT_PARA_29)->ShowWindow(SW_HIDE);
#endif
	m_bParaLock = TRUE;;
	SetParameterEnable(m_bParaLock);

	strLog.Format(_T("CDialogTeach::OnInitDialog() End!!"));
	WriteLog(strLog);

	return TRUE;  // return TRUE unless you set the focus to a control
	// 예외: OCX 속성 페이지는 FALSE를 반환해야 합니다.
}

BEGIN_EVENTSINK_MAP(CDialogTeach, CDialogEx)
	ON_EVENT(CDialogTeach, IDC_BTNENHCTRL_APPLY, DISPID_CLICK, CDialogTeach::ClickBtnenhctrlApply, VTS_NONE)
	ON_EVENT(CDialogTeach, IDC_BTNENHCTRL_OK, DISPID_CLICK, CDialogTeach::ClickBtnenhctrlOk, VTS_NONE)
	ON_EVENT(CDialogTeach, IDC_BTNENHCTRL_CANCEL, DISPID_CLICK, CDialogTeach::ClickBtnenhctrlCancel, VTS_NONE)
	ON_EVENT(CDialogTeach, IDC_BTNENHCTRL_SELECT_IMAGE_IDX_LEFT, DISPID_CLICK, CDialogTeach::ClickBtnenhctrlSelectImageIdxLeft, VTS_NONE)
	ON_EVENT(CDialogTeach, IDC_BTNENHCTRL_SELECT_IMAGE_IDX_RIGHT, DISPID_CLICK, CDialogTeach::ClickBtnenhctrlSelectImageIdxRight, VTS_NONE)
	ON_EVENT(CDialogTeach, IDC_BTNENHCTRL_CURENT_POS_SET, DISPID_CLICK, CDialogTeach::ClickBtnenhctrlCurentPosSet, VTS_NONE)
	ON_EVENT(CDialogTeach, IDC_BTNENHCTRL_POS_MOVE, DISPID_CLICK, CDialogTeach::ClickBtnenhctrlPosMove, VTS_NONE)
	ON_EVENT(CDialogTeach, IDC_BTNENHCTRL_LIGHT_SET_1, DISPID_CLICK, CDialogTeach::ClickBtnenhctrlLightSet1, VTS_NONE)
	ON_EVENT(CDialogTeach, IDC_BTNENHCTRL_LIGHT_SET_2, DISPID_CLICK, CDialogTeach::ClickBtnenhctrlLightSet2, VTS_NONE)
	ON_EVENT(CDialogTeach, IDC_BTNENHCTRL_POS_MOVE2, DISPID_CLICK, CDialogTeach::ClickBtnenhctrlPosMove2, VTS_NONE)
	ON_EVENT(CDialogTeach, IDC_BTNENHCTRL_SELECT_IMAGE_IDX_LEFT2, DISPID_CLICK, CDialogTeach::ClickBtnenhctrlSelectImageIdxLeft2, VTS_NONE)
	ON_EVENT(CDialogTeach, IDC_BTNENHCTRL_SELECT_IMAGE_IDX_RIGHT2, DISPID_CLICK, CDialogTeach::ClickBtnenhctrlSelectImageIdxRight2, VTS_NONE)
	ON_EVENT(CDialogTeach, IDC_BTNENHCTRL_SELECT_IMAGE_IDX_LEFT3, DISPID_CLICK, CDialogTeach::ClickBtnenhctrlSelectImageIdxLeft3, VTS_NONE)
	ON_EVENT(CDialogTeach, IDC_BTNENHCTRL_SELECT_IMAGE_IDX_RIGHT3, DISPID_CLICK, CDialogTeach::ClickBtnenhctrlSelectImageIdxRight3, VTS_NONE)
	ON_EVENT(CDialogTeach, IDC_BTNENHCTRL_CURENT_POS_SET2, DISPID_CLICK, CDialogTeach::ClickBtnenhctrlCurentWaitPosSet, VTS_NONE)
	ON_EVENT(CDialogTeach, IDC_BTNENHCTRL_POS_MOVE3, DISPID_CLICK, CDialogTeach::ClickBtnenhctrlWaitPosMove, VTS_NONE)
	ON_EVENT(CDialogTeach, IDC_BTNENHCTRL_PARA_LOCK, DISPID_CLICK, CDialogTeach::ClickBtnenhctrlParaLock, VTS_NONE)  //20160906 sc
	ON_EVENT(CDialogTeach, IDC_BTNENHCTRL_INSP_TEACH, DISPID_CLICK, CDialogTeach::ClickBtnenhctrlInspTeach, VTS_NONE)
	ON_EVENT(CDialogTeach, IDC_BTNENHCTRL_SELECT_IMAGE_IDX_LEFT4, DISPID_CLICK, CDialogTeach::ClickBtnenhctrlSelectImageIdxLeft4, VTS_NONE)
	ON_EVENT(CDialogTeach, IDC_BTNENHCTRL_SELECT_IMAGE_IDX_RIGHT4, DISPID_CLICK, CDialogTeach::ClickBtnenhctrlSelectImageIdxRight4, VTS_NONE)
	ON_EVENT(CDialogTeach, IDC_BTNENHCTRL_INSP_ALIGN_TEACH, DISPID_CLICK, CDialogTeach::ClickBtnenhctrlInspAlignTeach, VTS_NONE)
	ON_EVENT(CDialogTeach, IDC_BTNENHCTRL_INSP_ALIGN_TEST, DISPID_CLICK, CDialogTeach::ClickBtnenhctrlInspAlignTest, VTS_NONE)
	ON_EVENT(CDialogTeach, IDC_BTNENHCTRL_GRAB_DISTANCE, DISPID_CLICK, CDialogTeach::ClickBtnenhctrlGrabDistance, VTS_NONE)
	ON_EVENT(CDialogTeach, IDC_BTNENHCTRL_DEVICE_DISTANCE, DISPID_CLICK, CDialogTeach::ClickBtnenhctrlDeviceDistance, VTS_NONE)
	ON_EVENT(CDialogTeach, IDC_BTNENHCTRL_INSP_TEACH2, DISPID_CLICK, CDialogTeach::ClickBtnenhctrlInspTeach2, VTS_NONE)
	ON_EVENT(CDialogTeach, IDC_BTNENHCTRL_REGION_COPY, DISPID_CLICK, CDialogTeach::ClickBtnenhctrlRegionCopy, VTS_NONE)
	ON_EVENT(CDialogTeach, IDC_TEACH_EDGE_INSP, DISPID_CLICK, CDialogTeach::ClickTeachEdgeInsp, VTS_NONE)
	ON_EVENT(CDialogTeach, IDC_TEACH_BLOB_INSP, DISPID_CLICK, CDialogTeach::ClickTeachBlobInsp, VTS_NONE)
END_EVENTSINK_MAP()

void CDialogTeach::CalTeachMovePos()
{
	int move_idx = 0;
	memset(&m_TeachMovePos, NULL, sizeof(typedefMovePosPara));
	
	for(int i = 0; i < m_TeachPara.TeachDeviceCntY; i ++)
	{
		for(int j = 0; j < m_TeachPara.TeachDeviceCntX; j ++)
		{
			for(int k = 0; k < m_TeachPara.TeachGrabCnt; k++)
			{
				move_idx = j + (m_TeachPara.TeachDeviceCntX * i);
				m_TeachMovePos.MovePos_X[move_idx][k] = (long)( m_TeachPara.TeachFirstDevicePos[MOTION_X]	+ 
																							(m_TeachPara.TeachDeviceMotionDistanceX * j) + 
																							(m_TeachPara.TeachGrabMotionDistanceX * k) );

				m_TeachMovePos.MovePos_Y[move_idx][k] = (long)( m_TeachPara.TeachFirstDevicePos[MOTION_Y]	+ 
																							(m_TeachPara.TeachDeviceMotionDistanceY * i) );

				m_TeachMovePos.MovePos_Z[move_idx][k] = (long)m_TeachPara.TeachFirstDevicePos[MOTION_Z];
			}
		}
	}
}

long CDialogTeach::GetWaitMovePos(int nMotionNum)
{
	return (long)m_TeachPara.TeachWaitPos[nMotionNum];
}

long CDialogTeach::GetTeachMovePos(int nMotionNum, int nDeviceIdxX, int nDeviceIdxY, int nGrabIdx)
{
	long rslt = 0;
	int move_idx = 0;
	move_idx = nDeviceIdxX + (m_TeachPara.TeachDeviceCntX * nDeviceIdxY);

	switch(nMotionNum)
	{
	case MOTION_X:
		rslt = m_TeachMovePos.MovePos_X[move_idx][nGrabIdx];
		break;
	case MOTION_Y:
		rslt = m_TeachMovePos.MovePos_Y[move_idx][nGrabIdx];
		break;
	case MOTION_Z:
		rslt = m_TeachMovePos.MovePos_Z[move_idx][nGrabIdx];
		break;
	}

	return rslt;
}

void CDialogTeach::Update(BOOL isUpdate)
{
	if(!isUpdate)
	{
		m_edit_nInspRegionCnt = m_TeachPara.TeachRegionCnt[m_edit_nSelectImageIdx - 1][m_edit_nSelectInspGrabIdx - 1];
		m_edit_nImageGrabCnt = m_TeachPara.TeachGrabCnt;
		m_edit_nInspGrabIdx = m_TeachPara.TeachInspGrabCnt;
		m_edit_nDeviceCntX = m_TeachPara.TeachArrayCntX;
		m_edit_nDeviceCntY = m_TeachPara.TeachArrayCntY;
		m_TeachPara.TeachDeviceCntX = m_TeachPara.TeachArrayCntX;
		m_TeachPara.TeachDeviceCntY = (long)Rounding((((double)m_TeachPara.TeachArrayCntY / (double)m_TeachPara.TeachOneGrabInspCnt)), 0);
		m_edit_dGrabDistanceX = m_TeachPara.TeachGrabMotionDistanceX;
		m_edit_dGrabDistanceY = m_TeachPara.TeachGrabMotionDistanceY;
		m_edit_dFirstDevicePosX = m_TeachPara.TeachFirstDevicePos[MOTION_X];
		m_edit_dFirstDevicePosY = m_TeachPara.TeachFirstDevicePos[MOTION_Y];
		m_edit_dFirstDevicePosZ = m_TeachPara.TeachFirstDevicePos[MOTION_Z];
		m_edit_nDeviceDistanceX = m_TeachPara.TeachDeviceMotionDistanceX;
		m_edit_nDeviceDistanceY = m_TeachPara.TeachDeviceMotionDistanceY;
		m_edit_dWaitPosX = m_TeachPara.TeachWaitPos[MOTION_X];
		m_edit_dWaitPosY = m_TeachPara.TeachWaitPos[MOTION_Y];
		m_edit_dWaitPosZ = m_TeachPara.TeachWaitPos[MOTION_Z];

		m_edit_dCircleArea = m_TeachPara.TeachCircleArea[m_edit_nSelectInspGrabIdx - 1];
		m_edit_dCircleErodeDistance = m_TeachPara.TeachCircleErodeDistance[m_edit_nSelectInspGrabIdx - 1];

		m_edit_nSpec = m_TeachPara.TeachSpec;
		m_edit_nShortSpec = m_TeachPara.TeachShortSpec;
		m_edit_nPixelSize = m_TeachPara.TeachPixelSize;

		m_edit_nOneGrabInspCnt = m_TeachPara.TeachOneGrabInspCnt;

		m_edit_nLight_0 = m_TeachPara.TeachLight[m_edit_nSelectInspGrabIdx - 1][0];
		m_edit_nLight_1 = m_TeachPara.TeachLight[m_edit_nSelectInspGrabIdx - 1][1];

		m_edit_nThreshHold1 = m_TeachPara.TeachThreshHold[m_edit_nSelectInspGrabIdx - 1];
		m_edit_nInspThreshHold1 = m_TeachPara.TeachInspThreshHold[m_edit_nSelectInspGrabIdx - 1][0];
		m_edit_nInspThreshHold2 = m_TeachPara.TeachInspThreshHold[m_edit_nSelectInspGrabIdx - 1][1];
		m_edit_nInspThreshHold3 = m_TeachPara.TeachInspThreshHold[m_edit_nSelectInspGrabIdx - 1][2];
		//20161202nht
		m_edit_nSecondInspThreshHold = m_TeachPara.TeachSecondInspThreshHold[m_edit_nSelectInspGrabIdx - 1];

		m_edit_nAlignSize = m_TeachPara.TeachMaskSize[m_edit_nSelectInspGrabIdx - 1];

		m_slider_Light_0.SetPos(m_edit_nLight_0);
		m_slider_Light_1.SetPos(m_edit_nLight_1);

		m_edit_nAlignScore = m_TeachPara.TeachAlignScore;

		if(m_TeachPara.TeachInspType == 0)
		{
			m_bEdgeInsp = TRUE;
			m_bBlobInsp = FALSE;
			m_btn_InspEdge.SetValue(1);
		}
		else if(m_TeachPara.TeachInspType == 1)
		{
			m_bEdgeInsp = FALSE;
			m_bBlobInsp = TRUE;
			m_btn_InspBlob.SetValue(1);
		}

		m_btn_NewInspection.SetValue(m_TeachPara.TeachNewInsp);

		
	}
	else
	{
		m_edit_nLight_0 = m_slider_Light_0.GetPos();
		m_edit_nLight_1 = m_slider_Light_1.GetPos();

		m_TeachPara.TeachRegionCnt[m_edit_nSelectImageIdx - 1][m_edit_nSelectInspGrabIdx - 1] = m_edit_nInspRegionCnt;
		m_TeachPara.TeachGrabCnt = m_edit_nImageGrabCnt;
		m_TeachPara.TeachInspGrabCnt = m_edit_nInspGrabIdx;
		m_TeachPara.TeachArrayCntX = m_edit_nDeviceCntX;
		m_TeachPara.TeachArrayCntY = m_edit_nDeviceCntY;
		m_TeachPara.TeachDeviceCntX = m_edit_nDeviceCntX;
		m_TeachPara.TeachDeviceCntY = (long)Rounding((((double)m_TeachPara.TeachArrayCntY / (double)m_TeachPara.TeachOneGrabInspCnt)), 0);
		m_TeachPara.TeachGrabMotionDistanceX = m_edit_dGrabDistanceX;
		m_TeachPara.TeachGrabMotionDistanceY = m_edit_dGrabDistanceY;
		m_TeachPara.TeachFirstDevicePos[MOTION_X] = m_edit_dFirstDevicePosX;
		m_TeachPara.TeachFirstDevicePos[MOTION_Y] = m_edit_dFirstDevicePosY;
		m_TeachPara.TeachFirstDevicePos[MOTION_Z] = m_edit_dFirstDevicePosZ;
		m_TeachPara.TeachDeviceMotionDistanceX = m_edit_nDeviceDistanceX;
		m_TeachPara.TeachDeviceMotionDistanceY = m_edit_nDeviceDistanceY;
		m_TeachPara.TeachWaitPos[MOTION_X] = m_edit_dWaitPosX;
		m_TeachPara.TeachWaitPos[MOTION_Y] = m_edit_dWaitPosY;
		m_TeachPara.TeachWaitPos[MOTION_Z] = m_edit_dWaitPosZ;

		m_TeachPara.TeachCircleArea[m_edit_nSelectInspGrabIdx - 1] = m_edit_dCircleArea;
		m_TeachPara.TeachCircleErodeDistance[m_edit_nSelectInspGrabIdx - 1] = m_edit_dCircleErodeDistance;

		m_TeachPara.TeachSpec = m_edit_nSpec;
		m_TeachPara.TeachShortSpec = m_edit_nShortSpec;
		m_TeachPara.TeachPixelSize = m_edit_nPixelSize;

		m_TeachPara.TeachOneGrabInspCnt = m_edit_nOneGrabInspCnt;

		m_TeachPara.TeachLight[m_edit_nSelectInspGrabIdx - 1][0] = m_edit_nLight_0;
		m_TeachPara.TeachLight[m_edit_nSelectInspGrabIdx - 1][1] = m_edit_nLight_1;

		m_TeachPara.TeachThreshHold[m_edit_nSelectInspGrabIdx - 1] = m_edit_nThreshHold1;
		m_TeachPara.TeachInspThreshHold[m_edit_nSelectInspGrabIdx - 1][0] = m_edit_nInspThreshHold1;
		m_TeachPara.TeachInspThreshHold[m_edit_nSelectInspGrabIdx - 1][1] = m_edit_nInspThreshHold2;
		m_TeachPara.TeachInspThreshHold[m_edit_nSelectInspGrabIdx - 1][2] = m_edit_nInspThreshHold3;
		//20161202nht
		m_TeachPara.TeachSecondInspThreshHold[m_edit_nSelectInspGrabIdx - 1] = m_edit_nSecondInspThreshHold;

		m_TeachPara.TeachMaskSize[m_edit_nSelectInspGrabIdx - 1] = m_edit_nAlignSize;

		m_TeachPara.TeachAlignScore = m_edit_nAlignScore;

		if(m_bEdgeInsp && !m_bBlobInsp)
		{
			m_TeachPara.TeachInspType = 0;
		}
		else if(!m_bEdgeInsp && m_bBlobInsp)
		{
			m_TeachPara.TeachInspType = 1;
		}
		m_TeachPara.TeachNewInsp = m_btn_NewInspection.GetValue();
	}
}

void CDialogTeach::SetApply()
{
	CLED_DisplayInspectDlg *pdlg = ((CLED_DisplayInspectDlg *)AfxGetApp()->m_pMainWnd);
	
	UpdateData(TRUE);
	Update(TRUE);	

	CalTeachMovePos();

	for(int i = 0; i < m_TeachPara.TeachRegionCnt[m_edit_nSelectImageIdx - 1][m_edit_nSelectInspGrabIdx - 1] * 3; i++)
	{
		//pdlg->m_dlgImageRect->m_Tracker[m_edit_nSelectImageIdx - 1][m_edit_nSelectInspGrabIdx - 1][i].SetActive(TRUE);
		if(m_edit_nSelectInspGrabIdx - 1 > 0) //20160923 sc
		{
			if(i == 2 || i % 3 == 2)
			{
				pdlg->m_dlgImageRect->m_Tracker[m_edit_nSelectImageIdx - 1][m_edit_nSelectInspGrabIdx - 1][i].SetActive(FALSE);
			}
			
		}
		else
		{
			pdlg->m_dlgImageRect->m_Tracker[m_edit_nSelectImageIdx - 1][m_edit_nSelectInspGrabIdx - 1][i].SetActive(FALSE);
		}
	}

	pdlg->m_dlgImageRect->SetReDraw();

	pdlg->m_dlgImageRect->SetInsIdx(m_edit_nSelectImageIdx);
	pdlg->m_dlgImageRect->SetGrabIdx(m_edit_nSelectInspGrabIdx);

	pdlg->SetGrid();
#ifdef CAMERA
	MdispControl(*pdlg->m_MilManage.GetMilDisplay(MAIN_DISP), M_OVERLAY_CLEAR, M_DEFAULT); //20161014 sc 화면 클리어 추가
#endif
}

void CDialogTeach::ClickBtnenhctrlApply()
{
	// TODO: 여기에 메시지 처리기 코드를 추가합니다.
	CLED_DisplayInspectDlg *pdlg = ((CLED_DisplayInspectDlg *)AfxGetApp()->m_pMainWnd);
	UpdateData(TRUE);
	Update(TRUE);

	CalTeachMovePos();

	for(int i = 0; i < m_TeachPara.TeachRegionCnt[m_edit_nSelectImageIdx - 1][m_edit_nSelectInspGrabIdx - 1] * 3; i++)
	{
		//pdlg->m_dlgImageRect->m_Tracker[m_edit_nSelectImageIdx - 1][m_edit_nSelectInspGrabIdx - 1][i].SetActive(TRUE);
		if(m_edit_nSelectInspGrabIdx - 1 > 0) //20160923 sc
		{
			if(i == 2 || i % 3 == 2)
			{
				pdlg->m_dlgImageRect->m_Tracker[m_edit_nSelectImageIdx - 1][m_edit_nSelectInspGrabIdx - 1][i].SetActive(TRUE);
			}
			
		}
		else
		{
			pdlg->m_dlgImageRect->m_Tracker[m_edit_nSelectImageIdx - 1][m_edit_nSelectInspGrabIdx - 1][i].SetActive(TRUE);
		}
	}

	pdlg->m_dlgImageRect->SetReDraw();

	pdlg->m_dlgImageRect->SetInsIdx(m_edit_nSelectImageIdx);
	pdlg->m_dlgImageRect->SetGrabIdx(m_edit_nSelectInspGrabIdx);

	pdlg->SetGrid();

	pdlg->m_dlgImageRect->SetApplyTracker();
#ifdef CAMERA
	MdispControl(*pdlg->m_MilManage.GetMilDisplay(MAIN_DISP), M_OVERLAY_CLEAR, M_DEFAULT); //20161014 sc 화면 클리어 추가
#endif
	/*if(m_edit_nImageGrabCnt < 2)
	{
		GetDlgItem(IDC_BTNENHCTRL_GRAB_DISTANCE)->ShowWindow(SW_HIDE);
	}
	else if(m_edit_nImageGrabCnt >= 2)
	{
		GetDlgItem(IDC_BTNENHCTRL_GRAB_DISTANCE)->ShowWindow(SW_SHOW);
	}
	pdlg->m_dlgImageRect->SetApplyTracker();*/
}

void CDialogTeach::SetSavePath(CString path)
{
	m_SavePath = path;
}

void CDialogTeach::ClickBtnenhctrlOk()
{
	// TODO: 여기에 메시지 처리기 코드를 추가합니다.
	CLED_DisplayInspectDlg *pdlg = ((CLED_DisplayInspectDlg *)AfxGetApp()->m_pMainWnd);
	UpdateData(TRUE);
	Update(TRUE);

	CalTeachMovePos();

	for(int i = 0; i < m_TeachPara.TeachRegionCnt[m_edit_nSelectImageIdx - 1][m_edit_nSelectInspGrabIdx - 1] * 3; i++)
	{
		//pdlg->m_dlgImageRect->m_Tracker[m_edit_nSelectImageIdx - 1][m_edit_nSelectInspGrabIdx - 1][i].SetActive(TRUE);
		if(m_edit_nSelectInspGrabIdx - 1 > 0) //20160923 sc
		{
			if(i == 2 || i % 3 == 2)
			{
				pdlg->m_dlgImageRect->m_Tracker[m_edit_nSelectImageIdx - 1][m_edit_nSelectInspGrabIdx - 1][i].SetActive(TRUE);
			}
			
		}
		else
		{
			pdlg->m_dlgImageRect->m_Tracker[m_edit_nSelectImageIdx - 1][m_edit_nSelectInspGrabIdx - 1][i].SetActive(TRUE);
		}
	}

	pdlg->m_dlgImageRect->SetReDraw();

	pdlg->m_dlgImageRect->SetInsIdx(m_edit_nSelectImageIdx);
	pdlg->m_dlgImageRect->SetGrabIdx(m_edit_nSelectInspGrabIdx);

	pdlg->SetGrid();

	pdlg->m_dlgImageRect->SetApplyTracker();
	CString temp;
	temp.Format(_T("%s"), pdlg->GetSelectModel().cPath);
	SaveTeachPara(temp);
}

void CDialogTeach::ClickBtnenhctrlCancel()
{
	// TODO: 여기에 메시지 처리기 코드를 추가합니다.
	CLED_DisplayInspectDlg *pdlg = ((CLED_DisplayInspectDlg *)AfxGetApp()->m_pMainWnd);
	LoadTeachPara(m_SavePath);
	pdlg->m_dlgImageRect->SetInsIdx(m_edit_nSelectImageIdx);
	pdlg->m_dlgImageRect->SetGrabIdx(m_edit_nSelectInspGrabIdx);
	pdlg->m_dlgImageRect->SetAllTracker();
	Update(FALSE);
	UpdateData(FALSE);
	pdlg->SetGrid();
	ShowWindow(SW_HIDE);
}

void CDialogTeach::OnShowWindow(BOOL bShow, UINT nStatus)
{
	CDialogEx::OnShowWindow(bShow, nStatus);

	CLED_DisplayInspectDlg *pdlg = ((CLED_DisplayInspectDlg *)AfxGetApp()->m_pMainWnd);
	int nRegionCnt = m_TeachPara.TeachRegionCnt[m_edit_nSelectImageIdx - 1][m_edit_nSelectInspGrabIdx - 1];
	int nGrabCnt = m_TeachPara.TeachGrabCnt;
	CRect rect;
	double l,t,r,b;

	UpdateData(TRUE); //20160923 sc
	int InspGrabIdx =  m_edit_nSelectInspGrabIdx;
	for(int i = 0; i < MAX_GRAB_CNT; i++)
	{
		for(int j = 0; j < MAX_INSP_GRAB_CNT; j++)
		{
			for(int k = 0; k < TRACKER_MAX_NUM; k++)
			{
				pdlg->m_dlgImageRect->m_Tracker[i][j][k].SetActive(FALSE);
			}
		}
	}

	if(bShow)
	{
		m_bParaLock = TRUE;;
		SetParameterEnable(m_bParaLock);

		pdlg->m_dlgImageRect->SetReDraw();

		LoadTeachPara(m_SavePath);
		pdlg->m_dlgImageRect->SetInsIdx(m_edit_nSelectImageIdx);
		pdlg->m_dlgImageRect->SetGrabIdx(m_edit_nSelectInspGrabIdx);
		pdlg->m_dlgImageRect->SetAllTracker();
		Update(FALSE);
		UpdateData(FALSE);

		if(m_TeachPara.TeachInspType == 0)
		{
			ClickTeachEdgeInsp();
		}
		else
		{
			ClickTeachBlobInsp();
		}

		//GetDlgItem(IDC_STATIC_TEACH_JOG)->GetClientRect(rect);
		GetDlgItem(IDC_STATIC_TEACH_JOG)->GetWindowRect(rect);
		//m_dlgJog->SetWindowPos((&CWnd::wndTopMost), rect.left, rect.top, rect.Width(), rect.Height(),
         //                                 SWP_NOMOVE|SWP_NOSIZE|SWP_SHOWWINDOW); 
		m_dlgJog->ShowWindow(SW_SHOW);
		m_dlgJog->MoveWindow(rect.left, rect.top, rect.Width(), rect.Height(), TRUE);

		for(int i = 0; i < m_TeachPara.TeachRegionCnt[m_edit_nSelectImageIdx - 1][m_edit_nSelectInspGrabIdx - 1] * 3; i++)
		{
			pdlg->m_dlgImageRect->m_Tracker[m_edit_nSelectImageIdx - 1][m_edit_nSelectInspGrabIdx - 1][i].GetZoom(&l, &t, &r, &b);
			pdlg->m_dlgImageRect->m_Tracker[m_edit_nSelectImageIdx - 1][m_edit_nSelectInspGrabIdx - 1][i].SetRect(
			(m_TeachPara.TeachRegion[m_edit_nSelectImageIdx - 1][m_edit_nSelectInspGrabIdx - 1][i].GetLeft() * pdlg->m_dlgImageRect->m_dZoomRatio) - (l * pdlg->m_dlgImageRect->m_dZoomRatio),
			(m_TeachPara.TeachRegion[m_edit_nSelectImageIdx - 1][m_edit_nSelectInspGrabIdx - 1][i].GetTop() * pdlg->m_dlgImageRect->m_dZoomRatio) - (t * pdlg->m_dlgImageRect->m_dZoomRatio),
			(m_TeachPara.TeachRegion[m_edit_nSelectImageIdx - 1][m_edit_nSelectInspGrabIdx - 1][i].GetRight() * pdlg->m_dlgImageRect->m_dZoomRatio) - (l * pdlg->m_dlgImageRect->m_dZoomRatio),
			(m_TeachPara.TeachRegion[m_edit_nSelectImageIdx - 1][m_edit_nSelectInspGrabIdx - 1][i].GetBottom() * pdlg->m_dlgImageRect->m_dZoomRatio) - (t * pdlg->m_dlgImageRect->m_dZoomRatio));

			if(InspGrabIdx - 1 > 0) //20160923 sc
			{
				if(((i % 3) == 0) || ((i % 3) == 1))
				{
					pdlg->m_dlgImageRect->m_Tracker[m_edit_nSelectImageIdx - 1][m_edit_nSelectInspGrabIdx - 1][i].SetActive(FALSE);
				}
				else
				{
					pdlg->m_dlgImageRect->m_Tracker[m_edit_nSelectImageIdx - 1][m_edit_nSelectInspGrabIdx - 1][i].SetActive(TRUE);
				}				
			}
			else
			{
				pdlg->m_dlgImageRect->m_Tracker[m_edit_nSelectImageIdx - 1][m_edit_nSelectInspGrabIdx - 1][i].SetActive(TRUE);
			}
		}

		pdlg->m_dlgImageRect->SetReDraw();
		SetLight(LIGHT_1_CH, m_TeachPara.TeachLight[m_edit_nSelectInspGrabIdx - 1][0], LIGHT_COM_PORT);
		SetLight(LIGHT_2_CH, m_TeachPara.TeachLight[m_edit_nSelectInspGrabIdx - 1][1], LIGHT_COM_PORT);
	}
	else
	{
		m_dlgJog->ShowWindow(SW_HIDE);
		
		pdlg->m_dlgImageRect->Invalidate();
		OnOffLight(FALSE, 0);
#ifdef CAMERA
		MdispControl(*pdlg->m_MilManage.GetMilDisplay(MAIN_DISP), M_OVERLAY_CLEAR, M_DEFAULT);
#endif
	}

	if(m_edit_nImageGrabCnt < 2)
	{
		GetDlgItem(IDC_BTNENHCTRL_GRAB_DISTANCE)->ShowWindow(SW_HIDE);
	}
	else if(m_edit_nImageGrabCnt >= 2)
	{
		GetDlgItem(IDC_BTNENHCTRL_GRAB_DISTANCE)->ShowWindow(SW_SHOW);
	}
}

typeTeachPara CDialogTeach::GetTeachPara()
{
	return m_TeachPara;
}

void CDialogTeach::SetTeachPara(typeTeachPara teachPara)
{
	m_TeachPara = teachPara;
}

BOOL CDialogTeach::LoadTeachPara(CString path)
{
	char temp[DAT_STR_LENG];
	BOOL rslt = TRUE;
	CString key, dat, temp_key;
	CString file_path, strModPath;
	double left, top, right, bottom;
	CString strLog;
	strLog.Format(_T("LoadTeachPara Start!!"));
	WriteLog(strLog);

	file_path = path + TEACH_PARA_FILE_NAME;
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
		key.Format("TEACH_PIXEL_SIZE");
		GetPrivateProfileString("TEACH_PARA", key, _T("25"), temp, 256, file_path);
		dat.Format("%s", temp);
		m_TeachPara.TeachPixelSize = atoi(dat);

		key.Empty();
		key.Format("TEACH_OFF_SET");
		GetPrivateProfileString("TEACH_PARA", key, _T("25"), temp, 256, file_path);
		dat.Format("%s", temp);
		m_TeachPara.TeachRsltOffSet = atoi(dat);

		key.Empty();
		key.Format("TEACH_GRAB_CNT");
		GetPrivateProfileString("TEACH_PARA", key, _T("1"), temp, 256, file_path);
		dat.Format("%s", temp);
		m_TeachPara.TeachGrabCnt = atoi(dat);

		key.Empty();
		key.Format(_T("DEVICE_CNT_X"));
		GetPrivateProfileString(_T("TEACH_PARA"), key, _T("1"), temp, sizeof(temp), file_path);
		m_TeachPara.TeachArrayCntX = atoi(temp);	

		key.Empty();
		key.Format(_T("DEVICE_CNT_Y"));
		GetPrivateProfileString(_T("TEACH_PARA"), key, _T("1"), temp, sizeof(temp), file_path);
		m_TeachPara.TeachArrayCntY = atoi(temp);	

		key.Empty();
		key.Format(_T("GRAB_DISTANCE_X"));
		GetPrivateProfileString(_T("TEACH_PARA"), key, _T("0.0"), temp, sizeof(temp), file_path);
		m_TeachPara.TeachGrabMotionDistanceX = atof(temp);	

		key.Empty();
		key.Format(_T("GRAB_DISTANCE_Y"));
		GetPrivateProfileString(_T("TEACH_PARA"), key, _T("0.0"), temp, sizeof(temp), file_path);
		m_TeachPara.TeachGrabMotionDistanceY = atof(temp);	

		key.Empty();
		key.Format(_T("DEVICE_DISTANCE_X"));
		GetPrivateProfileString(_T("TEACH_PARA"), key, _T("0.0"), temp, sizeof(temp), file_path);
		m_TeachPara.TeachDeviceMotionDistanceX = atof(temp);	

		key.Empty();
		key.Format(_T("DEVICE_DISTANCE_Y"));
		GetPrivateProfileString(_T("TEACH_PARA"), key, _T("0.0"), temp, sizeof(temp), file_path);
		m_TeachPara.TeachDeviceMotionDistanceY = atof(temp);
		
		for(int i = 0; i < MAX_MOTION_CNT; i++)
		{
			if(i == MOTION_X)
			{
				temp_key.Format(_T("X"));
			}
			else if(i == MOTION_Y)
			{
				temp_key.Format(_T("Y"));
			}
			else
			{
				temp_key.Format(_T("Z"));
			}

			key.Empty();
			key.Format("FIRST_DEVICE_POS_%s", temp_key);
			GetPrivateProfileString(_T("TEACH_PARA"), key, _T("0.0"), temp, sizeof(temp), file_path);
			m_TeachPara.TeachFirstDevicePos[i] = atof(temp);		

			key.Empty();
			key.Format("WAIT_POS_%s", temp_key);
			GetPrivateProfileString(_T("TEACH_PARA"), key, _T("0.0"), temp, sizeof(temp), file_path);
			m_TeachPara.TeachWaitPos[i] = atof(temp);
			
		}

		key.Empty();
		key.Format("TEACH_ALIGN_SCORE");
		GetPrivateProfileString("TEACH_PARA", key, _T("100"), temp, 256, file_path);
		dat.Format("%s", temp);
		m_TeachPara.TeachAlignScore = atoi(dat);

		key.Empty();
		key.Format("TEACH_SPEC");
		GetPrivateProfileString("TEACH_PARA", key, _T("100"), temp, 256, file_path);
		dat.Format("%s", temp);
		m_TeachPara.TeachSpec = atoi(dat);

		key.Empty();
		key.Format("TEACH_SHORT_SPEC");
		GetPrivateProfileString("TEACH_PARA", key, _T("25"), temp, 256, file_path);
		dat.Format("%s", temp);
		m_TeachPara.TeachShortSpec = atoi(dat);

		key.Empty();
		key.Format("TEACH_INSP_GRAB_CNT");
		GetPrivateProfileString("TEACH_PARA", key, _T("1"), temp, 256, file_path);
		dat.Format("%s", temp);
		m_TeachPara.TeachInspGrabCnt = atoi(dat);

		key.Empty();
		key.Format("TEACH_ONE_GRAB_INSP_CNT");
		GetPrivateProfileString("TEACH_PARA", key, _T("1"), temp, 256, file_path);
		dat.Format("%s", temp);
		m_TeachPara.TeachOneGrabInspCnt = atoi(dat);

		key.Empty();
		key.Format("TEACH_ALIGN");
		GetPrivateProfileString("TEACH_PARA", key, _T("1"), temp, 256, file_path);
		dat.Format("%s", temp);
		m_TeachPara.AlignPara.bAlign = atoi(dat);

		key.Empty();
		key.Format("TEACH_INSP_TYPE");
		GetPrivateProfileString("TEACH_PARA", key, _T("0"), temp, 256, file_path);
		dat.Format("%s", temp);
		m_TeachPara.TeachInspType = atoi(dat);


		key.Empty();
		key.Format("TEACH_NEW_INSP");
		GetPrivateProfileString("TEACH_PARA", key, _T("0"), temp, 256, file_path);
		dat.Format("%s", temp);
		m_TeachPara.TeachNewInsp = atoi(dat);

		for(int k = 0; k < TRACKER_MAX_NUM; k++)
		{
			key.Empty();
			key.Format("TEACH_ALIGN_X_%d", k);
			GetPrivateProfileString("TEACH_PARA", key, _T("0.0"), temp, 256, file_path);
			dat.Format("%s", temp);
			m_TeachPara.AlignPara.dCentX[k] = atof(dat);

			key.Empty();
			key.Format("TEACH_ALIGN_Y_%d", k);
			GetPrivateProfileString("TEACH_PARA", key, _T("0.0"), temp, 256, file_path);
			dat.Format("%s", temp);
			m_TeachPara.AlignPara.dCentY[k] = atof(dat);
		}

		for(int k = 0; k < m_TeachPara.TeachInspGrabCnt; k++)
		{
			key.Empty();
			key.Format("TEACH_CIRCLE_AREA_%d", k);
			GetPrivateProfileString("TEACH_PARA", key, _T("100.0"), temp, 256, file_path);
			dat.Format("%s", temp);
			m_TeachPara.TeachCircleArea[k] = atof(dat);

			key.Empty();
			key.Format("TEACH_CIRCLE_ERODE_DISTANCE_%d", k);
			GetPrivateProfileString("TEACH_PARA", key, _T("2.0"), temp, 256, file_path);
			dat.Format("%s", temp);
			m_TeachPara.TeachCircleErodeDistance[k] = atoi(dat);

			key.Empty();
			key.Format("TEACH_MASK_SIZE_%d", k);
			GetPrivateProfileString("TEACH_PARA", key, _T("0.0"), temp, 256, file_path);
			dat.Format("%s", temp);
			m_TeachPara.TeachMaskSize[k] = atof(dat);
		}

		for(int k = 0; k < m_TeachPara.TeachInspGrabCnt; k++)
		{
			key.Empty();
			key.Format("TEACH_LIGHT_0_%d", k);
			GetPrivateProfileString("TEACH_PARA", key, _T("0"), temp, 256, file_path);
			dat.Format("%s", temp);
			m_TeachPara.TeachLight[k][0] = atoi(dat);

			key.Empty();
			key.Format("TEACH_LIGHT_1_%d", k);
			GetPrivateProfileString("TEACH_PARA", key, _T("0"), temp, 256, file_path);
			dat.Format("%s", temp);
			m_TeachPara.TeachLight[k][1] = atoi(dat);

			key.Empty();
			key.Format("TEACH_THRESH_HOLD_%d", k);
			GetPrivateProfileString("TEACH_PARA", key, _T("0"), temp, 256, file_path);
			dat.Format("%s", temp);
			m_TeachPara.TeachThreshHold[k] = (int)(atof(dat));

			

			for(int i = 0; i < MAX_THRESH_NUM; i++)
			{
				key.Empty();
				key.Format("TEACH_INSP_THRESH_HOLD_%d_%d", k, i);
				GetPrivateProfileString("TEACH_PARA", key, _T("10"), temp, 256, file_path);
				dat.Format("%s", temp);
				m_TeachPara.TeachInspThreshHold[k][i] = atoi(dat);
			}
			

			//20161202nht
			key.Empty();
			key.Format("TEACH_SECOND_INSP_THRESH_HOLD_%d", k);
			GetPrivateProfileString("TEACH_PARA", key, _T("0"), temp, 256, file_path);
			dat.Format("%s", temp);
			m_TeachPara.TeachSecondInspThreshHold[k] = atof(dat);

			for(int i = 0; i < m_TeachPara.TeachGrabCnt; i++)
			{
				key.Empty();
				key.Format("TEACH_REGION_CNT_%d_%d", i, k);
				GetPrivateProfileString("TEACH_PARA", key, _T("1"), temp, 256, file_path);
				dat.Format("%s", temp);
				m_TeachPara.TeachRegionCnt[i][k] = atoi(dat);

				for(int j = 0; j < m_TeachPara.TeachRegionCnt[i][k] * 3; j++)
				{
					key.Empty();
					key.Format("TEACH_ALIGN_%d_%d_%d", i, k, j);
					GetPrivateProfileString("TEACH_PARA", key, _T("0"), temp, 256, file_path);
					dat.Format("%s", temp);
					m_TeachPara.AlignPara.bCreatMdID[i][k][j] = atoi(dat);	

					key.Empty();
					key.Format(("TEACH_REGION_[%03d][%03d][%03d]_LEFT"), i, k, j);
					dat.Empty();
					GetPrivateProfileString("TEACH_PARA", key, _T("100"), temp, 256, file_path);
					dat.Format("%s", temp);
					left = atof(dat);

					key.Empty();
					key.Format(("TEACH_REGION_[%03d][%03d][%03d]_TOP"), i, k, j);
					dat.Empty();
					GetPrivateProfileString("TEACH_PARA", key, _T("100"), temp, 256, file_path);
					dat.Format("%s", temp);
					top = atof(dat);

					key.Empty();
					key.Format(("TEACH_REGION_[%03d][%03d][%03d]_RIGHT"), i, k, j);
					dat.Empty();
					GetPrivateProfileString("TEACH_PARA", key, _T("200"), temp, 256, file_path);
					dat.Format("%s", temp);
					right = atof(dat);

					key.Empty();
					key.Format(("TEACH_REGION_[%03d][%03d][%03d]_BOTTOM"), i, k, j);
					dat.Empty();
					GetPrivateProfileString("TEACH_PARA", key, _T("200"), temp, 256, file_path);
					dat.Format("%s", temp);
					bottom = atof(dat);

					m_TeachPara.TeachRegion[i][k][j].SetdRectLTRB(left, top, right, bottom);
				}
			}
		}
	}

	strLog.Format(_T("LoadTeachPara End!!"));
	WriteLog(strLog);

	return rslt;
}

BOOL CDialogTeach::SaveTeachPara(CString path)
{
	BOOL rslt = TRUE;
	CString key, dat, temp_key;
	CString file_path, pat_path, extention, master_path, strModPath;

	CString strLog;
	strLog.Format(_T("SaveTeachPara Start!!"));
	WriteLog(strLog);

	file_path = path + TEACH_PARA_FILE_NAME;

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

		//write INI
		key.Empty();
		key.Format("TEACH_PIXEL_SIZE");
		dat.Empty();
		dat.Format("%d", m_TeachPara.TeachPixelSize);
		WritePrivateProfileString("TEACH_PARA", key, dat, file_path);

		key.Empty();
		key.Format("TEACH_OFF_SET");
		dat.Empty();
		dat.Format("%d", m_TeachPara.TeachRsltOffSet);
		WritePrivateProfileString("TEACH_PARA", key, dat, file_path);

		key.Empty();
		key.Format("TEACH_GRAB_CNT");
		dat.Empty();
		dat.Format("%d", m_TeachPara.TeachGrabCnt);
		WritePrivateProfileString("TEACH_PARA", key, dat, file_path);

		key.Empty();
		key.Format("DEVICE_CNT_X");
		dat.Empty();
		dat.Format("%d", m_TeachPara.TeachArrayCntX);
		WritePrivateProfileString("TEACH_PARA", key, dat, file_path);

		key.Empty();
		key.Format("DEVICE_CNT_Y");
		dat.Empty();
		dat.Format("%d", m_TeachPara.TeachArrayCntY);
		WritePrivateProfileString("TEACH_PARA", key, dat, file_path);

		key.Empty();
		key.Format("GRAB_DISTANCE_X");
		dat.Empty();
		dat.Format("%f", m_TeachPara.TeachGrabMotionDistanceX);
		WritePrivateProfileString("TEACH_PARA", key, dat, file_path);

		key.Empty();
		key.Format("GRAB_DISTANCE_Y");
		dat.Empty();
		dat.Format("%f", m_TeachPara.TeachGrabMotionDistanceY);
		WritePrivateProfileString("TEACH_PARA", key, dat, file_path);

		key.Empty();
		key.Format(_T("DEVICE_DISTANCE_X"));
		dat.Empty();
		dat.Format("%f", m_TeachPara.TeachDeviceMotionDistanceX);
		WritePrivateProfileString("TEACH_PARA", key, dat, file_path);	

		key.Empty();
		key.Format(_T("DEVICE_DISTANCE_Y"));
		dat.Empty();
		dat.Format("%f", m_TeachPara.TeachDeviceMotionDistanceY);
		WritePrivateProfileString("TEACH_PARA", key, dat, file_path);
		
		for(int i = 0; i < MAX_MOTION_CNT; i++)
		{
			if(i == MOTION_X)
			{
				temp_key.Format(_T("X"));
			}
			else if(i == MOTION_Y)
			{
				temp_key.Format(_T("Y"));
			}
			else
			{
				temp_key.Format(_T("Z"));
			}

			key.Empty();
			key.Format("FIRST_DEVICE_POS_%s", temp_key);
			dat.Empty();
			dat.Format("%f", m_TeachPara.TeachFirstDevicePos[i]);
			WritePrivateProfileString("TEACH_PARA", key, dat, file_path);	

			key.Empty();
			key.Format("WAIT_POS_%s", temp_key);
			dat.Empty();
			dat.Format("%f", m_TeachPara.TeachWaitPos[i]);
			WritePrivateProfileString("TEACH_PARA", key, dat, file_path);
		}

		key.Empty();
		key.Format("TEACH_ALIGN_SCORE");
		dat.Empty();
		dat.Format("%d", m_TeachPara.TeachAlignScore);
		WritePrivateProfileString("TEACH_PARA", key, dat, file_path);

		key.Empty();
		key.Format("TEACH_SPEC");
		dat.Empty();
		dat.Format("%d", m_TeachPara.TeachSpec);
		WritePrivateProfileString("TEACH_PARA", key, dat, file_path);

		key.Empty();
		key.Format("TEACH_SHORT_SPEC");
		dat.Empty();
		dat.Format("%d", m_TeachPara.TeachShortSpec);
		WritePrivateProfileString("TEACH_PARA", key, dat, file_path);

		key.Empty();
		key.Format("TEACH_INSP_GRAB_CNT");
		dat.Empty();
		dat.Format("%d", m_TeachPara.TeachInspGrabCnt);
		WritePrivateProfileString("TEACH_PARA", key, dat, file_path);

		key.Empty();
		key.Format("TEACH_ONE_GRAB_INSP_CNT");
		dat.Empty();
		dat.Format("%d", m_TeachPara.TeachOneGrabInspCnt);
		WritePrivateProfileString("TEACH_PARA", key, dat, file_path);

		key.Empty();
		key.Format("TEACH_ALIGN");
		dat.Empty();
		dat.Format("%d", m_TeachPara.AlignPara.bAlign);
		WritePrivateProfileString("TEACH_PARA", key, dat, file_path);

		key.Empty();
		key.Format("TEACH_INSP_TYPE");
		dat.Empty();
		dat.Format("%d", m_TeachPara.TeachInspType);
		WritePrivateProfileString("TEACH_PARA", key, dat, file_path);

		key.Empty();
		key.Format("TEACH_NEW_INSP");
		dat.Empty();
		dat.Format("%d", m_TeachPara.TeachNewInsp);
		WritePrivateProfileString("TEACH_PARA", key, dat, file_path);

		for(int k = 0; k < TRACKER_MAX_NUM; k++)
		{
			key.Empty();
			key.Format("TEACH_ALIGN_X_%d", k);
			dat.Empty();
			dat.Format("%f", m_TeachPara.AlignPara.dCentX[k]);
			WritePrivateProfileString("TEACH_PARA", key, dat, file_path);

			key.Empty();
			key.Format("TEACH_ALIGN_Y_%d", k);
			dat.Empty();
			dat.Format("%f", m_TeachPara.AlignPara.dCentY[k]);
			WritePrivateProfileString("TEACH_PARA", key, dat, file_path);
		}

		for(int k = 0; k < m_TeachPara.TeachInspGrabCnt; k++)
		{
			key.Empty();
			key.Format("TEACH_CIRCLE_AREA_%d", k);
			dat.Empty();
			dat.Format("%f", m_TeachPara.TeachCircleArea[k]);
			WritePrivateProfileString("TEACH_PARA", key, dat, file_path);

			key.Empty();
			key.Format("TEACH_CIRCLE_ERODE_DISTANCE_%d", k);
			dat.Empty();
			dat.Format("%d", m_TeachPara.TeachCircleErodeDistance[k]);
			WritePrivateProfileString("TEACH_PARA", key, dat, file_path);

			key.Empty();
			key.Format("TEACH_MASK_SIZE_%d", k);
			dat.Empty();
			dat.Format("%f", m_TeachPara.TeachMaskSize[k]);
			WritePrivateProfileString("TEACH_PARA", key, dat, file_path);
		}

		for(int k = 0; k < m_TeachPara.TeachInspGrabCnt; k++)
		{
			key.Empty();
			key.Format("TEACH_LIGHT_0_%d", k);
			dat.Empty();
			dat.Format("%d", m_TeachPara.TeachLight[k][0]);
			WritePrivateProfileString("TEACH_PARA", key, dat, file_path);

			key.Empty();
			key.Format("TEACH_LIGHT_1_%d", k);
			dat.Empty();
			dat.Format("%d", m_TeachPara.TeachLight[k][1]);
			WritePrivateProfileString("TEACH_PARA", key, dat, file_path);

			key.Empty();
			key.Format("TEACH_THRESH_HOLD_%d", k);
			dat.Empty();
			dat.Format("%d", m_TeachPara.TeachThreshHold[k]);
			WritePrivateProfileString("TEACH_PARA", key, dat, file_path);

			for(int i = 0; i < MAX_THRESH_NUM; i++)
			{
				key.Empty();
				key.Format("TEACH_INSP_THRESH_HOLD_%d_%d", k, i);
				dat.Empty();
				dat.Format("%d", m_TeachPara.TeachInspThreshHold[k][i]);
				WritePrivateProfileString("TEACH_PARA", key, dat, file_path);
			}

			key.Empty();
				key.Format("TEACH_SECOND_INSP_THRESH_HOLD_%d", k);
				dat.Empty();
				dat.Format("%.3f", m_TeachPara.TeachSecondInspThreshHold[k]);
				WritePrivateProfileString("TEACH_PARA", key, dat, file_path);

			for(int i = 0; i < m_TeachPara.TeachGrabCnt; i++)
			{
				key.Empty();
				key.Format("TEACH_REGION_CNT_%d_%d", i, k);
				dat.Empty();
				dat.Format("%d", m_TeachPara.TeachRegionCnt[i][k]);
				WritePrivateProfileString("TEACH_PARA", key, dat, file_path);

				for(int j = 0; j < m_TeachPara.TeachRegionCnt[i][k] * 3; j++)
				{
					key.Empty();
					key.Format("TEACH_ALIGN_%d_%d_%d", i, k, j);
					dat.Empty();
					dat.Format("%d", m_TeachPara.AlignPara.bCreatMdID[i][k][j]);
					WritePrivateProfileString("TEACH_PARA", key, dat, file_path);

					key.Empty();
					key.Format(("TEACH_REGION_[%03d][%03d][%03d]_LEFT"), i, k, j);
					dat.Empty();
					dat.Format("%f", m_TeachPara.TeachRegion[i][k][j].GetLeft());
					WritePrivateProfileString("TEACH_PARA", key, dat, file_path);

					key.Empty();
					key.Format(("TEACH_REGION_[%03d][%03d][%03d]_TOP"), i, k, j);
					dat.Empty();
					dat.Format("%f", m_TeachPara.TeachRegion[i][k][j].GetTop());
					WritePrivateProfileString("TEACH_PARA", key, dat, file_path);

					key.Empty();
					key.Format(("TEACH_REGION_[%03d][%03d][%03d]_RIGHT"), i, k, j);
					dat.Empty();
					dat.Format("%f", m_TeachPara.TeachRegion[i][k][j].GetRight());
					WritePrivateProfileString("TEACH_PARA", key, dat, file_path);

					key.Empty();
					key.Format(("TEACH_REGION_[%03d][%03d][%03d]_BOTTOM"), i, k, j);
					dat.Empty();
					dat.Format("%f", m_TeachPara.TeachRegion[i][k][j].GetBottom());
					WritePrivateProfileString("TEACH_PARA", key, dat, file_path);
				}
			}
		}
	}

	strLog.Format(_T("SaveTeachPara End!!"));
	WriteLog(strLog);

	return TRUE;
}

void CDialogTeach::OnNMCustomdrawSliderLight0(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMCUSTOMDRAW pNMCD = reinterpret_cast<LPNMCUSTOMDRAW>(pNMHDR);
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.

	m_edit_nLight_0 = m_slider_Light_0.GetPos();
	
	UpdateData(FALSE);

	SetLight(LIGHT_1_CH, m_edit_nLight_0, LIGHT_COM_PORT);

	*pResult = 0;
}


void CDialogTeach::OnNMCustomdrawSliderLight1(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMCUSTOMDRAW pNMCD = reinterpret_cast<LPNMCUSTOMDRAW>(pNMHDR);
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.

	m_edit_nLight_1 = m_slider_Light_1.GetPos();
	
	UpdateData(FALSE);

	SetLight(LIGHT_2_CH, m_edit_nLight_1, LIGHT_COM_PORT);

	*pResult = 0;
}

BYTE CDialogTeach::byValue2Hexa_High(BYTE byData)
{
	BYTE byValue1 = (byData >> 4);

	return byValue2NibbleAscii(byValue1);
}

BYTE CDialogTeach::byValue2Hexa_Low(BYTE byData)
{
	BYTE byValue1 = (byData & 0x0f);

	return byValue2NibbleAscii(byValue1);
}

BYTE CDialogTeach::byValue2NibbleAscii(BYTE byData)
{
	if((0 <= byData) && (byData <=9))
	{
		return byData + '0';
	}

	if(byData == 10)	byData = 'A';
	if(byData == 11)	byData = 'B';
	if(byData == 12)	byData = 'C';
	if(byData == 13)	byData = 'D';
	if(byData == 14)	byData = 'E';
	if(byData == 15)	byData = 'F';
	return byData;

	return '*';
}

void CDialogTeach::SetLight(int ch, int var, int grab_idx)
{
	/*
#ifdef LIGHT
	char packet[100];
#endif
	*/
	CString strLog;
	strLog.Format(_T("SetLight Start, GrabIdx %d, Ch %d, Var %d!!"), grab_idx, ch, var);
	WriteLog(strLog);

	BYTE bySndData[L_LIGHT_PACKET_SIZE];
	int sendcnt = L_LIGHT_PACKET_SIZE - 1;
	CString strCh;
	char light[4];
	strCh.Format("%1d", ch);


	sprintf(light, "%03d", var);

	bySndData[0] = 0x02;
	bySndData[1] = 'C';
	bySndData[2] = 'H';
	bySndData[3] = strCh.GetAt(0);
	bySndData[4] = 'S';
	bySndData[5] = light[0];
	bySndData[6] = light[1];
	bySndData[7] = light[2];
	bySndData[8] = 0x03;
	bySndData[9] = NULL;
#ifdef LIGHT
	m_cSerial.Write(bySndData, sendcnt);
#endif

	strLog.Format(_T("SetLight End, GrabIdx %d, Ch %d, Var %d!!"), grab_idx, ch, var);
	WriteLog(strLog);
}

void CDialogTeach::OnOffLight(BOOL LightOn, int grab_idx)
{
	/*
#ifdef LIGHT
	char packet[100];
#endif
	*/
	CString strLog;
	strLog.Format(_T("OnOffLight Start, GrabIdx %d!!"), grab_idx);
	WriteLog(strLog);

	BYTE bySndData[L_LIGHT_PACKET_SIZE];
	int sendcnt = L_LIGHT_PACKET_SIZE - 1;
	CString strCh;
	int var;
	char light[4];

	for(int ch = 0; ch < MAX_LIGHT; ch++)
	{
		strCh.Format("%1d", ch+1);

		if(LightOn)
		{
			var = m_TeachPara.TeachLight[grab_idx][ch];
		}
		else
		{
			var = 0;
		}

		sprintf(light, "%03d", var);

		bySndData[0] = 0x02;
		bySndData[1] = 'C';
		bySndData[2] = 'H';
		bySndData[3] = strCh.GetAt(0);
		bySndData[4] = 'S';
		bySndData[5] = light[0];
		bySndData[6] = light[1];
		bySndData[7] = light[2];
		bySndData[8] = 0x03;
		bySndData[9] = NULL;
#ifdef LIGHT
		m_cSerial.Write(bySndData, sendcnt);
#endif
		Sleep(100);
	}
	strLog.Format(_T("OnOffLight End, GrabIdx %d!!"), grab_idx);
	WriteLog(strLog);
}

int CDialogTeach::GetImageGrabCnt()
{
	return m_TeachPara.TeachGrabCnt;
}

BOOL CDialogTeach::WriteLog(CString strLog)
{
	CLED_DisplayInspectDlg *pdlg = ((CLED_DisplayInspectDlg *)AfxGetApp()->m_pMainWnd);
	BOOL rslt = FALSE;
	rslt = pdlg->WriteLog(strLog, TYPE_LOG_TEACH);

	return rslt;
}

int CDialogTeach::GetDeviceIdxX()
{
	return m_edit_nMoveCntX - 1;
}

int CDialogTeach::GetDeviceIdxY()
{
	return m_edit_nMoveCntY - 1;
}

int CDialogTeach::GetGrabIdx()
{
	return m_edit_nSelectImageIdx - 1;
}

void CDialogTeach::ClickBtnenhctrlSelectImageIdxLeft()
{
	// TODO: 여기에 메시지 처리기 코드를 추가합니다.
	CLED_DisplayInspectDlg *pdlg = ((CLED_DisplayInspectDlg *)AfxGetApp()->m_pMainWnd);
	for(int i = 0; i < MAX_GRAB_CNT; i++)
	{
		for(int j = 0; j < MAX_INSP_GRAB_CNT; j++)
		{
			for(int k = 0; k < TRACKER_MAX_NUM; k++)
			{
				pdlg->m_dlgImageRect->m_Tracker[i][j][k].SetActive(FALSE);
			}
		}
	}

	m_edit_nSelectImageIdx--;

	if(m_edit_nSelectImageIdx < 1)
	{
		m_edit_nSelectImageIdx = 1;
	}

	UpdateData(FALSE);

	for(int i = 0; i < m_TeachPara.TeachRegionCnt[m_edit_nSelectImageIdx - 1][m_edit_nSelectInspGrabIdx - 1] * 3; i++)
	{
		if(m_edit_nSelectInspGrabIdx - 1 > 0) //20160923 sc
		{
			if(i == 2 || i % 3 == 2)
			{
				pdlg->m_dlgImageRect->m_Tracker[m_edit_nSelectImageIdx - 1][m_edit_nSelectInspGrabIdx - 1][i].SetActive(TRUE);
			}
		}
		else
		{
			pdlg->m_dlgImageRect->m_Tracker[m_edit_nSelectImageIdx - 1][m_edit_nSelectInspGrabIdx - 1][i].SetActive(TRUE);
		}
	}

	pdlg->m_dlgImageRect->SetReDraw();

	pdlg->m_dlgImageRect->SetInsIdx(m_edit_nSelectImageIdx);
}


void CDialogTeach::ClickBtnenhctrlSelectImageIdxRight()
{
	// TODO: 여기에 메시지 처리기 코드를 추가합니다.
	CLED_DisplayInspectDlg *pdlg = ((CLED_DisplayInspectDlg *)AfxGetApp()->m_pMainWnd);
	for(int i = 0; i < MAX_GRAB_CNT; i++)
	{
		for(int j = 0; j < MAX_INSP_GRAB_CNT; j++)
		{
			for(int k = 0; k < TRACKER_MAX_NUM; k++)
			{
				pdlg->m_dlgImageRect->m_Tracker[i][j][k].SetActive(FALSE);
			}
		}
	}

	m_edit_nSelectImageIdx++;

	if(m_edit_nSelectImageIdx > m_TeachPara.TeachGrabCnt)
	{
		m_edit_nSelectImageIdx = m_TeachPara.TeachGrabCnt;
	}

	UpdateData(FALSE);

	for(int i = 0; i < m_TeachPara.TeachRegionCnt[m_edit_nSelectImageIdx - 1][m_edit_nSelectInspGrabIdx - 1] * 3; i++)
	{
		if(m_edit_nSelectInspGrabIdx - 1 > 0) //20160923 sc
		{
			if(i == 2 || i % 3 == 2)
			{
				pdlg->m_dlgImageRect->m_Tracker[m_edit_nSelectImageIdx - 1][m_edit_nSelectInspGrabIdx - 1][i].SetActive(TRUE);
			}
		}
		else
		{
			pdlg->m_dlgImageRect->m_Tracker[m_edit_nSelectImageIdx - 1][m_edit_nSelectInspGrabIdx - 1][i].SetActive(TRUE);
		}
	}

	pdlg->m_dlgImageRect->SetReDraw();

	pdlg->m_dlgImageRect->SetInsIdx(m_edit_nSelectImageIdx);
}


void CDialogTeach::ClickBtnenhctrlCurentPosSet()
{
	// TODO: 여기에 메시지 처리기 코드를 추가합니다.
	CLED_DisplayInspectDlg *pdlg = ((CLED_DisplayInspectDlg *)AfxGetApp()->m_pMainWnd);
	long pos_x, pos_y, pos_z;

	pos_x = pdlg->m_dlgManual->GetEZCurPos(MOTION_X);// / pdlg->m_dlgManual->GetManualPara().ManualPulseToMM[MOTION_X];
	pos_y = pdlg->m_dlgManual->GetEZCurPos(MOTION_Y);// / pdlg->m_dlgManual->GetManualPara().ManualPulseToMM[MOTION_Y];
	pos_z = pdlg->m_dlgManual->GetEZCurPos(MOTION_Z);// / pdlg->m_dlgManual->GetManualPara().ManualPulseToMM[MOTION_Z];

	m_edit_dFirstDevicePosX = pos_x;
	m_edit_dFirstDevicePosY = pos_y;
	m_edit_dFirstDevicePosZ = pos_z;

	CString strLog;
	strLog.Format(_T("Motion Num 0, First Device Teach Pos %d!!"), pos_x);
	WriteLog(strLog);
	strLog.Format(_T("Motion Num 1, First Device Teach Pos %d!!"), pos_y);
	WriteLog(strLog);
	strLog.Format(_T("Motion Num 2, First Device Teach Pos %d!!"), pos_z);
	WriteLog(strLog);

	UpdateData(FALSE);
}


void CDialogTeach::ClickBtnenhctrlPosMove()
{
	// TODO: 여기에 메시지 처리기 코드를 추가합니다.
	CLED_DisplayInspectDlg *pdlg = ((CLED_DisplayInspectDlg *)AfxGetApp()->m_pMainWnd);
	long pos_x, pos_y, pos_z;
	CString strLog;
	UpdateData(TRUE);

	pos_x = (long)m_edit_dFirstDevicePosX;// * pdlg->m_dlgManual->GetManualPara().ManualPulseToMM[MOTION_X];
	pos_y = (long)m_edit_dFirstDevicePosY;// * pdlg->m_dlgManual->GetManualPara().ManualPulseToMM[MOTION_Y];
	pos_z = (long)m_edit_dFirstDevicePosZ;// * pdlg->m_dlgManual->GetManualPara().ManualPulseToMM[MOTION_Z];

	if(!pdlg->m_dlgManual->SetEzPosMove(MOTION_X, pos_x))
	{
		strLog.Format(_T("Motion Num %d, ClickBtnenhctrlPosMove() Move Fail!!"), MOTION_X);
		WriteLog(strLog);
	}

	if(!pdlg->m_dlgManual->SetEzPosMove(MOTION_Y, pos_y))
	{
		strLog.Format(_T("Motion Num %d, ClickBtnenhctrlPosMove() Move Fail!!"), MOTION_Y);
		WriteLog(strLog);
	}

	if(!pdlg->m_dlgManual->SetEzPosMove(MOTION_Z, pos_z))
	{
		strLog.Format(_T("Motion Num %d, ClickBtnenhctrlPosMove() Move Fail!!"), MOTION_Z);
		WriteLog(strLog);
	}
}


void CDialogTeach::ClickBtnenhctrlLightSet1()
{
	// TODO: 여기에 메시지 처리기 코드를 추가합니다.	
	UpdateData(TRUE);

	SetLight(LIGHT_1_CH, m_edit_nLight_0, LIGHT_COM_PORT);
	m_slider_Light_0.SetPos(m_edit_nLight_0);
}


void CDialogTeach::ClickBtnenhctrlLightSet2()
{
	// TODO: 여기에 메시지 처리기 코드를 추가합니다.
	UpdateData(TRUE);

	SetLight(LIGHT_2_CH, m_edit_nLight_1, LIGHT_COM_PORT);
	m_slider_Light_1.SetPos(m_edit_nLight_1);
}


void CDialogTeach::ClickBtnenhctrlPosMove2()
{
	// TODO: 여기에 메시지 처리기 코드를 추가합니다.
	CLED_DisplayInspectDlg *pdlg = ((CLED_DisplayInspectDlg *)AfxGetApp()->m_pMainWnd);
	long pos_x, pos_y, pos_z;
	int temp_x, temp_y;
	CString strLog;
	int temp_x1, temp_y1, temp_img_idx;
	temp_x1 = m_nBeforeX;
	temp_y1 = m_nBeforeY;
	temp_img_idx = m_nBdforeImgIdx;
	UpdateData(TRUE);

	pos_x = GetTeachMovePos(MOTION_X, m_edit_nMoveCntX - 1, m_edit_nMoveCntY - 1, m_edit_nSelectImageIdx - 1);// * pdlg->m_dlgManual->GetManualPara().ManualPulseToMM[MOTION_X];
	pos_y = GetTeachMovePos(MOTION_Y, m_edit_nMoveCntX - 1, m_edit_nMoveCntY - 1, m_edit_nSelectImageIdx - 1);// * pdlg->m_dlgManual->GetManualPara().ManualPulseToMM[MOTION_Y];
	pos_z = GetTeachMovePos(MOTION_Z, m_edit_nMoveCntX - 1, m_edit_nMoveCntY - 1, m_edit_nSelectImageIdx - 1);// * pdlg->m_dlgManual->GetManualPara().ManualPulseToMM[MOTION_Z];

	if(!pdlg->m_dlgManual->SetEzPosMove(MOTION_X, pos_x))
	{
		strLog.Format(_T("Motion Num %d, ClickBtnenhctrlPosMove2() Move Fail!!"), MOTION_X);
		WriteLog(strLog);
	}

	if(!pdlg->m_dlgManual->SetEzPosMove(MOTION_Y, pos_y))
	{
		strLog.Format(_T("Motion Num %d, ClickBtnenhctrlPosMove2() Move Fail!!"), MOTION_Y);
		WriteLog(strLog);
	}

	if(!pdlg->m_dlgManual->SetEzPosMove(MOTION_Z, pos_z))
	{
		strLog.Format(_T("Motion Num %d, ClickBtnenhctrlPosMove2() Move Fail!!"), MOTION_Z);
		WriteLog(strLog);
	}

	strLog.Format(_T("Motion Move, ClickBtnenhctrlPosMove2() Move Num 0 : %d, Num 1: %d, NUm : %d!!"), pos_x, pos_y, pos_z);
	WriteLog(strLog);

	//20161130 nht 수동으로 위치 이동시 Grid에 표시

	for(int i = 0; i < GetTeachPara().TeachOneGrabInspCnt; i++)
	{
		pdlg->m_grid.SetGridOkNg_LED(10, (temp_x1 * GetTeachPara().TeachGrabCnt) + (temp_img_idx), (temp_y1 * GetTeachPara().TeachOneGrabInspCnt) + i, i);
		pdlg->m_grid.RedrawCell((temp_x1 * GetTeachPara().TeachGrabCnt) + (temp_img_idx), (temp_y1 * GetTeachPara().TeachOneGrabInspCnt) + i);
		temp_x = ((m_edit_nMoveCntX - 1) * GetTeachPara().TeachGrabCnt) + (m_edit_nSelectImageIdx - 1);
		temp_y = (m_edit_nMoveCntY - 1) * GetTeachPara().TeachOneGrabInspCnt + i;
		pdlg->m_grid.SetGridOkNg_LED(6, temp_x, temp_y, i);
		//pdlg->m_grid.SetGridOkNg_LED(2, temp_x, temp_y, i);
		pdlg->m_grid.RedrawCell(temp_x, temp_y);
	}

	m_nBeforeX = m_edit_nMoveCntX - 1;
	m_nBeforeY = m_edit_nMoveCntY - 1;
	m_nBdforeImgIdx = m_edit_nSelectImageIdx - 1;
}


void CDialogTeach::ClickBtnenhctrlSelectImageIdxLeft2()
{
	// TODO: 여기에 메시지 처리기 코드를 추가합니다.
	m_edit_nMoveCntX--;

	if(m_edit_nMoveCntX < 1)
	{
		m_edit_nMoveCntX = 1;
	}

	UpdateData(FALSE);
}


void CDialogTeach::ClickBtnenhctrlSelectImageIdxRight2()
{
	// TODO: 여기에 메시지 처리기 코드를 추가합니다.
	m_edit_nMoveCntX++;

	if(m_edit_nMoveCntX > m_TeachPara.TeachDeviceCntX)
	{
		m_edit_nMoveCntX = m_TeachPara.TeachDeviceCntX;
	}

	UpdateData(FALSE);
}


void CDialogTeach::ClickBtnenhctrlSelectImageIdxLeft3()
{
	// TODO: 여기에 메시지 처리기 코드를 추가합니다.
	m_edit_nMoveCntY--;

	if(m_edit_nMoveCntY < 1)
	{
		m_edit_nMoveCntY = 1;
	}

	UpdateData(FALSE);
}


void CDialogTeach::ClickBtnenhctrlSelectImageIdxRight3()
{
	// TODO: 여기에 메시지 처리기 코드를 추가합니다.
	m_edit_nMoveCntY++;

	if(m_edit_nMoveCntY > m_TeachPara.TeachDeviceCntY)
	{
		//20161010 ngh
		//m_edit_nMoveCntY = m_TeachPara.TeachArrayCntY;
		m_edit_nMoveCntY = m_TeachPara.TeachDeviceCntY;
	}

	UpdateData(FALSE);
}


void CDialogTeach::ClickBtnenhctrlCurentWaitPosSet()
{
	// TODO: 여기에 메시지 처리기 코드를 추가합니다.
	CLED_DisplayInspectDlg *pdlg = ((CLED_DisplayInspectDlg *)AfxGetApp()->m_pMainWnd);
	long pos_x, pos_y, pos_z;

	pos_x = pdlg->m_dlgManual->GetEZCurPos(MOTION_X);// / pdlg->m_dlgManual->GetManualPara().ManualPulseToMM[MOTION_X];
	pos_y = pdlg->m_dlgManual->GetEZCurPos(MOTION_Y);// / pdlg->m_dlgManual->GetManualPara().ManualPulseToMM[MOTION_Y];
	pos_z = pdlg->m_dlgManual->GetEZCurPos(MOTION_Z);// / pdlg->m_dlgManual->GetManualPara().ManualPulseToMM[MOTION_Z];

	m_edit_dWaitPosX = pos_x;
	m_edit_dWaitPosY = pos_y;
	m_edit_dWaitPosZ = pos_z;

	CString strLog;
	strLog.Format(_T("Motion Num 0, Wait Teach Pos %d!!"), pos_x);
	WriteLog(strLog);
	strLog.Format(_T("Motion Num 1, Wait Teach Pos %d!!"), pos_y);
	WriteLog(strLog);
	strLog.Format(_T("Motion Num 2, Wait Teach Pos %d!!"), pos_z);
	WriteLog(strLog);

	UpdateData(FALSE);
}


void CDialogTeach::ClickBtnenhctrlWaitPosMove()
{
	// TODO: 여기에 메시지 처리기 코드를 추가합니다.
	CLED_DisplayInspectDlg *pdlg = ((CLED_DisplayInspectDlg *)AfxGetApp()->m_pMainWnd);
	long pos_x, pos_y, pos_z;
	CString strLog;
	UpdateData(TRUE);

	pos_x = (long)m_edit_dWaitPosX;// * pdlg->m_dlgManual->GetManualPara().ManualPulseToMM[MOTION_X];
	pos_y = (long)m_edit_dWaitPosY;// * pdlg->m_dlgManual->GetManualPara().ManualPulseToMM[MOTION_Y];
	pos_z = (long)m_edit_dWaitPosZ;// * pdlg->m_dlgManual->GetManualPara().ManualPulseToMM[MOTION_Z];

	if(!pdlg->m_dlgManual->SetEzPosMove(MOTION_X, pos_x))
	{
		strLog.Format(_T("Motion Num %d, ClickBtnenhctrlWaitPosMove() Move Fail!!"), MOTION_X);
		WriteLog(strLog);
	}

	if(!pdlg->m_dlgManual->SetEzPosMove(MOTION_Y, pos_y))
	{
		strLog.Format(_T("Motion Num %d, ClickBtnenhctrlWaitPosMove() Move Fail!!"), MOTION_Y);
		WriteLog(strLog);
	}

	if(!pdlg->m_dlgManual->SetEzPosMove(MOTION_Z, pos_z))
	{
		strLog.Format(_T("Motion Num %d, ClickBtnenhctrlWaitPosMove() Move Fail!!"), MOTION_Z);
		WriteLog(strLog);
	}
}

void CDialogTeach::ClickBtnenhctrlParaLock()
{
	// TODO: 여기에 메시지 처리기 코드를 추가합니다.
	//m_dlgParaLock->ShowWindow(SW_SHOW);  //20160906 sc
	CDialogParameterLock dlgParaLock;  //20160906 sc

	if(dlgParaLock.DoModal())
	{
		if(m_bParaLock)
		{
			m_bParaLock = FALSE;
		}
		else
		{
			m_bParaLock = TRUE;
		}

		SetParameterEnable(m_bParaLock);
	}
}

void CDialogTeach::SetParameterEnable(BOOL isLock)  //20160906 sc
{
	if(!m_bParaLock)
	{
		m_edit_ctrl_para3.EnableWindow(1);
		m_edit_ctrl_para0.EnableWindow(1);
		m_edit_ctrl_para6.EnableWindow(1);
		m_edit_ctrl_para7.EnableWindow(1);
		m_edit_ctrl_para8.EnableWindow(1);
		m_edit_ctrl_para9.EnableWindow(1);
		m_edit_ctrl_para14.EnableWindow(1);
		m_edit_ctrl_para15.EnableWindow(1);
		m_edit_ctrl_para17.EnableWindow(1);
		m_edit_ctrl_para18.EnableWindow(1);
		m_edit_ctrl_para19.EnableWindow(1);
		m_edit_ctrl_para11.EnableWindow(1);
		m_edit_ctrl_para12.EnableWindow(1);
		m_edit_ctrl_para13.EnableWindow(1);
		m_edit_ctrl_para20.EnableWindow(1);
		m_edit_ctrl_para21.EnableWindow(1);
		m_edit_ctrl_para22.EnableWindow(1);
		m_edit_ctrl_para32.EnableWindow(1);
		m_edit_ctrl_para28.EnableWindow(1);
		m_edit_ctrl_para26.EnableWindow(1);
		m_edit_ctrl_para24.EnableWindow(1);
		m_edit_ctrl_para25.EnableWindow(1);
		m_edit_ctrl_para29.EnableWindow(1);
		m_edit_ctrl_para30.EnableWindow(1);
		m_edit_ctrl_para31.EnableWindow(1);
		m_edit_ctrl_para23.EnableWindow(1);
		GetDlgItem(IDC_BTNENHCTRL_CURENT_POS_SET)->EnableWindow(1);
		GetDlgItem(IDC_BTNENHCTRL_CURENT_POS_SET2)->EnableWindow(1);
		GetDlgItem(IDC_BTNENHCTRL_DEVICE_DISTANCE)->EnableWindow(1);
		GetDlgItem(IDC_BTNENHCTRL_GRAB_DISTANCE)->EnableWindow(1);
		GetDlgItem(IDC_BTNENHCTRL_REGION_COPY)->EnableWindow(1);
		GetDlgItem(IDC_BTNENHCTRL_INSP_ALIGN_TEACH)->EnableWindow(1);
		GetDlgItem(IDC_BTNENHCTRL_INSP_TEACH)->EnableWindow(1);

		m_edit_ctrl_para00.EnableWindow(1);

		m_btn_para_lock.SetValue(1);
		m_btn_para_lock.SetCaption("Parameter Lock");
	}
	else
	{
		m_edit_ctrl_para3.EnableWindow(0);
		m_edit_ctrl_para0.EnableWindow(0);
		m_edit_ctrl_para6.EnableWindow(0);
		m_edit_ctrl_para7.EnableWindow(0);
		m_edit_ctrl_para8.EnableWindow(0);
		m_edit_ctrl_para9.EnableWindow(0);
		m_edit_ctrl_para14.EnableWindow(0);
		m_edit_ctrl_para15.EnableWindow(0);
		m_edit_ctrl_para17.EnableWindow(0);
		m_edit_ctrl_para18.EnableWindow(0);
		m_edit_ctrl_para19.EnableWindow(0);
		m_edit_ctrl_para11.EnableWindow(0);
		m_edit_ctrl_para12.EnableWindow(0);
		m_edit_ctrl_para13.EnableWindow(0);
		m_edit_ctrl_para20.EnableWindow(0);
		m_edit_ctrl_para21.EnableWindow(0);
		m_edit_ctrl_para22.EnableWindow(0);
		m_edit_ctrl_para32.EnableWindow(0);
		m_edit_ctrl_para28.EnableWindow(0);
		m_edit_ctrl_para26.EnableWindow(0);
		m_edit_ctrl_para24.EnableWindow(0);
		m_edit_ctrl_para25.EnableWindow(0);
		m_edit_ctrl_para29.EnableWindow(0);
		m_edit_ctrl_para30.EnableWindow(0);
		m_edit_ctrl_para31.EnableWindow(0);
		m_edit_ctrl_para23.EnableWindow(0);
		GetDlgItem(IDC_BTNENHCTRL_CURENT_POS_SET)->EnableWindow(0);
		GetDlgItem(IDC_BTNENHCTRL_CURENT_POS_SET2)->EnableWindow(0);
		GetDlgItem(IDC_BTNENHCTRL_DEVICE_DISTANCE)->EnableWindow(0);
		GetDlgItem(IDC_BTNENHCTRL_GRAB_DISTANCE)->EnableWindow(0);
		GetDlgItem(IDC_BTNENHCTRL_REGION_COPY)->EnableWindow(0);
		GetDlgItem(IDC_BTNENHCTRL_INSP_ALIGN_TEACH)->EnableWindow(0);
		GetDlgItem(IDC_BTNENHCTRL_INSP_TEACH)->EnableWindow(0);

		m_edit_ctrl_para00.EnableWindow(0);

		m_btn_para_lock.SetValue(0);
		m_btn_para_lock.SetCaption("Parameter UnLock");
	}
}

void CDialogTeach::ClickBtnenhctrlInspTeach()
{
	// TODO: 여기에 메시지 처리기 코드를 추가합니다.
	CLED_DisplayInspectDlg *pdlg = ((CLED_DisplayInspectDlg *)AfxGetApp()->m_pMainWnd);
	UpdateData(TRUE);
#ifdef CAMERA
	MbufCopy(*pdlg->m_MilManage.GetMilDispBuf(MAIN_DISP), *pdlg->m_MilManage.GetMilInspBuf(0, m_edit_nSelectInspGrabIdx - 1));
#endif
	if(m_edit_nSelectInspGrabIdx == 1)
	{
		pdlg->Inspection(m_edit_nMoveCntX - 1, m_edit_nMoveCntY - 1, m_edit_nSelectImageIdx - 1, m_edit_nSelectInspGrabIdx - 1, INSPECTION_TEACH, 0);
	}
	else if(m_edit_nSelectInspGrabIdx == 2)
	{
		//20170102 ngh
		//pdlg->Inspection(m_edit_nMoveCntX - 1, m_edit_nMoveCntY - 1, m_edit_nSelectImageIdx - 1, m_edit_nSelectInspGrabIdx - 1, INSPECTION_MASK_SET, 0);
		pdlg->Inspection(m_edit_nMoveCntX - 1, m_edit_nMoveCntY - 1, m_edit_nSelectImageIdx - 1, m_edit_nSelectInspGrabIdx - 1, INSPECTION_TEACH, 0);
	}
	else
	{
		pdlg->Inspection(m_edit_nMoveCntX - 1, m_edit_nMoveCntY - 1, m_edit_nSelectImageIdx - 1, 1, INSPECTION_MASK_SET, 0);
		pdlg->Inspection(m_edit_nMoveCntX - 1, m_edit_nMoveCntY - 1, m_edit_nSelectImageIdx - 1, m_edit_nSelectInspGrabIdx - 1, INSPECTION_EDGE2, 0);
	}
	//InspectionAlignTeaching(m_edit_nSelectImageIdx - 1, m_edit_nSelectInspGrabIdx - 1);
}

int CDialogTeach::GetInspGrabIdx()
{
	return m_edit_nSelectInspGrabIdx - 1;
}

void CDialogTeach::ClickBtnenhctrlSelectImageIdxLeft4()
{
	// TODO: 여기에 메시지 처리기 코드를 추가합니다.
	CLED_DisplayInspectDlg *pdlg = ((CLED_DisplayInspectDlg *)AfxGetApp()->m_pMainWnd);
	//][MAX_INSP_GRAB_CNT][TRACKER_MAX_NUM
	#ifdef CAMERA
	MdispControl(*pdlg->m_MilManage.GetMilDisplay(MAIN_DISP), M_OVERLAY_CLEAR, M_DEFAULT); //20161014 sc 화면 클리어 추가
	#endif
	

	for(int i = 0; i < MAX_GRAB_CNT; i++)
	{
		for(int j = 0; j < MAX_INSP_GRAB_CNT; j++)
		{
			for(int k = 0; k < TRACKER_MAX_NUM; k++)
			{
				pdlg->m_dlgImageRect->m_Tracker[i][j][k].SetActive(FALSE);
			}
		}
	}


	/*for(int i = 0; i < m_TeachPara.TeachRegionCnt[m_edit_nSelectImageIdx - 1][m_edit_nSelectInspGrabIdx - 1] + 1; i++)
	{
		pdlg->m_dlgImageRect->m_Tracker[m_edit_nSelectImageIdx - 1][m_edit_nSelectInspGrabIdx - 1][i].SetActive(FALSE);
	}*/

	m_edit_nSelectInspGrabIdx--;

	if(m_edit_nSelectInspGrabIdx < 1)
	{
		m_edit_nSelectInspGrabIdx = 1;
	}

	Update(FALSE);
	UpdateData(FALSE);

	for(int i = 0; i < m_TeachPara.TeachRegionCnt[m_edit_nSelectImageIdx - 1][m_edit_nSelectInspGrabIdx - 1] * 3; i++)
	{
		//pdlg->m_dlgImageRect->m_Tracker[m_edit_nSelectImageIdx - 1][m_edit_nSelectInspGrabIdx - 1][i].SetActive(TRUE);
		if(m_edit_nSelectInspGrabIdx - 1 > 0) //20160923 sc
		{
			if(i == 2 || i % 3 == 2)
			{
				pdlg->m_dlgImageRect->m_Tracker[m_edit_nSelectImageIdx - 1][m_edit_nSelectInspGrabIdx - 1][i].SetActive(TRUE);
			}
			
		}
		else
		{
			pdlg->m_dlgImageRect->m_Tracker[m_edit_nSelectImageIdx - 1][m_edit_nSelectInspGrabIdx - 1][i].SetActive(TRUE);
		}
	}

	pdlg->m_dlgImageRect->SetReDraw();

	pdlg->m_dlgImageRect->SetGrabIdx(m_edit_nSelectInspGrabIdx);
}


void CDialogTeach::ClickBtnenhctrlSelectImageIdxRight4()
{
	// TODO: 여기에 메시지 처리기 코드를 추가합니다.
	CLED_DisplayInspectDlg *pdlg = ((CLED_DisplayInspectDlg *)AfxGetApp()->m_pMainWnd);
	#ifdef CAMERA
	MdispControl(*pdlg->m_MilManage.GetMilDisplay(MAIN_DISP), M_OVERLAY_CLEAR, M_DEFAULT); //20161014 sc 화면 클리어 추가
	#endif
	

	for(int i = 0; i < MAX_GRAB_CNT; i++)
	{
		for(int j = 0; j < MAX_INSP_GRAB_CNT; j++)
		{
			for(int k = 0; k < TRACKER_MAX_NUM; k++)
			{
				pdlg->m_dlgImageRect->m_Tracker[i][j][k].SetActive(FALSE);
			}
		}
	}

	/*for(int i = 0; i < m_TeachPara.TeachRegionCnt[m_edit_nSelectImageIdx - 1][m_edit_nSelectInspGrabIdx - 1] + 1; i++)
	{
		pdlg->m_dlgImageRect->m_Tracker[m_edit_nSelectImageIdx - 1][m_edit_nSelectInspGrabIdx - 1][i].SetActive(FALSE);
	}*/

	m_edit_nSelectInspGrabIdx++;

	if(m_edit_nSelectInspGrabIdx > m_edit_nInspGrabIdx)
	{
		m_edit_nSelectInspGrabIdx = m_edit_nInspGrabIdx;
	}
	Update(FALSE);
	UpdateData(FALSE);

	for(int i = 0; i < m_TeachPara.TeachRegionCnt[m_edit_nSelectImageIdx - 1][m_edit_nSelectInspGrabIdx - 1] * 3; i++)
	{
		//pdlg->m_dlgImageRect->m_Tracker[m_edit_nSelectImageIdx - 1][m_edit_nSelectInspGrabIdx - 1][i].SetActive(TRUE);
		if(m_edit_nSelectInspGrabIdx - 1 > 0) //20160923 sc
		{
			if(i == 2 || (i % 3 == 2))
			{
				pdlg->m_dlgImageRect->m_Tracker[m_edit_nSelectImageIdx - 1][m_edit_nSelectInspGrabIdx - 1][i].SetActive(TRUE);
			}
			
		}
		else
		{
			pdlg->m_dlgImageRect->m_Tracker[m_edit_nSelectImageIdx - 1][m_edit_nSelectInspGrabIdx - 1][i].SetActive(TRUE);
		}
	}

	pdlg->m_dlgImageRect->SetReDraw();

	pdlg->m_dlgImageRect->SetGrabIdx(m_edit_nSelectInspGrabIdx);
}

void CDialogTeach::ClickBtnenhctrlInspAlignTeach()
{
	// TODO: 여기에 메시지 처리기 코드를 추가합니다.
	CLED_DisplayInspectDlg *pdlg = ((CLED_DisplayInspectDlg *)AfxGetApp()->m_pMainWnd);
	UpdateData(TRUE);
	pdlg->Inspection(GetDeviceIdxX(), GetDeviceIdxY(), GetGrabIdx(), GetInspGrabIdx(), 0, 0);
}


void CDialogTeach::ClickBtnenhctrlInspAlignTest()
{
	// TODO: 여기에 메시지 처리기 코드를 추가합니다.
	CLED_DisplayInspectDlg *pdlg = ((CLED_DisplayInspectDlg *)AfxGetApp()->m_pMainWnd);
	UpdateData(TRUE);
	pdlg->Inspection(GetDeviceIdxX(), GetDeviceIdxY(), GetGrabIdx(), GetInspGrabIdx(), 2, 0);
	if(m_bEdgeInsp)
	{
		pdlg->Inspection(GetDeviceIdxX(), GetDeviceIdxY(), GetGrabIdx(), GetInspGrabIdx(), 10, 0);
	}
	else
	{
		pdlg->Inspection(GetDeviceIdxX(), GetDeviceIdxY(), GetGrabIdx(), GetInspGrabIdx(), 20, 0);
	}
}


void CDialogTeach::ClickBtnenhctrlGrabDistance()
{
	// TODO: 여기에 메시지 처리기 코드를 추가합니다.
	CLED_DisplayInspectDlg *pdlg = ((CLED_DisplayInspectDlg *)AfxGetApp()->m_pMainWnd);
	long curPos = 0;
	long distancePos = 0;
	UpdateData(TRUE);

	curPos = pdlg->m_dlgManual->GetEZCurPos(MOTION_X);
	distancePos = curPos - (long)m_edit_dFirstDevicePosX;
	m_edit_dGrabDistanceX = distancePos;

	curPos = pdlg->m_dlgManual->GetEZCurPos(MOTION_Y);
	distancePos = curPos - (long)m_edit_dFirstDevicePosY;
	m_edit_dGrabDistanceY = distancePos;

	UpdateData(FALSE);
}


void CDialogTeach::ClickBtnenhctrlDeviceDistance()
{
	// TODO: 여기에 메시지 처리기 코드를 추가합니다.
	CLED_DisplayInspectDlg *pdlg = ((CLED_DisplayInspectDlg *)AfxGetApp()->m_pMainWnd);
	long curPos = 0;
	long distancePos = 0;
	UpdateData(TRUE);

	curPos = pdlg->m_dlgManual->GetEZCurPos(MOTION_X);
	distancePos = curPos - (long)m_edit_dFirstDevicePosX;
	m_edit_nDeviceDistanceX = distancePos;

	curPos = pdlg->m_dlgManual->GetEZCurPos(MOTION_Y);
	distancePos = curPos - (long)m_edit_dFirstDevicePosY;
	m_edit_nDeviceDistanceY = distancePos;

	UpdateData(FALSE);
}


void CDialogTeach::ClickBtnenhctrlInspTeach2()
{
	// TODO: 여기에 메시지 처리기 코드를 추가합니다.
	CLED_DisplayInspectDlg *pdlg = ((CLED_DisplayInspectDlg *)AfxGetApp()->m_pMainWnd);
	UpdateData(TRUE);
	pdlg->Inspection(0,0,0,0,INSPECTION_EDGE, 0);
}


void CDialogTeach::ClickBtnenhctrlRegionCopy()
{
	// TODO: 여기에 메시지 처리기 코드를 추가합니다.
	CLED_DisplayInspectDlg *pdlg = ((CLED_DisplayInspectDlg *)AfxGetApp()->m_pMainWnd);
	int nRegionCnt = m_TeachPara.TeachRegionCnt[m_edit_nSelectImageIdx - 1][m_edit_nSelectInspGrabIdx - 1];
	double l,t,r,b;
	double left,top,right,bottom;

	for(int i = 0; i < nRegionCnt * 3; i++)
	{
		if(((m_edit_nSelectInspGrabIdx - 1) == 0) && (m_edit_nInspRegionCnt > 1))
		{
			if((i != 0 && (i % 3) == 0) || (i != 1 && (i % 3) == 1) || (i != 2 && (i % 3) == 2))
			{
				pdlg->m_dlgImageRect->m_Tracker[m_edit_nSelectImageIdx - 1][m_edit_nSelectInspGrabIdx - 1][i].GetZoom(&l, &t, &r, &b);
				pdlg->m_dlgImageRect->m_Tracker[m_edit_nSelectImageIdx - 1][m_edit_nSelectInspGrabIdx - 1][i].GetRect(&left, &top, &right, &bottom);

				//pdlg->m_dlgImageRect->m_Tracker[m_edit_nSelectImageIdx - 1][m_edit_nSelectInspGrabIdx - 1][i].SetRect(
				//(left) - (l * pdlg->m_dlgImageRect->m_dZoomRatio),
				//(top) - (t * pdlg->m_dlgImageRect->m_dZoomRatio),
				//(((m_TeachPara.TeachRegion[m_edit_nSelectImageIdx - 1][m_edit_nSelectInspGrabIdx - 1][i-3].GetRight() - m_TeachPara.TeachRegion[m_edit_nSelectImageIdx - 1][m_edit_nSelectInspGrabIdx - 1][i-3].GetLeft()) + m_TeachPara.TeachRegion[m_edit_nSelectImageIdx - 1][m_edit_nSelectInspGrabIdx - 1][i].GetLeft()) * pdlg->m_dlgImageRect->m_dZoomRatio) - (l * pdlg->m_dlgImageRect->m_dZoomRatio),
				//(((m_TeachPara.TeachRegion[m_edit_nSelectImageIdx - 1][m_edit_nSelectInspGrabIdx - 1][i-3].GetBottom() - m_TeachPara.TeachRegion[m_edit_nSelectImageIdx - 1][m_edit_nSelectInspGrabIdx - 1][i-3].GetTop()) + m_TeachPara.TeachRegion[m_edit_nSelectImageIdx - 1][m_edit_nSelectInspGrabIdx - 1][i].GetTop()) * pdlg->m_dlgImageRect->m_dZoomRatio) - (t * pdlg->m_dlgImageRect->m_dZoomRatio));
				//20161019 ngh
				pdlg->m_dlgImageRect->m_Tracker[m_edit_nSelectImageIdx - 1][m_edit_nSelectInspGrabIdx - 1][i].SetRect(
				(left) - (l * pdlg->m_dlgImageRect->m_dZoomRatio),
				(top) - (t * pdlg->m_dlgImageRect->m_dZoomRatio),
				((left) - (l * pdlg->m_dlgImageRect->m_dZoomRatio)) + (m_TeachPara.TeachRegion[m_edit_nSelectImageIdx - 1][m_edit_nSelectInspGrabIdx - 1][i-3].GetRight() - m_TeachPara.TeachRegion[m_edit_nSelectImageIdx - 1][m_edit_nSelectInspGrabIdx - 1][i-3].GetLeft()) * pdlg->m_dlgImageRect->m_dZoomRatio,
				((top) - (t * pdlg->m_dlgImageRect->m_dZoomRatio)) + (m_TeachPara.TeachRegion[m_edit_nSelectImageIdx - 1][m_edit_nSelectInspGrabIdx - 1][i-3].GetBottom() - m_TeachPara.TeachRegion[m_edit_nSelectImageIdx - 1][m_edit_nSelectInspGrabIdx - 1][i-3].GetTop()) * pdlg->m_dlgImageRect->m_dZoomRatio);
			}
	/*		else if(i != 1 && (i % 3) == 1)
			{
				pdlg->m_dlgImageRect->m_Tracker[m_edit_nSelectImageIdx - 1][m_edit_nSelectInspGrabIdx - 1][i].GetZoom(&l, &t, &r, &b);
				pdlg->m_dlgImageRect->m_Tracker[m_edit_nSelectImageIdx - 1][m_edit_nSelectInspGrabIdx - 1][i].SetRect(
				(100 * pdlg->m_dlgImageRect->m_dZoomRatio) - (l * pdlg->m_dlgImageRect->m_dZoomRatio),
				(100 * pdlg->m_dlgImageRect->m_dZoomRatio) - (t * pdlg->m_dlgImageRect->m_dZoomRatio),
				(((m_TeachPara.TeachRegion[m_edit_nSelectImageIdx - 1][m_edit_nSelectInspGrabIdx - 1][i-3].GetRight() - m_TeachPara.TeachRegion[m_edit_nSelectImageIdx - 1][m_edit_nSelectInspGrabIdx - 1][i-3].GetLeft()) + 100) * pdlg->m_dlgImageRect->m_dZoomRatio) - (l * pdlg->m_dlgImageRect->m_dZoomRatio),
				(((m_TeachPara.TeachRegion[m_edit_nSelectImageIdx - 1][m_edit_nSelectInspGrabIdx - 1][i-3].GetBottom() - m_TeachPara.TeachRegion[m_edit_nSelectImageIdx - 1][m_edit_nSelectInspGrabIdx - 1][i-3].GetTop()) + 100) * pdlg->m_dlgImageRect->m_dZoomRatio) - (t * pdlg->m_dlgImageRect->m_dZoomRatio));
			}
			else if(i != 2 && (i % 3) == 2)
			{
				pdlg->m_dlgImageRect->m_Tracker[m_edit_nSelectImageIdx - 1][m_edit_nSelectInspGrabIdx - 1][i].GetZoom(&l, &t, &r, &b);
				pdlg->m_dlgImageRect->m_Tracker[m_edit_nSelectImageIdx - 1][m_edit_nSelectInspGrabIdx - 1][i].SetRect(
				(100 * pdlg->m_dlgImageRect->m_dZoomRatio) - (l * pdlg->m_dlgImageRect->m_dZoomRatio),
				(100 * pdlg->m_dlgImageRect->m_dZoomRatio) - (t * pdlg->m_dlgImageRect->m_dZoomRatio),
				(((m_TeachPara.TeachRegion[m_edit_nSelectImageIdx - 1][m_edit_nSelectInspGrabIdx - 1][i-3].GetRight() - m_TeachPara.TeachRegion[m_edit_nSelectImageIdx - 1][m_edit_nSelectInspGrabIdx - 1][i-3].GetLeft()) + 100) * pdlg->m_dlgImageRect->m_dZoomRatio) - (l * pdlg->m_dlgImageRect->m_dZoomRatio),
				(((m_TeachPara.TeachRegion[m_edit_nSelectImageIdx - 1][m_edit_nSelectInspGrabIdx - 1][i-3].GetBottom() - m_TeachPara.TeachRegion[m_edit_nSelectImageIdx - 1][m_edit_nSelectInspGrabIdx - 1][i-3].GetTop()) + 100) * pdlg->m_dlgImageRect->m_dZoomRatio) - (t * pdlg->m_dlgImageRect->m_dZoomRatio));
			}*/
		}
		else
		{
			if(m_edit_nInspRegionCnt > 1)
			{
				if(i != 2 && (i % 3) == 2)
				{
					pdlg->m_dlgImageRect->m_Tracker[m_edit_nSelectImageIdx - 1][m_edit_nSelectInspGrabIdx - 1][i].GetZoom(&l, &t, &r, &b);
					pdlg->m_dlgImageRect->m_Tracker[m_edit_nSelectImageIdx - 1][m_edit_nSelectInspGrabIdx - 1][i].GetRect(&left, &top, &right, &bottom);

					//pdlg->m_dlgImageRect->m_Tracker[m_edit_nSelectImageIdx - 1][m_edit_nSelectInspGrabIdx - 1][i].SetRect(
					//(left) - (l * pdlg->m_dlgImageRect->m_dZoomRatio),
					//(top) - (t * pdlg->m_dlgImageRect->m_dZoomRatio),
					//(((m_TeachPara.TeachRegion[m_edit_nSelectImageIdx - 1][m_edit_nSelectInspGrabIdx - 1][i-3].GetRight() - m_TeachPara.TeachRegion[m_edit_nSelectImageIdx - 1][m_edit_nSelectInspGrabIdx - 1][i-3].GetLeft()) + m_TeachPara.TeachRegion[m_edit_nSelectImageIdx - 1][m_edit_nSelectInspGrabIdx - 1][i].GetLeft()) * pdlg->m_dlgImageRect->m_dZoomRatio) - (l * pdlg->m_dlgImageRect->m_dZoomRatio),
					//(((m_TeachPara.TeachRegion[m_edit_nSelectImageIdx - 1][m_edit_nSelectInspGrabIdx - 1][i-3].GetBottom() - m_TeachPara.TeachRegion[m_edit_nSelectImageIdx - 1][m_edit_nSelectInspGrabIdx - 1][i-3].GetTop()) + m_TeachPara.TeachRegion[m_edit_nSelectImageIdx - 1][m_edit_nSelectInspGrabIdx - 1][i].GetTop()) * pdlg->m_dlgImageRect->m_dZoomRatio) - (t * pdlg->m_dlgImageRect->m_dZoomRatio));
					//20161019 ngh
					pdlg->m_dlgImageRect->m_Tracker[m_edit_nSelectImageIdx - 1][m_edit_nSelectInspGrabIdx - 1][i].SetRect(
					(left) - (l * pdlg->m_dlgImageRect->m_dZoomRatio),
					(top) - (t * pdlg->m_dlgImageRect->m_dZoomRatio),
					((left) - (l * pdlg->m_dlgImageRect->m_dZoomRatio)) + (m_TeachPara.TeachRegion[m_edit_nSelectImageIdx - 1][m_edit_nSelectInspGrabIdx - 1][i-3].GetRight() - m_TeachPara.TeachRegion[m_edit_nSelectImageIdx - 1][m_edit_nSelectInspGrabIdx - 1][i-3].GetLeft()) * pdlg->m_dlgImageRect->m_dZoomRatio,
					((top) - (t * pdlg->m_dlgImageRect->m_dZoomRatio)) + (m_TeachPara.TeachRegion[m_edit_nSelectImageIdx - 1][m_edit_nSelectInspGrabIdx - 1][i-3].GetBottom() - m_TeachPara.TeachRegion[m_edit_nSelectImageIdx - 1][m_edit_nSelectInspGrabIdx - 1][i-3].GetTop()) * pdlg->m_dlgImageRect->m_dZoomRatio);
				}
			}
		}
	}

	pdlg->m_dlgImageRect->SetReDraw();
}


BOOL CDialogTeach::PreTranslateMessage(MSG* pMsg)
{
	// TODO: 여기에 특수화된 코드를 추가 및/또는 기본 클래스를 호출합니다.
	if(pMsg->message ==WM_KEYDOWN)
	{
		if(pMsg->wParam == VK_RETURN || pMsg->wParam == VK_ESCAPE)
			return TRUE;
	}

	return CDialogEx::PreTranslateMessage(pMsg);
}


//20161011 ht Teach다이얼로그 언어 설정
void CDialogTeach::ChangeLanguage(BOOL StateLanguage)
{
	switch(StateLanguage)
	{
	case 0:
		//Device Setting
		GetDlgItem(IDC_BTNENHCTRL_LABEL_16)->SetWindowText(_T("Device Grab Idx"));
		GetDlgItem(IDC_BTNENHCTRL_LABEL_4)->SetWindowText(_T("Move Idx X"));
		GetDlgItem(IDC_BTNENHCTRL_LABEL_6)->SetWindowText(_T("Move Idx Y"));
		GetDlgItem(IDC_BTNENHCTRL_LABEL_3)->SetWindowText(_T("Device Grab Cnt"));
		GetDlgItem(IDC_BTNENHCTRL_LABEL_5)->SetWindowText(_T("Device X Cnt"));
		GetDlgItem(IDC_BTNENHCTRL_LABEL_7)->SetWindowText(_T("Device Y Cnt"));
		GetDlgItem(IDC_BTNENHCTRL_POS_MOVE2)->SetWindowText(_T("Device Select Pos Move"));
		GetDlgItem(IDC_BTNENHCTRL_LIGHT_SET_1)->SetWindowText(_T("Set"));
		GetDlgItem(IDC_BTNENHCTRL_LIGHT_SET_2)->SetWindowText(_T("Set"));
		GetDlgItem(IDC_BTNENHCTRL_PARA_LOCK)->SetWindowText(_T("Parameter UnLock"));
		GetDlgItem(IDC_BTNENHCTRL_INSP_TEACH2)->SetWindowText(_T("Teach"));

		GetDlgItem(IDC_BTNENHCTRL_LABEL_34)->SetWindowText(_T("Mask Size"));
		GetDlgItem(IDC_BTNENHCTRL_LABEL_35)->SetWindowText(_T("Brightness Difference"));

		//Inspection
		GetDlgItem(IDC_BTNENHCTRL_LABEL_22)->SetWindowText(_T("Insp Spec(um)"));
		GetDlgItem(IDC_BTNENHCTRL_LABEL_23)->SetWindowText(_T("Pixel Size(um)"));

		//20161018 ht 추가기능 리소스 수정
		GetDlgItem(IDC_BTNENHCTRL_LABEL_31)->SetWindowText(_T("Insp Circle Erode Dist"));
		GetDlgItem(IDC_BTNENHCTRL_LABEL_30)->SetWindowText(_T("Insp Circle Area"));

		GetDlgItem(IDC_BTNENHCTRL_LABEL_20)->SetWindowText(_T("Mask Teach ThreshHold"));
		GetDlgItem(IDC_BTNENHCTRL_LABEL_21)->SetWindowText(_T("White Down ThreshHold"));
		GetDlgItem(IDC_BTNENHCTRL_LABEL_25)->SetWindowText(_T("White Insp ThreshHold"));
		GetDlgItem(IDC_BTNENHCTRL_LABEL_26)->SetWindowText(_T("Black Insp ThreshHold"));
		GetDlgItem(IDC_BTNENHCTRL_LABEL_28)->SetWindowText(_T("Insp Grab Idx"));
		GetDlgItem(IDC_BTNENHCTRL_LABEL_0)->SetWindowText(_T("Insp Region Cnt"));
		GetDlgItem(IDC_BTNENHCTRL_LABEL_29)->SetWindowText(_T("One Grab Insp Cnt"));
		GetDlgItem(IDC_BTNENHCTRL_LABEL_27)->SetWindowText(_T("Insp Grab Cnt"));
		GetDlgItem(IDC_BTNENHCTRL_LABEL_1)->SetWindowText(_T("Light 1"));
		GetDlgItem(IDC_BTNENHCTRL_LABEL_2)->SetWindowText(_T("Light 2"));
		GetDlgItem(IDC_BTNENHCTRL_REGION_COPY)->SetWindowText(_T("Region Copy"));
		GetDlgItem(IDC_BTNENHCTRL_INSP_ALIGN_TEACH)->SetWindowText(_T("AlignTeach"));
		GetDlgItem(IDC_BTNENHCTRL_INSP_TEACH)->SetWindowText(_T("Mask Teach"));
		GetDlgItem(IDC_BTNENHCTRL_INSP_ALIGN_TEST)->SetWindowText(_T("TEST"));

		//Pos Teaching
		GetDlgItem(IDC_BTNENHCTRL_LABEL_8)->SetWindowText(_T("Grab Distance X"));
		GetDlgItem(IDC_BTNENHCTRL_LABEL_10)->SetWindowText(_T("Grab Distance Y"));
		GetDlgItem(IDC_BTNENHCTRL_LABEL_14)->SetWindowText(_T("Device Distance X"));
		GetDlgItem(IDC_BTNENHCTRL_LABEL_15)->SetWindowText(_T("Device Distance Y"));
		GetDlgItem(IDC_BTNENHCTRL_LABEL_17)->SetWindowText(_T("Wait Pos X"));
		GetDlgItem(IDC_BTNENHCTRL_LABEL_18)->SetWindowText(_T("Wait Pos Y"));
		GetDlgItem(IDC_BTNENHCTRL_LABEL_19)->SetWindowText(_T("Wait Pos Z"));
		GetDlgItem(IDC_BTNENHCTRL_LABEL_11)->SetWindowText(_T("First Device Pos X"));
		GetDlgItem(IDC_BTNENHCTRL_LABEL_12)->SetWindowText(_T("First Device Pos Y"));
		GetDlgItem(IDC_BTNENHCTRL_LABEL_13)->SetWindowText(_T("First Device Pos Z"));
		GetDlgItem(IDC_BTNENHCTRL_GRAB_DISTANCE)->SetWindowText(_T("Grab Distance Set."));
		GetDlgItem(IDC_BTNENHCTRL_DEVICE_DISTANCE)->SetWindowText(_T("Device Distance Set."));
		GetDlgItem(IDC_BTNENHCTRL_CURENT_POS_SET2)->SetWindowText(_T("Current Pos Set."));
		GetDlgItem(IDC_BTNENHCTRL_POS_MOVE3)->SetWindowText(_T("Wait Pos Move"));
		GetDlgItem(IDC_BTNENHCTRL_CURENT_POS_SET)->SetWindowText(_T("First Pos Set."));
		GetDlgItem(IDC_BTNENHCTRL_POS_MOVE)->SetWindowText(_T("First Device Pos Move"));
		GetDlgItem(IDC_BTNENHCTRL_APPLY)->SetWindowText(_T("APPLY"));
		GetDlgItem(IDC_BTNENHCTRL_OK)->SetWindowText(_T("SAVE"));
		GetDlgItem(IDC_BTNENHCTRL_CANCEL)->SetWindowText(_T("CANCEL"));
		break;
	case 1:
		//Device Setting
		GetDlgItem(IDC_BTNENHCTRL_LABEL_16)->SetWindowText(_T("제품에서 촬상 위치"));
		GetDlgItem(IDC_BTNENHCTRL_LABEL_4)->SetWindowText(_T("X축 제품 선택"));
		GetDlgItem(IDC_BTNENHCTRL_LABEL_6)->SetWindowText(_T("Y축 제품 선택"));
		GetDlgItem(IDC_BTNENHCTRL_LABEL_3)->SetWindowText(_T("화면의 촬상 수량"));
		GetDlgItem(IDC_BTNENHCTRL_LABEL_5)->SetWindowText(_T("X축 제품 개수"));
		GetDlgItem(IDC_BTNENHCTRL_LABEL_7)->SetWindowText(_T("Y축 제품 개수"));
		GetDlgItem(IDC_BTNENHCTRL_POS_MOVE2)->SetWindowText(_T("선택한 위치로 이동"));	
		GetDlgItem(IDC_BTNENHCTRL_LIGHT_SET_1)->SetWindowText(_T("설정"));
		GetDlgItem(IDC_BTNENHCTRL_LIGHT_SET_2)->SetWindowText(_T("설정"));
		GetDlgItem(IDC_BTNENHCTRL_PARA_LOCK)->SetWindowText(_T("파라미터 잠금"));
		GetDlgItem(IDC_BTNENHCTRL_INSP_TEACH2)->SetWindowText(_T("티칭"));

		GetDlgItem(IDC_BTNENHCTRL_LABEL_34)->SetWindowText(_T("마스크\n크기"));
		GetDlgItem(IDC_BTNENHCTRL_LABEL_35)->SetWindowText(_T("밝기 차"));

		//Inspection
		GetDlgItem(IDC_BTNENHCTRL_LABEL_22)->SetWindowText(_T("검사 크기"));
		GetDlgItem(IDC_BTNENHCTRL_LABEL_23)->SetWindowText(_T("픽셀 크기"));

		//20161018 ht 추가 기능 리소스 수정
		GetDlgItem(IDC_BTNENHCTRL_LABEL_31)->SetWindowText(_T("원형 검사 영역\n크기 지정"));
		GetDlgItem(IDC_BTNENHCTRL_LABEL_30)->SetWindowText(_T("원형 검사 영역 설정"));

		GetDlgItem(IDC_BTNENHCTRL_LABEL_20)->SetWindowText(_T("마스크 티칭 값"));
		GetDlgItem(IDC_BTNENHCTRL_LABEL_21)->SetWindowText(_T("평균 값"));
		GetDlgItem(IDC_BTNENHCTRL_LABEL_25)->SetWindowText(_T("화이트 밝기 값"));
		GetDlgItem(IDC_BTNENHCTRL_LABEL_26)->SetWindowText(_T("블랙 밝기 값"));
		GetDlgItem(IDC_BTNENHCTRL_LABEL_28)->SetWindowText(_T("검사 촬상 Index"));		
		GetDlgItem(IDC_BTNENHCTRL_LABEL_0)->SetWindowText(_T("검사 영역 개수"));
		GetDlgItem(IDC_BTNENHCTRL_LABEL_29)->SetWindowText(_T("1회 검사시 촬상 개수"));	
		GetDlgItem(IDC_BTNENHCTRL_LABEL_27)->SetWindowText(_T("검사 촬상 개수"));
		GetDlgItem(IDC_BTNENHCTRL_LABEL_1)->SetWindowText(_T("Light 1"));				
		GetDlgItem(IDC_BTNENHCTRL_LABEL_2)->SetWindowText(_T("Light 2"));				
		GetDlgItem(IDC_BTNENHCTRL_REGION_COPY)->SetWindowText(_T("영역 복사"));				
		GetDlgItem(IDC_BTNENHCTRL_INSP_ALIGN_TEACH)->SetWindowText(_T("얼라인 티칭"));		
		GetDlgItem(IDC_BTNENHCTRL_INSP_TEACH)->SetWindowText(_T("마스크 티칭"));				
		GetDlgItem(IDC_BTNENHCTRL_INSP_ALIGN_TEST)->SetWindowText(_T("테스트"));				
		//Pos Teaching
		GetDlgItem(IDC_BTNENHCTRL_LABEL_8)->SetWindowText(_T("X축 동일제품 거리간격"));
		GetDlgItem(IDC_BTNENHCTRL_LABEL_10)->SetWindowText(_T("Y축 동일제품 거리간격"));
		GetDlgItem(IDC_BTNENHCTRL_LABEL_14)->SetWindowText(_T("X축 제품 거리간격"));
		GetDlgItem(IDC_BTNENHCTRL_LABEL_15)->SetWindowText(_T("Y축 제품 거리간격"));
		GetDlgItem(IDC_BTNENHCTRL_LABEL_17)->SetWindowText(_T("X축 대기 위치"));
		GetDlgItem(IDC_BTNENHCTRL_LABEL_18)->SetWindowText(_T("Y축 대기 위치"));
		GetDlgItem(IDC_BTNENHCTRL_LABEL_19)->SetWindowText(_T("Z축 대기 위치"));
		GetDlgItem(IDC_BTNENHCTRL_LABEL_11)->SetWindowText(_T("첫번째 제품의 X 위치"));
		GetDlgItem(IDC_BTNENHCTRL_LABEL_12)->SetWindowText(_T("첫번째 제품의 Y 위치"));
		GetDlgItem(IDC_BTNENHCTRL_LABEL_13)->SetWindowText(_T("첫번째 제품의 Z 위치"));
		GetDlgItem(IDC_BTNENHCTRL_GRAB_DISTANCE)->SetWindowText(_T("촬상 간격 설정"));			
		GetDlgItem(IDC_BTNENHCTRL_DEVICE_DISTANCE)->SetWindowText(_T("제품간\n거리간격 설정"));		
		GetDlgItem(IDC_BTNENHCTRL_CURENT_POS_SET2)->SetWindowText(_T("대기 위치 설정"));		
		GetDlgItem(IDC_BTNENHCTRL_POS_MOVE3)->SetWindowText(_T("대기 위치 이동"));			
		GetDlgItem(IDC_BTNENHCTRL_CURENT_POS_SET)->SetWindowText(_T("첫번째\n제품 위치 설정"));		
		GetDlgItem(IDC_BTNENHCTRL_POS_MOVE)->SetWindowText(_T("첫번째\n제품 위치 이동"));		
		GetDlgItem(IDC_BTNENHCTRL_APPLY)->SetWindowText(_T("적용"));
		GetDlgItem(IDC_BTNENHCTRL_OK)->SetWindowText(_T("저장"));
		GetDlgItem(IDC_BTNENHCTRL_CANCEL)->SetWindowText(_T("취소"));
		break;
	}
}

void CDialogTeach::ClickTeachEdgeInsp()
{
	// TODO: 여기에 메시지 처리기 코드를 추가합니다.
	m_bEdgeInsp = TRUE;
	m_bBlobInsp = FALSE;
}


void CDialogTeach::ClickTeachBlobInsp()
{
	// TODO: 여기에 메시지 처리기 코드를 추가합니다.
	m_bEdgeInsp = FALSE;
	m_bBlobInsp = TRUE;
	for(int i = 0; i < m_edit_nSelectInspGrabIdx; i++)
	{
		ClickBtnenhctrlSelectImageIdxLeft4();
	}
	m_bEdgeInsp = FALSE;
	m_bBlobInsp = TRUE;
}

BOOL CDialogTeach::GetEdgeInsp()
{
	return m_bEdgeInsp;
}

//20161122 NHT Mask사이즈 조절
double CDialogTeach::GetAlignSize(int ins_grab_idx)
{
	return m_TeachPara.TeachMaskSize[ins_grab_idx];
}

