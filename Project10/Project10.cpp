
#include <windows.h>
#include <windowsX.h>
#include <tchar.h>
#include <vector>
#include <string>
#include <commctrl.h>
#include "Resource.h"

#pragma comment(lib,"comctl32")

#define BUTTON_COUNT 16
#define TIMER_ID 1
#define TIMER_ID2 2


#pragma comment(linker,"\"/manifestdependency:type='win32' \
name='Microsoft.Windows.Common-Controls' version='6.0.0.0' \
processorArchitecture='*' publicKeyToken='6595b64144ccf1df' language='*'\"")

using namespace std;

WPARAM wParam;
LPARAM lParam;
HWND hSpin, hEdit1, hEdit2, hProgress;
vector<HWND> buttonHandles;
vector<int>answers;
int counter = 0;
TCHAR tx[200];

//Прототипы
void UpdateCounter(HWND hwnd, int count);
void initDlg(HWND hwnd, WPARAM wParam, LPARAM lParam);
void StartGame(HWND hwnd, HWND hEdit1, WPARAM wParam, LPARAM lParam);
void addNumbers(HWND hwnd, WPARAM wParam, LPARAM lParam);
void DisplayAnswers(HWND hEdit1, const vector<int>& answers);



void UpdateCounter(HWND hwnd, int count) {
	TCHAR buffer[100];
	_stprintf_s(buffer, _T("%d"), count);
	SetWindowText(hEdit2, buffer);
}

void CALLBACK OnTimer(HWND hwnd, UINT message, UINT_PTR idTimer, DWORD dwTime) {
	
	TCHAR buffer[100];
	GetDlgItemText(hwnd, IDC_EDIT2, buffer, sizeof(buffer) / sizeof(buffer[0]));
	int count = stoi(buffer);

	if (count > 0) {
		count--;
		UpdateCounter(hwnd, count);
	}
	else {
		KillTimer(hwnd, TIMER_ID);
		MessageBox(hwnd, _T("Время закончилось!"), _T("TIMER"), MB_OK | MB_ICONINFORMATION);
		MessageBox(hwnd, _T("Вы проиграли! :("), _T("GAME"), MB_OK | MB_ICONINFORMATION);
		StartGame(hwnd, hEdit1, wParam, lParam);
	}
}

void addNumbers(HWND hwnd, WPARAM wParam, LPARAM lParam)
{
	vector<int> numbers;

	int num;
	srand((NULL));
	for (int i = 0; i < BUTTON_COUNT; i++)
	{
		num = rand() % 100 + 1;
		for (int j = 0; j < numbers.size(); j++)
		{
			if (numbers.size() == NULL)
			{
				break;
			}
			else {
				if (num == numbers[j])
				{
					num += rand() % 100 + 1;
					break;
				}
			}
		}
		numbers.push_back(num);

	}

	

	//fill buttons with numbers
	TCHAR buffer[100];
	for (int i = 0; i < BUTTON_COUNT; i++)
	{
		wsprintf(buffer, TEXT("%d"), numbers[i]);
		SetDlgItemText(hwnd, IDC_BUTTON1 + i, buffer);

	}
}

void initDlg(HWND hwnd, WPARAM wParam, LPARAM lParam)
{
	hSpin = GetDlgItem(hwnd, IDC_SPIN1);
	hEdit1 = GetDlgItem(hwnd, IDC_EDIT1);
	hEdit2 = GetDlgItem(hwnd, IDC_EDIT2);
	hProgress = GetDlgItem(hwnd, IDC_PROGRESS1);


	SendMessage(hSpin, UDM_SETRANGE32, 0, 100);
	SendMessage(hSpin, UDM_SETBUDDY, WPARAM(hEdit2), 0);

	if (hEdit2 != NULL) {
		int textLength = GetWindowTextLength(hEdit2);

		if (textLength > 0) {
			// Эл управления содержит текст
			TCHAR text[300];
			GetWindowText(hEdit2, text, 10);
			int num = stoi(text);
			SendMessage(hProgress, PBM_SETRANGE, 0, MAKELPARAM(0, num));
		}
		else {
			// Эл управления пуст
			SetWindowText(hEdit2, TEXT("10"));
			SendMessage(hProgress, PBM_SETRANGE, 0, MAKELPARAM(0, 10));
		}
	}
	addNumbers(hwnd, wParam, lParam);
	SendMessage(hProgress, PBM_SETSTEP, 1, 0);
	SendMessage(hProgress, PBM_SETPOS, 0, 0);


	
	for (int i = 0; i < BUTTON_COUNT; i++) 
	{
		buttonHandles.push_back(GetDlgItem(hwnd, IDC_BUTTON1+i));
		EnableWindow(buttonHandles[i], TRUE);
	}
	SetTimer(hwnd, TIMER_ID, 1000, OnTimer);

}

