// D:\Project\디지티\LED_DisplayInspect\Source\Dialog\DialogModel.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "LED_DisplayInspect.h"
#include "DialogModel.h"
#include "afxdialogex.h"

BOOL XCopy(LPCTSTR Src, LPCTSTR Dest)
{
	HANDLE hSrch;
	WIN32_FIND_DATA wfd;
	BOOL bResult = TRUE;
	TCHAR WildCard[MAX_PATH];
	TCHAR SrcFile[MAX_PATH];
	TCHAR DestFile[MAX_PATH];

	wsprintf(WildCard, _T("%s\\*.*"), Src);
	CreateDirectory(Dest, NULL);
	hSrch=FindFirstFile(WildCard, &wfd);
	if (hSrch == INVALID_HANDLE_VALUE) 
	{
		return FALSE;
	}

	while(bResult)
	{
		wsprintf(SrcFile, _T("%s\\%s"), Src, wfd.cFileName);
		wsprintf(DestFile, _T("%s\\%s"), Dest, wfd.cFileName);
		// 서브 디렉토리가 발견되면 서브 디렉토리를 복사한다.
		if(wfd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)
		{
			if (lstrcmp(wfd.cFileName,_T(".")) && lstrcmp(wfd.cFileName,_T("..")))
			{
				XCopy(SrcFile, DestFile);
			}
		} 
		else 
		{
			CopyFile(SrcFile,DestFile,FALSE);
		}
		bResult=FindNextFile(hSrch,&wfd);
	}
	FindClose(hSrch);

	return TRUE;
}

void _MakeDirectories1(CString path)
{
	char tmp_path[MAX_PATH];
	char sub[MAX_PATH];
	long size;
	size = sizeof(path);

	long index = 0;

	wsprintf(tmp_path, _T("%s"), (LPCSTR)path);
	memset(sub, NULL, sizeof(sub));

	while(1)
	{
		if(tmp_path[index] == '\\')
		{
			CreateDirectory(sub, NULL);
		}

		sub[index] = tmp_path[index];

		if(tmp_path[index] == '\0')
		{
			CreateDirectory(sub, NULL);
			break;
		}

		//base_index++;
		index++;

		if( (index > MAX_PATH) )
		{
			AfxMessageBox(_T("Error : index overflow(_MakeDirectories)!"));
			break;
		}

	}
}

// CDialogModel 대화 상자입니다.

IMPLEMENT_DYNAMIC(CDialogModel, CDialogEx)

CDialogModel::CDialogModel(CWnd* pParent /*=NULL*/)
	: CDialogEx(CDialogModel::IDD, pParent)
	, m_edit_SelectModelName(_T(""))
	, m_edit_NewModelName(_T(""))
	, m_edit_strSrcModelName(_T(""))
	, m_edit_strDesModelName(_T(""))
	, m_edit_strSrcModelIdx(_T(""))
	, m_edit_strDesModelIdx(_T(""))
	, m_edit_SelectModelIdx(_T(""))
	, m_edit_NewModelIdx(_T(""))
{
	memset(&m_SelectModel, NULL, sizeof(typedefModelPara));
	memset(m_AllModel, NULL, sizeof(typedefModelPara) * MAX_MODEL_NUM);
	memset(m_DeleteModel, NULL, sizeof(typedefModelPara) * MAX_MODEL_NUM);
	m_bCopyModel = FALSE;
}

CDialogModel::~CDialogModel()
{
	SaveSelectModelNo();
	//SaveModelLable();
}

void CDialogModel::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST_MODEL, m_list_Model);
	DDX_Text(pDX, IDC_EDIT_MODEL_NAME, m_edit_SelectModelName);
	DDX_Text(pDX, IDC_EDIT_MODEL_NAME2, m_edit_NewModelName);
	DDX_Text(pDX, IDC_EDIT_MODEL_NAME3, m_edit_strSrcModelName);
	DDX_Text(pDX, IDC_EDIT_MODEL_NAME4, m_edit_strDesModelName);
	DDX_Text(pDX, IDC_EDIT_MODEL_NAME5, m_edit_strSrcModelIdx);
	DDX_Text(pDX, IDC_EDIT_MODEL_NAME6, m_edit_strDesModelIdx);
	DDX_Text(pDX, IDC_EDIT_MODEL_NAME7, m_edit_SelectModelIdx);
	DDX_Text(pDX, IDC_EDIT_MODEL_NAME8, m_edit_NewModelIdx);
}


