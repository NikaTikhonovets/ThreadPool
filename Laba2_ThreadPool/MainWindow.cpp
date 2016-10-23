#include <Windows.h>
#include <string>
#include "string.h"
#include "afxres.h"
#include "resource1.h"
#include "View.h"
#include "ThreadPool.h"



BOOL CALLBACK DlgProc(HWND, UINT, WPARAM, LPARAM);
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


Thread* thread = 0;
Task* task = 0;
int countThread = 0, complexity = 0, priorityThread = 0, priorityTask = 0, countTask = 0;
CHAR priorityThreadLine[20] = "Высокий", complexityTask[20] = "Высокая", priorityTaskLine[20] = "Высокий";
HWND hPriorityThread, hPriorityTask, hComplexity;

Thread* AddThread(Thread* thread, const int countThread)
{
	if (countThread == 0)
	{		
		thread = new Thread[countThread + 1];
	}
	else
	{
		Thread* tempThread = new Thread[countThread + 1];
 
		for (int i = 0; i < countThread; i++)
		{
			tempThread[i] = thread[i]; 
		}
		delete [] thread;
 
		thread = tempThread;		
	}			
	return thread;
}

void setDataThread(Thread* thread, const int countThread, char* name,int priority)
{
	strcpy_s(thread[countThread].name, 20,  name);
	thread[countThread].priority = priority;	
}

void PrintAllThread(CHAR* nameThread, CHAR* allThread, HWND hAllThread)
{
	strcat_s(nameThread,20, "--");
	strcat_s(allThread,255, nameThread);
	strcat_s(allThread,255, priorityThreadLine);
	strcat_s(allThread,255, "\n");
	SetWindowText(hAllThread, allThread);
}

Task* AddTask(Task* task, const int countTask)
{
	if (countTask == 0)
	{		
		task = new Task[countTask + 1];
	}
	else
	{
		Task* tempTask = new Task[countTask + 1];
 
		for (int i = 0; i < countTask; i++)
		{
			tempTask[i] = task[i]; 
		}
		delete [] task;
 
		task = tempTask;		
	}			
	return task;
}

void setDataTask(Task* task, const int countTask, char* name,int priority, int complexity)
{
	strcpy_s(task[countTask].name, 20,  name);
	task[countTask].priority = priority;	
	task[countTask].complexity = complexity;
}

void PrintAllTask(CHAR* nameTask, CHAR* allTask, HWND hAllTask, CHAR* complexity)
{
	strcat_s(nameTask,20, "--");
	strcat_s(allTask,255, nameTask);
	strcat_s(allTask,255, priorityTaskLine);
	strcat_s(allTask,255, "--");
	strcat_s(allTask,255, complexity);
	strcat_s(allTask, 255, "\n");
	SetWindowText(hAllTask, allTask);
}

