#include "stdafx.h"
#include "_CtrlTracker.h"


//������
C_CtrlTracker::C_CtrlTracker()
{
	m_bGroup = FALSE;
	m_bEnable = FALSE;
	m_iType = 0;
	m_sCaption.Empty();
	m_cColor = RGB(127, 127, 127);
	m_rect=CRect(0,0,0,0);
	m_bDown=FALSE;
	m_pParentWnd=NULL;
	m_nProcess=0;
	m_nLineDot=10;
	m_nLindTick=3;
	m_nChoNum=-1;
	m_bSet=FALSE;
	m_OldPoint=CPoint(0,0);
	m_rectOld=CRect(0,0,0,0);//0527
	m_bActive = TRUE;
	m_dZoomStartX = 0;
	m_dZoomStartY = 0;
	m_dZoomEndX = 0;
	m_dZoomEndY = 0;
	m_dRectL = 0;
	m_dRectT = 0;
	m_dRectR = 0;
	m_dRectB = 0;
}

C_CtrlTracker::C_CtrlTracker(CWnd *pWnd, int type, int dot, int tick)
{
	m_bGroup = FALSE;
	m_bEnable = FALSE;
	m_iType = type;
	m_sCaption.Empty();
	m_cColor = RGB(127, 127, 127);
	m_rect=CRect(0,0,0,0);
	m_pParentWnd=pWnd;
	m_bDown=FALSE;
	m_nProcess=0;
	m_nLineDot=dot;
	m_nLindTick=tick;
	m_nChoNum=-1;
	m_bSet=FALSE;
	m_OldPoint=CPoint(0,0);
	m_rectOld=CRect(0,0,0,0);//0527
	m_bActive = TRUE;
	m_dZoomStartX = 0;
	m_dZoomStartY = 0;
	m_dZoomEndX = 0;
	m_dZoomEndY = 0;
	m_dRectL = 0;
	m_dRectT = 0;
	m_dRectR = 0;
	m_dRectB = 0;
}

