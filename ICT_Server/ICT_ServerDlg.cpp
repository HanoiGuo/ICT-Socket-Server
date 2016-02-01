
// ICT_ServerDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "ICT_Server.h"
#include "ICT_ServerDlg.h"
#include "afxdialogex.h"
#include "Resource.h"
#include <WinSock2.h>
#include <Windows.h>
#include <iostream>
#include <string.h>
#include <string>
#include <Iphlpapi.h>
#include <shlwapi.h>

#pragma comment(lib,"Shlwapi.lib") //如果没有这行，会出现link错误
#pragma  comment(lib,"ws2_32.lib")
#pragma comment(lib,"Iphlpapi.lib") //mac地址

using namespace std;
#ifdef _DEBUG
#define new DEBUG_NEW
#endif

CString str;
static int GlobalState = 0;
static int OldGlobalState = 0;
static bool bStartAgain = false;
static bool isClose = false;
HANDLE hThread = NULL;
SOCKET ServerGlobal;
SOCKET ClientGlobal;

#define WM_SHOWTASK (WM_USER +1)
// 用于应用程序“关于”菜单项的 CAboutDlg 对话框

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// 对话框数据
	enum { IDD = IDD_ABOUTBOX };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

// 实现
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


// CICT_ServerDlg 对话框
CICT_ServerDlg::CICT_ServerDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CICT_ServerDlg::IDD, pParent)
{
	//m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
	m_hIcon = AfxGetApp()->LoadIcon(IDI_ICON_LOG);
	//构造函数赋初始值
	port = 0;
	memset(ipAddress,0,sizeof(ipAddress));
}

void CICT_ServerDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_STATIC_TITLE, m_staticTitle);
	DDX_Control(pDX, IDC_STATIC_BAR, m_staticBar);
	DDX_Control(pDX, IDC_STATIC_RESULT, m_result);
	DDX_Control(pDX, IDC_STATIC_LOG, m_StaticLog);
	DDX_Control(pDX, IDC_STATIC_SHOWSTATE, m_state);
}

BEGIN_MESSAGE_MAP(CICT_ServerDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_BUTTON_CONNECT, &CICT_ServerDlg::OnBnClickedButtonConnect)
	ON_WM_TIMER()
	ON_BN_CLICKED(IDC_BUTTON_CLOSE, &CICT_ServerDlg::OnBnClickedButtonClose)
	ON_WM_CLOSE()
	ON_MESSAGE(WM_SHOWTASK,OnShowTask)
END_MESSAGE_MAP()


// CICT_ServerDlg 消息处理程序

BOOL CICT_ServerDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// 将“关于...”菜单项添加到系统菜单中。

	// IDM_ABOUTBOX 必须在系统命令范围内。
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

	// 设置此对话框的图标。当应用程序主窗口不是对话框时，框架将自动
	//  执行此操作
	SetIcon(m_hIcon, TRUE);			// 设置大图标
	SetIcon(m_hIcon, FALSE);		// 设置小图标

	// TODO: 在此添加额外的初始化代码
	m_staticTitle.SetTransparent(TRUE);
	m_staticTitle.SetBkClr(RGB(159,227,251));
	m_staticTitle.SetTextClr(RGB(255,0,0));
	m_staticTitle.SetWindowText(_T("Bojay ICT Server"));
	m_staticTitle.SetTextFont(40, 1, 0, _T("宋体") );//设置字体
	GetDlgItem(IDC_BUTTON_CLOSE)->EnableWindow(FALSE);

	bool res = false;
	res = file.ReadConfig("config.txt",ipAddress,port);
	if (!res)
	{
		str = "\n\n\t没有配置文件,默认端口为8888";
		GetDlgItem(IDC_STATIC_SHOWSTATE)->SetWindowText(str);
	}
	string serial;
	file.ReadSerialFile("Serial.dat",serial);
	int macNum = 0;
	char macList[10][18] = {0};
	GetSysMac(macNum,macList);
	//只检查第一组的MAC地址
	res = CheckMACAddress(serial,macList[0],strlen(macList[0]));
	if (!res)
	{
		file.SaveDebugFile("the license is not correct\n");
		str = "\n\n\t需要注册电脑";
		GetDlgItem(IDC_STATIC_SHOWSTATE)->SetWindowText(str);
		GetDlgItem(IDC_BUTTON_CONNECT)->EnableWindow(FALSE);
		GetDlgItem(IDC_BUTTON_CLOSE)->EnableWindow(FALSE);
		return false;
	}

	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
}

