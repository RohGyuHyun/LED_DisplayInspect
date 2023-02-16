// D:\Project\디지티\LED_DisplayInspect\Source\Dialog\DialogData.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "LED_DisplayInspect.h"
#include "LED_DisplayInspectDlg.h"
#include "DialogData.h"
#include "afxdialogex.h"

// CDialogData 대화 상자입니다.
//폴더 삭제
BOOL DeleteDirectorie(CString strFolder)
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
			if(ff.IsDirectory())
			{
				DeleteDirectorie(Filename);
				RemoveDirectory(Filename);
			}
			else
			{
				DeleteFile(Filename);
			}
		}
	}
	ff.Close();

	return TRUE;
}

IMPLEMENT_DYNAMIC(CDialogData, CDialogEx)

	CDialogData::CDialogData(CWnd* pParent /*=NULL*/)
	: CDialogEx(CDialogData::IDD, pParent)
	, m_edit_nDeleteDay(0)
	, m_edit_nDeleteHour(0)
	, m_edit_nDeleteMinute(0)
{
	memset(&m_SelectModel, NULL, sizeof(typedefModelPara));
	memset(&m_DataPara, NULL, sizeof(typedefDataPara));

	m_DataPara.DataDeleteDay = 1;
}

CDialogData::~CDialogData()
{
	delete m_dlgModel;
	SaveDataPara(DB_PATH);
}

void CDialogData::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT_PARA_0, m_edit_nDeleteDay);
	DDX_Text(pDX, IDC_EDIT_PARA_1, m_edit_nDeleteHour);
	DDX_Text(pDX, IDC_EDIT_PARA_16, m_edit_nDeleteMinute);
	DDX_Control(pDX, IDC_BTNENHCTRL_CHECK_0, m_btn_OkImageSave);
	DDX_Control(pDX, IDC_BTNENHCTRL_CHECK_1, m_btn_NgImageSave);
	DDX_Control(pDX, IDC_BTNENHCTRL_CHECK_2, m_btn_GrabImageSave);
	DDX_Control(pDX, IDC_BTNENHCTRL_LANG_ENGLISH, m_ctrlLanguageEng);
	DDX_Control(pDX, IDC_BTNENHCTRL_LANG_KOREAN, m_ctrlLanguageKor);
	DDX_Control(pDX, IDC_BTNENHCTRL_CHECK_3, m_btn_InspRsltImgSave);
}


BEGIN_MESSAGE_MAP(CDialogData, CDialogEx)
	ON_WM_SHOWWINDOW()
END_MESSAGE_MAP()


// CDialogData 메시지 처리기입니다.


BOOL CDialogData::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  여기에 추가 초기화 작업을 추가합니다.
	CLED_DisplayInspectDlg *pdlg = ((CLED_DisplayInspectDlg *)AfxGetApp()->m_pMainWnd);
	CRect rect,sys_rect;

	CString strLog;
	strLog.Format(_T("CDialogData::OnInitDialog() Start!!"));
	WriteLog(strLog);

	m_dlgModel = new CDialogModel();	
	m_dlgModel->Create(IDD_DIALOG_MODEL, this);
	GetDlgItem(IDC_STATIC_MODEL)->GetWindowRect(rect);
	m_dlgModel->MoveWindow(rect.left, rect.top +68, rect.Width(), rect.Height(), TRUE);



	LoadDataPara(DB_PATH);
	Update(FALSE);

	strLog.Format(_T("CDialogData::OnInitDialog() End!!"));
	WriteLog(strLog);

	return TRUE;  // return TRUE unless you set the focus to a control
	// 예외: OCX 속성 페이지는 FALSE를 반환해야 합니다.
}


