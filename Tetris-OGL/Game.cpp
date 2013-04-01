#pragma warning(disable:4996) // Disable deprecated functions warning

#include <string>
#include "Game.h"

#define SWITCH_FULLSCREEN "-fullscreen"
#define WINDOW_WIDTH 800
#define WINDOW_HEIGHT 600

/** Constructor. */
Game::Game(HINSTANCE hInstance)
	:m_hInstance(hInstance), 
	 m_bFullscreen(false) {
}

/** Deconstructor. */
Game::~Game() {}

/**
 * Parses the command line arguments for a fullscreen switch.
 */
void Game::getArgs(LPSTR args) {
	LPSTR argList = strtok(args, " ");

	while (argList) {
		if (!stricmp(argList, SWITCH_FULLSCREEN)) {
			m_bFullscreen = true;
		}

		argList = strtok(NULL, " ");
	}
}

/**
 * Main game loop.
 * Processes any incoming messages and updates/draws the app window.
 */
void Game::run() {
	GameWindow gameWindow(WINDOW_WIDTH, WINDOW_HEIGHT, m_bFullScreen);

	MSG winMessage;
	winMessage.message = ~WM_QUIT;

	DWORD sleepTime = FRAME_TIME;
	DWORD expirationTime = GetTickCount() + FRAME_TIME;

	bool bUpdate = false;

	// Process messages until WM_QUIT is received (game loop)
	while (winMessage.message != WM_QUIT) {
		// MsgWaitForMultipleObjectsEx returns WAIT_TIMEOUT if no messages have arrived
		bool bMsgArrived = (WAIT_TIMEOUT != MsgWaitForMultipleObjectsEx(0, NULL, sleepTime, QS_ALLEVENTS, 0));

		// At least one message has arrived, so process them all
		if (bMsgArrived) {
			while (PeekMessage(&winMessage, NULL, 0, 0, PM_REMOVE)) {
				TranslateMessage(&winMessage);
				DispatchMessage(&winMessage);
			}

			// Update the app window when the clock time is up 
			bUpdate = (GetTickCount() >= expirationTime);
		}
		else {
			// Update the app window on each WAIT_TIMEOUT
			bUpdate = true;
		}

		// Update the app window if needed
		if (bUpdate) {
			DWORD currentTime = GetCurrentTime();

			gameWindow.update(currentTime);
			gameWindow.draw();

			expirationTime += FRAME_TIME;
		}

		sleepTime = expirationTime - GetCurrentTime();

		// Recompute expirationTime in case processing was stopped 
		// (e.g. due to moving the window)
		if (sleepTime > FRAME_TIME) {
			sleepTime = FRAME_TIME;
			expirationTime = GetCurrentTime() + FRAME_TIME;
		}
	}
}
