/////////////////////////////////////////////////////////////////////////////
//	Skeleton Class for a Derived CUGCtrl class

#include "stdafx.h"
#include "LED_DisplayInspectDlg.h"
//#include "resource.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

BEGIN_MESSAGE_MAP(MyCug,CUGCtrl)
	//{{AFX_MSG_MAP(MyCug)
	// NOTE - the ClassWizard will add and remove mapping macros here.
	//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


/////////////////////////////////////////////////////////////////////////////
// Standard MyCug construction/destruction
MyCug::MyCug()
{
	for(int i = 0; i< 5; i++)
	{
		m_nBeforeDeviceX[i] = 0;
		m_nBeforeDeviceY[i] = 0;
	}
	//20161130 nht
	for(int i=0;i<3;i++)
	{
		m_nMoveBeforeDeviceX[i] = 0;
	}
	for(int i=0;i<3;i++)
	{
		m_nMoveBeforeDeviceY[i] = i;	
	}
}

MyCug::~MyCug()
{
}

/////////////////////////////////////////////////////////////////////////////
//	OnSetup
//		This function is called just after the grid window 
//		is created or attached to a dialog item.
//		It can be used to initially setup the grid
void MyCug::OnSetup()
{
	//SetDefColWidth(528);
	//SetDefRowHeight(250);
	//SetNumberRows(1); // 이 부분 포함
	//SetNumberCols(1); // 이 부분 포함


	//	SetCell(2,1,&cell);
	//	SetCell(3,1,&cell); 
}

void MyCug::SetGridOkNg_LED(int OkNg,int col,int row,int grabcnt)
{
	CUGCell cell, temp_cell;
	COLORREF color_OK, color_NG, color_Text, color_Sel, color_Ins, color_Grab, color_Empty;
	CString temp, temp1;
	color_Text = RGB(0,0,0);
	color_OK = RGB(0, 250, 0); 
	color_NG = RGB(250, 0, 0);
	color_Ins = RGB(250, 250, 0);
	color_Grab = RGB(0, 250, 250);
	color_Empty = RGB(150, 150, 150);


	GetCell(m_nDeviceX[col], m_nDeviceY[row], &cell); 

	if(OkNg == 1)
	{
		cell.SetText("Ok");
		cell.SetTextColor(color_Text);
		cell.SetBackColor(color_OK);
		SetCell(m_nDeviceX[col], m_nDeviceY[row], &cell);
	}
	else if(OkNg == 0)
	{
		cell.SetText("NG");
		cell.SetTextColor(color_Text);
		cell.SetBackColor(color_NG);
		SetCell(m_nDeviceX[col], m_nDeviceY[row], &cell);
	}
	else if(OkNg == 2)
	{
		GetCell(m_nDeviceX[m_nBeforeDeviceX[grabcnt]], m_nDeviceY[m_nBeforeDeviceY[grabcnt]], &cell);
		temp.Format(_T("%s"), cell.GetText());
		if(temp.GetLength() > 5)
		{
			temp1.Format(_T("%s"), temp.Mid(0, (temp.GetLength() - 4)));
		}
		else
		{
			temp1.Format(_T("%s"), temp.Left(2));
		}
		cell.SetText(temp1);
		SetCell(m_nDeviceX[m_nBeforeDeviceX[grabcnt]], m_nDeviceY[m_nBeforeDeviceY[grabcnt]], &cell);
		//RedrawCell(m_nDeviceX[m_nBeforeDeviceX], m_nDeviceY[m_nBeforeDeviceY]);

		GetCell(m_nDeviceX[col], m_nDeviceY[row], &cell);
		temp.Format(_T("%s(★)"), cell.GetText());
		color_Sel = cell.GetBackColor();
		cell.SetText(temp);
		cell.SetBackColor(color_Sel);
		SetCell(m_nDeviceX[col], m_nDeviceY[row], &cell);
		//RedrawCell(m_nDeviceX[col], m_nDeviceY[row]);


		m_nBeforeDeviceX[grabcnt] = col;
		m_nBeforeDeviceY[grabcnt] = row;
	}
	else if(OkNg == 3)
	{
		temp.Format(_T("◎ Insp ◎"));
		cell.SetText(temp); 
		cell.SetBackColor(color_Ins);
		SetCell(m_nDeviceX[col], m_nDeviceY[row], &cell);
	}
	else if(OkNg == 4)
	{
		temp.Format(_T("☞ Grab ☜"));
		cell.SetText(temp); 
		cell.SetBackColor(color_Grab);
		SetCell(m_nDeviceX[col], m_nDeviceY[row], &cell);
	}
	else if(OkNg == 5)
	{
		temp.Format(_T("Empty"));
		cell.SetText(temp); 
		cell.SetBackColor(color_Empty);
		SetCell(m_nDeviceX[col], m_nDeviceY[row], &cell);
	}
	else if(OkNg == 6)
	{
		GetCell(m_nDeviceX[col], m_nDeviceY[row], &cell);
		temp.Format(_T("%s"), cell.GetText());
		if(temp.Compare(_T("Empty")))
		{
			//20161123 nht
			temp1.Format(_T("%s(★)"), temp.Left(5));
		}
		else
		{
			temp1.Format(_T("%s(★)"), temp.Left(5));
		}
		color_Sel = cell.GetBackColor();
		cell.SetText(temp1);
		cell.SetBackColor(color_Sel);
		SetCell(m_nDeviceX[col], m_nDeviceY[row], &cell);
		//20161213 nht
		//RedrawCell(m_nDeviceX[col], m_nDeviceY[row]);
	}
	else if(OkNg == 7)
	{
		GetCell(m_nDeviceX[col], m_nDeviceY[row], &cell);
		temp.Format(_T("%s"), cell.GetText());
		if(temp.Left(2).Compare(_T("OK")))
		{
			temp1.Format(_T("%s->OK"), temp.Left(2));
		}
		else
		{
			temp1.Format(_T("%s"), temp.Left(2));
		}
		color_Sel = cell.GetBackColor();
		cell.SetText(temp1);
		cell.SetBackColor(color_Sel);
		SetCell(m_nDeviceX[col], m_nDeviceY[row], &cell);
	}
	else if(OkNg == 8)
	{
		GetCell(m_nDeviceX[col], m_nDeviceY[row], &cell);
		temp.Format(_T("%s"), cell.GetText());
		if(temp.Left(2).Compare(_T("NG")))
		{
			temp1.Format(_T("%s->NG"), temp.Left(2));
		}
		else
		{
			temp1.Format(_T("%s"), temp.Left(2));
		}
		color_Sel = cell.GetBackColor();
		cell.SetText(temp1);
		cell.SetBackColor(color_Sel);
		SetCell(m_nDeviceX[col], m_nDeviceY[row], &cell);
	}
	else if(OkNg == 9)
	{
		GetCell(m_nDeviceX[col], m_nDeviceY[row], &cell);
		temp.Format(_T("%s"), cell.GetText());
		temp1.Format(_T("%s->NG"), temp.Left(2));
		color_Sel = cell.GetBackColor();
		cell.SetText(temp1);
		cell.SetBackColor(color_Sel);
		SetCell(m_nDeviceX[col], m_nDeviceY[row], &cell);
	}
	else if(OkNg == 10)
	{
		GetCell(m_nDeviceX[col], m_nDeviceY[row], &cell);
		temp.Format(_T("%s"), cell.GetText());
		if(temp.GetLength() > 5)
		{
			temp1.Format(_T("%s"), temp.Mid(0, (temp.GetLength() - 4)));
		}
		else
		{
			temp1.Format(_T("%s"), temp.Left(2));
		}
		cell.SetText(temp1);
		SetCell(m_nDeviceX[col], m_nDeviceY[row], &cell);
	}
	else if(OkNg == 11)
	{

	}

	//20161130 nht
	else if(OkNg == 100)
	{
		GetCell(m_nDeviceX[m_nMoveBeforeDeviceX[grabcnt]], m_nDeviceY[m_nMoveBeforeDeviceY[grabcnt]], &cell);
		temp.Format(_T("%s"), cell.GetText());
		if(temp.GetLength() > 5)
		{
			temp1.Format(_T("%s"), temp.Mid(0, (temp.GetLength() - 4)));
		}
		else
		{
			temp1.Format(_T("%s"), temp.Left(2));
		}
		cell.SetText(temp1);
		SetCell(m_nDeviceX[m_nMoveBeforeDeviceX[grabcnt]], m_nDeviceY[m_nMoveBeforeDeviceY[grabcnt]], &cell);
		RedrawCell(m_nDeviceX[m_nMoveBeforeDeviceX[grabcnt]], m_nDeviceY[m_nMoveBeforeDeviceY[grabcnt]]);

		GetCell(m_nDeviceX[col], m_nDeviceY[row], &cell);
		temp.Format(_T("%s(★)"), cell.GetText());
		color_Sel = cell.GetBackColor();
		cell.SetText(temp);
		cell.SetBackColor(color_Sel);
		SetCell(m_nDeviceX[col], m_nDeviceY[row], &cell);
		RedrawCell(m_nDeviceX[col], m_nDeviceY[row]);


		m_nMoveBeforeDeviceX[grabcnt] = col;
		m_nMoveBeforeDeviceY[grabcnt] = row;
	}


}

