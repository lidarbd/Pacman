#include "Game.h"

using namespace std;

Game::Game(bool color, vector<string> fileName, GHOST_LEVEL level, bool load) :
	pacmanLives(PACMAN_LIVES), gameScore(SCORE), ghostLevel(level), timePointDeath(0), timePointWin(0), _currGameIdx(0),_load(load)
{
	for (int i = 0; i < fileName.size(); ++i)
	{
		_myBoard.push_back(fileName[i]);
		_boardsVec.push_back(new Board(_myBoard[i].getBoardShape(), _myBoard[i].getRows(), _myBoard[i].getCols()));
	}
	_fileNames = fileName;

	_gameOver = false;
	this->color = color;
	_isPassed = true;
};

void Game::initCreatures(bool silent)
{
	for (int i = 0; i < currNumOfCreatures; ++i)
	{
		creatures[i] = new Strategy(_boardsVec[_currGameIdx]->getPacmanPosition(), _boardsVec[_currGameIdx], silent);
	}
	
	Pacman* _pacman = new Pacman(_boardsVec[_currGameIdx]->getPacmanPosition(), _boardsVec[_currGameIdx], pacmanLives, gameScore, silent);
	creatures[PacIdx] = _pacman;
	_pacman = nullptr;
	delete _pacman;

	Fruit* _fruit = new Fruit(_boardsVec[_currGameIdx]->getFruitPos(), _boardsVec[_currGameIdx], silent);
	creatures[FruitIdx] = _fruit;
	_fruit = nullptr;
	delete _fruit;

	initGhosts(silent);
}

void Game::initGhosts(bool silent)
{
	for (int i = GhostIdx; i < currNumOfCreatures; ++i)
	{
		Ghost* ghost = new Ghost(_boardsVec[_currGameIdx]->getGhostPos(i - GhostIdx), ghostLevel, _boardsVec[_currGameIdx], silent);
		creatures[i] = ghost;
		ghost = nullptr;
		delete ghost;
	}
}

/*This function is running the game*/
void Game::GameLoop()
{
	bool lost = false;
	char userKey;

	fstream stepsFile, resultFile;
	string currFileName;

	while (!lost && _currGameIdx < _boardsVec.size())
	{
		openFiles(stepsFile, resultFile, _fileNames[_currGameIdx]);

		currNumOfCreatures = _boardsVec[_currGameIdx]->getCurrGhosts() + GhostIdx;
		initCreatures();

		_boardsVec[_currGameIdx]->printBoard(color);
		Pacman* pacman = dynamic_cast<Pacman*>(creatures[PacIdx]);
		Fruit* fruit = dynamic_cast<Fruit*>(creatures[FruitIdx]);

		printLives(pacman);
		printScore(pacman);

		userKey = _getch();
		bool flag = true;
		_gameOver = lost = false;
		timePointDeath = timePointWin = 0;

		stepsFile << color << endl;

		while (!_gameOver)
		{
			if (userKey != Cell::ESC)
			{
				DIRECTION userDir = getDirectionUser(userKey);
				creatures[PacIdx]->setDirection(userDir);
			}

			while (!_kbhit() && !_gameOver)
			{
				Sleep(200);

				if (userKey == Cell::ESC)
					userPressESC(userKey);

				for (int i = 0; i < currNumOfCreatures && !_gameOver; ++i)
				{
					if (i == PacIdx || flag)
					{
						creatures[i]->move(color, false);
						writeStepsToFile(stepsFile, i, fruit);
					}

					if (i == PacIdx)
					{
						timePointWin++;
						timePointDeath++;
					}

					/*Checks if pacman hits fruit*/
					pacmanHitsFruit(fruit, pacman);
					updatesPacmanStatus(pacman,resultFile, stepsFile, fruit);
				}
				if (flag)
				{
					flag = false;
				}
				else
				{
					flag = true;
				}

				if (isGameOver(pacman) || isWon(pacman))
				{
					_gameOver = true;
				}
			}
			if (!_gameOver)
			{
				userKey = _getch();
			}
		}

		setForNextBoard(lost, resultFile, pacman);

		if (!lost)
			resultFile << "Point of time that the pacman won: " << timePointWin << endl;

		stepsFile.close();
		resultFile.close();
	}

	printIfWon(lost);

	userKey = _getch();
	system("cls");
}


