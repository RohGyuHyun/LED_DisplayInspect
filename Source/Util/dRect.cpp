#include "stdafx.h"
#include "dRect.h"
#include <cmath>

extern "C" __declspec(dllexport) double Rounding(double x, int digit)
{
	double rslt = 0.;
	rslt = (floor((x) * pow(float(10), digit) + 0.9f) / pow(float(10), digit));

	return rslt;
}

extern "C" __declspec(dllexport) BOOL _DeleteFile(CString strFolder, CString strFilename)
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
			
			if(!Filename.Compare(strFilename))
			{
				DeleteFile(Filename);
				break;
			}
		}
	}
	ff.Close();

	return TRUE;
}

CdPoint::CdPoint()
{
	x = 0.;
	y = 0.;
}

CdPoint::~CdPoint()
{

}

void CdPoint::SetPoint(double p_x, double p_y)
{
	x = p_x;
	y = p_y;
}

CdPoint CdPoint::GetPoint()
{
	CdPoint point;
	point.SetPoint(x, y);

	return point;
}

double CdPoint::GetX(){ return x; }
double CdPoint::GetY(){ return y; }

void CdPoint::MoveToX(double offset_x)
{
	x+=offset_x;
}

void CdPoint::MoveToY(double offset_y)
{
	y+=offset_y;
}

void CdPoint::MoveToXY(double offset_x, double offset_y)
{
	x+=offset_x;
	y+=offset_y;
}

CdRect::CdRect()
{
	left = 0.;
	top = 0.;
	right = 0.;
	bottom = 0.;
	center_x = 0.;
	center_y = 0.;
	width = 0.;
	height = 0.;
}

CdRect::~CdRect()
{

}

void CdRect::SetdRect(CRect rect, double offset)
{
	left = rect.left - offset;
	top = rect.top - offset;
	right = rect.right + offset;
	bottom = rect.bottom + offset;

	width = rect.Width() + (offset * 2);
	center_x = rect.CenterPoint().x;
	height = rect.Height() + (offset * 2);
	center_y = rect.CenterPoint().y;
}

void CdRect::SetdRect(CdRect rect, double offset)
{
	left = rect.left - offset;
	top = rect.top - offset;
	right = rect.right + offset;
	bottom = rect.bottom + offset;

	width = rect.GetWidth() + (offset * 2);
	center_x = rect.GetCenter().GetX();
	height = rect.GetHeight() + (offset * 2);
	center_y = rect.GetCenter().GetY();
}

void CdRect::SetdRectLTRB(double l, double t, double r, double b, double offset)
{
	left = l - offset;
	top = t - offset;
	right = r + offset;
	bottom = b + offset;

	width = right - left;
	center_x = right - (width * 0.5);
	height = bottom - top;
	center_y = bottom - (height * 0.5);
}

void CdRect::SetdRectCWH(double dCx, double dCy, double dWidth, double dHeight, double offset)
{
	left = dCx - (dWidth * 0.5) - offset;
	top = dCy - (dHeight * 0.5) - offset;
	right = dCx + (dWidth * 0.5) + offset;
	bottom = dCy + (dHeight * 0.5) + offset;

	width = right - left;
	center_x = right - (width * 0.5);
	height = bottom - top;
	center_y = bottom - (height * 0.5);
}

BOOL CdRect::PointInRect(double x, double y)
{
	if( (left <= x) && (x <= right) && (top <= y) && (y <= bottom))
	{
		return TRUE;
	}
	else
	{
		return FALSE;
	}
}

void CdRect::MoveToX(double offset_x)
{
	left+=offset_x;
	right+=offset_x;
}

void CdRect::MoveToY(double offset_y)
{
	top+=offset_y;
	bottom+=offset_y;
}

void CdRect::MoveToXY(double offset_x, double offset_y)
{
	left+=offset_x;
	right+=offset_x;
	top+=offset_y;
	bottom+=offset_y;
}

BOOL CdRect::IntersectRect(CdRect rect, double main_offset, double sub_offset)
{
	CdRect main_Rect, sub_Rect;
	main_Rect.SetdRectLTRB(left, top, right, bottom);
	if(main_offset > 0.)
	{
		main_Rect.SetdRectLTRB(left, top, right, bottom);
		SetdRectLTRB(left, top, right, bottom, main_offset);
	}
	sub_Rect.SetdRectLTRB(rect.GetLeft(), rect.GetTop(), rect.GetRight(), rect.GetBottom(), sub_offset);

	if(main_Rect.PointInRect(sub_Rect.GetLeft(), sub_Rect.GetTop()) || main_Rect.PointInRect(sub_Rect.GetRight(), sub_Rect.GetBottom()) ||
		main_Rect.PointInRect(sub_Rect.GetLeft(), sub_Rect.GetBottom()) || main_Rect.PointInRect(sub_Rect.GetRight(), sub_Rect.GetTop()) ||
		sub_Rect.PointInRect(main_Rect.GetLeft(), main_Rect.GetTop()) || sub_Rect.PointInRect(main_Rect.GetRight(), main_Rect.GetBottom()) ||
		sub_Rect.PointInRect(main_Rect.GetLeft(), main_Rect.GetBottom()) || sub_Rect.PointInRect(main_Rect.GetRight(), main_Rect.GetTop()) ||
		main_Rect.PointInRect(sub_Rect.GetCenter().GetX(), sub_Rect.GetCenter().GetY()) || sub_Rect.PointInRect(main_Rect.GetCenter().GetX(), main_Rect.GetCenter().GetY()))
	{
		return TRUE;
	}
	else
	{
		return FALSE;
	}
}

