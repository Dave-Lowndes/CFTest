// FileTdlg.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CFileTestDlg dialog

class CFileTestDlg : public CDialog
{
// Construction
public:
	CFileTestDlg(CWnd* pParent = NULL);	// standard constructor

// Dialog Data
	//{{AFX_DATA(CFileTestDlg)
	enum { IDD = IDD_FILETEST_DIALOG };
	CString	m_FileName;
	BOOL	m_bRead;
	BOOL	m_bShareRead;
	BOOL	m_bShareWrite;
	BOOL	m_bWrite;
	BOOL	m_bTemporary;
	BOOL	m_bDelete;
	BOOL	m_bShareDelete;
	//}}AFX_DATA

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CFileTestDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	HICON m_hIcon;

	// Generated message map functions
	//{{AFX_MSG(CFileTestDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	afx_msg void OnOpen();
	afx_msg void OnClose();
	virtual void OnCancel();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedReadBuff();
	afx_msg void OnBnClickedBrowse();
private:
	HANDLE m_hFile;	// The current file handle
public:
	afx_msg void OnBnClickedWriteBuff();
private:
	// FILE_FLAG_BACKUP_SEMANTICS
	BOOL m_bBackupAttr;
	BOOL m_bNoBuffering;
	BOOL m_bPreserveTimestamps;
	BOOL m_bPosixSemantics;
public:
	afx_msg void OnDropFiles(HDROP hDropInfo);
};
