
// LED_DisplayInspectDlg.h : 헤더 파일
//

#pragma once
#include "stdafx.h"
#include "typedef.h"
#include "Source/camera/MilManage.h"
#include "Source/camera/BaslerManage.h"
#include "DialogJog.h"
#include "DialogManual.h"
#include "DialogTeach.h"
#include "DialogImageRect.h"
#include "DialogNgView.h"
#include "DialogData.h"
#include "DialogParameterLock.h"  //20160906 sc
#include "DialogLogAlarm.h"
#include "DialogProcessBar.h"
#include "afxwin.h"
#include "btnenh.h"
#include "afxcmn.h"

#define USER_MSG_MOT						WM_USER+21

class CMotionData  
{
public:

	CMotionData();
	virtual ~CMotionData();

	//Motion process status
	typeMotionProcessSts *m_ProSts[MOT_STATUS_MAX_NUM];

	//Motion sequence
	typeMotionSeq m_Seq[MOT_STATUS_MAX_NUM];

	void ResetSeq();
};

// CLED_DisplayInspectDlg 대화 상자
class CLED_DisplayInspectDlg : public CDialogEx
{
// 생성입니다.
public:
	CLED_DisplayInspectDlg(CWnd* pParent = NULL);	// 표준 생성자입니다.

// 대화 상자 데이터입니다.
	enum { IDD = IDD_LED_DISPLAYINSPECT_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 지원입니다.

public:
	void ScreenCapture(CString strPath);


	MIL_ID m_MilEdge[2];

	CDialogProcessBar *m_dlgProcessBar;
	CDialogLogAlarm *m_dlgLogAlarm;
	CDialogNgView *m_dlgNgView;
	CDialogImageRect *m_dlgImageRect;
	CDialogTeach *m_dlgTeach;
	CDialogManual *m_dlgManual;
	CDialogData *m_dlgData;	
#ifdef CAMERA
	CMilManage m_MilManage;
#endif
	MyCug m_grid;
	CScrollBar m_scrol_NgDisp;
	int m_RsltIdx;

	BOOL WriteLog(CString text, int log_type, BOOL isList = TRUE);
	
	void SetSelectModel(typedefModelPara select_model);
	typedefModelPara GetSelectModel();

	BOOL InspectionTeach(int nGrabIdx, int nInspGrabIdx);

	void SetTowerLamp(int nLampColor, BOOL bBuzzerOn = FALSE);
	void SetSeqParaReset(BOOL isReset);

	void DeleteGrid();
	void SetGrid();
	void SetGrid(int isOK, int nDeviceIdxX, int nDeviceIdxY, int nGrabIdx);
	void SetGridDbClick(int col,int row, int grab_idx);

	int Inspection(int nDeviceIdxX, int nDeviceIdxY, int nGrabIdx, int nInspGrabIdx, int insp_type, BOOL isGrabComp);

	void NGDisplay(int nDeviceIdxX, int nDeviceIdxY, int nGrabIdx, int nInspGrabIdx, int nDisplayNum, typeInsRsltPara rsltPara);
	void NGAllDisplay();

	typeInsRsltPara GetBeforeRslt();
	void SetBright(int nBright, int x, int y);

	CPoint GetDeviceIdx(int nIdx);
	int GetGrabIdx(int nIdx);
	int GetInspGrabIdx(int nIdx);

	typeInsRsltPara GetRsltPara(int nDeviceIdxX, int nDeviceIdxY, int nGrabIdx, int nInspGrabIdx);
	typeInsRsltPara GetRsltPara();
	BOOL m_bBeforeView;
	BOOL GetInspectionStart();
	void SetSelectGrid(int type, int nDeviceIdxX, int nDeviceIdxY, int nGrabIdx);

	void SetSeq(int prc_idx, int seq_idx, int seq_sub_idx);
	void SetInspectionReset(BOOL isReset);

	void SetCapa(BOOL isNg, int cnt);
	void SetCapa(CString path);
	void SetAllCapa(BOOL isNg, int cnt);
	void SetAllCapa();
	int *GetCapa(CString path);
	int *GetAllCapa();

