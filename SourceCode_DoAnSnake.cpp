#include <iostream>
#include <conio.h>
#include <Windows.h>
#include <time.h>
#include <stdlib.h>
#include <string.h>
#include <thread>
#include <fstream>
#include <malloc.h>
#define MAX_SIZE_SNAKE 33
#define MAX_SIZE_FOOD 4
#define MAX_SIZE_SPEED
using namespace std;
POINT* snakey;
POINT muiten;
POINT* food = NULL;
POINT gate[6] = { 0 };
int ROUND = 1;
int count_SIZE = 0;
int CHAR_LOCK;
int check = 1;
int check_gate = 0;
int check_outgate = 0;
int z = 1;
int MOVING;
int SPEED;
int HEIGH_CONSOLE = 20, WIDTH_CONSOLE = 70;
int FOOD_INDEX;
int SIZE_SNAKE;
int STATE;
char p[34] = "019120564191205971912056819120551";
char q[34] = "                                ";

HANDLE consolehandle = GetStdHandle(STD_OUTPUT_HANDLE);
void doimau(int color)//doi mau chu 
{
	SetConsoleTextAttribute(consolehandle, color);//chinh mau cua chu
}
void HideCursor() // xoa con tro ve ki tu
{
	CONSOLE_CURSOR_INFO lpCursor;
	lpCursor.bVisible = false;
	lpCursor.dwSize = 20;
	SetConsoleCursorInfo(consolehandle, &lpCursor);
}
void FixConsoleWindow()//xoa nut maximize va co dinh man hinh
{
	HWND consoleWindow = GetConsoleWindow();
	LONG style = GetWindowLong(consoleWindow, GWL_STYLE);
	style = style & ~(WS_MAXIMIZEBOX) & ~(WS_THICKFRAME);
	SetWindowLong(consoleWindow, GWL_STYLE, style);
}
void GotoXY(int x, int y)//di chuyen den toa do (x, y) tren man hinh
{
	COORD coord;
	coord.X = x;
	coord.Y = y;
	SetConsoleCursorPosition(consolehandle, coord);
}
bool IsValid(int x, int y)//kiem tra toa do co trung con ran khong
{
	for (int i = 0; i < SIZE_SNAKE; i++)
	{
		if (i == 0)
			continue;
		if (snakey[i].x == x && snakey[i].y == y)
			return false;
	}
	return true;
}
bool Isvalidgate(int x, int y)//kiem tra toa do co trung voi cong khong
{
	for (int i = 0; i < 5; i++)
		if (x == gate[i].x && y == gate[i].y)
			return false;
	return true;
}
void GenerateFood()// tao mang thuc an
{
	if (food == NULL)
		food = (POINT*)malloc(MAX_SIZE_FOOD * sizeof(POINT));
	int x, y;
	srand(time(NULL));
	for (int i = 0; i < MAX_SIZE_FOOD; i++) {
		do {
			x = rand() % (WIDTH_CONSOLE - 1) + 2;
			y = rand() % (HEIGH_CONSOLE - 1) + 1;
		} while (!IsValid(x, y));
		food[i] = { x, y };
	}
}
void generateSnake()
{
	POINT* snake_temp = (POINT*)realloc(snakey, (SIZE_SNAKE + 1) * sizeof(POINT));
	if (snake_temp != NULL)
	{
		snake_temp[SIZE_SNAKE].x = food[FOOD_INDEX].x;
		snake_temp[SIZE_SNAKE].y = food[FOOD_INDEX].y;
		SIZE_SNAKE++;
		snakey = snake_temp;
	}
}
void generateGate()//tao cong de qua man
{
	int x, y;
	srand(time(NULL));
	do {
		x = rand() % ((WIDTH_CONSOLE - 5) - 5) + 5;
		y = rand() % ((HEIGH_CONSOLE - 5) - 5) + 5;
	} while (!IsValid(x, y));
	gate[0] = { x - 1, y + 1 };
	gate[1] = { x - 1, y };
	gate[2] = { x, y };
	gate[3] = { x + 1, y };
	gate[4] = { x + 1, y + 1 };
	gate[5] = { x, y + 1 };
	for (int i = 0; i < 6; i++)
	{
		if (!IsValid(gate[i].x, gate[i].y))
			return generateGate();
	}
}
void drawGate(POINT gate[])//ve cong vao
{
	doimau(245);
	for (int i = 0; i < 5; i++)
	{
		GotoXY(gate[i].x, gate[i].y);
		cout << "o";
	}
}
void drawOutGate(POINT gate[])//ve cong di ra
{
	doimau(245);
	for (int i = 0; i <= 5; i++)
	{
		if (i == 2) continue;
		GotoXY(gate[i].x, gate[i].y);
		cout << "o";
	}
}
void cleanGate(POINT gate[])//xoa cong
{
	for (int i = 0; i <= 5; i++)
	{
		GotoXY(gate[i].x, gate[i].y);
		cout << " ";
		gate[i].x = 0; gate[i].y = 0;
	}
}
void ResetData()// thiet lap lai du lieu luc bat dau
{
	CHAR_LOCK = 'A', MOVING = 'D', SPEED = 8;
	FOOD_INDEX = 0, WIDTH_CONSOLE = 70, HEIGH_CONSOLE = 20, SIZE_SNAKE = 6;
	if (snakey != NULL)
		free(snakey);
	if (food != NULL)
		free(food);
	food = NULL;
	snakey = (POINT*)malloc((SIZE_SNAKE + 1) * sizeof(POINT));
	snakey[5] = { 10,5 }; snakey[4] = { 11,5 };
	snakey[3] = { 12,5 }; snakey[2] = { 13,5 };
	snakey[1] = { 14, 5 }; snakey[0] = { 15,5 };
	count_SIZE = 0; check_gate = 0; check_outgate = 0;
	cleanGate(gate);
	GenerateFood();
	ROUND = 1;
	check_gate = 0;
}
void DrawBoard(int x, int y, int width, int height, int curPosX = 0, int curPosY = 0)//ve bang cho ran chay, huong dan, thong so luc choi,...
{
	Sleep(100);
	char k = 205, a = 186, b = 187, c = 188, d = 201, e = 200;
	doimau(241);
	for (int i = 0; i <= width + 2; i++)
	{
		Sleep(10);//lam cham lai de co hieu ung ve
		GotoXY(x + i, y);
		cout << (char)219;
		GotoXY(x + i, height + y);
		cout << (char)219;
	}
	for (int i = y + 1; i < height + y; i++)
	{
		Sleep(20);//lam cham lai de co hieu ung ve
		GotoXY(x, i);
		cout << (char)219 << (char)219;
		GotoXY(x + width + 1, i);
		cout << (char)219 << (char)219;
	}
	GotoXY(x + width + 20, y + 1);
	cout << "Huong dan";
	GotoXY(x + width + 5, y + 2);
	cout << "'W': Di Len";
	GotoXY(x + width + 5, y + 4);
	cout << "'A': Qua Phai";
	GotoXY(x + width + 5, y + 6);
	cout << "'S': Di xuong";
	GotoXY(x + width + 5, y + 8);
	cout << "'D': Qua Phai";
	GotoXY(x + width + 5, y + 10);
	cout << "(P): Pause game";
	GotoXY(x + width + 5, y + 12);
	cout << "'Bam nut bat ki de tiep tuc'";
	GotoXY(x + width + 5, y + 14);
	cout << "(L): Luu game";
	GotoXY(x + width + 5, y + 16);
	cout << "(T): load game";
	GotoXY(WIDTH_CONSOLE + 4, 0);
		cout << d;
	GotoXY(WIDTH_CONSOLE + 45, 0);
		cout << b;
	GotoXY(WIDTH_CONSOLE + 4, 18);
		cout << e;
	GotoXY(WIDTH_CONSOLE + 45, 18);
		cout << c;
	for (int i = 0; i < 40; i++)
	{
		GotoXY(WIDTH_CONSOLE + 5 + i, 0);
		cout << k;
		GotoXY(WIDTH_CONSOLE + 5 + i, 18);
		cout << k;
	}
	for (int i = 0; i < 17; i++)
	{
		GotoXY(WIDTH_CONSOLE + 4, 1 + i);
		cout << a;
		GotoXY(WIDTH_CONSOLE + 45, 1 + i);
		cout << a;
	}
	GotoXY(curPosX, curPosY);
	GotoXY(15, HEIGH_CONSOLE + 2); cout << "Thuc an con lai la: ";
	GotoXY(15, HEIGH_CONSOLE + 4); cout << "Man: ";
	GotoXY(11, HEIGH_CONSOLE + 1);
	cout << d;
	GotoXY(61, HEIGH_CONSOLE + 1);
	cout << b;
	for (int i = 1; i < 50; i++)
	{
		GotoXY(11 + i, HEIGH_CONSOLE + 1);
		cout << k;
		GotoXY(11 + i, HEIGH_CONSOLE + 5);
		cout << k;
	}
	for (int i = 2; i < 5; i++)
	{
		GotoXY(11, i + HEIGH_CONSOLE);
		cout << a;
		GotoXY(61, i + HEIGH_CONSOLE);
		cout << a;
	}
	GotoXY(11, HEIGH_CONSOLE + 5);
	cout << e;
	GotoXY(61, HEIGH_CONSOLE + 5);
	cout << c;
}
void StartGame()
{
	doimau(255); system("cls");
	ResetData();
	DrawBoard(0, 0, WIDTH_CONSOLE, HEIGH_CONSOLE);
	STATE = 1;//de kich hoat ve con ran
}
void ExitGame(HANDLE t)
{
	if (snakey != NULL)
		free(snakey);
	if (food != NULL)
		free(food);
	snakey = NULL;
	food = NULL;
	doimau(255); system("cls");
	TerminateThread(t, 0);
}
void PauseGame(HANDLE t)
{
	SuspendThread(t);
}
void ProcessWin()
{
	char a = 1, b = 31, c = 16, d = 17, e = 30;
	STATE = 0;
	Sleep(100);
	for (int i = 74; i < 120; i++)
	{
		for (int j = 0; j < 25; j++)
		{
			GotoXY(i, j);
			cout << " ";
		}
	}
	GotoXY(40, HEIGH_CONSOLE + 2);
	cout << 0;
	doimau(240 +10);
	Sleep(150);

	for (int i = 83; i < 113; i++)
	{
		GotoXY(i, 2); cout << e;
	}
	for (int i = 83; i < 113; i++)
	{
		GotoXY(i, 16); cout << b;
	}
	for (int i = 3; i < 16; i++)
	{
		GotoXY(83, i); cout << d;
	}
	for (int i = 3; i < 16; i++)
	{
		GotoXY(112, i); cout << c;
	}

	GotoXY(88, 4); cout << a << a << "  " << a << a << "   " << a << a << a << "    " << a << "   " << a;
	GotoXY(88, 5); cout << a << a << "  " << a << a << "  " << a << "   " << a << "   " << a << "   " << a;
	GotoXY(90, 6); cout << a << a << "    " << a << "   " << a << "   " << a << "   " << a;
	GotoXY(90, 7); cout << a << a << "    " << a << "   " << a << "   " << a << "   " << a;
	GotoXY(90, 8); cout << a << a << "     " << a << a << a << "     " << a << a << a;

	GotoXY(87, 10); cout << a << "     " << a << "  " << a << a << a << a << a << a << "  " << a << "   " << a;
	GotoXY(87, 11); cout << a << "  " << a << "  " << a << "    " << a << a << "    " << a << a << "  " << a;
	GotoXY(87, 12); cout << a << " " << a << " " << a << " " << a << "    " << a << a << "    " << a << " " << a << " " << a;
	GotoXY(87, 13); cout << a << a << "   " << a << a << "    " << a << a << "    " << a << "  " << a << a;
	GotoXY(87, 14); cout << a << "     " << a << "  " << a << a << a << a << a << a << "  " << a << "   " << a;
	GotoXY(83, 19); cout << "An phim bat ki de quay lai menu!";
}
void Eat()
{

	generateSnake();
	if (SIZE_SNAKE == 33)
	{
		ProcessWin();
		return;
	}
	//snakey[SIZE_SNAKE] = food[FOOD_INDEX];
	if (FOOD_INDEX == MAX_SIZE_FOOD - 1)
	{
		FOOD_INDEX = 0;
		check_gate = 1;
		generateGate();
		drawGate(gate);
		GenerateFood();
	}
	else
	{
		FOOD_INDEX++;
	}
}
void ProcessDead()
{
	char a = 1, b = 31, c = 16, d = 17, e = 30;
	STATE = 2;
	z = 1;
	for (int i = 74; i < 120; i++)
	{
		for (int j = 0; j < 20; j++)
		{
			GotoXY(i, j);
			cout << " ";
		}
	}
	doimau(240 +12);
	Sleep(50);
	for (int i = 83; i < 113; i++)
	{
		GotoXY(i, 2); cout << e;
	}
	for (int i = 83; i < 113; i++)
	{
		GotoXY(i, 16); cout << b;
	}
	for (int i = 3; i < 16; i++)
	{
		GotoXY(83, i); cout << d;
	}
	for (int i = 3; i < 16; i++)
	{
		GotoXY(112, i); cout << c;
	}
	GotoXY(88, 4); cout << a << a << "  " << a << a << "   " << a << a << a << "    " << a << "   " << a;
	GotoXY(88, 5); cout << a << a << "  " << a << a << "  " << a << "   " << a << "   " << a << "   " << a;
	GotoXY(90, 6); cout << a << a << "    " << a << "   " << a << "   " << a << "   " << a;
	GotoXY(90, 7); cout << a << a << "    " << a << "   " << a << "   " << a << "   " << a;
	GotoXY(90, 8); cout << a << a << "     " << a << a << a << "     " << a << a << a;

	GotoXY(88, 10); cout << a << "      " << a << a << a << "   " << a << a << a << "  " << a << a << a;
	GotoXY(88, 11); cout << a << "     " << a << "   " << a << "  " << a << "    " << a;
	GotoXY(88, 12); cout << a << "     " << a << "   " << a << "   " << a << "   " << a << a << a;
	GotoXY(88, 13); cout << a << "     " << a << "   " << a << "    " << a << "  " << a;
	GotoXY(88, 14); cout << a << a << a << a << "   " << a<< a << a << "   " << a << a << a << "  " << a << a << a;
	GotoXY(83, 19); cout << "An phim bat ki de quay lai menu!";
}
void ProcessInGate()
{
	for (int i = 0; i < SIZE_SNAKE - 1; i++)
	{
		snakey[i].x = snakey[i + 1].x;
		snakey[i].y = snakey[i + 1].y;
	}
	SIZE_SNAKE--;
	count_SIZE++;
	if (SIZE_SNAKE == 0)
	{
		cleanGate(gate);
		check_outgate = 1;
		SIZE_SNAKE = 1;
	}
}
void ProcessOutGate()
{
	for (int i = SIZE_SNAKE - 1; i < SIZE_SNAKE; i++)
	{
		snakey[i].x = gate[5].x;
		snakey[i].y = gate[5].y - 1;
		count_SIZE--;
		if (count_SIZE == 0)
		{
			SIZE_SNAKE--;
			cleanGate(gate);
			check_outgate = 0;
			check_gate = 0;
			SPEED += 3;
			break;
		}
	}
	SIZE_SNAKE++;
}
void DrawSnakeAndFood(char str[], char str2)
{
	doimau(242);
	if (check_gate != 1)
	{
		GotoXY(food[FOOD_INDEX].x, food[FOOD_INDEX].y);
		cout << str2;
	}
	doimau(244);
	for (int i = 0; i <= SIZE_SNAKE - 1; i++)
	{
		if ((i == 0 && str[i] != ' ' && count_SIZE == 0) || (i == 0 && str[i] != ' ' && check_outgate == 1))
		{
			GotoXY(snakey[i].x, snakey[i].y);
			cout << (char)1;
			continue;
		}
		GotoXY(snakey[i].x, snakey[i].y);
		if (check_outgate == 1)
			cout << str[i];
		else
		cout << str[i + count_SIZE];
	}
}
void MoveRight()
{
	int n = snakey[0].x;
	int m = snakey[0].y;
	if (n == WIDTH_CONSOLE || !IsValid(n + 1, m))
	{
		ProcessDead();
		return;
	}
	if (n == food[FOOD_INDEX].x && m == food[FOOD_INDEX].y)
		Eat();
	if (!Isvalidgate(n + 1, m))
	{
		ProcessDead();
		return;
	}
	if (check_outgate == 1)
		ProcessOutGate();
	for (int i = SIZE_SNAKE - 1; i > 0; i--)
	{
		snakey[i].x = snakey[i - 1].x;
		snakey[i].y = snakey[i - 1].y;
	}
	snakey[0].x++;
}
void MoveLeft()
{
	int n = snakey[0].x;
	int m = snakey[0].y;
	if (n - 2 == 0 || !IsValid(n - 1, m))
	{
		ProcessDead();
		return;
	}
	if (n == food[FOOD_INDEX].x && m == food[FOOD_INDEX].y)
		Eat();
	if (!Isvalidgate(n - 1, m))
	{
		ProcessDead();
		return;
	}
	if (check_outgate == 1)
		ProcessOutGate();
	for (int i = SIZE_SNAKE - 1; i > 0; i--)
	{
		snakey[i].x = snakey[i - 1].x;
		snakey[i].y = snakey[i - 1].y;
	}
	snakey[0].x--;
}
void MoveDown()
{
	int n = snakey[0].x;
	int m = snakey[0].y;
	if (m + 1 == HEIGH_CONSOLE || !IsValid(n, m + 1))
	{
		ProcessDead();
		return;
	}
	else
	{
		if (n == food[FOOD_INDEX].x && m == food[FOOD_INDEX].y)
			Eat();
		if (!Isvalidgate(n, m + 1))
		{
			ProcessDead();
			return;
		}
		if (check_outgate == 1)
			ProcessOutGate();
		for (int i = SIZE_SNAKE - 1; i > 0; i--)
		{
			snakey[i].x = snakey[i - 1].x;
			snakey[i].y = snakey[i - 1].y;
		}
		snakey[0].y++;
	}
}
void MoveUp()
{
	int n = snakey[0].x;
	int m = snakey[0].y;
	if (m - 1 == 0 || !IsValid(n, m - 1))
	{
		ProcessDead();
		return;
	}
	else
	{
		if (n == food[FOOD_INDEX].x && m == food[FOOD_INDEX].y)
			Eat();
		if (n == gate[2].x && m - 1 == gate[2].y)
		{
			ProcessInGate();
		}
		else if (check_outgate == 1)
		{
			if (SIZE_SNAKE == 1)
			{
				generateGate();
				drawOutGate(gate);
				//check_gate = 0;
				ROUND++;
			}
			ProcessOutGate();
		}
		else if (!Isvalidgate(n, m - 1))
		{
			ProcessDead();
			return;
		}
		for (int i = SIZE_SNAKE - 1; i > 0; i--)
		{
			snakey[i].x = snakey[i - 1].x;
			snakey[i].y = snakey[i - 1].y;
		}
		snakey[0].y--;
	}
}
void SaveGame()
{
	doimau(242);
	GotoXY(50, 10);
	cout << "                              ";
	GotoXY(50, 10);
	cout << "nhap ten file de luu!";
	char* a = (char*)malloc(100); memset(a, '\0', 100);
	GotoXY(50, 11);
	cout << "                              ";
	GotoXY(50, 11);
	cin.getline(a, 100);
	fstream foo("FileName.txt", ios::app);
	fstream fo(a, ios::out);//tao bien mo file de luu
	foo << a << endl;
	foo.close();
	fo << SPEED << endl;
	fo << FOOD_INDEX << endl;
	fo << SIZE_SNAKE << endl;
	fo << count_SIZE << endl;
	fo << CHAR_LOCK << endl;
	fo << MOVING << endl;
	for (int i = 0; i < SIZE_SNAKE; i++)
	{
		fo << snakey[i].x << endl;
		fo << snakey[i].y << endl;
	}
	for (int i = 0; i < MAX_SIZE_FOOD; i++)
	{
		fo << food[i].x << endl;
		fo << food[i].y << endl;
	}
	fo << ROUND << endl;
	for (int i = 0; i <= 5; i++)
	{
		fo << gate[i].x << endl;
		fo << gate[i].y << endl;
	}
	fo << check_gate << endl;
	fo << check_outgate << endl;
	fo.close();
	free(a);
	GotoXY(50, 12);
	cout << "luu thanh cong!";
	int temp4 = toupper(_getch());
}
void LoadGame()
{
	doimau(242);
	GotoXY(50, 10);
	cout << "                              ";
	GotoXY(50, 10);
	cout << "nhap ten file muon mo";
	if (food == NULL)
		food = (POINT*)malloc(MAX_SIZE_FOOD * sizeof(POINT));
	char* a = (char*)malloc(100); memset(a, '\0', 100);
	GotoXY(50, 11);
	cout << "                              ";
	GotoXY(50, 11);
	cin.getline(a, 100);
	fstream fi(a, ios::in);//tao bien mo file de doc
	if (!fi)
	{
		GotoXY(50, 12);
		cout << "file khong ton tai";
		check = 3;// check = 3 de bao la khong load duoc file
		int temp3 = toupper(_getch());
		doimau(255); system("cls");
	}
	else
	{
		fi >> SPEED >> FOOD_INDEX >> SIZE_SNAKE >> count_SIZE >> CHAR_LOCK >> MOVING;
		POINT* snakynew = (POINT*)malloc((SIZE_SNAKE + count_SIZE) * sizeof(POINT));
		if (snakynew != NULL)
		{
			if (snakey != NULL)
				free(snakey);
			snakey = snakynew;
		}
		for (int i = 0; i < SIZE_SNAKE; i++)
			fi >> snakey[i].x >> snakey[i].y;
		for (int i = 0; i < MAX_SIZE_FOOD; i++)
			fi >> food[i].x >> food[i].y;
	}
	fi >> ROUND;
	for (int i = 0; i <= 5; i++)
		fi >> gate[i].x >> gate[i].y;
	fi >> check_gate >> check_outgate;
	fi.close();
	free(a);
}
void menu()
{
	int k;
	muiten.x = 42;
	muiten.y = 20;
	while (1)
	{
		if (check == 1)
		{
			do{
				k = rand() % 14;
			}while(k== 7 && k== 8 && k == 15);
			doimau(240 +k);
			Sleep(150);
			if (check != 1)
				continue;
			char a = 47, b = 92, c = 95, d = 179, e = 219;
			GotoXY(3, 5); cout << "  " << c << c << c << c << c;//dong1
			GotoXY(3, 6); cout << " " << a << " " << e << e << e << e;//dong2 
			GotoXY(3, 7); cout << d << " " << e << e << "  " << e << e << " " << "                                         " << c << c << c << c << c << c << c << c << c;//dong3
			GotoXY(3, 8); cout << d << " " << e << e << c << c << c << e << e << "     " << c << c << "   " << c << c << "        " << c << c << c << "       " << c << c << c << "  " << c << "     " << d << "        " << b;//dong4
			GotoXY(3, 9); cout << d << " " << e << e << "   " << b << "     " << d << "  " << b << " " << d << "  " << b << "      " << a << "   " << b << "      " << d << "  " << b << d << " " << b << "    " << d << " " << e << e << e << e << e << e << e << e;//dong5
			GotoXY(3, 10); cout << " " << b << " " << e << e << e << e << e << b << "    " << d << " " << e << e << b << d << " " << e << e << "     " << a << " " << e << e << e << e << b << "     " << d << " " << e << e << d << e << e << d << "   " << d << " " << e << e << c << c;//dong6
			GotoXY(3, 11); cout << "  " << b << c << e << e << e << e << e << "    " << d << " " << e << e << e << b << " " << e << e << "    " << a << " " << e << e << a << b << e << e << b << "    " << d << " " << e << e << d << e << e << d << "   " << d << " " << e << e << "  " << b;//dong7
			GotoXY(3, 12); cout << "  " << c << "   " << b << e << e << "    " << d << " " << e << e << e << e << b << e << e << "   " << a << " " << e << e << a << c << c << b << e << e << b << "   " << d << " " << e << e << e << e << d << "    " << d << " " << e << e << e << e << e;//dong8
			GotoXY(3, 13); cout << a << "  " << b << c << c << d << e << e << "    " << d << " " << e << e << b << e << e << e << e << "  " << d << " " << e << e << e << e << e << e << e << e << e << e << " " << "  " << d << " " << e << e << e << e << b << "    " << d << " " << e << e << c << c << c << c << c;//dong9
			GotoXY(3, 14); cout << d << " " << e << e << e << e << e << e << e << "    " << d << " " << e << e << " " << b << e << e << e << "  " << d << " " << e << e << "      " << e << e << " " << "  " << d << " " << e << e << b << e << e << b << "   " << d << " " << e << e << "     " << b;//dong1<<e
			GotoXY(3, 15); cout << " " << b << e << e << e << e << e << e << "      " << b << e << e << "  " << b << e << e << "   " << b << e << e << "      " << e << e << " " << "   " << b << e << e << " " << b << e << e << b << "   " << b << e << e << e << e << e << e << e << e;//dong11

			GotoXY(70, 7); cout << c << c << c << c << c << c << "      " << c << c << c << c << c << c << "      " << c << "      " << c << c << "   " << c << c << c << c << c << c << c << c << c;/*1*/
			GotoXY(69, 8); cout << a << "      " << b << "    " << a << "      " << b << "    " << a << " " << b << "    " << a << "  " << b << "  " << d << "        " << b;/*2*/
			GotoXY(68, 9); cout << d << "  " << e << e << e << e << e << e << b << "  " << d << "  " << e << e << e << e << e << e << b << "  " << d << " " << e << e << b << "  " << a << " " << e << e << e << "  " << d << " " << e << e << e << e << e << e << e << e;/*3*/
			GotoXY(68, 10); cout << d << " " << e << e << "  " << c << b << e << e << "  " << d << " " << e << e << c << c << d << " " << e << e << "  " << d << " " << e << e << e << b << a << " " << a << e << e << e << "  " << d << " " << e << e << c << c;/*4*/
			GotoXY(68, 11); cout << d << " " << e << e << " " << d << "   " << b << "  " << d << " " << e << e << "    " << e << e << "  " << d << " " << e << e << e << e << b << a << e << e << e << e << "  " << d << " " << e << e << "  " << b;/*5*/
			GotoXY(68, 12); cout << d << " " << e << e << " " << b << e << e << e << e << "  " << d << " " << e << e << e << e << e << e << e << e << "  " << d << " " << e << e << b << e << e << e << e << e << e << e << "  " << d << " " << e << e << e << e << e;/*6*/
			GotoXY(68, 13); cout << d << " " << e << e << c << c << d << " " << e << e << "  " << d << " " << e << e << "  " << d << " " << e << e << "  " << d << " " << e << e << " " << b << e << e << e << d << e << e << "  " << d << " " << e << e << c << c << c << c << c;/*7*/
			GotoXY(69, 14); cout << b << e << e << e << "   " << e << e << "  " << d << " " << e << e << "  " << d << " " << e << e << "  " << d << " " << e << e << "  " << b << e << " " << d << e << e << "  " << d << " " << e << e << "     " << b;/*8*/
			GotoXY(70, 15); cout << b << e << e << e << e << e << e << e << "   " << b << e << e << "   " << b << e << e << "   " << b << e << e << "     " << b << e << e << "   " << b << e << e << e << e << e << e << e << e;/*9*/
			for (int i = 20; i < 26; i = i + 2)
			{
				doimau(240 + 4);
				GotoXY(42, i);
				cout << "    ";
			}
			GotoXY(muiten.x, muiten.y);
			doimau(240 +12);
			cout << "-->";
			if (muiten.y == 20)
				doimau(250);
			else
				doimau(252);
			GotoXY(55, 20);
			cout << "__START GAME__";
			if (muiten.y == 22)
				doimau(250);
			else
				doimau(252);
			GotoXY(55, 22);
			cout << "__OPEN FILE__";
			if (muiten.y == 24)
				doimau(250);
			else
				doimau(252);
			GotoXY(55, 24);
			cout << "__EXIT GAME__";
		}
	}
	return;
}
void ThreadFunc()
{
	while (1)
	{
		if (STATE == 1)
		{
			DrawSnakeAndFood(q, ' ');
			switch (MOVING)
			{
			case 'A':
			{
				MoveLeft();
				break;
			}
			case 'D':
			{
				MoveRight();
				break;
			}
			case 'W':
			{
				MoveUp();
				break;
			}
			case 'S':
			{
				MoveDown();
				break;
			}
			}
			doimau(240 +7);
			DrawSnakeAndFood(p, p[SIZE_SNAKE]);
			GotoXY(40, HEIGH_CONSOLE + 2);
			if (check_gate != 0)
				cout << 0;
			else if(ROUND == 7 && STATE == 1)
			{
				cout << 3 - FOOD_INDEX;
			}
			else if (STATE == 1)
				cout << MAX_SIZE_FOOD - FOOD_INDEX;
			GotoXY(40, HEIGH_CONSOLE + 4);
			cout << ROUND;
			Sleep(1000 / SPEED);//toc do con ran khi choi
		}
		else if (STATE == 2)
		{
			GotoXY(snakey[0].x, snakey[0].y);
			doimau(252);
			cout << p[0];
			Sleep(150);
			if (z < SIZE_SNAKE)
			{
				doimau(240 +10);
				GotoXY(snakey[z].x, snakey[z].y);
				cout << 'X';
				z++;
			}
			GotoXY(snakey[0].x, snakey[0].y);
			doimau(240 +79);
			cout <<(char)1;
			Sleep(150);
		}
	}
}
int main()
{
	doimau(240);
	system("cls");
	int temp{};
	int temp2;
	FixConsoleWindow();
	HideCursor();
	STATE = 0;
	thread t2(menu);
	HANDLE handle_t2 = t2.native_handle();
	thread t1(ThreadFunc);
	HANDLE handle_t1 = t1.native_handle();
	while (1)
	{
		if (check == 100)//check = 100 de chay con ran
		{
			temp = toupper(_getch());
			if (STATE == 2)
				STATE = 0;
		}
		if (check == 1)//check = 1 chay menu;
		{
			while (1)
			{
				temp2 = toupper(_getch());
				if (temp2 == 'S' && muiten.y < 24)
					muiten.y += 2;
				else if (temp2 == 'S' && muiten.y == 24)
					muiten.y = 20;
				else if (temp2 == 'W' && muiten.y > 20)
					muiten.y -= 2;
				else if (temp2 == 'W' && muiten.y == 20)
					muiten.y = 24;
				else if (temp2 == 13 && muiten.y == 20)
				{
					check = 0;
					doimau(255); system("cls");
					break;
				}
				else if (temp2 == 13 && muiten.y == 24)
				{
					ExitGame(handle_t2);
					ExitGame(handle_t1);
					t2.join();
					t1.join();
					return 0;
				}
				else if (temp2 == 13 && muiten.y == 22)
				{
					check = 0;
					doimau(255); system("cls");
					LoadGame();
					if (check != 3)
						check = 2;
					temp = 0;
					break;
				}
			}
		}
		if (check == 2)//xu li sau ki load file
		{
			doimau(255); system("cls");
			DrawBoard(0, 0, WIDTH_CONSOLE, HEIGH_CONSOLE);
			STATE = 1;
			PauseGame(handle_t1);
			DrawSnakeAndFood(p, p[SIZE_SNAKE]);
			if (check_outgate == 1)
				drawOutGate(gate);
			else if (check_gate == 1)
				drawGate(gate);
			check = 100;
			temp = toupper(_getch());
		}
		if (check == 0)//xu li khi tat menu
		{
			StartGame();
			check = 100;
		}
		if (STATE == 1)
			if (temp == 'P')
				PauseGame(handle_t1);
			else if (temp == 27)
			{
				ExitGame(handle_t2);
				ExitGame(handle_t1);
				t1.join();
				t2.join();
				doimau(255); system("cls");
				return 0;
			}
			else if (temp == 'L')
			{
				PauseGame(handle_t1);
				SaveGame();
				doimau(255); system("cls");
				temp = ' ';
				check = 1;
				continue;
			}
			else if (temp == 'T')
			{
				STATE = 0;
				doimau(255); system("cls");
				LoadGame();
				if (check != 3)
					check = 2;
			}
			else
			{
				ResumeThread(handle_t1);
				if ((temp != CHAR_LOCK) && (temp == 'D' || temp == 'A' || temp == 'W' || temp == 'S'))
				{
					if (temp == 'D')
						CHAR_LOCK = 'A';
					else if (temp == 'W')
						CHAR_LOCK = 'S';
					else if (temp == 'S')
						CHAR_LOCK = 'W';
					else
						CHAR_LOCK = 'D';
					MOVING = temp;
				}
			}
		else
		{
				Sleep(200);
				doimau(255); system("cls");
				check = 1;
		}
	}
}