void MyCug::SetGrid_LED(int cols,int rows,int grabcnt, int one_insp_cnt)
{
	int totalwidth = 528,totalheight = 250,gab = 4,width = 0,height = 0;
	int iRet = 0;
	int t = 0,count = 0;
	BOOL bRet = FALSE;
	BOOL bGab[100];

	CUGCell cell;
	COLORREF color_gab;
	color_gab = RGB(0,0,0);

	COLORREF color_title;		//20161006 sc
	color_title = RGB(0,0,0);
	CString text;
	int nDevice_x[100];
	int nDevice_idx_x = 1;
	int nDevice_idx_y = 1;		//20161006 sc

	int cell_width, cell_height;
	int gab_cnt_x, device_cnt_x, grab_cnt_x, total_cnt_x;
	int gab_cnt_y, device_cnt_y, grab_cnt_y, total_cnt_y;
	int device_idx = 0, grab_idx = 0, gab_idx = 0, col_idx = 0, row_idx = 0;



	//m_nDeviceCntX = 0;
	//m_nDeviceCntY = 0;
	//m_nGabCnt = 0;
	//device_cnt_x = cols;
	//
	//gab_cnt_x = cols - 1;
	////total_cnt_x = (device_cnt_x * grab_cnt_x) + gab_cnt_x;




	//20161221 nht 그리드 그려주기위해 기초적인 변수들을 몇가지 선언하였습니다.

	int Grid_Cols;				// cols 인자를 받기위해 만든 변수입니다.
	int Grid_Rows;				// rows 인자를 받기위해 만든 변수입니다.
	int Title_x = 1;			// 1제품당 2번이상의 촬영을 하는 제품들의 대해 그리드 위에 인덱스로 표시하기위해 만든 변수입니다.
	int Title_Grab_cnt = 0;		// 1제품당 2번이상의 촬영을 하는 제품들의 대해 그리드 위에 인덱스로 표시하기위해 만든 변수입니다.

	grab_cnt_x = grabcnt; //1제품당 그랩 수
	grab_cnt_y = one_insp_cnt; // 1회 촬영당 몇개 제품을 촬영하는지의 대한 변수
	Grid_Cols = cols; 
	Grid_Rows = rows;
	CPen* GridBorderPen = new CPen(PS_SOLID, 3, RGB(0, 0, 0));


	//SetNumberCols(total_cnt_x);
	//memset(m_nGrabIdxX, -1, 100);
	//memset(m_bGab, -1, 100);
	//for(int i = 0; i < 100; i++)
	//{
	//	if(gab_idx == 0)
	//	{
	//		m_nDeviceX[i] = col_idx;
	//	}
	//	if(grab_idx == grab_cnt_x)
	//	{

	//		grab_idx = 0;
	//		gab_idx++;
	//	}

	//	if(gab_idx == 1)
	//	{
	//		gab_idx = 0;
	//		device_idx++;
	//		m_bGab[col_idx++] = TRUE;
	//		m_nDeviceCntX++;
	//		m_nGabCnt++;
	//	}

	//	m_nGrabIdxX[i] = grab_idx;

	//	if(device_idx == (device_cnt_x - 1))
	//	{
	//		break;
	//	}

	//	if(col_idx == 0)
	//	{
	//	m_nGrabIdxX[col_idx] = 1;
	//	}
	//	m_nGrabIdxX[col_idx] = col_idx / grab_cnt_x;

	//	col_idx++;
	//	grab_idx++;
	//}


	//grab_idx = 1;
	//for(int i = 0; i < 100; i++)
	//{
	//	if(m_nGrabIdxX[i] == -1)
	//	{
	//		m_nGrabIdxX[i] = grab_idx++;
	//		if(grab_idx == grab_cnt_x)
	//		{
	//			break;
	//		}
	//	}
	//}

	//col_idx = 0;
	//for(int i = 0; i < total_cnt_x; i++)
	//{
	//	if(m_bGab[i] == 1)
	//	{
	//		SetDefColWidth(GRID_GAB_WIDTH);
	//		cell_width = GRID_GAB_WIDTH;
	//		nDevice_x[i] = GRID_GAB_WIDTH; //20161006 sc
	//	}
	//	else
	//	{
	//		m_nDeviceX[col_idx++] = i;
	//		SetDefColWidth(GRID_CELL_WIDTH);
	//		cell_width = GRID_CELL_WIDTH;
	//		nDevice_x[i] = GRID_CELL_WIDTH;	//20161006 sc
	//	}
	//	SetColWidth(i, cell_width);
	//}

	//int idx = 0;		//20161006 sc
	//grab_idx = 0;
	//gab_idx = 0;

	//for(int i = 0; i < total_cnt_x; i++)
	//{
	//	text.Format((""));
	//	//20161129 nht i->i+1
	//	QuickSetText(i,-1,text);
	//}

	//if(grab_cnt_x > 1)
	//{
	//	for(int i = 0;i < total_cnt_x;i++)
	//	{
	//		if(grab_idx == grab_cnt_x)
	//		{
	//			gab_idx++;
	//			grab_idx = 0;
	//			idx++;
	//			continue;
	//		}
	//		text.Format(("%d-%d"),idx+1,grab_idx);
	//		QuickSetText(m_nDeviceX[i - gab_idx],-1,text);
	//		grab_idx++;
	//	}
	//}
	//else
	//{
	//	idx = 0;
	//	for(int i = 0;i < total_cnt_x;i++)
	//	{
	//		if(m_bGab[i] == TRUE)
	//		{
	//			continue;
	//		}
	//		text.Format(("%d"),idx+1);
	//		QuickSetText(m_nDeviceX[idx],-1,text);
	//		idx++;
	//	}
	//}  //20161006 sc

	//device_cnt_y = rows;
	//grab_cnt_y = 1;
	//gab_cnt_y = rows - 1;
	//total_cnt_y = (device_cnt_y * grab_cnt_y) + gab_cnt_y;

	//SetNumberRows(total_cnt_y);

	//memset(bGab, NULL, 100);
	//gab_idx = 0;
	//grab_idx = 0;
	//device_idx = 0;

	//for(int i = 0; i < 100; i++)
	//{
	//	if(gab_idx == 0)
	//	{
	//		m_nDeviceY[i] = row_idx;
	//	}

	//	if(grab_idx == grab_cnt_y)
	//	{
	//		grab_idx = 0;
	//		gab_idx++;
	//	}

	//	if(gab_idx == 1)
	//	{
	//		gab_idx = 0;
	//		device_idx++;
	//		bGab[row_idx++] = TRUE;
	//		m_nDeviceCntY++;
	//	}

	//	if(device_idx == (device_cnt_y - 1))
	//	{
	//		break;
	//	}
	//	row_idx++;
	//	grab_idx++;
	//}

	//row_idx = 0;
	//for(int i = 0; i < total_cnt_y; i++)
	//{
	//	if(bGab[i])
	//	{
	//		SetDefRowHeight(GRID_GAB_HEIGHT);
	//		cell_height = GRID_GAB_HEIGHT;
	//	}
	//	else
	//	{
	//		m_nDeviceY[row_idx++] = i;
	//		SetDefRowHeight(GRID_CELL_HEIGHT);
	//		cell_height = GRID_CELL_HEIGHT;
	//	}
	//	SetRowHeight(i, cell_height);
	//}

	//idx = 0;		//20161006 sc
	//for(int i = 0;i < total_cnt_y;i++)
	//{
	//	if(bGab[i] == TRUE)
	//	{
	//		continue;
	//	}
	//	text.Format(("%c"),idx+65);
	//	QuickSetText(-1,m_nDeviceY[idx],text);
	//	idx++;
	//}  //20161006 sc


	//20161221 nht 수정한 부분입니다.


	/*SetNumberCols(cols * grab_cnt_x);
	SetNumberRows(Grid_Rows);
	for(int i=0;i<cols * grab_cnt_x;i++)
	{
		SetDefColWidth(GRID_CELL_WIDTH);
		SetColWidth(i, GRID_CELL_WIDTH);
		for(int j=0;j<Grid_Rows;j++)
		{
			SetDefRowHeight(GRID_CELL_HEIGHT);
			SetRowHeight(j, GRID_CELL_HEIGHT);
			cell.SetBorder(UG_BDR_RTHICK|UG_BDR_BTHICK);
			cell.SetBorderColor(GridBorderPen);
			SetCell(i,j,&cell);
		}
	}*/

	memset(m_nDeviceX, 0, 100);
	memset(m_nDeviceY, 0, 100);
	if(grab_cnt_x > 1)
	{
		SetNumberCols(cols * grab_cnt_x);
		SetNumberRows(Grid_Rows);
		for(int i=0;i<cols * grab_cnt_x;i++)
		{
			SetDefColWidth(GRID_CELL_WIDTH);
			SetColWidth(i, GRID_CELL_WIDTH);
			m_nDeviceX[i] = i;
			for(int j=0;j<Grid_Rows;j++)
			{
				SetDefRowHeight(GRID_CELL_HEIGHT);
				SetRowHeight(j, GRID_CELL_HEIGHT);
				//20170104
				if((i == 0) || ((i + 1) % (grab_cnt_x)) > 0)
				{
					cell.SetBorder(UG_BDR_RTHIN | UG_BDR_BTHICK);
					cell.SetBorderColor(GridBorderPen);
				}
				else
				{
					cell.SetBorder(UG_BDR_RTHICK | UG_BDR_BTHICK);
					cell.SetBorderColor(GridBorderPen);
				}
				SetCell(i,j,&cell);
				m_nDeviceY[j] = j;
			}
		}
	}
	else
	{
		SetNumberCols(cols * grab_cnt_x);
		SetNumberRows(Grid_Rows * grab_cnt_y);
		for(int i = 0; i < cols * grab_cnt_x; i++)
		{
			SetDefColWidth(GRID_CELL_WIDTH);
			SetColWidth(i, GRID_CELL_WIDTH);
			m_nDeviceX[i] = i;
			for(int j = 0; j < Grid_Rows * grab_cnt_y; j++)
			{
				SetDefRowHeight(GRID_CELL_HEIGHT);
				SetRowHeight(j, GRID_CELL_HEIGHT);
				cell.SetBorder(UG_BDR_RTHICK | UG_BDR_BTHICK);
				cell.SetBorderColor(GridBorderPen);
				SetCell(i,j,&cell);
				m_nDeviceY[j] = j;
			}
		}
	}


	for(int idx = 0;idx < Grid_Rows * grab_cnt_y;idx++)
	{
		text.Format(("%c"),idx+65);
		QuickSetText(-1,idx,text);
	}

	if(grab_cnt_x > 1)
	{
		for(int i = 0; i < cols; i++)
		{
			for(int j = 0;j < grab_cnt_x; j++)
			{
				text.Format(("%d-%d"),Title_x,j+1);
				QuickSetText(Title_Grab_cnt,-1,text);
				Title_Grab_cnt++;
				m_nGrabIdxX[Title_Grab_cnt - 1] = j;//20170104
			}
			Title_x++;
		}

	}
	else{
		for(int idx = 0;idx < Grid_Cols;idx++)
		{
			m_nGrabIdxX[idx] = 0;//20170104
			text.Format(("%d"),idx+1);
			QuickSetText(idx,-1,text);

		}
	}

	//if(grab_cnt_x > 1)
	//{
	//	for(int i = 0;i < total_cnt_x;i++)
	//	{
	//		if(grab_idx == grab_cnt_x)
	//		{
	//			gab_idx++;
	//			grab_idx = 0;
	//			idx++;
	//			continue;
	//		}
	//		text.Format(("%d-%d"),idx+1,grab_idx);
	//		QuickSetText(m_nDeviceX[i - gab_idx],-1,text);
	//		grab_idx++;
	//	}
	//}
	//else
	//{
	//	idx = 0;
	//	for(int i = 0;i < total_cnt_x;i++)
	//	{
	//		if(m_bGab[i] == TRUE)
	//		{
	//			continue;
	//		}
	//		text.Format(("%d"),idx+1);
	//		QuickSetText(m_nDeviceX[idx],-1,text);
	//		idx++;
	//	}
	//}  //20161006 sc










	delete GridBorderPen;

}

