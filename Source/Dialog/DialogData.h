#pragma once
#include "TypeDef.h"
#include "DialogModel.h"
#include "DialogChangePassword.h"


// CDialogData 대화 상자입니다.

class CDialogData : public CDialogEx
{
	DECLARE_DYNAMIC(CDialogData)

public:
	CDialogData(CWnd* pParent = NULL);   // 표준 생성자입니다.
	virtual ~CDialogData();

// 대화 상자 데이터입니다.
	enum { IDD = IDD_DIALOG_DATA };

public:
	typedefModelPara *GetAllModel();
	typedefModelPara GetSelectModel();
	BOOL GetNonModel();

	typedefDataPara GetDataPara();

	BOOL SaveDataPara(CString path);
	void ChangeLanguage(BOOL StateLanguage);	//20161011 ht 언어 설정 함수
	BOOL GetCurrentLanguage() { return m_DataPara.m_bDataLanguage;}		//20161011 ht 메인다이얼로그에서 어떤 언어를 사용할지 알려주는 함수
protected:
	CDialogModel *m_dlgModel;
	

	typedefModelPara m_SelectModel;
	typedefDataPara m_DataPara;

	void DeleteModelPath();
	BOOL LoadDataPara(CString path);

	void Update(BOOL isUpdate);

	BOOL WriteLog(CString strLog);

	CBtnEnh m_ctrlLanguageEng;
	CBtnEnh m_ctrlLanguageKor;

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	afx_msg void OnShowWindow(BOOL bShow, UINT nStatus);
	DECLARE_EVENTSINK_MAP()
	void ClickBtnenhctrlApply();
	void ClickBtnenhctrlOk();
	void ClickBtnenhctrlCancel();
	int m_edit_nDeleteDay;
	int m_edit_nDeleteHour;
	int m_edit_nDeleteMinute;
	CBtnEnh m_btn_OkImageSave;
	CBtnEnh m_btn_NgImageSave;
	CBtnEnh m_btn_GrabImageSave;
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	void ClickBtnenhctrlLangEnglish();
	void ClickBtnenhctrlLangKorean();
	void ClickBtnChangepw();
	CBtnEnh m_btn_InspRsltImgSave;
	void ClickBtnenhctrlCheck3();
};
