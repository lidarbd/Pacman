#pragma once
#include <iostream>
#include <cctype>

#include <string>
#include <sstream>

#include <stdlib.h>
#include <stdio.h>
#include <conio.h>
#include <windows.h>
#include <vector>

#include "Board.h"
#include "BoardShapes.h"
#include "definitions.h"
#include "Colors.h"

#include "Strategy.h"
#include "Pacman.h"
#include "Ghost.h"
#include "Fruit.h"

using std::stringstream;
using std::fstream;

class Game
{
public:
	static const int NUM_OF_CREATURES = 6;
	static const int PACMAN_LIVES = 3;
	static const int SCORE = 0;
	static const int PacIdx = 0;
	static const int FruitIdx = 1;
	static const int GhostIdx = 2;
	static const int Min_Len_Line_Fruit = 7;
	static const int Len_Line = 7;

private:
	std::vector <BoardShapes> _myBoard;
	std::vector<Board*> _boardsVec;
	std::vector<string> _fileNames;
	Strategy* creatures[NUM_OF_CREATURES];
	GHOST_LEVEL ghostLevel;
	int pacmanLives; 
	int gameScore, timePointWin, timePointDeath;
	int currNumOfCreatures;
	bool _gameOver;
	bool color;
	int _currGameIdx; 
	bool _load;
	bool _isPassed;

public:
	Game(bool color, vector<string> fileName, GHOST_LEVEL level, bool load);
	Game(const Game& other) = delete;
	Game& operator= (const Game & other) = delete;

	~Game();
	void deleteCreatures();
	void deleteBoards();

	void loadGame(bool silent);
	void silentMode(fstream& resultFile);
	void GameLoop();
	void initCreatures(bool silent = false);
	void initGhosts(bool silent = false);

	bool isGameOver(Pacman* pacman);
	bool isWon(Pacman* pacman);

	bool isOtherCreatureInCell(Coord currPacPos, Coord otherPos);
	void pacmanHitsGhost(Pacman* pacman, int currGhost, fstream& resultFile, fstream& stepsFile, Fruit* fruit, bool silent = false);
	void pacmanHitsFruit(Fruit* fruit, Pacman* pacman);
	void setScoreAfterHitsFruit(Cell cell, Pacman* pacman, Fruit* fruit);
	void userPressESC(char& userKey);
	void printLives(Pacman* pacman)const;
	void printScore(Pacman* pacman)const;
	void printIfWon(bool lost)const;
	void setForNextBoard(bool& lost, fstream& resultFile, Pacman* pacman, bool silent = false);
	void printSilent();
	void updatesPacmanStatus(Pacman* pacman, fstream& resultFile,fstream& stepsFile, Fruit* fruit,  bool silent = false);

	DIRECTION getDirectionUser(char userDir);
	void openFiles(fstream& stepsFile, fstream& resultFile, string fileName);
	void writeStepsToFile(fstream& stepsFile, int index, Fruit* fruit);
	void getDirectionFromFile(fstream& stepsFile);
	void readStepsFromFile(fstream& stepsFile, fstream& resultFile, string line, Fruit* fruit, Pacman* pacman, bool silent);
	void handleFruitFromFile(string line, stringstream& input, Fruit* fruit);
};