/////////////////////////////////////////////////////////////////////////////
//	OnSheetSetup	
//		This notification is called for each additional sheet that the grid
//		might contain, here you can customize each sheet in the grid.
//	Params:
//		sheetNumber - index of current sheet
//	Return:
//		<none>
void MyCug::OnSheetSetup(int sheetNumber)
{
	UNREFERENCED_PARAMETER(sheetNumber);
}

/////////////////////////////////////////////////////////////////////////////
//	OnCanMove
//		is sent when a cell change action was instigated
//		( user clicked on another cell, used keyboard arrows,
//		or Goto[...] function was called ).
//	Params:
//		oldcol, oldrow - 
//		newcol, newrow - cell that is gaining focus
//	Return:
//		TRUE - to allow the move
//		FALSE - to prevent new cell from gaining focus
int MyCug::OnCanMove(int oldcol,long oldrow,int newcol,long newrow)
{
	UNREFERENCED_PARAMETER(oldcol);
	UNREFERENCED_PARAMETER(oldrow);
	UNREFERENCED_PARAMETER(newcol);
	UNREFERENCED_PARAMETER(newrow);
	return TRUE;
}

/////////////////////////////////////////////////////////////////////////////
//	OnCanViewMove
//		is called when current grid's view is about to be scrolled.
//	Params:
//		oldcol, oldrow - coordinates of original top-left cell
//		newcol, newrow - coordinates of top-left cell that is gaining the focus
//	Return:
//		TRUE - to allow for the scroll
//		FALSE - to prevent the view from scrolling
int MyCug::OnCanViewMove(int oldcol,long oldrow,int newcol,long newrow)
{
	UNREFERENCED_PARAMETER(oldcol);
	UNREFERENCED_PARAMETER(oldrow);
	UNREFERENCED_PARAMETER(newcol);
	UNREFERENCED_PARAMETER(newrow);
	return TRUE;
}

/////////////////////////////////////////////////////////////////////////////
//	OnHitBottom
//		This notification allows for dynamic row loading, it will be called
//		when the grid's drawing function has hit the last row.  It allows the grid
//		to ask the datasource/developer if there are additional rows to be displayed.
//	Params:
//		numrows		- known number of rows in the grid
//		rowspast	- number of extra rows that the grid is looking for in the datasource
//		rowsfound	- number of rows actually found, usually equal to rowspast or zero.
//	Return:
//		<none>
void MyCug::OnHitBottom(long numrows,long rowspast,long rowsfound)
{
	UNREFERENCED_PARAMETER(numrows);
	UNREFERENCED_PARAMETER(rowspast);
	UNREFERENCED_PARAMETER(rowsfound);
	// used by the datasources
	/*if ( rowsfound > 0 )
	{
	SetNumberRows( numrows + rowsfound, FALSE );
	}*/
}

/////////////////////////////////////////////////////////////////////////////
//	OnHitTop
//		Is called when the user has scrolled all the way to the top of the grid.
//	Params:
//		numrows		- known number of rows in the grid
//		rowspast	- number of extra rows that the grid is looking for in the datasource
//	Return:
//		<none>
void MyCug::OnHitTop(long numrows,long rowspast)
{
	UNREFERENCED_PARAMETER(numrows);
	UNREFERENCED_PARAMETER(rowspast);
}

/////////////////////////////////////////////////////////////////////////////
//	OnCanSizeCol
//		is sent when the mouse was moved in the area between
//		columns on the top heading, indicating that the user
//		might want to resize a column.
//	Params:
//		col - identifies column number that will be sized
//	Return:
//		TRUE - to allow sizing
//		FALSE - to prevent sizing
int MyCug::OnCanSizeCol(int col)
{
	UNREFERENCED_PARAMETER(col);
	return TRUE;
}

/////////////////////////////////////////////////////////////////////////////
//	OnColSizing
//		is sent continuously when user is sizing a column.
//		This notification is ideal to provide min/max width checks.
//	Params:
//		col - column currently sizing
//		width - current new column width
//	Return:
//		<none>
void MyCug::OnColSizing(int col,int *width)
{
	UNREFERENCED_PARAMETER(col);
	UNREFERENCED_PARAMETER(*width);
}

/////////////////////////////////////////////////////////////////////////////
//	OnColSized
//		This is sent when the user finished sizing the 
//		given column.(see above for more information)
//	Params:
//		col - column sized
//		width - new column width
//	Return:
//		<none>
void MyCug::OnColSized(int col,int *width)
{
	UNREFERENCED_PARAMETER(col);
	UNREFERENCED_PARAMETER(*width);
}

/////////////////////////////////////////////////////////////////////////////
//	OnCanSizeRow
//		is sent when the mouse was moved in the area between
//		rows on the side heading, indicating that the user
//		might want to resize a row.
//	Params:
//		row - identifies row number that will be sized
//	Return:
//		TRUE - to allow sizing
//		FALSE - to prevent sizing
int MyCug::OnCanSizeRow(long row)
{
	UNREFERENCED_PARAMETER(row);
	return TRUE;
}

