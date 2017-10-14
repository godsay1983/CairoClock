
// CairoClockDlg.h : 头文件
//

#pragma once


// CCairoClockDlg 对话框
class CCairoClockDlg : public CDialogEx
{
// 构造
public:
	CCairoClockDlg(CWnd* pParent = NULL);	// 标准构造函数

// 对话框数据
	enum { IDD = IDD_CAIROCLOCK_DIALOG };

	enum { CAIROCLOCK };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 支持

	void DrawClock();
// 实现
protected:
	HICON m_hIcon;
	BOOL bIntHour;
	BOOL bGoOffWork;

	// 生成的消息映射函数
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnTimer(UINT_PTR nIDEvent);
};
