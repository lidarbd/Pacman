#include "Pacman.h"
#include "Board.h"


void Pacman::setLives(const int newLives)
{
	lives = newLives;
}
void Pacman::setScore(const int newScore)
{
	score = newScore;
}

void Pacman::setEatenBreadCrumbs(const int newEatenBreadCrumbs)
{
	eatenBreadCrumbs = newEatenBreadCrumbs;
}

void Pacman::move(bool color, bool load)
{
	Strategy::move(color, load);

	board->setPacmanPos(currentPos);
}


void Pacman::checkCollision(bool color, bool load)
{
	if (newPos.getSign() == Cell::SIGN::WALL) /*if the pacman hits a wall, it stays in it's position*/
	{
		if (!silent)
		{
			gotoxy(oldPos.getCoord().getY(), oldPos.getCoord().getX());
			if (color)
				SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), (int)Colors::COLORS::YELLOW);
			cout << '@';
		}

		currentPos = oldPos.getCoord();
	}
	else if (newPos.getSign() != Cell::SIGN::GHOST_ON_CRUMB && newPos.getSign() != Cell::SIGN::GHOST_ON_NONE 
		&& newPos.getSign() != Cell::SIGN::FRUIT_ON_CRUMB && newPos.getSign() != Cell::SIGN::FRUIT_ON_NONE)
		{
		if (newPos.getSign() == Cell::SIGN::BreadCrumbs)
		{
			score++;
			eatenBreadCrumbs++;
		}
		updatePosition(color);
	}

}


/*This function checks if the pacman ate breadCrumb, and updates the score*/
void Pacman::updatePosition(bool color)
{
	oldPos.setSign(Cell::SIGN::NONE);
	if(!silent)
		printCreatureAfterMove(PACMAN, color, (int)Colors::COLORS::YELLOW, oldPos.getCoord(), newPos.getCoord());
	currentPos = newPos.getCoord();
	
	newPos.setSign(Cell::SIGN::PACMAN);
}

/*This function brings the pacman to it's starting position and takes one life of the pacman*/
void Pacman::pacmanHitsGhost(bool color)
{
	oldPos.setSign(Cell::SIGN::NONE); /*Updates the sign in the cell*/
	newPos.setCoord(startPos.getX(), startPos.getY()); /*Brings back pacman to his start position*/
	newPos.setSign(Cell::SIGN::PACMAN);/*Updates the sign in the cell*/
	
	if(!silent)
		printCreatureAfterMove(PACMAN, color, (int)Colors::COLORS::YELLOW, oldPos.getCoord(), newPos.getCoord());

	currentPos = newPos.getCoord();

	board->setPacmanPos(currentPos);/*Updates pacman position in the board*/

	lives--; /*Updates pacman's lives*/
}

void Pacman::pacmanHitsFruit(bool color)
{
	newPos.setSign(Cell::SIGN::PACMAN);/*Updates the sign in the cell*/

	if(!silent)
		printCreatureAfterMove(PACMAN, color, (int)Colors::COLORS::YELLOW, oldPos.getCoord(), newPos.getCoord());

	currentPos = newPos.getCoord();
}
