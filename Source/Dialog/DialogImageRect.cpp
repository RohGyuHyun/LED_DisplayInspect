// DialogImageRect.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "LED_DisplayInspect.h"
#include "LED_DisplayInspectDlg.h"
#include "DialogImageRect.h"
#include "afxdialogex.h"


// CDialogImageRect 대화 상자입니다.

IMPLEMENT_DYNAMIC(CDialogImageRect, CDialog)

CDialogImageRect::CDialogImageRect(CWnd* pParent /*=NULL*/)
	: CDialog(CDialogImageRect::IDD, pParent)
{
	m_dZoomRatio = 1.;
	m_lImgWd = 1024;
	m_lImgHt = 5000;

	m_iDlgIdx = 0;

	m_iTrackerNum = 0;

	m_lPanSX = 0;
	m_lPanSY = 0;
	m_lPanEX = 0;
	m_lPanEY = 0;
	m_bPanDN = FALSE;
	
	m_dPX = 0.;
	m_dPY = 0.;
	m_poScroll.x = 0;
	m_poScroll.y = 0;

	m_nIns_idx = 0;
	m_nGrabIdx = 0;
}

CDialogImageRect::~CDialogImageRect()
{
}

void CDialogImageRect::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CDialogImageRect, CDialog)
	ON_WM_PAINT()
	ON_WM_TIMER()
	ON_WM_LBUTTONUP()
	ON_WM_LBUTTONDOWN()
	ON_WM_MOUSEMOVE()
	ON_WM_LBUTTONDBLCLK()
	ON_MESSAGE(USER_MSG_IMG_RECT, OnImageRectMessage)
	ON_WM_SHOWWINDOW()
	ON_WM_RBUTTONDOWN()
	ON_WM_RBUTTONUP()
	ON_WM_RBUTTONDBLCLK()
	ON_WM_HSCROLL()
	ON_WM_VSCROLL()
END_MESSAGE_MAP()


// CDialogImageRect 메시지 처리기입니다.

#define INSPECTION_REGION			0

BOOL CDialogImageRect::OnInitDialog()
{
	CDialog::OnInitDialog();

	CLED_DisplayInspectDlg *pdlg = ((CLED_DisplayInspectDlg *)AfxGetApp()->m_pMainWnd);

	CRect rect;
	char temp[50];
	int ins_idx = 0;
	int align_ins_idx = 0;
	int align_search_idx = 0;

	GetClientRect(&rect);
	//m_dZoomRatio = (double)rect.Width() / CAMERA_WIDTH;

	for(int i = 0; i < MAX_GRAB_CNT; i++)
	{
		for(int j = 0; j < MAX_INSP_GRAB_CNT; j++)
		{
			align_search_idx = 0;
			align_ins_idx = 0;
			ins_idx = 0;
			for(int k = 0; k < TRACKER_MAX_NUM; k++)
			{
				m_Tracker[i][j][k] = C_CtrlTracker(this, 0, 4, 2);
				if((k == 0) || ((k % 3) == 0))
				{
					m_Tracker[i][j][k].SetColor(RGB(0, 0, 255));
					sprintf(temp, "Grab_%03d_%03d_Align_Search_%03d", i + 1, j, align_search_idx++);
				}
				else if((k == 1) || ((k % 3) == 1))
				{
					m_Tracker[i][j][k].SetColor(RGB(255, 0, 0));
					sprintf(temp, "Grab_%03d_%03d_Align_%d", i + 1, j, align_ins_idx++);
				}
				else if((k == 2) || ((k % 3) == 2))
				{
					m_Tracker[i][j][k].SetColor(RGB(0, 255, 0));
					sprintf(temp, "Grab_%03d_%03d, Inspection_%03d", i + 1, j, ins_idx++);
				}

				m_Tracker[i][j][k].SetCaption(temp);

				m_Tracker[i][j][k].SetZoom(0, 0, CAMERA_WIDTH, CAMERA_HEIGHT);
				//m_Tracker[i][j].SetRect(100, 100, 200, 200);
				//SetTracker(i, j, (double)pdlg->m_dlgTeach->GetTeachPara().TeachRegion[i][j].left, (double)pdlg->m_dlgTeach->GetTeachPara().TeachRegion[i][j].top, 
				//			(double)pdlg->m_dlgTeach->GetTeachPara().TeachRegion[i][j].right, (double)pdlg->m_dlgTeach->GetTeachPara().TeachRegion[i][j].bottom);
		
				m_Tracker[i][j][k].SetActive(FALSE);
			}
		}
	}

	return TRUE;
}

HWND CDialogImageRect::GetDispHwnd()
{
	return this->m_hWnd;
}

void CDialogImageRect::SetDlgRect(CRect rect)
{
	m_rectDlg = rect;
}

CRect CDialogImageRect::GetDlgRect()
{
	return m_rectDlg;
}

HWND CDialogImageRect::GetWinHwnd()
{
	return this->m_hWnd;
}

