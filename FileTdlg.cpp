#// FileTdlg.cpp : implementation file
//

#include "stdafx.h"
#include "FileTest.h"
#include "FileTdlg.h"
#include <windowsx.h>
#include <Shlwapi.h>

#include <vector>
using namespace std;

#ifdef _DEBUG
#undef THIS_FILE
static char BASED_CODE THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CAboutDlg dialog used for App About

class CAboutDlg : public CDialog
{
public:
	CAboutDlg();

// Dialog Data
	//{{AFX_DATA(CAboutDlg)
	enum { IDD = IDD_ABOUTBOX };
	//}}AFX_DATA

// Implementation
protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support
	//{{AFX_MSG(CAboutDlg)
	virtual BOOL OnInitDialog();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialog(CAboutDlg::IDD)
{
	//{{AFX_DATA_INIT(CAboutDlg)
	//}}AFX_DATA_INIT
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CAboutDlg)
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialog)
	//{{AFX_MSG_MAP(CAboutDlg)
		// No message handlers
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CAboutDlg message handlers

BOOL CAboutDlg::OnInitDialog()
{
	CDialog::OnInitDialog();
	CenterWindow();
	
	// TODO: Add extra about dlg initialization here
	
	return TRUE;  // return TRUE  unless you set the focus to a control
}

/////////////////////////////////////////////////////////////////////////////
// CFileTestDlg dialog

CFileTestDlg::CFileTestDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CFileTestDlg::IDD, pParent)
	, m_bBackupAttr(false)
	, m_bNoBuffering(false)
	, m_bPreserveTimestamps(false)
	, m_bPosixSemantics(FALSE)
{
	//{{AFX_DATA_INIT(CFileTestDlg)
	m_FileName = _T("c:\\test.txt");
	m_bRead = FALSE;
	m_bShareRead = FALSE;
	m_bShareWrite = FALSE;
	m_bWrite = FALSE;
	m_bTemporary = FALSE;
	m_bDelete = FALSE;
	m_bShareDelete = FALSE;
	//}}AFX_DATA_INIT
	// Note that LoadIcon does not require a subsequent DestroyIcon in Win32
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);

	m_hFile = INVALID_HANDLE_VALUE;
}

void CFileTestDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CFileTestDlg)
	DDX_Text(pDX, IDC_FILENAME, m_FileName);
//	DDV_MaxChars(pDX, m_FileName, 256);
	DDX_Check(pDX, IDC_READ, m_bRead);
	DDX_Check(pDX, IDC_SHARE_READ, m_bShareRead);
	DDX_Check(pDX, IDC_SHARE_WRITE, m_bShareWrite);
	DDX_Check(pDX, IDC_WRITE, m_bWrite);
	DDX_Check(pDX, IDC_TEMPORARY, m_bTemporary);
	DDX_Check(pDX, IDC_DELETE, m_bDelete);
	DDX_Check(pDX, IDC_SHARE_DELETE, m_bShareDelete);
	DDX_Check(pDX, IDC_BACKUP_ATTR, m_bBackupAttr);
	DDX_Check(pDX, IDC_NO_BUFFER_FLAG, m_bNoBuffering);
	DDX_Check(pDX, IDC_PRESERVE_TIMESTAMP, m_bPreserveTimestamps );
	DDX_Check(pDX, IDC_POSIX_SEMANTICS, m_bPosixSemantics );
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CFileTestDlg, CDialog)
	//{{AFX_MSG_MAP(CFileTestDlg)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_OPEN, OnOpen)
	ON_BN_CLICKED(IDC_CLOSE, OnClose)
	//}}AFX_MSG_MAP
	ON_BN_CLICKED(IDC_READ_BUFF, OnBnClickedReadBuff)
	ON_BN_CLICKED(IDC_BROWSE, OnBnClickedBrowse)
	ON_BN_CLICKED(IDC_WRITE_BUFF, OnBnClickedWriteBuff)
	ON_WM_DROPFILES()
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CFileTestDlg message handlers

