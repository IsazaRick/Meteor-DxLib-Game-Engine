#include "meteor.h"
#include "DxLib.h"
#include <math.h>
#include <time.h>


void logo::DrawAniLogo(int& index, int& framecount, int& scenestate, int& frame, int lenght, int ammountOfFrames, int animationSpeed, float exrate) {
	if (!CheckSoundMem(logosound)) PlaySoundMem(logosound, DX_PLAYTYPE_BACK);
	framecount++;
	if (framecount >= animationSpeed) {
		index++;
		if (index >= ammountOfFrames) index = 0;
		framecount = 0;
	}
	DrawRotaGraph(WIN_WIDTH / 2, WIN_HEIGHT / 2, exrate, 0, logo[index], true);
	if (frame > lenght * 2) {
		scenestate++;
		frame = 0;
	} 
}

int heldFlag(int key)
{
	int VALUE;

	if (key) VALUE = TRUE;
	else VALUE = FALSE;

	return VALUE;
}

int buttonFlag(int key, int oldkey) {
	int flag = FALSE;

	if (key && !oldkey) flag = TRUE;
	else if (key == oldkey) flag = FALSE;

	return flag;
}

// フェイドイン・アウト
void FadeFromBlack(int& pal, int duration) {
	int faderate = 255 / duration;
	pal += faderate;
	if (pal < 0) pal = 0;
	if (pal >= 255) pal = 255;
	SetDrawBlendMode(DX_BLENDMODE_ALPHA, pal);
}

void FadeToBlack(int& pal, int duration)
{
	int faderate = 255 / duration;
	pal -= faderate;
	if (pal >= 255) pal = 255; 
	if (pal < 0) pal = 0;
	SetDrawBlendMode(DX_BLENDMODE_ALPHA, pal);
}


// ドロー

void DrawStatic(int image, int x, int y, int camx, int camy) {
	DrawGraph(x - camx, y - camy, image, true);
}

void DrawRotaStatic(int x, int y, int camx, int camy, int image, int exrate) {
	DrawRotaGraph(x - camx, y - camy, exrate, 0, image, true);
}

void DrawReferencePoint(int x, int y, int camx, int camy, int size) {
	DrawCircle(x - camx, y - camy, size, GetColor(255, 100, 200), true);
}

void DrawReferenceBox(int x, int y, int camx, int camy, int sizeX, int sizeY) {
	int x1 = x - camx, y1 = y - camy;
	DrawBox(x1, y1, x1 + sizeX, y1 + sizeY, GetColor(255, 255, 255), true);
}

void DrawAnimation(int x, int y, int image[], int& index, int& framecount, int framerate, int firstframe, int ammountofframes, int loopflag, int camX, int camY) {
	framecount++;
	if (index < firstframe) index = firstframe;
	if (index > firstframe + ammountofframes && loopflag) index = firstframe;
	if (framecount >= framerate) {
		framecount = 0;
		index++;
		if (loopflag && index >= firstframe + ammountofframes) index = firstframe;
		else if (!loopflag && index >= firstframe + ammountofframes) index = firstframe + ammountofframes - 1;
	}
	DrawRotaGraph(x - camX, y - camY, 1, 0, image[index], true);
}

void DrawRotaAnimation(int x, int y, int image[], int& index, int& framecount, int framerate, int firstframe, int ammountofframes, int loopflag, int camX, int camY, float angle) {
	framecount++;
	if (index < firstframe) index = firstframe;
	if (index > firstframe + ammountofframes) index = firstframe;
	if (framecount >= framerate) {
		framecount = 0;
		index++;
		if (loopflag) if (index >= firstframe + ammountofframes) index = firstframe;
		else if (!loopflag) if (index >= firstframe + ammountofframes) index = firstframe + ammountofframes;
	}
	DrawRotaGraph(x - camX, y - camY, 1, angle, image[index], true);
}



// 当たり判定

