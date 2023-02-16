//Tracker
class C_CtrlTracker{
protected:
	//�׷�ȭ
	BOOL m_bGroup;
	int m_iType;//���� ���� 0: rectangle(no angle), 1: line(angle), 2: circle, 3: ellipse
	CString m_sCaption;//���� ����
	BOOL m_bEnable;//Ȱ��ȭ ����
	COLORREF m_cColor;//�� ��
	CWnd *m_pParentWnd;//�θ� ������ �ڵ鷯
	BOOL m_bDown;//���콺 ��ư �÷��� 
	CRect m_rectOld;//���� ���� ��ǥ ���� ����
	//���콺�� �����ڽ����� 8������� �̵� ��ġ�� ������� �ִ°��� 
	//���� �����ϰ� �ִ� ����
	int m_nChoNum;
	//���� ���μ����� �����ϰ� �ִ� ����
	int m_nProcess;
	//8�� ������ ��ǥ�� 4�������� �̵���ǥ ���� ����
	CRect m_rectDirect[12];
	//���� ���콺 ������ ����
	CPoint m_OldPoint;
	//m_rect������ �����Ǹ� �̿����� ���� m_rectDirect�� ������ ������ִ� 
	//�Լ�
	void MakeDirect();
	//8�� ���� �ڽ� ���� ������ �󸶳� �β���� �����ϴ� ����
	int m_nLineDot;
	//�����ڽ� ������ �β� 
	int m_nLindTick;
	//m_rectDirect������ �����Ǿ� ������ TRUE�׷��� ������ FALSE
	int m_bSet;

	//Zoom
	double m_dZoomStartX;
	double m_dZoomStartY;
	double m_dZoomEndX;
	double m_dZoomEndY;

	//Rectangle//////////////////////////////////
	//�̵��Ǵ� ���� ��ǥ ���� ����
	CRect m_rectTrack;
	//���� �����ڽ��� ��ǥ
	CRect m_rect;
	BOOL m_bActive;
	double m_dRectL;
	double m_dRectT;
	double m_dRectR;
	double m_dRectB;


public:
	void GetZoom(double *start_x, double *start_y, double *end_x, double *end_y);
	void SetZoom(double start_x, double start_y, double end_x, double end_y);
	void GetRect(double *left, double *top, double *right, double *bottom);
	void SetRect(double left, double top, double right, double bottom);

	void SetActive(BOOL active);
	void Draw(int idx);
	BOOL GetGroup();
	void SetEnableGroup(BOOL enable);

	int GetType();
	BOOL GetEnable();
	void SetEnable(BOOL enable);
	void SetCaption(char* caption);
	void SetColor(COLORREF color);
	C_CtrlTracker();
	C_CtrlTracker(CWnd *pWnd, int type = 0, int dot = 2, int tick = 3);

	void OnMouseMove(UINT nFlags, CPoint point);
	void OnLButtonDown(UINT nFlags, CPoint point);
	void OnLButtonUp(UINT nFlags, CPoint point);
	void SetProcess(int num);
	int  GetProcess();

	int PtInRect(CPoint point);
	void SetMouseCursor();
	int GetChoNum(){return m_nChoNum;}
	BOOL OnLButtonDblClk(UINT nFlags, CPoint point);

	CRect GetRect();
	void SetRect(CRect rect);
};