/////////////////////////////////////////////////////////////////////////////
//	OnRowSizing
//		Sent during user sizing of a row, can provide
//		feed back on current height
//	Params:
//		row - row sizing
//		height - pointer to current new row height
//	Return:
//		<none>
void MyCug::OnRowSizing(long row,int *height)
{
	UNREFERENCED_PARAMETER(row);
	UNREFERENCED_PARAMETER(*height);
}

/////////////////////////////////////////////////////////////////////////////
//	OnRowSized
//		This is sent when the user is finished sizing the
//		given row.
//	Params:
//		row - row sized
//		height - pointer to current new row height
//	Return:
//		<none>
void MyCug::OnRowSized(long row,int *height)
{
	UNREFERENCED_PARAMETER(row);
	UNREFERENCED_PARAMETER(*height);
}

/////////////////////////////////////////////////////////////////////////////
//	OnCanSizeSideHdg
//		Sent when the user is about to start sizing of the side heading
//	Params:
//		<none>
//	Return:
//		TRUE - to allow sizing
//		FALSE - to prevent sizing
int MyCug::OnCanSizeSideHdg()
{
	return TRUE;
}

/////////////////////////////////////////////////////////////////////////////
//	OnSideHdgSizing
//		Sent while the heading is being sized
//	Params:
//		width - pointer to current new width of the side heading
//	Return:
//		TRUE - to accept current new size
//		FALSE - to stop sizing, the size is either too large or too small
int MyCug::OnSideHdgSizing(int *width)
{
	UNREFERENCED_PARAMETER(*width);
	return TRUE;
}

/////////////////////////////////////////////////////////////////////////////
//	OnSideHdgSized
//		Sent when the user has completed the sizing of the side heading
//	Params:
//		width - pointer to new width
//	Return:
//		TRUE - to accept new size
//		FALSE - to revert to old size
int MyCug::OnSideHdgSized(int *width)
{
	UNREFERENCED_PARAMETER(*width);
	return TRUE;
}

/////////////////////////////////////////////////////////////////////////////
//	OnCanSizeTopHdg
//		Sent when the user is about to start sizing of the top heading
//	Params:
//		<none>
//	Return:
//		TRUE - to allow sizing
//		FALSE - to prevent sizing
int MyCug::OnCanSizeTopHdg()
{
	return TRUE;
}

/////////////////////////////////////////////////////////////////////////////
//	OnTopHdgSizing
//		Sent while the top heading is being sized
//	Params:
//		height - pointer to current new height of the top heading
//	Return:
//		TRUE - to accept current new size
//		FALSE - to stop sizing, the size is either too large or too small
int MyCug::OnTopHdgSizing(int *height)
{
	UNREFERENCED_PARAMETER(*height);
	return TRUE;
}

/////////////////////////////////////////////////////////////////////////////
//	OnTopHdgSized
//		Sent when the user has completed the sizing of the top heading
//	Params:
//		height - pointer to new height
//	Return:
//		TRUE - to accept new size
//		FALSE - to revert to old size
int MyCug::OnTopHdgSized(int *height)
{
	UNREFERENCED_PARAMETER(*height);
	return TRUE;
}

/////////////////////////////////////////////////////////////////////////////
//	OnColChange
//		Sent whenever the current column changes
//	Params:
//		oldcol - column that is loosing the focus
//		newcol - column that the user move into
//	Return:
//		<none>
void MyCug::OnColChange(int oldcol,int newcol)
{
	UNREFERENCED_PARAMETER(oldcol);
	UNREFERENCED_PARAMETER(newcol);
}

/////////////////////////////////////////////////////////////////////////////
//	OnRowChange
//		Sent whenever the current row changes
//	Params:
//		oldrow - row that is loosing the locus
//		newrow - row that user moved into
//	Return:
//		<none>
void MyCug::OnRowChange(long oldrow,long newrow)
{
	UNREFERENCED_PARAMETER(oldrow);
	UNREFERENCED_PARAMETER(newrow);
}

/////////////////////////////////////////////////////////////////////////////
//	OnCellChange
//		Sent whenever the current cell changes
//	Params:
//		oldcol, oldrow - coordinates of cell that is loosing the focus
//		newcol, newrow - coordinates of cell that is gaining the focus
//	Return:
//		<none>
void MyCug::OnCellChange(int oldcol,int newcol,long oldrow,long newrow)
{
	UNREFERENCED_PARAMETER(oldcol);
	UNREFERENCED_PARAMETER(newcol);
	UNREFERENCED_PARAMETER(oldrow);
	UNREFERENCED_PARAMETER(newrow);
}

/////////////////////////////////////////////////////////////////////////////
//	OnLeftColChange
//		Sent whenever the left visible column in the grid changes, indicating
//		that the view was scrolled horizontally
//	Params:
//		oldcol - column that used to be on the left
//		newcol - new column that is going to be the first visible column from the left
//	Return:
//		<none>
void MyCug::OnLeftColChange(int oldcol,int newcol)
{
	UNREFERENCED_PARAMETER(oldcol);
	UNREFERENCED_PARAMETER(newcol);
}

/////////////////////////////////////////////////////////////////////////////
//	OnTopRowChange
//		Sent whenever the top visible row in the grid changes, indicating
//		that the view was scrolled vertically
//	Params:
//		oldrow - row that used to be on the top
//		newrow - new row that is going to be the first visible row from the top
//	Return:
//		<none>
void MyCug::OnTopRowChange(long oldrow,long newrow)
{
	UNREFERENCED_PARAMETER(oldrow);
	UNREFERENCED_PARAMETER(newrow);
}

/////////////////////////////////////////////////////////////////////////////
//	OnViewMoved
//		This notification is fired after the current viewing area
//		was scrolled.
//	Params:
//		nScrolDir - UG_VSCROLL, UG_HSCROLL
//
//		if the nScrolDir == UG_VSCROLL
//			oldPos - row that used to be on the top
//			newPos - row that is now the first visible row from the top
//
//		if the nScrolDir == UG_VSCROLL
//			oldPos - column that used to be on the left
//			newPos - column that is now the first visible column from the left
//	Return:
//		<none>
void MyCug::OnViewMoved( int nScrolDir, long oldPos, long newPos )
{
	UNREFERENCED_PARAMETER(nScrolDir);
	UNREFERENCED_PARAMETER(oldPos);
	UNREFERENCED_PARAMETER(newPos);
}

/////////////////////////////////////////////////////////////////////////////
//	OnSelectionChanged
//		The OnSelectionChanged notification is called by the multiselect
//		class when a change occurred in current selection (i.e. user clicks
//		on a new cell, drags a mouse selecting range of cells, or uses
//		CTRL/SHIFT - left click key combination to select cells.)
//	Params:
//		startCol, startRow	- coordinates of the cell starting the selection block
//		endCol, endRow		- coordinates of the cell ending the selection block
//		blockNum			- block number representing this range, this will
//							always represent total number of selection blocks.
//	Return:
//		<none>
void MyCug::OnSelectionChanged(int startCol,long startRow,int endCol,long endRow,int blockNum)
{
	UNREFERENCED_PARAMETER(startCol);
	UNREFERENCED_PARAMETER(startRow);
	UNREFERENCED_PARAMETER(endCol);
	UNREFERENCED_PARAMETER(endRow);
	UNREFERENCED_PARAMETER(blockNum);
}

/////////////////////////////////////////////////////////////////////////////
//	OnLClicked
//		Sent whenever the user clicks the left mouse button within the grid
//		this message is sent when the button goes down then again when the button goes up
//	Params:
//		col, row	- coordinates of a cell that received mouse click event
//		updn		- is TRUE if mouse button is 'down' and FALSE if button is 'up'
//		processed	- indicates if current event was processed by other control in the grid.
//		rect		- represents the CDC rectangle of cell in question
//		point		- represents the screen point where the mouse event was detected
//	Return:
//		<none>
void MyCug::OnLClicked(int col,long row,int updn,RECT *rect,POINT *point,int processed)
{
	UNREFERENCED_PARAMETER(col);
	UNREFERENCED_PARAMETER(row);
	UNREFERENCED_PARAMETER(updn);
	UNREFERENCED_PARAMETER(*rect);
	UNREFERENCED_PARAMETER(*point);
	UNREFERENCED_PARAMETER(processed);

}

