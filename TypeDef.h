
#pragma once
#include "share.h"

#include "mil.h"
#include "FAS_EziMOTIONPlusR.h"
#include "dRect.h"

#define NOTE_BOOK
#define BLOB_TEST
#define INSPECTION_TEST
#define INSPECTION_MASK_TEST
//#define SHIFT_INSPECTION

#ifndef NOTE_BOOK
#define MOTION
#define LIGHT
#define CAMERA
#define BASLER
#else
//#define TEST
#define CAMERA
#define TEST_INSP_IMAGE_SAVE	0
#define TEST_INSP_DISP
#endif

#ifdef SHIFT_INSPECTION
#define MAX_SHIFT_CNT			5
#else
#define MAX_SHIFT_CNT			1
#endif

template<typename T>  //20160906 sc
inline T limit(const T& value)  //20160906 sc
{
	return ( (value > 255) ? 255 : ((value < 0) ? 0 : value) );
}

#define MAX_DEVICE_X_CNT		10
#define MAX_DEVICE_Y_CNT		10

#define PATTERN_FIND_MAX_NUM		1

#define RGB2GRAY(r, g, b) (0.299*(r) + 0.587*(g) + 0.114*(b))  //20160906 sc

#define GRID_CELL_WIDTH		80
#define GRID_CELL_HEIGHT	33
#define GRID_GAB_WIDTH		5
#define GRID_GAB_HEIGHT		GRID_GAB_WIDTH

#define INSPECTION_ALIGN_TEACH	0
#define INSPECTION_TEACH		1
#define INSPECTION_ALIGN		2
#define INSPECTION_EDGE			3
#define INSPECTION_MASK_SET		4
#define INSPECTION_EDGE2		5
#define INSPECTION_EDGE3		6
#define INSPECTION_TEST			10
#define INSPECTION_BLOB			20
#define INSPECTION_BLOB2		21
#define INSPECTION_BLOB3		22

#define MAX_INSP_TYPE			4
#define START_INSP_TYPE			INSPECTION_ALIGN

#define DEFAULT_DRIVE			"D:\\"

#define DRAW_RECTANGLE		0

#define TRACKER_MAX_NUM			20		//IMSI

#define BACK_COLOR_RED			RGB(255, 0, 0)
#define BACK_COLOR_GREEN		RGB(0, 255, 0)
#define BACK_COLOR_BLUE			RGB(0, 0, 255)

#define MAIN_DISP				0
#define NG_DISP_1				1
#define NG_DISP_2				2
#define NG_DISP_3				3
#define NG_VIEW_DISP			4

#define MAX_DISP				8
#define MAX_NG_DISP				3
#define MAX_LIGHT_PARA			4
#define MAX_GRAB_CNT			5		
#define MAX_INSP_GRAB_CNT		3
#define MAX_INS_PARA			TRACKER_MAX_NUM
#define MAX_RSLT_NUM			1024

#define MAX_BEFORE_RSLT_CNT		5

#define MAX_LIGHT				2
#define MAX_LIGHT_PORT_CNT		7
#define L_LIGHT_PACKET_SIZE		10
#define LIGHT_1_CH				1
#define LIGHT_2_CH				2

#define LIGHT_COM_PORT			6
#define LIGHT_COM_BAUDRATE		9600

#define TIMER_GRAB_LIVE			0
#define TIMER_TIME_SHOW			1
#define TIMER_TIME_SHOW			1
#define TIMER_ALL_ORIGIN_START	2
#define TIMER_MOTION_AMP		3

#define CAMERA_WIDTH			2592
#define CAMERA_HEIGHT			1944

#define DI_MAX_NUM				10
#define DO_MAX_NUM				DI_MAX_NUM
#define READ_INPUT				0
#define READ_OUTPUT				1

#define AUTO_WINDOW				0
#define MANUAL_WINDOW			1
#define TEACH_WINDOW			2
#define DATA_WINDOW				3
#define NG_WINDOW				4