void CICT_ServerDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	if(nID==SC_MINIMIZE)
	{
		ToTray();	
	}
	else
	{
		CDialogEx::OnSysCommand(nID, lParam);
	}
}

// 如果向对话框添加最小化按钮，则需要下面的代码
//  来绘制该图标。对于使用文档/视图模型的 MFC 应用程序，
//  这将由框架自动完成。

void CICT_ServerDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // 用于绘制的设备上下文

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// 使图标在工作区矩形中居中
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// 绘制图标
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
		DrawBK(IDB_BITMAP_BK,IDC_STATIC_TITLE);
		DrawBK(IDB_BITMAP_BAR,IDC_STATIC_BAR);
		DrawBK(IDB_BITMAP_BOJAY,IDC_STATIC_LOG);
		if (GlobalState == 0)
		{
			DrawBK(IDB_BITMAP_READY,IDC_STATIC_RESULT);
		}
		else if(GlobalState == 1)
		{
			DrawBK(IDB_BITMAP_NG,IDC_STATIC_RESULT);
		}
		else if(GlobalState == 2)
		{
			DrawBK(IDB_BITMAP_OK,IDC_STATIC_RESULT);
		}
	}
}

//当用户拖动最小化窗口时系统调用此函数取得光标
//显示。
HCURSOR CICT_ServerDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}



void CICT_ServerDlg::DrawBK(UINT bmpID,UINT id)
{
	CPaintDC   dc(GetDlgItem(id));   
	CRect   rect;   
	GetDlgItem(id)->GetClientRect(&rect);   
	CDC   dcMem;   
	dcMem.CreateCompatibleDC(&dc);   
	CBitmap   bmpBackground;   
	bmpBackground.LoadBitmap(bmpID);   
	BITMAP   bitmap;       
	bmpBackground.GetBitmap(&bitmap);   
	CBitmap   *pbmpOld=dcMem.SelectObject(&bmpBackground);   
	dc.SetStretchBltMode(HALFTONE); 
	dc.StretchBlt(0,0,rect.Width(),rect.Height(),&dcMem,0,0,bitmap.bmWidth,bitmap.bmHeight,SRCCOPY);
	if (IDC_STATIC_TITLE == id || IDC_STATIC_BAR == id)
	{
		m_staticTitle.SetTransparent(TRUE);
	}
}


void CICT_ServerDlg::OnBnClickedButtonConnect()
{
	// TODO: Add your control notification handler code here
	SetTimer(100,1000,NULL);
	SetTimer(101,1000,NULL);
	isClose = false;
	bool res = false;
	res = InitSocket();
	OldGlobalState = GlobalState = 0;
	if (res)
	{
		GetDlgItem(IDC_BUTTON_CONNECT)->EnableWindow(FALSE);
		GetDlgItem(IDC_BUTTON_CLOSE)->EnableWindow(TRUE);
	}
	else
	{
		GlobalState = 1;
		Invalidate(TRUE);
	}
}


