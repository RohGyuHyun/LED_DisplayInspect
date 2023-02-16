#pragma once
#include "afxcmn.h"
#include "TypeDef.h"


// CDialogModel 대화 상자입니다.

class CDialogModel : public CDialogEx
{
	DECLARE_DYNAMIC(CDialogModel)

public:
	CDialogModel(CWnd* pParent = NULL);   // 표준 생성자입니다.
	virtual ~CDialogModel();

// 대화 상자 데이터입니다.
	enum { IDD = IDD_DIALOG_MODEL };

public:
	typedefModelPara GetSelectModel();
	typedefModelPara GetDeleteModel(int delete_cnt);
	typedefModelPara GetSrcCopyModel();
	typedefModelPara GetDesCopyModel();
	typedefModelPara *GetAllModel();

	BOOL SetSaveModelLable();
	BOOL SetSaveSelectModelNo();
	BOOL SetCopyModel();
	int GetDeleteModelCnt();
	BOOL GetNonModel();
	BOOL GetCopyModel();
	void ChangeLanguage(BOOL StateLanguage);	//20161011 ht Model다이얼로그 언어 설정
protected:
	void InitList();
	void AddList();
	typedefModelPara m_AllModel[MAX_MODEL_NUM];
	typedefModelPara m_SelectModel;

	BOOL LoadModelLable();
	BOOL SaveModelLable();
	BOOL LoadSelectModelNo();
	BOOL SaveSelectModelNo();

	BOOL NewModel();
	BOOL SelectModel();
	BOOL DeleteModel();
	BOOL CopyModel();

	BOOL GetOpenList(int idx);

	int m_nDeleteCnt;
	typedefModelPara m_DeleteModel[MAX_MODEL_NUM];
	typedefModelPara m_CopySrcModel;
	typedefModelPara m_CopyDesModel;
	typedefModelPara m_DesSelectModel;

	BOOL m_bNonModel;

	BOOL m_bCopyModel;

	int m_nNewModelNum;
	BOOL m_bNewModel;

	int m_nLClickIdx;
	int m_nRClickIdx;
	int m_nDBClickIdx;
	BOOL m_bSelModel;

	void ListUpdate();

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()
public:
	CListCtrl m_list_Model;
	afx_msg void OnNMDblclkListModel(NMHDR *pNMHDR, LRESULT *pResult);
	virtual BOOL OnInitDialog();
	DECLARE_EVENTSINK_MAP()
	void ClickBtnenhctrlSelect();
	void ClickBtnenhctrlDelete();
	void ClickBtnenhctrlNew();
	afx_msg void OnShowWindow(BOOL bShow, UINT nStatus);
	CString m_edit_SelectModelName;
	CString m_edit_NewModelName;
	CString m_edit_strSrcModelName;
	CString m_edit_strDesModelName;
	void ClickBtnenhctrlCopy();
	afx_msg void OnNMClickListModel(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnNMRClickListModel(NMHDR *pNMHDR, LRESULT *pResult);
	CString m_edit_strSrcModelIdx;
	CString m_edit_strDesModelIdx;
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	afx_msg void OnNMRDblclkListModel(NMHDR *pNMHDR, LRESULT *pResult);
	CString m_edit_SelectModelIdx;
	CString m_edit_NewModelIdx;
};