	CString GetStartTime(int idx);
	CString GetStartDate(int idx);
	CString GetBeforeRsltFilePath(int idx);

	BOOL FileSearch(CString FilePath, CString FileName);

	BOOL SetLoadRsltPara(int idx);
	BOOL SetRsltFile();
	BOOL DeleteRsltFilePath(int idx);
	int GetBeforeRsltCnt();
	void InitBeforeRsltPara();
	void SetVerifyRslt(BOOL isNg, int nDeviceIdxX, int nDeviceIdxY, int nGrabIdx, int file_idx);
	void SetMainState(LPCTSTR text); //20161006 sc
	void ChangeLanguage(BOOL StateLanguage);		//20161011 ht 메인다이얼로그 언어 설정
	void SetMotionAmp(int nMotionIdx, BOOL isAmp);
	BOOL GetMotionAmp(int nMotionIdx);

	BOOL GetSeqStart();

	//20161201 ngh
	void DrowArrow(int nDisplayType, CPoint poStart, int nArrowLength = 60, int nArrowBreadth = 20);

protected:
	void VerifyRsltCal();

	BOOL LoadRsltFile(CString file_path);
	BOOL SaveRsltFile(CString file_path, CString data);

	int m_nBeforeRsltFileCnt;
	vector<CString> m_cBeforeRsltFilePath;
	vector<CString> m_cStartTime;
	vector<CString> m_cStartDate;

	BOOL m_bInspOKNG[MAX_GRAB_CNT][MAX_GRAB_CNT];

	CPoint m_MouseWheelPoint;
	BOOL m_bMouseWheel;
	int m_nViewMode;
	int m_nBeforeSeq;
	BOOL m_bInspectionOKNG;
	BOOL m_bRepeat;
	int m_nRepeatCnt;
	CPoint m_poDeviceIdx[2];
	int m_nGrabIdx[2];
	int m_nInspGrabIdx[2];
#ifdef CAMERA
#ifdef BASLER
	CBaslerManage *m_BaslerManage;
#endif
#endif
	CRITICAL_SECTION	m_CSInline;

	void SetRsltPara(typeInsRsltPara rsltPara);
	
	void AlarmOperation(int pro_idx, int sts_idx, int sub_seq_idx);

	double m_dZoomRatio;

	
	void NGToMainDisplay(int nDeviceIdxX, int nDeviceIdxY, int nGrabIdx, int nInspGrabIdx, int nDisplayNum, typeInsRsltPara rsltPara);
	void SetInspRslt(typeInsRsltPara rsltPara, int nDeviceIdxX, int nDeviceIdxY, int nGrabIdx);
	typeInsRsltPara m_RsltPara;
	typeInsRsltPara m_InsRsltPara[2];
	typeInsRsltPara m_BeforeRsltPara;
	typeInsCntPara m_RsltCnt;
	typedefModelPara m_SelectModel;

	void InitRsltPara();
	void InitInsRsltPara(int idx);

	void Grab();

	void initProductInfoCnt();
	void ProductInfoView();//PRODUCT INFORMATION


	void AllHide();
	void WindowVisible(int win_type);
	void AutoVisible();
	void ManualVisible();
	void TeachVisible();
	void DataVisible();

	void NGDiaplayClear(int nDisplayNum);

	CString m_strGrabImagePath;
	CString m_strOKImagePath;
	CString m_strNGImagePath;

	CWinThread *m_pInspThread;
	static UINT InspThread(LPVOID pParam);
	void InspThread();
	BOOL m_bInspThreadStart;
	BOOL m_bInspThreadEnd;

	CWinThread *m_pMotionThread;
	static UINT MotionThread(LPVOID pParam);
	void MotionThread();
	BOOL MotionProcess(int prc_idx, int seq_idx);
	BOOL m_bMotionProcessStart;
	BOOL m_bMotionProcessEnd;
	int m_nSeqIdx;

