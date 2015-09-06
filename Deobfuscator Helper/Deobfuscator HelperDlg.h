
// Deobfuscator HelperDlg.h : header file
//

#pragma once


// CDeobfuscatorHelperDlg dialog
class CDeobfuscatorHelperDlg : public CDialog
{
// Construction
public:
	CDeobfuscatorHelperDlg(CWnd* pParent = NULL);	// standard constructor

// Dialog Data
	enum { IDD = IDD_DEOBFUSCATORHELPER_DIALOG };

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
	afx_msg void OnBnClickedOk();
	afx_msg void OnBnClickedPaste();
	afx_msg void OnBnClickedGo();
	afx_msg void OnBnClickedCopy();
	afx_msg void OnBnClickedAbout();
};
