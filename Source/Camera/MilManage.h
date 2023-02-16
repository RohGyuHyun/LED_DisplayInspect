#pragma once
#include "Mil.h"
#include "typedef.h"

#ifdef CAMERA
class CMilManage
{
public:
	CMilManage(void);
	~CMilManage(void);
private:
	MIL_ID m_MilApplication;
	MIL_ID m_MilSystem;
	MIL_ID m_MilGrabBuf;
	MIL_ID m_MilDisplay[MAX_DISP];
	MIL_ID m_MilDispBuf[MAX_DISP];
	MIL_ID m_MilInspBuf[2][3];

	MIL_ID m_MilOverlay[MAX_DISP];
	
public:
	BOOL Open(HWND *_disp_hwnd, double *dZoom_rat);
	BOOL Close();

	MIL_ID *GetMilSystem();
	MIL_ID *GetMilDigitizer();
	MIL_ID *GetMilGrabBuf();
	MIL_ID *GetMilDisplay(int index);
	MIL_ID *GetMilDispBuf(int index);
	MIL_ID *GetMilOverlay(int index);
	MIL_ID *GetMilInspBuf(int ins_idx, int index);

	
};

#endif