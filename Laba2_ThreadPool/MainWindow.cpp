#include <Windows.h>
#include <string>
#include "string.h"
#include "afxres.h"
#include "resource1.h"
#include "View.h"
#include "ThreadPool.h"



BOOL CALLBACK DlgProc(HWND, UINT, WPARAM, LPARAM);



Thread* threads = 0;
Task* tasks= 0;
int countThreads = 0, complex = 0, priorityThreads = 0, priorityTask = 0, countTasks = 0;
CHAR priorityThreadsLine[20] = "1.Высокий", complexityTask[20] = "1.Высокая", priorityTaskLine[20] = "1.Высокий";
HWND hPriorityThreads, hPriorityTask, hComplexity;

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
	strcat_s(allThread,255, priorityThreadsLine);
	strcat_s(allThread,255, "\n");
	SetWindowText(hAllThread, allThread);
}

Task* AddTask(Task* task, const int countTasks)
{
	if (countTasks == 0)
	{		
		task = new Task[countTasks + 1];
	}
	else
	{
		Task* tempTask = new Task[countTasks + 1];
 
		for (int i = 0; i < countTasks; i++)
		{
			tempTask[i] = task[i]; 
		}
		delete [] task;
 
		task = tempTask;		
	}			
	return task;
}

void setDataTask(Task* task, const int countTasks, char* name,int priority, int complexity)
{
	strcpy_s(task[countTasks].name, 20,  name);
	task[countTasks].priority = priority;	
	task[countTasks].complexity = complexity;
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
			hPriorityThreads = GetDlgItem(hwnd,IDC_PRIORITY_THREAD);
			hPriorityTask = GetDlgItem(hwnd, IDC_PRIORITY_TASK);
			hComplexity= GetDlgItem(hwnd, IDC_COMPLEXITY);

			SetIcon(hwnd);
			SetStringComboBox(hPriorityThreads, 0);
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
					priorityThreads = SendMessage(hPriorityThreads, CB_GETCURSEL, 0, 0);
					SendMessage(hPriorityThreads, CB_GETLBTEXT, priorityThreads, (LPARAM)priorityThreadsLine);
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
					complex = SendMessage(hComplexity, CB_GETCURSEL, 0, 0);
					SendMessage(hComplexity, CB_GETLBTEXT, complex, (LPARAM)complexityTask);
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
					threads = AddThread(threads, countThreads);
					setDataThread(threads, countThreads, nameThread, priorityThreads);
					countThreads++;
					PrintAllThread(nameThread, allThread, hAllThread);
					SetWindowText(hTextBoxNameThread,"");
					_itoa_s(countThreads,countThreadStr,10);
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
				HWND hcountTasks = GetDlgItem(hwnd, IDC_COUNT_TASK);

				CHAR nameTask[20] = {""};
				CHAR allTask[255]={""};
				CHAR countTasksStr[2] = {""};

				GetWindowText(hTextBoxNameTask, nameTask, 20);
				if (nameTask[0] != 0)
				{
					GetWindowText(hAllTask, allTask, 255);
					tasks = AddTask(tasks, countTasks);
					setDataTask(tasks, countTasks, nameTask, priorityTask, complex);
					countTasks++;
					PrintAllTask(nameTask, allTask, hAllTask, complexityTask);
					SetWindowText(hTextBoxNameTask, "");
					_itoa_s(countTasks,countTasksStr,10);
					SetWindowText(hcountTasks, countTasksStr);
				}
				else
				MessageBox(hwnd,"Заполните все поля","Info",MB_OK);
			}
			break;
		case IDC_START:
			{
				std::qsort(tasks,countTasks,sizeof(Task),CompareTask);
				std::qsort(threads,countThreads,sizeof(Thread),CompareThread);
				ThreadPool* threadPool = new ThreadPool(countThreads,threads, hwnd);
				threadPool->GetAllTasks(tasks, countTasks);
			}
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



