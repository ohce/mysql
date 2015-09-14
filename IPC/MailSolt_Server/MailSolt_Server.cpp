// MailSolt_Server.cpp : 定义控制台应用程序的入口点。
//

#include "stdafx.h"

/*头文件*/
#include <windows.h>
#include <stdio.h>
/*全局变量*/
HANDLE hSlot;
LPTSTR lpszSlotName = TEXT("\\\\.\\mailslot\\sample_mailslot");
LPTSTR Message = TEXT("Message for mailslot in  primary domain");
/*************************************************
* int main()
* 功能:进程间mailslot通信客户端
**************************************************/
int _tmain(int argc, _TCHAR* argv[])
{
	DWORD dwMessageSize = 0 ;			//消息的大小
	DWORD dwMessageCount = 0;			//消息的数量
	DWORD dwMessageRead = 0;			//实际读取的长度
	BOOL bResult = false;				//函数调用结果
	LPTSTR lpszBuffer = NULL;			//接受缓冲区指针
	TCHAR achID[80];
	::memset(achID, 0, sizeof(achID));
	DWORD  dwAllMessageCount = 0;
	HANDLE hEvent;
	OVERLAPPED ov;
	
	//创建mailslot
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
		//获取mailslot信息
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
			//提示信息
			wsprintf((LPTSTR)achID, TEXT("\nMessage #%d of %d\n"), dwAllMessageCount - dwMessageCount + 1, dwAllMessageCount);
		

			//分配空间
			lpszBuffer = (LPTSTR)HeapAlloc(GetProcessHeap(), HEAP_ZERO_MEMORY, lstrlen((LPTSTR)achID) * sizeof(TCHAR) + dwMessageSize);

			if (NULL == lpszBuffer)
			{
				return 0;
			}

			//读消息
			bResult = ReadFile(hSlot, lpszBuffer, dwMessageSize, &dwMessageRead, NULL);
			if (!bResult)
			{
				printf("ReadFile Failed with %d. \n", GetLastError());
				GlobalFree((HGLOBAL)lpszBuffer);
				return 0;
			}
			//处理信息 显示
			lstrcat(lpszBuffer, (LPTSTR)achID);
			wprintf(L"Contents of the mailslot: %s\n", lpszBuffer);
			HeapFree(GetProcessHeap(), 0, lpszBuffer);
			//计算剩余的消息数
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

