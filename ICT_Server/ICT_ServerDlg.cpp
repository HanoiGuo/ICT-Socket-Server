
// ICT_ServerDlg.cpp : ʵ���ļ�
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

#pragma comment(lib,"Shlwapi.lib") //���û�����У������link����
#pragma  comment(lib,"ws2_32.lib")
#pragma comment(lib,"Iphlpapi.lib") //mac��ַ

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
// ����Ӧ�ó��򡰹��ڡ��˵���� CAboutDlg �Ի���

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// �Ի�������
	enum { IDD = IDD_ABOUTBOX };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

// ʵ��
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


// CICT_ServerDlg �Ի���
CICT_ServerDlg::CICT_ServerDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CICT_ServerDlg::IDD, pParent)
{
	//m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
	m_hIcon = AfxGetApp()->LoadIcon(IDI_ICON_LOG);
	//���캯������ʼֵ
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


// CICT_ServerDlg ��Ϣ�������

BOOL CICT_ServerDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// ��������...���˵�����ӵ�ϵͳ�˵��С�

	// IDM_ABOUTBOX ������ϵͳ���Χ�ڡ�
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

	// ���ô˶Ի����ͼ�ꡣ��Ӧ�ó��������ڲ��ǶԻ���ʱ����ܽ��Զ�
	//  ִ�д˲���
	SetIcon(m_hIcon, TRUE);			// ���ô�ͼ��
	SetIcon(m_hIcon, FALSE);		// ����Сͼ��

	// TODO: �ڴ���Ӷ���ĳ�ʼ������
	m_staticTitle.SetTransparent(TRUE);
	m_staticTitle.SetBkClr(RGB(159,227,251));
	m_staticTitle.SetTextClr(RGB(255,0,0));
	m_staticTitle.SetWindowText(_T("Bojay ICT Server"));
	m_staticTitle.SetTextFont(40, 1, 0, _T("����") );//��������
	GetDlgItem(IDC_BUTTON_CLOSE)->EnableWindow(FALSE);

	bool res = false;
	res = file.ReadConfig("config.txt",ipAddress,port);
	if (!res)
	{
		str = "\n\n\tû�������ļ�,Ĭ�϶˿�Ϊ8888";
		GetDlgItem(IDC_STATIC_SHOWSTATE)->SetWindowText(str);
	}
	string serial;
	file.ReadSerialFile("Serial.dat",serial);
	int macNum = 0;
	char macList[10][18] = {0};
	GetSysMac(macNum,macList);
	//ֻ����һ���MAC��ַ
	res = CheckMACAddress(serial,macList[0],strlen(macList[0]));
	if (!res)
	{
		file.SaveDebugFile("the license is not correct\n");
		str = "\n\n\t��Ҫע�����";
		GetDlgItem(IDC_STATIC_SHOWSTATE)->SetWindowText(str);
		GetDlgItem(IDC_BUTTON_CONNECT)->EnableWindow(FALSE);
		GetDlgItem(IDC_BUTTON_CLOSE)->EnableWindow(FALSE);
		return false;
	}

	return TRUE;  // ���ǽ��������õ��ؼ������򷵻� TRUE
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

// �����Ի��������С����ť������Ҫ����Ĵ���
//  �����Ƹ�ͼ�ꡣ����ʹ���ĵ�/��ͼģ�͵� MFC Ӧ�ó���
//  �⽫�ɿ���Զ���ɡ�

void CICT_ServerDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // ���ڻ��Ƶ��豸������

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// ʹͼ���ڹ����������о���
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// ����ͼ��
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

//���û��϶���С������ʱϵͳ���ô˺���ȡ�ù��
//��ʾ��
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
	//�����׽��ֿ�
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

	//�������ڼ������׽���
	SOCKET m_socket = socket(AF_INET, SOCK_STREAM, 0);//SOCK_STREAM:TCP    SOCK_DGRAM:UDP
	if(m_socket == INVALID_SOCKET)
	{
		file.SaveDebugFile("invalid socket!");
		WSACleanup();
		return false;
	}

	//��֤�ر�SOCKET����ȫ�ر�
	BOOL bOptVal = TRUE;
	int bOptLen = sizeof(BOOL);
	setsockopt(m_socket, SOL_SOCKET, SO_REUSEADDR, (char *)&bOptVal, bOptLen);


	//��IP�Ͷ˿�
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

	//����һ�̼߳��� 
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
	delete lpParameter; //�ͷ��ڴ�Ĳ���
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
	char recvBuf[256]={0};//��ȡ�ͻ��˷��͵���Ϣ
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
				//1:��ȡresult.txt������
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
				str = "\n\t���͵���е�ֱ�:\t";
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
						//��¼�ش����ĵ�
						fstream statusOut("status.txt",ios::out);
						if (statusOut.is_open())
						{
							statusOut << recvBuf;
						}
						statusOut.close();
						str += "\n\t��е�ֱۻش�:\t";
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

//����Ƿ���result.txt�ĵ�����
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
	//��¼��������
	int netCardNum = 0;

	PIP_ADAPTER_INFO pIpAdapterInfo  = (PIP_ADAPTER_INFO)malloc(sizeof(IP_ADAPTER_INFO));
	unsigned long stSize = sizeof(IP_ADAPTER_INFO);
	int nRel = GetAdaptersInfo(pIpAdapterInfo,&stSize);

	if (ERROR_BUFFER_OVERFLOW == nRel) {
		free(pIpAdapterInfo);
		//���������ڴ�
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

	//�ͷ��ڴ�ռ�
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
			//�����ֵĴ�����
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
	nid.uCallbackMessage=WM_SHOWTASK;//�Զ������Ϣ����
	nid.hIcon=LoadIcon(AfxGetInstanceHandle(),MAKEINTRESOURCE(IDR_MAINFRAME));
	wcscpy(nid.szTip,L"����Server"); //��Ϣ��ʾ��
	Shell_NotifyIcon(NIM_ADD,&nid); //�����������ͼ��
	ShowWindow(SW_HIDE); //����������

}

void CICT_ServerDlg::DeleteTray(void)
{
	NOTIFYICONDATA nid;
	nid.cbSize=(DWORD)sizeof(NOTIFYICONDATA);
	nid.hWnd=this->m_hWnd;
	nid.uID=IDR_MAINFRAME;
	nid.uFlags=NIF_ICON|NIF_MESSAGE|NIF_TIP ;
	nid.uCallbackMessage=WM_SHOWTASK;//�Զ������Ϣ����
	nid.hIcon=LoadIcon(AfxGetInstanceHandle(),MAKEINTRESOURCE(IDR_MAINFRAME));
	wcscpy(nid.szTip,L"����Server"); //��Ϣ��ʾ��Ϊ���ƻ��������ѡ�
	Shell_NotifyIcon(NIM_DELETE,&nid); //��������ɾ��ͼ��


}

LRESULT CICT_ServerDlg::OnShowTask(WPARAM wParam,LPARAM lParam)
{
	if(wParam!=IDR_MAINFRAME)
		return 1;
	switch(lParam)
	{
	case WM_RBUTTONUP://�Ҽ�����ʱ������ݲ˵�������ֻ��һ�����رա�
		{ 
			LPPOINT lpoint=new tagPOINT;
			::GetCursorPos(lpoint);//�õ����λ��
			CMenu menu;
			menu.CreatePopupMenu();//����һ������ʽ�˵�
			//���Ӳ˵���رա������������ϢWM_DESTROY�������ڽ����������
			menu.AppendMenu(MF_STRING,WM_DESTROY,L"�ر�"); //ȷ������ʽ�˵���λ��
			menu.TrackPopupMenu(TPM_LEFTALIGN,lpoint->x,lpoint->y,this); //��Դ����
			HMENU hmenu=menu.Detach();
			menu.DestroyMenu();
			delete lpoint;
		} 
		break;
	case WM_LBUTTONDBLCLK://˫������Ĵ���
		{ 
			this->ShowWindow(SW_SHOW);//��ʾ������
			DeleteTray();
		} 
		break;
	default: break;
	}
	return 0;
}


