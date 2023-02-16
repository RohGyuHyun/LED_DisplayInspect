// DialogNgView.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "LED_DisplayInspect.h"
#include "LED_DisplayInspectDlg.h"
#include "DialogNgView.h"
#include "afxdialogex.h"


// CDialogNgView 대화 상자입니다.

IMPLEMENT_DYNAMIC(CDialogNgView, CDialogEx)

CDialogNgView::CDialogNgView(CWnd* pParent /*=NULL*/)
	: CDialogEx(CDialogNgView::IDD, pParent)
{
	m_dZoomDefault = 0;
	m_dZoomRatio = 0;
	m_dZoomRatioOld = 0;
	m_dZoomRatio_RB = 0;
	m_bPanDN = 0;
	m_dZoomSX = 0;
	m_dZoomSY = 0;
	m_dZoomSX_RB = 0;
	m_dZoomSY_RB = 0;
	m_lPanSX = 0;
	m_lPanSY = 0;
	m_lPanEX = 0;
	m_lPanEY = 0;
	m_RsltIdx = 0;
	m_nDefListIdx = 0;
	m_nFileIdx = 0;
}

CDialogNgView::~CDialogNgView()
{
	delete m_dlgVerify;
}

void CDialogNgView::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_SCROLLBAR_NG_DISP, m_scrol_NgDisp);
	DDX_Control(pDX, IDC_DEVICE_IDX, m_btn_DeviceIdx);
	DDX_Control(pDX, IDC_LIST_BEFORE_RSLT, m_list_BeforeRslt);
	DDX_Control(pDX, IDC_SELECT_RSLT_FILE, m_btn_SelectRsltFile);
}


BEGIN_MESSAGE_MAP(CDialogNgView, CDialogEx)
	ON_BN_CLICKED(IDC_BUTTON1, &CDialogNgView::OnBnClickedButton1)
	ON_WM_SHOWWINDOW()
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONUP()
	ON_WM_MOUSEMOVE()
	ON_WM_VSCROLL()
	ON_NOTIFY(NM_DBLCLK, IDC_LIST_BEFORE_RSLT, &CDialogNgView::OnNMDblclkListBeforeRslt)
	ON_WM_MOUSEWHEEL()
END_MESSAGE_MAP()


// CDialogNgView 메시지 처리기입니다.


void CDialogNgView::OnBnClickedButton1()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	CLED_DisplayInspectDlg *pdlg = ((CLED_DisplayInspectDlg *)AfxGetApp()->m_pMainWnd);
	ShowWindow(SW_HIDE);
	m_dlgVerify->ShowWindow(SW_HIDE);
	pdlg->m_bBeforeView = FALSE;
}

double CDialogNgView::GetZoomRatio()
{
	return m_dZoomRatio;
}

HWND CDialogNgView::GetDispHwnd()
{
	return GetDlgItem(IDC_MAIN_FRM_DISP)->m_hWnd;
}

void CDialogNgView::GetNgDispHwnd(HWND *hWnd)
{
	//HWND ng[3];
	hWnd[0] = GetDlgItem(IDC_STATIC_NG_VIEW_NG_IMAGE_0)->m_hWnd;
	hWnd[1] = GetDlgItem(IDC_STATIC_NG_VIEW_NG_IMAGE_1)->m_hWnd;
	hWnd[2] = GetDlgItem(IDC_STATIC_NG_VIEW_NG_IMAGE_2)->m_hWnd;

	//return ng;
}

void CDialogNgView::SetImageLoadPath(CString strPath)
{
	m_strImageLoadPath.Format(_T("%s"), strPath);
}

void CDialogNgView::SetVerifyRslt(BOOL isNg, int nDeviceIdxX, int nDeviceIdxY, int nGrabIdx)
{
	CLED_DisplayInspectDlg *pdlg = ((CLED_DisplayInspectDlg *)AfxGetApp()->m_pMainWnd);
	m_bVerifyStart = TRUE;
	if(m_bVerifyComp)
	{
		pdlg->SetVerifyRslt(isNg, nDeviceIdxX, nDeviceIdxY, nGrabIdx, m_nFileIdx);
	}
	else
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
			rsltPara.dWidth[i] = 0.;
			rsltPara.dAng[i] = 0.;//20161017 ngh
			rsltPara.nGrabIdx[i] = 0;
			rsltPara.nInspGrabIdx[i] = 0;
			rsltPara.bAddNg[i] = FALSE;
			rsltPara.bNg[i] = FALSE;
		}
		rsltPara = pdlg->GetBeforeRslt();
		BOOL isCompare = FALSE;;

		if(isNg)
		{
			for(int i = 0; i < rsltPara.nNgCnt; i++)
			{
				if((rsltPara.nDeviceIdx[i].x == nDeviceIdxX) && (rsltPara.nDeviceIdx[i].y == nDeviceIdxY) && (rsltPara.nGrabIdx[i] == nGrabIdx))
				{
					isCompare = TRUE;
					break;
				}
			}
		}

		if(!isCompare)
		{
			m_VerifyData[m_nVerifyCnt].isNg = isNg;
			m_VerifyData[m_nVerifyCnt].nDeviceIdx.SetPoint(nDeviceIdxX, nDeviceIdxY);
			m_VerifyData[m_nVerifyCnt].nGrabIdx = nGrabIdx;
			m_nVerifyCnt++;
		}
		else
		{
			int idx = 0;
			for(int i = 0; i < m_nVerifyCnt; i++)
			{
				if((m_VerifyData[i].nDeviceIdx.x == nDeviceIdxX) && (m_VerifyData[i].nDeviceIdx.y == nDeviceIdxY) && (m_VerifyData[i].nGrabIdx == nGrabIdx))
				{
					for(int j = 0; j < m_nVerifyCnt - i - 1; j++)
					{
						idx = j + 1;
						m_VerifyData[i + j].isNg = m_VerifyData[idx].isNg;
						m_VerifyData[i + j].nDeviceIdx = m_VerifyData[idx].nDeviceIdx;
						m_VerifyData[i + j].nGrabIdx = m_VerifyData[idx].nGrabIdx;
						
					}
					m_nVerifyCnt--;
				}
			}
		}
	}
}

