

#define ROUNDING(x, dig)    ( floor((x) * pow(float(10), dig) + 0.5f) / pow(float(10), dig) )

extern "C" __declspec(dllexport) double Rounding(double x, int digit);
extern "C" __declspec(dllexport) BOOL _DeleteFile(CString path, CString file_name);

class CdPoint
{
public:
	CdPoint();
	~CdPoint();
protected:
	double x;
	double y;
public:
	void SetPoint(double p_x, double p_y);
	CdPoint GetPoint();
	double GetX();
	double GetY();
	void MoveToX(double offset_x);
	void MoveToY(double offset_y);
	void MoveToXY(double offset_x, double offset_y);
};

class CdRect
{
public:
	CdRect();
	~CdRect();
protected:
	double left;
	double top;
	double right;
	double bottom;
	double center_x;
	double center_y;
	double width;
	double height;
public:
	void SetdRect(CRect rect, double offset = 0.);
	void SetdRect(CdRect rect, double offset = 0.);
	void SetdRectLTRB(double left, double top, double right, double bottom, double offset = 0.);
	void SetdRectCWH(double dCenterX, double dCenterY, double dWidth, double dHeight, double offset = 0.);
	BOOL PointInRect(double x, double y);
	void MoveToX(double offset_x);
	void MoveToY(double offset_y);
	void MoveToXY(double offset_x, double offset_y);
	BOOL IntersectRect(CdRect rect, double main_offset = 0., double sub_offset = 0.);
	void UnionRect(CdRect rect);
	void UnionRect(CRect rect);
	void UnionRect(CdRect rect1, CdRect rect2);
	void UnionRect(CRect rect1, CdRect rect2);
	void UnionRect(CRect rect1, CRect rect2);
	void Rotate90();
	double GetLeft();
	double GetRight();
	double GetTop();
	double GetBottom();
	double GetWidth();
	double GetHeight();
	CdPoint GetLeftTop();
	CdPoint GetRightBottom();
	CdPoint GetCenter();
};