#define DB_PATH					DEFAULT_DRIVE"DATABASE"
#define MODEL_PATH				DEFAULT_DRIVE"DATABASE\\MODEL"
#define LOG_PATH				DEFAULT_DRIVE"DATABASE\\LOG"
#define RESULT_PATH				DEFAULT_DRIVE"RESULT"
#define CAPA_PATH				RESULT_PATH"\\CAPA"
#define GRAB_IMAGE_SAVE_PATH	"\\GrabImage\\"
#define OK_IMAGE_SAVE_PATH		"\\OKImage\\"
#define NG_IMAGE_SAVE_PATH		"\\NGImage\\"

#define TEACH_PARA_FILE_NAME	"\\TeachPara.par"
#define DIO_LABEL_FILE_NAME		"\\DIOLabel.par"
#define MANUAL_PARA_FILE_NAME	"\\ManualPara.par"
#define SYSTEM_PARA				"\\System.par"
#define PARA_LOCK_FILE_NAME		"\\ParameterLock.par"  //20160906 sc

#define TYPE_LOG_MAIN			0
#define TYPE_LOG_MANUAL			1
#define TYPE_LOG_TEACH			2
#define TYPE_LOG_DATA			3
#define TYPE_LOG_DEBUG			4



#define MAIN_LOG_FILE_NAME		"MainLog.log"
#define MANUAL_LOG_FILE_NAME	"ManualLog.log"
#define TEACH_LOG_FILE_NAME		"TeachLog.log"
#define DATA_LOG_FILE_NAME		"DataLog.log"
#define DEBUG_LOG_FILE_NAME		"DebugLog.log"




#define INSPECTION_OK		0
#define ALIGN_NG			1
#define INSPECTION_NG		2
#define NON_TEACHING_IAMGE  3

#define MAX_THRESH_NUM		3

#define NG_DISPLAY_IAMGE_WIDTH		170
#define NG_DISPLAY_IAMGE_HEIGHT		100

#define NG_INFO_PRINT_X				NG_DISPLAY_IAMGE_WIDTH + 5
#define NG_INFO_PRINT_Y				NG_DISPLAY_IAMGE_HEIGHT + 5
#define NG_INFO_PRINT_GAP			15

#define MAX_MOVE_CNT			100

#define MOTION_DEFAULT_COMPORT		0
#define MAX_COMPORT_NUM				10
#define MAX_BAUD_RATE_NUM			8

#define COMBO_BAUD_RATE_0			0
#define COMBO_BAUD_RATE_1			1
#define COMBO_BAUD_RATE_2			2
#define COMBO_BAUD_RATE_3			3
#define COMBO_BAUD_RATE_4			4
#define COMBO_BAUD_RATE_5			5
#define COMBO_BAUD_RATE_6			6
#define COMBO_BAUD_RATE_7			7

#define BAUD_RATE_0					4800
#define BAUD_RATE_1					9600
#define BAUD_RATE_2					14400
#define BAUD_RATE_3					19200
#define BAUD_RATE_4					38400
#define BAUD_RATE_5					56000
#define BAUD_RATE_6					57600
#define BAUD_RATE_7					115200

#define MOTION_STEP_MODE			0
#define MOTION_JOG_MODE				1

#define TIMEOUT_MOTOR_MOVE				50000//ms
#define MAX_MOTION_CNT					3
#define MOTION_X						0
#define MOTION_Y						1
#define MOTION_Z						2

#define MOTION_Z_SW_LIMT				1000000

#define MOTION_STATE_CUR_POS			0
#define MOTION_STATE_AMP_FAULT_LEVEL	1
#define MOTION_STATE_LIMT_SENSER_P		2
#define MOTION_STATE_LIMT_SENSER_N		3
#define MOTION_STATE_HOME				4

#define TIMER_MOTION_HOME		100
#define TIMER_MOTION_MONITOR	200
#define TIMER_MOTION_MOVE_COMP	300
#define TIMER_MOTION_JOG		400