void CDialogNgView::SetRsltPara(int nDeviceIdxX, int nDeviceIdxY, int nGrabIdx, int nInspGrabIdx, typeInsRsltPara rsltPara)
{
	CLED_DisplayInspectDlg *pdlg = ((CLED_DisplayInspectDlg *)AfxGetApp()->m_pMainWnd);
	CString file;
	m_RsltPara.nNgCnt = 0;
	for(int i = 0; i < MAX_RSLT_NUM; i++)
	{
		m_RsltPara.nDeviceIdx[i].SetPoint(0, 0);
		m_RsltPara.dArea[i] = 0.;
		m_RsltPara.dCentX[i] = 0.;
		m_RsltPara.dCentY[i] = 0.;
		m_RsltPara.dHeight[i] = 0.;
		m_RsltPara.dWidth[i] = 0.;
		m_RsltPara.dAng[i] = 0.;//20161017 ngh
		m_RsltPara.dBreadth[i] = 0.;
		m_RsltPara.nGrabIdx[i] = 0;
		m_RsltPara.nInspGrabIdx[i] = 0;
		m_RsltPara.bAddNg[i] = FALSE;
		m_RsltPara.bNg[i] = FALSE;
		m_RsltPara.bAlign[i] = FALSE;
	}

	//memset(&m_RsltPara, NULL, sizeof(typeInsRsltPara));
	for(int i = 0; i < rsltPara.nNgCnt; i++)
	{
		if((rsltPara.nDeviceIdx[i].x == (int)(nDeviceIdxX / pdlg->m_dlgTeach->GetTeachPara().TeachGrabCnt)) && (rsltPara.nDeviceIdx[i].y == nDeviceIdxY))
		{
			if((rsltPara.nGrabIdx[i] == nGrabIdx))// && (rsltPara.nInspGrabIdx[i] == nInspGrabIdx))
			{
				m_RsltPara.dCentX[m_RsltPara.nNgCnt] = rsltPara.dCentX[i];
				m_RsltPara.dCentY[m_RsltPara.nNgCnt] = rsltPara.dCentY[i];
				m_RsltPara.dWidth[m_RsltPara.nNgCnt] = rsltPara.dWidth[i];
				m_RsltPara.dHeight[m_RsltPara.nNgCnt] = rsltPara.dHeight[i];
				m_RsltPara.dLong[m_RsltPara.nNgCnt] = rsltPara.dLong[i];
				m_RsltPara.dShort[m_RsltPara.nNgCnt] = rsltPara.dShort[i];
				m_RsltPara.dAng[m_RsltPara.nNgCnt] = rsltPara.dAng[i];
				m_RsltPara.dBreadth[m_RsltPara.nNgCnt] = rsltPara.dBreadth[i];

				m_RsltPara.bNg[m_RsltPara.nNgCnt] = rsltPara.bAlign[i];
				m_RsltPara.bNg[m_RsltPara.nNgCnt] = rsltPara.bNg[i];
				m_RsltPara.bAddNg[m_RsltPara.nNgCnt] = rsltPara.bAddNg[i];

				m_RsltPara.nDeviceIdx[m_RsltPara.nNgCnt].SetPoint(nDeviceIdxX, nDeviceIdxY);
				m_RsltPara.nGrabIdx[m_RsltPara.nNgCnt] = nGrabIdx;
				m_RsltPara.nInspGrabIdx[m_RsltPara.nNgCnt] = nInspGrabIdx;

				m_RsltPara.nNgCnt++;
			}
		}
	}
	if(m_RsltPara.nNgCnt == 0)
	{
		SetImageLoadPath(_T(""));
	}

	if(m_RsltPara.nNgCnt > 3)
	{
		m_scrol_NgDisp.SetScrollRange(0, m_RsltPara.nNgCnt - 3);
	}
	else
	{
		m_scrol_NgDisp.SetScrollRange(0, 0);
	}
	m_RsltIdx = 0;
}

void CDialogNgView::ListInit()
{
	CLED_DisplayInspectDlg *pdlg = ((CLED_DisplayInspectDlg *)AfxGetApp()->m_pMainWnd);
	CString before_file_name;
	m_list_BeforeRslt.DeleteAllItems();
	for(int i = 0; i < pdlg->GetBeforeRsltCnt(); i++)
	{
		before_file_name.Format(_T("%s_%s"), pdlg->GetStartDate(i), pdlg->GetStartTime(i));
		m_list_BeforeRslt.InsertItem(i, before_file_name);
	}
}

//20161129 ngh
void CDialogNgView::SetListIdxInit()
{
	m_nFileIdx = 0;
	m_nDefListIdx = 0;
	m_nVerifyCnt = 0;
	m_bVerifyStart = FALSE;
	m_bVerifyComp = FALSE;
}