void CDialogImageRect::SetZoom(double ratio, long img_wd, long img_ht)
{
	m_lImgWd = img_wd;
	m_lImgHt = img_ht;
	m_dZoomRatio = ratio;
}

double CDialogImageRect::GetZoom()
{
	//Matrox에 맞게 계산해서 보냄
	return m_dZoomRatio;
}

void CDialogImageRect::SetDlgIdx(int idx)
{
	m_iDlgIdx = idx;
}

void CDialogImageRect::SetTrackerNum(int i)
{
	m_iTrackerNum = i;
}

void CDialogImageRect::SetEnable(int ins_idx, int grab_idx, int idx, BOOL enable)
{
	int i;

	for(i = 0; i < TRACKER_MAX_NUM; i++)
	{
		m_Tracker[ins_idx][grab_idx][i].SetEnable(FALSE);
	}

	m_Tracker[ins_idx][grab_idx][idx].SetEnable(enable);
}

int CDialogImageRect::GetEnable(int ins_idx, int grab_idx, int idx)
{
	int i;

	for(i = 0; i < TRACKER_MAX_NUM; i++)
	{
		if(m_Tracker[ins_idx][grab_idx][idx].GetEnable())
		{
			return i;
		}
	}

	return -1;
}

void CDialogImageRect::InitTracker()
{
	CLED_DisplayInspectDlg *pdlg = ((CLED_DisplayInspectDlg *)AfxGetApp()->m_pMainWnd);

	for(int i = 0; i < MAX_GRAB_CNT; i++)
	{
		for(int j = 0; j < MAX_INSP_GRAB_CNT; j++)
		{
			for(int k = 0; k < TRACKER_MAX_NUM; k++)
			{
				SetTracker(i, j, k, pdlg->m_dlgTeach->GetTeachPara().TeachRegion[i][j][k].GetLeft(), pdlg->m_dlgTeach->GetTeachPara().TeachRegion[i][j][k].GetTop(), 
							pdlg->m_dlgTeach->GetTeachPara().TeachRegion[i][j][k].GetRight(), pdlg->m_dlgTeach->GetTeachPara().TeachRegion[i][j][k].GetBottom());
		
				m_Tracker[i][j][k].SetActive(FALSE);
			}
		}
	}
}

void CDialogImageRect::SetTracker(int ins_idx, int grab_idx, int idx, double left, double top, double right, double bottom)
{
	CdRect img_rect;
	CRect win_rect;
	double ratio_x, ratio_y;
	double sx, sy, ex, ey;

	m_Tracker[ins_idx][grab_idx][idx].GetZoom(&sx, &sy, &ex, &ey);
	img_rect.SetdRectLTRB(sx, sy, ex, ey);
	GetClientRect(&win_rect);

	ratio_x = m_dZoomRatio;//((double)win_rect.Width()) / ((double)img_rect.Width());
	ratio_y = ratio_x;//((double)win_rect.Height()) / ((double)img_rect.Height());
	
	m_Tracker[ins_idx][grab_idx][idx].SetRect(win_rect.left + (left - sx) * ratio_x, 
						   win_rect.top + (top - sy) * ratio_y, 
						   win_rect.left + (right - sx) * ratio_x, 
						   win_rect.top + (bottom - sy) * ratio_y);
}

void CDialogImageRect::GetTracker(int ins_idx, int grab_idx, int idx, double *left, double *top, double *right, double *bottom)
{
	CRect img_rect, win_rect;
	double ratio_x, ratio_y;
	double sx, sy, ex, ey;
	double win_left, win_top, win_right, win_bottom;

	m_Tracker[ins_idx][grab_idx][idx].GetZoom(&sx, &sy, &ex, &ey);
	img_rect = CRect((int)sx, (int)sy, (int)ex, (int)ey);
	GetClientRect(&win_rect);

	ratio_x = m_dZoomRatio;//((double)win_rect.Width()) / ((double)img_rect.Width());
	ratio_y = ratio_x;//((double)img_rect.Height()) / ((double)win_rect.Height());

	m_Tracker[ins_idx][grab_idx][idx].GetRect(&win_left, &win_top, &win_right, &win_bottom);
	
	*left = sx + (win_left - win_rect.left) * ratio_x;
	*top = sy + (win_top - win_rect.top) * ratio_y; 
	*right = sx + (win_right - win_rect.left) * ratio_x;
	*bottom = sy + (win_bottom - win_rect.top) * ratio_y; 

}

void CDialogImageRect::SetActiveTracker(int ins_idx, int grab_idx, int idx, BOOL active)
{
	m_Tracker[ins_idx][grab_idx][idx].SetActive(active);
}

void CDialogImageRect::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.

	switch(nIDEvent)
	{
	case 0:
		KillTimer(nIDEvent);
		//Tracker
		for(int i = 0; i < TRACKER_MAX_NUM; i++)
		{
			m_Tracker[m_nIns_idx][m_nGrabIdx][i].Draw(DRAW_RECTANGLE);
		}
		break;
	}

	CDialog::OnTimer(nIDEvent);
}