BEGIN_MESSAGE_MAP(CDialogModel, CDialogEx)
	ON_NOTIFY(NM_DBLCLK, IDC_LIST_MODEL, &CDialogModel::OnNMDblclkListModel)
	ON_WM_SHOWWINDOW()
	ON_NOTIFY(NM_CLICK, IDC_LIST_MODEL, &CDialogModel::OnNMClickListModel)
	ON_NOTIFY(NM_RCLICK, IDC_LIST_MODEL, &CDialogModel::OnNMRClickListModel)
	ON_NOTIFY(NM_RDBLCLK, IDC_LIST_MODEL, &CDialogModel::OnNMRDblclkListModel)
END_MESSAGE_MAP()


// CDialogModel 메시지 처리기입니다.

BOOL CDialogModel::GetOpenList(int idx)
{
	// TODO: 여기에 메시지 처리기 코드를 추가합니다.
	//존재하면 FALSE
	CString str;
	str.Format(_T("%s"), m_AllModel[idx].cName);
	if(str.IsEmpty())
	{
		return TRUE;
	}
	else
	{
		//모델명이 존재하면
		return FALSE;
	}
}

void CDialogModel::OnNMDblclkListModel(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMITEMACTIVATE pNMItemActivate = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	NM_LISTVIEW* pNMListView = (NM_LISTVIEW*)pNMHDR;

	int idx = pNMListView->iItem;
	CString strTemp;
	m_bSelModel = FALSE;
	strTemp = m_list_Model.GetItemText(idx, 0);
	m_nDBClickIdx = atoi(strTemp) - 1;
	m_DesSelectModel.nIdx = m_nDBClickIdx;
	m_edit_SelectModelIdx = strTemp;
	strTemp = m_list_Model.GetItemText(idx, 1);
	sprintf(m_DesSelectModel.cName, "%s", strTemp);
	m_edit_SelectModelName = strTemp;
	strTemp = m_list_Model.GetItemText(idx, 2);
	sprintf(m_DesSelectModel.cPath, "%s", strTemp);
	strTemp = m_list_Model.GetItemText(idx, 3);
	sprintf(m_DesSelectModel.cCreatDate, "%s", strTemp);
	ListUpdate();
	UpdateData(FALSE);

	*pResult = 0;
}


BOOL CDialogModel::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  여기에 추가 초기화 작업을 추가합니다.

	LoadModelLable();
	LoadSelectModelNo();

	InitList();

	CString str;
	m_bNonModel = TRUE;
	for(int i = 0; i < MAX_MODEL_NUM; i++)
	{
		str.Format(_T("%s"), m_AllModel[i].cName);
		if(!str.IsEmpty())
		{
			m_bNonModel = FALSE;
		}
	}

	if(m_bNonModel)
	{
		AfxMessageBox(_T("모델이 없습니다. 모델을 생성 하십시오."));
	}

	return TRUE;  // return TRUE unless you set the focus to a control
	// 예외: OCX 속성 페이지는 FALSE를 반환해야 합니다.
}

BOOL CDialogModel::GetNonModel()
{
	return m_bNonModel;
}

BEGIN_EVENTSINK_MAP(CDialogModel, CDialogEx)
	ON_EVENT(CDialogModel, IDC_BTNENHCTRL_SELECT, DISPID_CLICK, CDialogModel::ClickBtnenhctrlSelect, VTS_NONE)
	ON_EVENT(CDialogModel, IDC_BTNENHCTRL_DELETE, DISPID_CLICK, CDialogModel::ClickBtnenhctrlDelete, VTS_NONE)
	ON_EVENT(CDialogModel, IDC_BTNENHCTRL_NEW, DISPID_CLICK, CDialogModel::ClickBtnenhctrlNew, VTS_NONE)
	ON_EVENT(CDialogModel, IDC_BTNENHCTRL_COPY, DISPID_CLICK, CDialogModel::ClickBtnenhctrlCopy, VTS_NONE)
