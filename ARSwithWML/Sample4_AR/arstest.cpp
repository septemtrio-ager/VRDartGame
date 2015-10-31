#pragma once

#include <windows.h>
#include <tchar.h>
#define D3D_DEBUG_INFO
#include <stdlib.h>
#include <math.h>
#include <d3dx9.h>
#include <XAudio2.h>
#include <vector>

#include "../include/WindowManager.h"
#include "../include/ars.h"
#include "arstest.h"

#include <iostream>
using namespace std;

//-----------------------------------------------------------------------------
// Name: WinMain()
// Desc: The application's entry point
//-----------------------------------------------------------------------------

// new design branch

void subtract_maskf(Texture* result, Texture* bg, Texture* src, DWORD border);
void bg_subtract(Texture* result, Texture* background, Texture* src, DWORD border);

// Aボタンが押された時などで状況をリセットするときに呼び出す
void reset(int *point, int *totalPoint, int *threwCount, int *lastPoint, int *lastTotalPoint, int *lastThrewCount, int *gameoverCount, Dart *dart[]);
// Aボタンが押された時に表示をリセットするときに呼び出す
void resetDisplay(int *point, int *totalPoint, int *threwCount, int *lastPoint, int *lastTotalPoint, int *lastThrewCount, ARSG *g, Dart *dart[], Texture *totalPointArray[], Texture *pointArray[], Texture *threwNumberArray[]);

// ウィンドウサイズ
const unsigned int sizex = 640; 
const unsigned int sizey = 480;

// ダーツがスタートする位置
const float START_X_POINT = 6.0f;
const float START_Y_POINT = 3.0f;
const float START_Z_POINT = 2.0f;

// ダーツボードの場所
const float DARTBOARD_X_POINT = -6.5f;
const float DARTBOARD_Y_POINT = 0.0f;
const float DARTBOARD_Z_POINT = 0.0f;

// プレイできる回数
const int NUMBER_OF_PLAYS = 3;

// ダーツとボードの当たり判定を開始するX軸座標
const float START_JUDGEMENT_X_POINT = -4.0f;

// ダーツのスケール
const float DART_SCALE = 2.3f;
// ダーツボードのスケール
const float DARTBOARD_SCALE = 0.8f;
// 次のダーツがセットされるまでのダーツの回転量
const float DART_ROTATION = PI;// 半回転したら次のダーツをセットするようにする

// ダーツボードの回転量
const float DARTBOARD_ROTATION = 0.05f;

 