BOOL CDialogImageRect::CheckTracker(int ins_idx, int grab_idx, int idx, CPoint point)
{
	CRect rect, win_rect;
	POINT pnt;

	rect = m_Tracker[ins_idx][grab_idx][idx].GetRect();

	GetClientRect(&win_rect);
	win_rect.left = win_rect.left + 10;
	win_rect.top = win_rect.top + 10;
	win_rect.right = win_rect.right - 10;
	win_rect.bottom = win_rect.bottom - 10;

	if(!win_rect.PtInRect(point))
	{
		return FALSE;
	}

	pnt.x = rect.left;
	pnt.y = rect.top;
	if(!win_rect.PtInRect(pnt))
	{
		return FALSE;
	}

	pnt.x = rect.right;
	pnt.y = rect.bottom;
	if(!win_rect.PtInRect(pnt))
	{
		return FALSE;
	}

	return TRUE;
}


void CDialogImageRect::OnLButtonUp(UINT nFlags, CPoint point) 
{
	// TODO: Add your message handler code here and/or call default
	//Tracker
	int i;

	m_bPanDN = FALSE;

	for(int k = 0; k < MAX_GRAB_CNT; k++)
	{
		for(int j = 0; j < MAX_INSP_GRAB_CNT; j++)
		{
			for(i = 0; i < TRACKER_MAX_NUM; i++)
			{
				m_Tracker[k][j][i].OnLButtonUp(nFlags, point);
			}
		}
	}

	//CRect rect = CRect(100, 100, 300, 300);
	Invalidate();

	//Region confirm
	CLED_DisplayInspectDlg *pdlg = ((CLED_DisplayInspectDlg *)AfxGetApp()->m_pMainWnd);

	/* Clear the overlay image. */
	//MdispControl(*pdlg->GetDisplay(m_iDlgIdx), M_OVERLAY_CLEAR, M_DEFAULT);

	::PostMessage(this->m_hWnd, USER_MSG_IMG_RECT, m_iDlgIdx, 0);
	
	CDialog::OnLButtonUp(nFlags, point);
}

void CDialogImageRect::OnLButtonDown(UINT nFlags, CPoint point) 
{
	// TODO: Add your message handler code here and/or call default
	CLED_DisplayInspectDlg *pdlg = ((CLED_DisplayInspectDlg *)AfxGetApp()->m_pMainWnd);

	//Tracker
	int i;
	BOOL chk = TRUE;

	for(int k = 0; k < MAX_GRAB_CNT; k++)
	{
		for(int j = 0; j < MAX_INSP_GRAB_CNT; j++)
		{
			for(i = 0; i < TRACKER_MAX_NUM; i++)
			{
				if(m_Tracker[k][j][i].GetEnable())
				{
					chk = FALSE;
				}
			}
		}
	}

	//활성화가 하나도 되어있지 않으면 image pan
	if(chk)
	{
		m_bPanDN = TRUE;

		m_lPanSX = point.x;
		m_lPanSY = point.y;
	}
	else
	{
		for(int k = 0; k < MAX_GRAB_CNT; k++)
		{
			for(int j = 0; j < MAX_INSP_GRAB_CNT; j++)
			{
				for(i = 0; i < TRACKER_MAX_NUM; i++)
				{
					m_Tracker[k][j][i].OnLButtonDown(nFlags, point);
				}	
			}
		}
	}
	::PostMessage(this->m_hWnd, USER_MSG_IMG_RECT, m_iDlgIdx, 8);
	
	CDialog::OnLButtonDown(nFlags, point);
}

void CDialogImageRect::SetReDraw()
{
	m_bPanDN = FALSE;
	Invalidate();
}