END_EVENTSINK_MAP()

void CDialogModel::InitList()
{
	CString str;
	int i;
	long setstyle;
	LV_COLUMN column;
	CImageList *img_list_slct;
	CBitmap bitmap;
	m_list_Model;

	//List
	img_list_slct = new CImageList;
	img_list_slct->Create(16, 16, ILC_COLOR4, 3, 3);

	bitmap.LoadBitmap(IDB_BITMAP_NOTSELECTED);
	img_list_slct->Add(&bitmap, RGB(0, 0, 0));
	bitmap.DeleteObject();
	bitmap.LoadBitmap(IDB_BITMAP_SELECTED);
	img_list_slct->Add(&bitmap, RGB(0, 0, 0));
	bitmap.DeleteObject();
	bitmap.LoadBitmap(IDB_BITMAP_NEW_SEL);
	img_list_slct->Add(&bitmap, RGB(0, 0, 0));
	bitmap.DeleteObject();
	bitmap.LoadBitmap(IDB_BITMAP_MODEL_LCLCK);
	img_list_slct->Add(&bitmap, RGB(0, 0, 0));
	bitmap.DeleteObject();
	bitmap.LoadBitmap(IDB_BITMAP_MODEL_RCLCK);
	img_list_slct->Add(&bitmap, RGB(0, 0, 0));
	bitmap.DeleteObject();
	bitmap.LoadBitmap(IDB_BITMAP_MODEL_DBCLCK);
	img_list_slct->Add(&bitmap, RGB(0, 0, 0));
	bitmap.DeleteObject();
	m_list_Model.SetImageList(img_list_slct, LVSIL_SMALL);

	setstyle = GetWindowLong(m_list_Model.m_hWnd, GWL_STYLE);
	setstyle |= LVS_EDITLABELS;
	SetWindowLong(m_list_Model.m_hWnd, GWL_STYLE, setstyle);

	m_list_Model.RedrawWindow();

	char* list[4]={_T("NO."), _T("NAME"), _T("PATH"), _T("Creat Date")};
	int width[5]={60, 300, 500, 150};
	for(i = 0; i < 4; i++)
	{
		column.mask= LVCF_FMT | LVCF_SUBITEM | LVCF_TEXT | LVCF_WIDTH;
		column.fmt=LVCFMT_LEFT;
		column.pszText=list[i];
		column.iSubItem=i;
		column.cx=width[i];
		m_list_Model.InsertColumn(i, &column);
	}
	
	AddList();
}

void CDialogModel::AddList()
{
	m_list_Model.DeleteAllItems();
	CString str;
	int i = 0;
	//ADD List
	for(i = 0; i < MAX_MODEL_NUM; i++)
	{
		str.Format(_T("%04d"), i + 1);
		m_list_Model.InsertItem(i, str);
		m_list_Model.SetItemText(i, 1, m_AllModel[i].cName);
		m_list_Model.SetItemText(i, 2, m_AllModel[i].cPath);
		m_list_Model.SetItemText(i, 3, m_AllModel[i].cCreatDate);
	}

	ListUpdate();
}