int checkHit(int playerX, int playerY, int objX, int objY, int playerSize, int objSize) {
	int flag = FALSE;
	float a = pow(playerX - objX, 2);
	if (a < 0) a = -a;
	float b = pow(playerY - objY, 2);
	if (b < 0) b = -b;
	float c = sqrt(a + b);
	if (c <= playerSize + objSize) flag = TRUE;
	else flag = FALSE;

	return flag;
}

int checkDist(int playerX, int playerY, int objX, int objY) {
	float a = pow(playerX - objX, 2);
	if (a < 0) a = -a;
	float b = pow(playerY - objY, 2);
	if (b < 0) b = -b;
	float c = sqrt(a + b);
	round(c);

	return c;
}

void circleHitBox(int& playerX, int& playerY, int objX, int objY, int playerSize, int objSize) {
	int dist = checkDist(playerX, playerY, objX, objY);
	if (dist < playerSize + objSize) {
		int disp = playerSize + objSize - dist;
		if (objX >= playerX) playerX -= disp;
		if (objX <= playerX) playerX += disp;
		if (objY >= playerY) playerY -= disp;
		if (objY <= playerY) playerY += disp;
		// NOT PERFECT YET
	}
}

void squareHitBox(int x, int y, int sizeX, int sizeY, int& playerX, int& playerY, int playerSize, int prevX, int prevY) {
	int x2 = x + sizeX;
	int y2 = y + sizeY;

	int boxcenterX = x + sizeX / 2;
	int boxcenterY = y + sizeY / 2;

	int up = playerY - playerSize;
	int down = playerY + playerSize;
	int left = playerX - playerSize;
	int right = playerX + playerSize;

	int horflag = FALSE;
	int verflag = FALSE;

	if (right >= x && left <= x2) horflag = TRUE;
	if (down >= y && up <= y2) verflag = TRUE;

	// Prev
	int prevup = prevY - playerSize;
	int prevdown = prevY + playerSize;
	int prevleft = prevX - playerSize;
	int prevright = prevX + playerSize;

	int prevhorflag = FALSE;
	int prevverflag = FALSE;

	if (prevright >= x && prevleft <= x2) prevhorflag = TRUE;
	if (prevdown >= y && prevup <= y2) prevverflag = TRUE;

	// Calculations
	if (horflag && verflag) {
		if (prevhorflag) playerY = prevY;
		else if (prevverflag) playerX = prevX;
	}
}

int checkSquareHit(int x, int y, int sizeX, int sizeY, int playerX, int playerY, int playerSize) {
	int x2 = x + sizeX;
	int y2 = y + sizeY;

	int up = playerY - playerSize;
	int down = playerY + playerSize;
	int left = playerX - playerSize;
	int right = playerX + playerSize;

	int horflag = FALSE;
	int verflag = FALSE;

	if (right >= x && left <= x2) horflag = TRUE;
	if (down >= y && up <= y2) verflag = TRUE;

	int flag = FALSE;
	if (horflag && verflag) flag = TRUE;
	return flag;
}


// 使いやすくするため

void IncreaseIntValue(int& variable, int increaseAmmount, int MaxAmmount) {
	variable += increaseAmmount;
	if (variable >= MaxAmmount) variable = MaxAmmount;
}

void DecreaseIntValue(int& variable, int decreaseAmmount, int MinAmmount) {
	variable -= decreaseAmmount;
	if (variable <= MinAmmount) variable = MinAmmount;
}

void IncreaseFloatValue(float& variable, float increaseAmmount, float MaxAmmount) {
	variable += increaseAmmount;
	if (variable >= MaxAmmount) variable = MaxAmmount;
}

void DecreaseFloatValue(float& variable, float decreaseAmmount, float MinAmmount) {
	variable -= decreaseAmmount;
	if (variable <= MinAmmount) variable = MinAmmount;
}



// スクリプト

void SimpleFollowScript(int& x, int& y, int chaseX, int chaseY, int speed) {
	if (x < chaseX) x += speed;
	else if (x > chaseX) x -= speed;
	if (y < chaseY) y += speed;
	else if (y > chaseY) y -= speed;
}