void CDialogImageRect::OnMouseMove(UINT nFlags, CPoint point) 
{
	// TODO: Add your message handler code here and/or call default
	CLED_DisplayInspectDlg *pdlg = ((CLED_DisplayInspectDlg *)AfxGetApp()->m_pMainWnd);

	//Tracker
	int i;
	CRect rect;
	double diff_x, diff_y, diff_img_x, diff_img_y;

	double win_left, win_top, win_right, win_bottom;
	double sx, sy, ex, ey;
	CRect img_rect;
	double ratio_x, ratio_y;
	double sx0, sy0, ex0, ey0;
	CRect subRect, zoomRect;

	BOOL chk = TRUE;
	for(int k = 0; k < MAX_GRAB_CNT; k++)
	{
		for(int j = 0; j < MAX_INSP_GRAB_CNT; j++)
		{
			for(i = 0; i < TRACKER_MAX_NUM; i++)
			{
				if(m_Tracker[k][j][i].GetEnable())
				{
					chk = FALSE;
				}
			}
		}
	}

	//활성화가 하나도 되어있지 않으면 image pan
	if(chk)
	{
		if(m_bPanDN)
		{
			diff_x = point.x - m_lPanSX;
			diff_y = point.y - m_lPanSY;

			m_lPanEX = point.x;
			m_lPanEY = point.y;

			m_lPanSX = m_lPanEX;
			m_lPanSY = m_lPanEY;
			
			for(int k = 0; k < MAX_GRAB_CNT; k++)
			{
				for(int j = 0; j < MAX_INSP_GRAB_CNT; j++)
				{
					for(i = 0; i < TRACKER_MAX_NUM; i++)
					{
						m_Tracker[k][j][i].GetZoom(&sx, &sy, &ex, &ey);
						m_Tracker[k][j][i].GetRect(&win_left, &win_top, &win_right, &win_bottom);	

						//Zoom///////////////////////////

						img_rect = CRect((int)sx, (int)sy, (int)ex, (int)ey);	
						ratio_x = ((double)img_rect.Width()) / ((double)m_rectDlg.Width());
						ratio_y = ((double)img_rect.Height()) / ((double)m_rectDlg.Height());

						diff_img_x = diff_x * ratio_x;
						diff_img_y = diff_y * ratio_y;

						sx0 = sx - diff_img_x;
						if(0 > sx0)
						{
							sx0 = 0;
							diff_img_x = sx;
							m_bPanDN = FALSE;
						}

						sy0 = sy - diff_img_y;
						if(0 > sy0)
						{
							sy0 = 0;
							diff_img_y = sy;
							m_bPanDN = FALSE;
						}

						ex0 = ex - diff_img_x;
						if((CAMERA_WIDTH) < ex0)
						{
							ex0 = CAMERA_WIDTH;
							diff_img_x = ex - CAMERA_WIDTH;
							m_bPanDN = FALSE;
						}

						ey0 = ey - diff_img_y;
						if(CAMERA_HEIGHT < ey0)
						{
							ey0 = CAMERA_HEIGHT;
							diff_img_y = ey - CAMERA_HEIGHT;
							m_bPanDN = FALSE;
						}
			
						m_Tracker[k][j][i].SetZoom(sx - diff_img_x, sy - diff_img_y, ex - diff_img_x, ey - diff_img_y);

						//Rect///////////////////////////
						diff_x = diff_img_x / ratio_x;
						diff_y = diff_img_y / ratio_y;

						m_Tracker[k][j][i].SetRect(win_left + diff_x, win_top + diff_y, win_right + diff_x, win_bottom + diff_y);
					}
				}
			}
#ifdef CAMERA
			MdispPan(*pdlg->m_MilManage.GetMilDisplay(0), sx - diff_img_x, sy - diff_img_y);
#endif
		}
	}
	else
	{
		for(int k = 0; k < MAX_GRAB_CNT; k++)
		{
			for(int j = 0; j < MAX_INSP_GRAB_CNT; j++)
			{
				for(i = 0; i < TRACKER_MAX_NUM; i++)
				{
					rect = m_Tracker[k][j][i].GetRect();
					m_Tracker[k][j][i].OnMouseMove(nFlags, point);
			
					if(!CheckTracker(k, j, i, point))
					{
						m_Tracker[k][j][i].SetRect(rect);
					}	
				}
			}
		}
	}

#ifdef CAMERA
	long pitch;
	BYTE *by_source;
#endif

	m_Tracker[0][0][0].GetZoom(&sx, &sy, &ex, &ey);

	diff_x = point.x;
	diff_y = point.y;

	img_rect = CRect((int)sx, (int)sy, (int)ex, (int)ey);	
	ratio_x = ((double)img_rect.Width()) / ((double)m_rectDlg.Width());
	ratio_y = ((double)img_rect.Height()) / ((double)m_rectDlg.Height());

	diff_img_x = (long)((double)(diff_x) * ratio_x) + (int)(sx);
	diff_img_y = (long)((double)(diff_y) * ratio_y) + (int)(sy);
#ifdef CAMERA
	MbufInquire(*pdlg->m_MilManage.GetMilDispBuf(MAIN_DISP), M_PITCH, &pitch);
	MbufInquire(*pdlg->m_MilManage.GetMilDispBuf(MAIN_DISP), M_HOST_ADDRESS, &by_source);
#endif
	if(diff_img_x < 0)
	{
		diff_img_x = 0;
	}

	if(diff_img_y < 0)
	{
		diff_img_y = 0;
	}
	
	if(diff_img_x <= CAMERA_WIDTH && diff_img_y <= CAMERA_HEIGHT)
	{
#ifdef CAMERA
		int idx;

		idx = (long)diff_img_x + (pitch * (long)diff_img_y);
		pdlg->SetBright(by_source[idx], (int)diff_img_x, (int)diff_img_y);
#endif
	}

	//체크가 하나도 않되어 있으면 영상을 움직인다.
	
	CDialog::OnMouseMove(nFlags, point);
}

