
// ICT_ServerDlg.h : ͷ�ļ�
//

#pragma once

#include "MyStatic.h"
#include "OperateFile.h"
// CICT_ServerDlg �Ի���
class CICT_ServerDlg : public CDialogEx
{
// ����
public:
	CICT_ServerDlg(CWnd* pParent = NULL);	// ��׼���캯��

// �Ի�������
	enum { IDD = IDD_ICT_SERVER_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV ֧��


// ʵ��
protected:
	HICON m_hIcon;

	// ���ɵ���Ϣӳ�亯��
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
private:
	OperateFile file;
	int port;
	char ipAddress[64];
	typedef struct _RECVPARAM   
	{   
		SOCKET sock;   
		HWND hwnd;   
	}RECVPARAM;
public:
	MyStatic m_staticTitle;
	void DrawBK(UINT bmpID,UINT id);
	CStatic m_staticBar;
	afx_msg void OnBnClickedButtonConnect();
	afx_msg LRESULT OnShowTask(WPARAM wParam,LPARAM lParam);
	bool InitSocket(void);
	static DWORD WINAPI WorkProc(LPVOID lpParameter);
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	CStatic m_result;
	afx_msg void OnBnClickedButtonClose();
	CStatic m_StaticLog;
	CStatic m_state;
	afx_msg void OnClose();
	int GetSysMac(int &macNum,char macList[][18]);
	bool CheckMACAddress(string serial,char *macList,int len);
	void ToTray(void);
	void DeleteTray(void);
};