#define MAX_STR_NUM				512
#define MAX_MODEL_NUM			100

#define MODEL_NEW					0
#define MODEL_SELECT				1
#define MODEL_DELETE				2


#define DILATE_PIXEL				4
#define ERODE_PIXEL					DILATE_PIXEL

#define LANGUAGE_ENG				FALSE
#define LANGUAGE_KOR				TRUE

//20161019 ht 카메라 상태체크 위한 define
#define SUCCESS_CAMERA				0
#define ERROR_CAMERA_NOT_OPEN		1
#define ERROR_CAMERA_AVAILABLE		2


enum MOTION_INPUT_ADDRESS
{
	INPUT_SLVE0_SPARE_0					=	STEP_IN_BITMASK_PTA0,	
	INPUT_SLVE0_SPARE_1					=	STEP_IN_BITMASK_PTA1,	
	INPUT_SLVE0_SPARE_2					=	STEP_IN_BITMASK_PTA2,	
	INPUT_START_BUTTON_1				=	STEP_IN_BITMASK_USERIN0,
	INPUT_START_BUTTON_2				=	STEP_IN_BITMASK_USERIN1,
	INPUT_PAUSE_BUTTON					=	STEP_IN_BITMASK_USERIN2,
	INPUT_SAFETY_LIGHT_CURTAIN_DETECT	=	STEP_IN_BITMASK_USERIN3,
	INPUT_SLVE0_SPARE_7					=	STEP_IN_BITMASK_USERIN4,
	INPUT_SLVE0_SPARE_8					=	STEP_IN_BITMASK_USERIN5,
};

#define INPUT_START_BTN_1				0
#define INPUT_START_BTN_2				1
#define INPUT_PAUSE						2
#define INPUT_AREA_SENSER				3

enum MOTION_OUTPUT_ADDRESS
{
	OUTPUT_TOWER_LAMP_RED				=	STEP_OUT_BITMASK_USEROUT0,
	OUTPUT_TOWER_LAMP_YELLOW			=	STEP_OUT_BITMASK_USEROUT1,
	OUTPUT_TOWER_LAMP_GREEN				=	STEP_OUT_BITMASK_USEROUT2,
	OUTPUT_TOWER_LAMP_BUZZER			=	STEP_OUT_BITMASK_USEROUT3,
	OUTPUT_SLVE0_SPARE_4				=	STEP_OUT_BITMASK_USEROUT4,
	OUTPUT_SLVE0_SPARE_5				=	STEP_OUT_BITMASK_USEROUT5,
	OUTPUT_SLVE0_SPARE_6				=	STEP_OUT_BITMASK_USEROUT6,
	OUTPUT_SLVE0_SPARE_7				=	STEP_OUT_BITMASK_USEROUT7,
	OUTPUT_SLVE0_SPARE_8				=	STEP_OUT_BITMASK_USEROUT8,
};

#define	MAX_OUTPUT_NUM					4
#define MAX_LAMP_NUM					3

#define OUTPUT_LAM_RED					0
#define OUTPUT_LAM_YELLOW				1
#define OUTPUT_LAM_GREEN				2
#define OUTPUT_LAM_BUZZER				3

typedef struct VerifyParam
{
	CPoint nDeviceIdx;
	int nGrabIdx;
	BOOL isNg;
}typedefVerifyData;

typedef struct DataParam
{
	BOOL DataGrabImageSave;
	BOOL DataOKImageSave;
	BOOL DataNGImageSave;
	BOOL DataInspRsltImageSave;
	BOOL m_bDataLanguage;

	int DataDeleteDay;
	int DataDeleteHour;
	int DataDeleteMinute;
	char m_cPassword[MAX_PATH];
}typedefDataPara;

typedef struct ModelParam
{
	int nIdx;
	char cName[MAX_STR_NUM];
	char cPath[MAX_STR_NUM];
	char cCreatDate[MAX_STR_NUM];
}typedefModelPara;