UINT MainLoop(WindowManager *winmgr)
{
	ShowDebugWindow();

	//for debug(1/2)
	//Window window2;
	//winmgr->RegisterWindow(&window2);

	//ARSG arsgd(window2.hWnd, sizex, sizey, true);
	//Texture debug(&arsgd, sizex, sizey);
	//debug.SetDrawMode(true);

	float xDart = 0, yDart = 0, zDart = 0;
	
	int point = 0;
	int totalPoint = 0;
	int threwCount = 0;

	int lastPoint = 0;
	int lastTotalPoint = 0;
	int lastThrewCount = 0;

	int gameoverCount = 0;
	
	Window window;
	winmgr->RegisterWindow(&window);

	ARSG g(window.hWnd, sizex, sizey, true);
	g.SetBackgroundColor(255,0,0,0);

	Light light(&g);
	light.SetLightIntensity(5.0f);
	g.Register(&light);

	// 背景画像を設定する
	Texture backgroundImage(&g, L"../../../material/background.jpg");
	backgroundImage.SetDrawMode(TRUE);
	g.Register(&backgroundImage);

	// ウィンドウに表示される映像
	Texture mainScreen(&g, sizex, sizey);
	mainScreen.SetDrawMode(TRUE);
	g.Register(&mainScreen);
	
	ARSD d;
	d.Init();
	d.AttachCam(0);
	d.StartGraph();
	
	// 手とダーツが接触している領域を格納する
	Texture hitArea_Hand_and_Dart(&g,sizex,sizey);
	// ダーツとボードが接触している領域を格納する
	Texture *hitAreaArray[10];
	for (int i = 0; i < 10; i++) {
		hitAreaArray[i] = new Texture(&g, sizex, sizey);
		hitAreaArray[i]->SetDrawMode(TRUE);
	}
	// 流れてくるフレームを一時的に保存する
	Texture stored (&g,sizex,sizey);
	// 身体映像を切り抜く際に背景画像として利用する
	Texture source (&g,sizex,sizey);
	source.SetDrawMode(TRUE);
	
	// ダーツをNUMBER_OF_PLAYS分だけ用意する
	Dart *dart[NUMBER_OF_PLAYS];
	for (int i = 0; i < NUMBER_OF_PLAYS; i++) {
		dart[i] = new Dart(&g, L"../../../material/dart.x");
		dart[i]->SetScale(DART_SCALE, DART_SCALE, DART_SCALE);
		dart[i]->SetPosition(START_X_POINT, START_Y_POINT, START_Z_POINT);
	}
	
	// ダーツボード
	DartBoard *dartBoardPoint[10];
	dartBoardPoint[0] = new DartBoard(&g, L"../../../material/pointzone/0point.x");
	dartBoardPoint[1] = new DartBoard(&g, L"../../../material/pointzone/1point.x");
	dartBoardPoint[2] = new DartBoard(&g, L"../../../material/pointzone/2point.x");
	dartBoardPoint[3] = new DartBoard(&g, L"../../../material/pointzone/3point.x");
	dartBoardPoint[4] = new DartBoard(&g, L"../../../material/pointzone/4point.x");
	dartBoardPoint[5] = new DartBoard(&g, L"../../../material/pointzone/5point.x");
	dartBoardPoint[6] = new DartBoard(&g, L"../../../material/pointzone/6point.x");
	dartBoardPoint[7] = new DartBoard(&g, L"../../../material/pointzone/7point.x");
	dartBoardPoint[8] = new DartBoard(&g, L"../../../material/pointzone/8point.x");
	dartBoardPoint[9] = new DartBoard(&g, L"../../../material/pointzone/9point.x");
	for (int i = 0; i < 10; i++) {
		dartBoardPoint[i]->SetScale(0.8f, 0.8f, 0.8f);
		dartBoardPoint[i]->SetPosition(-6.5f, 0.0f, 0.0f);
		g.Register(dartBoardPoint[i]);
	}

	// 点数表示の画像を読み込む
	Texture *pointArray[10];
	pointArray[0] = new Texture(&g, L"../../../material/point/0.png");
	pointArray[1] = new Texture(&g, L"../../../material/point/1.png");
	pointArray[2] = new Texture(&g, L"../../../material/point/2.png");
	pointArray[3] = new Texture(&g, L"../../../material/point/3.png");
	pointArray[4] = new Texture(&g, L"../../../material/point/4.png");
	pointArray[5] = new Texture(&g, L"../../../material/point/5.png");
	pointArray[6] = new Texture(&g, L"../../../material/point/6.png");
	pointArray[7] = new Texture(&g, L"../../../material/point/7.png");
	pointArray[8] = new Texture(&g, L"../../../material/point/8.png");
	pointArray[9] = new Texture(&g, L"../../../material/point/9.png");
	for (int i = 0; i < 10; i++) {
		pointArray[i]->SetDrawMode(TRUE);
	}
	g.Register(pointArray[0]);

	// 合計得点表示の画像を読み込む
	Texture *totalPointArray[28];
	totalPointArray[0] = new Texture(&g, L"../../../material/totalpoint/0.png");
	totalPointArray[1] = new Texture(&g, L"../../../material/totalpoint/1.png");
	totalPointArray[2] = new Texture(&g, L"../../../material/totalpoint/2.png");
	totalPointArray[3] = new Texture(&g, L"../../../material/totalpoint/3.png");
	totalPointArray[4] = new Texture(&g, L"../../../material/totalpoint/4.png");
	totalPointArray[5] = new Texture(&g, L"../../../material/totalpoint/5.png");
	totalPointArray[6] = new Texture(&g, L"../../../material/totalpoint/6.png");
	totalPointArray[7] = new Texture(&g, L"../../../material/totalpoint/7.png");
	totalPointArray[8] = new Texture(&g, L"../../../material/totalpoint/8.png");
	totalPointArray[9] = new Texture(&g, L"../../../material/totalpoint/9.png");
	totalPointArray[10] = new Texture(&g, L"../../../material/totalpoint/10.png");
	totalPointArray[11] = new Texture(&g, L"../../../material/totalpoint/11.png");
	totalPointArray[12] = new Texture(&g, L"../../../material/totalpoint/12.png");
	totalPointArray[13] = new Texture(&g, L"../../../material/totalpoint/13.png");
	totalPointArray[14] = new Texture(&g, L"../../../material/totalpoint/14.png");
	totalPointArray[15] = new Texture(&g, L"../../../material/totalpoint/15.png");
	totalPointArray[16] = new Texture(&g, L"../../../material/totalpoint/16.png");
	totalPointArray[17] = new Texture(&g, L"../../../material/totalpoint/17.png");
	totalPointArray[18] = new Texture(&g, L"../../../material/totalpoint/18.png");
	totalPointArray[19] = new Texture(&g, L"../../../material/totalpoint/19.png");
	totalPointArray[20] = new Texture(&g, L"../../../material/totalpoint/20.png");
	totalPointArray[21] = new Texture(&g, L"../../../material/totalpoint/21.png");
	totalPointArray[22] = new Texture(&g, L"../../../material/totalpoint/22.png");
	totalPointArray[23] = new Texture(&g, L"../../../material/totalpoint/23.png");
	totalPointArray[24] = new Texture(&g, L"../../../material/totalpoint/24.png");
	totalPointArray[25] = new Texture(&g, L"../../../material/totalpoint/25.png");
	totalPointArray[26] = new Texture(&g, L"../../../material/totalpoint/26.png");
	totalPointArray[27] = new Texture(&g, L"../../../material/totalpoint/27.png");
	for (int i = 0; i < 28; i++) {
		totalPointArray[i]->SetDrawMode(TRUE);
	}
	g.Register(totalPointArray[0]);

	// 投げた回数を表示させる
	Texture *threwNumberArray[NUMBER_OF_PLAYS];
	threwNumberArray[0] = new Texture(&g, L"../../../material/threw/1.png");
	threwNumberArray[1] = new Texture(&g, L"../../../material/threw/2.png");
	threwNumberArray[2] = new Texture(&g, L"../../../material/threw/3.png");
	for (int i = 0; i < 3; i++) {
		threwNumberArray[i]->SetDrawMode(TRUE);
	}
	g.Register(threwNumberArray[0]);
	
	// Game Over画面を読み込む
	Texture gameover(&g, L"../../../material/gameover.png");
	gameover.SetDrawMode(TRUE);

	ARSI *keyIn = window.GetInputHandler();
	
	while(!d.GetCamImage(&stored));

	// アニメーション制御部分
	while (!winmgr->WaitingForTermination()){

		cout << "Now you threw[" << threwCount << "]dart," << endl;
		dart[threwCount]->GetPosition(&xDart, &yDart, &zDart);
		cout << "x = " << xDart << ", y = " << yDart << ", z = " << zDart << endl;
		cout << "Game Over Count = " << gameoverCount << endl;
		cout << "Angle = " << dart[0]->getAngle() << endl;
		
		// 3回投げたかどうか判定する
		if (gameoverCount < NUMBER_OF_PLAYS) {
			
			// 投げるダーツを描画するように登録
			g.Register(dart[threwCount]);

			// Aボタンを押した時の処理
			if (keyIn->GetKeyTrig('A')){
				
				d.GetCamImage(&stored);
				
				resetDisplay(&point, &totalPoint, &threwCount, &lastPoint, &lastTotalPoint, &lastThrewCount, &g, dart, totalPointArray, pointArray, threwNumberArray);
				reset(&point, &totalPoint, &threwCount, &lastPoint, &lastTotalPoint, &lastThrewCount, &gameoverCount, dart);
			}
		
			d.GetCamImage(&source);
		
			if (keyIn->GetKeyTrig('Q')) break;
			
			// ダーツと手が接触している領域のマスク画像を作成
			subtract_maskf(&hitArea_Hand_and_Dart,&stored,&source,0x20202020);
			// ダーツとボードが接触している領域のマスク画像を作成
			for (int i = 0; i < 10; i++) {
				g.Draw(dartBoardPoint[i], hitAreaArray[i]);
			}
			
			// ダーツボードを回転させる
			for (int i = 0; i < 10; i++) {
				dartBoardPoint[i]->SetRotationX(DARTBOARD_ROTATION);
			}
			
			// START_JUDGEMENT_X_POINTの値までは当たり判定をしないようにしている
			// ダーツがボードに当たっている場合は判定しないようにした
			if (xDart < START_JUDGEMENT_X_POINT && !dart[threwCount]->getHitDartBoard()) {

				// ダーツがボードに当たったかどうかを判定する
				// ダーツがボードに当たっている時の処理
				if (dart[threwCount]->whereToHitDartBoard(hitAreaArray[0])
					|| dart[threwCount]->whereToHitDartBoard(hitAreaArray[1])
					|| dart[threwCount]->whereToHitDartBoard(hitAreaArray[2])
					|| dart[threwCount]->whereToHitDartBoard(hitAreaArray[3])
					|| dart[threwCount]->whereToHitDartBoard(hitAreaArray[4])
					|| dart[threwCount]->whereToHitDartBoard(hitAreaArray[5])
					|| dart[threwCount]->whereToHitDartBoard(hitAreaArray[6])
					|| dart[threwCount]->whereToHitDartBoard(hitAreaArray[7])
					|| dart[threwCount]->whereToHitDartBoard(hitAreaArray[8])
					|| dart[threwCount]->whereToHitDartBoard(hitAreaArray[9])) {

					// ボードに当たったはじめの1回だけ処理される部分
					if (!dart[threwCount]->getHitDartBoard()) {

						// 当たった時の座標を取得する
						dart[threwCount]->GetPosition(&xDart, &yDart, &zDart);
						dart[threwCount]->setHitPoint(xDart, yDart, zDart);
						// 当たった時の初期回転角を計算する
						dart[threwCount]->setAngle(-atan2(yDart, zDart));
						dart[threwCount]->setBeforeAngle(-atan2(yDart, zDart));
						
						// 前回のの得点を消す
						g.Unregister(pointArray[lastPoint]);
						g.Unregister(totalPointArray[lastTotalPoint]);

						// ボードのどこにダーツが当たったかを判定する
						for (int i = 0; i < 10; i++) {
							if (dart[threwCount]->whereToHitDartBoard(hitAreaArray[i])) {
								cout << "Hit " << i << "point zone" << endl;
								point = i;
							}
						}

						// 合計ポイントを計算する
						totalPoint += point;

						// 当たったポイントを退避しておく
						lastPoint = point;
						lastTotalPoint = totalPoint;

						// 前に当たったポイントの表示から現在のポイント表示に変更する
						g.Register(pointArray[point]);
						g.Register(totalPointArray[totalPoint]);

					}// ボードに当たったはじめの1回だけ処理される部分終了
					
					// ダーツがボードに当たったフラグを立てる
					dart[lastThrewCount]->setHitDartBoard(true);
					
				} else {// ダーツがボードに当たっていない時の処理
					// ダーツがボードに当たっていないフラグを立てる
					dart[threwCount]->setHitDartBoard(false);
				}
				
			}// START_JUDGEMENT_X_POINTのif文終了

			// 一つ前の数値を格納しておく
			lastThrewCount = threwCount;
			if (dart[threwCount]->calcDeltaAngle() > DART_ROTATION) {

				// ゲーム終了までのカウントを+1する
				gameoverCount++;
				
				if (threwCount < NUMBER_OF_PLAYS - 1) {
					// 投げた回数を+1する
					threwCount++;	
				}
			}
			// 投げた回数を表示させる
			g.Unregister(threwNumberArray[lastThrewCount]);
			g.Register(threwNumberArray[threwCount]);
			
			// ダーツを動かす
			if (threwCount == 0) {
				dart[0]->react(&hitArea_Hand_and_Dart);
				dart[0]->move();
			} else if (threwCount == 1) {
				dart[0]->react(&hitArea_Hand_and_Dart);
				dart[0]->move();
				dart[1]->react(&hitArea_Hand_and_Dart);
				dart[1]->move();
			} else if (threwCount == 2) {
				dart[0]->react(&hitArea_Hand_and_Dart);
				dart[0]->move();
				dart[1]->react(&hitArea_Hand_and_Dart);
				dart[1]->move();
				dart[2]->react(&hitArea_Hand_and_Dart);
				dart[2]->move();
			}

			// 身体映像の切り抜きを行う
			bg_subtract(&mainScreen, &stored, &source, 0x20202020);
			
			//for debug(2/2)
			//debug = hitArea;
			//arsgd.Draw(&debug);
			
		} else {// 3回投げた場合の処理

			// Game Over画面を表示させる
			g.Register(&gameover);

			// Aボタンを押した時の処理
			if (keyIn->GetKeyTrig('A')) {

				g.Unregister(&gameover);
				
				resetDisplay(&point, &totalPoint, &threwCount, &lastPoint, &lastTotalPoint, &lastThrewCount, &g, dart, totalPointArray, pointArray, threwNumberArray);
				reset(&point, &totalPoint, &threwCount, &lastPoint, &lastTotalPoint, &lastThrewCount, &gameoverCount, dart);
			}

			if (keyIn->GetKeyTrig('Q')) {
				break;
			}
			
		}// 3回投げた場合の処理終了

		g.Draw();// 画面を描画する
		
	}// アニメーション制御部分終了
	
	// 動的に確保したメモリを解放する
	for (int i = 0; i < 10; i++) {
		delete hitAreaArray[i];
		delete dartBoardPoint[i];
		delete pointArray[i]; 
	}
	for (int i = 0; i < NUMBER_OF_PLAYS; i++) {
		delete dart[i];
		delete threwNumberArray[i]; 
	}
	for (int i = 0; i < 28; i++) {
		delete totalPointArray[i];
	}
	
	d.StopGraph();
	return 0;
}