void Game::loadGame(bool silent)
{
	bool lost = false, won=false;

	fstream stepsFile, resultFile;
	string currFileName, line;
	
	while (!lost &&_currGameIdx < _boardsVec.size())
	{
		openFiles(stepsFile, resultFile, _fileNames[_currGameIdx]);

		if (!stepsFile || !resultFile)
		{
			cout << "File doesn't exist!" << endl;
			return;
		}

		getline(stepsFile, line);
		if (line == "1")
			color = true;

		currNumOfCreatures = _boardsVec[_currGameIdx]->getCurrGhosts() + GhostIdx;
		initCreatures(silent);

		if(!silent)
			_boardsVec[_currGameIdx]->printBoard(color);

		Pacman* pacman = dynamic_cast<Pacman*>(creatures[PacIdx]);
		Fruit* fruit = dynamic_cast<Fruit*>(creatures[FruitIdx]);
		
		if (!silent)
		{
			printLives(pacman);
			printScore(pacman);
		}

		_gameOver = lost = false;
		timePointDeath = timePointWin = 0;

		while (getline(stepsFile, line) && !_gameOver)
		{
			if(!silent)
				Sleep(100);
			readStepsFromFile(stepsFile,resultFile, line, fruit, pacman, silent);
			if (isGameOver(pacman) || isWon(pacman))
			{
				_gameOver = true;
			}
		}

		setForNextBoard(lost, resultFile, pacman, silent);

		printSilent();

		stepsFile.close();
		resultFile.close();
	}

	printIfWon(lost);
}

void Game::setForNextBoard(bool& lost, fstream& resultFile, Pacman* pacman, bool silent)
{
	if (!silent)
		system("cls");

	if (isGameOver(pacman))
	{
		if (color)
			SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), (int)Colors::COLORS::WHITE);
		if (!silent)
		{
			cout << "\n\n\t\t\t\tGAME OVER!" << endl;
		}
		lost = true;
	}
	pacmanLives = pacman->getLives();
	gameScore = pacman->getScore();

	if (silent)
	{
		silentMode(resultFile);
	}

	deleteCreatures();
	_currGameIdx++;
}

void Game::printSilent()
{
	if (_isPassed)
	{
		cout << "Test Passed" << endl;
	}
	else
	{
		cout << "Test Failed" << endl;
	}
}

void Game::printIfWon(bool lost)const
{
	if (!lost)
	{
		if (color)/*The user won*/
			SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), (int)Colors::COLORS::WHITE);
		cout << "\n\n\t\t\t\tYOU WON! CONGRATULATION! :)" << endl;
		cout << "\n\n\t\t\t\tPress any key to continue..." << endl;
	}
}

void Game::silentMode(fstream& resultFile)
{
	string line, tmp;
	int num;
	
	if (color)/*The user won*/
		SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), (int)Colors::COLORS::WHITE);
		
	getline(resultFile, line);

	stringstream input(line);

	for (int i = 0; i < Len_Line; ++i)
	{
		input >> tmp;
	}
	input >> num;

	if ((tmp == "died:" && num != timePointDeath) || (tmp == "won:" && num != timePointWin))
	{
		_isPassed = false;
	}
}

void Game::writeStepsToFile(fstream& stepsFile, int index, Fruit* fruit)
{
	switch (index)
	{
	case PacIdx:
		stepsFile << "Pacman ";
		break;
	case FruitIdx:
		stepsFile << "Fruit ";
		if (fruit->getCurrTimeCount() == 1 && fruit->isDisappear())
		{
			stepsFile << "disappearance ";
		}
		else if (fruit->getTimeCount() == fruit->getCurrTimeCount() + 1 && !fruit->isDisappear())
		{
			stepsFile << "appearance ";
			stepsFile << fruit->getCurrentPos().getX() << " " << fruit->getCurrentPos().getY() <<" ";
			stepsFile << "value: ";
			stepsFile << fruit->getValue()<< " ";
		}
		break;
	case GhostIdx:
	case GhostIdx+1:
	case GhostIdx+2:
	case GhostIdx+3:
		stepsFile << "Ghost " << index-GhostIdx << " ";
		break;
	}
	
	stepsFile << (int)creatures[index]->getDirection() << endl;
}

