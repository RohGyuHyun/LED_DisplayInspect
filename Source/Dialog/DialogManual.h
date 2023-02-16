#pragma once
#include "afxcmn.h"
#include "DialogJog.h"
#include "afxwin.h"
// CDialogManual 대화 상자입니다.

class CDialogManual : public CDialogEx
{
	DECLARE_DYNAMIC(CDialogManual)

public:
	CDialogManual(CWnd* pParent = NULL);   // 표준 생성자입니다.
	virtual ~CDialogManual();

// 대화 상자 데이터입니다.
	enum { IDD = IDD_DIALOG_MANUAL };

public:
	BOOL SaveManualPara(CString path);
	long GetEZCurPos(int nMotionNum);
	BOOL GetEzPosComplete(int nMotionNum, long nPos);
	BOOL SetEzSpeed(int nMotionNum, int nSpeed);

	int GetServoAmp(int nMotionNum);
	BOOL GetAmpFaultLevel(int nMotionNum, long& level);
	BOOL GetLimtSenser(int nMotionNum, BOOL isDirection);
	BOOL GetHomeSenser(int nMotionNum);

	BOOL SetJogMove(int nMotionNum, BOOL isDirection, long speed, CRect rect, long x, long y);
	BOOL SetStepMove(int nMotionNum, double nStep, long speed);
	BOOL SetEzPosMove(int nMotionNum, int nPos);
	BOOL SetMoveStop(int nMotionNum);

	BOOL GetInput(int nMotionNum, int nInputIdx);
	BOOL GetOutput(int nMotionNum, int nOutputIdx);

	BOOL WriteOutput(int nMotionNum, int nOutputIdx, BOOL isOn);
	

	typeManualPara GetManualPara();

	BOOL SetMotorOrigin(BOOL isAll = TRUE, int nMotionNum = 0);

	BOOL SetMotorAmp(BOOL isAll = TRUE, int nMotionNum = 0, BOOL isAmp = TRUE);
	BOOL SetMotorAlarmReset(BOOL isAll = TRUE, int nMotionNum = 0);

	void SetAllHomeFlag(BOOL isFlag);
	
	void ChangeLanguage(BOOL StateLanguage);	//20161011 ht Data다이얼로그 언어 설정
	BOOL InitMotion(int nMotionNUm);
private:
	long m_nMovePos[MAX_MOTION_CNT];
	char m_cDILabel[DI_MAX_NUM][100];
	char m_cDOLabel[DO_MAX_NUM][100];
	CString m_strDILabel[DI_MAX_NUM];
	CString m_strDOLabel[DO_MAX_NUM];
	BOOL m_bDI[DI_MAX_NUM];
	BOOL m_bDO[DO_MAX_NUM];
	BOOL m_bDI_list[DI_MAX_NUM];
	BOOL m_bDO_list[DO_MAX_NUM];
	CFont m_font;
	
	BOOL InitIO();
	BOOL LoadDIOLabel();
	void ReadDisplayInputStatus(BOOL disp);
	void ReadDisplayOutputStatus(BOOL disp);

	
	void CloseMotion();
	BOOL LoadManualPara(CString path);

	long m_lCurPos[MAX_MOTION_CNT];

	BOOL MotionOrigin(int nMotionNum);
	BOOL MotionAmp(int nMotionNum, BOOL isAmp);
	BOOL MotionAlarmReset(int nMotionNum);

	BOOL SetOutput(int nMotionNum, DWORD nOutputIdx, BOOL isOn);

	CDialogJog *m_dlgJog;
	BOOL GetSWLimtPosRslt(int nMotionNum, int nMovePos);

	BOOL m_bAmpState;
protected:
	typeManualPara m_ManualPara;
	CCriticalSection m_AxisSyncIntlock;
	BOOL CheckInputStatus(int nMotionNum, DWORD dwAdd);
	BOOL CheckOutputStatus(int nMotionNum,DWORD dwAdd);
	void MotionStateUpdate(int nMotionNum, int* isState);

	DWORD m_InputAddr[10];
	DWORD m_OutputAddr[10];
	BOOL m_bStepToJog;

	void Update(BOOL isUpdate);
	BOOL WriteLog(CString strLog);

	void InitCombo();

	void SetComboComPort(int nIdx);
	void SetComboBaudrate(int nIdx);

	int GetIdxBaudrate(int nIdx);

	CWinThread *m_pAmpStateThread;
	static UINT AmpStateThread(LPVOID pParam);
	void AmpState();
	BOOL m_bAmpStateStart;
	BOOL m_bAmpStateEnd;

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()
public:
	CListCtrl m_list_input;
	CListCtrl m_list_output;
	virtual BOOL OnInitDialog();
	afx_msg void OnShowWindow(BOOL bShow, UINT nStatus);
	DECLARE_EVENTSINK_MAP()
	void ClickBtnenhctrlApply();
	void ClickBtnenhctrlOk();
	void ClickBtnenhctrlCancel();
	afx_msg void OnNMDblclkListIoOutput(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnTimer(UINT_PTR nIDEvent);

	CBtnEnh m_btn_MotionX_CurPos;
	CBtnEnh m_btn_MotionX_P_Limt;
	CBtnEnh m_btn_MotionX_N_Limt;
	CBtnEnh m_btn_MotionX_Home;
	CBtnEnh m_btn_MotionX_Amp;

	CBtnEnh m_btn_MotionY_CurPos;
	CBtnEnh m_btn_MotionY_P_Limt;
	CBtnEnh m_btn_MotionY_N_Limt;
	CBtnEnh m_btn_MotionY_Home;
	CBtnEnh m_btn_MotionY_Amp;

	CBtnEnh m_btn_MotionZ_CurPos;
	CBtnEnh m_btn_MotionZ_P_Limt;
	CBtnEnh m_btn_MotionZ_N_Limt;
	CBtnEnh m_btn_MotionZ_Home;
	CBtnEnh m_btn_MotionZ_Amp;
	
	CComboBox m_combo_nComportNum_0;
	CComboBox m_combo_nBaudrate_0;
	int m_nComportNum_0;
	int m_nBaudrate_0;
	int m_edit_nSlaveNo_0;
	double m_edit_dPulseToMM_0;
	int m_edit_nSpeed_0;
	afx_msg void OnCbnSelchangeComboComAxis0();
	afx_msg void OnCbnSelchangeComboBaudrateAxis0();

	int m_edit_nSlaveNo_1;
	double m_edit_dPulseToMM_1;
	int m_edit_nSpeed_1;

	int m_edit_nSlaveNo_2;
	double m_edit_dPulseToMM_2;
	int m_edit_nSpeed_2;
	void ClickBtnenhctrlServoOrigin0();
	void ClickBtnenhctrlServoOrigin1();
	void ClickBtnenhctrlServoOrigin2();
	void ClickBtnenhctrlServoAmp0();
	void ClickBtnenhctrlServoAmp1();
	void ClickBtnenhctrlServoAmp2();
	void ClickBtnenhctrlMotorStop();
	void ClickBtnenhctrlMotorAlarmReset();
	CBtnEnh m_btn_MotionConnect;
	void ClickBtnenhctrlMotionConnect();
	CBtnEnh m_btn_MotionX_OriginState;
	CBtnEnh m_btn_MotionY_OriginState;
	CBtnEnh m_btn_MotionZ_OriginState;
	virtual BOOL PreTranslateMessage(MSG* pMsg);
};