bool CICT_ServerDlg::InitSocket(void)
{
	//加载套接字库
	WORD wVersionRequested;
	WSAData wsaData;
	int err;
	wVersionRequested = MAKEWORD(1,1);
	err = WSAStartup(wVersionRequested,&wsaData);
	if (err != 0)
	{
		file.SaveDebugFile("initial socket error");
		return false;
	}

	if (LOBYTE(wsaData.wVersion) != 1 || HIBYTE(wsaData.wVersion) != 1) 
	{
		WSACleanup();
		file.SaveDebugFile("the version is not correct");
		return false;
	}

	//创建用于监听的套接字
	SOCKET m_socket = socket(AF_INET, SOCK_STREAM, 0);//SOCK_STREAM:TCP    SOCK_DGRAM:UDP
	if(m_socket == INVALID_SOCKET)
	{
		file.SaveDebugFile("invalid socket!");
		WSACleanup();
		return false;
	}

	//保证关闭SOCKET是完全关闭
	BOOL bOptVal = TRUE;
	int bOptLen = sizeof(BOOL);
	setsockopt(m_socket, SOL_SOCKET, SO_REUSEADDR, (char *)&bOptVal, bOptLen);


	//绑定IP和端口
	SOCKADDR_IN addrSock;  
	int retval = 0;  
	addrSock.sin_family=AF_INET; 
	if (port == 0)
	{
		port = 8888;
	}
	addrSock.sin_port=htons(port);   
	addrSock.sin_addr.S_un.S_addr=htonl(INADDR_ANY); 
	retval=bind(m_socket,(SOCKADDR*)&addrSock,sizeof(SOCKADDR));   
	if(SOCKET_ERROR==retval)   
	{   
		file.SaveDebugFile("Bind error");
		closesocket(m_socket); 
		return FALSE;   
	}

	//创建一线程监听 
	RECVPARAM *pRecvParam=new RECVPARAM;
	pRecvParam->sock=m_socket;   
	pRecvParam->hwnd=m_hWnd; 
	hThread=CreateThread(NULL,0,WorkProc,(LPVOID)pRecvParam,0,NULL);
	CloseHandle(hThread); 
	return true;
}


DWORD WINAPI CICT_ServerDlg::WorkProc(LPVOID lpParameter)
{
	SOCKET sock=((RECVPARAM*)lpParameter)->sock;   
	HWND hwnd=((RECVPARAM*)lpParameter)->hwnd;   
	delete lpParameter; //释放内存的操作
	ServerGlobal = sock;
	if(listen(sock,10) == SOCKET_ERROR)   
	{   
		GlobalState = 1;
		closesocket(sock);
		return 0;   
	} 
	SOCKADDR_IN addrFrom;
	int len=sizeof(SOCKADDR);   
	int retval = 0;
	int count = 0;
	char ResultFileChar[64] = {0};
	char recvBuf[256]={0};//获取客户端发送的消息
	while(TRUE)   
	{   
		if (isClose)
		{
			return false;
		}
		SOCKET ConnectSocket = accept(sock,(sockaddr*)&addrFrom,&len);
		ClientGlobal = ConnectSocket;
		if (ConnectSocket == SOCKET_ERROR)
		{
			GlobalState = 1;
			continue;
		}
		GlobalState = 2;
		while(TRUE)
		{
			if (isClose)
			{
				return false;
			}
			if (PathFileExists(L"result.txt"))
			{
				Sleep(100);
				//1:获取result.txt的内容
				memset(ResultFileChar,0,sizeof(ResultFileChar));
				for(int i=0; i<5; i++)
				{
					FILE *fp = fopen("result.txt","rb");
					count = 0;
					if (fp)
					{
						while(!feof(fp))
						{
							unsigned char ch = fgetc(fp);
							if (ch >= 0 && ch <= 127)
							{
								ResultFileChar[count++] = ch;
							}
							
						}
						//ResultFileChar[count] = '\0';
						fclose(fp);
						break;
					}
					else
					{
						if (fp)
						{
							fclose(fp);
						}
						continue;
					}
				}
				str = "\n\t发送到机械手臂:\t";
				str += ResultFileChar;
				//strcpy(ResultFileChar,"tony");
				retval = send(ConnectSocket,ResultFileChar, strlen(ResultFileChar),0);
				if (retval == SOCKET_ERROR)
				{
					GlobalState = 1;
					bStartAgain = true;
					break;
				}

				while(TRUE)
				{
					if (isClose)
					{
						return false;
					}
					memset(recvBuf,0,sizeof(recvBuf));
					retval=recv(ConnectSocket,recvBuf,256,0); 
					if (retval == SOCKET_ERROR)
					{
						GlobalState = 1;
						bStartAgain = true;
						break;
					}
					if (strlen(recvBuf) > 0)
					{
						//记录回传的文档
						fstream statusOut("status.txt",ios::out);
						if (statusOut.is_open())
						{
							statusOut << recvBuf;
						}
						statusOut.close();
						str += "\n\t机械手臂回传:\t";
						str += recvBuf;
						break;
					}
				}
				DeleteFile(L"result.txt");
			}
			else
			{
				continue;
			}
		}
	} 
 	return 0;
}

