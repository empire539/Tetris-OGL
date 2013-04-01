#pragma warning(disable:4996) // Disable deprecated functions warning

#include "Game.h"
#include <string>

/** Constructor. */
Game::Game(HINSTANCE hInstance)
	:m_hInstance(hInstance), 
	 m_bFullscreen(false) {
}

/** Deconstructor. */
Game::~Game() {}

void Game::getArgs(LPSTR args) {
	LPSTR argList = strtok(args, " ");

	while (argList) {
		if (!stricmp(argList, "-fullscreen")) {
			m_bFullscreen = true;
		}

		argList = strtok(NULL, " ");
	}
}

void Game::run() {

}