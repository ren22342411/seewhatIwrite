; CLW file contains information for the MFC ClassWizard

[General Info]
Version=1
LastClass=CWinSockDlg
LastTemplate=CDialog
NewFileInclude1=#include "stdafx.h"
NewFileInclude2=#include "WinSock.h"

ClassCount=3
Class1=CWinSockApp
Class2=CWinSockDlg
Class3=CAboutDlg

ResourceCount=3
Resource1=IDD_ABOUTBOX
Resource2=IDR_MAINFRAME
Resource3=IDD_WINSOCK_DIALOG

[CLS:CWinSockApp]
Type=0
HeaderFile=WinSock.h
ImplementationFile=WinSock.cpp
Filter=N

[CLS:CWinSockDlg]
Type=0
HeaderFile=WinSockDlg.h
ImplementationFile=WinSockDlg.cpp
Filter=D
BaseClass=CDialog
VirtualFilter=dWC

[CLS:CAboutDlg]
Type=0
HeaderFile=WinSockDlg.h
ImplementationFile=WinSockDlg.cpp
Filter=D

[DLG:IDD_ABOUTBOX]
Type=1
Class=CAboutDlg
ControlCount=4
Control1=IDC_STATIC,static,1342177283
Control2=IDC_STATIC,static,1342308480
Control3=IDC_STATIC,static,1342308352
Control4=IDOK,button,1342373889

[DLG:IDD_WINSOCK_DIALOG]
Type=1
Class=CWinSockDlg
ControlCount=11
Control1=IDC_IPADDRESS1,SysIPAddress32,1342242816
Control2=IDC_EDIT1,edit,1350631552
Control3=IDC_OPEN,button,1342242816
Control4=IDC_PROGRESSRECV,msctls_progress32,1350565888
Control5=IDC_STATIC,static,1342308352
Control6=IDC_PROGRESSSEND,msctls_progress32,1350565888
Control7=IDC_STATIC,static,1342308352
Control8=IDC_RECV,button,1342242816
Control9=IDC_SEND,button,1342242816
Control10=IDC_EDITRECV,edit,1350633600
Control11=IDC_EDITSEND,edit,1350633600

