#include "ThreadPool.h"
#include <iostream>
#include <csignal>
#include <fstream>
#include <string>
#include "string.h"


HANDLE * threadArray;
int countThread,countTask, complexity, currThread;
CHAR resultStr[1000];



Thread*  thread = 0;
Task* task = 0;

void WriteMessage(char str[255], int threadID)
{
	ofstream file("myLogfile.log", ios::app);
	file<<"ID: "<< threadID<< ":" << str<< "\n\n";
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

ThreadPool::ThreadPool(int countThreads, Thread* allThreads, int countTasks, Task* allTasks)
{
	countThread = countThreads;
	countTask = countTasks;
	GetThreads(countThread, allThreads);
	GetTasks(countTask, allTasks);
	ofstream ofs;
	ofs.open("myLogfile.log", ios::trunc);
	ofs.close();
	CreateThread(NULL, 0, ThreadPool::ManagementThread, this, 0, NULL);
}

static DWORD WINAPI Proc(LPVOID lpParam)
{
	int complexity = (int)lpParam + 1;
	int sleep = 30000/complexity;
	Sleep(sleep);
	strcat_s(resultStr,1000,"Поток ");
	strcat_s(resultStr,1000, thread[currThread].name);
	strcat_s(resultStr,1000," выполнил функцию ");
	strcat_s(resultStr,1000,task[currThread].name);
	return 1;
}

void SignalHandler(int signal)
{
    if (signal == SIGINT)
	{
		threadArray = (HANDLE *)calloc(countThread, sizeof(HANDLE));
		for(int currentThread = 0; currentThread <= countThread - 1; currentThread++)
		{
			if (countTask>=currentThread)
			{
				complexity = task[currentThread].complexity;
				currThread = currentThread;
				threadArray[currentThread] = CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)Proc, (LPVOID)complexity, 0, NULL);
				thread[currentThread].id = GetCurrentThreadId();
				WriteMessage("Thread created!", thread[currentThread].id);
			}
		}
    } 
}

DWORD WINAPI ThreadPool::ManagementThread(PVOID thisContext)
{
	signal(SIGINT,SignalHandler);
	return 1;
}


ThreadPool::~ThreadPool(void)
{
}
