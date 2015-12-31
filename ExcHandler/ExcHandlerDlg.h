
// ExcHandlerDlg.h : header file
//

#pragma once
#include "afxwin.h"
#include "PictureCtrl.h"

// CExcHandlerDlg dialog
class CExcHandlerDlg : public CDialogEx
{
// Construction
public:
	CExcHandlerDlg(CWnd* pParent = NULL);	// standard constructor

// Dialog Data
	enum { IDD = IDD_EXCHANDLER_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support


// Implementation
protected:
	HICON m_hIcon;

	// Generated message map functions
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	CPictureCtrl m_PictureCtrl;
	afx_msg void OnDropFiles(HDROP hDropInfo);
	void HandleExc(CString file_path);
};