void CDialogNgView::OnShowWindow(BOOL bShow, UINT nStatus)
{
	CDialogEx::OnShowWindow(bShow, nStatus);
	CLED_DisplayInspectDlg *pdlg = ((CLED_DisplayInspectDlg *)AfxGetApp()->m_pMainWnd);
	CString tmpPath_Name;
	if(bShow)
	{
		m_nFileIdx = 0;
		m_nDefListIdx = 0;
		m_nVerifyCnt = 0;
		m_bVerifyStart = FALSE;
		m_bVerifyComp = FALSE;
		ListInit();

		SetGrid();
		for(int i = 0; i < (pdlg->m_dlgTeach->GetTeachPara().TeachDeviceCntY * pdlg->m_dlgTeach->GetTeachPara().TeachOneGrabInspCnt); i++)
		{
			for(int j = 0; j < pdlg->m_dlgTeach->GetTeachPara().TeachDeviceCntX; j++)
			{
				for(int k = 0; k < pdlg->m_dlgTeach->GetTeachPara().TeachGrabCnt; k++)
				{
					if(i < pdlg->m_dlgTeach->GetTeachPara().TeachArrayCntY)
					{
						GridDrow(TRUE, (j * pdlg->m_dlgTeach->GetTeachPara().TeachGrabCnt) + k, i, k);
					}
					else
					{
						GridDrow(5, (j * pdlg->m_dlgTeach->GetTeachPara().TeachGrabCnt), i, k);
					}
				}
			}
		}
		
#ifdef CAMERA
		MdispControl(*pdlg->m_MilManage.GetMilDisplay(NG_VIEW_DISP), M_OVERLAY_CLEAR, M_DEFAULT);
#endif
		pdlg->m_RsltIdx = 0;
		InitRsltPara();
		pdlg->SetLoadRsltPara(0);
		
		if(m_RsltPara.nNgCnt > 0)
		{
			GridDrow();
			//SetGridDbClick(m_RsltPara.nDeviceIdx[0].x, m_RsltPara.nDeviceIdx[0].y, m_RsltPara.nGrabIdx[0]);
			m_grid.SetGridOkNg_LED(2, (m_RsltPara.nDeviceIdx[0].x * pdlg->m_dlgTeach->GetTeachPara().TeachGrabCnt) + m_RsltPara.nGrabIdx[0], m_RsltPara.nDeviceIdx[0].y, m_RsltPara.nGrabIdx[0]);//m_RsltPara.nDeviceIdx[0].x, m_RsltPara.nDeviceIdx[0].y, m_RsltPara.nGrabIdx[0]);
			pdlg->NGDisplay(m_RsltPara.nDeviceIdx[0].x, m_RsltPara.nDeviceIdx[0].y, m_RsltPara.nGrabIdx[0], m_RsltPara.nInspGrabIdx[0], NG_VIEW_DISP, m_RsltPara);
		}
		m_grid.RedrawAll();
		
		m_btn_SelectRsltFile.SetWindowTextA(pdlg->GetBeforeRsltFilePath(0));

		LV_ITEM lvitem;
		CString temp;
		char temp1[512]; 
		temp = m_list_BeforeRslt.GetItemText(0, 0);
		sprintf(temp1, "%s", temp);
		lvitem.mask=LVIF_TEXT | LVIF_IMAGE;
		lvitem.iItem=0;
		lvitem.iSubItem=0;
		lvitem.iImage = 1;
		lvitem.pszText = temp1;
		m_list_BeforeRslt.SetItem(&lvitem);
	}
	else
	{
		pdlg->NGAllDisplay();
	}

	// TODO: 여기에 메시지 처리기 코드를 추가합니다.
}

//20161017 ngh
void CDialogNgView::InitRsltPara()
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
	}
}

BOOL CDialogNgView::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  여기에 추가 초기화 작업을 추가합니다.

	CRect rect;
	GetDlgItem(IDC_MAIN_FRM_DISP)->GetWindowRect(rect);

	m_dZoomRatio = (double)rect.Width() / (double)CAMERA_WIDTH;
	m_dZoomDefault = m_dZoomRatio;
	m_dZoomRatioOld = m_dZoomDefault;
	m_ZoomImage.SetdRectLTRB(0, 0, CAMERA_WIDTH, CAMERA_HEIGHT);
	m_bPanDN = FALSE;
	InitGrid();

	SCROLLINFO scinfo;

	scinfo.cbSize = sizeof(scinfo);
	scinfo.fMask = SIF_ALL;
	scinfo.nMin = 0;
	scinfo.nMax = 0;
	scinfo.nPage = 1;
	scinfo.nTrackPos = 0;
	scinfo.nPos = 0;
	m_scrol_NgDisp.SetScrollInfo(&scinfo);

	m_dlgVerify = new CDialogVerify();
	m_dlgVerify->Create(IDD_DIALOG_VERIFY, this);


	// 리스트 스타일 설정
	//m_list_BeforeRslt.SetExtendedStyle(LVS_EX_GRIDLINES);
	//m_list_BeforeRslt.InsertColumn(0, _T("Before Rslt"), LVCFMT_LEFT, 370, -1);

	LV_COLUMN m_lvColumn;
	CBitmap bitmap;
	CFont m_font;
	CImageList *img_list_slct;
	img_list_slct = new CImageList;
	img_list_slct->Create(16, 16, ILC_COLOR4, 3, 3);

	m_font.CreatePointFont(100, _T("굴림"));
	bitmap.LoadBitmap(IDB_BITMAP_IO_OFF);
	img_list_slct->Add(&bitmap, RGB(0, 0, 0));
	bitmap.DeleteObject();
	bitmap.LoadBitmap(IDB_BITMAP_IO_ON);
	img_list_slct->Add(&bitmap, RGB(0, 0, 0));
	bitmap.DeleteObject();
	m_list_BeforeRslt.SetImageList(img_list_slct, LVSIL_SMALL);

	m_lvColumn.mask= LVCF_FMT | LVCF_SUBITEM | LVCF_TEXT | LVCF_WIDTH;
	m_lvColumn.fmt=LVCFMT_LEFT;
	m_lvColumn.pszText=_T("Before Rslt");
	m_lvColumn.iSubItem=0;
	m_lvColumn.cx=370;
	m_list_BeforeRslt.InsertColumn(0, &m_lvColumn);
	m_list_BeforeRslt.SetFont(&m_font, TRUE);

	m_list_BeforeRslt.DeleteAllItems();

	delete img_list_slct;

	return TRUE;  // return TRUE unless you set the focus to a control
	// 예외: OCX 속성 페이지는 FALSE를 반환해야 합니다.
}

BEGIN_EVENTSINK_MAP(CDialogNgView, CDialogEx)
	ON_EVENT(CDialogNgView, IDC_BTNENHCTRL_ZOOMIN, DISPID_CLICK, CDialogNgView::ClickBtnenhctrlZoomin, VTS_NONE)
	ON_EVENT(CDialogNgView, IDC_BTNENHCTRL_ZOOMOUT, DISPID_CLICK, CDialogNgView::ClickBtnenhctrlZoomout, VTS_NONE)
	ON_EVENT(CDialogNgView, IDC_BTNENHCTRL_ONE, DISPID_CLICK, CDialogNgView::ClickBtnenhctrlOne, VTS_NONE)
	ON_EVENT(CDialogNgView, IDC_BTNENHCTRL_FIT, DISPID_CLICK, CDialogNgView::ClickBtnenhctrlFit, VTS_NONE)
	ON_EVENT(CDialogNgView, IDC_BTNENHCTRL_FSLT_FILE_COMP, DISPID_CLICK, CDialogNgView::ClickBtnenhctrlFsltFileComp, VTS_NONE)
	ON_EVENT(CDialogNgView, IDC_BTNENHCTRL_FSLT_FILE_COMP2, DISPID_CLICK, CDialogNgView::ClickBtnenhctrlFsltFileComp2, VTS_NONE)
