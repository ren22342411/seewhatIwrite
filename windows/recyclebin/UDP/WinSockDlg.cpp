// WinSockDlg.cpp : implementation file
//

#include "stdafx.h"
#include "WinSock.h"
#include "WinSockDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
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
	
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CAboutDlg)
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL
	
	// Implementation
protected:
	//{{AFX_MSG(CAboutDlg)
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
// CWinSockDlg dialog

CWinSockDlg::CWinSockDlg(CWnd* pParent /*=NULL*/)
: CDialog(CWinSockDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CWinSockDlg)
	// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	// Note that LoadIcon does not require a subsequent DestroyIcon in Win32
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
	
	m_beginprgrecv=TRUE;
	m_beginprgsend=TRUE;
}

void CWinSockDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CWinSockDlg)
	DDX_Control(pDX, IDC_PROGRESSSEND, m_prgsend);
	DDX_Control(pDX, IDC_PROGRESSRECV, m_prgrecv);
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CWinSockDlg, CDialog)
//{{AFX_MSG_MAP(CWinSockDlg)
ON_WM_SYSCOMMAND()
ON_WM_PAINT()
ON_WM_QUERYDRAGICON()
ON_BN_CLICKED(IDC_OPEN, OnOpen)
ON_BN_CLICKED(IDC_RECV, OnRecv)
ON_BN_CLICKED(IDC_SEND, OnSend)
	//}}AFX_MSG_MAP
ON_MESSAGE(UM_PRGRECV,onprgrecv)
ON_MESSAGE(UM_PRGROVER,onprgrover)
ON_MESSAGE(UM_PRGSEND,onprgsend)
ON_MESSAGE(UM_PRGSOVER,onprgsover)
ON_MESSAGE(UM_PRGRRDY,onprgrrdy)
ON_MESSAGE(UM_PRGRBGN,onprgrbgn)
ON_MESSAGE(UM_PRGSBGN,onprgsbgn)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CWinSockDlg message handlers

BOOL CWinSockDlg::OnInitDialog()
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
	
	(GetDlgItem(IDC_EDITRECV))->SetWindowText("尚未准备好接收");
    (GetDlgItem(IDC_EDITSEND))->SetWindowText("尚未准备好发送");
	
	// TODO: Add extra initialization here
	
	return TRUE;  // return TRUE  unless you set the focus to a control
}

void CWinSockDlg::OnSysCommand(UINT nID, LPARAM lParam)
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

void CWinSockDlg::OnPaint() 
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
HCURSOR CWinSockDlg::OnQueryDragIcon()
{
	return (HCURSOR) m_hIcon;
}

