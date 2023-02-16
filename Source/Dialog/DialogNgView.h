#pragma once
#include "afxwin.h"
#include "DialogVerify.h"
#include "afxcmn.h"

// CDialogNgView 대화 상자입니다.

class CDialogNgView : public CDialogEx
{
	DECLARE_DYNAMIC(CDialogNgView)

public:
	CDialogNgView(CWnd* pParent = NULL);   // 표준 생성자입니다.
	virtual ~CDialogNgView();

// 대화 상자 데이터입니다.
	enum { IDD = IDD_DIALOG_NG_VIEW };

	CDialogVerify *m_dlgVerify;
	typeInsRsltPara m_RsltPara;
	int m_RsltIdx;

	double GetZoomRatio();
	HWND GetDispHwnd();

	//HWND *GetNgDispHwnd();					//20161010 ngh 함수에 잠재적인 문제가 있는 것으로 판단하여 아래 형태로 수정함.
	void GetNgDispHwnd(HWND *hWnd);

	void SetImageLoadPath(CString strPath);
	void SetRsltPara(int nDeviceIdxX, int nDeviceIdxY, int nGrabIdx, int nInspGrabIdx, typeInsRsltPara rsltPara);
	void SetGridDbClick(int col,int row, int grab_idx);
	void SetGridRClick(int col, int row, int grab_idx, BOOL isOK);
	void SetGrid(int isOK, int nDeviceIdxX, int nDeviceIdxY, int nGrabIdx);
	void SetVerifyRslt(BOOL isNg, int nDeviceIdxX, int nDeviceIdxY, int nGrabIdx);
	void ChangeLanguage(BOOL StateLanguage);	//20161011 ht NGView다이얼로그 언어 설정

	void SetListIdxInit();//20161129 ngh
protected:
	//20161006 ngh
	BOOL m_bMouseWheel;
	CPoint m_MouseWheelPoint;

	BOOL m_bVerifyStart;
	int m_nVerifyCnt;
	typedefVerifyData m_VerifyData[MAX_RSLT_NUM];

	BOOL m_bVerifyComp;
	int m_nFileIdx;
	void InitRsltPara();

	void ListInit();

	
	short m_nDefListIdx;

	CString m_strImageLoadPath;

	CPoint m_SelDeviceIdx;
	CPoint m_BeforeDeviceIdx;
	
	void ZoomDisplay();
	double m_dZoomDefault;
	double m_dZoomRatio;
	double m_dZoomRatioOld;
	double m_dZoomRatio_RB;
	BOOL m_bPanDN;
	double m_dZoomSX;
	double m_dZoomSY;
	double m_dZoomSX_RB;
	double m_dZoomSY_RB;
	long m_lPanSX;
	long m_lPanSY;
	long m_lPanEX;
	long m_lPanEY;
	CdRect m_ZoomImage;

	MyCug m_grid;
	void InitGrid();
	void GridDrow();
	void GridDrow(int isOK, int nDeviceIdxX, int nDeviceIdxY, int nGrabIdx);
	void DeleteGrid();
	void SetGrid();
	

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedButton1();
	afx_msg void OnShowWindow(BOOL bShow, UINT nStatus);
	virtual BOOL OnInitDialog();
	DECLARE_EVENTSINK_MAP()
	void ClickBtnenhctrlZoomin();
	void ClickBtnenhctrlZoomout();
	void ClickBtnenhctrlOne();
	void ClickBtnenhctrlFit();
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	CScrollBar m_scrol_NgDisp;
	afx_msg void OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	CBtnEnh m_btn_DeviceIdx;
	CListCtrl m_list_BeforeRslt;
	afx_msg void OnNMDblclkListBeforeRslt(NMHDR *pNMHDR, LRESULT *pResult);
	CBtnEnh m_btn_SelectRsltFile;
	void ClickBtnenhctrlFsltFileComp();
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	void ClickBtnenhctrlFsltFileComp2();
	afx_msg BOOL OnMouseWheel(UINT nFlags, short zDelta, CPoint pt);
};