END_EVENTSINK_MAP()

void CDialogNgView::ClickBtnenhctrlZoomin()
{
	// TODO: 여기에 메시지 처리기 코드를 추가합니다.
	m_dZoomRatio = m_dZoomRatio + 0.2;
	ZoomDisplay();
}

void CDialogNgView::ClickBtnenhctrlZoomout()
{
	// TODO: 여기에 메시지 처리기 코드를 추가합니다.
	m_dZoomRatio = m_dZoomRatio - 0.2;
	ZoomDisplay();
}

void CDialogNgView::ClickBtnenhctrlOne()
{
	// TODO: 여기에 메시지 처리기 코드를 추가합니다.
	m_dZoomRatio = 1;
	ZoomDisplay();
}

void CDialogNgView::ClickBtnenhctrlFit()
{
	// TODO: 여기에 메시지 처리기 코드를 추가합니다.
	m_dZoomRatio = m_dZoomDefault;
	ZoomDisplay();
}


void CDialogNgView::OnLButtonDown(UINT nFlags, CPoint point)
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.
	m_lPanSX = point.x;
	m_lPanSY = point.y;

	CDialogEx::OnLButtonDown(nFlags, point);
}


void CDialogNgView::OnLButtonUp(UINT nFlags, CPoint point)
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.
	CDialogEx::OnLButtonUp(nFlags, point);
}


void CDialogNgView::OnMouseMove(UINT nFlags, CPoint point)
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.
	CLED_DisplayInspectDlg *pdlg = ((CLED_DisplayInspectDlg *)AfxGetApp()->m_pMainWnd);

	if(nFlags == MK_LBUTTON)
	{
		double diff_x = 0., diff_y = 0., diff_img_x = 0., diff_img_y = 0.;

		CRect rect;
		long ex = 0, ey = 0;

		GetDlgItem(IDC_MAIN_FRM_DISP)->GetClientRect(rect);

		diff_x = point.x - m_lPanSX;
		diff_y = point.y - m_lPanSY;

		m_lPanEX = point.x;
		m_lPanEY = point.y;

		m_lPanSX = m_lPanEX;
		m_lPanSY = m_lPanEY;

		diff_img_x = diff_x / m_dZoomRatio;
		diff_img_y = diff_y / m_dZoomRatio;

		m_dZoomSX = m_dZoomSX - diff_img_x;
		m_dZoomSY = m_dZoomSY - diff_img_y;

		ex = (long)(m_dZoomSX + (rect.Width() / m_dZoomRatio));
		ey = (long)(m_dZoomSY + (rect.Height() / m_dZoomRatio));

		if(0 > m_dZoomSX)
		{
			m_dZoomSX = 0;
		}
		if(0 > m_dZoomSY)
		{
			m_dZoomSY = 0;
		}
		if(CAMERA_WIDTH < ex)
		{
			m_dZoomSX = m_dZoomDefault - (rect.Width() / m_dZoomRatio);			
		}
		if(CAMERA_HEIGHT < ey)
		{
			m_dZoomSY = m_dZoomDefault - (rect.Height() / m_dZoomRatio);
		}
#ifdef CAMERA
		MdispPan(*pdlg->m_MilManage.GetMilDisplay(NG_VIEW_DISP), (int)m_dZoomSX, (int)m_dZoomSY);
#endif
		CDialog::OnMouseMove(nFlags, point);
	}

	CDialogEx::OnMouseMove(nFlags, point);
}

void CDialogNgView::ZoomDisplay()
{
	CLED_DisplayInspectDlg *pdlg = ((CLED_DisplayInspectDlg *)AfxGetApp()->m_pMainWnd);
	CRect rect;
	long long width = 0, height = 0;

	GetDlgItem(IDC_MAIN_FRM_DISP)->GetClientRect(rect);
	double img_disp_wd_old = 0., img_disp_ht_old = 0., img_disp_wd = (double)width, img_disp_ht = (double)height;
	
	if(m_dZoomDefault >= m_dZoomRatio)
	{
		m_dZoomRatio = m_dZoomDefault;
		m_dZoomSX = 0;
		m_dZoomSY = 0;
	}
	else
	{
		//기존 pan position 찾기
		img_disp_wd_old = CAMERA_WIDTH * (m_dZoomDefault / m_dZoomRatioOld);
		img_disp_ht_old = CAMERA_HEIGHT * (m_dZoomDefault / m_dZoomRatioOld);
		img_disp_wd = CAMERA_WIDTH		* (m_dZoomDefault / m_dZoomRatio);
		img_disp_ht = CAMERA_HEIGHT * (m_dZoomDefault / m_dZoomRatio);

		m_dZoomSX = m_dZoomSX + (img_disp_wd_old - img_disp_wd) * 0.5;
		m_dZoomSY = m_dZoomSY + (img_disp_ht_old - img_disp_ht) * 0.5;

		m_dZoomSX_RB = m_dZoomSX + (img_disp_wd_old - img_disp_wd) * 0.5;
		m_dZoomSY_RB = m_dZoomSY + (img_disp_ht_old - img_disp_ht) * 0.5;		
	}

	m_dZoomRatioOld = m_dZoomRatio;

	m_dZoomRatio_RB = m_dZoomRatio;
#ifdef CAMERA
	MdispZoom(*pdlg->m_MilManage.GetMilDisplay(NG_VIEW_DISP), m_dZoomRatio, m_dZoomRatio);
	MdispPan(*pdlg->m_MilManage.GetMilDisplay(NG_VIEW_DISP), (int)m_dZoomSX, (int)m_dZoomSY);
#endif
}


void CDialogNgView::InitGrid()
{
	CLED_DisplayInspectDlg *pdlg = ((CLED_DisplayInspectDlg *)AfxGetApp()->m_pMainWnd);
	m_grid.AttachGrid(this, IDC_STATIC_GRID, pdlg->m_dlgTeach->GetTeachPara().TeachDeviceCntX, pdlg->m_dlgTeach->GetTeachPara().TeachDeviceCntY, pdlg->m_dlgTeach->GetTeachPara().TeachGrabCnt);
}