void CDialogData::OnShowWindow(BOOL bShow, UINT nStatus)
{
	CDialogEx::OnShowWindow(bShow, nStatus);

	if(bShow)
	{
		m_dlgModel->ShowWindow(SW_SHOW);
		LoadDataPara(DB_PATH);
		Update(FALSE);
		UpdateData(FALSE);

	}
	else
	{
		m_dlgModel->ShowWindow(SW_HIDE);
	}

	// TODO: 여기에 메시지 처리기 코드를 추가합니다.
}
BEGIN_EVENTSINK_MAP(CDialogData, CDialogEx)
	ON_EVENT(CDialogData, IDC_BTNENHCTRL_APPLY, DISPID_CLICK, CDialogData::ClickBtnenhctrlApply, VTS_NONE)
	ON_EVENT(CDialogData, IDC_BTNENHCTRL_OK, DISPID_CLICK, CDialogData::ClickBtnenhctrlOk, VTS_NONE)
	ON_EVENT(CDialogData, IDC_BTNENHCTRL_CANCEL, DISPID_CLICK, CDialogData::ClickBtnenhctrlCancel, VTS_NONE)
	ON_EVENT(CDialogData, IDC_BTNENHCTRL_LANG_ENGLISH, DISPID_CLICK, CDialogData::ClickBtnenhctrlLangEnglish, VTS_NONE)
	ON_EVENT(CDialogData, IDC_BTNENHCTRL_LANG_KOREAN, DISPID_CLICK, CDialogData::ClickBtnenhctrlLangKorean, VTS_NONE)
	ON_EVENT(CDialogData, IDC_BTN_CHANGEPW, DISPID_CLICK, CDialogData::ClickBtnChangepw, VTS_NONE)
	ON_EVENT(CDialogData, IDC_BTNENHCTRL_CHECK_3, DISPID_CLICK, CDialogData::ClickBtnenhctrlCheck3, VTS_NONE)
END_EVENTSINK_MAP()


void CDialogData::ClickBtnenhctrlApply()
{
	// TODO: 여기에 메시지 처리기 코드를 추가합니다.
	CLED_DisplayInspectDlg *pdlg = ((CLED_DisplayInspectDlg *)AfxGetApp()->m_pMainWnd);
	m_dlgModel->SetSaveModelLable();
	m_dlgModel->SetSaveSelectModelNo();
	pdlg->SetSelectModel(m_dlgModel->GetSelectModel());
	DeleteModelPath();

	if(m_dlgModel->GetCopyModel())
	{
		m_dlgModel->SetCopyModel();
	}

	UpdateData(TRUE);
	Update(TRUE);
}

void CDialogData::ClickBtnenhctrlOk()
{
	// TODO: 여기에 메시지 처리기 코드를 추가합니다.
	CLED_DisplayInspectDlg *pdlg = ((CLED_DisplayInspectDlg *)AfxGetApp()->m_pMainWnd);
	m_dlgModel->SetSaveModelLable();
	m_dlgModel->SetSaveSelectModelNo();
	pdlg->SetSelectModel(m_dlgModel->GetSelectModel());
	DeleteModelPath();

	//20161112 nht 모델변경시 이전검사결과 초기화

	int BeforeRsltCntSize = pdlg->GetBeforeRsltCnt();

	for(int i = 0;i < BeforeRsltCntSize;i++)
	{
		pdlg->DeleteRsltFilePath(0);
	}



	if(m_dlgModel->GetCopyModel())
	{
		m_dlgModel->SetCopyModel();
	}

	UpdateData(TRUE);
	Update(TRUE);
	SaveDataPara(DB_PATH);

	//load
	CString path;
	path.Format(_T("%s"), m_dlgModel->GetSelectModel().cPath);
	pdlg->m_dlgTeach->SetSavePath(path);
	pdlg->m_dlgTeach->LoadTeachPara(path);
	pdlg->m_dlgTeach->Update(FALSE);
	pdlg->m_dlgTeach->UpdateData(FALSE);
	pdlg->m_dlgTeach->SetApply();
	//m_dlgImageRect->SetReDraw();
	pdlg->SetGrid();
	pdlg->SetInspectionReset(FALSE);

	//20161011 ht OK버튼 눌렀을 시 언어 변경
	if(m_ctrlLanguageEng.GetValue()!=LANGUAGE_ENG)
	{
		pdlg->ChangeLanguage(LANGUAGE_ENG);
	}
	else if(m_ctrlLanguageEng.GetValue()==LANGUAGE_ENG)
	{
		pdlg->ChangeLanguage(LANGUAGE_KOR);
	}
	ShowWindow(SW_HIDE);
}