void CDialogImageRect::SetApplyTracker()
{
	CLED_DisplayInspectDlg *pdlg = ((CLED_DisplayInspectDlg *)AfxGetApp()->m_pMainWnd);
	int i;
	double l, t, r, b;
	CdRect subRect, zoomRect;
	typeTeachPara teachPara;
	UINT nFlags = 1;
	CPoint point;
	point.SetPoint(1, 1);
	teachPara = pdlg->m_dlgTeach->GetTeachPara();

	for(i = 0; i < TRACKER_MAX_NUM; i++)
	{
		m_Tracker[m_nIns_idx][m_nGrabIdx][i].SetEnable(FALSE);
	}

	for(i = 0; i < TRACKER_MAX_NUM; i++)
	{
		if(m_Tracker[m_nIns_idx][m_nGrabIdx][i].OnLButtonDblClk(nFlags, point))
		{
			m_Tracker[m_nIns_idx][m_nGrabIdx][i].SetEnable(TRUE);
			break;
		}
		else
		{
			m_Tracker[m_nIns_idx][m_nGrabIdx][i].SetEnable(FALSE);
			subRect.SetdRect(m_Tracker[m_nIns_idx][m_nGrabIdx][i].GetRect());
			m_Tracker[m_nIns_idx][m_nGrabIdx][i].GetZoom(&l, &t, &r, &b);
			zoomRect.SetdRectLTRB(l, t, r, b);
			//subRect.SetRect(win_left + diff_x, win_top + diff_y, win_right + diff_x, win_bottom + diff_y);
			//zoomRect.SetRect(sx - diff_img_x, sy - diff_img_y, ex - diff_img_x, ey - diff_img_y);

			teachPara.TeachRegion[m_nIns_idx][m_nGrabIdx][i].SetdRectLTRB(zoomRect.GetLeft() + (subRect.GetLeft() / m_dZoomRatio),
																		zoomRect.GetTop() + (subRect.GetTop() / m_dZoomRatio),
																		zoomRect.GetLeft() + (subRect.GetRight() / m_dZoomRatio),
																		zoomRect.GetTop() + (subRect.GetBottom() / m_dZoomRatio));
		}
	}
	pdlg->m_dlgTeach->SetTeachPara(teachPara);

	::PostMessage(this->m_hWnd, USER_MSG_IMG_RECT, m_iDlgIdx, 0);
}

void CDialogImageRect::OnLButtonDblClk(UINT nFlags, CPoint point) 
{
	// TODO: Add your message handler code here and/or call default
	//Tracker
	CLED_DisplayInspectDlg *pdlg = ((CLED_DisplayInspectDlg *)AfxGetApp()->m_pMainWnd);
	int i;
	double l, t, r, b;
	CdRect subRect, zoomRect;
	typeTeachPara teachPara;
	teachPara = pdlg->m_dlgTeach->GetTeachPara();

	for(i = 0; i < TRACKER_MAX_NUM; i++)
	{
		m_Tracker[m_nIns_idx][m_nGrabIdx][i].SetEnable(FALSE);
	}

	for(i = 0; i < TRACKER_MAX_NUM; i++)
	{
		if(m_Tracker[m_nIns_idx][m_nGrabIdx][i].OnLButtonDblClk(nFlags, point))
		{
			m_Tracker[m_nIns_idx][m_nGrabIdx][i].SetEnable(TRUE);
			break;
		}
		else
		{
			m_Tracker[m_nIns_idx][m_nGrabIdx][i].SetEnable(FALSE);
			subRect.SetdRect(m_Tracker[m_nIns_idx][m_nGrabIdx][i].GetRect());
			m_Tracker[m_nIns_idx][m_nGrabIdx][i].GetZoom(&l, &t, &r, &b);
			zoomRect.SetdRectLTRB(l, t, r, b);
			//subRect.SetRect(win_left + diff_x, win_top + diff_y, win_right + diff_x, win_bottom + diff_y);
			//zoomRect.SetRect(sx - diff_img_x, sy - diff_img_y, ex - diff_img_x, ey - diff_img_y);

			teachPara.TeachRegion[m_nIns_idx][m_nGrabIdx][i].SetdRectLTRB(zoomRect.GetLeft() + (subRect.GetLeft() / m_dZoomRatio),
																		zoomRect.GetTop() + (subRect.GetTop() / m_dZoomRatio),
																		zoomRect.GetLeft() + (subRect.GetRight() / m_dZoomRatio),
																		zoomRect.GetTop() + (subRect.GetBottom() / m_dZoomRatio));
		}
	}
	pdlg->m_dlgTeach->SetTeachPara(teachPara);

	::PostMessage(this->m_hWnd, USER_MSG_IMG_RECT, m_iDlgIdx, 0);
	
	CDialog::OnLButtonDblClk(nFlags, point);
}


LRESULT CDialogImageRect::OnImageRectMessage(WPARAM para0, LPARAM para1)
{

	// TODO: Add your control notification handler code here
	CLED_DisplayInspectDlg *pdlg = ((CLED_DisplayInspectDlg *)AfxGetApp()->m_pMainWnd);

	//Tracker
/*	for(int i = 0; i < TRACKER_MAX_NUM; i++)
	{
		m_Tracker[i].Draw(i);
	}*/

	//Tracker
	SetTimer(0, 1, NULL);


	return 0;
}