typedef struct MovePosParam
{
	long MovePos_X[MAX_MOVE_CNT][MAX_GRAB_CNT];
	long MovePos_Y[MAX_MOVE_CNT][MAX_GRAB_CNT];
	long MovePos_Z[MAX_MOVE_CNT][MAX_GRAB_CNT];
}typedefMovePosPara;

typedef struct AlignParam
{
	BOOL bAlign;
	BOOL bCreatMdID[MAX_GRAB_CNT][MAX_INSP_GRAB_CNT][TRACKER_MAX_NUM];
	MIL_ID MilSearchContext[MAX_GRAB_CNT][MAX_INSP_GRAB_CNT][TRACKER_MAX_NUM];
	double dAlignScore;
	double dX[TRACKER_MAX_NUM];
	double dY[TRACKER_MAX_NUM];
	double dAng[TRACKER_MAX_NUM];

	double dCentX[TRACKER_MAX_NUM];
	double dCentY[TRACKER_MAX_NUM];
}typeAlignPara;

typedef struct TeachParam
{
	CdRect TeachRegion[MAX_GRAB_CNT][MAX_INSP_GRAB_CNT][TRACKER_MAX_NUM];
	int TeachRegionCnt[MAX_GRAB_CNT][MAX_INSP_GRAB_CNT];
	int TeachGrabCnt;
	int TeachOneGrabInspCnt;
	int TeachInspGrabCnt;
	int TeachExpTime;
	int TeachLight[MAX_INSP_GRAB_CNT][MAX_LIGHT];

	double TeachGrabMotionDistanceX;
	double TeachGrabMotionDistanceY;
	long TeachArrayCntX;
	long TeachArrayCntY;
	long TeachDeviceCntX;
	long TeachDeviceCntY;
	double TeachDeviceMotionDistanceX;
	double TeachDeviceMotionDistanceY;
	double TeachFirstDevicePos[MAX_MOTION_CNT];
	double TeachWaitPos[MAX_MOTION_CNT];
	int TeachThreshHold[MAX_INSP_GRAB_CNT];
	int TeachInspThreshHold[MAX_INSP_GRAB_CNT][MAX_THRESH_NUM + 1];
	int TeachSpec;
	int TeachShortSpec;
	int TeachPixelSize;
	double TeachMaskSize[MAX_INSP_GRAB_CNT];

	//20161026 ngh
	int TeachInspType;
	BOOL TeachNewInsp;

	//20161017 ngh InspectionTest
#ifdef INSPECTION_TEST
	double TeachCircleArea[MAX_INSP_GRAB_CNT];
	int TeachCircleErodeDistance[MAX_INSP_GRAB_CNT];
#endif

	//20161202 nht
	double TeachSecondInspThreshHold[MAX_INSP_GRAB_CNT];

	int TeachAlignScore;

	typeAlignPara AlignPara;

	int TeachRsltOffSet;
}typeTeachPara;

typedef struct ManualParam
{
	int ManualComport;
	DWORD ManualBaudrate;
	int ManualSlaveNo[MAX_MOTION_CNT];
	double ManualPulseToMM[MAX_MOTION_CNT];

	long ManualSpeed[MAX_MOTION_CNT];
	BOOL ManualServoOrigin[MAX_MOTION_CNT];
}typeManualPara;

typedef struct RsltWidthPoint
{
	double dStartX;
	double dEndX;
}typeRsltWidthPoint;

typedef struct RsltHeightPoint
{
	double dStartY;
	double dEndY;
}typeRsltHeightPoint;

typedef struct RsltPoint
{
	typeRsltWidthPoint RsltWidthPoint;
	typeRsltHeightPoint RsltHeightPoint;
	double dWidth;
	double dHeight;
	double dCentX;
	double dCentY;
	double dOffsetY;
}typeRsltPoint;