/////////////////////////////////////////////////////////////////////////////
//	OnRClicked
//		Sent whenever the user clicks the right mouse button within the grid
//		this message is sent when the button goes down then again when the button goes up
//	Params:
//		col, row	- coordinates of a cell that received mouse click event
//		updn		- is TRUE if mouse button is 'down' and FALSE if button is 'up'
//		processed	- indicates if current event was processed by other control in the grid.
//		rect		- represents the CDC rectangle of cell in question
//		point		- represents the screen point where the mouse event was detected
//	Return:
//		<none>
void MyCug::OnRClicked(int col,long row,int updn,RECT *rect,POINT *point,int processed)
{
	UNREFERENCED_PARAMETER(col);
	UNREFERENCED_PARAMETER(row);
	UNREFERENCED_PARAMETER(updn);
	UNREFERENCED_PARAMETER(*rect);
	UNREFERENCED_PARAMETER(*point);
	UNREFERENCED_PARAMETER(processed);
	int width_ = 0,height_ = 0;
	GetColWidth(col,&width_);
	GetRowHeight(row,&height_);

	if(width_ <= 5 || height_ <= 5)
	{
		return;
	}

	BOOL isCompare = FALSE;
	int c, r, g;

	for(int i = 0; i < m_GI->m_numberCols; i++)
	{
		if(m_nDeviceX[i] == col)
		{
			c = i;
			for(int j = 0; j < m_GI->m_numberRows; j++)
			{
				if(m_nDeviceY[j] == row)
				{
					isCompare = TRUE;
					r = j;
					break;
				}
			}
		}
		if(isCompare)
		{
			break;
		}
	}

	if(!isCompare)
	{
		return;
	}

	g = m_nGrabIdxX[c];

	m_nDeviceX[col];
	m_nDeviceY[row];

	int gab_cnt = 0;
	int nGabCnt = 0;
	for(int i = 0; i < col; i++)
	{
		if(m_bGab[i] == 1)
		{
			nGabCnt++;
		}
	}

	gab_cnt = 0;
	if(c >= nGabCnt - 1)
	{
		for(int i = 0; i < c + nGabCnt; i++)
		{
			if(m_bGab[i] == 1)
			{
				gab_cnt++;
			}
		}
	}
	else
	{
		c = 0;
	}
	COLORREF color_OK, color_NG, color_temp;
	CUGCell cell;
	BOOL isOk = FALSE;
	color_OK = RGB(0, 250, 0); 
	color_NG = RGB(250, 0, 0);
	color_temp = RGB(0, 0, 0);

	GetCell(m_nDeviceX[col], m_nDeviceY[row], &cell); 
	color_temp = cell.GetBackColor();

	if(color_temp == color_NG)
	{
		isOk = FALSE;
	}
	else if(color_temp == color_OK)
	{
		isOk = TRUE;
	}

	CLED_DisplayInspectDlg *pdlg = ((CLED_DisplayInspectDlg *)AfxGetApp()->m_pMainWnd);
	pdlg->m_dlgNgView->SetGridRClick(gab_cnt + 1, r + 1, g + 1, isOk);
}

/////////////////////////////////////////////////////////////////////////////
//	OnDClicked
//		Sent whenever the user double clicks the left mouse button within the grid
//	Params:
//		col, row	- coordinates of a cell that received mouse click event
//		processed	- indicates if current event was processed by other control in the grid.
//		rect		- represents the CDC rectangle of cell in question
//		point		- represents the screen point where the mouse event was detected
//	Return:
//		<none>
void MyCug::OnDClicked(int col,long row,RECT *rect,POINT *point,BOOL processed)//20170104
{
	UNREFERENCED_PARAMETER(col);
	UNREFERENCED_PARAMETER(row);
	UNREFERENCED_PARAMETER(*rect);
	UNREFERENCED_PARAMETER(*point);
	UNREFERENCED_PARAMETER(processed);
	int width_ = 0,height_ = 0;
	GetColWidth(col,&width_);
	GetRowHeight(row,&height_);

	if(width_ <= 5 || height_ <= 5)
	{
		return;
	}

	BOOL isCompare = FALSE;
	int c, r, g;

	for(int i = 0; i < m_GI->m_numberCols; i++)
	{
		if(m_nDeviceX[i] == col)
		{
			c = i;
			for(int j = 0; j < m_GI->m_numberRows; j++)
			{
				if(m_nDeviceY[j] == row)
				{
					isCompare = TRUE;
					r = j;
					break;
				}
			}
		}
		if(isCompare)
		{
			break;
		}
	}

	if(!isCompare)
	{
		return;
	}

	g = m_nGrabIdxX[c];

	m_nDeviceX[col];
	m_nDeviceY[row];

	int gab_cnt = 0;
	int nGabCnt = 0;
	for(int i = 0; i < col; i++)
	{
		if(m_bGab[i] == 1)
		{
			nGabCnt++;
		}
	}
	/*
	gab_cnt = 0;
	if(c >= nGabCnt - 1)
	{
		for(int i = 0; i < c + nGabCnt; i++)
		{
			if(m_bGab[i] == 1)
			{
				gab_cnt++;
			}
		}
	}
	else
	{
		c = 0;
	}*/

	CLED_DisplayInspectDlg *pdlg = ((CLED_DisplayInspectDlg *)AfxGetApp()->m_pMainWnd);
	pdlg->SetGridDbClick(c + 1, r + 1, g + 1);
}	

/////////////////////////////////////////////////////////////////////////////
//	OnMouseMove
//		is sent when the user moves mouse over the grid area.
//	Params:
//		col, row	- coordinates of a cell that received mouse click event
//		point		- represents the screen point where the mouse event was detected
//		nFlags		- 
//		processed	- indicates if current event was processed by other control in the grid.
//	Return:
//		<none>
void MyCug::OnMouseMove(int col,long row,POINT *point,UINT nFlags,BOOL processed)
{
	UNREFERENCED_PARAMETER(col);
	UNREFERENCED_PARAMETER(row);
	UNREFERENCED_PARAMETER(*point);
	UNREFERENCED_PARAMETER(nFlags);
	UNREFERENCED_PARAMETER(processed);
}

/////////////////////////////////////////////////////////////////////////////
//	OnTH_LClicked
//		Sent whenever the user clicks the left mouse button within the top heading
//		this message is sent when the button goes down then again when the button goes up
//	Params:
//		col, row	- coordinates of a cell that received mouse click event
//		updn		- is TRUE if mouse button is 'down' and FALSE if button is 'up'
//		processed	- indicates if current event was processed by other control in the grid.
//		rect		- represents the CDC rectangle of cell in question
//		point		- represents the screen point where the mouse event was detected
//	Return:
//		<none>
void MyCug::OnTH_LClicked(int col,long row,int updn,RECT *rect,POINT *point,BOOL processed)
{
	UNREFERENCED_PARAMETER(col);
	UNREFERENCED_PARAMETER(row);
	UNREFERENCED_PARAMETER(updn);
	UNREFERENCED_PARAMETER(*rect);
	UNREFERENCED_PARAMETER(*point);
	UNREFERENCED_PARAMETER(processed);
}

/////////////////////////////////////////////////////////////////////////////
//	OnTH_RClicked
//		Sent whenever the user clicks the right mouse button within the top heading
//		this message is sent when the button goes down then again when the button goes up
//	Params:
//		col, row	- coordinates of a cell that received mouse click event
//		updn		- is TRUE if mouse button is 'down' and FALSE if button is 'up'
//		processed	- indicates if current event was processed by other control in the grid.
//		rect		- represents the CDC rectangle of cell in question
//		point		- represents the screen point where the mouse event was detected
//	Return:
//		<none>
void MyCug::OnTH_RClicked(int col,long row,int updn,RECT *rect,POINT *point,BOOL processed)
{
	UNREFERENCED_PARAMETER(col);
	UNREFERENCED_PARAMETER(row);
	UNREFERENCED_PARAMETER(updn);
	UNREFERENCED_PARAMETER(*rect);
	UNREFERENCED_PARAMETER(*point);
	UNREFERENCED_PARAMETER(processed);
}

/////////////////////////////////////////////////////////////////////////////
//	OnTH_DClicked
//		Sent whenever the user double clicks the left mouse
//		button within the top heading
//	Params:
//		col, row	- coordinates of a cell that received mouse click event
//		processed	- indicates if current event was processed by other control in the grid.
//		rect		- represents the CDC rectangle of cell in question
//		point		- represents the screen point where the mouse event was detected
//	Return:
//		<none>
void MyCug::OnTH_DClicked(int col,long row,RECT *rect,POINT *point,BOOL processed)
{
	UNREFERENCED_PARAMETER(col);
	UNREFERENCED_PARAMETER(row);
	UNREFERENCED_PARAMETER(*rect);
	UNREFERENCED_PARAMETER(*point);
	UNREFERENCED_PARAMETER(processed);
}