void CDialogModel::ListUpdate()
{
	//Set
	CString str;
	LV_ITEM lvitem;
	for(int i = 0; i < MAX_MODEL_NUM; i++)
	{
		str.Format(_T("%04d"), i + 1);

		if(i == m_SelectModel.nIdx)
		{
			lvitem.mask=LVIF_TEXT | LVIF_IMAGE;
			lvitem.iItem=i;
			lvitem.iSubItem=0;
			lvitem.iImage = 1;
			lvitem.pszText=(LPSTR)(LPCSTR)str;
			m_list_Model.SetItem(&lvitem);
		}
		else if(i == m_nDBClickIdx)
		{
			lvitem.mask=LVIF_TEXT | LVIF_IMAGE;
			lvitem.iItem=i;
			lvitem.iSubItem=0;
			lvitem.iImage = 5;
			lvitem.pszText=(LPSTR)(LPCSTR)str;
			m_list_Model.SetItem(&lvitem);
		}
		else if(i == m_nLClickIdx)
		{
			lvitem.mask=LVIF_TEXT | LVIF_IMAGE;
			lvitem.iItem=i;
			lvitem.iSubItem=0;
			lvitem.iImage = 3;
			lvitem.pszText=(LPSTR)(LPCSTR)str;
			m_list_Model.SetItem(&lvitem);
		}
		else if(i == m_nRClickIdx)
		{
			lvitem.mask=LVIF_TEXT | LVIF_IMAGE;
			lvitem.iItem=i;
			lvitem.iSubItem=0;
			lvitem.iImage = 4;
			lvitem.pszText=(LPSTR)(LPCSTR)str;
			m_list_Model.SetItem(&lvitem);
		}
		else if(i == m_nNewModelNum)
		{
			lvitem.mask=LVIF_TEXT | LVIF_IMAGE;
			lvitem.iItem=i;
			lvitem.iSubItem=0;
			lvitem.iImage = 2;
			lvitem.pszText=(LPSTR)(LPCSTR)str;
			m_list_Model.SetItem(&lvitem);
		}
		else
		{
			lvitem.mask=LVIF_TEXT | LVIF_IMAGE;
			lvitem.iItem=i;
			lvitem.iSubItem=0;
			lvitem.iImage = 0;
			lvitem.pszText=(LPSTR)(LPCSTR)str;
			m_list_Model.SetItem(&lvitem);
		}
	}
	UpdateData(FALSE);
}

BOOL CDialogModel::LoadModelLable()
{
	BOOL rslt = TRUE;
	CString key, dat;
	char temp[DAT_STR_LENG];
	CString file_path, temp_dat;
	CString name, path, creat_date;
	
	_MakeDirectories1(MODEL_PATH);

	file_path.Format(_T("%s\\ModelLabel.par"), MODEL_PATH);

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
		
		for(int i = 0; i < MAX_MODEL_NUM; i++)
		{
			key.Empty();
			key.Format(_T("MODEL NAME_%d"), i + 1);
			GetPrivateProfileString(_T("MODEL LABEL"), key, _T(" "), temp, sizeof(temp), file_path);
			temp_dat.Format(_T("%s"), temp);
			AfxExtractSubString(name, temp_dat, 0, ',');
			AfxExtractSubString(path, temp_dat, 1, ',');
			AfxExtractSubString(creat_date, temp_dat, 2, ',');

			m_AllModel[i].nIdx = i;
			sprintf(m_AllModel[i].cName, "%s", name);
			sprintf(m_AllModel[i].cPath, "%s", path);
			sprintf(m_AllModel[i].cCreatDate, "%s", creat_date);
		}
	}
	fd=0;
	
	return rslt;
}

BOOL CDialogModel::SaveModelLable()
{
	BOOL rslt = TRUE;
	CString key, dat;
	CString file_path, temp_dat;
	
	_MakeDirectories1(MODEL_PATH);

	file_path.Format(_T("%s\\ModelLabel.par"), MODEL_PATH);

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
		
		for(int i = 0; i < MAX_MODEL_NUM; i++)
		{
			key.Empty();
			key.Format(_T("MODEL NAME_%d"), i + 1);
			dat.Empty();
			dat.Format(_T("%s"), m_AllModel[i].cName);
			temp_dat.Format(_T("%s,%s"), dat, m_AllModel[i].cPath);
			dat.Format(_T("%s,%s"), temp_dat, m_AllModel[i].cCreatDate);
			WritePrivateProfileString(_T("MODEL LABEL"), key, dat, file_path);			
		}

	}
	fd=0;
	
	return rslt;
}