void DisplayAnswers(HWND hEdit1, const vector<int>& answers) {
	wstring text;

	for (int answer : answers) {
		text += to_wstring(answer) + L"\r\n";
	}

	SetWindowTextW(hEdit1, text.c_str());

}

void StartGame(HWND hwnd, HWND hEdit1, WPARAM wParam, LPARAM lParam)
{
	MessageBox(hwnd, TEXT("Start new game!"), TEXT("GAME"), MB_OK | MB_ICONINFORMATION);

	answers.clear();
	counter = 0;
	SetWindowText(hEdit1, TEXT(" "));
	SetWindowText(hEdit2, TEXT("30"));
	SendMessage(hProgress, PBM_SETRANGE, 0, 10);
	initDlg(hwnd, wParam, lParam);
}

INT_PTR CALLBACK DlgProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	TCHAR buf[256];
	int buttonValue;
	

	switch (uMsg)
	{
	case WM_INITDIALOG:
		SetTimer(hwnd, TIMER_ID2, 1000, NULL);
		return TRUE;
	case WM_TIMER:
		if (wParam == TIMER_ID2) {
			SendMessage(hProgress, PBM_STEPIT, 0, 0);

			/*KillTimer(hwnd, TIMER_ID2);*/
		}
		break;
	case WM_COMMAND:

		if (LOWORD(wParam) == IDCANCEL)
		{
			EndDialog(hwnd, 0);
			return TRUE;
		}

		else if (LOWORD(wParam) == IDC_BUTTON17)
		{
			StartGame(hwnd, hEdit1, wParam, lParam);
		}

		else if (LOWORD(wParam) >= IDC_BUTTON1 && LOWORD(wParam) <= IDC_BUTTON1 + BUTTON_COUNT - 1) 
		{
			
			if (HIWORD(wParam) == BN_CLICKED) 
			{
				for (int i = 0; i < buttonHandles.size(); i++) 
				{
					if (LOWORD(wParam) == GetDlgCtrlID(buttonHandles[i])) 
					{
						GetDlgItemText(hwnd, GetDlgCtrlID(buttonHandles[i]), buf, 200);
						buttonValue = _tstoi(buf);
						if (answers.empty())
						{
							answers.push_back(buttonValue);
							EnableWindow(buttonHandles[i], FALSE);
							SetDlgItemText(hwnd, GetDlgCtrlID(hEdit1), buf);

						}
						else if (!(answers.empty()) && buttonValue > answers.back())
						{
							if (counter == 14)
							{
								MessageBox(hwnd, TEXT("YOU WIN!"), TEXT("CONGRATULATIONS!"), MB_OK | MB_ICONINFORMATION);
								StartGame(hwnd, hEdit1, wParam, lParam);
								break;
							}
							else
							{	counter++;
								answers.push_back(buttonValue);
								EnableWindow(buttonHandles[i], FALSE);
								DisplayAnswers(hEdit1, answers);
										
							}

						}
						else 
						{
							break;
						}
							
					}
				}
			}
		}
		break;
	}


	return FALSE;
}




//int WINAPI WinMain(HINSTANCE hInst, HINSTANCE, LPSTR, int)
//{
//	return DialogBox(hInst, MAKEINTRESOURCE(IDD_DIALOG1), NULL, DlgProc);
//}

int WINAPI _tWinMain(HINSTANCE hInst, HINSTANCE hPrev, LPTSTR lpszCmdLine, int nCmdShow)
{
	INITCOMMONCONTROLSEX icc = { sizeof(INITCOMMONCONTROLSEX) };
	icc.dwICC = ICC_WIN95_CLASSES;
	InitCommonControlsEx(&icc);
	return DialogBox(hInst, MAKEINTRESOURCE(IDD_DIALOG1), NULL, DlgProc);
}