void CDialogImageRect::OnPaint() 
{
	CPaintDC dc(this); // device context for painting
	
	// TODO: Add your message handler code here

	//Tracker
	for(int i = 0; i < MAX_GRAB_CNT; i++)
	{
		for(int j = 0; j < MAX_INSP_GRAB_CNT; j++)
		{
			for(int k = 0; k < TRACKER_MAX_NUM; k++)
			{
				m_Tracker[i][j][k].Draw(i);
			}
		}
	}
	// Do not call CDialog::OnPaint() for painting messages
}

void CDialogImageRect::SetAllTracker()
{
	CLED_DisplayInspectDlg *pdlg = ((CLED_DisplayInspectDlg *)AfxGetApp()->m_pMainWnd);
	CRect rect;
	double l, t, r, b;
	typeTeachPara teachPara;
	teachPara = pdlg->m_dlgTeach->GetTeachPara();

	for(int k = 0; k < MAX_GRAB_CNT; k ++)
	{
		for(int j = 0; j < MAX_INSP_GRAB_CNT; j++)
		{
			for(int i = 0; i < TRACKER_MAX_NUM; i++)
			{
				m_Tracker[k][j][i].GetZoom(&l, &t, &r, &b);
				m_Tracker[k][j][i].SetRect((teachPara.TeachRegion[k][j][i].GetLeft() * m_dZoomRatio) - (l * m_dZoomRatio),
											(teachPara.TeachRegion[k][j][i].GetTop() * m_dZoomRatio) - (t * m_dZoomRatio),
											(teachPara.TeachRegion[k][j][i].GetRight() * m_dZoomRatio) - (l * m_dZoomRatio),
											(teachPara.TeachRegion[k][j][i].GetBottom() * m_dZoomRatio) - (t * m_dZoomRatio));
			}
		}
	}
}

void CDialogImageRect::OnShowWindow(BOOL bShow, UINT nStatus)
{
	CDialog::OnShowWindow(bShow, nStatus);
	CLED_DisplayInspectDlg *pdlg = ((CLED_DisplayInspectDlg *)AfxGetApp()->m_pMainWnd);
	CRect rect;

	if(bShow)
	{
		typeTeachPara teachPara;
		teachPara = pdlg->m_dlgTeach->GetTeachPara();
#ifdef CAMERA
		MdispControl(*pdlg->m_MilManage.GetMilDisplay(MAIN_DISP), M_OVERLAY_CLEAR, M_DEFAULT);
#endif
		if(pdlg->m_dlgTeach->IsWindowVisible())
		{
			SetAllTracker();
		}
	}
}

void CDialogImageRect::OnRButtonDown(UINT nFlags, CPoint point)
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.
	CLED_DisplayInspectDlg *pdlg = ((CLED_DisplayInspectDlg *)AfxGetApp()->m_pMainWnd);

	CRect img_rect, main_rect;
	double ratio_x, ratio_y, sx, sy, ex, ey;
	long diff_x, diff_y, diff_img_x, diff_img_y;
	MIL_INT pitch = 0;
	BYTE *by_source = NULL;
	int idx;
	CString str_brightness, strX, strY;

	str_brightness.Format("0");
	strX.Format("0");
	strY.Format("0");

	m_Tracker[0][0][0].GetZoom(&sx, &sy, &ex, &ey);

	GetWindowRect(&main_rect);
	diff_x = point.x;
	diff_y = point.y;

	img_rect = CRect((int)sx, (int)sy, (int)ex, (int)ey);	
	ratio_x = ((double)img_rect.Width()) / ((double)m_rectDlg.Width());
	ratio_y = ((double)img_rect.Height()) / ((double)m_rectDlg.Height());

	diff_img_x = (long)((double)(diff_x) * ratio_x) + (int)(sx);
	diff_img_y = (long)((double)(diff_y) * ratio_y) + (int)(sy);
#ifdef CAMERA
	MbufInquire(*pdlg->m_MilManage.GetMilDispBuf(MAIN_DISP), M_HOST_ADDRESS, &by_source);
	MbufInquire(*pdlg->m_MilManage.GetMilDispBuf(MAIN_DISP), M_PITCH, &pitch);
#endif
	if(diff_img_x <= CAMERA_WIDTH)
	{
		idx = diff_img_x + (pitch * diff_img_y);
		str_brightness.Format("%d", by_source[idx]);
		strX.Format("%d", diff_img_x);
		strY.Format("%d", diff_img_y);
		pdlg->SetBright(by_source[idx], diff_img_x, diff_img_y);
	}

	CDialog::OnRButtonDown(nFlags, point);
}

void CDialogImageRect::OnRButtonUp(UINT nFlags, CPoint point)
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.

	CDialog::OnRButtonUp(nFlags, point);
}