//检查是否有result.txt文档存在
void CICT_ServerDlg::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: Add your message handler code here and/or call default
	if(nIDEvent == 100)
	{
		KillTimer(nIDEvent);
		GetDlgItem(IDC_STATIC_SHOWSTATE)->SetWindowText(str);
		SetTimer(100,1000,NULL);
	}
	if (nIDEvent == 101)
	{
		KillTimer(nIDEvent);
		if (OldGlobalState != GlobalState)
		{
			Invalidate(TRUE);
			OldGlobalState = GlobalState;
		}
		SetTimer(101,1000,NULL);
	}
	CDialogEx::OnTimer(nIDEvent);
}


void CICT_ServerDlg::OnBnClickedButtonClose()
{
	// TODO: Add your control notification handler code here
	isClose = true;
	if (!ServerGlobal)
	{
		closesocket(ServerGlobal);
	}
	if (!ClientGlobal)
	{
		closesocket(ClientGlobal);
	}
	WSACleanup();
	if (!hThread )
	{
		CloseHandle(hThread);
		hThread = NULL;
	}
	if (!GetDlgItem(IDC_BUTTON_CONNECT)->IsWindowEnabled())
	{
		GetDlgItem(IDC_BUTTON_CONNECT)->EnableWindow(TRUE);
	}
	if (GetDlgItem(IDC_BUTTON_CLOSE)->IsWindowEnabled())
	{
		GetDlgItem(IDC_BUTTON_CLOSE)->EnableWindow(FALSE);
	}
	GlobalState = OldGlobalState = 0;
	Invalidate(TRUE);
}


void CICT_ServerDlg::OnClose()
{
	// TODO: Add your message handler code here and/or call default
	isClose = true;
	if (!ServerGlobal)
	{
		closesocket(ServerGlobal);
	}
	if (!ClientGlobal)
	{
		closesocket(ClientGlobal);
	}
	WSACleanup();
	if (!hThread )
	{
		CloseHandle(hThread);
		hThread = NULL;
	}
	CDialogEx::OnClose();
}


int CICT_ServerDlg::GetSysMac(int &macNum,char macList[][18])
{
	char mac[18] = { 0 };
	//记录网卡数量
	int netCardNum = 0;

	PIP_ADAPTER_INFO pIpAdapterInfo  = (PIP_ADAPTER_INFO)malloc(sizeof(IP_ADAPTER_INFO));
	unsigned long stSize = sizeof(IP_ADAPTER_INFO);
	int nRel = GetAdaptersInfo(pIpAdapterInfo,&stSize);

	if (ERROR_BUFFER_OVERFLOW == nRel) {
		free(pIpAdapterInfo);
		//重新申请内存
		pIpAdapterInfo = (PIP_ADAPTER_INFO)malloc(stSize);
		nRel=GetAdaptersInfo(pIpAdapterInfo,&stSize); 
	}

	if (ERROR_SUCCESS == nRel) {
		while (pIpAdapterInfo) {
			char temp[4] = { 0 };
			for (DWORD i = 0; i < pIpAdapterInfo->AddressLength; i++) {
				if (i < pIpAdapterInfo->AddressLength-1)
				{
					sprintf(temp,"%02X:", pIpAdapterInfo->Address[i]);
					strcat(mac,temp);
					memset(temp,0,sizeof(temp));
				}
				else
				{
					sprintf(temp,"%02X", pIpAdapterInfo->Address[i]);
					strcat(mac,temp);
					memset(temp,0,sizeof(temp));
				}
			}
			memcpy(macList[netCardNum],mac,sizeof(mac));
			memset(mac,0,sizeof(mac));
			pIpAdapterInfo = pIpAdapterInfo->Next;
			netCardNum++;
		}
	}

	//释放内存空间
	if (pIpAdapterInfo)
	{
		free (pIpAdapterInfo);
	}

	macNum = netCardNum;

	return 0;
}


