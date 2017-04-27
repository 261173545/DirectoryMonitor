// DirectoryMonitorDlg.cpp : implementation file
//

#include <windows.h> 
#include "stdafx.h"
#include "DirectoryMonitor.h"
#include "DirectoryMonitorDlg.h"
#include "OPTIONS.h"
#include "Appmonitor.h"
#include "SplashWnd.h"
#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif
#include "FileVersion.h"
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

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CAboutDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
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
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDirectoryMonitorDlg dialog

/************************************************************************/
/* ��������CDirectoryMonitorDlg

   �������ܣ���ʼ��ϵͳͼ��
   �����ߣ�Peter
   ����ʱ�䣺2015-6-16
   �޸�ʱ��: 2015-6-16
   �޸����ݣ��޸�ϵͳͼ��                                               */
/************************************************************************/
CDirectoryMonitorDlg::CDirectoryMonitorDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CDirectoryMonitorDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDirectoryMonitorDlg)
	m_strDirectoryPath = _T("");
	m_thread=NULL;
	index=0;
	strPathCommon="";
	m_autoMin = FALSE;
	m_autoMonitor = FALSE;
	m_autoRun = FALSE;
	m_windows = FALSE;
	//}}AFX_DATA_INIT
	// Note that LoadIcon does not require a subsequent DestroyIcon in Win32
	m_hIcon = AfxGetApp()->LoadIcon(IDI_ICON1);
}

void CDirectoryMonitorDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDirectoryMonitorDlg)
	DDX_Control(pDX, IDC_LIST1, m_list);
	DDX_Text(pDX, IDC_EDIT1, m_strDirectoryPath);
	DDX_Check(pDX, IDC_AUTOMIN, m_autoMin);
	DDX_Check(pDX, IDC_AUTOMONITOR, m_autoMonitor);
	DDX_Check(pDX, IDC_AUTORUN, m_autoRun);
	DDX_Check(pDX, IDC_WINDOWS, m_windows);
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CDirectoryMonitorDlg, CDialog)
	//{{AFX_MSG_MAP(CDirectoryMonitorDlg)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_BTN_OPTIONS, OnBtnOptions)
	ON_BN_CLICKED(IDC_BTN_APP, OnBtnApp)
	ON_BN_CLICKED(IDC_BTN_SELECTDIR, OnBtnSelectdir)
	ON_BN_CLICKED(IDC_BTN_START, OnBtnStart)
	ON_BN_CLICKED(IDC_BTN_STOP, OnBtnStop)
	ON_BN_CLICKED(IDC_BTN_CLEAR, OnBtnClear)
	ON_BN_CLICKED(IDC_AUTOMONITOR, OnAutomonitor)
	ON_BN_CLICKED(IDC_AUTOMIN, OnAutomin)
	ON_BN_CLICKED(IDC_AUTORUN, OnAutorun)
	ON_BN_CLICKED(IDC_WINDOWS, OnWindows)
	ON_MESSAGE(WM_TRAY,OnTrayNotify)
	ON_WM_CLOSE()
	ON_WM_SIZE()
	ON_WM_CREATE()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDirectoryMonitorDlg message handlers
/* ��������CDirectoryMonitorDlg

   �������ܣ���ʼ���Ի���
   �����ߣ�Peter
   ����ʱ�䣺2015-6-16
   �޸�ʱ��: 2015-6-16
   �޸����ݣ��޸�ϵͳ�ĳ�ʼ����Ա����                                              */
