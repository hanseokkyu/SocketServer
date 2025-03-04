// ListenSocket.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "SocketServer.h"
#include "ListenSocket.h"
#include "ClientSocket.h"
#include "SocketServerDlg.h"


// CListenSocket

CListenSocket::CListenSocket()
{
}

CListenSocket::~CListenSocket()
{
}

void CListenSocket::OnAccept(int nErrorCode)
{
	CClientSocket* pClient = new CClientSocket;
	CString str;
	
	if(Accept(*pClient)) { // Å¬¶óÀÌ¾ðÆ® Á¢¼Ó ¿äÃ»ÀÌ ¿À¸é ¼­¹ö-Å¬¶óÀÌ¾ðÆ®¸¦ ¿¬°á½ÃÄÑÁØ´Ù
		pClient->SetListenSocket(this);
		m_ptrClientSocketList.AddTail(pClient);

		CSocketServerDlg* pMain = (CSocketServerDlg*)AfxGetMainWnd(); // CSocketServerDlgÀÇ ÇÚµéÀ» °¡Á®¿È
		str.Format(_T("Client (%d)"), (int)m_ptrClientSocketList.Find(pClient)); // Å¬¶óÀÌ¾ðÆ® ¹øÈ£(POSITION °ª)
		pMain->clientList->AddString(str); // Å¬¶óÀÌ¾ðÆ®°¡ Á¢¼ÓÇÒ¶§¸¶´Ù ¸®½ºÆ®¿¡ Å¬¶óÀÌ¾ðÆ® ÀÌ¸§ Ãß°¡
		
	} else {
		delete pClient;
		AfxMessageBox(_T("ERROR : Failed can't accept new Client!"));
	}

	CAsyncSocket::OnAccept(nErrorCode);
}

void CListenSocket::CloseClientSocket(CSocket* pClient)
{
	POSITION pos;
	pos = m_ptrClientSocketList.Find(pClient);
	
	if(pos != NULL) {
		if(pClient != NULL) {
			// Å¬¶óÀÌ¾ðÆ® ¿¬°áÁßÁöÈÄ Á¾·á
			pClient->ShutDown();
			pClient->Close();
		}

		CSocketServerDlg* pMain = (CSocketServerDlg*)AfxGetMainWnd();
		CString str1, str2;		
		UINT indx = 0, posNum;	
		pMain->clientList->SetCurSel(0);
		// Á¢¼Ó Á¾·áµÇ´Â Å¬¶óÀÌ¾ðÆ® Ã£±â
		while(indx < pMain->clientList->GetCount()) {
			posNum = (int)m_ptrClientSocketList.Find(pClient); 
			pMain->clientList->GetText(indx, str1);
			str2.Format(_T("%d"), posNum);
			// ¼ÒÄÏ¸®½ºÆ®, Å¬¶óÀÌ¾ðÆ®¸®½ºÆ®¸¦ ºñ±³ÇØ¼­ °°Àº Å¬¶óÀÌ¾ðÆ® ¹øÈ£(POSITION °ª) ÀÖÀ¸¸é ¸®½ºÆ®¿¡¼­ »èÁ¦
			if(str1.Find(str2) != -1) {
				AfxMessageBox(str1 + str2);
				pMain->clientList->DeleteString(indx);
				break;
			}
			indx ++;
		}

		m_ptrClientSocketList.RemoveAt(pos);
		delete pClient;
	}
}

void CListenSocket::SendAllMessage(TCHAR* pszMessage)
{
	POSITION pos;
	pos = m_ptrClientSocketList.GetHeadPosition();
	CClientSocket* pClient = NULL;

	while(pos != NULL) {
		pClient = (CClientSocket*)m_ptrClientSocketList.GetNext(pos);
		if(pClient != NULL) {
			// SendÇÔ¼öÀÇ µÎ¹øÂ° ÀÎÀÚ´Â ¸Þ¸ð¸®ÀÇ Å©±âÀÎµ¥ À¯´ÏÄÚµå¸¦ »ç¿ëÇÏ°í ÀÖÀ¸¹Ç·Î *2¸¦ ÇÑ Å©±â°¡ µÈ´Ù.
			// ÀÌ ÇÔ¼ö´Â Àü¼ÛÇÑ µ¥ÀÌÅÍÀÇ ±æÀÌ¸¦ ¹ÝÈ¯ÇÑ´Ù.
			int checkLenOfData = pClient->Send(pszMessage, lstrlen(pszMessage) * 2);
			if(checkLenOfData != lstrlen(pszMessage) * 2) {
				AfxMessageBox(_T("ÀÏºÎ µ¥ÀÌÅÍ°¡ Á¤»óÀûÀ» Àü¼ÛµÇÁö ¸øÇß½À´Ï´Ù!"));
			}
		}
	}
}