//�����ڽ��� 8�� ���� ������ �̵�������
//���콺 �����Ͱ� �ִ°��� ã�� �Լ�
int C_CtrlTracker::PtInRect(CPoint point)
{
	int i;

	//�׷�����̸� 
	if(m_bGroup)
	{
		return 8;//�̵����
	}
		
	//12���� ������ m_rect�� �����Ͽ� �����.
	MakeDirect();
	//�� 12���� �������� ������ �ش� ��ȣ��
	//�����Ѵ�.
	for(i=0;i<12;i++)
	{
		if(m_rectDirect[i].PtInRect(point))
		{
			return i;
		}
	}

	//���� �ڽ� �������� ������ -1�� ����
	return -1;
}
//�ش� ���⿡ ���� ���콺 Ŀ���� �����Ѵ�.
void C_CtrlTracker::SetMouseCursor()
{
	if(!m_bActive)
	{
		return;
	}

	switch(m_iType)
	{
		case 0:
		case 1:
		case 2:
			switch(m_nChoNum)
			{
				case 0:
				case 2:
						SetClassLong(m_pParentWnd->m_hWnd,GCL_HCURSOR,(long)AfxGetApp()->LoadCursor(AFX_IDC_TRACKNWSE));
						break;
				case 1:
				case 3:
						SetClassLong(m_pParentWnd->m_hWnd,GCL_HCURSOR,(long)AfxGetApp()->LoadCursor(AFX_IDC_TRACKNESW));
						break;
				case 4:
				case 5:
						SetClassLong(m_pParentWnd->m_hWnd,GCL_HCURSOR,(long)AfxGetApp()->LoadCursor(AFX_IDC_TRACKNS));
						break;
				case 6:
				case 7:
						SetClassLong(m_pParentWnd->m_hWnd,GCL_HCURSOR,(long)AfxGetApp()->LoadCursor(AFX_IDC_TRACKWE));
						break;
				case 8:
				case 9:
				case 10:
				case 11:
						SetClassLong(m_pParentWnd->m_hWnd,GCL_HCURSOR,(long)AfxGetApp()->LoadCursor(AFX_IDC_TRACK4WAY));
						break;
				default:
						SetClassLong(m_pParentWnd->m_hWnd,GCL_HCURSOR,(long)LoadCursor(NULL,IDC_ARROW));
						break;
			}
			break;
		default:
			AfxMessageBox("Error : invalid index(SetMouseCursor)!");
			break;
	}
}
//���콺 �̵���
void C_CtrlTracker::OnMouseMove(UINT nFlags, CPoint point)
{
	if(!m_bActive)
	{
		return;
	}

	switch(m_iType)
	{
		case 0:
		case 2:
			if(m_bDown && m_nProcess!=0)
			{
				m_rectTrack.right=point.x;
				m_rectTrack.bottom=point.y;
				CDC *pDC=m_pParentWnd->GetDC();
				CPen pen,*oldPen;
				pDC->SetROP2(R2_XORPEN);
				pen.CreatePen(PS_DOT,2,RGB(127, 127, 127));
				oldPen=pDC->SelectObject(&pen);
				pDC->MoveTo(m_rectOld.left,m_rectOld.top);
				pDC->LineTo(m_rectOld.right,m_rectOld.top);
				pDC->LineTo(m_rectOld.right,m_rectOld.bottom);
				pDC->LineTo(m_rectOld.left,m_rectOld.bottom);
				pDC->LineTo(m_rectOld.left,m_rectOld.top);

				pDC->MoveTo(m_rectTrack.left,m_rectTrack.top);
				pDC->LineTo(m_rectTrack.right,m_rectTrack.top);
				pDC->LineTo(m_rectTrack.right,m_rectTrack.bottom);
				pDC->LineTo(m_rectTrack.left,m_rectTrack.bottom);
				pDC->LineTo(m_rectTrack.left,m_rectTrack.top);
				pDC->SelectObject(oldPen);
				pen.DeleteObject();
				m_rectOld=m_rectTrack;
				m_pParentWnd->ReleaseDC(pDC);
			}

			if(m_nProcess==0 && !m_bDown && m_bSet)
			{
				if(m_bEnable)
				{
					m_nChoNum = PtInRect(point);
					SetMouseCursor();
				}
				else
				{
					m_nChoNum = -1;
				}
			}
			else if(m_nProcess==0 && m_bSet)
			{
				//�����ڽ� ������ �ƴϸ� ����
				if(m_nChoNum<0)
					return;

				//�����ڽ� �����ϰ��
				switch(m_nChoNum)
				{
					case 0://���� ��� ���ý�
							m_rectTrack.left=point.x;
							m_rectTrack.top=point.y;
							break;
					case 1://���� ��� ���ý� 
							m_rectTrack.right=point.x;
							m_rectTrack.top=point.y;
							break;
					case 2://���� �ϴ� ���ý�
							m_rectTrack.right=point.x;
							m_rectTrack.bottom=point.y;
							break;
					case 3://���� �ϴ� ���ý�
							m_rectTrack.left=point.x;
							m_rectTrack.bottom=point.y;
							break;
					case 4://�������� 
							m_rectTrack.top=point.y;
							break;
					case 5://�Ʒ���
							m_rectTrack.bottom=point.y;
							break;
					case 6://��������
							m_rectTrack.left=point.x;
							break;
					case 7://��������
							m_rectTrack.right=point.x;
							break;
							//�̵��ϰ��
					case 8:case 9:case 10:case 11:
						{
							CPoint def;
							def=point-m_OldPoint;
							m_rectTrack.top+=def.y;
							m_rectTrack.bottom+=def.y;
							m_rectTrack.left+=def.x;
							m_rectTrack.right+=def.x;
							m_OldPoint=point;
						}
							break;
					//��ü������ ���ϴ� 13�� ��쿡�� �ƹ� ���۵�  ���� �ʴ´�
				}
				CDC *pDC=m_pParentWnd->GetDC();
				CPen pen,*oldPen;
				pDC->SetROP2(R2_XORPEN);
				//���� �׷��� ���� �����
				pen.CreatePen(PS_DOT,2,RGB(127, 127, 127));
				oldPen=pDC->SelectObject(&pen);
				pDC->MoveTo(m_rectOld.left,m_rectOld.top);
				pDC->LineTo(m_rectOld.right,m_rectOld.top);
				pDC->LineTo(m_rectOld.right,m_rectOld.bottom);
				pDC->LineTo(m_rectOld.left,m_rectOld.bottom);
				pDC->LineTo(m_rectOld.left,m_rectOld.top);
				//���ο� ������ �׸���.
				pDC->MoveTo(m_rectTrack.left,m_rectTrack.top);
				pDC->LineTo(m_rectTrack.right,m_rectTrack.top);
				pDC->LineTo(m_rectTrack.right,m_rectTrack.bottom);
				pDC->LineTo(m_rectTrack.left,m_rectTrack.bottom);
				pDC->LineTo(m_rectTrack.left,m_rectTrack.top);
				pDC->SelectObject(oldPen);
				pen.DeleteObject();
				m_rectOld=m_rectTrack;
				m_pParentWnd->ReleaseDC(pDC);

				m_dRectL = m_rectTrack.left;
				m_dRectT = m_rectTrack.top;
				m_dRectR = m_rectTrack.right;
				m_dRectB = m_rectTrack.bottom;
			}
			break;
		case 1:
			if(m_bDown && m_nProcess!=0)
			{
				m_rectTrack.right=point.x;
				m_rectTrack.bottom=point.y;
				CDC *pDC=m_pParentWnd->GetDC();
				CPen pen,*oldPen;
				pDC->SetROP2(R2_XORPEN);
				pen.CreatePen(PS_DOT,2,RGB(127, 127, 127));
				oldPen=pDC->SelectObject(&pen);
				pDC->MoveTo(m_rectOld.left,m_rectOld.top);
				pDC->LineTo(m_rectOld.right,m_rectOld.bottom);

				pDC->MoveTo(m_rectTrack.left,m_rectTrack.top);
				pDC->LineTo(m_rectTrack.right,m_rectTrack.bottom);
				pDC->SelectObject(oldPen);
				pen.DeleteObject();
				m_rectOld=m_rectTrack;
				m_pParentWnd->ReleaseDC(pDC);
			}
			if(m_nProcess==0 && !m_bDown && m_bSet)
			{
				if(m_bEnable)
				{
					m_nChoNum=PtInRect(point);
					SetMouseCursor();
				}
				else
				{
					m_nChoNum = -1;
				}
			}
			else if(m_nProcess==0 && m_bSet)
			{
				//�����ڽ� ������ �ƴϸ� ����
				if(m_nChoNum<0)
					return;
				//�����ڽ� �����ϰ��
				switch(m_nChoNum)
				{
					case 0://���� ��� ���ý�
							m_rectTrack.left=point.x;
							m_rectTrack.top=point.y;
							break;
					case 1://���� ��� ���ý� 
							m_rectTrack.right=point.x;
							m_rectTrack.top=point.y;
							break;
					case 2://���� �ϴ� ���ý�
							m_rectTrack.right=point.x;
							m_rectTrack.bottom=point.y;
							break;
					case 3://���� �ϴ� ���ý�
							m_rectTrack.left=point.x;
							m_rectTrack.bottom=point.y;
							break;
					case 4://�������� 
							m_rectTrack.top=point.y;
							break;
					case 5://�Ʒ���
							m_rectTrack.bottom=point.y;
							break;
					case 6://��������
							m_rectTrack.left=point.x;
							break;
					case 7://��������
							m_rectTrack.right=point.x;
							break;
							//�̵��ϰ��
					case 8:case 9:case 10:case 11:
						{
							CPoint def;
							def=point-m_OldPoint;
							m_rectTrack.top+=def.y;
							m_rectTrack.bottom+=def.y;
							m_rectTrack.left+=def.x;
							m_rectTrack.right+=def.x;
							m_OldPoint=point;
						}
							break;
					//��ü������ ���ϴ� 13�� ��쿡�� �ƹ� ���۵�  ���� �ʴ´�
				}
				CDC *pDC=m_pParentWnd->GetDC();
				CPen pen,*oldPen;
				pDC->SetROP2(R2_XORPEN);
				//���� �׷��� ���� �����
				pen.CreatePen(PS_DOT,2,RGB(127, 127, 127));
				oldPen=pDC->SelectObject(&pen);
				pDC->MoveTo(m_rectOld.left,m_rectOld.top);
				pDC->LineTo(m_rectOld.right,m_rectOld.bottom);

				//���ο� ������ �׸���.
				pDC->MoveTo(m_rectTrack.left,m_rectTrack.top);
				pDC->LineTo(m_rectTrack.right,m_rectTrack.bottom);
				pDC->SelectObject(oldPen);
				pen.DeleteObject();
				m_rectOld=m_rectTrack;
				m_pParentWnd->ReleaseDC(pDC);
			}
			break;
		default:
			AfxMessageBox("Error : invalid index(OnMouseMove)!");
			break;
	}
}
//���콺 ��ư ��������
void C_CtrlTracker::OnLButtonDown(UINT nFlags, CPoint point) 
{
	if(!m_bActive)
	{
		return;
	}

	//���࿡ ���� �ƹ��͵� �����Ǿ� �����ʰ�
	//���콺��ư�� ����
	//m_OldPoint�� point�� ����
	m_OldPoint=point;
	m_bDown=TRUE;
}
//���콺 ��ư ����
void C_CtrlTracker::OnLButtonUp(UINT nFlags, CPoint point) 
{
	CDC *pDC;
	CPen pen,*oldPen;
	
	if(!m_bActive)
	{
		return;
	}
	
	switch(m_iType)
	{
	case 0:
	case 2:
		pDC=m_pParentWnd->GetDC();
		pDC->SetROP2(R2_XORPEN);
		pen.CreatePen(PS_DOT,2,m_cColor);
		oldPen=pDC->SelectObject(&pen);
		pDC->MoveTo(m_rectOld.left,m_rectOld.top);
		pDC->LineTo(m_rectOld.right,m_rectOld.top);
		pDC->LineTo(m_rectOld.right,m_rectOld.bottom);
		pDC->LineTo(m_rectOld.left,m_rectOld.bottom);
		pDC->LineTo(m_rectOld.left,m_rectOld.top);
		
		pDC->SelectObject(oldPen);
		pen.DeleteObject();
		m_pParentWnd->ReleaseDC(pDC);
		m_rectOld=CRect(0,0,0,0);
		m_bDown=FALSE;
		
		//���콺��ư�� up�ɶ� �簢�� ���� �� �Է�
		SetRect(m_rectTrack);
		break;
	case 1:
		pDC=m_pParentWnd->GetDC();
		pDC->SetROP2(R2_XORPEN);
		pen.CreatePen(PS_DOT,2,m_cColor);
		oldPen=pDC->SelectObject(&pen);
		pDC->MoveTo(m_rectOld.left,m_rectOld.top);
		pDC->LineTo(m_rectOld.right,m_rectOld.bottom);
		
		pDC->SelectObject(oldPen);
		pen.DeleteObject();
		m_pParentWnd->ReleaseDC(pDC);
		m_rectOld=CRect(0,0,0,0);
		m_bDown=FALSE;
		
		//���콺��ư�� up�ɶ� �簢�� ���� �� �Է�
		SetRect(m_rectTrack);
		break;
	default:
		AfxMessageBox("Error : invalid index(OnLButtonUp)!");
		break;
	}
	
}
//���μ��� �� ����(����� �ʿ� ����, ����Ѵٸ� �׻� �� ���� 0���� �ٲ���� ��)
void C_CtrlTracker::SetProcess(int num)
{
	m_nProcess=num;
}
//���� ������ ���μ��� �� ����
int C_CtrlTracker::GetProcess()
{
	return m_nProcess;
}
//12���� ������ ����� �ش�.
void C_CtrlTracker::MakeDirect()
{
	int line_dot = m_nLineDot;

	switch(m_iType)
	{
		case 0:
		case 2:
			m_rect=CRect(min(m_rect.left,m_rect.right),
						 min(m_rect.top,m_rect.bottom),
						 max(m_rect.left,m_rect.right),
						 max(m_rect.top,m_rect.bottom));
			break;
		case 1:

			break;
		default:
			AfxMessageBox("Error : invalid index(MakeDirect)!");
			break;
	}
	m_rectTrack=m_rect;

	int pos;
	m_rectDirect[0]=CRect(m_rect.left-m_nLineDot,m_rect.top-m_nLineDot,
		m_rect.left+m_nLineDot,m_rect.top+m_nLineDot);
	m_rectDirect[1]=CRect(m_rect.right-m_nLineDot,m_rect.top-m_nLineDot,
			m_rect.right+m_nLineDot,m_rect.top+m_nLineDot);

	m_rectDirect[2]=CRect(m_rect.right-m_nLineDot,m_rect.bottom-m_nLineDot,
		m_rect.right+m_nLineDot,m_rect.bottom+m_nLineDot);

	m_rectDirect[3]=CRect(m_rect.left-m_nLineDot,m_rect.bottom-m_nLineDot,
		m_rect.left+m_nLineDot,m_rect.bottom+m_nLineDot);

	pos=(m_rect.Width()/2)+m_rect.TopLeft().x; 
	m_rectDirect[4]=CRect(pos-m_nLineDot,m_rect.top-m_nLineDot,
		pos+m_nLineDot,m_rect.top+m_nLineDot);
	pos=(m_rect.Width()/2)+m_rect.TopLeft().x; 
	m_rectDirect[5]=CRect(pos-m_nLineDot,m_rect.bottom-m_nLineDot,
		pos+m_nLineDot,m_rect.bottom+m_nLineDot);
	pos=(m_rect.Height()/2)+m_rect.TopLeft().y; 
	m_rectDirect[6]=CRect(m_rect.left-m_nLineDot,pos-m_nLineDot,
		m_rect.left+m_nLineDot,pos+m_nLineDot);
	m_rectDirect[7]=CRect(m_rect.right-m_nLineDot,pos-m_nLineDot,
		m_rect.right+m_nLineDot,pos+m_nLineDot);

	line_dot = line_dot * 2;
	m_rectDirect[8]=CRect(m_rect.left,m_rect.top-line_dot,
		m_rect.right,m_rect.top+line_dot);
	m_rectDirect[9]=CRect(m_rect.left-line_dot,m_rect.top,
		m_rect.left+line_dot,m_rect.bottom);
	m_rectDirect[10]=CRect(m_rect.right-line_dot,m_rect.top,
		m_rect.right+line_dot,m_rect.bottom);
	m_rectDirect[11]=CRect(m_rect.left,m_rect.bottom-line_dot,
		m_rect.right,m_rect.bottom+line_dot);
}