void CDialogNgView::DeleteGrid()
{
	for(int i = 0 ; i < m_grid.m_GI->m_numberCols; i++)
	{
		for(int j = 0; j < m_grid.m_GI->m_numberRows; j++)
		{
			m_grid.DeleteCell(i, j);
		}
	}
}

void CDialogNgView::SetGrid(int isOK, int nDeviceIdxX, int nDeviceIdxY, int nGrabIdx)
{
	if(isOK == 7)
	{
		GridDrow(isOK, nDeviceIdxX, nDeviceIdxY, nGrabIdx);
	}
	else if(isOK == 8)
	{
		GridDrow(isOK, nDeviceIdxX, nDeviceIdxY, nGrabIdx);
	}
	else
	{
		GridDrow(isOK, nDeviceIdxX, nDeviceIdxY, nGrabIdx);
	}
}

void CDialogNgView::SetGrid()
{
	CLED_DisplayInspectDlg *pdlg = ((CLED_DisplayInspectDlg *)AfxGetApp()->m_pMainWnd);
	DeleteGrid();
	m_grid.RedrawAll();

	int cnt = 0;
	if(pdlg->m_dlgTeach->GetTeachPara().TeachArrayCntY % pdlg->m_dlgTeach->GetTeachPara().TeachOneGrabInspCnt != 0.)
	{
		cnt = (pdlg->m_dlgTeach->GetTeachPara().TeachArrayCntY % pdlg->m_dlgTeach->GetTeachPara().TeachOneGrabInspCnt);
	}


	//20161221 수식때문에 변경한 부분입니다. 넘겨주는 인자값을 함수에 하나 더 추가 하였습니다 (1번촬상당 촬영하는 제품의수)
	//m_grid.SetGrid_LED(pdlg->m_dlgTeach->GetTeachPara().TeachDeviceCntX, pdlg->m_dlgTeach->GetTeachPara().TeachDeviceCntY * pdlg->m_dlgTeach->GetTeachPara().TeachOneGrabInspCnt, pdlg->m_dlgTeach->GetTeachPara().TeachGrabCnt);
	m_grid.SetGrid_LED(pdlg->m_dlgTeach->GetTeachPara().TeachDeviceCntX, pdlg->m_dlgTeach->GetTeachPara().TeachDeviceCntY, pdlg->m_dlgTeach->GetTeachPara().TeachGrabCnt,pdlg->m_dlgTeach->GetTeachPara().TeachInspGrabCnt);
	m_grid.RedrawAll();

	for(int i = 0; i < (pdlg->m_dlgTeach->GetTeachPara().TeachDeviceCntY * pdlg->m_dlgTeach->GetTeachPara().TeachOneGrabInspCnt /*+ cnt*/); i++) // 20170109 nht
	{
		for(int j = 0; j < pdlg->m_dlgTeach->GetTeachPara().TeachDeviceCntX; j++)
		{
			for(int k = 0; k < pdlg->m_dlgTeach->GetTeachPara().TeachGrabCnt; k++)
			{
				if(i < pdlg->m_dlgTeach->GetTeachPara().TeachArrayCntY)
				{
					GridDrow(TRUE, j, i, k);
				}
				else
				{
					m_grid.SetGridOkNg_LED(5, (j * pdlg->m_dlgTeach->GetTeachPara().TeachGrabCnt) + k, i, k);
				}
			}
		}
	}
	m_grid.RedrawAll();
}

void CDialogNgView::GridDrow()
{
	CLED_DisplayInspectDlg *pdlg = ((CLED_DisplayInspectDlg *)AfxGetApp()->m_pMainWnd);
	typeInsRsltPara rsltPara;
	rsltPara.nNgCnt = 0;
	for(int i = 0; i < MAX_RSLT_NUM; i++)
	{
		rsltPara.nDeviceIdx[i].SetPoint(0, 0);
		rsltPara.dArea[i] = 0.;
		rsltPara.dCentX[i] = 0.;
		rsltPara.dCentY[i] = 0.;
		rsltPara.dHeight[i] = 0.;
		rsltPara.dWidth[i] = 0.;
		rsltPara.dAng[i] = 0.;//20161017 ngh
		rsltPara.dLong[i] = 0.;
		rsltPara.dShort[i] = 0.;
		rsltPara.dBreadth[i] = 0.;
		rsltPara.nGrabIdx[i] = 0;
		rsltPara.nInspGrabIdx[i] = 0;
		rsltPara.bAddNg[i] = FALSE;
		rsltPara.bNg[i] = FALSE;
	}
	rsltPara = pdlg->GetBeforeRslt();

	if(pdlg->GetBeforeRsltCnt() > 0)
	{
		for(int i = 0; i < rsltPara.nNgCnt; i++)
		{
			GridDrow(FALSE, (rsltPara.nDeviceIdx[i].x * pdlg->m_dlgTeach->GetTeachPara().TeachGrabCnt) + rsltPara.nGrabIdx[i], rsltPara.nDeviceIdx[i].y, rsltPara.nGrabIdx[i]);
		}
	}
}

void CDialogNgView::GridDrow(int isOK, int nDeviceIdxX, int nDeviceIdxY, int nGrabIdx)
{
	m_grid.SetGridOkNg_LED(isOK, nDeviceIdxX, nDeviceIdxY, nGrabIdx);
	//m_grid.RedrawAll();
}

void CDialogNgView::SetGridRClick(int col, int row, int grab_idx, BOOL isOK)
{
	//if(!m_dlgVerify->IsWindowVisible())
	//20161010 ngh
	if(!m_dlgVerify->IsWindowVisible() && IsWindowVisible() )
	{
		m_dlgVerify->SetIdx(col, row, grab_idx, isOK);
		m_dlgVerify->ShowWindow(SW_SHOW);
	}
}

