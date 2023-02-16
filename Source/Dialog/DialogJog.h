#pragma once


// CDialogJog 대화 상자입니다.

class CDialogJog : public CDialogEx
{
	DECLARE_DYNAMIC(CDialogJog)

public:
	CDialogJog(CWnd* pParent = NULL);   // 표준 생성자입니다.
	virtual ~CDialogJog();

// 대화 상자 데이터입니다.
	enum { IDD = IDD_DIALOG_JOG };

public:
	BOOL GetStepToJog();
protected:
	BOOL m_bStepToJog;
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()
public:
	DECLARE_EVENTSINK_MAP()
	void MouseDownBtnenhctrlJogLeft(short Button, short Shift, long x, long y);
	void MouseDownBtnenhctrlJogRight(short Button, short Shift, long x, long y);
	void MouseDownBtnenhctrlJogTop(short Button, short Shift, long x, long y);
	void MouseDownBtnenhctrlJogBottom(short Button, short Shift, long x, long y);
	void MouseDownBtnenhctrlJogUp(short Button, short Shift, long x, long y);
	void MouseDownBtnenhctrlJogDown(short Button, short Shift, long x, long y);
	virtual BOOL OnInitDialog();
	CBtnEnh m_btn_StepToJog;
	void MouseUpBtnenhctrlJogLeft(short Button, short Shift, long x, long y);
	void MouseUpBtnenhctrlJogRight(short Button, short Shift, long x, long y);
	void MouseUpBtnenhctrlJogTop(short Button, short Shift, long x, long y);
	void MouseUpBtnenhctrlJogBottom(short Button, short Shift, long x, long y);
	void MouseUpBtnenhctrlJogUp(short Button, short Shift, long x, long y);
	void MouseUpBtnenhctrlJogDown(short Button, short Shift, long x, long y);
	void ClickBtnenhctrlJogLeft();
	void ClickBtnenhctrlJogRight();
	void ClickBtnenhctrlJogTop();
	void ClickBtnenhctrlJogBottom();
	void ClickBtnenhctrlJogUp();
	void ClickBtnenhctrlJogDown();
	long m_edit_lSpeed;
	double m_edit_dStep;
	void ClickBtnenhctrlStepToJog();
	virtual BOOL PreTranslateMessage(MSG* pMsg);
};
