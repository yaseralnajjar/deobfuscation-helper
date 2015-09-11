
// Deobfuscator HelperDlg.cpp : implementation file
//

#include "stdafx.h"
#include "Deobfuscator Helper.h"
#include "Deobfuscator HelperDlg.h"
#include <stdexcept>

#ifdef _DEBUG
#define new DEBUG_NEW
#endif
// CAboutDlg dialog used for App About

//defs---------------------------------------------------------------------
int ReachBytes(CString sArg);
CString GetBytesArray(CString sArg, int ReachedByte);
CString AddWildCards(CString sArg);
CString TakeOutSpaces(CString sArg, CButton* ChkSpace);
CString ReplaceByNops(CString sArg);
CString FixLines(CEdit* editbox, CString input);
//end-defs-----------------------------------------------------------------

class CAboutDlg : public CDialog
{

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

// Implementation
protected:
	DECLARE_MESSAGE_MAP()
};

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialog)
END_MESSAGE_MAP()


// CDeobfuscatorHelperDlg dialog




CDeobfuscatorHelperDlg::CDeobfuscatorHelperDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CDeobfuscatorHelperDlg::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CDeobfuscatorHelperDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CDeobfuscatorHelperDlg, CDialog)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	//}}AFX_MSG_MAP
	ON_BN_CLICKED(IDOK, &CDeobfuscatorHelperDlg::OnBnClickedOk)
	ON_BN_CLICKED(ID_PASTE, &CDeobfuscatorHelperDlg::OnBnClickedPaste)
	ON_BN_CLICKED(IDGO, &CDeobfuscatorHelperDlg::OnBnClickedGo)
	ON_BN_CLICKED(ID_COPY, &CDeobfuscatorHelperDlg::OnBnClickedCopy)
	ON_BN_CLICKED(IDC_ABOUT, &CDeobfuscatorHelperDlg::OnBnClickedAbout)
END_MESSAGE_MAP()


// CDeobfuscatorHelperDlg message handlers

BOOL CDeobfuscatorHelperDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

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
	CButton* pBtn = (CButton*)GetDlgItem(IDC_GEN);
	pBtn->SetCheck(1);
	CButton* pBtn2 = (CButton*)GetDlgItem(IDC_GW);
	pBtn2->SetCheck(1);


	return TRUE;  // return TRUE  unless you set the focus to a control
}

void CDeobfuscatorHelperDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialog::OnSysCommand(nID, lParam);
	}
}

// If you add a minimize button to your dialog, you will need the code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework.

void CDeobfuscatorHelperDlg::OnPaint()
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
		CDialog::OnPaint();
	}
}

// The system calls this function to obtain the cursor to display while the user drags
//  the minimized window.
HCURSOR CDeobfuscatorHelperDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}


void CDeobfuscatorHelperDlg::OnBnClickedOk()
{
	
	OnOK();
}

void CDeobfuscatorHelperDlg::OnBnClickedPaste()
{
	CEdit* editbox = (CEdit*)GetDlgItem(IDC_INPUT);

	editbox->SetWindowTextW(NULL);
	editbox->Paste();
}


void CDeobfuscatorHelperDlg::OnBnClickedGo()
{
	CEdit* editbox = (CEdit*)GetDlgItem(IDC_INPUT);
	CEdit* editbox2 = (CEdit*)GetDlgItem(IDC_RESULT);
	CButton* CheckGen = (CButton*)GetDlgItem(IDC_GEN);
	CButton* CheckSpace = (CButton*)GetDlgItem(IDC_GW);

	CString input, Bytes, Wilded, Spaced, FinalResult(""), getC;
	CArray<CString, CString> v;
	
	int x, y, index = 0;
	editbox->GetWindowText(input);

	input = FixLines(editbox, input);

	// splitting lines
	while (AfxExtractSubString(getC, input, index, _T('\n')))
	{
		v.Add(getC);
		++index;
	}

	//if (index != 1) index--;
	//for (y = 0; index; y++) {
	
	try {
		for (y = 0; y < index; y++) {
			//if (y >= index)break;
			x = ReachBytes(v[y]);
			Bytes = GetBytesArray(v[y], x);
			Wilded = AddWildCards(Bytes);
			Spaced = TakeOutSpaces(Wilded, CheckSpace);
			FinalResult += Spaced;
		}
	}
	catch (std::exception) {
		MessageBox(_T("Seems like this isn't what you copied from Olly, it's manipulated !"), _T("Error"), MB_ICONINFORMATION);
	};

	//fix the first char
	if (CheckSpace->GetCheck() == 0) FinalResult = FinalResult.Mid(1);

	//generating
	if (CheckGen->GetCheck() == 1) {
		//repl eip, ##, ##, x
		FinalResult = _T("repl eip, #") + FinalResult + _T("#, #") + ReplaceByNops(FinalResult) + _T("#, x");
		editbox2->SetWindowTextW(FinalResult);
	}
	else {
		editbox2->SetWindowTextW(FinalResult);
	}

}