void CDialogNgView::SetGridDbClick(int col, int row, int grab_idx)
{
	CLED_DisplayInspectDlg *pdlg = ((CLED_DisplayInspectDlg *)AfxGetApp()->m_pMainWnd);
	CString temp;
	int d_x,d_y,g_i,ig_i;
	
	int ntemp;
	ntemp = col - 1;
	d_x = col;
	d_y = row;

	g_i = grab_idx;
	ig_i = 0;

	//X : 00 Y : 00 G : 00

	//SetRsltPara(d_x - 1, d_y - 1, g_i - 1, ig_i, pdlg->GetBeforeRslt());

	CString tmpPath_Name, temp_path, temp_name, strLog;

	if(!pdlg->GetStartDate(m_nDefListIdx).IsEmpty() && !pdlg->GetStartTime(m_nDefListIdx).IsEmpty())
	{
		temp_path.Format(_T("%s%s\\%s\\%s\\%s\\"), RESULT_PATH, GRAB_IMAGE_SAVE_PATH, pdlg->GetStartDate(m_nDefListIdx), pdlg->GetSelectModel().cName, pdlg->GetStartTime(m_nDefListIdx));
		temp_name.Format(_T("X_%02d_Y_%02d_G_%02d_I_0.bmp"),  (int)(col / pdlg->m_dlgTeach->GetTeachPara().TeachGrabCnt), row, grab_idx);
		tmpPath_Name.Format(_T("%s%s"), temp_path, temp_name);
		m_strImageLoadPath.Format(_T(""));
		if(pdlg->FileSearch(temp_path, temp_name))
		{
			m_strImageLoadPath.Format(_T("%s"), tmpPath_Name);
#ifdef CAMERA
			MbufImport((char *)(LPCTSTR)tmpPath_Name, M_BMP, M_LOAD, *pdlg->m_MilManage.GetMilSystem(), pdlg->m_MilManage.GetMilDispBuf(NG_VIEW_DISP));
#endif
		}
		else
		{
			strLog.Format(_T("%s File Search Fail !!"), tmpPath_Name);
			AfxMessageBox(strLog);
#ifdef CAMERA
			MbufClear(*pdlg->m_MilManage.GetMilDispBuf(NG_VIEW_DISP), 0x00);
#endif
		}
	}

	/*if(!m_strImageLoadPath.IsEmpty())
	{
		tmpPath_Name.Format(_T("%s"), m_strImageLoadPath);
#ifdef CAMERA
		MbufImport((char *)(LPCTSTR)tmpPath_Name, M_BMP, M_LOAD, *pdlg->m_MilManage.GetMilSystem(), pdlg->m_MilManage.GetMilDispBuf(NG_VIEW_DISP));
#endif
	}
	else
	{
		tmpPath_Name.Format(_T("%s%s\\%s\\%s\\%s\\X_%02d_Y_%02d_G_%02d_I_0.bmp"), RESULT_PATH, GRAB_IMAGE_SAVE_PATH, pdlg->GetStartDate(m_nDefListIdx), pdlg->GetSelectModel().cName, pdlg->GetStartTime(m_nDefListIdx), col, row, grab_idx);
#ifdef CAMERA
		MbufImport((char *)(LPCTSTR)tmpPath_Name, M_BMP, M_LOAD, *pdlg->m_MilManage.GetMilSystem(), pdlg->m_MilManage.GetMilDispBuf(NG_VIEW_DISP));
#endif
	}*/

	if(this->IsWindowVisible())
	{
#ifdef CAMERA
		MdispControl(*pdlg->m_MilManage.GetMilDisplay(NG_VIEW_DISP), M_OVERLAY_CLEAR, M_DEFAULT);
#endif
		pdlg->m_RsltIdx = 0;
		m_scrol_NgDisp.SetScrollPos(0);
		pdlg->NGDisplay(m_RsltPara.nDeviceIdx[0].x, m_RsltPara.nDeviceIdx[0].y, m_RsltPara.nGrabIdx[0], m_RsltPara.nInspGrabIdx[0], NG_VIEW_DISP, m_RsltPara);
		//GridDrow();

		//20170104 ngh
		m_grid.SetGridOkNg_LED(2, col/* * pdlg->m_dlgTeach->GetTeachPara().TeachGrabCnt) + grab_idx*/, row, grab_idx);//m_RsltPara.nDeviceIdx[0].x, m_RsltPara.nDeviceIdx[0].y, m_RsltPara.nGrabIdx[0]);
		
		m_grid.RedrawAll();
		temp.Format(_T("X = %02d, Y = %02d, G = %02d"), (int)(col / pdlg->m_dlgTeach->GetTeachPara().TeachGrabCnt) + 1/*col + 1*/, row + 1, grab_idx + 1);
		m_btn_DeviceIdx.SetWindowTextA(temp);
	}
}

void CDialogNgView::OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar)
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.
	CLED_DisplayInspectDlg *pdlg = ((CLED_DisplayInspectDlg *)AfxGetApp()->m_pMainWnd);
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

					if(scrinfo.nPos < (m_RsltPara.nNgCnt - 2) && scrinfo.nPos > -1)
					{
						// 스크롤바의 위치를 변경한다.
						pdlg->NGDisplay(m_RsltPara.nDeviceIdx[0].x, m_RsltPara.nDeviceIdx[0].y, m_RsltPara.nGrabIdx[0], m_RsltPara.nInspGrabIdx[0], NG_VIEW_DISP, m_RsltPara);
						pScrollBar->SetScrollPos(m_RsltIdx);
						m_RsltIdx = scrinfo.nPos;
					}
				}
			}
		}
	}
	CDialogEx::OnVScroll(nSBCode, nPos, pScrollBar);
}