void reset(int *point, int *totalPoint, int *threwCount, int *lastPoint, int *lastTotalPoint, int *lastThrewCount, int *gameoverCount, Dart *dart[]) {
	
	cout << "Reset!" << endl;

	// それぞれのダーツの情報を初期化する
	for (int i = 0; i < NUMBER_OF_PLAYS; i++) {
		dart[i]->setHitHand(false);
		dart[i]->setHitDartBoard(false);
		dart[i]->setHitPoint(0.0f, 0.0f, 0.0f);
		dart[i]->setAngle(0.0f);
		dart[i]->SetPosition(START_X_POINT, START_Y_POINT, START_Z_POINT);
	}
	
	// それぞれの制御変数を初期化する
	*point = 0;
	*totalPoint = 0;
	*threwCount = 0;

	*gameoverCount = 0;
	
	*lastPoint = 0;
	*lastTotalPoint = 0;
	*lastThrewCount = 0;
	
	
}

void resetDisplay(int *point, int *totalPoint, int *threwCount, int *lastPoint, int *lastTotalPoint, int *lastThrewCount, ARSG *g, Dart *dart[], Texture *totalPointArray[], Texture *pointArray[], Texture *threwNumberArray[]) {
	
	cout << "reset display" << endl;

	// 投げたダーツを非表示にする
	g->Unregister(dart[1]);
	g->Unregister(dart[2]);
	
	// 今まで表示していた得点表示を非表示にする
	g->Unregister(totalPointArray[*totalPoint]);
	g->Unregister(pointArray[*point]);
	g->Unregister(threwNumberArray[*threwCount]);

	// 得点表示を0に戻す
	g->Register(pointArray[0]);
	g->Register(totalPointArray[0]);
	g->Register(threwNumberArray[0]);
}

