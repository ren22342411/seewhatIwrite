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
	
	(GetDlgItem(IDC_EDITRECV))->SetWindowText("��δ׼���ý���");
    (GetDlgItem(IDC_EDITSEND))->SetWindowText("��δ׼���÷���");
	
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
    HWND hwnd=((sockrecv*)lpParameter)->hwnd;//���������ݵ����ر���
	CString recvfname;
	
	SOCKET socketrecv=socket(AF_INET,SOCK_STREAM,0); //�����׽���
	if(INVALID_SOCKET==socketrecv)
	{
        closesocket(socketrecv);
		::MessageBox(hwnd,"�׽��ִ���ʧ��!","����",MB_OK);
		return FALSE;
	}
	
	SOCKADDR_IN addrSrv;
	addrSrv.sin_addr.S_un.S_addr=htonl(INADDR_ANY);
	addrSrv.sin_family=AF_INET;
	addrSrv.sin_port=htons(8848);
    
	int ret1;
	ret1=bind(socketrecv,(SOCKADDR*)&addrSrv,sizeof(SOCKADDR)); //���׽���
	if(SOCKET_ERROR==ret1)
	{
		closesocket(socketrecv);
		::MessageBox(hwnd,"��ʧ��!","����",MB_OK);
		
		return FALSE;
	}
	
	int ret2;


	ret2=listen(socketrecv,5);					//��ʼ����
	if(SOCKET_ERROR==ret2)
	{
		closesocket(socketrecv);
		::MessageBox(hwnd,"����ʧ��!","����",MB_OK);
		return FALSE;
	}
	
    SOCKADDR_IN addrconn;
	int len=sizeof(SOCKADDR);
	
	
	
	::PostMessage(hwnd,UM_PRGRRDY,0,0);        //�������ڷ�����Ϣ
	
	
	SOCKET sockConn=accept(socketrecv,(SOCKADDR*)&addrconn,&len);   //��ʼ����
	if(INVALID_SOCKET ==sockConn)		
	{
		closesocket(socketrecv);
		::MessageBox(hwnd,"����ʧ��!","����",MB_OK);
		return FALSE;
	}
	
	
	::PostMessage(hwnd,UM_PRGRBGN,0,0);			//�������ڷ�����Ϣ
	
	BOOL bRet = TRUE; 
    
    int dataLength, cbBytesRet, cbLeftToReceive;
    
    BYTE* recdData = NULL;
    
    CFile destFile;
    CFileException fe;
    BOOL bFileIsOpen = FALSE;
	

	
	//�����յ��򱾵ط����ļ�������ʱ�����ļ�����Ի���
	CFileDialog fileDlg(FALSE);
    fileDlg.m_ofn.lpstrTitle="���潫���յ����ļ�";
	fileDlg.m_ofn.lpstrFilter="All Files(*.*)\0*.*\0\0";
	if(IDOK==fileDlg.DoModal())
	{
		recvfname=fileDlg.GetPathName();
	}
	
	
	//�ô��ļ�����Ի����еõ����ļ�����ָ��λ�ô����ļ�
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
	
	//���Ƚ����ļ��ĳ�����Ϣ
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
	
	
	
    dataLength = ntohl( dataLength );  //���ļ��ĳ�����Ϣת��Ϊ�����ֽ���
	
    
    //׼����ʼ�����ļ�
    recdData = new byte[RECV_BUFFER_SIZE];
    cbLeftToReceive = dataLength;
    
    do
    {    
        //�������ڷ�����Ϣ��Ҫ���ڿ��ƽ�����
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
	
	::PostMessage(hwnd,UM_PRGROVER,0,0);   //�������ڷ�����Ϣ
	
PreReturnCleanup:						   //�ļ����ս�����ǩ
    
    delete[] recdData;					   //�ͷ��ڴ�
    
    if ( bFileIsOpen )
        destFile.Close();
	
    
    closesocket(sockConn);
	closesocket(socketrecv);
    
    return bRet;
	
}



DWORD WINAPI CWinSockDlg::SocketSend(LPVOID lpParameter)
{
	//���������ݸ����ر���
	HWND hwnd=((socksend*)lpParameter)->hwnd;
    CString sendfname=((socksend*)lpParameter)->sendfname;
	DWORD dwip=((socksend*)lpParameter)->dwip;
	
	SOCKET socketsend=socket(AF_INET,SOCK_STREAM,0);//����Socket
	
	SOCKADDR_IN addrSrv;
	addrSrv.sin_addr.S_un.S_addr=htonl(dwip);
	addrSrv.sin_family=AF_INET;
	addrSrv.sin_port=htons(8848);
	
	int ret1;
	ret1=connect(socketsend,(SOCKADDR*)&addrSrv,sizeof(SOCKADDR));//����ָ����ַ
	
    if(SOCKET_ERROR==ret1)
	{
		closesocket(socketsend);
		::MessageBox(hwnd,"����ʧ��!","����",MB_OK);
		return FALSE;
	}
	
	
	::PostMessage(hwnd,UM_PRGSBGN,0,0);				//�������ڷ�����Ϣ


    //���������ļ����͵ı��ر���
	BOOL bRet = TRUE;
    
    int fileLength, cbLeftToSend;
    
    
    BYTE* sendData = NULL;
	
    CFile sourceFile;
    CFileException fe;
    BOOL bFileIsOpen = FALSE;
    

	//�򿪲������ݽ������ļ����ڷ���
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
	
    
	//���ȴ��ݽ�Ҫ�������ļ��ĳ��ȸ����ն�
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
    
    
    //��ʼ�������
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
    
	
	::PostMessage(hwnd,UM_PRGSOVER,0,0);//�������ڷ�����Ϣ�������
    
    PreReturnCleanup:					//������ǩ���ͷ��ڴ�
    
    
    
    
    delete[] sendData;
    
    if ( bFileIsOpen )
        sourceFile.Close();
	
	
	closesocket(socketsend);
    
    return bRet;
	
	
}



