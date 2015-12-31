
// ExcHandlerDlg.cpp : implementation file
//

#include "stdafx.h"
#include "ExcHandler.h"
#include "ExcHandlerDlg.h"
#include "afxdialogex.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CAboutDlg dialog used for App About

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

	// Dialog Data
	enum { IDD = IDD_ABOUTBOX };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	// Implementation
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


// CExcHandlerDlg dialog



CExcHandlerDlg::CExcHandlerDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CExcHandlerDlg::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CExcHandlerDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_STATIC_PICTURE, m_PictureCtrl);
}

BEGIN_MESSAGE_MAP(CExcHandlerDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_WM_DROPFILES()
END_MESSAGE_MAP()


// CExcHandlerDlg message handlers

BOOL CExcHandlerDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// Add "About..." menu item to system menu.

	// IDM_ABOUTBOX must be in the system command range.
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
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// Set the icon for this dialog.  The framework does this automatically
	//  when the application's main window is not a dialog
	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon

	// TODO: Add extra initialization here

	//m_drag.ModifyStyle(0, BS_OWNERDRAW);

	CString sPath;
	GetModuleFileName(NULL, sPath.GetBufferSetLength(MAX_PATH + 1), MAX_PATH);
	sPath.ReleaseBuffer();
	int nPos;
	nPos = sPath.ReverseFind(_T('\\'));
	sPath = sPath.Left(nPos);

	sPath += L"\\drag-files-realsize.jpg";
	m_PictureCtrl.LoadFromFile(sPath);

	DragAcceptFiles(TRUE);
	return TRUE;  // return TRUE  unless you set the focus to a control
}

void CExcHandlerDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialogEx::OnSysCommand(nID, lParam);
	}
}

// If you add a minimize button to your dialog, you will need the code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework.

void CExcHandlerDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // device context for painting

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// Center icon in client rectangle
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// Draw the icon
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

// The system calls this function to obtain the cursor to display while the user drags
//  the minimized window.
HCURSOR CExcHandlerDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}



void CExcHandlerDlg::OnDropFiles(HDROP hDropInfo)
{
	// TODO: Add your message handler code here and/or call default
	wchar_t	szDroppedFile[MAX_PATH];
	HDROP	hDrop;
	int nFiles;
	CString csString;

	nFiles = DragQueryFile(hDropInfo,		// Struture Identifier
		-1,				// -1 to Drop more than one file
		szDroppedFile,	// Droped File Name
		MAX_PATH);		// Max char

	for (int i = 0; i < nFiles; i++)
	{
		DragQueryFile(hDropInfo, i, szDroppedFile, MAX_PATH);
		HandleExc(szDroppedFile);
	}
	CDialogEx::OnDropFiles(hDropInfo);
}


void CExcHandlerDlg::HandleExc(CString file_path)
{
	int nPos = file_path.ReverseFind(_T('.'));
	
	CString extend = file_path.Right(file_path.GetLength() - nPos);
	if (extend.CompareNoCase(L".exc") == 0) {
		CStdioFile file;
		CStdioFile file_bk;
		if (!file.Open(file_path.GetString(), CFile::modeRead)) {
			AfxMessageBox(L"Open Source exc fail");
			return;
		}
		
		if (!file_bk.Open((file_path + L"_bk").GetString(), CFile::modeCreate|CFile::modeWrite)) {
			AfxMessageBox(L"Open Back file failed");
			return;
		}
		
		CString line_str;
		BOOL add_plus = FALSE;

		while (file.ReadString(line_str)) {
			if (line_str.IsEmpty()) {
				add_plus = TRUE;
				file_bk.WriteString(L"\n");
				continue;
			}

			if (add_plus) {
				line_str = L"+" + line_str;
				add_plus = FALSE;
			}

			file_bk.WriteString(line_str.GetString());
			file_bk.WriteString(L"\n");
		}

		file.Close();
		file_bk.Close();



	}
}