BOOL CDialogModel::LoadSelectModelNo()
{
	BOOL rslt = TRUE;
	CString key, dat;
	char temp[DAT_STR_LENG];
	CString file_path;
	
	_MakeDirectories1(MODEL_PATH);

	file_path.Format(_T("%s\\ModelNo.par"), MODEL_PATH);

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
		key.Format(_T("MODEL NAME_NO"));
		GetPrivateProfileString(_T("MODEL NO"), key, _T("0"), temp, sizeof(temp), file_path);
		dat.Format(_T("%s"), temp);
		m_SelectModel.nIdx = atoi(dat);

		key.Empty();
		key.Format(_T("MODEL NAME_NAME"));
		GetPrivateProfileString(_T("MODEL NO"), key, _T("NULL"), temp, sizeof(temp), file_path);
		dat.Format(_T("%s"), temp);
		sprintf(m_SelectModel.cName, "%s", dat);

		key.Empty();
		key.Format(_T("MODEL NAME_PATH"));
		GetPrivateProfileString(_T("MODEL NO"), key, _T("NULL"), temp, sizeof(temp), file_path);
		dat.Format(_T("%s"), temp);
		sprintf(m_SelectModel.cPath, "%s", dat);

		key.Empty();
		key.Format(_T("MODEL NAME_CREAT_DATE"));
		GetPrivateProfileString(_T("MODEL NO"), key, _T("NULL"), temp, sizeof(temp), file_path);
		dat.Format(_T("%s"), temp);
		sprintf(m_SelectModel.cCreatDate, "%s", dat);
		
	}
	fd=0;

	return rslt;
}

BOOL CDialogModel::SaveSelectModelNo()
{
	BOOL rslt = TRUE;
	CString key, dat;
	CString file_path;
	
	_MakeDirectories1(MODEL_PATH);

	file_path.Format(_T("%s\\ModelNo.par"), MODEL_PATH);


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
		key.Format(_T("MODEL NAME_NO"));
		dat.Empty();
		dat.Format(_T("%d"), m_SelectModel.nIdx);
		WritePrivateProfileString(_T("MODEL NO"), key, dat, file_path);	

		key.Empty();
		key.Format(_T("MODEL NAME_NAME"));
		dat.Empty();
		dat.Format(_T("%s"), m_SelectModel.cName);
		WritePrivateProfileString(_T("MODEL NO"), key, dat, file_path);

		key.Empty();
		key.Format(_T("MODEL NAME_PATH"));
		dat.Empty();
		dat.Format(_T("%s"), m_SelectModel.cPath);
		WritePrivateProfileString(_T("MODEL NO"), key, dat, file_path);

		key.Empty();
		key.Format(_T("MODEL NAME_CREAT_DATE"));
		dat.Empty();
		dat.Format(_T("%s"), m_SelectModel.cCreatDate);
		WritePrivateProfileString(_T("MODEL NO"), key, dat, file_path);

	}
	fd=0;
	
	return rslt;
}

void CDialogModel::ClickBtnenhctrlSelect()
{
	// TODO: 여기에 메시지 처리기 코드를 추가합니다.
	m_bSelModel = TRUE;
	m_SelectModel = m_DesSelectModel;
	SelectModel();
}


void CDialogModel::ClickBtnenhctrlDelete()
{
	// TODO: 여기에 메시지 처리기 코드를 추가합니다.
	DeleteModel();
}


void CDialogModel::ClickBtnenhctrlNew()
{
	// TODO: 여기에 메시지 처리기 코드를 추가합니다.
	m_bNewModel = TRUE;
	NewModel();
}

BOOL CDialogModel::SetSaveModelLable()
{
	return SaveModelLable();
}

BOOL CDialogModel::SetSaveSelectModelNo()
{
	return SaveSelectModelNo();
}


BOOL CDialogModel::NewModel()
{
	UpdateData(TRUE);
	CString strLog;
	CTime cTime;
	cTime = CTime::GetCurrentTime();

	if(m_edit_NewModelName.IsEmpty())
	{
		strLog.Format(_T("Model Name is Empty!"));
		AfxMessageBox(strLog);
		return FALSE;
	}

	

	sprintf(m_AllModel[m_nNewModelNum].cName, "%s", m_edit_NewModelName);
	sprintf(m_AllModel[m_nNewModelNum].cPath, "%s\\%s", MODEL_PATH, m_AllModel[m_nNewModelNum].cName);
	sprintf(m_AllModel[m_nNewModelNum].cCreatDate, "%04d%02d%02d-%02d:%02d:%02d", cTime.GetYear(), cTime.GetMonth(), cTime.GetDay(),
																	cTime.GetHour(), cTime.GetMinute(), cTime.GetSecond());

	
	/*sprintf(m_SelectModel.cName, "%s", m_AllModel[m_SelectModel.nIdx].cName);
	sprintf(m_SelectModel.cPath, "%s", m_AllModel[m_SelectModel.nIdx].cPath);
	sprintf(m_SelectModel.cCreatDate, "%s", m_AllModel[m_SelectModel.nIdx].cCreatDate);*/

	m_bNonModel = FALSE;

	AddList();

	return TRUE;
}

