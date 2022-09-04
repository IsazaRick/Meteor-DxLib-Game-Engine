#include "DxLib.h"
#include "meteor.h"
#include<math.h>
#include<time.h>

const char TITLE[] = "M3T30R ENGINE VERSION 2.14.4";

int WINAPI WinMain(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE hPrevInstance, _In_ LPSTR lpCmdLine, _In_ int nCmdShow) {
	ChangeWindowMode(TRUE); // ウィンドウモードに設定
	SetWindowSizeChangeEnableFlag(TRUE, TRUE);
	SetMainWindowText(TITLE);
	SetGraphMode(WIN_WIDTH, WIN_HEIGHT, 32); // 画面サイズを設定(解像度との比率で設定)

	SetWindowSizeExtendRate(1.0);

	SetBackgroundColor(0x00, 0x00, 0x00);	// 画面の背景色を設定する
	if (DxLib_Init() == -1) { return -1; }
	SetDrawScreen(DX_SCREEN_BACK);
	// ESSENTIAL VARIABLES
	int frame = 0, pal = 0;
	int camX = 0, camY = 0;
	int alpha1 = 255;
	enum DIRECTIONS {
		UP,
		DOWN,
		LEFT,
		RIGHT,
		UPLEFT,
		UPRIGHT,
		DOWNLEFT,
		DOWNRIGHT,
	};
	int fadefromblackflag = FALSE, fadetoblackflag = FALSE;
	// AJKGames Variables
	logo AjikuGames;
	AjikuGames.logosound = LoadSoundMem("ajikugamessound.wav");
	LoadDivGraph("ajikugameslogo.png", 4, 2, 2, 640, 256, AjikuGames.logo, true);
	logo M3T30R_ENGINE;
	M3T30R_ENGINE.spX = (WIN_WIDTH / 2) + 400;
	M3T30R_ENGINE.spY = (WIN_HEIGHT / 2) - 400;
	M3T30R_ENGINE.logo[0] = LoadGraph("M3T30R.png");
	M3T30R_ENGINE.sublogo[0] = LoadGraph("GLASS.png");

	// ゲームループで使う変数の宣言
	enum scenstates {
		ajikugamesLOGO,
		meteorengineLOGO,
		gameplay,
	};
	int scenestate = ajikugamesLOGO; // DEBUG HERE

	char key[256] = { 0 };
	char prevkey[256] = { 0 };

	// ゲームループ
	while (true) {
		GetHitKeyStateAll(key);
		ClearDrawScreen();

		//---------  GLOBAL MATH & LOGIC / KEYBINDS  ----------//


		//---------  SCENESTATE SWITCH  ----------//

		switch (scenestate) {

		case ajikugamesLOGO:

			if (frame < 170) FadeFromBlack(pal, 60);
			if (frame >= 170) FadeToBlack(pal, 60);

			AjikuGames.DrawAniLogo(AjikuGames.index, AjikuGames.framecount, scenestate, frame, 110, 4, 5, 1);

			break; // Breaks LOGO 1



		case meteorengineLOGO:

			FadeFromBlack(pal, 10);

			IncreaseFloatValue(M3T30R_ENGINE.exrate, 1, 10);
			DecreaseIntValue(M3T30R_ENGINE.spX, 40, WIN_WIDTH / 2);
			IncreaseIntValue(M3T30R_ENGINE.spY, 40, WIN_HEIGHT / 2);

			DrawRotaStatic(M3T30R_ENGINE.spX, M3T30R_ENGINE.spY, camX, camY, M3T30R_ENGINE.logo[0], M3T30R_ENGINE.exrate);

			// Make these 4 a function. DrawRotaBlendStatic
			SetDrawBlendMode(DX_BLENDMODE_ALPHA, alpha1); // 1
			if (frame >= 10) DrawRotaStatic(WIN_WIDTH / 2, WIN_HEIGHT / 2, camX, camY, M3T30R_ENGINE.sublogo[0], 9); // 2
			DecreaseIntValue(alpha1, 5, 0); // 3
			SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 255); // 4
			// Shake the screen THEN make it disappear.
			
			
			break; // Breaks LOGO 2



		case gameplay:

			break; // This breaks gameplay
		}
		
		// DEBUG
		DrawFormatString(0, 0, GetColor(255, 255, 255), "%04hx	%04hx	%04hx", frame, alpha1, scenestate);

		//---------  ESSENTIALS  ---------//
		GetHitKeyStateAll(prevkey);
		frame++;
		ScreenFlip();
		WaitTimer(20); // 20 miliseconds = 60 FPS
		if (ProcessMessage() == -1) {
			break;
		}

		// ---------------------------------------- PRESS B TO BREAK. DELETE THIS AT THE END. ---------------------------------------- //
		if (CheckHitKey(KEY_INPUT_B) == 1) {
			break;
		}
	}
	DxLib_End(); // If this gives an error, restart Visual Studio.
	return 0;
}
