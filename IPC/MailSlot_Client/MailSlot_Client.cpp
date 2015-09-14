// MailSlot_Client.cpp : �������̨Ӧ�ó������ڵ㡣
//

#include "stdafx.h"
#include <windows.h>
#include <stdio.h>

HANDLE hSlot;
LPTSTR lpszSlotname = TEXT("\\\\.\\mailslot\\sample_mailslot");
LPTSTR lpszMessage = TEXT("Test Message for mailslot ");

int _tmain(int argc, _TCHAR* argv[])
{
	BOOL bResult = false;
	HANDLE hFile;
	DWORD dwWritten = 0;
	DWORD dwMessageSize = 0;

	//��mailslot
	hFile = CreateFile(lpszSlotname, GENERIC_WRITE, FILE_SHARE_READ, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
	if (INVALID_HANDLE_VALUE == hFile)
	{
		printf("CreateFile failed with %d.\n", GetLastError());
		return 0;
	}

	//��mailslotд��
	bResult = WriteFile(hFile, lpszMessage, (lstrlen(lpszMessage) + 1)* sizeof(TCHAR), &dwWritten, NULL);
	if (!bResult)
	{
		printf("WriteFile failed with %d.\n", GetLastError());
		return 0;
	}

	//����
	printf("Slot written to successfully.\n");
	CloseHandle(hFile);
	system("pause");
	return 0;
}

