
// CairoClockDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "CairoClock.h"
#include "CairoClockDlg.h"
#include "afxdialogex.h"

#include <cairommconfig.h>
#include <cairomm/context.h>
#include <cairomm/surface.h>
#include <cairomm/win32_surface.h>

#include <boost/date_time/posix_time/posix_time.hpp>
#include <boost/date_time/gregorian/gregorian.hpp>
using namespace boost::posix_time;
using namespace boost::gregorian;

#include <boost/format.hpp>
using boost::format;

#include <wykobi.hpp>
#include "ConfigDlg.h"
using namespace wykobi;

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

#include <string>
#include <vector>

using std::string;
using std::vector;

inline string utf8_trans_impl(string const& from, int from_code, int to_code)
{
	int len16 = MultiByteToWideChar(from_code, 0, from.c_str(), from.size(), 0, 0);
	if (len16 > 0)
	{
		vector<wchar_t> ucs2_buf(len16, 0);
		len16 = MultiByteToWideChar(from_code, 0, from.c_str(), from.size(), &ucs2_buf[0], len16);
		int len8 = WideCharToMultiByte(to_code, 0, &ucs2_buf[0], len16, 0, 0, 0, 0);
		string result(len8, 0);
		WideCharToMultiByte(to_code, 0, &ucs2_buf[0], len16, &result[0], len8, 0, 0);
		return result;
	}
	return string();
}

inline string from_utf8(string const& from)
{
	return utf8_trans_impl(from, CP_UTF8, CP_ACP);
}

inline string to_utf8(string const&from)
{
	return utf8_trans_impl(from, CP_ACP, CP_UTF8);
}

// ����Ӧ�ó��򡰹��ڡ��˵���� CAboutDlg �Ի���

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// �Ի�������
	enum { IDD = IDD_ABOUTBOX };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

// ʵ��
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialogEx(CAboutDlg::IDD)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
END_MESSAGE_MAP()


// CCairoClockDlg �Ի���




CCairoClockDlg::CCairoClockDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CCairoClockDlg::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
	bIntHour = TRUE;
	bGoOffWork = TRUE;
}

void CCairoClockDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CCairoClockDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_WM_TIMER()
END_MESSAGE_MAP()


// CCairoClockDlg ��Ϣ�������

BOOL CCairoClockDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// ��������...���˵�����ӵ�ϵͳ�˵��С�

	// IDM_ABOUTBOX ������ϵͳ���Χ�ڡ�
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		BOOL bNameValid;
		CString strAboutMenu;
		bNameValid = strAboutMenu.LoadString(IDS_ABOUTBOX);
		ASSERT(bNameValid);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			CString config = _T("����");
			pSysMenu->AppendMenuW(MF_STRING, IDM_CONFIGBOX, config);
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// ���ô˶Ի����ͼ�ꡣ��Ӧ�ó��������ڲ��ǶԻ���ʱ����ܽ��Զ�
	//  ִ�д˲���
	SetIcon(m_hIcon, TRUE);			// ���ô�ͼ��
	SetIcon(m_hIcon, FALSE);		// ����Сͼ��

	// TODO: �ڴ���Ӷ���ĳ�ʼ������
	SetTimer(CAIROCLOCK, 10, NULL);

	return TRUE;  // ���ǽ��������õ��ؼ������򷵻� TRUE
}

void CCairoClockDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else if ((nID & 0xFFF0) == IDM_CONFIGBOX)
	{
		CConfigDlg dlg;
		dlg.m_bIntHour = bIntHour;
		dlg.m_bGoOffWork = bGoOffWork;
		if (dlg.DoModal() == IDOK)
		{
			bIntHour = dlg.m_bIntHour;
			bGoOffWork = dlg.m_bGoOffWork;
		}
	}
	else
	{
		CDialogEx::OnSysCommand(nID, lParam);
	}
}

// �����Ի��������С����ť������Ҫ����Ĵ���
//  �����Ƹ�ͼ�ꡣ����ʹ���ĵ�/��ͼģ�͵� MFC Ӧ�ó���
//  �⽫�ɿ���Զ���ɡ�

void CCairoClockDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // ���ڻ��Ƶ��豸������

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// ʹͼ���ڹ����������о���
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// ����ͼ��
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

//���û��϶���С������ʱϵͳ���ô˺���ȡ�ù��
//��ʾ��
HCURSOR CCairoClockDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