DWORD WINAPI CWinSockDlg::SocketRecv(LPVOID lpParameter)
{
    HWND hwnd=((sockrecv*)lpParameter)->hwnd;//将参数传递到本地变量
	CString recvfname;
	
	SOCKET socketrecv=socket(AF_INET,SOCK_STREAM,0); //创建套接字
	if(INVALID_SOCKET==socketrecv)
	{
        closesocket(socketrecv);
		::MessageBox(hwnd,"套接字创建失败!","警告",MB_OK);
		return FALSE;
	}
	
	SOCKADDR_IN addrSrv;
	addrSrv.sin_addr.S_un.S_addr=htonl(INADDR_ANY);
	addrSrv.sin_family=AF_INET;
	addrSrv.sin_port=htons(8848);
    
	int ret1;
	ret1=bind(socketrecv,(SOCKADDR*)&addrSrv,sizeof(SOCKADDR)); //绑定套接字
	if(SOCKET_ERROR==ret1)
	{
		closesocket(socketrecv);
		::MessageBox(hwnd,"绑定失败!","警告",MB_OK);
		
		return FALSE;
	}
	
	int ret2;


	ret2=listen(socketrecv,5);					//开始监听
	if(SOCKET_ERROR==ret2)
	{
		closesocket(socketrecv);
		::MessageBox(hwnd,"监听失败!","警告",MB_OK);
		return FALSE;
	}
	
    SOCKADDR_IN addrconn;
	int len=sizeof(SOCKADDR);
	
	
	
	::PostMessage(hwnd,UM_PRGRRDY,0,0);        //向主窗口发送消息
	
	
	SOCKET sockConn=accept(socketrecv,(SOCKADDR*)&addrconn,&len);   //开始接收
	if(INVALID_SOCKET ==sockConn)		
	{
		closesocket(socketrecv);
		::MessageBox(hwnd,"接收失败!","警告",MB_OK);
		return FALSE;
	}
	
	
	::PostMessage(hwnd,UM_PRGRBGN,0,0);			//向主窗口发送消息
	
	BOOL bRet = TRUE; 
    
    int dataLength, cbBytesRet, cbLeftToReceive;
    
    BYTE* recdData = NULL;
    
    CFile destFile;
    CFileException fe;
    BOOL bFileIsOpen = FALSE;
	

	
	//当接收到向本地发送文件的请求时弹出文件保存对话框
	CFileDialog fileDlg(FALSE);
    fileDlg.m_ofn.lpstrTitle="保存将接收到的文件";
	fileDlg.m_ofn.lpstrFilter="All Files(*.*)\0*.*\0\0";
	if(IDOK==fileDlg.DoModal())
	{
		recvfname=fileDlg.GetPathName();
	}
	
	
	//用从文件保存对话框中得到的文件名在指定位置创建文件
	if( !( bFileIsOpen = destFile.Open( recvfname, CFile::modeCreate | 
		CFile::modeWrite | CFile::typeBinary, &fe ) ) )
	{
		TCHAR strCause[256];
		fe.GetErrorMessage( strCause, 255 );
		TRACE( "GetFileFromRemoteSender encountered an error while opening the local file\n"
			"\tFile name = %s\n\tCause = %s\n\tm_cause = %d\n\tm_IOsError = %d\n",
			fe.m_strFileName, strCause, fe.m_cause, fe.m_lOsError );
        
		
        
		bRet = FALSE;
		goto PreReturnCleanup;
	}
	
	//首先接收文件的长度信息
	cbLeftToReceive = sizeof( dataLength );   
    
    do
    {
        BYTE* bp = (BYTE*)(&dataLength) + sizeof(dataLength) - cbLeftToReceive;
        cbBytesRet = recv(sockConn, (char*)bp, cbLeftToReceive,0);
        
        
        if ( cbBytesRet == SOCKET_ERROR || cbBytesRet == 0 )
        {
            int iErr = ::GetLastError();
            TRACE( "GetFileFromRemoteSite returned a socket error while getting file length\n"
				"\tNumber of Bytes received (zero means connection was closed) = %d\n"
				"\tGetLastError = %d\n", cbBytesRet, iErr );
            
			
            
            bRet = FALSE;
            goto PreReturnCleanup;
        }
        
		
        
        cbLeftToReceive -= cbBytesRet;
        
    }
    while ( cbLeftToReceive > 0 );
	
	
	
    dataLength = ntohl( dataLength );  //将文件的长度信息转化为本地字节序
	
    
    //准备开始接收文件
    recdData = new byte[RECV_BUFFER_SIZE];
    cbLeftToReceive = dataLength;
    
    do
    {    
        //向主窗口发送消息主要用于控制进度条
		::PostMessage(hwnd,UM_PRGRECV,0,(LPARAM)cbLeftToReceive);
		int iiGet, iiRecd;
		
        iiGet = (cbLeftToReceive<RECV_BUFFER_SIZE) ? 
cbLeftToReceive : RECV_BUFFER_SIZE ;
        iiRecd = recv(sockConn, (char*)recdData, iiGet,0 );
		
        
        if ( iiRecd == SOCKET_ERROR || iiRecd == 0 )
        {
            int iErr = ::GetLastError();
            TRACE( "GetFileFromRemoteSite returned a socket error while getting chunked file data\n"
                "\tNumber of Bytes received (zero means connection was closed) = %d\n"
                "\tGetLastError = %d\n", iiRecd, iErr );
            
			
            
            bRet = FALSE;
            goto PreReturnCleanup;
        }
		
        
        destFile.Write( recdData, iiRecd); 
        cbLeftToReceive -= iiRecd;
		
    } 
    while ( cbLeftToReceive > 0 );
	
	::PostMessage(hwnd,UM_PRGROVER,0,0);   //向主窗口发送消息
	
PreReturnCleanup:						   //文件接收结束标签
    
    delete[] recdData;					   //释放内存
    
    if ( bFileIsOpen )
        destFile.Close();
	
    
    closesocket(sockConn);
	closesocket(socketrecv);
    
    return bRet;
	
}