BOOL CFileTestDlg::OnInitDialog()
{
	CDialog::OnInitDialog();
	CenterWindow();

	// Add "About..." menu item to system menu.

	// IDM_ABOUTBOX must be in the system command range.
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	CString strAboutMenu;
	if ( strAboutMenu.LoadString(IDS_ABOUTBOX) )
	{
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}
	
	// TODO: Add extra initialization here
	SetIcon( m_hIcon, false );
	SetIcon( m_hIcon, true );

	ComboBox_SetCurSel( ::GetDlgItem( m_hWnd, IDC_CREATION ), 2 );
	
	/* Allow auto-complete on the file name edit control */
	CoInitialize( NULL );
	SHAutoComplete( GetDlgItem( IDC_FILENAME )->m_hWnd, SHACF_FILESYS_ONLY );

	/* Allow drag & drop of file */
	DragAcceptFiles();

	/* Set the caption text to the application name (resource string) */
	SetWindowText( AfxGetAppName() );

	return TRUE;  // return TRUE  unless you set the focus to a control
}

void CFileTestDlg::OnSysCommand(UINT nID, LPARAM lParam)
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

void CFileTestDlg::OnPaint() 
{
	if (IsIconic())
	{
		CPaintDC dc(this); // device context for painting

		SendMessage(WM_ICONERASEBKGND, (WPARAM) dc.GetSafeHdc(), 0);

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

// The system calls this to obtain the cursor to display while the user drags
//  the minimized window.
HCURSOR CFileTestDlg::OnQueryDragIcon()
{
	return (HCURSOR) m_hIcon;
}

void ReportErrorCode( LPCTSTR pFmt )
{
	const DWORD ErrCode = GetLastError();

	CString ErrMsg;
	ErrMsg.Format( pFmt, ErrCode );

	LPVOID lpMsgBuf;
	if ( FormatMessage( 
		FORMAT_MESSAGE_ALLOCATE_BUFFER | 
		FORMAT_MESSAGE_FROM_SYSTEM | 
		FORMAT_MESSAGE_IGNORE_INSERTS,
		NULL,
		ErrCode,
		MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT), // Default language
		(LPTSTR) &lpMsgBuf,
		0,
		NULL ))
	{
		// Append the system error string.
		ErrMsg += _T("\r\n\r\n");
		ErrMsg += (LPCTSTR) lpMsgBuf;

		// Free the buffer.
		LocalFree( lpMsgBuf );
	}

	AfxMessageBox( ErrMsg, MB_OK | MB_ICONERROR );
}

void CFileTestDlg::OnOpen() 
{
	/* Get the dialog controls updated */
	UpdateData();
	
	/* Build up the flags that we have selected from the controls on the dialog */
	DWORD dwRWMode = 0;

	if ( m_bRead )
	{
		dwRWMode |= GENERIC_READ;
	}

	if ( m_bWrite )
	{
		dwRWMode |= GENERIC_WRITE;
	}

	DWORD dwShareMode = 0;

	if ( m_bShareRead )
	{
		dwShareMode |= FILE_SHARE_READ;
	}

	if ( m_bShareWrite )
	{
		dwShareMode |= FILE_SHARE_WRITE;
	}

	if ( m_bShareDelete )
	{
		dwShareMode |= FILE_SHARE_DELETE;
	}

	DWORD dwAttribs = 0;

	if ( m_bTemporary )
	{
		dwAttribs |= FILE_ATTRIBUTE_TEMPORARY;
	}

	if ( m_bDelete )
	{
		dwAttribs |= FILE_FLAG_DELETE_ON_CLOSE;
	}

	if ( m_bBackupAttr )
	{
		dwAttribs |= FILE_FLAG_BACKUP_SEMANTICS;
	}

	if ( m_bNoBuffering )
	{
		dwAttribs |= FILE_FLAG_NO_BUFFERING;
	}

	if ( m_bPosixSemantics )
	{
		dwAttribs |= FILE_FLAG_POSIX_SEMANTICS;
	}

	DWORD dwCreateMode;
	{
		CString strCreation;
		CComboBox * pcb = (CComboBox *) GetDlgItem(IDC_CREATION);

		pcb->GetWindowText( strCreation );

		const int indx = pcb->FindStringExact( 0, strCreation );

		/* We've found the indexed entry in the combo box - so convert it to the equivalent "OPEN" value */
		static const DWORD cmodes[] =
		{
			CREATE_NEW,
			CREATE_ALWAYS,
			OPEN_EXISTING,
			OPEN_ALWAYS,
			TRUNCATE_EXISTING 
		};

		if ( ( indx != CB_ERR ) && ( indx < DIM(cmodes) ) )
		{
			dwCreateMode = cmodes[indx];
		}
		else
		{
			/* Fail to a safe value (shouldn't ever occur) */
			dwCreateMode = OPEN_ALWAYS;
		}
	}

	/* Open the file */
	m_hFile = CreateFile( m_FileName,
			dwRWMode,
			dwShareMode,
			NULL,
			dwCreateMode,
			FILE_ATTRIBUTE_NORMAL | dwAttribs,
			NULL );

	/* have we got an error? */
	if ( INVALID_HANDLE_VALUE == m_hFile )
	{
		ReportErrorCode( _T("Error Opening File. GetLastError reports %d") );
	}
	else
	{
		if ( m_bPreserveTimestamps )
		{
			static const FILETIME ftNoChange = { (unsigned) -1, (unsigned) -1 };
			SetFileTime( m_hFile, NULL, &ftNoChange, &ftNoChange );
		}

		/* Disable the Open button to prevent re-use */
		GetDlgItem( IDC_OPEN )->EnableWindow( FALSE );
		GetDlgItem( IDC_CLOSE )->EnableWindow( true );

		/* Enable the read & write buttons */
		GetDlgItem( IDC_READ_BUFF )->EnableWindow( TRUE );
		GetDlgItem( IDC_WRITE_BUFF )->EnableWindow( TRUE );
	}
}

void CFileTestDlg::OnClose() 
{
	/* If we had opened a file - we had better close it */
	if ( INVALID_HANDLE_VALUE != m_hFile )
	{
		if ( !CloseHandle( m_hFile ) )
		{
			ReportErrorCode( _T("Error closing File. GetLastError reports %d") );
		}
		else
		{
			/* Invalidate the handle to prevent us from trying to close it again */
			m_hFile = INVALID_HANDLE_VALUE;

			/* Enable the Open button to prevent re-use */
			GetDlgItem( IDC_OPEN )->EnableWindow( TRUE );
			GetDlgItem( IDC_CLOSE )->EnableWindow( false );

			/* Disable the read & write buttons */
			GetDlgItem( IDC_READ_BUFF )->EnableWindow( FALSE );
			GetDlgItem( IDC_WRITE_BUFF )->EnableWindow( FALSE );
		}
	}	
}

void CFileTestDlg::OnCancel() 
{
	/* Tidy up any open files, just in case the user hasn't done it */
	OnClose();
	
	CDialog::OnCancel();
}

typedef struct
{
	TCHAR szByte[ sizeof("ff ")-1 ];
} SZBYTE;

void CFileTestDlg::OnBnClickedReadBuff()
{
	/* File open? */
	if ( m_hFile != INVALID_HANDLE_VALUE )
	{
		SetFilePointer( m_hFile, 0, NULL, FILE_BEGIN );

		BYTE Buffer[10];
		DWORD BytesRead;

		if ( ReadFile( m_hFile, Buffer, sizeof( Buffer ), &BytesRead, NULL ) )
		{
			if ( BytesRead != 0 )
			{
				SZBYTE szBuffer[ sizeof( Buffer ) ];

				for ( DWORD indx = 0; indx < BytesRead; indx++ )
				{
					wsprintf( szBuffer[indx].szByte, _T("%.2X"), Buffer[indx] );
					szBuffer[indx].szByte[2] = _T(' ');
				}

				/* Null terminate the last entry */
				szBuffer[BytesRead-1].szByte[2] = _T('\0');

				SetDlgItemText( IDC_RW_BUFFER, szBuffer[0].szByte );
			}
			else
			{
				SetDlgItemText( IDC_RW_BUFFER, _T("file is empty!") );
			}
		}
		else
		{
			ReportErrorCode( _T("Error reading from the File. GetLastError reports %d") );
		}
	}
	else
	{
		MessageBeep( MB_OK );
	}
}

static bool HexCharToUInt( TCHAR ch, UINT & Val )
{
	bool bOK = true;

	ch = (TCHAR) CharUpper( (LPTSTR) ch );

	if ( ( ch >= _T('0') ) && ( ch <= _T('9') ) )
	{
		Val = ch & 0x0F;
	}
	else if ( ( ch >= _T('A') ) && ( ch <= _T('F') ) )
	{
		Val = ch - _T('A') + 10;
	}
	else
	{
		bOK = false;
		Val = 0;
	}

	return bOK;
}

void CFileTestDlg::OnBnClickedWriteBuff()
{
	/* File open? */
	if ( m_hFile != INVALID_HANDLE_VALUE )
	{
		/* Read and parse the string in the edit field */
		CString sVals;
		GetDlgItemText( IDC_RW_BUFFER, sVals );

		sVals.Trim();

		if ( !sVals.IsEmpty() )
		{
			SetFilePointer( m_hFile, 0, NULL, FILE_BEGIN );

			/* The string must be formatted as pairs of hex characters separated by a space */
			const SZBYTE * psVals = (const SZBYTE *) (LPCTSTR) sVals;

			/* Assume 3 characters per byte, and allow the last item not to be space terminated */
			const size_t NumBytes = (sVals.GetLength()+1)/3;

			/* Create a sufficiently large buffer */
			vector<BYTE> vBuffer( NumBytes );

			size_t indx;
			for ( indx = 0; indx < NumBytes; indx++ )
			{
				UINT Val;

				if ( HexCharToUInt( psVals[indx].szByte[0], Val ) )
				{
					Val *= 16;

					UINT Val2;
					if ( HexCharToUInt( psVals[indx].szByte[1], Val2 ) )
					{
						Val += Val2;

						vBuffer[indx] = static_cast<BYTE>( Val );
					}
					else
					{
						break;
					}
				}
				else
				{
					break;
				}
			}

			if ( indx == NumBytes )
			{
				DWORD BytesWritten;

				if ( WriteFile( m_hFile, &vBuffer[0], NumBytes, &BytesWritten, NULL ) )
				{
				}
				else
				{
					ReportErrorCode( _T("Error writing to the File. GetLastError reports %d") );
				}
			}
			else
			{
				AfxMessageBox( _T("Invalid hex format input") );
			}
		}
		else
		{
			MessageBeep( MB_ICONHAND );
		}
	}
	else
	{
		MessageBeep( MB_OK );
	}
}

void CFileTestDlg::OnBnClickedBrowse()
{
	/* Get any existing name from the edit field to pre-populate the dialog with */
	CString sFile;
	GetDlgItemText( IDC_FILENAME, sFile );

	CFileDialog dlg( true, 0, sFile );

	if ( dlg.DoModal() == IDOK )
	{
		SetDlgItemText( IDC_FILENAME, dlg.GetPathName() );
	}
}

void CFileTestDlg::OnDropFiles(HDROP hDropInfo)
{
	UINT NumFiles = DragQueryFile( hDropInfo, static_cast<UINT>(-1), NULL, 0 );

	if ( NumFiles == 1 )
	{
		TCHAR szFileName[_MAX_PATH];
		if ( DragQueryFile( hDropInfo, 0, szFileName, _countof( szFileName ) ) )
		{
			m_FileName = szFileName;

			UpdateData( 0 );
		}
	}
	else
	{
		AfxMessageBox( _T("You can only drag & drop a single file") );
	}

	DragFinish( hDropInfo );

	CDialog::OnDropFiles(hDropInfo);
}