int CALLBACK WinMain(HINSTANCE hInstance,
    HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{ 
	DialogBoxParam(hInstance,MAKEINTRESOURCE(IDD_DIALOG1), 0, DlgProc,0);
	return 0;
}

int CompareTask(const void* a, const void* b) 
{
	const Task* task1 = (const Task*)a;
    const Task* task2 = (const Task*)b;
	return ((task1->priority) - (task2->priority));              
}

int CompareThread(const void* a, const void* b) 
{
	const Thread* thread1 = (const Thread*)a;
    const Thread* thread2 = (const Thread*)b;
	return ((thread1->priority) - (thread2->priority));              
}

BOOL CALLBACK DlgProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	switch (uMsg)
	{
	case WM_INITDIALOG:
		{  
			hPriorityThread = GetDlgItem(hwnd,IDC_PRIORITY_THREAD);
			hPriorityTask = GetDlgItem(hwnd, IDC_PRIORITY_TASK);
			hComplexity= GetDlgItem(hwnd, IDC_COMPLEXITY);

			SetIcon(hwnd);
			SetStringComboBox(hPriorityThread, 0);
			SetStringComboBox(hPriorityTask, 0);
			SetStringComboBox(hComplexity, 0);
			break;
		}
	case WM_COMMAND:
		switch (LOWORD(wParam))
		{
		case IDC_PRIORITY_THREAD:
			{
				if (HIWORD(wParam) == CBN_SELENDOK)
				{
					priorityThread = SendMessage(hPriorityThread, CB_GETCURSEL, 0, 0);
					SendMessage(hPriorityThread, CB_GETLBTEXT, priorityThread, (LPARAM)priorityThreadLine);
				}
			}
			break;
		case IDC_PRIORITY_TASK:
			{
				if (HIWORD(wParam) == CBN_SELENDOK)
				{
					priorityTask = SendMessage(hPriorityTask, CB_GETCURSEL, 0, 0);
					SendMessage(hPriorityTask, CB_GETLBTEXT, priorityTask, (LPARAM)priorityTaskLine);
				}
			}
			break;
		case IDC_COMPLEXITY:
			{
				if (HIWORD(wParam) == CBN_SELENDOK)
				{
					complexity = SendMessage(hComplexity, CB_GETCURSEL, 0, 0);
					SendMessage(hComplexity, CB_GETLBTEXT, complexity, (LPARAM)complexityTask);
				}
			}
			break;
		case IDC_ADD_THREAD:
			{
				HWND hTextBoxNameThread = GetDlgItem(hwnd, IDC_NAME_THREAD);
				HWND hAllThread = GetDlgItem(hwnd, IDC_ALL_THREAD);
				HWND hCountThread = GetDlgItem(hwnd, 1012);

				CHAR nameThread[20] = {""};
				CHAR allThread[255] = {""};
				CHAR countThreadStr[2] = {""};

				GetWindowText(hTextBoxNameThread, nameThread, 20);
				if (nameThread[0] != 0)
				{
					GetWindowText(hAllThread, allThread, 255);
					thread = AddThread(thread, countThread);
					setDataThread(thread, countThread, nameThread, priorityThread);
					countThread++;
					PrintAllThread(nameThread, allThread, hAllThread);
					SetWindowText(hTextBoxNameThread,"");
					_itoa_s(countThread,countThreadStr,10);
					SetWindowText(hCountThread, countThreadStr);
				}
				else
				MessageBox(hwnd,"Заполните все поля","Info",MB_OK);
			}
			break;
		case IDC_ADD_TASK:
			{
				HWND hTextBoxNameTask = GetDlgItem(hwnd, IDC_NAME_TASK);
				HWND hAllTask = GetDlgItem(hwnd, IDC_ALL_TASK);
				HWND hCountTask = GetDlgItem(hwnd, IDC_COUNT_TASK);

				CHAR nameTask[20] = {""};
				CHAR allTask[255]={""};
				CHAR countTaskStr[2] = {""};

				GetWindowText(hTextBoxNameTask, nameTask, 20);
				if (nameTask[0] != 0)
				{
					GetWindowText(hAllTask, allTask, 255);
					task = AddTask(task, countTask);
					setDataTask(task, countTask, nameTask, priorityTask, complexity);
					countTask++;
					PrintAllTask(nameTask, allTask, hAllTask, complexityTask);
					SetWindowText(hTextBoxNameTask, "");
					_itoa_s(countTask,countTaskStr,10);
					SetWindowText(hCountTask, countTaskStr);
				}
				else
				MessageBox(hwnd,"Заполните все поля","Info",MB_OK);
			}
			break;
		case IDC_START:
			std::qsort(task,countTask,sizeof(Task),CompareTask);
			std::qsort(thread,countThread,sizeof(Thread),CompareThread);
			ThreadPool::ThreadPool(countThread,thread,countTask,task);
			break;
		case IDCANCEL:
			EndDialog(hwnd,0);
			return FALSE;
		default:
			break;
		}
		break;
	case WM_CLOSE:
		EndDialog(hwnd, 0);
		return FALSE;
	}
	return FALSE;
}