void CWinSockDlg::OnOpen() 
{
	
	CFileDialog fileDlg(TRUE);				//�������ļ��Ի���
    fileDlg.m_ofn.lpstrTitle="���ļ�";
	fileDlg.m_ofn.lpstrFilter="All Files(*.*)\0*.*\0\0"; //�趨ɸѡ
	if(IDOK==fileDlg.DoModal())
	{
		m_sendfname=fileDlg.GetPathName();     
		]
		//����û�ѡ��ȷ�����÷����ļ������ԶԻ�����ʾ����Ӧ����
	
	    (GetDlgItem(IDC_EDIT1))->SetWindowText(m_sendfname);
	
	    (GetDlgItem(IDC_EDITSEND))->SetWindowText("׼���÷���");
	    Invalidate();
	}
	
	
}

void CWinSockDlg::OnRecv() 
{
	sockrecv* sr=new sockrecv;	//����һ�����ղ����ṹ��
	sr->hwnd=m_hWnd;			//�������ھ�����͸���
	HANDLE handl=CreateThread(NULL,0,SocketRecv,(LPVOID)sr,0,NULL);//�����߳����ڽ���
	CloseHandle(handl);
	
}

void CWinSockDlg::OnSend() 
{
    socksend* ss=new socksend;  //����һ�����Ͳ����ṹ��
	
	//ȡ��IP��ַ�ؼ��ϵĵ�ֵַ,�趨�����ļ���,���������ھ��
	((CIPAddressCtrl*)GetDlgItem(IDC_IPADDRESS1))->GetAddress(ss->dwip);
    ss->sendfname=m_sendfname;
	ss->hwnd=m_hWnd;
	
	HANDLE handl=CreateThread(NULL,0,SocketSend,(LPVOID)ss,0,NULL);
	CloseHandle(handl);
	
}

afx_msg void CWinSockDlg::onprgrecv(WPARAM wParam,LPARAM lParam)
{
    //���ƽ��ս���������Ϣ��Ӧ����
	int prgnow=(int)lParam;				//��ʣ������ֽ���Ϊ�������ݸ�����
	if(m_beginprgrecv)					//�ж��Ƿ��һ�νӵ�����Ϣ
	{
		m_prgrecv.SetRange32(0,prgnow); //���ý�������Χ
        m_prgrecvpre=prgnow;			//����Ա������ֵ	
		m_beginprgrecv=FALSE;			//�ı��־
	}
    int thistran=m_prgrecvpre-prgnow;   //���δ����ֽ���
	int now=m_prgrecv.GetPos();         //��ǰ���ܴ�����
	m_prgrecv.SetPos(thistran+now);		//���ý�����λ��

}

afx_msg void CWinSockDlg::onprgrover(WPARAM wParam,LPARAM lParam)
{
	//�ָ�����״̬
	m_prgrecv.SetPos(0);
	m_prgrecvpre=0;
    m_beginprgrecv=TRUE;

	(GetDlgItem(IDC_EDITRECV))->SetWindowText("��δ׼���ý���");
    
}

afx_msg void CWinSockDlg::onprgsend(WPARAM wParam,LPARAM lParam)
{
	//��ο�������Ϣ��Ӧ��������
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
	//�ָ�����״̬
	m_prgsend.SetPos(0);
	m_prgsendpre=0;
    m_beginprgsend=TRUE;
	(GetDlgItem(IDC_EDITSEND))->SetWindowText("��δ׼���÷���");
}


afx_msg void CWinSockDlg::onprgrrdy(WPARAM wParam,LPARAM lParam)
{
(GetDlgItem(IDC_EDITRECV))->SetWindowText("׼���ý���");//UM_PRGRRDY��Ϣ��Ӧ
}

afx_msg void CWinSockDlg::onprgrbgn(WPARAM wParam,LPARAM lParam)
{
(GetDlgItem(IDC_EDITRECV))->SetWindowText("��ʼ����");  //UM_PRGRBGN��Ϣ��Ӧ

}
afx_msg void CWinSockDlg::onprgsbgn(WPARAM wParam,LPARAM lParam) //UM_PRGSBGN��Ϣ��Ӧ
{
(GetDlgItem(IDC_EDITSEND))->SetWindowText("��ʼ����");
}
