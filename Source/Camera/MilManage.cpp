#include "StdAfx.h"
#include "MilManage.h"

#ifdef CAMERA
CMilManage::CMilManage(void)
{
}


CMilManage::~CMilManage(void)
{
}


BOOL CMilManage::Open(HWND *_disp_hwnd, double *dZoom_rat)
{
	MappAlloc(M_DEFAULT, &m_MilApplication);
	MsysAlloc(M_SYSTEM_HOST, 0, M_DEFAULT, &m_MilSystem);

	MbufAlloc2d(m_MilSystem, CAMERA_WIDTH, CAMERA_HEIGHT, 8 + M_UNSIGNED, M_IMAGE + M_PROC, &m_MilGrabBuf);
	MbufClear(m_MilGrabBuf, 0x00);
	for(int i = 0; i < MAX_DISP; i++)
	{
		MbufAlloc2d(m_MilSystem, CAMERA_WIDTH, CAMERA_HEIGHT, 8 + M_UNSIGNED, M_IMAGE + M_DISP + M_PROC, &m_MilDispBuf[i]);
		MbufClear(m_MilDispBuf[i], 0x00);

		MdispAlloc(m_MilSystem, M_DEFAULT, "M_DEFAULT", M_WINDOWED, &m_MilDisplay[i]);
		MdispSelectWindow(m_MilDisplay[i], m_MilDispBuf[i], _disp_hwnd[i]);
		MdispControl(m_MilDisplay[i], M_INTERPOLATION_MODE, M_FAST);

		if(i == MAIN_DISP)
		{
			MdispZoom(m_MilDisplay[i], dZoom_rat[0], dZoom_rat[0]);
		}
		else if(i == NG_VIEW_DISP)
		{
			MdispZoom(m_MilDisplay[i], dZoom_rat[1], dZoom_rat[1]);
		}
		else
		{
			MdispZoom(m_MilDisplay[i], 1.5, 1.5);
		}

		MdispControl(m_MilDisplay[i], M_OVERLAY, M_ENABLE);
		MdispInquire(m_MilDisplay[i], M_OVERLAY_ID, &m_MilOverlay[i]);
	}	

	for(int i = 0; i < 2; i++)
	{
		for(int j = 0; j < 3; j++)
		{
			MbufAlloc2d(m_MilSystem, CAMERA_WIDTH, CAMERA_HEIGHT, 8 + M_UNSIGNED, M_IMAGE + M_PROC, &m_MilInspBuf[i][j]);
			MbufClear(m_MilInspBuf[i][j], 0x00);
		}
	}

	return TRUE;
}



BOOL CMilManage::Close()
{
	if(!MappGetError(M_GLOBAL, M_NULL))
	{
		MappControl(M_ERROR, M_PRINT_DISABLE);
		for(int i = 0; i < 2; i++)
		{
			for(int j = 0; j < 3; j++)
			{
				MbufFree(m_MilInspBuf[i][j]);
			}
		}

		for(int i = 0; i < MAX_DISP; i++)
		{
			MbufFree(m_MilDispBuf[i]);
			//MbufFree(m_MilOverlay[i]);
		}

		MbufFree(m_MilGrabBuf);
		for(int i = 0; i < MAX_DISP; i++)
		{
			MdispFree(m_MilDisplay[i]);
		}
		MsysFree(m_MilSystem);
 		MappFree(m_MilApplication);
	}

	return TRUE;
}

MIL_ID *CMilManage::GetMilSystem()
{
	return &m_MilSystem;
}
#ifdef DCF
MIL_ID *CMilManage::GetMilDigitizer()
{
	return &m_MilDigitizer;
}
#endif

MIL_ID *CMilManage::GetMilGrabBuf()
{
	return &m_MilGrabBuf;
}

MIL_ID *CMilManage::GetMilDisplay(int index)
{
	return &m_MilDisplay[index];
}

MIL_ID *CMilManage::GetMilDispBuf(int index)
{
	return &m_MilDispBuf[index];
}

MIL_ID *CMilManage::GetMilOverlay(int index)
{
	return &m_MilOverlay[index];
}

MIL_ID *CMilManage::GetMilInspBuf(int ins_idx, int index)
{
	return &m_MilInspBuf[ins_idx][index];
}
#endif