DWORD WINAPI CWinSockDlg::SocketSend(LPVOID lpParameter)
{
	//将参数传递给本地变量
	HWND hwnd=((socksend*)lpParameter)->hwnd;
    CString sendfname=((socksend*)lpParameter)->sendfname;
	DWORD dwip=((socksend*)lpParameter)->dwip;
	
	SOCKET socketsend=socket(AF_INET,SOCK_STREAM,0);//创建Socket
	
	SOCKADDR_IN addrSrv;
	addrSrv.sin_addr.S_un.S_addr=htonl(dwip);
	addrSrv.sin_family=AF_INET;
	addrSrv.sin_port=htons(8848);
	
	int ret1;
	ret1=connect(socketsend,(SOCKADDR*)&addrSrv,sizeof(SOCKADDR));//连接指定地址
	
    if(SOCKET_ERROR==ret1)
	{
		closesocket(socketsend);
		::MessageBox(hwnd,"连接失败!","警告",MB_OK);
		return FALSE;
	}
	
	
	::PostMessage(hwnd,UM_PRGSBGN,0,0);				//向主窗口发送消息


    //声明用于文件发送的本地变量
	BOOL bRet = TRUE;
    
    int fileLength, cbLeftToSend;
    
    
    BYTE* sendData = NULL;
	
    CFile sourceFile;
    CFileException fe;
    BOOL bFileIsOpen = FALSE;
    

	//打开参数传递进来的文件用于发送
    if( !( bFileIsOpen = sourceFile.Open( sendfname, 
		CFile::modeRead | CFile::typeBinary, &fe ) ) )
    {
        TCHAR strCause[256];
        fe.GetErrorMessage( strCause, 255 );
        TRACE( "SendFileToRemoteRecipient encountered an error while opening the local file\n"
			"\tFile name = %s\n\tCause = %s\n\tm_cause = %d\n\tm_IOsError = %d\n",
			fe.m_strFileName, strCause, fe.m_cause, fe.m_lOsError );
        
        
        
        bRet = FALSE;
        goto PreReturnCleanup;
    }
	
    
	//首先传递将要被发送文件的长度给接收端
    fileLength = sourceFile.GetLength();
    fileLength = htonl( fileLength );
	
    cbLeftToSend = sizeof( fileLength );
	
    do
    {
        int cbBytesSent;
        BYTE* bp = (BYTE*)(&fileLength) + sizeof(fileLength) - cbLeftToSend;
        cbBytesSent = send(socketsend, (const char*)bp, cbLeftToSend,0 );
        
        
        if ( cbBytesSent == SOCKET_ERROR )
        {
            int iErr = ::GetLastError();
            TRACE( "SendFileToRemoteRecipient returned a socket error while sending file length\n"
                "\tNumber of Bytes sent = %d\n"
                "\tGetLastError = %d\n", cbBytesSent, iErr );
            
            
			
            bRet = FALSE;
            goto PreReturnCleanup;
        }
        
        
        cbLeftToSend -= cbBytesSent;
    }
    while ( cbLeftToSend>0 );
    
    
    //开始发送晚间
    sendData = new BYTE[SEND_BUFFER_SIZE]; 
    
    cbLeftToSend = sourceFile.GetLength();
    
    do
    {
        ::PostMessage(hwnd,UM_PRGSEND,0,(LPARAM)cbLeftToSend);
        
        int sendThisTime, doneSoFar, buffOffset;
        
        sendThisTime = sourceFile.Read( sendData, SEND_BUFFER_SIZE );
        buffOffset = 0;
        
        do
        {
            
			
			doneSoFar = send(socketsend,(const char*) (sendData + buffOffset), 
				sendThisTime,0 ); 
            
            
            if ( doneSoFar == SOCKET_ERROR )
            {
                int iErr = ::GetLastError();
                TRACE( "SendFileToRemoteRecipient returned a socket error while sending chunked file data\n"
					"\tNumber of Bytes sent = %d\n"
					"\tGetLastError = %d\n", doneSoFar, iErr );
				
				
				
				bRet = FALSE;
				goto PreReturnCleanup;
            }
            
            
            
            buffOffset += doneSoFar;
            sendThisTime -= doneSoFar;
            cbLeftToSend -= doneSoFar;
        }
        while ( sendThisTime > 0 );
        
    }
    while ( cbLeftToSend > 0 );
    
	
	::PostMessage(hwnd,UM_PRGSOVER,0,0);//向主窗口发送消息发送完毕
    
    PreReturnCleanup:					//结束标签后释放内存
    
    
    
    
    delete[] sendData;
    
    if ( bFileIsOpen )
        sourceFile.Close();
	
	
	closesocket(socketsend);
    
    return bRet;
	
	
}



