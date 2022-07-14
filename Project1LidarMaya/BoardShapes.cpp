#include "BoardShapes.h"

using namespace std;

BoardShapes::BoardShapes(string fileName) :rows(0), cols(0)
{
	char ch;
	int maxCols = 0;
	bool isText = false;
	bool firstLine = true;
	
	ifstream myfile;
	myfile.open(fileName, ios::in);

	string line;

	while (getline(myfile, line) && rows<=ROWS)
	{
		vector<char> str;

		if (firstLine)
		{
			maxCols = line.size();
			firstLine = false;

			if (maxCols > COLS)
				maxCols = COLS;
		}

		cols = line.size();

		if (cols > 0)
		{
			if (cols > COLS)
				cols = COLS;

			for (int i = 0; i < cols && !isText; ++i)
			{
				ch = line.at(i);

				if (ch == '&')
				{
					handleText(myfile, line, maxCols);
					isText = true;
				}
				else
				{
					str.push_back(ch);
				}

			}
		}
		if (!isText)
		{
			if (cols < maxCols)
			{
				for (int i = cols; i < maxCols; ++i)
				{
					str.push_back(' ');
				}
			}
			board_shape.push_back(str);
			rows++;
		}
		else
			isText = false;

	}
	cols = maxCols;
	myfile.close();
}

void BoardShapes::handleText(istream &myfile, string line, int maxCols)
{
	char ch;
	int j;
	int tmpCol=-1;
	bool firstLine;

	for (int i = 0; i < HEIGHT;++i)
	{
		vector<char> str;

		for ( j = 0; j < cols; ++j)
		{
			ch = line.at(j);
			
			if (ch == '&')
			{
				str.push_back(ch);
				tmpCol = j;
				firstLine = true;
			}
			if (j == tmpCol)
			{
				if (firstLine)
				{
					firstLine = false;
					for (int k = 0; k < WIDTH-1; ++k)
					{
						str.push_back('%');
					}
				}
				else
				{
					for (int k = 0; k < WIDTH; ++k)
					{
						str.push_back('%');
					}
				}
				
				j += WIDTH;

				if(j < cols)
					ch = line.at(j);
			}
			if (j < cols)
				str.push_back(ch);
		}
		if (j > cols)
		{
			j--;
		}
		if (j < maxCols)
		{
			for (int i = j; i < maxCols; ++i)
			{
				str.push_back(' ');
			}
		}
		board_shape.push_back(str);

		if (i < HEIGHT - 1)
		{
			if (!getline(myfile, line))
				line[tmpCol]=' ';
			else
			{
				cols = line.size();
				if (cols > COLS)
					cols = COLS;
			}
		}	
	}
	
	rows += 3;
}