/************************************************************************/
BOOL CDirectoryMonitorDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// Add "About..." menu item to system menu.

	// IDM_ABOUTBOX must be in the system command range.
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		CString strAboutMenu;
		strAboutMenu.LoadString(IDS_ABOUTBOX);
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
	InitTray();
    InitListView();
	//��ʼ����ֹͣ��ť
    GetDlgItem(IDC_BTN_STOP)->EnableWindow(FALSE);

   //InitProfile(m_autoMin,"Minimized",1);
  // InitProfile(m_autoMin,"AutoMonitor",2);
    CString str,str1,str2,str3;
	GetPrivateProfileString("Settings","Minimized","",str.GetBuffer(MAX_PATH),MAX_PATH,".\\Settings.ini");

	GetPrivateProfileString("Settings","AutoMonitor","",str1.GetBuffer(MAX_PATH),MAX_PATH,".\\Settings.ini");

	GetPrivateProfileString("Settings","AlwaysOnTop","",str2.GetBuffer(MAX_PATH),MAX_PATH,".\\Settings.ini");

	GetPrivateProfileString("Settings","AutoRun","",str3.GetBuffer(MAX_PATH),MAX_PATH,".\\Settings.ini");

	if(str=="1")
	{
       m_autoMin=true;
	   UpdateData(FALSE);
	   PostMessage(WM_SYSCOMMAND, SC_MINIMIZE, 0);
	}
	else
	{
	   m_autoMin=false;
	   UpdateData(FALSE);
	}
	if(str1=="1")
	{
		m_autoMonitor=true;
		m_strDirectoryPath="C:\\";
		UpdateData(FALSE);
		GetDlgItem(IDC_EDIT1)->SetWindowText("���ڼ��"+m_strDirectoryPath);
	    OnBtnStart();
	}
	else
	{
		m_autoMonitor=false;
		UpdateData(FALSE);
	}
	if(str2=="1")
	{
		m_windows=true;
		UpdateData(FALSE);
		OnWindows();
	}
	else
	{
		m_windows=false;
		UpdateData(FALSE);
	}
	if (str3=="1")
	{
        m_autoRun=true;
		UpdateData(FALSE);
	}
	else
	{
		m_autoRun=false;
		UpdateData(FALSE);
	}
	return TRUE;  // return TRUE  unless you set the focus to a control
}

void CDirectoryMonitorDlg::InitProfile(BOOL b,CString strName,int type)
{
    CString str;
	GetPrivateProfileString("Settings",strName,"",str.GetBuffer(MAX_PATH),MAX_PATH,".\\Settings.ini");
	if(str=="1")
	{	
		b = true;
		UpdateData(FALSE);

	    if(type==1)
		   PostMessage(WM_SYSCOMMAND, SC_MINIMIZE, 0);
		else if(type==2)
		{
		 m_strDirectoryPath="C:\\";
	 	 UpdateData(FALSE);
		 GetDlgItem(IDC_EDIT1)->SetWindowText("���ڼ��"+m_strDirectoryPath);
	     OnBtnStart();
		}
	}
	else if(str=="0")
	{
		b=false;
		UpdateData(FALSE);
	}
}

void CDirectoryMonitorDlg::OnSysCommand(UINT nID, LPARAM lParam)
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

void CDirectoryMonitorDlg::OnPaint() 
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
HCURSOR CDirectoryMonitorDlg::OnQueryDragIcon()
{
	return (HCURSOR) m_hIcon;
}
/* ��������OnBtnOptions

   �������ܣ�������ѡ���ضԻ���
   �����ߣ�Peter
   ����ʱ�䣺2015-6-16
   �޸�ʱ��: 
   �޸����ݣ�                                              */
/************************************************************************/
void CDirectoryMonitorDlg::OnBtnOptions() 
{
	// TODO: Add your control notification handler code here
	COPTIONS dlg;
	dlg.DoModal();
}
/* ��������OnBtnApp

   �������ܣ�������Ӧ�ó����ضԻ���
   �����ߣ�Peter
   ����ʱ�䣺2015-6-16
   �޸�ʱ��: 
   �޸����ݣ�                                              */
/************************************************************************/
void CDirectoryMonitorDlg::OnBtnApp() 
{
	// TODO: Add your control notification handler code here
	CAppmonitor dlg;
	dlg.DoModal();
}
/* ��������OnBtnSelectdir

   �������ܣ�ѡ����Ŀ¼
   �����ߣ�Peter
   ����ʱ�䣺2015-6-25
   �޸�ʱ��: 
   �޸����ݣ�                                              */