typedef struct InsRsltParam
{
	double dCentX[MAX_RSLT_NUM];
	double dCentY[MAX_RSLT_NUM];
	double dLong[MAX_RSLT_NUM];
	double dShort[MAX_RSLT_NUM];
	double dWidth[MAX_RSLT_NUM];
	double dHeight[MAX_RSLT_NUM];
	double dArea[MAX_RSLT_NUM];
	double dAng[MAX_RSLT_NUM];//20161017 ngh
	double dBreadth[MAX_RSLT_NUM];
	BOOL bNg[MAX_RSLT_NUM];
	BOOL bAlign[MAX_RSLT_NUM];

	CPoint nDeviceIdx[MAX_RSLT_NUM];
	int nGrabIdx[MAX_RSLT_NUM];
	int nInspGrabIdx[MAX_RSLT_NUM];
	int nNgCnt;
	BOOL bAddNg[MAX_RSLT_NUM];
}typeInsRsltPara;

typedef struct InsRsltCntParam
{
	int nTotalCnt;
	int nOk1Cnt;
	int nNgCnt;
	int nNgRat;
}typeInsCntPara;

typedef struct {
	BOOL bUse;
	short nDevNo;
} DIO_CFG;

typedef struct {
	int nBaudRate;
}BAUD_RATE;

enum DIO {
	DI_START = 0,									//INPUT START
	Di_START_BUTTON_1 = DI_START,					// DI_X000
	Di_START_BUTTON_2,								// DI_X001
	Di_PAUSE_BUTTON,								// DO_Y005
	DI_SAFETY_LIGHT_CURTAIN_DETECT,					// DO_Y006
	DI_X004,										// DI_X004
	DI_X005,										// DI_X005
	DI_X006,										// DI_X006
	DI_X007,										// DI_X007
	DI_X008,										// DI_X008
	DI_X009,										// DI_X009
	DI_X00A,										// DI_X00A 
	DI_X00B,										// DI_X00B
	DI_X00C,										// DI_X00C
	DI_X00D,										// DI_X00D
	DI_X00E,										// DI_X00E
	DI_X00F,										// DI_X00F
	DI_X010,										// DI_X010
	DI_X011,										// DI_X011
	DI_X012,								       	// DI_X012
	DI_X013,										// DI_X013
	DI_X014,										// DI_X014
	DI_X015,							      		// DI_X015
	DI_X016,							  		    // DI_X016  
	DI_X017,										// DI_X017  
	DI_X018,								        // DI_X018
	DI_X019,					   					// DI_X019
	DI_X01A,										// DI_X01A
	DI_X01B,										// DI_X01B
	DI_X01C,										// DI_X01C
	DI_X01D,										// DI_X01D
	DI_X01E,										// DI_X01E
	DI_X01F,										// DI_X01F

	DI_END = DI_X01F,
	DI_TOTAL = DI_END - DI_START,

	DO_START = DI_END+1,							//OUTPUT START
	DO_TOWER_LAMP_RED,								// DO_Y000
	DO_TOWER_LAMP_YELLOW,							// DO_Y001
	DO_TOWER_LAMP_GREEN,							// DO_Y002
	DO_TOWER_LAMP_BUZZER,							// DO_Y003
	DO_Y004,								   		// DO_Y004
	DO_Y005,										// DO_Y005
	DO_Y006,										// DO_Y006
	DO_Y007,										// DO_Y007
	DO_Y008,										// DO_Y008
	DO_Y009,										// DO_Y009
	DO_Y00A,    									// DO_Y00A
	DO_Y00B,										// DO_Y00B
	DO_Y00C,										// DO_Y00C
	DO_Y00D,										// DO_Y00D
	DO_Y00E,										// DO_Y00E
	DO_Y00F,										// DO_Y00F
	DO_Y010,										// DO_Y010
	DO_Y011,										// DO_Y011
	DO_Y012,										// DO_Y012
	DO_Y013,										// DO_Y013
	DO_Y014,									 	// DO_Y014
	DO_Y015,										// DO_Y015
	DO_Y016,										// DO_Y016
	DO_Y017,										// DO_Y017
	DO_Y018,										// DO_Y018
	DO_Y019,										// DO_Y019
	DO_Y01A,										// DO_Y01A
	DO_Y01B,										// DO_Y01B
	DO_Y01C,										// DO_Y01C
	DO_Y01D,					  					// DO_Y01D 
	DO_Y01E,										// DO_Y01E
	DO_Y01F,										// DO_Y01F