void CdRect::UnionRect(CdRect rect)
{
	double uleft, utop, uright, ubottom;
	if(left < rect.GetLeft())
	{
		uleft = left;
	}
	else
	{
		uleft = rect.GetLeft();
	}

	if(top < rect.GetTop())
	{
		utop = top;
	}
	else
	{
		utop = rect.GetTop();
	}

	if(right < rect.GetRight())
	{
		uright = rect.GetRight();
	}
	else
	{
		uright = right;
	}

	if(bottom < rect.GetBottom())
	{
		ubottom = rect.GetBottom();
	}
	else
	{
		ubottom = bottom;
	}
	SetdRectLTRB(uleft, utop, uright, ubottom);
}

void CdRect::UnionRect(CRect rect)
{
	double uleft, utop, uright, ubottom;
	if(left < rect.left)
	{
		uleft = left;
	}
	else
	{
		uleft = rect.left;
	}

	if(top < rect.top)
	{
		utop = top;
	}
	else
	{
		utop = rect.top;
	}

	if(right < rect.right)
	{
		uright = rect.right;
	}
	else
	{
		uright = right;
	}

	if(bottom < rect.bottom)
	{
		ubottom = rect.bottom;
	}
	else
	{
		ubottom = bottom;
	}
	SetdRectLTRB(uleft, utop, uright, ubottom);
}

void CdRect::UnionRect(CdRect rect1, CdRect rect2)
{
	double uleft, utop, uright, ubottom;
	if(rect1.GetLeft() < rect2.GetLeft())
	{
		uleft = rect1.GetLeft();
	}
	else
	{
		uleft = rect2.GetLeft();
	}

	if(rect1.GetTop() < rect2.GetTop())
	{
		utop = rect1.GetTop();
	}
	else
	{
		utop = rect2.GetTop();
	}

	if(rect1.GetRight() < rect2.GetRight())
	{
		uright = rect2.GetRight();
	}
	else
	{
		uright = rect1.GetRight();
	}

	if(rect1.GetBottom() < rect2.GetBottom())
	{
		ubottom = rect2.GetBottom();
	}
	else
	{
		ubottom = rect1.GetBottom();
	}
	SetdRectLTRB(uleft, utop, uright, ubottom);
}

void CdRect::UnionRect(CRect rect1, CdRect rect2)
{
	double uleft, utop, uright, ubottom;
	if(rect1.left < rect2.GetLeft())
	{
		uleft = rect1.left;
	}
	else
	{
		uleft = rect2.GetLeft();
	}

	if(rect1.top < rect2.GetTop())
	{
		utop = rect1.top;
	}
	else
	{
		utop = rect2.GetTop();
	}

	if(rect1.right < rect2.GetRight())
	{
		uright = rect2.GetRight();
	}
	else
	{
		uright = rect1.right;
	}

	if(rect1.bottom < rect2.GetBottom())
	{
		ubottom = rect2.GetBottom();
	}
	else
	{
		ubottom = rect1.bottom;
	}
	SetdRectLTRB(uleft, utop, uright, ubottom);
}

void CdRect::UnionRect(CRect rect1, CRect rect2)
{
	double uleft, utop, uright, ubottom;
	if(rect1.left < rect2.left)
	{
		uleft = rect1.left;
	}
	else
	{
		uleft = rect2.left;
	}

	if(rect1.top < rect2.top)
	{
		utop = rect1.top;
	}
	else
	{
		utop = rect2.top;
	}

	if(rect1.right < rect2.right)
	{
		uright = rect2.right;
	}
	else
	{
		uright = rect1.right;
	}

	if(rect1.bottom < rect2.bottom)
	{
		ubottom = rect2.bottom;
	}
	else
	{
		ubottom = rect1.bottom;
	}
	SetdRectLTRB(uleft, utop, uright, ubottom);
}

void CdRect::Rotate90()
{
	CdRect sub_rect;
	double l, t, r, b;
	sub_rect.SetdRectLTRB(left, top, right, bottom);
	l = sub_rect.GetCenter().GetX() - (sub_rect.GetHeight() / 2);
	r = sub_rect.GetCenter().GetX() + (sub_rect.GetHeight() / 2);
	t = sub_rect.GetCenter().GetY() - (sub_rect.GetWidth() / 2);
	b = sub_rect.GetCenter().GetY() + (sub_rect.GetWidth() / 2);
	SetdRectLTRB(l, t, r, b);
}

double CdRect::GetLeft(){ return left; }
double CdRect::GetRight(){ return right; }
double CdRect::GetTop(){ return top; }
double CdRect::GetBottom(){ return bottom; }
double CdRect::GetWidth(){ return width; }
double CdRect::GetHeight(){ return height; }

CdPoint CdRect::GetLeftTop()
{
	CdPoint point;
	point.SetPoint(left, top);
	return point;
}

CdPoint CdRect::GetRightBottom()
{
	CdPoint point;
	point.SetPoint(right, bottom);
	return point;
}

CdPoint CdRect::GetCenter()
{
	CdPoint point;
	point.SetPoint(center_x, center_y);
	return point;
}