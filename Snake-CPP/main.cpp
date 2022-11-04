#include <iostream>
#include <conio.h>
#include <windows.h>
#include <dos.h>
#include <time.h>

#define MAXSNAKESize 100
#define MAXFRAMEX 119
#define MAXFRAMEY 29

HANDLE console = GetStdHandle(STD_OUTPUT_HANDLE);
COORD CursorPosition;

void GoToXY(int x, int y);
void SetCursor(bool visible, DWORD Size);

class Point
{
private:
	int x;
	int y;
public:
	Point() { x = y = 10; }

	Point(int x, int y) 
	{
		this->x = x;
		this->y = y;
	}

	void SetPoint(int x, int y) 
	{
		this->x = x;
		this->y = y;
	}

	int GetX() { return x; }
	int GetY() { return y; }

	void MoveUp() { y--; if (y < 0) y = MAXFRAMEY; }
	void MoveDown() { y++; if (y > MAXFRAMEY) y = 0; }
	void MoveLeft() { x--; if (x < 0) x = MAXFRAMEX; }
	void MoveRight() { x++; if (x > MAXFRAMEX) x = 0; }

	void Draw(char ch = 'O') 
	{
		GoToXY(x, y);
		std::cout << ch;
	}

	void Erase() 
	{
		GoToXY(x, y);
		std::cout << " ";
	}

	void CopyPos(Point* p) 
	{
		p->x = x;
		p->y = y;
	}

	int IsEqual(Point* p) 
	{
		if (p->x == x && p->y == y) return 1;
		return 0;
	}

	void Debug() { std::cout << "(" << x << "," << y << ") "; }
};

class Snake 
{
private:
	Point* Cell[MAXSNAKESize];
	Point Fruit;
	int Size, State, Started, Blink; 
	char Dir;
public:
	Snake() 
	{
		Size = 1; 

		Cell[0] = new Point(20, 20); 
		for (int i = 1; i < MAXSNAKESize; i++) Cell[i] = NULL;

		Fruit.SetPoint(rand() % MAXFRAMEX, rand() % MAXFRAMEY);

		State = 0;
		Started = 0;
	}

	void AddCell(int x, int y) { Cell[Size++] = new Point(x, y); }

	void TurnUp() { if (Dir != 's') Dir = 'w'; }
	void TurnDown() { if (Dir != 'w') Dir = 's'; }
	void TurnLeft() { if (Dir != 'd') Dir = 'a'; }
	void TurnRight() { if (Dir != 'a') Dir = 'd'; }

	void Move() 
	{
		system("cls");

		if (State == 0) 
		{
			if (!Started) 
			{
				std::cout << "press any key to start";

				std::cout << "\n\nmade by sakiere (https://github.com/sakiere)";

				_getch();

				Started = 1;
				State = 1;
			}

			else 
			{
				std::cout << "game over - press any key to start again";

				_getch();

				State = 1;
				Size = 1;
			}
		}

		for (int i = Size - 1; i > 0; i--) { Cell[i - 1]->CopyPos(Cell[i]); }

		switch (Dir) 
		{
		case 'w':
			Cell[0]->MoveUp();
			break;
		case 's':
			Cell[0]->MoveDown();
			break;
		case 'a':
			Cell[0]->MoveLeft();
			break;
		case 'd':
			Cell[0]->MoveRight();
			break;
		}

		if (SelfCollision()) State = 0;

		if (Fruit.GetX() == Cell[0]->GetX() && Fruit.GetY() == Cell[0]->GetY()) 
		{
			AddCell(0, 0);
			Fruit.SetPoint(rand() % MAXFRAMEX, rand() % MAXFRAMEY);
		}

		for (int i = 0; i < Size; i++) Cell[i]->Draw();

		SetConsoleTextAttribute(console, 242);

		if (!Blink) Fruit.Draw('#');
		Blink = !Blink;

		SetConsoleTextAttribute(console, 252);

		Sleep(100);
	}

	int SelfCollision() 
	{
		for (int i = 1; i < Size; i++) if (Cell[0]->IsEqual(Cell[i])) return 1;
		return 0;
	}

	void Debug() 
	{
		for (int i = 0; i < Size; i++) Cell[i]->Debug();
	}

};

void GoToXY(int x, int y)
{
	CursorPosition.X = x;
	CursorPosition.Y = y;

	SetConsoleCursorPosition(console, CursorPosition);
}
void SetCursor(bool visible, DWORD Size)
{
	if (Size == 0) Size = 20;

	CONSOLE_CURSOR_INFO lpCursor;
	lpCursor.bVisible = visible;
	lpCursor.dwSize = Size;
	SetConsoleCursorInfo(console, &lpCursor);
}

int main() 
{
	SetCursor(0, 0);
	srand((unsigned)time(NULL));
 
	Snake snake;
	char op = 'l';

	do 
	{
		if (_kbhit()) op = _getch();

		switch (op) 
		{
		case 'w':
		case 'W':
			snake.TurnUp();
			break;
		case 's':
		case 'S':
			snake.TurnDown();
			break;
		case 'a':
		case 'A':
			snake.TurnLeft();
			break;
		case 'd':
		case 'D':
			snake.TurnRight();
			break;
		}

		snake.Move();

	} while (op != 'e');

	return 0;
}