/************************************************************************/
void CDirectoryMonitorDlg::OnBtnSelectdir() 
{
	// TODO: Add your control notification handler code here
/*	CFileDialog dlg(true);
	CString path;
	if(dlg.DoModal()!=IDOK)
	{
       return;
	}
	else
		 path=dlg.GetPathName();
    AfxMessageBox(path);*/
	static TCHAR strDirName[MAX_PATH];
	BROWSEINFO bi;
	CString szString = TEXT("ѡ��һ��Դ�ļ����ļ���");
	bi.hwndOwner = this->GetSafeHwnd();
	bi.pidlRoot = NULL;
    bi.pszDisplayName = strDirName;
	bi.lpszTitle = szString;
    bi.ulFlags = BIF_BROWSEFORCOMPUTER | BIF_DONTGOBELOWDOMAIN | BIF_RETURNONLYFSDIRS;
    bi.lpfn = NULL;
	bi.lParam = 0;
	bi.iImage = 0;

	LPITEMIDLIST pItemIDList = SHBrowseForFolder(&bi);	
	if(pItemIDList == NULL)		
	{		
		return ;		
	}		
	SHGetPathFromIDList(pItemIDList, strDirName);
	strPathCommon=strDirName;
	if(strPathCommon != "" && strPathCommon.Right(1) != "\\")
		strPathCommon += "\\";
	GetDlgItem(IDC_EDIT1)->SetWindowText(strPathCommon);
	
	//SetDlgItemText(IDC_EDIT1,str);
}
/* ��������InitListView

   �������ܣ���ʼ���б���ͼ�ؼ�
   �����ߣ�Peter
   ����ʱ�䣺2015-6-25
   �޸�ʱ��: 
   �޸����ݣ�                                              */