void CDialogNgView::OnNMDblclkListBeforeRslt(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMITEMACTIVATE pNMItemActivate = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	NM_LISTVIEW* pNMListView = (NM_LISTVIEW*)pNMHDR;
	CLED_DisplayInspectDlg *pdlg = ((CLED_DisplayInspectDlg *)AfxGetApp()->m_pMainWnd);

	int idx = pNMListView->iItem;

	if((pdlg->GetBeforeRsltCnt() < idx) || (0 > idx))  //20161014 sc && -> || 조건 변경
	{
		return;
	}

	if(!m_bVerifyStart)
	{
		m_nDefListIdx = idx;
		m_nFileIdx = idx;

		SetGrid();
		for(int i = 0; i < (pdlg->m_dlgTeach->GetTeachPara().TeachDeviceCntY * pdlg->m_dlgTeach->GetTeachPara().TeachOneGrabInspCnt); i++)
		{
			for(int j = 0; j < pdlg->m_dlgTeach->GetTeachPara().TeachDeviceCntX; j++)
			{
				for(int k = 0; k < pdlg->m_dlgTeach->GetTeachPara().TeachGrabCnt; k++)
				{
					if(i < pdlg->m_dlgTeach->GetTeachPara().TeachArrayCntY)
					{
						GridDrow(TRUE, (j * pdlg->m_dlgTeach->GetTeachPara().TeachGrabCnt) + k, i, k);
					}
					else
					{
						GridDrow(5, (j * pdlg->m_dlgTeach->GetTeachPara().TeachGrabCnt), i, k);
					}
				}
			}
		}
		m_grid.RedrawAll();

#ifdef CAMERA
		MdispControl(*pdlg->m_MilManage.GetMilDisplay(NG_VIEW_DISP), M_OVERLAY_CLEAR, M_DEFAULT);
#endif
		pdlg->m_RsltIdx = 0;
		InitRsltPara();
		pdlg->SetLoadRsltPara(idx);
		
		if(m_RsltPara.nNgCnt > 0)
		{
			GridDrow();
			m_grid.SetGridOkNg_LED(2, (m_RsltPara.nDeviceIdx[0].x * pdlg->m_dlgTeach->GetTeachPara().TeachGrabCnt) + m_RsltPara.nGrabIdx[0], m_RsltPara.nDeviceIdx[0].y, m_RsltPara.nGrabIdx[0]);//m_RsltPara.nDeviceIdx[0].x, m_RsltPara.nDeviceIdx[0].y, m_RsltPara.nGrabIdx[0]);
			pdlg->NGDisplay(m_RsltPara.nDeviceIdx[0].x, m_RsltPara.nDeviceIdx[0].y, m_RsltPara.nGrabIdx[0], m_RsltPara.nInspGrabIdx[0], NG_VIEW_DISP, m_RsltPara);
		}
		m_grid.RedrawAll();

		m_btn_SelectRsltFile.SetWindowTextA(pdlg->GetBeforeRsltFilePath(idx));

		//ListInit();
	
		LV_ITEM lvitem;
		CString temp;
		char temp1[512]; 

		for(int i = 0; i < pdlg->GetBeforeRsltCnt(); i++)
		{
			temp = m_list_BeforeRslt.GetItemText(i, 0);
			sprintf(temp1, "%s", temp);
			lvitem.mask=LVIF_TEXT | LVIF_IMAGE;
			lvitem.iItem=i;
			lvitem.iSubItem=0;
			lvitem.iImage = 0;
			lvitem.pszText = temp1;
			m_list_BeforeRslt.SetItem(&lvitem);
		}


		temp = m_list_BeforeRslt.GetItemText(idx, 0);
		sprintf(temp1, "%s", temp);
		lvitem.mask=LVIF_TEXT | LVIF_IMAGE;
		lvitem.iItem=idx;
		lvitem.iSubItem=0;
		lvitem.iImage = 1;
		lvitem.pszText = temp1;
		m_list_BeforeRslt.SetItem(&lvitem);
	}
	else
	{
		AfxMessageBox(_T("Verify Comp Error"));
	}
	
	GetDlgItem(IDC_MAIN_FRM_DISP)->SetFocus();
	*pResult = 0;
}