void CCairoClockDlg::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: �ڴ������Ϣ�����������/�����Ĭ��ֵ
	if (nIDEvent == CAIROCLOCK)
	{
		DrawClock();
		CTime time = CTime::GetCurrentTime();
		int hour = time.GetHour();
		int minute = time.GetMinute();
		int second = time.GetSecond();
		int dayOfWeek = time.GetDayOfWeek();

		if (bIntHour)
		{
			if (minute == 0 && second == 0)
			{
				LockWorkStation();
			}
		}

		if (bGoOffWork)
		{
			if (dayOfWeek >= 2 && dayOfWeek <= 5)//��һ������
			{
				if (hour == 11 && minute == 50 && second == 0)
				{
					LockWorkStation();
					::PostMessage(HWND_BROADCAST, WM_SYSCOMMAND, SC_MONITORPOWER, 2);
				}

				if (hour == 18 && minute == 0 && second == 0)
				{
					LockWorkStation();
					::PostMessage(HWND_BROADCAST, WM_SYSCOMMAND, SC_MONITORPOWER, 2);
				}
			}
			else if (dayOfWeek == 6)//����
			{
				if (hour == 11 && minute == 50 && second == 0)
				{
					LockWorkStation();
					::PostMessage(HWND_BROADCAST, WM_SYSCOMMAND, SC_MONITORPOWER, 2);
				}

				if (hour == 17 && minute == 0 && second == 0)
				{
					LockWorkStation();
					::PostMessage(HWND_BROADCAST, WM_SYSCOMMAND, SC_MONITORPOWER, 2);
				}
			}
		}
	}

	CDialogEx::OnTimer(nIDEvent);
}

