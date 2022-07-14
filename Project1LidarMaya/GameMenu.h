#pragma once
#include "Game.h"
#include <iostream>
#include <vector>
#include <string>


class GameMenu
{
	Game* game;
	vector<string> fileNames;

public:
	GameMenu();
	~GameMenu();

	void intructions();
	void printMenu(int argc, char* argv[]);
	void loadGame(bool color, bool silent);
	bool getInfoFromCommandLine(int argc, char* argv[]);
	bool isSilent(int argc, char* argv[]);
	GHOST_LEVEL getLevelChoice();
	void startNewGame(bool color);
	void getBoardsFromFile();
	bool findNameScreen(string name);

	void sortFileNames();
	void swapFileName(string s1, string s2);


};