inline void Dart::react(Texture* _hitArea)
{
	int gx,gy;
	bool overlapping = get_overlapping_center(_hitArea, &gx, &gy,100);

	// 手がダーツに接触したかどうかを判定する
	if (overlapping) {
		hitHand = true;
	}
	
	VECTOR2D c;		
	GetARSG()->Convert3Dto2D(&c, GetPosition());

	switch (state) {
	case ACTIVE:
		if (overlapping) {
			vx = (c.x - gx) * 0.05f;
			vy = -(c.y - gy) * 0.05f;
			state = INACTIVE;
		}
		break;
	case INACTIVE:
		if (!overlapping)
			state = ACTIVE;
		break;
	default:
		break;
	}
}


inline void Dart::move()
{
	VECTOR2D c;
	GetARSG()->Convert3Dto2D(&c, GetPosition());
	float r;
	
	// 手がダーツに当たったらダーツを動かす
	if (hitHand) {
		
		//枠の反射
		if (c.x < 200 || c.x > sizex){
			// vx *= -1.0f;
			vx = 0;
		}
		if (c.y > sizey-50 && vy<0){
			vy *= -1.0f;
		}

		//自由落下または停止
		if (c.y > sizey-50 && vy<0.03f) 
			vy = 0;
		else
			vy -= 0.03f;

		//空気抵抗
		vx *= 0.8f;
		vy *= 0.8f;

		// ダーツボードに当たっているときの処理
		if (hitDartBoard) {

			// ダーツボードの中心と当たったところの長さを計算する
			r = sqrt(hitYPoint * hitYPoint + START_Z_POINT *START_Z_POINT);
			
			// 円運動させる
			SetPosition(-6.5f, r * (-sin(angle)), r * cos(angle) + START_Z_POINT);

			// 回転角を増やす
			angle += 0.05f;
			
		} else {// ダーツボードに当たっていなかったらそのまま普通に動かす
			SetPosition(vx, vy, 0.0f, GL_RELATIVE);
		}
		
	} else {// 手がダーツに当たっていない場合は動かさない

		SetPosition(START_X_POINT, START_Y_POINT, START_Z_POINT);
	}
	
	
}

