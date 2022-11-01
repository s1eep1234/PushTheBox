#include <iostream>
#include<graphics.h>
#include<stdlib.h>
#include<string>
#include<conio.h>
#include<Windows.h>

#define BOX_SIZE 60  
#define LINE 9
#define COLUMN 12

#define KEY_UP  "w"  //定义键盘按键
#define KEY_DOWN  "s"
#define KEY_LEFT  "a"
#define KEY_RIGHT  "d"
#define KEY_QUIT "q"

#define isValid(Pos) Pos.x>0 && Pos.x<LINE && Pos.y>0 && Pos.y<COLUMN //判断是否越界


typedef struct {
	int x;
	int y;
}Position;

enum BoxType
{
	WALL,
	FLOOR,
	BOX_DES,
	MAN,
	BOX,
	HIT,
	ALL
};

enum DIRECTION
{
	UP,
	DOWN,
	LEFT,
	RIGHT
};

IMAGE images[ALL];  //全局地图
Position man;       //小人位置

int BOX_NUM = 4;    //箱子数
int HIT_NUM = 0;	//推到目的的的箱子数

bool quit = false;  //是否结束

int map[LINE][COLUMN] = {
	{0,0,0,0,0,0,0,0,0,0,0,0},
	{0,1,0,1,1,1,1,1,1,1,0,0},
	{0,1,4,1,0,2,1,0,2,1,0,0},
	{0,1,0,1,0,1,0,0,1,1,1,0},
	{0,1,0,2,0,1,1,4,1,1,1,0},
	{0,1,1,1,0,3,1,1,1,4,1,0},
	{0,1,2,1,1,4,1,1,1,1,1,0},
	{0,1,0,0,1,0,1,1,0,0,1,0},
	{0,0,0,0,0,0,0,0,0,0,0,0}
};

void update(Position pos, enum BoxType boxType) {  //更新地图
	map[pos.x][pos.y] = boxType;
	putimage(100 + pos.y * BOX_SIZE, 100 + pos.x * BOX_SIZE, &images[boxType]);
}

void checkOver() {  //检测是否结束
	if (HIT_NUM == BOX_NUM) {
		::MessageBox(NULL, TEXT("success"), TEXT("game is over!!!"), NULL);
		quit = true;
		//printf("OKOK");
	}
}

void gameControl(enum DIRECTION direction) { //热键控制

	Position manNextPos = man;
	Position manNextNextPos = man;

	switch (direction)
	{
	case UP:
		manNextPos.x--;
		manNextNextPos.x = manNextNextPos.x - 2;
		break;
	case DOWN:
		manNextPos.x++;
		manNextNextPos.x = manNextNextPos.x + 2;
		break;
	case LEFT:
		manNextPos.y--;
		manNextNextPos.y = manNextNextPos.y - 2;
		break;
	case RIGHT:
		manNextPos.y++;
		manNextNextPos.y = manNextNextPos.y + 2;
		break;
	default:
		break;
	}

	if (isValid(manNextPos) && map[manNextPos.x][manNextPos.y] == FLOOR) {
		update(man, FLOOR);
		update(manNextPos, MAN);
		man = manNextPos;
	}
	else if (isValid(manNextPos) && map[manNextPos.x][manNextPos.y] == BOX) {
		if (isValid(manNextNextPos)) {
			if (map[manNextNextPos.x][manNextNextPos.y] == FLOOR) {
				update(man, FLOOR);
				update(manNextPos, MAN);
				update(manNextNextPos, BOX);
				man = manNextPos;
			}
			else if (map[manNextNextPos.x][manNextNextPos.y] == BOX_DES) {
				update(man, FLOOR);
				update(manNextPos, MAN);
				update(manNextNextPos, HIT);
				man = manNextPos;
				HIT_NUM++;
			}
		}
	}
};

int main()
{
	initgraph(960, 768);//初始化地图大小
	IMAGE bg_image;

	loadimage(&bg_image, TEXT("blackground.bmp"), 960, 768, true); //设置地图背景
	putimage(0, 0, &bg_image);

	loadimage(&images[WALL], TEXT("wall_right.bmp"), BOX_SIZE, BOX_SIZE, true); //加载其他图片
	loadimage(&images[FLOOR], TEXT("floor.bmp"), BOX_SIZE, BOX_SIZE, true);
	loadimage(&images[BOX_DES], TEXT("des.bmp"), BOX_SIZE, BOX_SIZE, true);
	loadimage(&images[MAN], TEXT("man.bmp"), BOX_SIZE, BOX_SIZE, true);
	loadimage(&images[BOX], TEXT("box.bmp"), BOX_SIZE, BOX_SIZE, true);
	loadimage(&images[HIT], TEXT("box.bmp"), BOX_SIZE, BOX_SIZE, true);


	//设置地图背景
	for (int i = 0; i < LINE; i++) {
		for (int j = 0; j < COLUMN; j++) {
			if (map[i][j] == MAN) {
				man.x = i;
				man.y = j;
			}
			putimage(100 + j * BOX_SIZE, 100 + i * BOX_SIZE, &images[(map[i][j])]);
		}
	}


	//设置键盘
	do {
		checkOver();
		if (_kbhit()) {
			char ch = _getch();
			if (ch == *KEY_UP) {
				gameControl(UP);
			}
			else if (ch == *KEY_DOWN) {
				gameControl(DOWN);
			}
			else if (ch == *KEY_RIGHT) {
				gameControl(RIGHT);
			}
			else if (ch == *KEY_LEFT) {
				gameControl(LEFT);
			}
			else if (ch == *KEY_QUIT) {
				quit = true;
			}
		}
	} while (!quit);

	system("pause");
	return 0;
}
