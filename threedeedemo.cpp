// threedeedemo.cpp : Defines the entry point for the console application.
//


#include "stdafx.h"
#include <stdio.h> 
#include <windows.h>
#include <process.h>
#include <iostream>
#include <cwchar>
#include <memory>
#include <clocale>
#include <chrono>
#include <conio.h>


#define WINDOW_WIDTH 1200
#define WINDOW_HEIGHT 1000
#define SCREEN_WIDTH 145
#define SCREEN_HEIGHT 95
#define FLOOR_HEIGHT 15
#define CEIL_HEIGHT 15
#define MAP_HEIGHT 10
#define MAP_WIDTH 10
#define SIGHT_INCREMENT 0.01
#define BORDER_THICKNESS 0.02
#define PI 3.14159

int main()
{
	//FORGOT TO CHANGE DIMENSIONS FROM PIXELS TO CHARS


	HANDLE wHnd, rHnd;
	COORD characterBufferSize, bufferSize;
	COORD characterPosition;
	SMALL_RECT consoleWriteArea;
	SMALL_RECT windowSize;
	CHAR_INFO* consoleBuffer = new CHAR_INFO[1200 * 800];


	float playerX = 2, playerY = 2;
	float playerAng = PI / 2;
	float playerFOV = PI / 4;
	float playerLOS = 8;

	CONSOLE_FONT_INFOEX cfi;
	cfi.cbSize = sizeof(cfi);
	cfi.nFont = 0;
	cfi.dwFontSize.X = 8;                   // Width of each character in the font
	cfi.dwFontSize.Y = 9;                  // Height
	cfi.FontFamily = FF_DONTCARE;
	cfi.FontWeight = FW_NORMAL;
	std::wcscpy(cfi.FaceName, L"Raster Fonts"); // Choose your font
	SetCurrentConsoleFontEx(GetStdHandle(STD_OUTPUT_HANDLE), FALSE, &cfi);

	HWND console = GetConsoleWindow();
	RECT r;
	GetWindowRect(console, &r);
	MoveWindow(console, r.left, r.top, 1200, 800, TRUE);
	
	windowSize = { 0, 0, WINDOW_WIDTH - 1, WINDOW_HEIGHT - 1 };

	/* A COORD struct for specificying the console's screen buffer dimensions */
	bufferSize = { SCREEN_WIDTH, SCREEN_HEIGHT };
	wHnd = GetStdHandle(STD_OUTPUT_HANDLE);
	rHnd = GetStdHandle(STD_INPUT_HANDLE);
	characterBufferSize = { SCREEN_WIDTH, SCREEN_HEIGHT };
	characterPosition = { 0, 0 };
	consoleWriteArea = { 0, 0, SCREEN_WIDTH - 1, SCREEN_HEIGHT - 1 };
	SetConsoleScreenBufferSize(wHnd, bufferSize);


	char* map = new char[MAP_WIDTH * MAP_HEIGHT];
	INPUT_RECORD* eventBuffer = new INPUT_RECORD[1];
	for (int y = 0; y < MAP_HEIGHT; y++) {
		for (int x = 0; x < MAP_WIDTH; x++) {
			map[x + y * MAP_WIDTH] = ' ';
			if (x == 0 || y == 0 || x == MAP_WIDTH - 1 || y == MAP_HEIGHT - 1 || (x == MAP_WIDTH / 2 && (y <= MAP_HEIGHT / 2 + 1 && y >= MAP_HEIGHT / 2 - 1))) {
				map[x + y * MAP_WIDTH] = '#';
			}
			
		}
	}
	//map[MAP_WIDTH / 2 + MAP_HEIGHT / 2 * MAP_WIDTH] = '#';
	
	auto start = std::chrono::system_clock::now();
	auto end = std::chrono::system_clock::now();
	std::chrono::duration<double> elapsedTime = end - start;

	while (true) {
		start = end;
		end = std::chrono::system_clock::now();
		elapsedTime = end - start;

		char input = ' ';

		/* Variable for holding the number of current events, and a point to it */
		DWORD numEvents = 0;


		/* Variable for holding how many events were read */
		DWORD numEventsRead = 0;


		/* Put the number of console input events into numEvents */
		GetNumberOfConsoleInputEvents(rHnd, &numEvents);


		if (numEvents) /* if there's an event */
		{
			/* Allocate the correct amount of memory to store the events */
			eventBuffer = (INPUT_RECORD*)malloc(sizeof(INPUT_RECORD) * numEvents);

			/* Place the stored events into the eventBuffer pointer */
			ReadConsoleInput(rHnd, eventBuffer, numEvents, &numEventsRead);
		
			switch (eventBuffer[0].EventType) {
			case KEY_EVENT:
				input = eventBuffer[0].Event.KeyEvent.uChar.AsciiChar;
				break;
			}
		}

		
		/*switch (input) {
		case 'w':
			playerX += cos(playerAng) * 5.4 * elapsedTime.count();
			playerY += sin(playerAng) * 5.4 * elapsedTime.count();
			break;
		case 's':
			playerX -= cos(playerAng) * 5.4 * elapsedTime.count();
			playerY -= sin(playerAng) * 5.4 * elapsedTime.count();
			break;
		case 'a':
			playerAng -= 3.5 * elapsedTime.count();
			//system("cls");
			//std::cout << playerAng << std::endl;
			break;
		case 'd':
			playerAng += 3.5 * elapsedTime.count();
			//system("cls");
			//std::cout << playerAng << std::endl;
			break;
		}*/

		bool wDown = false;
		bool sDown = false;
		bool aDown = false;
		bool dDown = false;
		bool qDown = false;
		bool eDown = false;
		if (GetAsyncKeyState(0x57)) {
			wDown = true;
		}
		if (GetAsyncKeyState(0x41)) {
			aDown = true;
		}
		if (GetAsyncKeyState(0x53)) {
			sDown = true;
		}
		if (GetAsyncKeyState(0x44)) {
			dDown = true;
		}
		if (GetAsyncKeyState(0x51)) {
			qDown = true;
		}
		if (GetAsyncKeyState(0x45)) {
			eDown = true;
		}
		if (wDown) {
			playerX += cos(playerAng) * 5.4 * elapsedTime.count();
			playerY += sin(playerAng) * 5.4 * elapsedTime.count();
			if (round(playerX) < 0 || round(playerX) >= MAP_WIDTH || map[(int)round(playerX) + (int)round(playerY) * MAP_WIDTH] == '#') {
				playerX -= cos(playerAng) * 5.4 * elapsedTime.count();
			}
			if (round(playerY) < 0 || round(playerY) >= MAP_HEIGHT || map[(int)round(playerX) + (int)round(playerY) * MAP_WIDTH] == '#') {
				playerY -= sin(playerAng) * 5.4 * elapsedTime.count();
			}
		}
		if (sDown) {
			playerX -= cos(playerAng) * 5.4 * elapsedTime.count();
			playerY -= sin(playerAng) * 5.4 * elapsedTime.count();
			if (round(playerX) < 0 || round(playerX) >= MAP_WIDTH || map[(int)round(playerX) + (int)round(playerY) * MAP_WIDTH] == '#') {
				playerX += cos(playerAng) * 5.4 * elapsedTime.count();
			}
			if (round(playerY) < 0 || round(playerY) >= MAP_HEIGHT || map[(int)round(playerX) + (int)round(playerY) * MAP_WIDTH] == '#') {
				playerY += sin(playerAng) * 5.4 * elapsedTime.count();
			}
		}
		if (aDown) {
			playerX += sin(playerAng) * 5.4 * elapsedTime.count();
			playerY -= cos(playerAng) * 5.4 * elapsedTime.count();
			if (round(playerX) < 0 || round(playerX) >= MAP_WIDTH || map[(int)round(playerX) + (int)round(playerY) * MAP_WIDTH] == '#') {
				playerX -= sin(playerAng) * 5.4 * elapsedTime.count();
			}
			if (round(playerY) < 0 || round(playerY) >= MAP_HEIGHT || map[(int)round(playerX) + (int)round(playerY) * MAP_WIDTH] == '#') {
				playerY += cos(playerAng) * 5.4 * elapsedTime.count();
			}
		}
		if (dDown) {
			playerX -= sin(playerAng) * 5.4 * elapsedTime.count();
			playerY += cos(playerAng) * 5.4 * elapsedTime.count();
			if (round(playerX) < 0 || round(playerX) >= MAP_WIDTH || map[(int)round(playerX) + (int)round(playerY) * MAP_WIDTH] == '#') {
				playerX += sin(playerAng) * 5.4 * elapsedTime.count();
			}
			if (round(playerY) < 0 || round(playerY) >= MAP_HEIGHT || map[(int)round(playerX) + (int)round(playerY) * MAP_WIDTH] == '#') {
				playerY -= cos(playerAng) * 5.4 * elapsedTime.count();
			}
		}
		if (qDown) {
			playerAng -= 3.5 * elapsedTime.count();
		}
		if (eDown) {
			playerAng += 3.5 * elapsedTime.count();
		}
		char* tempStore = new char[SCREEN_WIDTH];
		double* distStore = new double[SCREEN_WIDTH];
		for (int i = 0; i < SCREEN_WIDTH; i++) {
			tempStore[i] = -1;
			distStore[i] = -1;
		}
		for (int y = 0; y < SCREEN_HEIGHT; y++) {
			for (int x = 0; x < SCREEN_WIDTH; x++) {
				char _char = ' ';
				
				//for (double a = playerAng - playerFOV / 2; a <= playerAng + playerFOV / 2; a += playerFOV / SCREEN_WIDTH) {
				double a = playerAng - playerFOV / 2 + playerFOV * ((double)x / SCREEN_WIDTH);
				if (y == 0 || tempStore[x] == -1) {
					double px = playerX;
					double py = playerY;
					double dist = 0;
					for (double d = 0; d < playerLOS; d += SIGHT_INCREMENT) {
						px += cos(a) * SIGHT_INCREMENT;
						py += sin(a) * SIGHT_INCREMENT;
						if (map[(int)round(px) + (int)round(py)* MAP_WIDTH] != ' ')
							break;
					}
					dist = sqrt((px - playerX) * (px - playerX) + (py - playerY) * (py - playerY));
					dist = round(dist * 100) / 100;
					//std::cout << dist << std::endl;
					_char = ' ';
					if (dist < playerLOS) {
						_char = '#';
					}
					if (dist < 0.75 * playerLOS) {
						_char = 176;
					}
					if (dist < 0.5 * playerLOS) {
						_char = 177;
					}
					if (dist < 0.25 * playerLOS) {
						_char = 178;
					}
					if (abs(round(px * 100) / 100 - round(px)) < BORDER_THICKNESS || abs(round(py * 100) / 100 - round(py)) < BORDER_THICKNESS) {
						_char = ' ';
					}
					tempStore[x] = _char;
					distStore[x] = dist;
				}
				int thickness = 10 + min(SCREEN_HEIGHT, SCREEN_HEIGHT / distStore[x] * 0.75);
				if (abs(y - SCREEN_HEIGHT / 2) <= thickness) {
					_char = tempStore[x];
				}
				else {
					if (y >= (SCREEN_HEIGHT - FLOOR_HEIGHT)) {
						_char = ' ';
						if (y - (SCREEN_HEIGHT - FLOOR_HEIGHT) > 0.25 * (FLOOR_HEIGHT))
							_char = '.';
						if (y - (SCREEN_HEIGHT - FLOOR_HEIGHT) > 0.5 * (FLOOR_HEIGHT))
							_char = '-';
						if (y - (SCREEN_HEIGHT - FLOOR_HEIGHT) > 0.75 * (FLOOR_HEIGHT))
							_char = '+';
					}
					else if (y <= CEIL_HEIGHT) {
						_char = ' ';
						if (y < 0.75 * CEIL_HEIGHT)
							_char = '.';
						if (y < 0.5 * CEIL_HEIGHT)
							_char = '-';
						if (y < 0.25 * CEIL_HEIGHT)
							_char = '+';
					}
					else {

						_char = ' ';

					}
				}
				//}
				CHAR_INFO cinfo;
				cinfo.Attributes = FOREGROUND_BLUE | FOREGROUND_GREEN | FOREGROUND_RED;
				cinfo.Char.AsciiChar = _char;
				consoleBuffer[x + y * SCREEN_WIDTH] = cinfo;
			}
		}
		for (int y = 0; y < MAP_HEIGHT; y++) {
			for (int x = 0; x < MAP_WIDTH; x++) {
				CHAR_INFO cinfo;
				cinfo.Attributes = FOREGROUND_BLUE | FOREGROUND_GREEN | FOREGROUND_RED;
				cinfo.Char.AsciiChar = map[x + y * MAP_WIDTH];
				if (x == (int)round(playerX) && y == (int)round(playerY)) {
					cinfo.Char.AsciiChar = 'X';
				}
				consoleBuffer[(MAP_WIDTH - 1 - x) + (MAP_HEIGHT - 1 - y) * SCREEN_WIDTH] = cinfo;
			}
		}
		WriteConsoleOutputA(wHnd, consoleBuffer, characterBufferSize, characterPosition, &consoleWriteArea);
	}
    return 0;
}