/************************************************************************/
void CDirectoryMonitorDlg::InitListView()
{
   // CRect rect;
   // m_list.GetClientRect(&rect);
	m_list.ModifyStyle(LVS_TYPEMASK,LVS_REPORT);
	m_list.SetExtendedStyle(LVS_EX_GRIDLINES|LVS_EX_FLATSB|LVS_EX_FULLROWSELECT|LVS_EX_TWOCLICKACTIVATE);
	m_list.InsertColumn(0,"��  ��",LVCFMT_LEFT,50);
    m_list.InsertColumn(1,"ʱ  ��",LVCFMT_LEFT,150);
    m_list.InsertColumn(2,"��  ��",LVCFMT_LEFT,70);
    m_list.InsertColumn(3,"�仯����",LVCFMT_LEFT,600);

/*m_list.InsertItem(3,"111");
				m_list.SetItemText(0,1,"111");
				m_list.SetItemText(0,2,"111");
				m_list.SetItemText(0,3,"111");
				m_list.InsertItem(5,"111");*/
}
/* ��������ThreadProc

�������ܣ��̴߳�����
�����ߣ�Peter
����ʱ�䣺2015-6-25
�޸�ʱ��: 2015-7-4
�޸����ݣ�����ѡ����ƹ��ܣ�����ѡ���ѡȡ�����в�ͬ���͵��ļ����                                             */
/************************************************************************/
DWORD WINAPI ThreadProc(LPVOID lpParam)
{

//	AfxMessageBox(strTime);
	BOOL b1,b2,b3,b4,b5;
    b1=(GetPrivateProfileInt("Selections","Check1Del",0,".\\Selections.ini")==0 ?false:true);
	b2=(GetPrivateProfileInt("Selections","Check2Modify",0,".\\Selections.ini")==0 ?false:true);
	b3=(GetPrivateProfileInt("Selections","Check3Rename",0,".\\Selections.ini")==0 ?false:true);
	b4=(GetPrivateProfileInt("Selections","Check4Mdfatr",0,".\\Selections.ini")==0 ?false:true);
	b5=(GetPrivateProfileInt("Selections","Check5Other",0,".\\Selections.ini")==0 ?false:true);

    BOOL bRet = FALSE; 
    BYTE Buffer[1024] = { 0 }; 

    CDirectoryMonitorDlg *pdlg = (CDirectoryMonitorDlg*)lpParam;

    FILE_NOTIFY_INFORMATION *pBuffer = (FILE_NOTIFY_INFORMATION *)Buffer; 

    DWORD BytesReturned = 0; 

    //AfxMessageBox(pdlg->m_strDirectoryPath);

     HANDLE hFile = CreateFile(pdlg->m_strDirectoryPath.GetBuffer(0),
                FILE_LIST_DIRECTORY,  
                FILE_SHARE_READ|FILE_SHARE_DELETE|FILE_SHARE_WRITE, 
                NULL, 
                OPEN_EXISTING, 
                FILE_FLAG_BACKUP_SEMANTICS, 
                NULL); 
    if ( INVALID_HANDLE_VALUE == hFile ) 
    { 
		CString str;
		str.Format("Error is %d",GetLastError());
		AfxMessageBox(str);
        return 1; 
    } 

  CString strInfo;
  CString strType;
  CString strIndex;
  COPTIONS dlg;
  while ( TRUE ) 
    { 
        ZeroMemory(Buffer, 1024); 

        bRet = ReadDirectoryChangesW(hFile, 
                        &Buffer, 
                        sizeof(Buffer), 
                        TRUE, 
                        FILE_NOTIFY_CHANGE_FILE_NAME | 
                        FILE_NOTIFY_CHANGE_ATTRIBUTES | 
                        FILE_NOTIFY_CHANGE_LAST_WRITE , 
                        &BytesReturned, 
                        NULL, NULL); 
         
        if ( bRet == TRUE ) 
		{
			//��ȡ��ǰ��ϵͳʱ��
			CTime tm;
			CString strTime;
			tm=CTime::GetCurrentTime();
	strTime = tm.Format(_T("%Y-%m-%d %H:%M:%S"));
 
            switch(pBuffer->Action)  
            {  
               
            case FILE_ACTION_ADDED:  
                { 
					strType = "���";
					strInfo = "����ļ���";
                    strInfo += pBuffer->FileName;
                    break; 	
                } 
                
            case FILE_ACTION_REMOVED:  
                { 
					if(b1)
					{
                    strType = "ɾ��";
					strInfo = "ɾ���ļ���";
					strInfo += pBuffer->FileName;
                    break; 
					}
                } 
                
            case FILE_ACTION_MODIFIED:  
                { 
					if(b2&&b4)
					{
                    strType = "�޸�";
					strInfo = "�޸��ļ���";
					strInfo += pBuffer->FileName;
                    break;
					}
                } 
                
            case FILE_ACTION_RENAMED_OLD_NAME:  
                { 
					if(b3)
					{
						strType = "������";
						strInfo = "���ļ���";
						strInfo += pBuffer->FileName;
				
						if ( pBuffer->NextEntryOffset != 0 ) 
						{ 
							FILE_NOTIFY_INFORMATION *tmpBuffer = (FILE_NOTIFY_INFORMATION *)((DWORD)pBuffer + pBuffer->NextEntryOffset); 
							switch ( tmpBuffer->Action ) 
							{ 
							case FILE_ACTION_RENAMED_NEW_NAME: 
								{         
									strInfo += "������Ϊ��";
									strInfo += tmpBuffer->FileName;
									break; 
								} 
							}			
						} 
						break; 
                    } 
                     
                } 
            case FILE_ACTION_RENAMED_NEW_NAME:  
                { 
					strType = "������";
					strInfo = "��������";
					strInfo += pBuffer->FileName;
					break;
                } 
			default:
				if(b5)
				{
					strType = "����";
					break;
				}
		
			}
			if(pdlg != NULL)
			{
				strIndex.Format("%d",pdlg->index);
				pdlg->m_list.InsertItem(pdlg->index,strIndex);
				pdlg->m_list.SetItemText(pdlg->index,1,strTime);
				pdlg->m_list.SetItemText(pdlg->index,2,strType);
				pdlg->m_list.SetItemText(pdlg->index++,3,strInfo);
			}
        } 
    } 
 	
   CloseHandle(hFile); 
   return 0; 	
}
/* ��������OnBtnStart

�������ܣ���ʼ���
�����ߣ�Peter
����ʱ�䣺2015-6-25
�޸�ʱ��: 
�޸����ݣ�                                              */
/************************************************************************/
void CDirectoryMonitorDlg::OnBtnStart() 
{
	// TODO: Add your control notification handler code here
	UpdateData(TRUE);
	if(m_strDirectoryPath=="")
	{
	 AfxMessageBox("����ѡ��Ҫ��ص��ļ���·��");
	 return;
	}
	if(strPathCommon != "")
	   GetDlgItem(IDC_EDIT1)->SetWindowText("���ڼ��:"+strPathCommon);

	 GetDlgItem(IDC_BTN_STOP)->EnableWindow(true);
	 GetDlgItem(IDC_BTN_START)->EnableWindow(FALSE);
	 GetDlgItem(IDC_BTN_SELECTDIR)->EnableWindow(FALSE);
	 m_thread=CreateThread(NULL,0,(LPTHREAD_START_ROUTINE)&ThreadProc,(LPVOID)this,0,NULL);
	 
	 if(m_thread == NULL)
	 {
		 AfxMessageBox("���ʧ��");
	 }
	 else
	 {
		 AfxMessageBox("��سɹ�");
		 //WaitForSingleObject(m_thread, INFINITE); 
         //CloseHandle(m_thread); 
	 }
}
/* ��������OnBtnStop

�������ܣ��������
�����ߣ�Peter
����ʱ�䣺2015-6-25
�޸�ʱ��: 
�޸����ݣ�                                              */
/************************************************************************/
void CDirectoryMonitorDlg::OnBtnStop() 
{
	// TODO: Add your control notification handler code here
	GetDlgItem(IDC_BTN_SELECTDIR)->EnableWindow(TRUE);
	GetDlgItem(IDC_BTN_START)->EnableWindow(TRUE);
	 GetDlgItem(IDC_BTN_STOP)->EnableWindow(FALSE);
	 if(m_thread)
	 {
		 TerminateThread(m_thread,0);
		 CloseHandle(m_thread);
		 m_thread=NULL;
	 }

	 GetDlgItem(IDC_EDIT1)->SetWindowText("ֹͣ���:"+strPathCommon);
}