void CDialogData::ClickBtnenhctrlCancel()
{
	// TODO: 여기에 메시지 처리기 코드를 추가합니다.
	ShowWindow(SW_HIDE);
}

typedefModelPara CDialogData::GetSelectModel()
{
	return m_dlgModel->GetSelectModel();
}

BOOL CDialogData::GetNonModel()
{
	return m_dlgModel->GetNonModel();
}

void CDialogData::DeleteModelPath()
{
	if(m_dlgModel->GetDeleteModelCnt() > 0)
	{
		CString delete_path;
		for(int i = 0; i < m_dlgModel->GetDeleteModelCnt(); i++)
		{
			delete_path.Format(_T("%s"), m_dlgModel->GetDeleteModel(i).cPath);

			//delete path
			DeleteDirectorie(delete_path);
			RemoveDirectory(delete_path);
		}
	}
}

typedefDataPara CDialogData::GetDataPara()
{
	return m_DataPara;
}

void CDialogData::Update(BOOL isUpdate)
{
	if(isUpdate)
	{
		m_DataPara.DataDeleteDay = m_edit_nDeleteDay;
		m_DataPara.DataDeleteHour = m_edit_nDeleteHour;
		m_DataPara.DataDeleteMinute = m_edit_nDeleteMinute;
		m_DataPara.DataGrabImageSave = m_btn_GrabImageSave.GetValue();
		m_DataPara.DataNGImageSave = m_btn_NgImageSave.GetValue();
		m_DataPara.DataOKImageSave = m_btn_OkImageSave.GetValue();
		m_DataPara.DataInspRsltImageSave = m_btn_InspRsltImgSave.GetValue();
	}
	else
	{
		m_edit_nDeleteDay = m_DataPara.DataDeleteDay;
		m_edit_nDeleteHour = m_DataPara.DataDeleteHour;
		m_edit_nDeleteMinute = m_DataPara.DataDeleteMinute;
		m_btn_GrabImageSave.SetValue(m_DataPara.DataGrabImageSave);
		m_btn_NgImageSave.SetValue(m_DataPara.DataNGImageSave);
		m_btn_OkImageSave.SetValue(m_DataPara.DataOKImageSave);
		m_btn_InspRsltImgSave.SetValue(m_DataPara.DataInspRsltImageSave);
		//20161011 ht 언어 변경 및 현재 언어 표시
		if(m_DataPara.m_bDataLanguage == 1)
		{
			ClickBtnenhctrlLangKorean();
			m_ctrlLanguageEng.SetValue(FALSE);
			m_ctrlLanguageKor.SetValue(TRUE);

		}
		else if(m_DataPara.m_bDataLanguage == 0)
		{
			ClickBtnenhctrlLangEnglish();
			m_ctrlLanguageEng.SetValue(TRUE);
			m_ctrlLanguageKor.SetValue(FALSE);
		}
	}
}

