#include "Game.h"
#include "GameException.h"

/**
 * The application entry point.
 */
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow) {
	try {
		Game tetrisGame(hInstance);
		tetrisGame.getArgs(lpCmdLine);
		tetrisGame.run();
	}
	catch (GameException e) {
		MessageBox(NULL, e.what(), "Error", MB_OK | MB_ICONEXCLAMATION);
	}

	return 0;
}