void CCairoClockDlg::DrawClock()
{
	CClientDC dc(this);
	CRect clientRect;
	GetClientRect(clientRect);
	Cairo::RefPtr<Cairo::Win32Surface> surface = Cairo::Win32Surface::create_with_dib(Cairo::Format::FORMAT_ARGB32, clientRect.Width(), clientRect.Height());
	Cairo::RefPtr<Cairo::Context> cr = Cairo::Context::create(surface);

	double radius = min(clientRect.Width()/2, clientRect.Height()/2);
	CPoint center = CPoint(clientRect.Width()/2, clientRect.Height()/2);

	cr->set_source_rgb(1, 1, 1);
	cr->paint();

	cr->set_source_rgba(0.33, 0.76, 0.95, 1);
	cr->arc(center.x, center.y, radius, 0, 2*PI);
	cr->fill();

	ptime now = microsec_clock::local_time();
	time_duration td = now.time_of_day();
	td.unit();
	int hour = td.hours();
	if (hour > 12)
	{
		hour = hour-12;
	}
	int minute = td.minutes();
	int second = td.seconds();
	int ms = td.fractional_seconds();
	date today = now.date();

	cr->set_source_rgba(1, 1, 1, 1);
	double x = center.x+radius/2-radius/9.5;
	double y = center.y+radius/10-radius/5;
	double width = radius/4;
	double height = radius/5;
	double aspect = 1.0;
	double corner_radius = height /10.0;
	double rect_radius = corner_radius / aspect;
	double degrees = PI / 180.0;
	cr->begin_new_sub_path();
	cr->arc(x + width - rect_radius, y + rect_radius, rect_radius, -90 * degrees, 0 * degrees);
	cr->arc(x + width - rect_radius, y + height - rect_radius, rect_radius, 0 * degrees, 90 * degrees);
	cr->arc(x + rect_radius, y + height - rect_radius, rect_radius, 90 * degrees, 180 * degrees);
	cr->arc(x + rect_radius, y + rect_radius, rect_radius, 180 * degrees, 270 * degrees);
	cr->close_path();
	cr->fill_preserve();
	cr->set_line_width(5);
	cr->set_source_rgba(0.5, 0, 0, 0.3);
	cr->stroke();

	cr->set_source_rgba(0, 0, 0, 1);
	cr->move_to(center.x+radius/2-radius/15, center.y+radius/15);
	cr->set_font_size(radius/6);
	cr->select_font_face(to_utf8("����"), Cairo::FontSlant::FONT_SLANT_NORMAL, Cairo::FontWeight::FONT_WEIGHT_NORMAL);
	string dayMonth;
	int day = today.day();
	if (day < 10)
	{
		dayMonth = str(format("0%d") % day);
	}
	else
	{
		dayMonth = str(format("%d") % day);
	}
 	cr->show_text(to_utf8(dayMonth));

	cr->set_source_rgba(1, 1, 1, 1);
	x = center.x-radius/2-radius/9.5;
	y = center.y-radius/10/*-radius/5*/;
	width = radius/4;
	height = radius/5;
	aspect = 1.0;
	corner_radius = height /10.0;
	rect_radius = corner_radius / aspect;
	degrees = PI / 180.0;
	cr->begin_new_sub_path();
	cr->arc(x + width - rect_radius, y + rect_radius, rect_radius, -90 * degrees, 0 * degrees);
	cr->arc(x + width - rect_radius, y + height - rect_radius, rect_radius, 0 * degrees, 90 * degrees);
	cr->arc(x + rect_radius, y + height - rect_radius, rect_radius, 90 * degrees, 180 * degrees);
	cr->arc(x + rect_radius, y + rect_radius, rect_radius, 180 * degrees, 270 * degrees);
	cr->close_path();
	cr->fill_preserve();
	cr->set_line_width(5);
	cr->set_source_rgba(0.5, 0, 0, 0.3);
	cr->stroke();

	cr->set_source_rgba(0, 0, 0, 1);
	cr->move_to(center.x-radius/2-radius/15, center.y+radius/15);
	cr->set_font_size(radius/6);
	cr->select_font_face(to_utf8("����"), Cairo::FontSlant::FONT_SLANT_NORMAL, Cairo::FontWeight::FONT_WEIGHT_NORMAL);
	string dayWeek;
	day = today.day_of_week();
	switch (day)
	{
	case 0:
		dayWeek = "��";
		break;
	case 1:
		dayWeek = "һ";
		break;
	case 2:
		dayWeek = "��";
		break;
	case 3:
		dayWeek = "��";
		break;
	case 4:
		dayWeek = "��";
		break;
	case 5:
		dayWeek = "��";
		break;
	case 6:
		dayWeek = "��";
		break;
	default:
		break;
	}
	cr->show_text(to_utf8(dayWeek));

	cr->move_to(center.x, center.y);
	cr->set_line_width(15);
	cr->set_source_rgba(1, 1, 1, 0.8);
	cr->arc(center.x, center.y, radius*0.5, hour/12.0*2*PI + minute/60.0*2*PI/12 - PI/2, hour/12.0*2*PI + minute/60.0*2*PI/12 - PI/2);
	cr->set_line_cap(Cairo::LineCap::LINE_CAP_ROUND);
	cr->stroke();

	cr->set_line_width(10);
	cr->set_source_rgba(1, 1, 1, 0.8);
	cr->move_to(center.x, center.y);
	cr->arc(center.x, center.y, radius*0.7, minute/60.0*2*PI - PI/2, minute/60.0*2*PI - PI/2);
	cr->set_line_cap(Cairo::LineCap::LINE_CAP_ROUND);
	cr->stroke();

	cr->set_line_width(5);
	cr->set_source_rgba(1.0, 0.96, 0.5, 0.8);
	cr->move_to(center.x, center.y);
	cr->arc(center.x, center.y, radius*0.7, (second + ms/1000000.0)/60.0*2*PI - PI/2, (second + ms/1000000.0)/60.0*2*PI - PI/2);
	cr->move_to(center.x, center.y);
	cr->arc(center.x, center.y, radius*0.2, (second + ms/1000000.0)/60.0*2*PI - PI/2 + PI, (second + ms/1000000.0)/60.0*2*PI - PI/2 + PI);
	cr->set_line_cap(Cairo::LineCap::LINE_CAP_ROUND);
	cr->stroke();

	cr->set_source_rgba(1.0, 0.96, 0.5, 1);
	cr->arc(center.x, center.y, 15, 0, 2*PI);
	cr->fill();

	for (int i=0; i<12; i++)
	{
		cr->save();
		cr->set_line_cap(Cairo::LineCap::LINE_CAP_BUTT);
		cr->set_line_width(10);
		cr->set_source_rgba(1.0, 0.96, 0.5, 1);
		point2d<double> movePt = wykobi::rotate(360*i/12.0, make_point<double>(center.x, center.y + radius*0.8), make_point<double>(center.x, center.y));
		point2d<double> linePt = wykobi::rotate(360*i/12.0, make_point<double>(center.x, center.y + radius*0.9), make_point<double>(center.x, center.y));
		cr->move_to(movePt.x, movePt.y);
		cr->line_to(linePt.x, linePt.y);
		cr->stroke();
		cr->restore();
	}

	for (int i=0; i<60; i++)
	{
		cr->save();
		cr->set_line_cap(Cairo::LineCap::LINE_CAP_BUTT);
		cr->set_line_width(5);
		cr->set_source_rgba(1.0, 0.96, 0.5, 1);
		point2d<double> movePt = wykobi::rotate(360*i/60.0, make_point<double>(center.x, center.y + radius*0.85), make_point<double>(center.x, center.y));
		point2d<double> linePt = wykobi::rotate(360*i/60.0, make_point<double>(center.x, center.y + radius*0.9), make_point<double>(center.x, center.y));
		cr->move_to(movePt.x, movePt.y);
		cr->line_to(linePt.x, linePt.y);
		cr->stroke();
		cr->restore();
	}

	HDC src = surface->get_dc();
	CDC srcDC;
	srcDC.Attach(src);
	dc.BitBlt(0, 0, clientRect.Width(), clientRect.Height(), &srcDC, 0, 0, SRCCOPY);
}