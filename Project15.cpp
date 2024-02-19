#include "framework.h"
#include "Project15.h"﻿
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
using namespace std;

WPARAM wParam;
LPARAM lParam;
HWND hSpin, hEdit1, hEdit2, hProgress;
vector<HWND> buttonHandles;
vector<int>answers;
int counter = 0;
TCHAR tx[200];

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
			TCHAR text[300];
			GetWindowText(hEdit2, text, 10);
			int num = stoi(text);
			SendMessage(hProgress, PBM_SETRANGE, 0, MAKELPARAM(0, num));
		}
		else {
			SetWindowText(hEdit2, TEXT("10"));
			SendMessage(hProgress, PBM_SETRANGE, 0, MAKELPARAM(0, 10));
		}
	}
	addNumbers(hwnd, wParam, lParam);
	SendMessage(hProgress, PBM_SETSTEP, 1, 0);
	SendMessage(hProgress, PBM_SETPOS, 0, 0);



	for (int i = 0; i < BUTTON_COUNT; i++)
	{
		buttonHandles.push_back(GetDlgItem(hwnd, IDC_BUTTON1 + i));
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
							{
								counter++;
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
int WINAPI _tWinMain(HINSTANCE hInst, HINSTANCE hPrev, LPTSTR lpszCmdLine, int nCmdShow)
{
	INITCOMMONCONTROLSEX icc = { sizeof(INITCOMMONCONTROLSEX) };
	icc.dwICC = ICC_WIN95_CLASSES;
	InitCommonControlsEx(&icc);
	return DialogBox(hInst, MAKEINTRESOURCE(IDD_DIALOG1), NULL, DlgProc);
}

ATOM MyRegisterClass(HINSTANCE hInstance)
{
    WNDCLASSEXW wcex;

    wcex.cbSize = sizeof(WNDCLASSEX);

    wcex.style          = CS_HREDRAW | CS_VREDRAW;
    wcex.lpfnWndProc    = WndProc;
    wcex.cbClsExtra     = 0;
    wcex.cbWndExtra     = 0;
    wcex.hInstance      = hInstance;
    wcex.hIcon          = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_PROJECT15));
    wcex.hCursor        = LoadCursor(nullptr, IDC_ARROW);
    wcex.hbrBackground  = (HBRUSH)(COLOR_WINDOW+1);
    wcex.lpszMenuName   = MAKEINTRESOURCEW(IDC_PROJECT15);
    wcex.lpszClassName  = szWindowClass;
    wcex.hIconSm        = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));

    return RegisterClassExW(&wcex);
}
BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{
   hInst = hInstance;

   HWND hWnd = CreateWindowW(szWindowClass, szTitle, WS_OVERLAPPEDWINDOW,
      CW_USEDEFAULT, 0, CW_USEDEFAULT, 0, nullptr, nullptr, hInstance, nullptr);

   if (!hWnd)
   {
      return FALSE;
   }

   ShowWindow(hWnd, nCmdShow);
   UpdateWindow(hWnd);

   return TRUE;
}


LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    switch (message)
    {
    case WM_COMMAND:
        {
            int wmId = LOWORD(wParam);
            switch (wmId)
            {
            case IDM_ABOUT:
                DialogBox(hInst, MAKEINTRESOURCE(IDD_ABOUTBOX), hWnd, About);
                break;
            case IDM_EXIT:
                DestroyWindow(hWnd);
                break;
            default:
                return DefWindowProc(hWnd, message, wParam, lParam);
            }
        }
        break;
    case WM_PAINT:
        {
            PAINTSTRUCT ps;
            HDC hdc = BeginPaint(hWnd, &ps);
            EndPaint(hWnd, &ps);
        }
        break;
    case WM_DESTROY:
        PostQuitMessage(0);
        break;
    default:
        return DefWindowProc(hWnd, message, wParam, lParam);
    }
    return 0;
}

INT_PTR CALLBACK About(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
    UNREFERENCED_PARAMETER(lParam);
    switch (message)
    {
    case WM_INITDIALOG:
        return (INT_PTR)TRUE;

    case WM_COMMAND:
        if (LOWORD(wParam) == IDOK || LOWORD(wParam) == IDCANCEL)
        {
            EndDialog(hDlg, LOWORD(wParam));
            return (INT_PTR)TRUE;
        }
        break;
    }
    return (INT_PTR)FALSE;
}
