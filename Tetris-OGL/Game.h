#ifndef GAME_H
#define GAME_H

#include <Windows.h>

/**
 * The main game class to encapsulate the message queue
 */
class Game {
public:
	Game(HINSTANCE hInstance);
	~Game();

	void getArgs(LPSTR args);
	void run();

private:
	static const int FRAME_TIME = 75; // ms

	HINSTANCE m_hInstance;
	bool m_bFullscreen;
};

#endif