void CWinSockDlg::OnOpen() 
{
	
	CFileDialog fileDlg(TRUE);				//创建打开文件对话框
    fileDlg.m_ofn.lpstrTitle="打开文件";
	fileDlg.m_ofn.lpstrFilter="All Files(*.*)\0*.*\0\0"; //设定筛选
	if(IDOK==fileDlg.DoModal())
	{
		m_sendfname=fileDlg.GetPathName();     
		]
		//如果用户选择确定设置发送文件名并对对话框显示做响应调整
	
	    (GetDlgItem(IDC_EDIT1))->SetWindowText(m_sendfname);
	
	    (GetDlgItem(IDC_EDITSEND))->SetWindowText("准备好发送");
	    Invalidate();
	}
	
	
}

void CWinSockDlg::OnRecv() 
{
	sockrecv* sr=new sockrecv;	//声明一个接收参数结构体
	sr->hwnd=m_hWnd;			//将主窗口句柄传送给它
	HANDLE handl=CreateThread(NULL,0,SocketRecv,(LPVOID)sr,0,NULL);//创建线程用于接收
	CloseHandle(handl);
	
}

void CWinSockDlg::OnSend() 
{
    socksend* ss=new socksend;  //声明一个发送参数结构体
	
	//取得IP地址控件上的地址值,设定发送文件名,传递主窗口句柄
	((CIPAddressCtrl*)GetDlgItem(IDC_IPADDRESS1))->GetAddress(ss->dwip);
    ss->sendfname=m_sendfname;
	ss->hwnd=m_hWnd;
	
	HANDLE handl=CreateThread(NULL,0,SocketSend,(LPVOID)ss,0,NULL);
	CloseHandle(handl);
	
}

afx_msg void CWinSockDlg::onprgrecv(WPARAM wParam,LPARAM lParam)
{
    //控制接收进度条的消息响应函数
	int prgnow=(int)lParam;				//将剩余接收字节作为参数传递给函数
	if(m_beginprgrecv)					//判断是否第一次接到此消息
	{
		m_prgrecv.SetRange32(0,prgnow); //设置进度条范围
        m_prgrecvpre=prgnow;			//给成员变量赋值	
		m_beginprgrecv=FALSE;			//改变标志
	}
    int thistran=m_prgrecvpre-prgnow;   //本次传输字节数
	int now=m_prgrecv.GetPos();         //先前的总传输量
	m_prgrecv.SetPos(thistran+now);		//设置进度条位置

}

afx_msg void CWinSockDlg::onprgrover(WPARAM wParam,LPARAM lParam)
{
	//恢复接收状态
	m_prgrecv.SetPos(0);
	m_prgrecvpre=0;
    m_beginprgrecv=TRUE;

	(GetDlgItem(IDC_EDITRECV))->SetWindowText("尚未准备好接收");
    
}

afx_msg void CWinSockDlg::onprgsend(WPARAM wParam,LPARAM lParam)
{
	//请参考接收消息响应函数解释
	int prgnow=(int)lParam;
	if(m_beginprgsend)
	{
		m_prgsend.SetRange32(0,prgnow);
        m_prgsendpre=prgnow;
		m_beginprgsend=FALSE;
	}
    int thistran=m_prgsendpre-prgnow;
	int now=m_prgsend.GetPos();
	m_prgsend.SetPos(thistran+now);

}

afx_msg void CWinSockDlg::onprgsover(WPARAM wParam,LPARAM lParam)
{
	//恢复发送状态
	m_prgsend.SetPos(0);
	m_prgsendpre=0;
    m_beginprgsend=TRUE;
	(GetDlgItem(IDC_EDITSEND))->SetWindowText("尚未准备好发送");
}


afx_msg void CWinSockDlg::onprgrrdy(WPARAM wParam,LPARAM lParam)
{
(GetDlgItem(IDC_EDITRECV))->SetWindowText("准备好接收");//UM_PRGRRDY消息响应
}

afx_msg void CWinSockDlg::onprgrbgn(WPARAM wParam,LPARAM lParam)
{
(GetDlgItem(IDC_EDITRECV))->SetWindowText("开始接收");  //UM_PRGRBGN消息响应

}
afx_msg void CWinSockDlg::onprgsbgn(WPARAM wParam,LPARAM lParam) //UM_PRGSBGN消息响应
{
(GetDlgItem(IDC_EDITSEND))->SetWindowText("开始发送");
}