/////////////////////////////////////////////////////////////////////////////
//	OnSH_LClicked
//		Sent whenever the user clicks the left mouse button within the side heading
//		this message is sent when the button goes down then again when the button goes up
//	Params:
//		col, row	- coordinates of a cell that received mouse click event
//		updn		- is TRUE if mouse button is 'down' and FALSE if button is 'up'
//		processed	- indicates if current event was processed by other control in the grid.
//		rect		- represents the CDC rectangle of cell in question
//		point		- represents the screen point where the mouse event was detected
//	Return:
//		<none>
void MyCug::OnSH_LClicked(int col,long row,int updn,RECT *rect,POINT *point,BOOL processed)
{
	UNREFERENCED_PARAMETER(col);
	UNREFERENCED_PARAMETER(row);
	UNREFERENCED_PARAMETER(updn);
	UNREFERENCED_PARAMETER(*rect);
	UNREFERENCED_PARAMETER(*point);
	UNREFERENCED_PARAMETER(processed);
}

/////////////////////////////////////////////////////////////////////////////
//	OnSH_RClicked
//		Sent whenever the user clicks the right mouse button within the side heading
//		this message is sent when the button goes down then again when the button goes up
//	Params:
//		col, row	- coordinates of a cell that received mouse click event
//		updn		- is TRUE if mouse button is 'down' and FALSE if button is 'up'
//		processed	- indicates if current event was processed by other control in the grid.
//		rect		- represents the CDC rectangle of cell in question
//		point		- represents the screen point where the mouse event was detected
//	Return:
//		<none>
void MyCug::OnSH_RClicked(int col,long row,int updn,RECT *rect,POINT *point,BOOL processed)
{
	UNREFERENCED_PARAMETER(col);
	UNREFERENCED_PARAMETER(row);
	UNREFERENCED_PARAMETER(updn);
	UNREFERENCED_PARAMETER(*rect);
	UNREFERENCED_PARAMETER(*point);
	UNREFERENCED_PARAMETER(processed);
}

/////////////////////////////////////////////////////////////////////////////
//	OnSH_DClicked
//		Sent whenever the user double clicks the left mouse button within the side heading
//	Params:
//		col, row	- coordinates of a cell that received mouse click event
//		processed	- indicates if current event was processed by other control in the grid.
//		rect		- represents the CDC rectangle of cell in question
//		point		- represents the screen point where the mouse event was detected
//	Return:
//		<none>
void MyCug::OnSH_DClicked(int col,long row,RECT *rect,POINT *point,BOOL processed)
{
	UNREFERENCED_PARAMETER(col);
	UNREFERENCED_PARAMETER(row);
	UNREFERENCED_PARAMETER(*rect);
	UNREFERENCED_PARAMETER(*point);
	UNREFERENCED_PARAMETER(processed);
}

/////////////////////////////////////////////////////////////////////////////
//	OnCB_LClicked
//		Sent whenever the user clicks the left mouse button within the top corner button
//		this message is sent when the button goes down then again when the button goes up
//	Params:
//		updn		- is TRUE if mouse button is 'down' and FALSE if button is 'up'
//		processed	- indicates if current event was processed by other control in the grid.
//		rect		- represents the CDC rectangle of cell in question
//		point		- represents the screen point where the mouse event was detected
//	Return:
//		<none>
void MyCug::OnCB_LClicked(int updn,RECT *rect,POINT *point,BOOL processed)
{
	UNREFERENCED_PARAMETER(updn);
	UNREFERENCED_PARAMETER(*rect);
	UNREFERENCED_PARAMETER(*point);
	UNREFERENCED_PARAMETER(processed);
}

/////////////////////////////////////////////////////////////////////////////
//	OnCB_RClicked
//		Sent whenever the user clicks the right mouse button within the top corner button
//		this message is sent when the button goes down then again when the button goes up
//	Params:
//		updn		- is TRUE if mouse button is 'down' and FALSE if button is 'up'
//		processed	- indicates if current event was processed by other control in the grid.
//		rect		- represents the CDC rectangle of cell in question
//		point		- represents the screen point where the mouse event was detected
//	Return:
//		<none>
void MyCug::OnCB_RClicked(int updn,RECT *rect,POINT *point,BOOL processed)
{
	UNREFERENCED_PARAMETER(updn);
	UNREFERENCED_PARAMETER(*rect);
	UNREFERENCED_PARAMETER(*point);
	UNREFERENCED_PARAMETER(processed);
}

/////////////////////////////////////////////////////////////////////////////
//	OnCB_DClicked
//		Sent whenever the user double clicks the left mouse
//		button within the top corner button
//	Params:
//		processed	- indicates if current event was processed by other control in the grid.
//		rect		- represents the CDC rectangle of cell in question
//		point		- represents the screen point where the mouse event was detected
//	Return:
//		<none>
void MyCug::OnCB_DClicked(RECT *rect,POINT *point,BOOL processed)
{
	UNREFERENCED_PARAMETER(*rect);
	UNREFERENCED_PARAMETER(*point);
	UNREFERENCED_PARAMETER(processed);
}

/////////////////////////////////////////////////////////////////////////////
//	OnKeyDown
//		Sent when grid received a WM_KEYDOWN message, usually as a result
//		of a user pressing any key on the keyboard.
//	Params:
//		vcKey		- virtual key code of the key user has pressed
//		processed	- indicates if current event was processed by other control in the grid.
//	Return:
//		<none>
void MyCug::OnKeyDown(UINT *vcKey,BOOL processed)
{
	UNREFERENCED_PARAMETER(*vcKey);
	UNREFERENCED_PARAMETER(processed);
	//	GetScrollBarCtrl(SB_HORZ)->EnableScrollBar(ESB_DISABLE_BOTH);
}

/////////////////////////////////////////////////////////////////////////////
//	OnKeyUp
//		Sent when grid received a WM_KEYUP message, usually as a result
//		of a user releasing a key.
//	Params:
//		vcKey		- virtual key code of the key user has pressed
//		processed	- indicates if current event was processed by other control in the grid.
//	Return:
//		<none>
void MyCug::OnKeyUp(UINT *vcKey,BOOL processed)
{
	UNREFERENCED_PARAMETER(*vcKey);
	UNREFERENCED_PARAMETER(processed);
	//	GetScrollBarCtrl(SB_HORZ)->EnableScrollBar(ESB_DISABLE_BOTH);
}

/////////////////////////////////////////////////////////////////////////////
//	OnCharDown
//		Sent when grid received a WM_CHAR message, usually as a result
//		of a user pressing any key that represents a printable character.
//	Params:
//		vcKey		- virtual key code of the key user has pressed
//		processed	- indicates if current event was processed by other control in the grid.
//	Return:
//		<none>
void MyCug::OnCharDown(UINT *vcKey,BOOL processed)
{
	UNREFERENCED_PARAMETER(*vcKey);
	UNREFERENCED_PARAMETER(processed);
	//	GetScrollBarCtrl(SB_HORZ)->EnableScrollBar(ESB_DISABLE_BOTH);
}

/////////////////////////////////////////////////////////////////////////////
//	OnGetCell
//		This message is sent every time the grid needs to
//		draw a cell in the grid. At this point the cell
//		object has been populated with all of the information
//		that will be used to draw the cell. This information
//		can now be changed before it is used for drawing.
//	Warning:
//		This notification is called for each cell that needs to be painted
//		Placing complicated calculations here will slowdown the refresh speed.
//	Params:
//		col, row	- coordinates of cell currently drawing
//		cell		- pointer to the cell object that is being drawn
//	Return:
//		<none>
void MyCug::OnGetCell(int col,long row,CUGCell *cell)
{
	UNREFERENCED_PARAMETER(col);
	UNREFERENCED_PARAMETER(row);
	UNREFERENCED_PARAMETER(*cell);
}

/////////////////////////////////////////////////////////////////////////////
//	OnSetCell
//		This notification is sent every time a cell is set,
//		here you have a chance to make final modification
//		to the cell's content before it is saved to the data source.
//	Params:
//		col, row	- coordinates of cell currently saving
//		cell		- pointer to the cell object that is being saved
//	Return:
//		<none>
void MyCug::OnSetCell(int col,long row,CUGCell *cell)
{
	UNREFERENCED_PARAMETER(col);
	UNREFERENCED_PARAMETER(row);
	UNREFERENCED_PARAMETER(*cell);
}

/////////////////////////////////////////////////////////////////////////////
//	OnDataSourceNotify
//		This message is sent from a data source and this message
//		depends on the data source - check the information
//		on the data source(s) being used
//		- The ID of the Data source is also returned
//	Params:
//		ID		- datasource ID
//		msg		- message ID to identify current process
//		param	- additional information or object that might be needed
//	Return:
//		<none>
void MyCug::OnDataSourceNotify(int ID,long msg,long param)
{
	UNREFERENCED_PARAMETER(ID);
	UNREFERENCED_PARAMETER(msg);
	UNREFERENCED_PARAMETER(param);
}