void Game::readStepsFromFile(fstream& stepsFile, fstream& resultFile, string line, Fruit* fruit, Pacman* pacman, bool silent)
{
	stringstream input(line);
	int direction, currGhost;
	string creatureType;

	input >> creatureType;

	if (creatureType == "Pacman")
	{
		input >> direction;
		creatures[PacIdx]->setDirection((DIRECTION)direction);
		creatures[PacIdx]->move(color, true);
		timePointWin++;
		timePointDeath++;
	}
	else if (creatureType == "Fruit")
	{
		handleFruitFromFile(line, input, fruit);
	}
	else if (creatureType == "Ghost")
	{
		input >> currGhost;
		input >> direction;

		creatures[currGhost + GhostIdx]->setDirection((DIRECTION)direction);
		creatures[currGhost + GhostIdx]->move(color, true);
	}
	pacmanHitsFruit(fruit, pacman);
	updatesPacmanStatus(pacman, resultFile, stepsFile, fruit, silent);
}

void Game::updatesPacmanStatus(Pacman* pacman, fstream& resultFile, fstream& stepsFile,Fruit*fruit, bool silent)
{
	for (int j = GhostIdx; j < currNumOfCreatures && !_gameOver; ++j)
	{
		pacmanHitsGhost(pacman, j,resultFile, stepsFile, fruit, silent);

		if (isGameOver(pacman))
			_gameOver = true;
		else if(!silent)
			printLives(pacman);
	}
	
	if(!silent)
		printScore(pacman);
}

void Game::handleFruitFromFile(string line, stringstream& input, Fruit* fruit)
{
	string appearanceState, tmp;
	int x, y, value, direction;
	Coord fruitPos;

	if (line.size() > Min_Len_Line_Fruit)
	{
		input >> appearanceState;

		if (appearanceState == "appearance")
		{
			input >> x >> y;
			fruitPos.setX(x);
			fruitPos.setY(y);
			creatures[FruitIdx]->setCurrentPos(fruitPos);

			input >> tmp >> value ;

			fruit->setValue(value);
			fruit->show(color);
		}
		else
		{
			fruit->fruitHitsGhostOrPac(color, _load);
		}
	}
	input >> direction;
	creatures[FruitIdx]->setDirection((DIRECTION)direction);
	creatures[FruitIdx]->move(color, _load);
}

void Game::getDirectionFromFile(fstream& stepsFile)
{
	string line, creature;
	stringstream input(line);
	int direction, currGhost;
	int index;

	while (!getline(stepsFile, line))
	{
		input >> creature;
		
		if (creature == "Ghost")
		{
			input >> currGhost;
			index = GhostIdx + currGhost;
		}
		else if (creature == "Pacman")
		{
			index = PacIdx;
		}
		else
		{
			index = FruitIdx;
		}

		input >> direction;
		creatures[index]->setDirection((DIRECTION)direction);
	}
}

void Game::openFiles(fstream& stepsFile, fstream& resultFile,string fileName)
{
	string resultFileName, stepsFileName;
	int len, screenLen;

	screenLen = 6;

	len = fileName.size() - screenLen;
	fileName.erase(fileName.begin()+len, fileName.end());

	resultFileName = fileName + "result";
	stepsFileName = fileName + "steps";

	if (!_load)
	{
		stepsFile.open(stepsFileName, ios::out, ios::trunc);
		resultFile.open(resultFileName, ios::out, ios::trunc);
	}
	else
	{
		stepsFile.open(stepsFileName, ios::in);
		resultFile.open(resultFileName, ios::in);
	}
	
}

/*This function checks if the game is over*/
bool Game::isGameOver(Pacman* pacman)
{
	bool res = false;
	if (pacman->getLives() == 0)
	{
		res = true;
	}
	return res;
}

/*This function checks if the user is won, if he ate all the breadCrumbs*/
bool Game::isWon(Pacman* pacman)
{
	bool res = false;

	if (pacman->getEatenBreadCrumbs() == _boardsVec[_currGameIdx]->getNumOfBreadCrumbs())
	{
		res = true;
	}
	return res;
}