BOOL CDialogModel::SelectModel()
{
	CString str;

	str.Format(_T("%s"), m_SelectModel.cName);
	if(str.IsEmpty())
	{
		AfxMessageBox(_T("해당 모델은 생성 되어 있지 않습니다."));
		return FALSE;
	}

	m_bNonModel = FALSE;
	m_SelectModel.nIdx = m_nDBClickIdx;;

	ListUpdate();

	return TRUE;
}

BOOL CDialogModel::DeleteModel()
{
	CString str;
	m_nDeleteCnt++;

	str.Format(_T("%s"), m_AllModel[m_SelectModel.nIdx].cName);

	if(str.IsEmpty())
	{
		m_nDeleteCnt--;
		return FALSE;
	}

	m_DeleteModel[m_nDeleteCnt] = m_AllModel[m_SelectModel.nIdx];

	sprintf(m_AllModel[m_SelectModel.nIdx].cName, "");
	sprintf(m_AllModel[m_SelectModel.nIdx].cPath, "");
	sprintf(m_AllModel[m_SelectModel.nIdx].cCreatDate, "");

	sprintf(m_SelectModel.cName, "");
	sprintf(m_SelectModel.cPath, "");
	sprintf(m_SelectModel.cCreatDate, "");

	m_bNonModel = TRUE;
	for(int i = 0; i < MAX_MODEL_NUM; i++)
	{
		str.Format(_T("%s"), m_AllModel[i].cName);

		if(!str.IsEmpty())
		{
			m_bNonModel = FALSE;
			m_SelectModel.nIdx = i;
			sprintf(m_SelectModel.cName, "%s", m_AllModel[i].cName);
			sprintf(m_SelectModel.cPath, "%s", m_AllModel[i].cPath);
			sprintf(m_SelectModel.cCreatDate, "%s", m_AllModel[i].cCreatDate);
			break;
		}
	}

	AddList();

	return TRUE;
}

BOOL CDialogModel::SetCopyModel()
{
	return CopyModel();
}

BOOL CDialogModel::CopyModel()
{
	UpdateData(TRUE);
	CString strLog;
	CTime cTime;
	cTime = CTime::GetCurrentTime();

	if(!XCopy(m_CopySrcModel.cPath, m_CopyDesModel.cPath))
	{
		strLog.Format(_T("기본 Model No %d 폴더가 없습니다."), m_CopySrcModel.nIdx);
		AfxMessageBox(strLog);
		return FALSE;
	}

	sprintf(m_AllModel[m_CopyDesModel.nIdx].cName, "%s", m_CopyDesModel.cName);
	sprintf(m_AllModel[m_CopyDesModel.nIdx].cPath, "%s\\%s", MODEL_PATH, m_CopyDesModel.cName);
	sprintf(m_AllModel[m_CopyDesModel.nIdx].cCreatDate, "%04d%02d%02d-%02d:%02d:%02d", cTime.GetYear(), cTime.GetMonth(), cTime.GetDay(),
																	cTime.GetHour(), cTime.GetMinute(), cTime.GetSecond());

	AddList();

	m_bCopyModel = FALSE;

	return TRUE;
}

typedefModelPara CDialogModel::GetDeleteModel(int delete_cnt)
{
	return m_DeleteModel[delete_cnt];
}

int CDialogModel::GetDeleteModelCnt()
{
	return m_nDeleteCnt + 1;
}

BOOL CDialogModel::GetCopyModel()
{
	return m_bCopyModel;
}