//�������� ������ �����Ѵ�.
void C_CtrlTracker::SetRect(CRect rect)
{
	m_rect=rect;
	m_rectTrack=rect;
}

//�������� ������ ��´�
CRect C_CtrlTracker::GetRect()
{
	return m_rectTrack;
}

//�� �� ����
void C_CtrlTracker::SetColor(COLORREF color)
{
	m_cColor = color;
}

//���� ����
void C_CtrlTracker::SetCaption(char *caption)
{
	m_sCaption = caption;
}

//���� Ȱ��ȭ ����
void C_CtrlTracker::SetEnable(BOOL enable)
{
	m_bEnable = enable;
}

//Ȱ��ȭ ���� ���
BOOL C_CtrlTracker::GetEnable()
{
	return m_bEnable;
}

//���� ���� ���
int C_CtrlTracker::GetType()
{
	return m_iType;
}

//���콺 ��ư ���� Ŭ�� ������(���� �ȿ� ����Ʈ�� ������ TRUE��ȯ)
BOOL C_CtrlTracker::OnLButtonDblClk(UINT nFlags, CPoint point) 
{
	if(!m_bActive)
	{
		return FALSE;
	}

	if(0 > PtInRect(point))
	{
		return FALSE;
	}


	return TRUE;
}

//�׷�ȭ ����
void C_CtrlTracker::SetEnableGroup(BOOL enable)
{
	m_bGroup = enable;
	SetClassLong(m_pParentWnd->m_hWnd,GCL_HCURSOR,(long)LoadCursor(NULL,IDC_ARROW));
}