void Game::pacmanHitsGhost(Pacman* pacman, int currGhost, fstream& resultFile, fstream& stepsFile, Fruit* fruit, bool silent)
{
	if (isOtherCreatureInCell(creatures[PacIdx]->getNewPos().getCoord(), creatures[currGhost]->getNewPos().getCoord())
		|| isOtherCreatureInCell(creatures[PacIdx]->getCurrentPos(), creatures[currGhost]->getNewPos().getCoord()))
	{
		if (pacman)
			pacman->pacmanHitsGhost(color);
		
		if (!_load)
		{
			resultFile << "Point of time that the pacman died: " << timePointDeath << endl;
			creatures[currGhost]->move(color, false);
			writeStepsToFile(stepsFile, currGhost, fruit);
		}
		else
		{
			if (silent)
			{
				silentMode(resultFile);
			}
		}
	}
}

void Game::pacmanHitsFruit(Fruit* fruit, Pacman* pacman)
{
	Cell newCell;
	if (!fruit->isDisappear())
	{
		if (isOtherCreatureInCell(creatures[PacIdx]->getCurrentPos(), creatures[FruitIdx]->getNewPos().getCoord()))
		{
			pacman->setScore(pacman->getScore() + fruit->getValue());
			fruit->fruitHitsGhostOrPac(color, _load);
		}
		else if (isOtherCreatureInCell(creatures[FruitIdx]->getCurrentPos(), creatures[PacIdx]->getNewPos().getCoord()))
		{
			newCell.setCoord(creatures[FruitIdx]->getCurrentPos().getX(), creatures[FruitIdx]->getCurrentPos().getY());
			newCell.setSign(_boardsVec[_currGameIdx]->getBoard()[newCell.getCoord().getX()][newCell.getCoord().getY()].getSign());
			setScoreAfterHitsFruit(newCell, pacman, fruit);
			fruit->fruitHitsGhostOrPac(color, _load);
			pacman->pacmanHitsFruit(color);
		}
		newCell.setCoord(0, 0);
		creatures[FruitIdx]->setNewPos(newCell);
	}
}

void Game::setScoreAfterHitsFruit(Cell cell, Pacman* pacman, Fruit* fruit)
{
	if (cell.getSign() == Cell::SIGN::FRUIT_ON_CRUMB)
	{
		pacman->setScore(pacman->getScore() + fruit->getValue() + 1);
		pacman->setEatenBreadCrumbs(pacman->getEatenBreadCrumbs() + 1);
	}
	else
	{
		pacman->setScore(pacman->getScore() + fruit->getValue());
	}
}


/*This function pauses the game until the user press ESC again*/
void Game::userPressESC(char& userKey)
{
	do
	{
		userKey = _getch();
	} while (userKey != Cell::ESC);
	userKey = 0;
}

/*This function converts user key to specific direction */
DIRECTION Game::getDirectionUser(char userDir)
{
	if (userDir == 'A' || userDir == 'a')
	{
		return DIRECTION::LEFT;
	}
	else if (userDir == 'D' || userDir == 'd')
	{
		return DIRECTION::RIGHT;
	}
	else if (userDir == 'W' || userDir == 'w')
	{
		return DIRECTION::UP;
	}
	else if (userDir == 'X' || userDir == 'x')
	{
		return DIRECTION::DOWN;
	}
	else if (userDir == 'S' || userDir == 's')
	{
		return DIRECTION::STAY;
	}

}


bool Game::isOtherCreatureInCell(Coord currPos, Coord otherPos)
{
	return(((currPos.getX() == otherPos.getX()) &&
		(currPos.getY() == otherPos.getY())));
}


void Game::printLives(Pacman* pacman)const
{
	gotoxy(_boardsVec[_currGameIdx]->getTextPos().getY(), _boardsVec[_currGameIdx]->getTextPos().getX());
	if (color)
		SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), (int)Colors::COLORS::LIGHTRED);
	cout << "LIVES: ";
	cout << pacman->getLives() << "\n";
}

void Game::printScore(Pacman* pacman)const
{
	gotoxy(_boardsVec[_currGameIdx]->getTextPos().getY(), (_boardsVec[_currGameIdx]->getTextPos().getX()) + 1);
	if (color)
		SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), (int)Colors::COLORS::LIGHTRED);
	cout << "SCORE: ";
	cout << pacman->getScore();
}

Game::~Game()
{
	deleteBoards();
}

void Game::deleteBoards()
{
	for (int i = 0; i < _boardsVec.size(); ++i)
	{
		delete _boardsVec[i];
	}
}
void Game::deleteCreatures()
{
	for (int i = 0; i < currNumOfCreatures; ++i)
	{
		delete creatures[i];
	}
}

