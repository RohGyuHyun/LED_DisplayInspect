#pragma once
#include "afxcmn.h"
#include "serialPort.h"
#include "DialogJog.h"
#include "DialogParameterLock.h"
#include "afxwin.h"


// DialogTeach 대화 상자입니다.

class CDialogTeach : public CDialogEx
{
	DECLARE_DYNAMIC(CDialogTeach)

public:
	CDialogTeach(CWnd* pParent = NULL);   // 표준 생성자입니다.
	virtual ~CDialogTeach();

// 대화 상자 데이터입니다.
	enum { IDD = IDD_DIALOG_TEACH };

public:
	typeTeachPara GetTeachPara();
	void SetTeachPara(typeTeachPara teachPara);
	BOOL LoadTeachPara(CString path);
	void Update(BOOL isUpdate);
	BOOL SaveTeachPara(CString path);

	void OnOffLight(BOOL LightOn, int grab_idx);
	void SetLight(int ch, int var, int grab_idx);
	BYTE byValue2Hexa_High(BYTE byData);
	BYTE byValue2Hexa_Low(BYTE byData);
	BYTE byValue2NibbleAscii(BYTE byData);

	int GetImageGrabCnt();

	void SetSavePath(CString path);

	long GetTeachMovePos(int nMotionNum, int nDeviceIdxX, int nDeviceIdxY, int nGrabIdx);
	long GetWaitMovePos(int nMotionNum);

	int GetDeviceIdxX();
	int GetDeviceIdxY();
	int GetGrabIdx();
	int GetInspGrabIdx();

	void SetParameterEnable(BOOL isLock);
	void ChangeLanguage(BOOL StateLanguage);	//20161011 ht Teach다이얼로그 언어 설정

	BOOL GetEdgeInsp();
	double GetAlignSize(int ins_grab_idx);//20161122 NHT Mask사이즈 조절

	int m_iBeforeidxX, m_iBeforeidxY, m_iBeforeidxX1,m_iBeforeidxY1;

	void SetApply();
private:

	int m_nBeforeX;
	int m_nBeforeY;
	int m_nBdforeImgIdx;
	
	BOOL m_bEdgeInsp;
	BOOL m_bBlobInsp;
	
	typeTeachPara m_TeachPara;
	CSerialPort m_cSerial;
	CDialogJog *m_dlgJog;
	

	typedefMovePosPara m_TeachMovePos;
	void CalTeachMovePos();

	CString m_SavePath;

	BOOL m_bParaLock;

protected:
	BOOL WriteLog(CString strLog);

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	DECLARE_EVENTSINK_MAP()
	void ClickBtnenhctrlApply();
	void ClickBtnenhctrlOk();
	void ClickBtnenhctrlCancel();
	afx_msg void OnShowWindow(BOOL bShow, UINT nStatus);
	int m_edit_nInspRegionCnt;
	CSliderCtrl m_slider_Light_0;
	CSliderCtrl m_slider_Light_1;
	afx_msg void OnNMCustomdrawSliderLight0(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnNMCustomdrawSliderLight1(NMHDR *pNMHDR, LRESULT *pResult);
	int m_edit_nLight_0;
	int m_edit_nLight_1;
	int m_edit_nImageGrabCnt;
	int m_edit_nSelectImageIdx;
	int m_edit_nMoveCntX;
	int m_edit_nMoveCntY;
	int m_edit_nDeviceCntX;
	int m_edit_nDeviceCntY;
	double m_edit_dGrabDistanceX;
	double m_edit_dGrabDistanceY;
	double m_edit_dFirstDevicePosX;
	double m_edit_dFirstDevicePosY;
	double m_edit_dFirstDevicePosZ;
	double m_edit_nDeviceDistanceX;
	double m_edit_nDeviceDistanceY;
	void ClickBtnenhctrlSelectImageIdxLeft();
	void ClickBtnenhctrlSelectImageIdxRight();
	void ClickBtnenhctrlCurentPosSet();
	void ClickBtnenhctrlPosMove();
	void ClickBtnenhctrlLightSet1();
	void ClickBtnenhctrlLightSet2();
	void ClickBtnenhctrlPosMove2();
	void ClickBtnenhctrlSelectImageIdxLeft2();
	void ClickBtnenhctrlSelectImageIdxRight2();
	void ClickBtnenhctrlSelectImageIdxLeft3();
	void ClickBtnenhctrlSelectImageIdxRight3();
	void ClickBtnenhctrlCurentWaitPosSet();
	void ClickBtnenhctrlWaitPosMove();
	double m_edit_dWaitPosX;
	double m_edit_dWaitPosY;
	double m_edit_dWaitPosZ;
	int m_edit_nThreshHold1;
	int m_edit_nInspThreshHold3;
	int m_edit_nSpec;
	int m_edit_nPixelSize;
	int m_edit_nAlignSize; //20161122 NHT Mask사이즈 조절
	CBtnEnh m_btn_para_lock;
	void ClickBtnenhctrlParaLock();
	CEdit m_edit_ctrl_para3;
	CEdit m_edit_ctrl_para0;
	CEdit m_edit_ctrl_para6;
	CEdit m_edit_ctrl_para7;
	CEdit m_edit_ctrl_para8;
	CEdit m_edit_ctrl_para9;
	CEdit m_edit_ctrl_para14;
	CEdit m_edit_ctrl_para15;
	CEdit m_edit_ctrl_para17;
	CEdit m_edit_ctrl_para18;
	CEdit m_edit_ctrl_para19;
	CEdit m_edit_ctrl_para11;
	CEdit m_edit_ctrl_para12;
	CEdit m_edit_ctrl_para13;
	CEdit m_edit_ctrl_para20;
	CEdit m_edit_ctrl_para21;
	CEdit m_edit_ctrl_para22;
	void ClickBtnenhctrlInspTeach();
	int m_edit_nInspThreshHold1;
	int m_edit_nInspThreshHold2;
	int m_edit_nInspGrabIdx;
	int m_edit_nSelectInspGrabIdx;
	void ClickBtnenhctrlSelectImageIdxLeft4();
	void ClickBtnenhctrlSelectImageIdxRight4();
	void ClickBtnenhctrlInspAlignTeach();
	void ClickBtnenhctrlInspAlignTest();
	void ClickBtnenhctrlGrabDistance();
	void ClickBtnenhctrlDeviceDistance();
	int m_edit_nOneGrabInspCnt;
	void ClickBtnenhctrlInspTeach2();
	void ClickBtnenhctrlRegionCopy();
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	double m_edit_dCircleArea;
	int m_edit_dCircleErodeDistance;
	void ClickTeachEdgeInsp();
	void ClickTeachBlobInsp();
	CBtnEnh m_btn_InspEdge;
	CBtnEnh m_btn_InspBlob;
	int m_edit_nAlignScore;
	CBtnEnh m_btn_NewInspection;


	double m_edit_nSecondInspThreshHold;
	CEdit m_edit_ctrl_para32;
	int m_edit_nShortSpec;
	CEdit m_edit_ctrl_para28;
	CEdit m_edit_ctrl_para26;
	CEdit m_edit_ctrl_para24;
	CEdit m_edit_ctrl_para25;
	CEdit m_edit_ctrl_para29;
	CEdit m_edit_ctrl_para30;
	CEdit m_edit_ctrl_para31;
	CEdit m_edit_ctrl_para23;
	CEdit m_edit_ctrl_para00;
};