void CDialogNgView::ClickBtnenhctrlFsltFileComp()
{
	// TODO: 여기에 메시지 처리기 코드를 추가합니다.
	CLED_DisplayInspectDlg *pdlg = ((CLED_DisplayInspectDlg *)AfxGetApp()->m_pMainWnd);
	int idx = 0;
	CString strLog;

	ListInit();
	m_bVerifyComp = TRUE;

	for(int i = 0; i < m_nVerifyCnt; i++)
	{
		SetVerifyRslt(m_VerifyData[i].isNg, m_VerifyData[i].nDeviceIdx.x, m_VerifyData[i].nDeviceIdx.y, m_VerifyData[i].nGrabIdx);
	}

	m_nVerifyCnt = 0;
	m_bVerifyStart = FALSE;
	pdlg->DeleteRsltFilePath(m_nFileIdx);
	//pdlg->InitBeforeRsltPara();

	if(pdlg->GetBeforeRsltCnt() == idx)
	{
		ListInit();

		SetGrid();
		for(int i = 0; i < (pdlg->m_dlgTeach->GetTeachPara().TeachDeviceCntY * pdlg->m_dlgTeach->GetTeachPara().TeachOneGrabInspCnt); i++)
		{
			for(int j = 0; j < pdlg->m_dlgTeach->GetTeachPara().TeachDeviceCntX; j++)
			{
				for(int k = 0; k < pdlg->m_dlgTeach->GetTeachPara().TeachGrabCnt; k++)
				{
					if(i < pdlg->m_dlgTeach->GetTeachPara().TeachArrayCntY)
					{
						GridDrow(TRUE, (j * pdlg->m_dlgTeach->GetTeachPara().TeachGrabCnt) + k, i, k);
					}
					else
					{
						GridDrow(5, (j * pdlg->m_dlgTeach->GetTeachPara().TeachGrabCnt), i, k);
					}
				}
			}
		}

#ifdef CAMERA
		MdispControl(*pdlg->m_MilManage.GetMilDisplay(NG_VIEW_DISP), M_OVERLAY_CLEAR, M_DEFAULT);
#endif
		pdlg->m_RsltIdx = 0;
		InitRsltPara();
		pdlg->SetLoadRsltPara(0);

		if(m_RsltPara.nNgCnt > 0)
		{
			GridDrow();
			//SetGridDbClick(m_RsltPara.nDeviceIdx[0].x, m_RsltPara.nDeviceIdx[0].y, m_RsltPara.nGrabIdx[0]);
			m_grid.SetGridOkNg_LED(2, (m_RsltPara.nDeviceIdx[0].x * pdlg->m_dlgTeach->GetTeachPara().TeachGrabCnt) + m_RsltPara.nGrabIdx[0], m_RsltPara.nDeviceIdx[0].y, m_RsltPara.nGrabIdx[0]);//m_RsltPara.nDeviceIdx[0].x, m_RsltPara.nDeviceIdx[0].y, m_RsltPara.nGrabIdx[0]);
			pdlg->NGDisplay(m_RsltPara.nDeviceIdx[0].x, m_RsltPara.nDeviceIdx[0].y, m_RsltPara.nGrabIdx[0], m_RsltPara.nInspGrabIdx[0], NG_VIEW_DISP, m_RsltPara);
		}
		m_grid.RedrawAll();

		m_btn_SelectRsltFile.SetWindowTextA(pdlg->GetBeforeRsltFilePath(0));

		LV_ITEM lvitem;
		CString temp;
		char temp1[512]; 
		temp = m_list_BeforeRslt.GetItemText(0, 0);
		sprintf(temp1, "%s", temp);
		lvitem.mask=LVIF_TEXT | LVIF_IMAGE;
		lvitem.iItem=0;
		lvitem.iSubItem=0;
		lvitem.iImage = 1;
		lvitem.pszText = temp1;
		m_list_BeforeRslt.SetItem(&lvitem);

		m_bVerifyComp = FALSE;
		return;
	}

	SetGrid();
	for(int i = 0; i < (pdlg->m_dlgTeach->GetTeachPara().TeachDeviceCntY * pdlg->m_dlgTeach->GetTeachPara().TeachOneGrabInspCnt); i++)
	{
		for(int j = 0; j < pdlg->m_dlgTeach->GetTeachPara().TeachDeviceCntX; j++)
		{
			for(int k = 0; k < pdlg->m_dlgTeach->GetTeachPara().TeachGrabCnt; k++)
			{
				if(i < pdlg->m_dlgTeach->GetTeachPara().TeachArrayCntY)
				{
					GridDrow(TRUE, (j * pdlg->m_dlgTeach->GetTeachPara().TeachGrabCnt) + k, i, k);
				}
				else
				{
					GridDrow(5, (j * pdlg->m_dlgTeach->GetTeachPara().TeachGrabCnt), i, k);
				}
			}
		}
	}

#ifdef CAMERA
	MdispControl(*pdlg->m_MilManage.GetMilDisplay(NG_VIEW_DISP), M_OVERLAY_CLEAR, M_DEFAULT);
#endif
	pdlg->m_RsltIdx = 0;
	InitRsltPara();
	m_nDefListIdx = 0;//20161111 ngh
	pdlg->SetLoadRsltPara(idx);

	if(m_RsltPara.nNgCnt > 0)
	{
		GridDrow();
		m_grid.SetGridOkNg_LED(2, (m_RsltPara.nDeviceIdx[0].x * pdlg->m_dlgTeach->GetTeachPara().TeachGrabCnt) + m_RsltPara.nGrabIdx[0], m_RsltPara.nDeviceIdx[0].y, m_RsltPara.nGrabIdx[0]);//m_RsltPara.nDeviceIdx[0].x, m_RsltPara.nDeviceIdx[0].y, m_RsltPara.nGrabIdx[0]);
		pdlg->NGDisplay(m_RsltPara.nDeviceIdx[0].x, m_RsltPara.nDeviceIdx[0].y, m_RsltPara.nGrabIdx[0], m_RsltPara.nInspGrabIdx[0], NG_VIEW_DISP, m_RsltPara);
	}
	m_grid.RedrawAll();

	m_btn_SelectRsltFile.SetWindowTextA(pdlg->GetBeforeRsltFilePath(idx));
	ListInit();
	
	LV_ITEM lvitem;
	CString temp;
	char temp1[512]; 

	for(int i = 0; i < pdlg->GetBeforeRsltCnt(); i++)
	{
		temp = m_list_BeforeRslt.GetItemText(i, 0);
		sprintf(temp1, "%s", temp);
		lvitem.mask=LVIF_TEXT | LVIF_IMAGE;
		lvitem.iItem=i;
		lvitem.iSubItem=0;
		lvitem.iImage = 0;
		lvitem.pszText = temp1;
		m_list_BeforeRslt.SetItem(&lvitem);
	}

	temp = m_list_BeforeRslt.GetItemText(idx, 0);
	sprintf(temp1, "%s", temp);
	lvitem.mask=LVIF_TEXT | LVIF_IMAGE;
	lvitem.iItem=idx;
	lvitem.iSubItem=0;
	lvitem.iImage = 1;
	lvitem.pszText = temp1;
	m_list_BeforeRslt.SetItem(&lvitem);
	m_nFileIdx = 0;
}


BOOL CDialogNgView::PreTranslateMessage(MSG* pMsg)
{
	// TODO: 여기에 특수화된 코드를 추가 및/또는 기본 클래스를 호출합니다.
	if(pMsg->message ==WM_KEYDOWN)
	{
		if(pMsg->wParam == VK_RETURN || pMsg->wParam == VK_ESCAPE)
			return TRUE;
	}

	return CDialogEx::PreTranslateMessage(pMsg);
}


void CDialogNgView::ClickBtnenhctrlFsltFileComp2()
{
	// TODO: 여기에 메시지 처리기 코드를 추가합니다.
	CLED_DisplayInspectDlg *pdlg = ((CLED_DisplayInspectDlg *)AfxGetApp()->m_pMainWnd);
	ShowWindow(SW_HIDE);
	m_dlgVerify->ShowWindow(SW_HIDE);
	pdlg->m_bBeforeView = FALSE;
}

//20161011 ht NGView다이얼로그 언어 설정
void CDialogNgView::ChangeLanguage(BOOL StateLanguage)
{
	switch(StateLanguage)
	{
	case 0:
		GetDlgItem(IDC_BTNENHCTRL_FSLT_FILE_COMP)->SetWindowText(_T("Result File Comp"));
		GetDlgItem(IDC_BTNENHCTRL_FSLT_FILE_COMP2)->SetWindowText(_T("EXIT(Reurn to Main View)"));
		break;
	case 1:
		GetDlgItem(IDC_BTNENHCTRL_FSLT_FILE_COMP)->SetWindowText(_T("결과 파일 비교"));
		GetDlgItem(IDC_BTNENHCTRL_FSLT_FILE_COMP2)->SetWindowText(_T("종료\n(메인 뷰로 돌아가기)"));
		break;
	}
}

BOOL CDialogNgView::OnMouseWheel(UINT nFlags, short zDelta, CPoint pt)
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.
	CRect rect;
	if(IsWindowVisible())
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

	return CDialogEx::OnMouseWheel(nFlags, zDelta, pt);
}