bool CICT_ServerDlg::CheckMACAddress(string serial,char *macList,int len)
{
	char password[64] = {0};
	int count = 0;
	for (int i=0; i<len; i++)
	{
		if ((macList[i] <= 'z' && macList[i] >='a') ||
			(macList[i] <= 'Z' && macList[i] >='A') ||
			(macList[i] <= '9' && macList[i] >='0'))
		{
			//是数字的处理方法
			if (macList[i] <= '9' && macList[i] >= '0')
			{
				password[count++] = macList[i] + 19;
			}
			else
			{
				if (macList[i] != 'z' || macList[i] != 'Z')
				{
					password[count++] = macList[i] +1;	
				}
				else
				{
					password[count++] = macList[i]-3;
				}
			}			
		}
	}
	password[count]= '\0';
	if (!strcmp(password,serial.c_str()))
	{
		return true;
	}
	return false;
}


void CICT_ServerDlg::ToTray(void)
{
	NOTIFYICONDATA nid;
	nid.cbSize=(DWORD)sizeof(NOTIFYICONDATA);
	nid.hWnd=this->m_hWnd; nid.uID=IDR_MAINFRAME;
	nid.uFlags=NIF_ICON|NIF_MESSAGE|NIF_TIP ;
	nid.uCallbackMessage=WM_SHOWTASK;//自定义的消息名称
	nid.hIcon=LoadIcon(AfxGetInstanceHandle(),MAKEINTRESOURCE(IDR_MAINFRAME));
	wcscpy(nid.szTip,L"博杰Server"); //信息提示条
	Shell_NotifyIcon(NIM_ADD,&nid); //在托盘区添加图标
	ShowWindow(SW_HIDE); //隐藏主窗口

}

void CICT_ServerDlg::DeleteTray(void)
{
	NOTIFYICONDATA nid;
	nid.cbSize=(DWORD)sizeof(NOTIFYICONDATA);
	nid.hWnd=this->m_hWnd;
	nid.uID=IDR_MAINFRAME;
	nid.uFlags=NIF_ICON|NIF_MESSAGE|NIF_TIP ;
	nid.uCallbackMessage=WM_SHOWTASK;//自定义的消息名称
	nid.hIcon=LoadIcon(AfxGetInstanceHandle(),MAKEINTRESOURCE(IDR_MAINFRAME));
	wcscpy(nid.szTip,L"博杰Server"); //信息提示条为“计划任务提醒”
	Shell_NotifyIcon(NIM_DELETE,&nid); //在托盘区删除图标


}

LRESULT CICT_ServerDlg::OnShowTask(WPARAM wParam,LPARAM lParam)
{
	if(wParam!=IDR_MAINFRAME)
		return 1;
	switch(lParam)
	{
	case WM_RBUTTONUP://右键起来时弹出快捷菜单，这里只有一个“关闭”
		{ 
			LPPOINT lpoint=new tagPOINT;
			::GetCursorPos(lpoint);//得到鼠标位置
			CMenu menu;
			menu.CreatePopupMenu();//声明一个弹出式菜单
			//增加菜单项“关闭”，点击则发送消息WM_DESTROY给主窗口将程序结束。
			menu.AppendMenu(MF_STRING,WM_DESTROY,L"关闭"); //确定弹出式菜单的位置
			menu.TrackPopupMenu(TPM_LEFTALIGN,lpoint->x,lpoint->y,this); //资源回收
			HMENU hmenu=menu.Detach();
			menu.DestroyMenu();
			delete lpoint;
		} 
		break;
	case WM_LBUTTONDBLCLK://双击左键的处理
		{ 
			this->ShowWindow(SW_SHOW);//显示主窗口
			DeleteTray();
		} 
		break;
	default: break;
	}
	return 0;
}