//�׷���� ���
BOOL C_CtrlTracker::GetGroup()
{
	return m_bGroup;
}

void C_CtrlTracker::Draw(int idx)
{
	CDC *pDC;
	CPen pen,*oldPen;
	CBrush brush,*oldBrush;
	CString caption;

	if(!m_bActive)
	{
		return;
	}

	pDC=m_pParentWnd->GetDC();
	switch(m_iType)
	{
		//Rectangle
		case 0:
			if(m_rect.left==0 && m_rect.top==0 && m_rect.bottom==0 && m_rect.right==0)
				return;
			//8�� ������ ������ �����Ѵ�.
			MakeDirect();
			//8�� ������ ������ �����Ǹ� m_bSet�� TRUE��
			m_bSet=TRUE;
			//�����ڽ��� ���� �ڽ��� �׸���.
			pen.CreatePen(PS_DOT,m_nLindTick,m_cColor);
			oldPen=pDC->SelectObject(&pen);
			pDC->MoveTo(m_rect.left,m_rect.top);
			pDC->LineTo(m_rect.right,m_rect.top);
			pDC->LineTo(m_rect.right,m_rect.bottom);
			pDC->LineTo(m_rect.left,m_rect.bottom);
			pDC->LineTo(m_rect.left,m_rect.top);
			pDC->SelectObject(oldPen);
			pen.DeleteObject();
			brush.CreateSolidBrush(m_cColor);
			oldBrush=pDC->SelectObject(&brush);
			//8�� ������ �ڽ��� �׸���.
			if(m_bEnable)
			{
				for(int i=0;i<8;i++)
					pDC->Rectangle(m_rectDirect[i]);
			}

			//���ڿ��� �׸���
			pDC->SetTextColor(m_cColor);
			pDC->SetBkMode(TRANSPARENT);
			caption.Format("%s", (LPCTSTR)m_sCaption);
			pDC->TextOut(m_rect.left, m_rect.bottom + m_nLineDot, caption);

			pDC->SelectObject(oldBrush);
			brush.DeleteObject();
			break;
		//Line
		case 1:
			if(m_rect.left==0 && m_rect.top==0 && m_rect.bottom==0 && m_rect.right==0)
				return;
			//8�� ������ ������ �����Ѵ�.
			MakeDirect();
			//8�� ������ ������ �����Ǹ� m_bSet�� TRUE��
			m_bSet=TRUE;
			//�����ڽ��� ���� �ڽ��� �׸���.
			pen.CreatePen(PS_DOT,m_nLindTick,m_cColor);
			oldPen=pDC->SelectObject(&pen);
			pDC->MoveTo(m_rect.left,m_rect.top);
			pDC->LineTo(m_rect.right,m_rect.bottom);
			pDC->SelectObject(oldPen);
			pen.DeleteObject();
			brush.CreateSolidBrush(m_cColor);
			oldBrush=pDC->SelectObject(&brush);
			//2�� ������ �ڽ��� �׸���(2���� ����Ѵ�).
			if(m_bEnable)
			{
				pDC->Rectangle(m_rectDirect[0]);
				pDC->Rectangle(m_rectDirect[2]);
			}

			//���ڿ��� �׸���
			pDC->SetTextColor(m_cColor);
			pDC->SetBkMode(TRANSPARENT);
			caption.Format("%d-%s", idx + 1, (LPCTSTR)m_sCaption);
			pDC->TextOut(m_rect.right, m_rect.bottom + m_nLineDot, caption);

			pDC->SelectObject(oldBrush);
			brush.DeleteObject();
			break;
		//Ellipse
		case 2:
			if(m_rect.left==0 && m_rect.top==0 && m_rect.bottom==0 && m_rect.right==0)
				return;
			//8�� ������ ������ �����Ѵ�.
			MakeDirect();
			//8�� ������ ������ �����Ǹ� m_bSet�� TRUE��
			m_bSet=TRUE;
			//�����ڽ��� ���� �ڽ��� �׸���.
			pen.CreatePen(PS_DOT,m_nLindTick,m_cColor);
			oldPen=pDC->SelectObject(&pen);
			pDC->MoveTo(m_rect.left,m_rect.top);
			pDC->LineTo(m_rect.right,m_rect.top);
			pDC->LineTo(m_rect.right,m_rect.bottom);
			pDC->LineTo(m_rect.left,m_rect.bottom);
			pDC->LineTo(m_rect.left,m_rect.top);
			
			oldBrush = (CBrush *)pDC->SelectStockObject(NULL_BRUSH);
			pDC->Ellipse(m_rect);
			pDC->SelectObject(oldBrush);

			pDC->SelectObject(oldPen);
			pen.DeleteObject();

			brush.CreateSolidBrush(m_cColor);
			oldBrush=pDC->SelectObject(&brush);

			//8�� ������ �ڽ��� �׸���.
			if(m_bEnable)
			{
				for(int i=0;i<8;i++)
					pDC->Rectangle(m_rectDirect[i]);
			}

			//���ڿ��� �׸���
			pDC->SetTextColor(m_cColor);
			pDC->SetBkMode(TRANSPARENT);
			caption.Format("%d-%s", idx + 1, (LPCTSTR)m_sCaption);
			pDC->TextOut(m_rect.left, m_rect.bottom + m_nLineDot, caption);

			pDC->SelectObject(oldBrush);
			brush.DeleteObject();
			break;
		default:
			AfxMessageBox("Error : invalid index(Draw)!");
			break;
	}
	m_pParentWnd->ReleaseDC(pDC);
}

void C_CtrlTracker::SetActive(BOOL active)
{
	m_bActive = active;
}

void C_CtrlTracker::SetZoom(double start_x, double start_y, double end_x, double end_y)
{
	m_dZoomStartX = start_x;
	m_dZoomStartY = start_y;
	m_dZoomEndX = end_x;
	m_dZoomEndY = end_y;
}

void C_CtrlTracker::GetZoom(double *start_x, double *start_y, double *end_x, double *end_y)
{
	*start_x = m_dZoomStartX;
	*start_y = m_dZoomStartY;
	*end_x = m_dZoomEndX;
	*end_y = m_dZoomEndY;
}

void C_CtrlTracker::GetRect(double *left, double *top, double *right, double *bottom)
{
	*left = m_dRectL; 
	*top = m_dRectT; 
	*right = m_dRectR; 
	*bottom = m_dRectB; 
}

void C_CtrlTracker::SetRect(double left, double top, double right, double bottom)
{
	m_dRectL = left;
	m_dRectT = top;
	m_dRectR = right;
	m_dRectB = bottom;

	m_rect=CRect((int)left, (int)top, (int)right, (int)bottom);
	m_rectTrack=CRect((int)left, (int)top, (int)right, (int)bottom);
}