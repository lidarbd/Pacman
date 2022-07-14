#include"GameMenu.h"
#include <filesystem>

using std::filesystem::current_path;
using namespace std;

GameMenu::GameMenu():game(nullptr)
{
	getBoardsFromFile();
}

GameMenu::~GameMenu()
{
	delete game;
}

bool GameMenu::getInfoFromCommandLine(int argc, char* argv[])
{
	bool res=false;

	if (argc > 1 && strcmp((char*)argv[1],"-load") == 0 )
	{
		res = true;
	}
	
	return res;
}

void GameMenu::printMenu(int argc, char* argv[])
{
	char choice;
	GHOST_LEVEL levelChoice;
	bool load = getInfoFromCommandLine(argc, argv);

	if (!load)
	{
		while (1)
		{
			cout << "(1) Start a new game without color" << endl << "(2) Start a new game with color" << endl << "(8) Present instructions and keys" << endl << "(9) EXIT " << endl;
			choice = _getch();

			system("cls");

			switch (choice)
			{
			case '1':
				startNewGame(false);
				break;
			case'2':
				startNewGame(true);
				break;
			case '8':
				intructions();
				break;
			case '9':
				return;
			default:
				cout << "Error! Please enter valid number\n" << endl;
			}
		}
	}
	else
	{
		bool silent = isSilent(argc, argv);
		loadGame(false, silent);
	}

}

bool GameMenu::isSilent(int argc, char* argv[])
{
	bool res = false;
	if (argc > 2 && strcmp((char*)argv[2], "-silent") == 0)
	{
		res = true;
	}
	return res;
}

void GameMenu:: loadGame(bool color, bool silent)
{
	GHOST_LEVEL levelChoice= GHOST_LEVEL::NOVICE;

	game = new Game(color, fileNames, levelChoice, true);
	game->loadGame(silent);
}

GHOST_LEVEL GameMenu::getLevelChoice()
{
	char choice;

	while (1)
	{
		cout << "Choose level:" <<endl << "(a) BEST" << endl << "(b) GOOD" << endl << "(c) NOVICE" << endl;
		choice = _getch();

		system("cls");

		switch (choice)
		{
		case 'a':
			return GHOST_LEVEL::BEST;
			break;
		case 'b':
			return GHOST_LEVEL::GOOD;
			break;
		case 'c':
			return GHOST_LEVEL::NOVICE;
			break;
		default:
			cout << "Error! Please enter valid choice\n" << endl;
			break;
		}

	}
}

void GameMenu::startNewGame(bool color)
{
	delete game;

	GHOST_LEVEL levelChoice = getLevelChoice();

	char choice;
	vector<string> screenName;
	string name;
	bool exist = false;


	cout << "If you want to choose a specific screen please enter y for yes" << endl;
	choice = _getch();

	system("cls");

	switch (choice)
	{
	case 'y':
		cout << "Please enter the screen name" << endl;
		cin >> name;
		system("cls");
		screenName.push_back(name);
		exist = findNameScreen(name);
		if (exist)
		{
			game = new Game(color, screenName, levelChoice, false);
			game->GameLoop();
		}
		else
			cout << "There is no such a screen!!\n\n" << endl;
		break;
	default:
		if (fileNames.size() == 0)
			cout << "There is no files!!" << endl;
		else
		{
			game = new Game(color, fileNames, levelChoice, false);
			game->GameLoop();
		}
			
		break;
	}
		
}

bool GameMenu::findNameScreen(string name)
{
	bool found = false;
	for (int i = 0; i < fileNames.size() && !found; ++i)
	{
		if (fileNames[i].compare(name) == 0)
			found = true;
	}

	return found;
}

void GameMenu::intructions()
{
	cout << "As PACMAN your mission is simple:" << endl
		<< "EAT ALL THE BREADCRUMBS!" << endl
		<< "Navigate through the board using the keyboard's following:" << endl
		<< "D- Right" << endl << "A- Left " << endl << "W- Up" << endl << "X- Down" << endl << "S- Stay" << endl
		<< "There are ghosts hunting you and your job is to avoid them!" << endl
		<< "Notice! The Pacman has 3 lives. " << endl
		<< "Every time the Pacman is eaten by the ghosts the Pacman loses one life." << endl
		<< "If the Pacman has 0 lives - the game is OVER" << endl
		<< "Collect as many breadcrumbs as possible to earn the highest score." << endl
		<< "* You can always pause and unpause your game using the ESC button." << endl
		<< "GOOD LUCK! :) \n" << endl;
}

void GameMenu::getBoardsFromFile()
{
	string path = current_path().string();
	string extention = ".screen";

	for (const auto& entry : filesystem::directory_iterator(path))
	{
		if (entry.path().extension() == extention)
		{
			fileNames.push_back(entry.path().stem().string() + ".screen");
		}
	}

	sortFileNames();
}


void GameMenu::sortFileNames()
{
	for (int i = 0; i < fileNames.size(); ++i)
	{
		for (int j = i; j < fileNames.size() - 1; ++j)
		{
			if (strcmp(fileNames[j + 1].c_str(), fileNames[j].c_str()) < 0)
				swapFileName(fileNames[j], fileNames[j + 1]);
		}
	}
}

void GameMenu::swapFileName(string s1, string s2)
{
	string tmp = s1;
	s1 = s2;
	s2 = tmp;
}