void CDialogImageRect::OnRButtonDblClk(UINT nFlags, CPoint point)
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.
	
	CDialog::OnRButtonDblClk(nFlags, point);
}

BEGIN_EVENTSINK_MAP(CDialogImageRect, CDialog)
END_EVENTSINK_MAP()

void CDialogImageRect::OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar)
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.
	CLED_DisplayInspectDlg *pdlg = ((CLED_DisplayInspectDlg *)AfxGetApp()->m_pMainWnd);
	long diff;
	double sx, sy, ex, ey;
	double win_left, win_top, win_right, win_bottom;
	double ratio_y, ratio_x;
	double diff_img_x, diff_img_y;
	double sx0, sy0, ex0, ey0;
	long diff_x, diff_y;
	CRect img_rect;

	diff = m_lPanEX - m_lPanSX;
	
	SCROLLINFO scrollInfo;
	scrollInfo.cbSize = sizeof(SCROLLINFO);
	
	GetScrollInfo(SB_HORZ, &scrollInfo);

	switch(nSBCode)
	{
	case SB_LINELEFT:
		m_lPanSX -= 2;
		
		break;
	case SB_LINERIGHT:
		m_lPanSX += 2;
		
		break;
	case SB_PAGELEFT:
		
		break;
	case SB_PAGERIGHT:
		
		break;
	case SB_THUMBTRACK:
	case SB_THUMBPOSITION:
		if(nSBCode == SB_THUMBPOSITION)
			m_lPanSX = scrollInfo.nPos;
		else
			m_lPanSX = scrollInfo.nTrackPos;
		break;
		
	case SB_ENDSCROLL:
		return;
	}

	m_poScroll.x = m_lPanSX;
	m_Tracker[m_nIns_idx][m_nGrabIdx][0].GetZoom(&sx, &sy, &ex, &ey);

	diff_x = (long)sx - m_lPanSX;
	diff_y = (long)sy - m_lPanSY;

	m_lPanEX = (long)sx;
	m_lPanEY = (long)sy;

	scrollInfo.fMask = SIF_ALL;
	scrollInfo.nPos = m_lPanSX;
	scrollInfo.nMax = CAMERA_WIDTH - m_lImgWd;
	SetScrollInfo(SB_HORZ, &scrollInfo);
					
	for(int i = 0; i < TRACKER_MAX_NUM; i++)
	{
		m_Tracker[m_nIns_idx][m_nGrabIdx][i].GetZoom(&sx, &sy, &ex, &ey);
		m_Tracker[m_nIns_idx][m_nGrabIdx][i].GetRect(&win_left, &win_top, &win_right, &win_bottom);	

		//Zoom///////////////////////////

		img_rect = CRect((int)sx, (int)sy, (int)ex, (int)ey);
					

		ratio_y = ((double)img_rect.Height()) / ((double)969);
		ratio_x = ((double)img_rect.Width()) / ((double)199);

		diff_img_x = diff_x * ratio_x;
		diff_img_y = diff_y * ratio_y;

		sx0 = sx - diff_img_x;
		if(0 > sx0)
		{
			sx0 = 0;
			diff_img_x = sx;
			m_bPanDN = FALSE;
		}

		sy0 = sy - diff_img_y;
		if(0 > sy0)
		{
			sy0 = 0;
			diff_img_y = sy;
			m_bPanDN = FALSE;
		}

		ex0 = ex - diff_img_x;
		if(CAMERA_WIDTH < ex0)
		{
			ex0 = CAMERA_WIDTH;
			diff_img_x = ex - CAMERA_WIDTH;
			m_bPanDN = FALSE;
		}

		ey0 = ey - diff_img_y;
		if(CAMERA_HEIGHT < ey0)
		{
			ey0 = CAMERA_HEIGHT;
			diff_img_y = ey - CAMERA_HEIGHT;
			m_bPanDN = FALSE;
		}

					
		m_Tracker[m_nIns_idx][m_nGrabIdx][i].SetZoom(sx - diff_img_x, sy - diff_img_y, ex - diff_img_x, ey - diff_img_y);

		//Rect///////////////////////////
		diff_x = (long)(diff_img_x / ratio_x);
		diff_y = (long)(diff_img_y / ratio_y);

		m_Tracker[m_nIns_idx][m_nGrabIdx][i].SetRect(win_left + diff_x, win_top + diff_y, win_right + diff_x, win_bottom + diff_y);
	}	
#ifdef CAMERA					
	MdispPan(*pdlg->m_MilManage.GetMilDisplay(MAIN_DISP), m_poScroll.x, m_poScroll.y);
#endif
	CDialog::OnHScroll(nSBCode, nPos, pScrollBar);
}