	CWinThread *m_pAutoDeleteThread;
	static UINT AutoDeleteThread(LPVOID pParam);
	void AutoDelete();
	BOOL StartAutoDelete_Img(typedefDataPara dataPara);
	BOOL CheckDeleteDate(typedefDataPara dataPara);
	BOOL DeleteFolderSearch(CString strFolder);
	BOOL DeleteFolder(CString strFolder);
	BOOL m_bAutoDeleteStart;
	BOOL m_bAutoDeleteEnd;
	int m_iDel_Year;
	int m_iDel_Month;
	int m_iDel_Day;
	int m_iDel_RecentDay;
	BOOL m_bDelete;
	CString m_strImgPath;

	//Motion data class
	CMotionData m_MDat[MOT_PROCESS_MAX_NUM];

	//Flag
	typeMotionFlag m_Flag;

	//Process result
	typeProcessResult m_Rslt;

	void SetSeqIdx(int prc_idx, int seq_idx, int seq_sub_idx);
	void SetSeqIdx(int prc_idx, int seq_idx, int seq_sub_idx, int next_seq_sub_idx, int type);
	void SetSeqIdx(int prc_idx, int seq_idx, int seq_sub_idx, int next_seq_sub_idx, int type, int device_idx);
	int CheckTimeout(int prc_idx, int seq_idx, int seq_sub_idx, char* status_type);
	int CheckDelay(int prc_idx, int seq_idx, int delay);
	int StopModeOperation(int *prc_idx, int *seq_idx);
	void SeqReset(BOOL isReset);
	int AutoStart();
	int AutoStop();

	int InspectionTest(int nDeviceIdxX, int nDeviceIdxY, int nGrabIdx, int nInspGrabIdx, BOOL isGrabComp);
	BOOL InspectionAlignTeaching(int nDeviceIdxX, int nDeviceIdxY, int nGrabIdx, int nInspGrabIdx);
	BOOL InspectionAlign(int nDeviceIdxX, int nDeviceIdxY, int nGrabIdx, int nInspGrabIdx, BOOL isGrabComp);
	int InspectionEdge(int nDeviceIdxX, int nDeviceIdxY, int nGrabIdx, int nInspGrabIdx, BOOL isGrabComp);
	int InspectionEdge2(int nDeviceIdxX, int nDeviceIdxY, int nGrabIdx, int nInspGrabIdx, BOOL isGrabComp);
	int InspectionEdge3(int nDeviceIdxX, int nDeviceIdxY, int nGrabIdx, int nInspGrabIdx, BOOL isGrabComp);
	BOOL InspectionMaskSet(int nDeviceIdxX, int nDeviceIdx, int nGrabIdx, int nInspGrabIdx, BOOL isGrabComp);
	BOOL EdgeCompare(int nDeviceIdxX, int nDeviceIdxY, int nGrabIdx, double cent_x, double cent_y, double width, double height, double distance);


	BOOL m_bInspectionImageSave;
	int InspectionBlobs(int nDeviceIdxX, int nDeviceIdxY, int nGrabIdx, int nInspGrabIdx, BOOL isGrabComp);
	int InspectionBlobs2(int nDeviceIdxX, int nDeviceIdxY, int nGrabIdx, int nInspGrabIdx, BOOL isGrabComp);
	int InspectionBlobs3(int nDeviceIdxX, int nDeviceIdxY, int nGrabIdx, int nInspGrabIdx, BOOL isGrabComp);
	BOOL BlobsCompare(int nDeviceIdxX, int nDeviceIdxY, int nInspGrabIdx, int nGrabIdx, double cent_x, double cent_y, double width, double height, double Long, double Short, double Breadth, double Ang, double distance, BOOL isGrabComp);
	BOOL InspectionImageSave(int nDeviceIdxX, int nDeviceIdxY, int nGrabIdx, int nInspGrabIdx, int nOk, BOOL isGrabComp);

	void InspectionParaReset(BOOL isReset);

	double m_dGrabStartTime;
	double m_dGrabEndTime;