/////////////////////////////////////////////////////////////////////////////
//	OnCellTypeNotify
//		This notification is sent by the celltype and it is different from cell-type
//		to celltype and even from notification to notification.  It is usually used to
//		provide the developer with some feed back on the cell events and sometimes to
//		ask the developer if given event is to be accepted or not
//	Params:
//		ID			- celltype ID
//		col, row	- co-ordinates cell that is processing the message
//		msg			- message ID to identify current process
//		param		- additional information or object that might be needed
//	Return:
//		TRUE - to allow celltype event
//		FALSE - to disallow the celltype event
int MyCug::OnCellTypeNotify(long ID,int col,long row,long msg,long param)
{
	UNREFERENCED_PARAMETER(ID);
	UNREFERENCED_PARAMETER(col);
	UNREFERENCED_PARAMETER(row);
	UNREFERENCED_PARAMETER(msg);
	UNREFERENCED_PARAMETER(param);
	return TRUE;
}

/////////////////////////////////////////////////////////////////////////////
//	OnEditStart
//		This message is sent whenever the grid is ready to start editing a cell
//	Params:
//		col, row - location of the cell that edit was requested over
//		edit -	pointer to a pointer to the edit control, allows for swap of edit control
//				if edit control is swapped permanently (for the whole grid) is it better
//				to use 'SetNewEditClass' function.
//	Return:
//		TRUE - to allow the edit to start
//		FALSE - to prevent the edit from starting
int MyCug::OnEditStart(int col, long row,CWnd **edit)
{
	UNREFERENCED_PARAMETER(col);
	UNREFERENCED_PARAMETER(row);
	UNREFERENCED_PARAMETER(**edit);
	return TRUE;
}

/////////////////////////////////////////////////////////////////////////////
//	OnEditVerify
//		This notification is sent every time the user hits a key while in edit mode.
//		It is mostly used to create custom behavior of the edit control, because it is
//		so easy to allow or disallow keys hit.
//	Params:
//		col, row	- location of the edit cell
//		edit		-	pointer to the edit control
//		vcKey		- virtual key code of the pressed key
//	Return:
//		TRUE - to accept pressed key
//		FALSE - to do not accept the key
int MyCug::OnEditVerify(int col, long row,CWnd *edit,UINT *vcKey)
{
	UNREFERENCED_PARAMETER(col);
	UNREFERENCED_PARAMETER(row);
	UNREFERENCED_PARAMETER(*edit);
	UNREFERENCED_PARAMETER(*vcKey);
	return TRUE;
}

/////////////////////////////////////////////////////////////////////////////
//	OnEditFinish
//		This notification is sent when the edit is being finished
//	Params:
//		col, row	- coordinates of the edit cell
//		edit		- pointer to the edit control
//		string		- actual string that user typed in
//		cancelFlag	- indicates if the edit is being canceled
//	Return:
//		TRUE - to allow the edit to proceed
//		FALSE - to force the user back to editing of that same cell
int MyCug::OnEditFinish(int col, long row,CWnd *edit,LPCTSTR string,BOOL cancelFlag)
{
	UNREFERENCED_PARAMETER(col);
	UNREFERENCED_PARAMETER(row);
	UNREFERENCED_PARAMETER(*edit);
	UNREFERENCED_PARAMETER(string);
	UNREFERENCED_PARAMETER(cancelFlag);
	return TRUE;
}

/////////////////////////////////////////////////////////////////////////////
//	OnEditContinue
//		This notification is called when the user pressed 'tab' or 'enter' keys
//		Here you have a chance to modify the destination cell
//	Params:
//		oldcol, oldrow - edit cell that is loosing edit focus
//		newcol, newrow - cell that the edit is going into, by changing their
//							values you are able to change where to edit next
//	Return:
//		TRUE - allow the edit to continue
//		FALSE - to prevent the move, the edit will be stopped
int MyCug::OnEditContinue(int oldcol,long oldrow,int* newcol,long* newrow)
{
	UNREFERENCED_PARAMETER(oldcol);
	UNREFERENCED_PARAMETER(oldrow);
	UNREFERENCED_PARAMETER(*newcol);
	UNREFERENCED_PARAMETER(*newrow);
	return TRUE;
}

/////////////////////////////////////////////////////////////////////////////
//	OnMenuCommand
//		This notification is called when the user has selected a menu item
//		in the pop-up menu.
//	Params:
//		col, row - the cell coordinates of where the menu originated from
//		section - identify for which portion of the gird the menu is for.
//				  possible sections:
//						UG_TOPHEADING, UG_SIDEHEADING,UG_GRID
//						UG_HSCROLL  UG_VSCROLL  UG_CORNERBUTTON
//		item - ID of the menu item selected
//	Return:
//		<none>
void MyCug::OnMenuCommand(int col,long row,int section,int item)
{
	UNREFERENCED_PARAMETER(col);
	UNREFERENCED_PARAMETER(row);
	UNREFERENCED_PARAMETER(section);
	UNREFERENCED_PARAMETER(item);
}

/////////////////////////////////////////////////////////////////////////////
//	OnMenuStart
//		Is called when the pop up menu is about to be shown
//	Params:
//		col, row	- the cell coordinates of where the menu originated from
//		setcion		- identify for which portion of the gird the menu is for.
//					possible sections:
//						UG_TOPHEADING, UG_SIDEHEADING,UG_GRID
//						UG_HSCROLL  UG_VSCROLL  UG_CORNERBUTTON
//	Return:
//		TRUE - to allow menu to show
//		FALSE - to prevent the menu from poping up
int MyCug::OnMenuStart(int col,long row,int section)
{
	UNREFERENCED_PARAMETER(col);
	UNREFERENCED_PARAMETER(row);
	UNREFERENCED_PARAMETER(section);
	return TRUE;
}

/////////////////////////////////////////////////////////////////////////////
//	OnGetContextHelpID 
//		this notification is called as result of context help bing activated
//		over the UG area and should return context help ID to be displayed
//	Params:
//		col, row	- coordinates of cell which received the message
//		section		- grid section which received this message
//	Return:
//		Context help ID to be shown in the help.
DWORD MyCug::OnGetContextHelpID( int col, long row, int section )
{
	UNREFERENCED_PARAMETER(col);
	UNREFERENCED_PARAMETER(row);
	UNREFERENCED_PARAMETER(section);
	return 0;
}

/////////////////////////////////////////////////////////////////////////////
//	OnHint
//		Is called whent the hint is about to be displayed on the main grid area,
//		here you have a chance to set the text that will be shown
//	Params:
//		col, row	- the cell coordinates of where the menu originated from
//		string		- pointer to the string that will be shown
//	Return:
//		TRUE - to show the hint
//		FALSE - to prevent the hint from showing
int MyCug::OnHint(int col,long row,int section,CString *string)
{
	UNREFERENCED_PARAMETER(section);
	string->Format( _T("Col:%d Row:%ld") ,col,row);
	return TRUE;
}

/////////////////////////////////////////////////////////////////////////////
//	OnVScrollHint
//		Is called when the hint is about to be displayed on the vertical scroll bar,
//		here you have a chance to set the text that will be shown
//	Params:
//		row		- current top row
//		string	- pointer to the string that will be shown
//	Return:
//		TRUE - to show the hint
//		FALSE - to prevent the hint from showing
int MyCug::OnVScrollHint(long row,CString *string)
{
	UNREFERENCED_PARAMETER(row);
	UNREFERENCED_PARAMETER(*string);
	return TRUE;
}

/////////////////////////////////////////////////////////////////////////////
//	OnHScrollHint
//		Is called whent the hint is about to be displayed on the horizontal scroll bar,
//		here you have a chance to set the text that will be shown
//	Params:
//		col		- current left col
//		string	- pointer to the string that will be shown
//	Return:
//		TRUE - to show the hint
//		FALSE - to prevent the hint from showing
int MyCug::OnHScrollHint(int col,CString *string)
{
	UNREFERENCED_PARAMETER(col);
	UNREFERENCED_PARAMETER(*string);
	return TRUE;
}

#ifdef __AFXOLE_H__
/////////////////////////////////////////////////////////////////////////////
//	OLE
//		following 3 functions are to be used with drag and drop functionality
/////////////////////////////////////////////////////////////////////////////
//	OnDragEnter
//	Params:
//		pDataObject - 
//	Return:
//		DROPEFFECT_NONE - no drag and drop
//		DROPEFFECT_COPY - allow drag and drop for copying
DROPEFFECT MyCug::OnDragEnter(COleDataObject* pDataObject)
{
	UNREFERENCED_PARAMETER(*pDataObject);
	return DROPEFFECT_NONE;
}

/////////////////////////////////////////////////////////////////////////////
//	OnDragOver
//	Params:
//		col, row	-
//		pDataObject - 
//	Return:
//		DROPEFFECT_NONE - no drag and drop
//		DROPEFFECT_COPY - allow drag and drop for copying
DROPEFFECT MyCug::OnDragOver(COleDataObject* pDataObject,int col,long row)
{
	UNREFERENCED_PARAMETER(*pDataObject);
	UNREFERENCED_PARAMETER(col);
	UNREFERENCED_PARAMETER(row);
	return DROPEFFECT_NONE;
}