void CDialogImageRect::OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar)
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.
	CLED_DisplayInspectDlg *pdlg = ((CLED_DisplayInspectDlg *)AfxGetApp()->m_pMainWnd);
	double sx, sy, ex, ey;
	double win_left, win_top, win_right, win_bottom;
	double ratio_y, ratio_x;
	double diff_img_x, diff_img_y;
	double sx0, sy0, ex0, ey0;
	long diff_x, diff_y;
	CRect img_rect;
	long diff;

	diff = m_lPanEY - m_lPanSY;
	
	SCROLLINFO scrollInfo;
	scrollInfo.cbSize = sizeof(SCROLLINFO);
	
	GetScrollInfo(SB_VERT, &scrollInfo);

	switch(nSBCode)
	{
	case SB_LINELEFT:
		m_lPanSY -= 2;
		
		break;
	case SB_LINERIGHT:
		m_lPanSY += 2;
		
		break;
	case SB_PAGELEFT:
		
		break;
	case SB_PAGERIGHT:
		
		break;
	case SB_THUMBTRACK:
	case SB_THUMBPOSITION:
		if(nSBCode == SB_THUMBPOSITION)
			m_lPanSY = scrollInfo.nPos;
		else
			m_lPanSY = scrollInfo.nTrackPos;
		break;
		
	case SB_ENDSCROLL:
		return;
	}

	m_poScroll.y = m_lPanSY;
	m_Tracker[m_nIns_idx][m_nGrabIdx][0].GetZoom(&sx, &sy, &ex, &ey);

	diff_x = (long)sx - m_lPanSX;
	diff_y = (long)sy - m_lPanSY;

	m_lPanEX = (long)sx;
	m_lPanEY = (long)sy;

	scrollInfo.fMask = SIF_ALL;
	scrollInfo.nPos = m_lPanSY;
	scrollInfo.nMax = CAMERA_HEIGHT - m_lImgHt;
	SetScrollInfo(SB_HORZ, &scrollInfo);
					
	for(int i = 0; i < TRACKER_MAX_NUM; i++)
	{
		m_Tracker[m_nIns_idx][m_nGrabIdx][i].GetZoom(&sx, &sy, &ex, &ey);
		m_Tracker[m_nIns_idx][m_nGrabIdx][i].GetRect(&win_left, &win_top, &win_right, &win_bottom);	

		//Zoom///////////////////////////

		img_rect = CRect((int)sx, (int)sy, (int)ex, (int)ey);
					

		ratio_y = ((double)img_rect.Height()) / ((double)969);
		ratio_x = ((double)img_rect.Width()) / ((double)199);

		diff_img_x = diff_x * ratio_x;
		diff_img_y = diff_y * ratio_y;

		sx0 = sx - diff_img_x;
		if(0 > sx0)
		{
			sx0 = 0;
			diff_img_x = sx;
			m_bPanDN = FALSE;
		}

		sy0 = sy - diff_img_y;
		if(0 > sy0)
		{
			sy0 = 0;
			diff_img_y = sy;
			m_bPanDN = FALSE;
		}

		ex0 = ex - diff_img_x;
		if(CAMERA_WIDTH < ex0)
		{
			ex0 = CAMERA_WIDTH;
			diff_img_x = ex - CAMERA_WIDTH;
			m_bPanDN = FALSE;
		}

		ey0 = ey - diff_img_y;
		if(CAMERA_HEIGHT < ey0)
		{
			ey0 = CAMERA_HEIGHT;
			diff_img_y = ey - CAMERA_HEIGHT;
			m_bPanDN = FALSE;
		}

					
		m_Tracker[m_nIns_idx][m_nGrabIdx][i].SetZoom(sx - diff_img_x, sy - diff_img_y, ex - diff_img_x, ey - diff_img_y);

		//Rect///////////////////////////
		diff_x = (long)(diff_img_x / ratio_x);
		diff_y = (long)(diff_img_y / ratio_y);

		m_Tracker[m_nIns_idx][m_nGrabIdx][i].SetRect(win_left + diff_x, win_top + diff_y, win_right + diff_x, win_bottom + diff_y);
	}
#ifdef CAMERA					
	MdispPan(*pdlg->m_MilManage.GetMilDisplay(MAIN_DISP), m_poScroll.x, m_poScroll.y);
#endif
	CDialog::OnVScroll(nSBCode, nPos, pScrollBar);
}

int CDialogImageRect::GetInsIdx()
{
	return m_nIns_idx;
}

void CDialogImageRect::SetInsIdx(int ins_idx)
{
	m_nIns_idx = ins_idx - 1;
}


int CDialogImageRect::GetGrabIdx()
{
	return m_nGrabIdx;
}

void CDialogImageRect::SetGrabIdx(int grab_idx)
{
	m_nGrabIdx = grab_idx - 1;
}

BOOL CDialogImageRect::PreTranslateMessage(MSG* pMsg)
{
	// TODO: 여기에 특수화된 코드를 추가 및/또는 기본 클래스를 호출합니다.
	if(pMsg->message ==WM_KEYDOWN)
	{
		if(pMsg->wParam == VK_RETURN || pMsg->wParam == VK_ESCAPE)
			return TRUE;
	}

	return CDialog::PreTranslateMessage(pMsg);
}