BOOL CDialogData::SaveDataPara(CString path)
{
	BOOL rslt = TRUE;
	CString key, dat;
	CString file_path;

	CString strLog;
	strLog.Format(_T("SaveDataPara Start!!"));
	WriteLog(strLog);

	file_path.Format(_T("%s\\Data.par"), DB_PATH);

	//file 존재 확인 
	HANDLE fd=CreateFile(   file_path,
		GENERIC_READ|GENERIC_WRITE,
		0,
		NULL,
		CREATE_ALWAYS,
		FILE_ATTRIBUTE_NORMAL,
		NULL
		);
	if(((LONG)fd)<0)
	{
		rslt = FALSE;
	}
	else
	{
		CloseHandle(fd);

		key.Empty();
		key.Format(_T("DELETE_DAY"));
		dat.Empty();
		dat.Format(_T("%d"), m_DataPara.DataDeleteDay);
		WritePrivateProfileString(_T("DATA_PARA"), key, dat, file_path);	

		key.Empty();
		key.Format(_T("DELETE_HOUR"));
		dat.Empty();
		dat.Format(_T("%d"), m_DataPara.DataDeleteHour);
		WritePrivateProfileString(_T("DATA_PARA"), key, dat, file_path);

		key.Empty();
		key.Format(_T("DELETE_MINUTE"));
		dat.Empty();
		dat.Format(_T("%d"), m_DataPara.DataDeleteMinute);
		WritePrivateProfileString(_T("DATA_PARA"), key, dat, file_path);

		key.Empty();
		key.Format(_T("GRAB_IMAGE_SAVE"));
		dat.Empty();
		dat.Format(_T("%d"), m_DataPara.DataGrabImageSave);
		WritePrivateProfileString(_T("DATA_PARA"), key, dat, file_path);

		key.Empty();
		key.Format(_T("OK_IMAGE_SAVE"));
		dat.Empty();
		dat.Format(_T("%d"), m_DataPara.DataOKImageSave);
		WritePrivateProfileString(_T("DATA_PARA"), key, dat, file_path);

		key.Empty();
		key.Format(_T("NG_IMAGE_SAVE"));
		dat.Empty();
		dat.Format(_T("%d"), m_DataPara.DataNGImageSave);
		WritePrivateProfileString(_T("DATA_PARA"), key, dat, file_path);

		key.Empty();
		key.Format(_T("INSP_RSLT_IMAGE_SAVE"));
		dat.Empty();
		dat.Format(_T("%d"), m_DataPara.DataInspRsltImageSave);
		WritePrivateProfileString(_T("DATA_PARA"), key, dat, file_path);

		//20161011 ht 언어 init 값 저장
		key.Empty();
		key.Format(_T("LANGUAGE"));
		dat.Empty();
		dat.Format(_T("%d"), m_DataPara.m_bDataLanguage);
		WritePrivateProfileString(_T("DATA_PARA"), key, dat, file_path);

		//20161014 ht 비밀번호 값 저장
		key.Empty();
		key.Format("PASSWORD");
		dat.Empty();
		dat.Format("%s", m_DataPara.m_cPassword);
		WritePrivateProfileString("DATA_PARA", key, dat, file_path);
	}

	fd=0;

	strLog.Format(_T("SaveDataPara Start!!"));
	WriteLog(strLog);

	return rslt;
}

