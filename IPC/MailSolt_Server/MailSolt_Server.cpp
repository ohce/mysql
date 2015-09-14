// MailSolt_Server.cpp : �������̨Ӧ�ó������ڵ㡣
//

#include "stdafx.h"

/*ͷ�ļ�*/
#include <windows.h>
#include <stdio.h>
/*ȫ�ֱ���*/
HANDLE hSlot;
LPTSTR lpszSlotName = TEXT("\\\\.\\mailslot\\sample_mailslot");
LPTSTR Message = TEXT("Message for mailslot in  primary domain");
/*************************************************
* int main()
* ����:���̼�mailslotͨ�ſͻ���
**************************************************/
int _tmain(int argc, _TCHAR* argv[])
{
	DWORD dwMessageSize = 0 ;			//��Ϣ�Ĵ�С
	DWORD dwMessageCount = 0;			//��Ϣ������
	DWORD dwMessageRead = 0;			//ʵ�ʶ�ȡ�ĳ���
	BOOL bResult = false;				//�������ý��
	LPTSTR lpszBuffer = NULL;			//���ܻ�����ָ��
	TCHAR achID[80];
	::memset(achID, 0, sizeof(achID));
	DWORD  dwAllMessageCount = 0;
	HANDLE hEvent;
	OVERLAPPED ov;
	
	//����mailslot
	hSlot = CreateMailslot(lpszSlotName, 0, MAILSLOT_WAIT_FOREVER, (LPSECURITY_ATTRIBUTES)NULL);
	if (INVALID_HANDLE_VALUE == hSlot)
	{
		printf("CreatemailSlot failed with %d \n", GetLastError());
		return 0;
	}
	else
	{
		printf("MailSlot created successfully . \n");
	}

	while (TRUE)
	{
		//��ȡmailslot��Ϣ
		bResult = GetMailslotInfo(hSlot, (LPDWORD)NULL, &dwMessageSize, &dwMessageCount, (LPDWORD)NULL);
		if (!bResult)
		{
			printf("GetmailslotInfo failed with %d. \n", GetLastError());
			return 0;
		}
		if (MAILSLOT_NO_MESSAGE == dwMessageSize)
		{
			Sleep(2000);
			continue;
		}
		dwAllMessageCount = dwMessageCount;
		while (dwMessageCount != 0)
		{
			//��ʾ��Ϣ
			wsprintf((LPTSTR)achID, TEXT("\nMessage #%d of %d\n"), dwAllMessageCount - dwMessageCount + 1, dwAllMessageCount);
		

			//����ռ�
			lpszBuffer = (LPTSTR)HeapAlloc(GetProcessHeap(), HEAP_ZERO_MEMORY, lstrlen((LPTSTR)achID) * sizeof(TCHAR) + dwMessageSize);

			if (NULL == lpszBuffer)
			{
				return 0;
			}

			//����Ϣ
			bResult = ReadFile(hSlot, lpszBuffer, dwMessageSize, &dwMessageRead, NULL);
			if (!bResult)
			{
				printf("ReadFile Failed with %d. \n", GetLastError());
				GlobalFree((HGLOBAL)lpszBuffer);
				return 0;
			}
			//������Ϣ ��ʾ
			lstrcat(lpszBuffer, (LPTSTR)achID);
			wprintf(L"Contents of the mailslot: %s\n", lpszBuffer);
			HeapFree(GetProcessHeap(), 0, lpszBuffer);
			//����ʣ�����Ϣ��
			bResult = GetMailslotInfo(hSlot, (LPDWORD)NULL, &dwMessageSize, &dwMessageCount, (LPDWORD)NULL);
			if (!bResult)
			{
				printf("GetMailSlotInfo failed %d\n", GetLastError());
				return 0;
			}
		}
	}
	return 0;
}

