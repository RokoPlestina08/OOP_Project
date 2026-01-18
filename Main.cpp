#include <iostream>
#include <iomanip>
#include <string>
#include <vector>
#include <thread>
#include <random>
#include <ctime>
#include <algorithm>
#include "Ship.h"
#include "Position.h"
#include "Player.h"
#include "Player1.h"
#include "Player2.h"
#include "Game.h"

#ifdef _WIN32
#include <windows.h>

#define ENABLE_VIRTUAL_TERMINAL_PROCESSING 0x0004
#define DISABLE_NEWLINE_AUTO_RETURN  0x0008

void activateVirtualTerminal()
{
	HANDLE handleOut = GetStdHandle(STD_OUTPUT_HANDLE);
	DWORD consoleMode;
	GetConsoleMode(handleOut, &consoleMode);
	consoleMode |= ENABLE_VIRTUAL_TERMINAL_PROCESSING;
	consoleMode |= DISABLE_NEWLINE_AUTO_RETURN;
	SetConsoleMode(handleOut, consoleMode);
}
#endif

int main() {
#ifdef _WIN32
	HWND activeWindow = GetActiveWindow();
	HMONITOR activeMonitor = MonitorFromWindow(activeWindow, MONITOR_DEFAULTTONEAREST);
	MONITORINFO monitorInfo = { sizeof(MONITORINFO) };
	GetMonitorInfo(activeMonitor, &monitorInfo);

	int screenWidth = monitorInfo.rcMonitor.right - monitorInfo.rcMonitor.left;
	int screenHeight = monitorInfo.rcMonitor.bottom - monitorInfo.rcMonitor.top;

	int Cols = 140;
	int Rows = 35;

	short fontWidth = static_cast<short>(screenWidth / Cols);
	short fontHeight = static_cast<short>(screenHeight / Rows);

	ShowWindow(GetConsoleWindow(), SW_MAXIMIZE);
	activateVirtualTerminal();
	HWND consoleWindow = GetConsoleWindow();
	SetWindowLong(consoleWindow, GWL_STYLE, GetWindowLong(consoleWindow, GWL_STYLE) & ~WS_MAXIMIZEBOX & ~WS_SIZEBOX);
	CONSOLE_FONT_INFOEX cfi;
	cfi.cbSize = sizeof(cfi);
	cfi.nFont = 0;
	cfi.dwFontSize.X = fontWidth;
	cfi.dwFontSize.Y = fontHeight;
	cfi.FontFamily = FF_DONTCARE;
	cfi.FontWeight = FW_NORMAL;
	std::wcscpy(cfi.FaceName, L"Cascadia code");
	SetCurrentConsoleFontEx(GetStdHandle(STD_OUTPUT_HANDLE), FALSE, &cfi);
#endif
	srand(time(0));
	Game game;
	game.StartGame();

	return 0;
}