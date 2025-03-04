// ClientSocket.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "SocketServer.h"
#include "ClientSocket.h"
#include "ListenSocket.h"
#include "SocketServerDlg.h"


// CClientSocket

CClientSocket::CClientSocket()
{
}

CClientSocket::~CClientSocket()
{
}

void CClientSocket::SetListenSocket(CAsyncSocket* pSocket)
{
	m_pListenSocket = pSocket;
}



void CClientSocket::OnClose(int nErrorCode)
{
	CSocket::OnClose(nErrorCode);

	CListenSocket* pServerSocket = (CListenSocket*)m_pListenSocket;
	pServerSocket->CloseClientSocket(this);
}

void CClientSocket::OnReceive(int nErrorCode)
{
	CString strTmp = _T(""), strIPAddress = _T("");
	UINT uPortNumber = 0;
	TCHAR strBuffer[1024];
	::ZeroMemory(strBuffer, sizeof(strBuffer)); 
	GetPeerName(strIPAddress, uPortNumber);
	if(Receive(strBuffer, sizeof(strBuffer)) > 0) { 
		CSocketServerDlg* pMain = (CSocketServerDlg*)AfxGetMainWnd();
		strTmp.Format(_T("[%s:%d]: %s"), strIPAddress, uPortNumber, strBuffer);
		pMain->m_List.AddString(strTmp);  
		pMain->m_List.SetCurSel(pMain->m_List.GetCount() - 1);

		CListenSocket* pServerSocket = (CListenSocket*)m_pListenSocket;
		pServerSocket->SendAllMessage(strBuffer); 
	}

	CSocket::OnReceive(nErrorCode);
}