BOOL CDialogData::LoadDataPara(CString path)
{
	BOOL rslt = TRUE;
	CString key, dat;
	char temp[DAT_STR_LENG];
	CString file_path;

	CString strLog;
	strLog.Format(_T("LoadDataPara Start!!"));
	WriteLog(strLog);

	file_path.Format(_T("%s\\Data.par"), DB_PATH);

	//file 존재 확인 
	HANDLE fd = CreateFile( file_path,
		GENERIC_READ|GENERIC_WRITE,
		0,
		NULL,
		OPEN_EXISTING,
		FILE_ATTRIBUTE_NORMAL,
		NULL
		);
	if(((LONG)fd)<0)
	{
		rslt = FALSE;
	}
	else
	{
		CloseHandle(fd);

		key.Empty();
		key.Format(_T("DELETE_DAY"));
		GetPrivateProfileString(_T("DATA_PARA"), key, _T("1"), temp, sizeof(temp), file_path);
		dat.Format(_T("%s"), temp);
		m_DataPara.DataDeleteDay = atoi(dat);

		key.Empty();
		key.Format(_T("DELETE_HOUR"));
		GetPrivateProfileString(_T("DATA_PARA"), key, _T("0"), temp, sizeof(temp), file_path);
		dat.Format(_T("%s"), temp);
		m_DataPara.DataDeleteHour = atoi(dat);

		key.Empty();
		key.Format(_T("DELETE_MINUTE"));
		GetPrivateProfileString(_T("DATA_PARA"), key, _T("0"), temp, sizeof(temp), file_path);
		dat.Format(_T("%s"), temp);
		m_DataPara.DataDeleteMinute = atoi(dat);

		key.Empty();
		key.Format(_T("GRAB_IMAGE_SAVE"));
		GetPrivateProfileString(_T("DATA_PARA"), key, _T("0"), temp, sizeof(temp), file_path);
		dat.Format(_T("%s"), temp);
		m_DataPara.DataGrabImageSave = atoi(dat);

		key.Empty();
		key.Format(_T("OK_IMAGE_SAVE"));
		GetPrivateProfileString(_T("DATA_PARA"), key, _T("0"), temp, sizeof(temp), file_path);
		dat.Format(_T("%s"), temp);
		m_DataPara.DataOKImageSave = atoi(dat);

		key.Empty();
		key.Format(_T("NG_IMAGE_SAVE"));
		GetPrivateProfileString(_T("DATA_PARA"), key, _T("0"), temp, sizeof(temp), file_path);
		dat.Format(_T("%s"), temp);
		m_DataPara.DataNGImageSave = atoi(dat);

		key.Empty();
		key.Format(_T("INSP_RSLT_IMAGE_SAVE"));
		GetPrivateProfileString(_T("DATA_PARA"), key, _T("0"), temp, sizeof(temp), file_path);
		dat.Format(_T("%s"), temp);
		m_DataPara.DataInspRsltImageSave = atoi(dat);

		//20161011 ht 언어 init값 불러오기
		key.Empty();
		key.Format(_T("LANGUAGE"));
		GetPrivateProfileString(_T("DATA_PARA"), key, _T("0"), temp, sizeof(temp), file_path);
		dat.Format(_T("%s"), temp);
		m_DataPara.m_bDataLanguage = atoi(dat);

		//20161014 ht 저장된 비밀번호값 불러오기
		key.Empty();
		key.Format(_T("PASSWORD"));
		GetPrivateProfileString(_T("DATA_PARA"), key, _T("0000"), temp, sizeof(temp), file_path);
		dat.Format(_T("%s"), temp);
		sprintf(m_DataPara.m_cPassword, "%s", dat);		
	}
	fd=0;

	strLog.Format(_T("LoadDataPara End!!"));
	WriteLog(strLog);

	return rslt;
}

BOOL CDialogData::WriteLog(CString strLog)
{
	CLED_DisplayInspectDlg *pdlg = ((CLED_DisplayInspectDlg *)AfxGetApp()->m_pMainWnd);
	BOOL rslt = FALSE;
	rslt = pdlg->WriteLog(strLog, TYPE_LOG_DATA);

	return rslt;
}

typedefModelPara *CDialogData::GetAllModel()
{
	return m_dlgModel->GetAllModel();
}

BOOL CDialogData::PreTranslateMessage(MSG* pMsg)
{
	// TODO: 여기에 특수화된 코드를 추가 및/또는 기본 클래스를 호출합니다.
	if(pMsg->message ==WM_KEYDOWN)
	{
		if(pMsg->wParam == VK_RETURN || pMsg->wParam == VK_ESCAPE)
			return TRUE;
	}

	return CDialogEx::PreTranslateMessage(pMsg);
}