void CDeobfuscatorHelperDlg::OnBnClickedCopy()
{
	// TODO: Add your control notification handler code here
	CEdit* editbox = (CEdit*)GetDlgItem(IDC_RESULT);
	CString szData;
	editbox->GetWindowText(szData);

	HGLOBAL h;
	LPTSTR arr;

	size_t bytes = (szData.GetLength() + 1)*sizeof(TCHAR);
	h = GlobalAlloc(GMEM_MOVEABLE, bytes);
	arr = (LPTSTR)GlobalLock(h);
	ZeroMemory(arr, bytes);
	_tcscpy_s(arr, szData.GetLength() + 1, szData);
	szData.ReleaseBuffer();
	GlobalUnlock(h);

	::OpenClipboard(NULL);
	EmptyClipboard();
	SetClipboardData(CF_UNICODETEXT, h);
	CloseClipboard();
}

int ReachBytes(CString sArg) {
	CString getC;
	int x, l = sArg.GetLength();
	//sArg[99]; //exception test

	for (x = 0; x < l; x++) {
		getC = sArg.Mid(x, 1);
		if (getC == " ") break;
		if (getC == "\n") break;
		/*if (getC != " ") {
			//minup[0] += getC;
		}

		else
		{
			break;
		}*/

	}

	for (x; x < l; x++) {
		getC = sArg.Mid(x, 1);
		if (isxdigit(getC[0])) break;
		if (getC == "\n") break;
	}

	return x;
}

CString GetBytesArray(CString sArg, int ReachedByte) {
	CString getC, strRet;
	strRet = "";
	int x = ReachedByte, l = sArg.GetLength();

	for (x; x <= l; x += 2) {
		getC = sArg.Mid(x, 2);
		if (getC != "  ") {
			strRet += getC;
		}
		else {
			break;
		}
	}

	return strRet;
}

CString AddWildCards(CString sArg) {
	CString getC;
	int x, z;

	x = sArg.Find(' ');
	z = sArg.GetLength();
	z--;
	if (x != -1) {
		//for (x; z; x++) {
		for (x; x < z; x++) {
			getC = sArg.Mid(x, 1);
			if (isxdigit(getC[0])) {
				sArg.SetAt(x, _T('?'));
				//minup[2].Replace(getC, _T("?"));
			}
			//if (x >= z)break;
		}
	}
	return sArg;
}

CString TakeOutSpaces(CString sArg, CButton* ChkSpace) {
	int z, y;

	if (ChkSpace->GetCheck() == 1) {
		sArg.Replace(_T(" "), _T(""));
	}
	else {
		z = sArg.ReverseFind(' ');
		y = sArg.GetLength() - 1;
		if (z == y) {
			sArg = sArg.Mid(0, y);
		}
		sArg.Insert(0, _T(" "));
	}

	return sArg;
}

CString ReplaceByNops(CString sArg) {
	int x, y = sArg.GetLength();
	
	//for (x = 0; y; x++) {
	for (x = 0; x < y; x++) {
		if (sArg[x] != _T(' ')) {
			//if (x >= y) break;
			sArg.SetAt(x, _T('9'));
			x++;
			sArg.SetAt(x, _T('0'));
		}
	}

	return sArg;
}

void CDeobfuscatorHelperDlg::OnBnClickedAbout()
{
	// TODO: Add your control notification handler code here
	MessageBox(_T("Deobfuscation Helper v0.1\nZ-Rantom | AT4RE\n\nUsage: just copy text from OllyDbg to get your script/bytes.\n\nSpecial thanks to: Ahmed18, Kurapica, Mr.eXoDia, atom0s, kao, xSRTsect.\n\nGreetings to: kOuD3LkA, Prodigy, Mouradpr, mICR0b, Azma, dj-siba, STRELiTZIAþ, ghost-dz, Sn!per X, Esso-X, Mr. Paradox, Service-Manual, abdo-ok, alfares, ColdZero.\nAnd all AT4RE and AoRE members\nAnd surely, You !"),
		_T("About"), MB_ICONINFORMATION);
}

CString FixLines(CEdit* editbox, CString input) {
	CString getC;
	
	while (input.Mid(0, 2) == "\r\n") {
		input = input.Mid(2);
	}
	
	while (input.Find(_T("\r\n\r\n"), 0) != -1) {
		input.Replace(_T("\r\n\r\n"), _T("\r\n"));
	}

	return input;
}