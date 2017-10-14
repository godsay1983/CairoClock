// ConfigDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "CairoClock.h"
#include "ConfigDlg.h"
#include "afxdialogex.h"


// CConfigDlg 对话框

IMPLEMENT_DYNAMIC(CConfigDlg, CDialogEx)

CConfigDlg::CConfigDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CConfigDlg::IDD, pParent)
	, m_bIntHour(FALSE)
	, m_bGoOffWork(FALSE)
{

}

CConfigDlg::~CConfigDlg()
{
}

void CConfigDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Check(pDX, IDC_CHECK_INTHOUR, m_bIntHour);
	DDX_Check(pDX, IDC_CHECK_GOOFFWORK, m_bGoOffWork);
}


BEGIN_MESSAGE_MAP(CConfigDlg, CDialogEx)
	ON_BN_CLICKED(IDOK, &CConfigDlg::OnBnClickedOk)
END_MESSAGE_MAP()


// CConfigDlg 消息处理程序


BOOL CConfigDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  在此添加额外的初始化
	UpdateData(FALSE);

	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常: OCX 属性页应返回 FALSE
}


void CConfigDlg::OnBnClickedOk()
{
	// TODO: 在此添加控件通知处理程序代码
	UpdateData(TRUE);

	CDialogEx::OnOK();
}