void CDialogModel::OnShowWindow(BOOL bShow, UINT nStatus)
{
	CDialogEx::OnShowWindow(bShow, nStatus);

	if(bShow)
	{
		m_bSelModel = TRUE;
		m_nLClickIdx = -1;
		m_nRClickIdx = -1;
		m_nDBClickIdx = -1;
		m_bNewModel = FALSE;
		m_nNewModelNum = -1;
		m_nDeleteCnt = -1;
		LoadModelLable();
		LoadSelectModelNo();

		AddList();
		m_edit_SelectModelIdx.Format(_T("%04d"), m_SelectModel.nIdx + 1);
		m_edit_SelectModelName.Format(_T("%s"), m_SelectModel.cName);
		m_bNonModel = TRUE;
		m_bCopyModel = FALSE;
		if(!m_edit_SelectModelName.IsEmpty())
		{
			m_bNonModel = FALSE;
		}
		m_bSelModel = FALSE;
		m_edit_NewModelName = _T("");
		m_edit_strSrcModelIdx = _T("");
		m_edit_strDesModelIdx = _T("");
		m_edit_strSrcModelName = _T("");
		m_edit_strDesModelName = _T("");
		m_edit_NewModelIdx = _T("");

		UpdateData(FALSE);
	}
	else
	{
		memset(m_AllModel, NULL, sizeof(typedefModelPara) * MAX_MODEL_NUM);
		memset(m_DeleteModel, NULL, sizeof(typedefModelPara) * MAX_MODEL_NUM);
		memset(&m_CopySrcModel, NULL, sizeof(typedefModelPara));
		memset(&m_CopyDesModel, NULL, sizeof(typedefModelPara));
	}

	// TODO: 여기에 메시지 처리기 코드를 추가합니다.
}

typedefModelPara CDialogModel::GetSrcCopyModel()
{
	return m_CopySrcModel;
}

typedefModelPara CDialogModel::GetDesCopyModel()
{
	return m_CopyDesModel;
}

typedefModelPara CDialogModel::GetSelectModel()
{
	return m_SelectModel;
}

void CDialogModel::ClickBtnenhctrlCopy()
{
	// TODO: 여기에 메시지 처리기 코드를 추가합니다.
	m_bCopyModel = TRUE;
}


void CDialogModel::OnNMClickListModel(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMITEMACTIVATE pNMItemActivate = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	NM_LISTVIEW* pNMListView = (NM_LISTVIEW*)pNMHDR;
	int idx = pNMListView->iItem;
	CString strTemp;
	UpdateData(TRUE);

	strTemp = m_list_Model.GetItemText(idx, 0);
	m_edit_strSrcModelIdx.Format(_T("%s"), strTemp);
	m_nLClickIdx =  atoi(strTemp) - 1;
	strTemp = m_list_Model.GetItemText(idx, 1);
	m_edit_strSrcModelName.Format(_T("%s"), strTemp);
	strTemp = m_list_Model.GetItemText(idx, 2);

	m_CopySrcModel;
	sprintf(m_CopySrcModel.cPath, "%s", strTemp);
	ListUpdate();
	UpdateData(FALSE);
	*pResult = 0;
}


void CDialogModel::OnNMRClickListModel(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMITEMACTIVATE pNMItemActivate = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	NM_LISTVIEW* pNMListView = (NM_LISTVIEW*)pNMHDR;
	int idx = pNMListView->iItem;
	CString strTemp;
	UpdateData(TRUE);

	
	strTemp = m_list_Model.GetItemText(idx, 0);
	m_CopyDesModel.nIdx = atoi(strTemp) - 1;
	m_nRClickIdx = m_CopyDesModel.nIdx;
	m_edit_strDesModelIdx.Format(_T("%s"), strTemp);
	strTemp = m_list_Model.GetItemText(idx, 1);
	m_edit_strDesModelName.Format(_T("%s"), strTemp);
	sprintf(m_CopyDesModel.cName, "%s", m_edit_strDesModelName);
	strTemp = m_list_Model.GetItemText(idx, 2);

	m_CopyDesModel;
	sprintf(m_CopyDesModel.cPath, "%s\\%s", MODEL_PATH, m_edit_strDesModelName);
	ListUpdate();
	UpdateData(FALSE);
	*pResult = 0;
}

typedefModelPara *CDialogModel::GetAllModel()
{
	return m_AllModel;
}

