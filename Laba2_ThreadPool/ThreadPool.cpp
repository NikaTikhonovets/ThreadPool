#include "ThreadPool.h"
#include <iostream>
#include <csignal>
#include <fstream>
#include <string>
#include "string.h"
#include "resource1.h"


HANDLE * threadArray;
HANDLE managerThread;
int countThread,countTask;
CHAR resultStr[1000];

HWND hResultStr;


Thread*  thread = 0;
Task* task = 0;

void WriteMessage(char str[255], char nameTask[100], char nameThread[100])
{
	ofstream file("myLogfile.log", ios::app);
	file<<"Поток "<< nameThread<<" " << str << nameTask << "\n\n";
	file.close();
}

void GetThreads(int countThread, Thread* allThreads)
{
    thread = new Thread[countThread];
 	for (int i = 0; i < countThread; i++)
	{
		thread[i] = allThreads[i]; 
	}
}

void GetTasks(int countTask, Task* allTasks)
{
    task = new Task[countTask];
 	for (int i = 0; i < countTask; i++)
	{
		task[i] = allTasks[i]; 
	}
}

void ThreadPool::GetAllTasks(Task* allTasks, int countTasks)
{
	countTask = countTasks;
	GetTasks(countTask,allTasks);
}
ThreadPool::ThreadPool(int countThreads, Thread* allThreads, HWND hwnd)
{
	hResultStr = GetDlgItem(hwnd, IDC_RESULT);
	countThread = countThreads;
	GetThreads(countThread, allThreads);
	ofstream ofs;
	ofs.open("myLogfile.log", ios::trunc);
	ofs.close();
	managerThread = CreateThread(NULL, 0, ThreadPool::ManagementThread, this, 0, NULL);
}

static void GetStrExecute(int currentThread)
{
	strcat_s(resultStr,1000,"Поток ");
	strcat_s(resultStr,1000, thread[currentThread].name);
	strcat_s(resultStr,1000," выполнил функцию ");
	strcat_s(resultStr,1000,task[currentThread].name);
	strcat_s(resultStr,1000,"!\n");
	WriteMessage("выполнил функцию ", task[currentThread].name,thread[currentThread].name);
	SetWindowText(hResultStr, resultStr);
}

static void GetStrStart(int currentTask)
{
	strcat_s(resultStr,1000,"+ Поток ");
	strcat_s(resultStr,1000, thread[currentTask].name);
	strcat_s(resultStr,1000," начал выполнять функцию ");
	strcat_s(resultStr,1000,task[currentTask].name);
	strcat_s(resultStr,1000,".\n\n");
	WriteMessage("начал выполнять функцию ", task[currentTask].name,thread[currentTask].name);
	SetWindowText(hResultStr, resultStr);
}

void CheckThread(int currentThread)
{
	if((currentThread == countThread-1) && (countTask > countThread))
	{
		strcat_s(resultStr,1000,"Превышение максимально допустимого количества работающих потоков!\n\n");
		SetWindowText(hResultStr, resultStr);
		WriteMessage("Превышение максимально допустимого количества работающих потоков!", "","----");
	}
}

static DWORD WINAPI Proc(LPVOID lpParam)
{
	if (countTask != 0)
	{
		int currentThread = (int)lpParam;
		if (countTask >= currentThread+1)
		{
			GetStrStart(currentThread);
			CheckThread(currentThread);
			int complexity = task[currentThread].complexity + 1;
			int sleep = 10000/complexity;
			Sleep(sleep);
			GetStrExecute(currentThread);
		}
	}
	return 1;
}

DWORD GetPriority(int currentThread)
{
	switch (thread[currentThread].priority)
	{
	case 0:
		return ABOVE_NORMAL_PRIORITY_CLASS;
	case 1:
		return NORMAL_PRIORITY_CLASS;
	case 2:
		return BELOW_NORMAL_PRIORITY_CLASS;
	default:
		return 0;
	}
}
void SetPriority(HANDLE thread, int currentThread)
{
	SetPriorityClass(thread, GetPriority(currentThread));
}
void SignalHandler(int signal)
{
    if (signal == SIGINT)
	{
		threadArray = (HANDLE *)calloc(countThread, sizeof(HANDLE));
		for(int currentThread = 0; currentThread <= countThread - 1; currentThread++)
		{
			threadArray[currentThread] = CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)Proc, (LPVOID)currentThread, 0, NULL);
			SetPriority(threadArray[currentThread], currentThread);
			long id = GetCurrentThreadId();
			WriteMessage("был создан", "", thread[currentThread].name);
			Sleep(100);
		}
    }
	if (signal == SIGABRT)
	{
		for (int currentThread = 0; currentThread < countThread; currentThread++)
		{
			TerminateThread(threadArray[currentThread], NULL);
			CloseHandle(threadArray[currentThread]);
		}
	}	
}

DWORD WINAPI ThreadPool::ManagementThread(PVOID thisContext)
{
	signal(SIGINT,SignalHandler);
	raise(SIGINT);
	return 1;
}


ThreadPool::~ThreadPool(void)
{
	signal(SIGABRT,SignalHandler);
	raise(SIGABRT);
	TerminateThread(managerThread, NULL);
	CloseHandle(managerThread);
}