void CDirectoryMonitorDlg::OnBtnClear() 
{
	// TODO: Add your control notification handler code here
	m_list.DeleteAllItems();

}
/* ��������OnAutomonitor

  �������ܣ��Զ���ʼ���
  �����ߣ�Peter
  ����ʱ�䣺2015-6-27
  �޸�ʱ��: 
�޸����ݣ�                                              */
/************************************************************************/
void CDirectoryMonitorDlg::OnAutomonitor() 
{
	// TODO: Add your control notification handler code here
	UpdateData(TRUE);
    WritePrivateProfileString("Settings","AutoMonitor",m_autoMonitor? "1":"0",".\\Settings.ini");
}
/* ��������OnAutomin

  �������ܣ��Զ���С��
  �����ߣ�Peter
  ����ʱ�䣺2015-6-27
  �޸�ʱ��: 
�޸����ݣ�                                              */
/************************************************************************/
void CDirectoryMonitorDlg::OnAutomin() 
{
	// TODO: Add your control notification handler code here
	UpdateData(TRUE);
    WritePrivateProfileString("Settings","Minimized",m_autoMin? "1":"0",".\\Settings.ini");

	//UpdateData(FALSE);
}
/* ��������OnAutorun

  �������ܣ��Զ�����
  �����ߣ�Peter
  ����ʱ�䣺2015-6-27
  �޸�ʱ��: 
�޸����ݣ�                                              */
/************************************************************************/
void CDirectoryMonitorDlg::OnAutorun() 
{
	// TODO: Add your control notification handler code here
	UpdateData(TRUE);
    WritePrivateProfileString("Settings","AutoRun",m_autoRun? "1":"0",".\\Settings.ini");
	if (m_autoRun==TRUE)
	{
		AutoRun();
	}
	else
		NoAutoRun();
}
/* ��������OnWindows

  �������ܣ��Զ�ʵ�ִ�����ǰ��
  �����ߣ�Peter
  ����ʱ�䣺2015-6-27
  �޸�ʱ��: 
�޸����ݣ�                                              */
/************************************************************************/
void CDirectoryMonitorDlg::OnWindows() 
{
	// TODO: Add your control notification handler code here
	UpdateData(TRUE);	
	if (m_windows)
	{
		SetWindowPos(&wndTopMost, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE | WS_EX_TOPMOST);
		WritePrivateProfileString("Settings","AlwaysOnTop","1",".\\Settings.ini");
	}
	
	else
	{
		::SetWindowPos(GetSafeHwnd(), HWND_NOTOPMOST, 0, 0, 0, 0,SWP_NOMOVE | SWP_NOSIZE);
		WritePrivateProfileString("Settings","AlwaysOnTop","0",".\\Settings.ini");
	}

}
/* ��������AutoRun

  �������ܣ�����������
  �����ߣ�Peter
  ����ʱ�䣺2015-8-24
  �޸�ʱ��: 
  �޸����ݣ�                                              */