BOOL CDialogModel::PreTranslateMessage(MSG* pMsg)
{
	// TODO: 여기에 특수화된 코드를 추가 및/또는 기본 클래스를 호출합니다.
	if(pMsg->message ==WM_KEYDOWN)
	{
		if(pMsg->wParam == VK_RETURN || pMsg->wParam == VK_ESCAPE)
			return TRUE;
	}

	return CDialogEx::PreTranslateMessage(pMsg);
}

//20161011 ht Model다이얼로그 언어 설정
void CDialogModel::ChangeLanguage(BOOL StateLanguage)
{
	switch(StateLanguage)
	{
	case 0:
		GetDlgItem(IDC_BTNENHCTRL_LABEL_2)->SetWindowText(_T("Model Name"));
		GetDlgItem(IDC_BTNENHCTRL_LABEL_4)->SetWindowText(_T("Model Name"));
		GetDlgItem(IDC_BTNENHCTRL_LABEL_5)->SetWindowText(_T("Model Idx"));
		GetDlgItem(IDC_BTNENHCTRL_LABEL_6)->SetWindowText(_T("Model Idx"));
		GetDlgItem(IDC_BTNENHCTRL_COPY)->SetWindowText(_T("Copy"));

		//select & delete model
		GetDlgItem(IDC_BTNENHCTRL_LABEL_9)->SetWindowText(_T("Model Idx"));
		GetDlgItem(IDC_BTNENHCTRL_LABEL_0)->SetWindowText(_T("Model Name"));
		GetDlgItem(IDC_BTNENHCTRL_SELECT)->SetWindowText(_T("Select"));
		GetDlgItem(IDC_BTNENHCTRL_DELETE)->SetWindowText(_T("Delete"));
		
		//New Model
		GetDlgItem(IDC_BTNENHCTRL_LABEL_32)->SetWindowText(_T("Model Idx"));
		GetDlgItem(IDC_BTNENHCTRL_LABEL_1)->SetWindowText(_T("Model Name"));
		GetDlgItem(IDC_BTNENHCTRL_NEW)->SetWindowText(_T("New"));
		break;
	case 1:
		GetDlgItem(IDC_BTNENHCTRL_LABEL_2)->SetWindowText(_T("모델 이름"));
		GetDlgItem(IDC_BTNENHCTRL_LABEL_4)->SetWindowText(_T("모델 이름"));
		GetDlgItem(IDC_BTNENHCTRL_LABEL_5)->SetWindowText(_T("모델 번호"));
		GetDlgItem(IDC_BTNENHCTRL_LABEL_6)->SetWindowText(_T("모델 번호"));
		GetDlgItem(IDC_BTNENHCTRL_COPY)->SetWindowText(_T("복사"));

		//select & delete model
		GetDlgItem(IDC_BTNENHCTRL_LABEL_9)->SetWindowText(_T("모델 번호"));
		GetDlgItem(IDC_BTNENHCTRL_LABEL_0)->SetWindowText(_T("모델 이름"));
		GetDlgItem(IDC_BTNENHCTRL_SELECT)->SetWindowText(_T("선택"));
		GetDlgItem(IDC_BTNENHCTRL_DELETE)->SetWindowText(_T("삭제"));
		
		//New Model
		GetDlgItem(IDC_BTNENHCTRL_LABEL_32)->SetWindowText(_T("모델 번호"));
		GetDlgItem(IDC_BTNENHCTRL_LABEL_1)->SetWindowText(_T("모델 이름"));
		GetDlgItem(IDC_BTNENHCTRL_NEW)->SetWindowText(_T("생성"));
		break;
	}
}

void CDialogModel::OnNMRDblclkListModel(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMITEMACTIVATE pNMItemActivate = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	NM_LISTVIEW* pNMListView = (NM_LISTVIEW*)pNMHDR;
	int idx = pNMListView->iItem;
	CString strTemp;
	UpdateData(TRUE);

	strTemp = m_list_Model.GetItemText(idx, 0);
	m_nNewModelNum = atoi(strTemp) - 1;
	m_edit_NewModelIdx = strTemp;


	ListUpdate();
	UpdateData(FALSE);
	*pResult = 0;
}
