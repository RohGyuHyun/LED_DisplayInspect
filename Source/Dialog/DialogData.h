#pragma once
#include "TypeDef.h"
#include "DialogModel.h"
#include "DialogChangePassword.h"


// CDialogData ��ȭ �����Դϴ�.

class CDialogData : public CDialogEx
{
	DECLARE_DYNAMIC(CDialogData)

public:
	CDialogData(CWnd* pParent = NULL);   // ǥ�� �������Դϴ�.
	virtual ~CDialogData();

// ��ȭ ���� �������Դϴ�.
	enum { IDD = IDD_DIALOG_DATA };

public:
	typedefModelPara *GetAllModel();
	typedefModelPara GetSelectModel();
	BOOL GetNonModel();

	typedefDataPara GetDataPara();

	BOOL SaveDataPara(CString path);
	void ChangeLanguage(BOOL StateLanguage);	//20161011 ht ��� ���� �Լ�
	BOOL GetCurrentLanguage() { return m_DataPara.m_bDataLanguage;}		//20161011 ht ���δ��̾�α׿��� � �� ������� �˷��ִ� �Լ�
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
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV �����Դϴ�.

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