/************************************************************************/
void CDirectoryMonitorDlg::AutoRun()
{
	CString keyStr;
	LONG lgtResult;
	CString fileName;
	HKEY hRegKey;
	
	keyStr =_T("Software\\Microsoft\\Windows\\CurrentVersion\\Run");
	if(RegOpenKey(HKEY_CURRENT_USER,keyStr,&hRegKey) !=  ERROR_SUCCESS)
		return ;
	else
	{
		TCHAR path[MAX_PATH];
		GetModuleFileName(NULL,path,sizeof(path));	
		CString strPath = path;
		//���fileName
		_splitpath(strPath.GetBuffer(0),NULL,NULL,fileName.GetBufferSetLength(MAX_PATH+1),NULL);
	//	AfxMessageBox(fileName);
		lgtResult=RegSetValueEx(hRegKey,fileName,0,REG_SZ,(CONST BYTE *)strPath.GetBuffer(0),strPath.GetLength());
		if(lgtResult!=ERROR_SUCCESS)
		{
			AfxMessageBox("����ʧ��");
			return ;
		}
		strPath.ReleaseBuffer();
		AfxMessageBox("���óɹ�");
		RegCloseKey(hRegKey);
	}
}
/* ��������NoAutoRun

  �������ܣ�������������
  �����ߣ�Peter
  ����ʱ�䣺2015-8-24
  �޸�ʱ��: 
  �޸����ݣ�                                              */
/************************************************************************/
void CDirectoryMonitorDlg::NoAutoRun()
{
	CString keyStr;
	LONG lgtResult;
	CString fileName;
	HKEY hRegKey;
	
	keyStr =_T("Software\\Microsoft\\Windows\\CurrentVersion\\Run");
	if(RegOpenKey(HKEY_CURRENT_USER,keyStr,&hRegKey) !=  ERROR_SUCCESS)
		return ;
	else
	{
		TCHAR path[MAX_PATH];
		GetModuleFileName(NULL,path,sizeof(path));	
		CString strPath = path;
		//���fileName
		_splitpath(strPath.GetBuffer(0),NULL,NULL,fileName.GetBufferSetLength(MAX_PATH+1),NULL);
		//AfxMessageBox(fileName);
		lgtResult=RegDeleteValue(hRegKey,fileName);
		if(lgtResult!=ERROR_SUCCESS)
		{
			AfxMessageBox("����ʧ��");
			return ;
		}
		strPath.ReleaseBuffer();
		AfxMessageBox("���óɹ�");
		RegCloseKey(hRegKey);
	}
}
/* ��������InitTray

  �������ܣ���ʼ������
  �����ߣ�Peter
  ����ʱ�䣺2015-8-24
  �޸�ʱ��: 
  �޸����ݣ�                                              */
/************************************************************************/
void CDirectoryMonitorDlg::InitTray()
{
	nid.cbSize=(DWORD)sizeof(NOTIFYICONDATA);  
     nid.hWnd=this->m_hWnd;   
     nid.uID=IDR_MAINFRAME;   
    nid.uFlags=NIF_ICON|NIF_MESSAGE|NIF_TIP ;   
	nid.uCallbackMessage=WM_TRAY;//�Զ������Ϣ����   
	nid.hIcon=LoadIcon(AfxGetInstanceHandle(),MAKEINTRESOURCE(IDR_MAINFRAME));   
    strcpy(nid.szTip,_T("�ļ����ϵͳ")); //��Ϣ��ʾ��   
    Shell_NotifyIcon(NIM_ADD,&nid); //�����������ͼ�� 
}
/* ��������OnTrayNotify

  �������ܣ�������Ϣ������
  �����ߣ�Peter
  ����ʱ�䣺2015-8-24
  �޸�ʱ��: 
  �޸����ݣ�                                              */