//20161011 ht Data다이얼로그 언어 설정
void CDialogData::ChangeLanguage(BOOL StateLanguage)
{
	switch(StateLanguage)
	{
	case 0:
		GetDlgItem(IDC_BTNENHCTRL_CHECK_0)->SetWindowText(_T("OK Image Save"));
		GetDlgItem(IDC_BTNENHCTRL_CHECK_1)->SetWindowText(_T("NG Image Save"));
		GetDlgItem(IDC_BTNENHCTRL_CHECK_2)->SetWindowText(_T("Grab Image Save"));

		GetDlgItem(IDC_BTNENHCTRL_AXIX_NAME)->SetWindowText(_T("Delete Day"));
		GetDlgItem(IDC_BTNENHCTRL_AXIX_NAME2)->SetWindowText(_T("Delete Hour"));
		GetDlgItem(IDC_BTNENHCTRL_AXIX_NAME3)->SetWindowText(_T("Delete Minute"));
		GetDlgItem(IDC_BTN_CHANGEPW)->SetWindowText(_T("Change P/W"));

		GetDlgItem(IDC_BTNENHCTRL_APPLY)->SetWindowText(_T("Apply"));
		GetDlgItem(IDC_BTNENHCTRL_OK)->SetWindowText(_T("Save"));
		GetDlgItem(IDC_BTNENHCTRL_CANCEL)->SetWindowText(_T("Cancel"));
		break;
	case 1:
		GetDlgItem(IDC_BTNENHCTRL_CHECK_0)->SetWindowText(_T("양품 이지미 저장"));
		GetDlgItem(IDC_BTNENHCTRL_CHECK_1)->SetWindowText(_T("불량 이미지 저장"));
		GetDlgItem(IDC_BTNENHCTRL_CHECK_2)->SetWindowText(_T("촬상 이미지 저장"));

		GetDlgItem(IDC_BTNENHCTRL_AXIX_NAME)->SetWindowText(_T("삭제 주기(일)"));
		GetDlgItem(IDC_BTNENHCTRL_AXIX_NAME2)->SetWindowText(_T("삭제 시간"));
		GetDlgItem(IDC_BTNENHCTRL_AXIX_NAME3)->SetWindowText(_T("삭제 분"));
		GetDlgItem(IDC_BTN_CHANGEPW)->SetWindowText(_T("비밀번호 변경"));

		GetDlgItem(IDC_BTNENHCTRL_APPLY)->SetWindowText(_T("적용"));
		GetDlgItem(IDC_BTNENHCTRL_OK)->SetWindowText(_T("저장"));
		GetDlgItem(IDC_BTNENHCTRL_CANCEL)->SetWindowText(_T("취소"));
		break;
	}
	m_dlgModel->ChangeLanguage(StateLanguage);
}

//20161011 ht 버튼 클릭 시 언어 선택 및 언어 init값 설정
void CDialogData::ClickBtnenhctrlLangEnglish()
{
	// TODO: 여기에 메시지 처리기 코드를 추가합니다.
	CLED_DisplayInspectDlg *pdlg = ((CLED_DisplayInspectDlg *)AfxGetApp()->m_pMainWnd);
	m_ctrlLanguageEng.SetValue(TRUE);
	m_ctrlLanguageKor.SetValue(FALSE);
	m_DataPara.m_bDataLanguage = 0;
	//pdlg->ChangeLanguage(LANGUAGE_ENG);
}

//20161011 ht 버튼 클릭 시 언어 선택 및 언어 init값 설정
void CDialogData::ClickBtnenhctrlLangKorean()
{
	// TODO: 여기에 메시지 처리기 코드를 추가합니다.
	CLED_DisplayInspectDlg *pdlg = ((CLED_DisplayInspectDlg *)AfxGetApp()->m_pMainWnd);
	m_ctrlLanguageEng.SetValue(FALSE);
	m_ctrlLanguageKor.SetValue(TRUE);
	m_DataPara.m_bDataLanguage = 1;
	//pdlg->ChangeLanguage(LANGUAGE_KOR);
}



//20161014 ht DialogData에서 비밀번호 변경 버튼 클릭시 다이얼로그생성 및 처리
void CDialogData::ClickBtnChangepw()
{
	CDialogChangePassword dlgChangePassword;

	dlgChangePassword.SetCurrentPW(m_DataPara.m_cPassword);

	int Result_PW = dlgChangePassword.DoModal();
	if(IDOK == Result_PW)
	{
		sprintf(m_DataPara.m_cPassword, "%s", dlgChangePassword.GetCurrentPW());
	}
	else if(IDCANCEL == Result_PW)
	{

	}
}


void CDialogData::ClickBtnenhctrlCheck3()
{
	// TODO: 여기에 메시지 처리기 코드를 추가합니다.
	m_DataPara.DataInspRsltImageSave = m_btn_InspRsltImgSave.GetValue();
}
