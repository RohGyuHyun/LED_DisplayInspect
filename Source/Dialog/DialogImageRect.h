#pragma once

#include "_CtrlTracker.h"
#include "Source/camera/MilManage.h"


#define	USER_MSG_IMG_RECT		WM_USER+10 

// CDialogImageRect 대화 상자입니다.

class CDialogImageRect : public CDialog
{
	DECLARE_DYNAMIC(CDialogImageRect)

public:
	CDialogImageRect(CWnd* pParent = NULL);   // 표준 생성자입니다.
	virtual ~CDialogImageRect();

// 대화 상자 데이터입니다.
	enum { IDD = IDD_DIALOG_IMAGE_RECT };

	void InitTracker();

	HWND GetDispHwnd();
	CRect GetDlgRect();
	void SetDlgRect(CRect rect);
	int GetEnable(int ins_idx, int grab_idx, int idx);
	void SetEnable(int ins_idx, int grab_idx, int idx, BOOL enable);
	void SetTrackerNum(int i);
	void SetDlgIdx(int idx);
	double GetZoom();
	void SetZoom(double ratio, long img_wd, long img_ht);
	HWND GetWinHwnd();
	void GetTracker(int ins_idx, int grab_idx, int idx, double *left, double *top, double *right, double *bottom);
	void SetTracker(int ins_idx, int grab_idx, int idx, double left, double top, double right, double bottom);
	void SetActiveTracker(int ins_idx, int grab_idx, int idx, BOOL active);
	BOOL CheckTracker(int ins_idx, int grab_idx, int idx, CPoint point);
	void SetAllTracker();
	//Image display
	LRESULT OnImageRectMessage(WPARAM para0, LPARAM para1);

	void SetReDraw();

	double m_dZoomDefault;
	double m_dZoomRatio;

	C_CtrlTracker m_Tracker[MAX_GRAB_CNT][MAX_INSP_GRAB_CNT][TRACKER_MAX_NUM];
	CRect m_rectDlg;

	int GetInsIdx();
	void SetInsIdx(int ins_idx);

	int GetGrabIdx();
	void SetGrabIdx(int grab_idx);

	void SetApplyTracker();

private:
	long m_lImgWd;
	long m_lImgHt;

	int m_iDlgIdx;

	int m_iTrackerNum;

	long m_lPanSX;
	long m_lPanSY;
	long m_lPanEX;
	long m_lPanEY;
	BOOL m_bPanDN;

	double m_dPX;
	double m_dPY;

	CPoint m_poStart;
	CPoint m_poEnd;

	CScrollBar m_ScrMain_V;
	CScrollBar m_ScrMain_H;
	CPoint m_poScroll;

	int m_nIns_idx;
	int m_nGrabIdx;

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.
	virtual BOOL OnInitDialog();


	DECLARE_MESSAGE_MAP()

	afx_msg void OnPaint();
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnLButtonDblClk(UINT nFlags, CPoint point);
	afx_msg void OnShowWindow(BOOL bShow, UINT nStatus);
	afx_msg void OnRButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnRButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnRButtonDblClk(UINT nFlags, CPoint point);
public:
	DECLARE_EVENTSINK_MAP()
	afx_msg void OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	afx_msg void OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	virtual BOOL PreTranslateMessage(MSG* pMsg);
};