inline bool Dart::whereToHitDartBoard(Texture *hitAreaMask) {
	
	int pGx, pGy;
	static Texture txtr;
	ARSG* g = GetARSG();
	txtr.Init(g, sizex, sizey);

	int pixel_count;

	g->Draw(this, &txtr);
	ARSC::and(&txtr, &txtr, hitAreaMask, 0x10101010);
	ARSC::getCG(&pGx, &pGy, &pixel_count, &txtr);

	return pixel_count > 25;
}

inline void subtract_maskf(Texture* result, Texture* backgrnd, Texture* src, DWORD border)
{
	ARSC::diff(result,backgrnd,src,border);
	ARSC::monochrome(result,result);
	ARSC::createmaskf(result,result,border);
}

inline bool Touchable::get_overlapping_center(Texture* hitArea, int *pGx, int *pGy, unsigned int threshold)
{	
	static Texture txtr;
	ARSG* g = GetARSG(); 
	txtr.Init(g,sizex,sizey);

	int pixel_count;

	g->Draw(this,&txtr);
	ARSC::and(&txtr, &txtr, hitArea, 0x10101010);

	ARSC::getCG(pGx, pGy, &pixel_count, &txtr);	
	return pixel_count > threshold;
}

inline void Touchable::react(Texture* _hitArea)
{
	int gx,gy;
	bool overlapping = get_overlapping_center(_hitArea, &gx, &gy,100);

	VECTOR2D c;		
	GetARSG()->Convert3Dto2D(&c, GetPosition());

	switch (state) {
	case ACTIVE:
		if (overlapping) {
			vx = (c.x - gx) * 0.05f;
			vy = -(c.y - gy) * 0.05f;
			state = INACTIVE;
		}
		break;
	case INACTIVE:
		if (!overlapping)
			state = ACTIVE;
		break;
	default:
		break;
	}
}


inline void Touchable::move()
{
	VECTOR2D c;
	GetARSG()->Convert3Dto2D(&c, GetPosition());
		
	//枠の反射
	if (c.x < 0 || c.x > sizex)	vx *= -1.0f;
	if (c.y > sizey-50 && vy<0)	vy *= -1.0f;

	//自由落下または停止
	if (c.y > sizey-50 && vy<0.03f) 
		vy = 0;
	else
		vy -= 0.03f;

	//空気抵抗
	vx *= 0.8f;
	vy *= 0.8f;

	SetPosition(vx, vy, 0.0f, GL_RELATIVE);
}

inline void bg_subtract(Texture* result, Texture* background, Texture* src, DWORD border) {
	subtract_maskf(result, background, src, border);
	ARSC::maskFilter(result, src, result);
}

int APIENTRY _tWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPTSTR lpCmdLine, int nCmdShow)
{
	WindowManager program(hInstance, &MainLoop);
#ifdef DEBUG
    MessageBox(NULL,L"OK?",TEXT(APPNAME), NULL);
#endif
    return 0;
}
