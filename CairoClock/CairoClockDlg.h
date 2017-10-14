
// CairoClockDlg.h : ͷ�ļ�
//

#pragma once


// CCairoClockDlg �Ի���
class CCairoClockDlg : public CDialogEx
{
// ����
public:
	CCairoClockDlg(CWnd* pParent = NULL);	// ��׼���캯��

// �Ի�������
	enum { IDD = IDD_CAIROCLOCK_DIALOG };

	enum { CAIROCLOCK };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV ֧��

	void DrawClock();
// ʵ��
protected:
	HICON m_hIcon;
	BOOL bIntHour;
	BOOL bGoOffWork;

	// ���ɵ���Ϣӳ�亯��
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnTimer(UINT_PTR nIDEvent);
};
