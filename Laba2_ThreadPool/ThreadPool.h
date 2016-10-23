#pragma once


#include "windows.h"




struct Thread
{
	char name[100];
	int priority;
	int id;
};

struct Task
{
	char name[20];
	int priority;
	int complexity;
};

typedef DWORD (WINAPI *FUNCTYPE)(LPVOID lpParam);
using namespace std;

class ThreadPool
{
public:
	ThreadPool(int countThreads, Thread* thread, int countTasks, Task* allTasks);
	~ThreadPool();
	static DWORD WINAPI ManagementThread(PVOID thisContext);
};