/************************************************************************/
LRESULT CDirectoryMonitorDlg::OnTrayNotify(WPARAM wParam, LPARAM lParam)
{
	if(wParam!=IDR_MAINFRAME)   
        return 1;   
	
    switch(lParam)   
    {  
    case WM_RBUTTONUP://�Ҽ�����ʱ������ݲ˵�������ֻ��һ�����رա�   
        {  
            LPPOINT lpoint=new tagPOINT;   
            GetCursorPos(lpoint);//�õ����λ��   
            CMenu menu;   
            menu.CreatePopupMenu();//����һ������ʽ�˵�   
            //���Ӳ˵���رա������������ϢWM_DESTROY�������ڣ������أ��������������   
            menu.AppendMenu(MF_STRING,WM_DESTROY,"�ر�"); //ȷ������ʽ�˵���λ��   
            SetForegroundWindow(); // ��ֹδѡ�˵�����ʧ  
            menu.TrackPopupMenu(TPM_LEFTALIGN,lpoint->x,lpoint->y,this); //��Դ����   
            HMENU hmenu=menu.Detach();   
            menu.DestroyMenu();   
            delete lpoint;   
        }   
        break;   
    case WM_LBUTTONDBLCLK://˫������Ĵ���   
        {  
            //����ǰ����ʾ  
            SetForegroundWindow();  
            ShowWindow(SW_SHOWNORMAL);  
        }   
        break;   
    default: break;   
    }   
    return 0;  
}
/* ��������OnClose

  �������ܣ��ر�Ӧ�ó���
  �����ߣ�Peter
  ����ʱ�䣺2015-8-24
  �޸�ʱ��: 
  �޸����ݣ�                                              */
/************************************************************************/
void CDirectoryMonitorDlg::OnClose() 
{
	// TODO: Add your message handler code here and/or call default
	
	CString strTitle,strText;
	strText=_T("��ȷ��Ҫ�˳�������");
	strTitle=_T("���̳���");
	int result;
	result=MessageBox(strText,strTitle,MB_ICONQUESTION|MB_OKCANCEL);
	if(IDOK==result)
	{
		Shell_NotifyIcon(NIM_DELETE,&nid); 
	}
	CDialog::OnClose();
}
/* ��������OnSize

  �������ܣ���С��������
  �����ߣ�Peter
  ����ʱ�䣺2015-8-24
  �޸�ʱ��: 
  �޸����ݣ�                                              */
/************************************************************************/
void CDirectoryMonitorDlg::OnSize(UINT nType, int cx, int cy) 
{
	CDialog::OnSize(nType, cx, cy);
	
	// TODO: Add your message handler code here
	if (nType==SIZE_MINIMIZED)
	{
		ShowWindow(SW_HIDE);
	}
}

int CDirectoryMonitorDlg::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	if (CDialog::OnCreate(lpCreateStruct) == -1)
		return -1;
	
	// TODO: Add your specialized creation code here
	CSplashWnd::ShowSplashScreen(this);
    Sleep(3000);

	return 0;
}

BOOL CAboutDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	CFileVersion cf;
	cf.Open("DirectoryMonitor.exe");
	CString str;
	str=cf.GetProductName();
	SetDlgItemText(IDC_PRODUCTNAME,str);
	str=cf.GetLegalCopyright();
	SetDlgItemText(IDC_COPYRIGHT,str);
	str=cf.GetProductVersion();
	SetDlgItemText(IDC_VERSION,str);
	str=cf.GetCompanyName();
	SetDlgItemText(IDC_COMPANY,str);
	str=cf.GetComments();
	SetDlgItemText(IDC_AUTHER,str);
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}
