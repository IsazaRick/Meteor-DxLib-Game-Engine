#pragma once

const int WIN_WIDTH = 42*32; // 42 32p Tiles
const int WIN_HEIGHT = 26*32; // 26 32p Tiles

class logo
{
public:

	int logo[20];
	int sublogo[20];
	int index = 0;
	int framecount = 0;
	int logosound;
	int spX = 0, spY = 0;
	float exrate = 0;

	void DrawAniLogo(int& index, int& framecount, int& scenestate, int& frame, int lenght, int ammountOfFrames, int animationSpeed, float exrate);
};

class particle {
public:
	bool isLive = 0;

	int image;
	int liveDuration = 0;
};

// ボタン
int heldFlag(int key);
int buttonFlag(int key, int oldkey);

// フェイドイン・アウト
void FadeFromBlack(int& pal, int duration);
void FadeToBlack(int& pal, int duration);

// ドロー
void DrawStatic(int image, int x, int y, int camx, int camy);
void DrawRotaStatic(int x, int y, int camx, int camy, int image, int exrate);
void DrawReferencePoint(int x, int y, int camx, int camy, int size);
void DrawReferenceBox(int x, int y, int camx, int camy, int sizeX, int sizeY);
void DrawAnimation(int x, int y, int image[], int& index, int& framecount, int framerate, int firstframe, int ammountofframes, int loopflag, int camX, int camY);
void DrawRotaAnimation(int x, int y, int image[], int& index, int& framecount, int framerate, int firstframe, int ammountofframes, int loopflag, int camX, int camY, float angle);


// 当たり判定
int checkHit(int playerX, int playerY, int objX, int objY, int playerSize, int objSize);
int checkSquareHit(int x, int y, int sizeX, int sizeY, int playerX, int playerY, int playerSize);
int checkDist(int playerX, int playerY, int objX, int objY);
void circleHitBox(int& playerX, int& playerY, int objX, int objY, int playerSize, int objSize);
void squareHitBox(int x, int y, int sizeX, int sizeY, int& playerX, int& playerY, int playerSize, int prevX, int prevY);

// 使いやすくするため
void IncreaseIntValue(int& variable, int increaseAmmount, int MaxAmmount);
void DecreaseIntValue(int& variable, int decreaseAmmount, int MinAmmount);

void IncreaseFloatValue(float& variable, float increaseAmmount, float MaxAmmount);
void DecreaseFloatValue(float& variable, float decreaseAmmount, float MinAmmount);

// エフェクト


// スクリプト
void SimpleFollowScript(int& x, int& y, int chaseX, int chaseY, int speed);