	double m_dGrabInspStartTime;
	double m_dGrabInspEndTime;
	double m_dDeviceInspStartTime;
	double m_dDeviceInspEndTime;
	double m_dAllInspStartTime;
	double m_dAllInspEndTime;

	void NGView(int nDeviceIdxX, int nDeviceIdxY, int nGrabIdx, int nInspGrabIdx);
	void InitGrid();
	void GridDrow(int isOK, int nDeviceIdxX, int nDeviceIdxY, int nGrabIdx);
	
	short m_nInspCnt;
	short m_nCapaCnt;
	short m_nAllCapaCnt;
	BOOL CapaSave(CString path, int *cnt);
	BOOL CapaLoad(CString path, int *cnt);
	BOOL AllCapaSave(int *cnt);
	BOOL AllCapaLoad(int *cnt);

	void MinusProductInfoCnt();

	BOOL m_bInspThreadTest;

	BOOL m_bMotionAmp[MAX_MOTION_CNT];

	LRESULT OnMotMessage(WPARAM para0, LPARAM para1);
// 구현입니다.

	int m_iFocus_Precision;  //20160906 sc
	CRect SetMoveRect(CRect *rect, int distance, int direction);//0 : orgin, 1 : left, 2 : top, 3 : right, 4 : bottom
protected:
	HICON m_hIcon;

	// 생성된 메시지 맵 함수
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedButtonTest();
	DECLARE_EVENTSINK_MAP()
	void ClickExitviewBtn();
	void ClickTeachviewBtn();
	void ClickBtnenhctrlLive();
	void ClickBtnenhctrlSave();
	void ClickBtnenhctrlLoad();
	void ClickBtnenhctrlZoomin();
	void ClickBtnenhctrlZoomout();
	void ClickBtnenhctrlOne();
	void ClickBtnenhctrlFit();
	void ClickBtnenhctrlTestrun();
	
	afx_msg void OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	CBtnEnh m_btnLive;
	void ClickManualmviewBtn();
	CBtnEnh m_btn_Total;
	CBtnEnh m_btn_Pass;
	CBtnEnh m_btn_Fail;
	CBtnEnh m_btn_FailRat;
	void ClickMainviewBtn();
	CBtnEnh m_btn_TimerData;
	CStatic m_static_Logo;
	void ClickDataviewBtn();
	CBtnEnh m_btn_SelectModelName;
	CBtnEnh m_btn_Auto;
	void ClickMainOpStart();
	void ClickMainOpStop();
	CBtnEnh m_btn_Start;
	CBtnEnh m_btn_End;
	CBtnEnh m_btn_Manual;
	CBtnEnh m_btn_Teach;
	CBtnEnh m_btn_Model;
	CBtnEnh m_btn_InspReset;
	CBtnEnh m_btn_UnitInit;
	void ClickMainOpInit();

	double FocusValue();  //20160906 sc
	int RGBtoGrayF(const long width, const long height, const long dest_pitch, const long src_pitch, const long src_left, const long src_top, 
			   BYTE* img_source_r, BYTE* img_dest); //20160906 sc
	double EdgeRoberts_D(long width, long height, long pitch, BYTE* img_source, double* img_dest, double multi); //20160906 sc
	CBtnEnh m_btn_focus_precision;  //20160906 sc
	void ClickMainInspectionReset();
	CListCtrl m_list_MainLog;
	void ClickMainBeforeRsltView();
	CBtnEnh m_btn_Bright;
	int m_edit_nRepeatMaxCnt;
	CBtnEnh m_btn_GrabTime;
	CBtnEnh m_btn_InspTime;
	void ClickMainProductReset();
	void ClickMainMotionStop();
	afx_msg BOOL OnMouseWheel(UINT nFlags, short zDelta, CPoint pt);
	CBtnEnh m_btn_main_op_state;
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	
	afx_msg void OnBnClickedButtonImgTest();

	
	
	afx_msg void OnBnClickedButtonRegrid();
};