/////////////////////////////////////////////////////////////////////////////
//	OnDragDrop
//	Params:
//		col, row	-
//		pDataObject - 
//	Return:
//		DROPEFFECT_NONE - no drag and drop
//		DROPEFFECT_COPY - allow drag and drop for copying
DROPEFFECT MyCug::OnDragDrop(COleDataObject* pDataObject,int col,long row)
{
	UNREFERENCED_PARAMETER(*pDataObject);
	UNREFERENCED_PARAMETER(col);
	UNREFERENCED_PARAMETER(row);
	return DROPEFFECT_NONE;
}
#endif

/////////////////////////////////////////////////////////////////////////////
//	OnScreenDCSetup
//		Is called when each of the components are painted.
//	Params:
//		dc		- pointer to the current CDC object
//		section	- section of the grid painted.
//					possible sections:
//						UG_TOPHEADING, UG_SIDEHEADING,UG_GRID
//						UG_HSCROLL  UG_VSCROLL  UG_CORNERBUTTON
//	Return:
//		<none>
void MyCug::OnScreenDCSetup(CDC *dc,int section)
{
	UNREFERENCED_PARAMETER(*dc);
	UNREFERENCED_PARAMETER(section);
}

/////////////////////////////////////////////////////////////////////////////
//	OnSortEvaluate
//		Sent as a result of the 'SortBy' call, this is called for each cell
//		comparison and allows for customization of the sorting routines.
//		We provide following code as example of what could be done here,
//		you might have to modify it to give your application customized sorting.
//	Params:
//		cell1, cell2	- pointers to cells that are compared
//		flags			- identifies sort direction
//	Return:
//		value less than zero to identify that the cell1 comes before cell2
//		value equal to zero to identify that the cell1 and cell2 are equal
//		value greater than zero to identify that the cell1 comes after cell2
int MyCug::OnSortEvaluate(CUGCell *cell1,CUGCell *cell2,int flags)
{
	// if one of the cells is NULL, do not compare its text
	if ( cell1 == NULL && cell2 == NULL )
		return 0;
	else if ( cell1 == NULL )
		return 1;
	else if ( cell2 == NULL )
		return -1;

	if(flags&UG_SORT_DESCENDING)
	{
		CUGCell *ptr = cell1;
		cell1 = cell2;
		cell2 = ptr;
	}

	// initialize variables for numeric check
	double num1, num2;
	// compare the cells, with respect to the cell's datatype
	switch(cell1->GetDataType())
	{
	case UGCELLDATA_STRING:
		if(NULL == cell1->GetText() && NULL == cell2->GetText())
			return 0;
		if(NULL == cell1->GetText())
			return 1;
		if(NULL == cell2->GetText())
			return -1;
		return _tcscmp(cell1->GetText(),cell2->GetText());	
	case UGCELLDATA_NUMBER:
	case UGCELLDATA_BOOL:
	case UGCELLDATA_CURRENCY:
		num1 = cell1->GetNumber();
		num2 = cell2->GetNumber();
		if(num1 < num2)
			return -1;
		if(num1 > num2)
			return 1;
		return 0;
	default:
		// if datatype is not recognized, compare cell's text
		if(NULL == cell1->GetText())
			return 1;
		if(NULL == cell2->GetText())
			return -1;
		return _tcscmp(cell1->GetText(),cell2->GetText());	
	}
}

/////////////////////////////////////////////////////////////////////////////
//	OnTabSelected
//		Called when the user selects one of the tabs on the bottom of the screen
//	Params:
//		ID	- id of selected tab
//	Return:
//		<none>
void MyCug::OnTabSelected(int ID)
{
	UNREFERENCED_PARAMETER(ID);
}

/////////////////////////////////////////////////////////////////////////////
//	OnAdjustComponentSizes
//		Called when the grid components are being created,
//		sized, and arranged.  This notification provides
//		us with ability to further customize the way
//		the grid will be presented to the user.
//	Params:
//		grid, topHdg, sideHdg, cnrBtn, vScroll, hScroll, 
//		tabs	- sizes and location of each of the grid components
//	Return:
//		<none>
void MyCug::OnAdjustComponentSizes(RECT *grid,RECT *topHdg,RECT *sideHdg,
	RECT *cnrBtn,RECT *vScroll,RECT *hScroll,RECT *tabs)
{
	UNREFERENCED_PARAMETER(*grid);
	UNREFERENCED_PARAMETER(*topHdg);
	UNREFERENCED_PARAMETER(*sideHdg);
	UNREFERENCED_PARAMETER(*cnrBtn);
	UNREFERENCED_PARAMETER(*vScroll);
	UNREFERENCED_PARAMETER(*hScroll);
	UNREFERENCED_PARAMETER(*tabs);
} 

/////////////////////////////////////////////////////////////////////////////
//	OnDrawFocusRect
//		Called when the focus rect needs to be painted.
//	Params:
//		dc		- pointer to the current CDC object
//		rect	- rect of the cell that requires the focus rect
//	Return:
//		<none>
void MyCug::OnDrawFocusRect(CDC *dc,RECT *rect)
{
	//DrawExcelFocusRect(dc,rect);

	rect->bottom --;
	rect->right --;	
	dc->DrawFocusRect(rect);
}

/////////////////////////////////////////////////////////////////////////////
//	OnGetDefBackColor
//		Sent when the area behind the grid needs to be painted.
//	Params:
//		section - Id of the grid section that requested this color
//				  possible sections:
//						UG_TOPHEADING, UG_SIDEHEADING, UG_GRID
//	Return:
//		RGB value representing the color of choice
COLORREF MyCug::OnGetDefBackColor(int section)
{
	UNREFERENCED_PARAMETER(section);
	return RGB(255,255,255);
}

/////////////////////////////////////////////////////////////////////////////
//	OnSetFocus
//		Sent when the grid is gaining focus.
//	Note:
//		The grid will loose focus when the edit is started, or drop list shown
//	Params:
//		section - Id of the grid section gaining focus, usually UG_GRID
//				  possible sections:
//						UG_TOPHEADING, UG_SIDEHEADING, UG_GRID
//	Return:
//		<none>
//
void MyCug::OnSetFocus(int section)
{
	UNREFERENCED_PARAMETER(section);
}

/////////////////////////////////////////////////////////////////////////////
//	OnKillFocus
//		Sent when the grid is loosing focus.
//	Params:
//		section - Id of the grid section loosing focus, usually UG_GRID
//				  possible sections:
//						UG_TOPHEADING, UG_SIDEHEADING, UG_GRID
//		pNewWnd	- pointer to the window that is gaining focus
//	Return:
//		<none>
void MyCug::OnKillFocus(int section, CWnd *pNewWnd)
{
	UNREFERENCED_PARAMETER(section);
}

/////////////////////////////////////////////////////////////////////////////
//	OnColSwapStart
//		Called to inform the grid that the col swap was started on given col.
//	Params:
//		col - identifies the column
//	Return:
//		TRUE - to allow for the swap to take place
//		FALSE - to disallow the swap
BOOL MyCug::OnColSwapStart(int col)
{
	UNREFERENCED_PARAMETER(col);
	return TRUE;
}

/////////////////////////////////////////////////////////////////////////////
//	OnCanColSwap
//		Called when col swap is about to take place, here you can 'lock' certain
//		cols in place so that are not swappable.
//	Params:
//		fromCol - where the col originated from
//		toCol	- where the col will be located if the swap is allowed
//	Return:
//		TRUE - to allow for the swap to take place
//		FALSE - to disallow the swap
BOOL MyCug::OnCanColSwap(int fromCol,int toCol)
{
	UNREFERENCED_PARAMETER(fromCol);
	UNREFERENCED_PARAMETER(toCol);
	return TRUE;
}

/////////////////////////////////////////////////////////////////////////////
//	OnColSwapped
//		Called just after column-swap operation was completed.
//	Params:
//		fromCol - where the col originated from
//		toCol	- where the col will be located if the swap is allowed
//	Return:
//		<none>
void MyCug::OnColSwapped(int fromCol,int toCol)
{
	UNREFERENCED_PARAMETER(fromCol);
	UNREFERENCED_PARAMETER(toCol);
}

/////////////////////////////////////////////////////////////////////////////
//	OnTrackingWindowMoved
//		Called to notify the grid that the tracking window was moved
//	Params:
//		origRect	- from
//		newRect		- to location and size of the track window
//	Return:
//		<none>
void MyCug::OnTrackingWindowMoved(RECT *origRect,RECT *newRect)
{
	UNREFERENCED_PARAMETER(*origRect);
	UNREFERENCED_PARAMETER(*newRect);
}
