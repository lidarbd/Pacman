#include "Fruit.h"
#include "Board.h"

void Fruit::setValue()
{
	value =(rand() % 5) + 5;
}

void Fruit::setValue(int newVal)
{
	value = newVal;
}
void Fruit::setTimeCount(const int newTimeCount)
{
	timeCount = newTimeCount;
}

void Fruit::setCurrTimeCount(const int newTimeCount)
{
	currTimeCount = newTimeCount;
}
void Fruit::setDisappear(const bool newVal)
{
	disappear = newVal;
}

void Fruit::getCoord(bool color)
{
	int x, y;

	x = rand() % board->getRowSize();
	y = rand() % board->getColSize();
	
	while (!isValidCoord(x, y))
	{
		x = rand() % board->getRowSize();
		y = rand() % board->getColSize();
	}
	currentPos.setX(x);
	currentPos.setY(y);

	setValue();
}

bool Fruit::isValidCoord(int x, int y)
{
	return((board->getBoard()[x][y].getSign() != Cell::SIGN::WALL) && (board->getBoard()[x][y].getSign() != Cell::SIGN::PACMAN)
		&& (board->getBoard()[x][y].getSign() != Cell::SIGN::GHOST_ON_CRUMB) && (board->getBoard()[x][y].getSign() != Cell::SIGN::GHOST_ON_NONE)
		&& (board->getBoard()[x][y].getSign() != Cell::SIGN::TEXT) && !isTunnel(board->getBoard()[x][y]));
}

void Fruit::show(bool color)
{
	Cell currPosCell;
	currPosCell.setCoord(currentPos.getX(), currentPos.getY());
	currPosCell.setSign(board->getBoard()[currentPos.getX()][currentPos.getY()].getSign());

	if (currPosCell.getSign() == Cell::SIGN::BreadCrumbs)
	{
		currPosCell.setSign(Cell::SIGN::FRUIT_ON_CRUMB);
	}
	else if (currPosCell.getSign() == Cell::SIGN::NONE)
	{
		currPosCell.setSign(Cell::SIGN::FRUIT_ON_NONE);
	}

	board->getBoard()[currentPos.getX()][currentPos.getY()].setSign(currPosCell.getSign());

	if (!silent)
	{
		gotoxy(currentPos.getY(), currentPos.getX());
		if (color)
			SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), (int)Colors::COLORS::LIGHTCYAN);
		cout << value;
	}

	disappear = false;
}

void Fruit::disappearance(bool color)
{
	Cell currPosCell;
	currPosCell.setCoord(currentPos.getX(), currentPos.getY());
	currPosCell.setSign(board->getBoard()[currentPos.getX()][currentPos.getY()].getSign());

	if(!silent)
		gotoxy(currentPos.getY(), currentPos.getX());

	if (currPosCell.getSign() == Cell::SIGN::FRUIT_ON_CRUMB)
	{
		currPosCell.setSign(Cell::SIGN::BreadCrumbs);
		if (color)
			SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), (int)Colors::COLORS::LIGHTCYAN);
		if (!silent)
			cout << '.';
	}
	else if (currPosCell.getSign() == Cell::SIGN::FRUIT_ON_NONE)
	{
		currPosCell.setSign(Cell::SIGN::NONE);
		if (!silent)
			cout << ' ';
	}


	board->getBoard()[currentPos.getX()][currentPos.getY()].setSign(currPosCell.getSign());
}


void Fruit::updatePosition(bool color)
{
	if (newPos.getSign() == Cell::SIGN::BreadCrumbs)
	{
		newPos.setSign(Cell::SIGN::FRUIT_ON_CRUMB);
	}
	else if (newPos.getSign() == Cell::SIGN::NONE)
	{
		newPos.setSign(Cell::SIGN::FRUIT_ON_NONE);
	}
	if (!silent)
	{
		gotoxy(newPos.getCoord().getY(), newPos.getCoord().getX());
		if (color)
			SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), (int)Colors::COLORS::LIGHTCYAN);
		cout << value;
		gotoxy(oldPos.getCoord().getY(), oldPos.getCoord().getX());

	}

	if (oldPos.getSign() ==Cell::SIGN::FRUIT_ON_CRUMB || oldPos.getSign() == Cell::SIGN::BreadCrumbs)
	{
		if (color)
			SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), (int)Colors::COLORS::LIGHTCYAN);
		if (!silent)
			cout << '.';
		oldPos.setSign(Cell::SIGN::BreadCrumbs);
	}
	else
	{
		if (!silent)
			cout << ' ';
		oldPos.setSign(Cell::SIGN::NONE);
	}

	currentPos = newPos.getCoord();
}

void Fruit::checkCollision(bool color, bool load)
{
	if (!isValidPos())
	{
		hitsTunnelOrWall();
	}
	else if (newPos.getSign() == Cell::SIGN::GHOST_ON_CRUMB || newPos.getSign() == Cell::SIGN::GHOST_ON_NONE)
	{
		fruitHitsGhostOrPac(color, load);
	}
	if (newPos.getSign() != Cell::SIGN::PACMAN)
	{
		updatePosition(color);
	}
		
}

void Fruit::fruitHitsGhostOrPac(bool color, bool load)
{
	currTimeCount= 0;
	unShow(color, load);
}

void Fruit::unShow(bool color, bool load)
{
	disappear = true;
	disappearance(color);

	if (!load)
	{
		getCoord(color);
		timeCount = (rand() % 40) + 30;
	}
}


void Fruit::move(bool color, bool load)
{
	if (!load)
	{
		if (currTimeCount == 0 && !disappear)
		{
			unShow(color, load);
		}
		else if (timeCount == currTimeCount)
		{
			show(color);
		}
	}

	if (!disappear)
	{
		currTimeCount--;

		if(!load)
			randomDirection();

		Strategy::move(color, load);
	}
	else
	{
		currTimeCount++;
	}
}