	DO_END = DO_Y01F,
	DO_TOTAL = DO_END - DO_START,
};

#include "mmsystem.h"
#pragma comment(lib, "winmm")

#define MOT_PROCESS_MAX_NUM					1
#define MOT_SEQ_MAX_NUM						4
#define MOT_STATUS_MAX_NUM					MOT_SEQ_MAX_NUM
#define MOT_STATUS_SUB_MAX_NUM				2000

//Motion process status parameter
typedef struct MotionProcessStatus
{
	//I/O status
	int m_bIOIn;

	//Timeout
	double m_dTimeoutS;//시작 시간
	double m_dTimeoutE;//종료 시간(현재)
	double m_dTimeout;//지정된 시간
	
	//Status문자(에러 또는 정상종료시 출력할 문자열)
	char m_cNGStatus[DAT_STR_LENG];
	int m_iNGCode;
	int m_iNGCategory;
	int m_iNGLevel;

	char m_cGoodStatus[DAT_STR_LENG];
	char m_iTypeStatus[DAT_STR_LENG];
	
	int m_iNGSeqIdx;

}typeMotionProcessSts;

typedef struct MotionSeq
{
	//Sequence
	int m_iSubSeqIdx;
	int m_iDispSubSeqIdx;
	int m_iNextSubSeqIdx;

	int m_iSubRoutineIdx;

	//Flag
	BOOL m_bAutoStart;
	BOOL m_bHomeOK;
	BOOL m_bHomeStart;
	BOOL m_bEmerAlarm;
	BOOL m_bLotStart;//0 : 정상 상태, 1 : 남은상태(정상상태가 아니면 남은상태에 따라서 비젼 검사를 하지않는다)
	BOOL m_bLotEnd;
	BOOL m_bAlarm;


	//버튼중심의 동작상태
	BOOL m_bOpSts;

	//정보
	int m_iDeviceIdx;


}typeMotionSeq;

//Motion flag
typedef struct MotionFlag
{
	BOOL m_bProcessThread;
	
	BOOL m_bAutoStart;
	BOOL m_bMainHomeStart;

	BOOL m_bFirstStart;
	BOOL m_bReset;

	BOOL m_bGlassExist;
}typeMotionFlag;

typedef struct ProcessResult
{
	BOOL m_bInspectionStart;
	int m_nInspType;
	int m_nMaxDeviceCntX;
	int m_nMaxDeviceCntY;
	int m_nMaxGrabCnt;
	int m_nDeviceIdxX;
	int m_nDeviceIdxY;
	int m_nOneGrabInspCnt;
	int m_nLightVal[MAX_INSP_GRAB_CNT][MAX_LIGHT];
	int m_nGrabIdx;
	int m_nInspGrabIdx;
	int m_nMaxInspGrabCnt;
	BOOL m_bGrabDirection;
	BOOL m_bInspectionRslt[MAX_INSP_GRAB_CNT][MAX_INSP_GRAB_CNT];
	BOOL m_bInspRslt[MAX_INSP_GRAB_CNT][MAX_INSP_GRAB_CNT];
	BOOL m_bAlignInsp[MAX_INSP_GRAB_CNT];
	BOOL m_bInspComp;
	BOOL m_bInspectionComp;
	BOOL m_bGrabComp;
	int m_nImageSel;
	//Motion
	long m_lMotPos[MAX_MOTION_CNT];
	long m_lDefaultPos[MAX_MOTION_CNT];
	char m_cStartTime[DAT_STR_LENG];
	char m_cStartDate[DAT_STR_LENG];
}typeProcessResult;