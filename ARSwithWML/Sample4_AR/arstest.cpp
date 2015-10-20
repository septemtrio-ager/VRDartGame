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

//-----------------------------------------------------------------------------
// Name: WinMain()
// Desc: The application's entry point
//-----------------------------------------------------------------------------

// version 0.2 git

void subtract_maskf(Texture* result, Texture* bg, Texture* src, DWORD border);
void bg_subtract(Texture* result, Texture* background, Texture* src, DWORD border);

const unsigned int sizex = 640; 
const unsigned int sizey = 480;

// const float hitThreshold = -3.0f;
const float restartDartPosition = -3.0f;

UINT MainLoop(WindowManager *winmgr)
{
	ShowDebugWindow();

	//for debug(1/2)
	//Window window2;
	//winmgr->RegisterWindow(&window2);

	//ARSG arsgd(window2.hWnd, sizex, sizey, true);
	//Texture debug(&arsgd, sizex, sizey);
	//debug.SetDrawMode(true);
	
	// ダーツとダーツ台の座標を格納する
	float xDart, yDart, zDart;
	// float xDartBoart, yDartBoard, zDartBoard;

	int pGx, pGy;

	int point = 0;
	int totalPoint = 0;
	
	Window window;
	winmgr->RegisterWindow(&window);

	// メインのARSGクラスオブジェクト
	// このgをメインで使う
	ARSG g(window.hWnd, sizex, sizey, true);
	g.SetBackgroundColor(255,0,0,0);

	Light light(&g);
	light.SetLightIntensity(5.0f);
	g.Register(&light);
	
	// 背景画像を設定する
	Texture backgroundImage(&g, L"../../../material/background.jpg");
	backgroundImage.SetDrawMode(TRUE);
	g.Register(&backgroundImage);
	
	// mask windowに表示されるオブジェクト
	// Window maskWindow;
	// winmgr->RegisterWindow(&maskWindow);
	
	// ARSG maskG(maskWindow.hWnd, sizex, sizey, true);
	// maskG.SetBackgroundColor(255, 0, 0, 0);
	
	// Light maskLight(&maskG);
	// maskLight.SetLightIntensity(5.0f);
	// maskG.Register(&maskLight);
	
	// ウィンドウに表示される映像
	Texture mainScreen(&g, sizex, sizey);
	mainScreen.SetDrawMode(TRUE);
	g.Register(&mainScreen);

	// mask
	// Texture maskScreen(&maskG, sizex, sizey);
	// maskScreen.SetDrawMode(TRUE);
	// maskG.Register(&maskScreen);

	ARSD d;
	d.Init();
	d.AttachCam(0);
	d.StartGraph();
	
	// 接触している領域を格納する
	// 手とダーツが接触している領域を格納する
	Texture hitArea_Hand_and_Dart(&g,sizex,sizey);
	
	// ダーツの矢とダーツ台が接触している領域を格納のする
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

	// mask
	// Texture maskSource(&g, sizex, sizey);
	// maskSource.SetDrawMode(TRUE);
	
	// ダーツの矢
	Dart dart(&g, L"../../../material/dart.x");
	dart.SetScale(2.0f, 2.0f, 2.0f);
	dart.SetPosition(6.0f, 3.0f, 0.0f);
	g.Register(&dart);
	
	// ダーツ台
	DartBoard dartBoard(&g, L"../../../material/dartBoard.x");
	dartBoard.SetScale(0.8f, 0.8f, 0.8f);
	dartBoard.SetPosition(-6.5f, 0.0f, 0.0f);
	g.Register(&dartBoard);

	// mask
	// マスクとなるダーツ
	// Dart maskDart(&g, L"../../../material/white_dart.x");
	// maskDart.SetScale(2.0f, 2.0f, 2.0f);
	// maskDart.SetPosition(6.0f, 3.0f, 0.0f);
	// g.Register(&maskDart);
		
	// ダーツ台の得点の範囲のマスク
	DartBoard *dartBoardMaskArray[10];
	dartBoardMaskArray[0] = new DartBoard(&g, L"../../../material/mask/0mask.x");
	dartBoardMaskArray[1] = new DartBoard(&g, L"../../../material/mask/1mask.x");
	dartBoardMaskArray[2] = new DartBoard(&g, L"../../../material/mask/2mask.x");
	dartBoardMaskArray[3] = new DartBoard(&g, L"../../../material/mask/3mask.x");
	dartBoardMaskArray[4] = new DartBoard(&g, L"../../../material/mask/4mask.x");
	dartBoardMaskArray[5] = new DartBoard(&g, L"../../../material/mask/5mask.x");
	dartBoardMaskArray[6] = new DartBoard(&g, L"../../../material/mask/6mask.x");
	dartBoardMaskArray[7] = new DartBoard(&g, L"../../../material/mask/7mask.x");
	dartBoardMaskArray[8] = new DartBoard(&g, L"../../../material/mask/8mask.x");
	dartBoardMaskArray[9] = new DartBoard(&g, L"../../../material/mask/9mask.x");
	for (int i = 0; i < 10; i++) {
		dartBoardMaskArray[i]->SetScale(0.8f, 0.8f, 0.8f);
		dartBoardMaskArray[i]->SetPosition(-6.5f, 0.0f, 0.0f);
	}
	
	ARSI *keyIn = window.GetInputHandler();
	
	while(!d.GetCamImage(&stored));

	// アニメーションの制御部分
	while (!winmgr->WaitingForTermination()){

		// ダーツの座標を取得する
		dart.GetPosition(&xDart, &yDart, &zDart);
		dart.setYDart(yDart);

		// std::cout << "xDart = " << xDart << " yDart = " << yDart << " zDart = " << zDart << std::endl;
		
		// Aボタンを押した時の挙動
		if (keyIn->GetKeyTrig('A')) {
			
			d.GetCamImage(&stored);
			
			// 状況がリセットされるのでfalseに設定
			dart.setOverlappingOnce(false);
			dart.setHitOnce(false);
			dart.SetPosition(6.0f, 3.0f, 0.0f);
			
			// マスクダーツの動きもリセット
			// maskDart.SetPosition(xDart, yDart, zDart);

			// 獲得したポイントもリセット
			point = 0;
			
			std::cout << "Aボタンが押されました" << endl;
		}
		
		d.GetCamImage(&source);

		
		if (keyIn->GetKeyTrig('Q')) break;
			
		// ダーツの矢と手が接触している領域のマスク画像を作成
		subtract_maskf(&hitArea_Hand_and_Dart,&stored,&source,0x20202020);
		// subtract_maskf(&hitArea_Dart_and_DartBoard0, &g, &g, 0x20202020);

		// ダーツの矢とダーツ台が接触している領域のマスク画像を作成
		for (int i = 0; i < 10; i++) {
			g.Draw(dartBoardMaskArray[i], hitAreaArray[i]);
		}

		
		// ダーツ台を回転させる
		dartBoard.SetRotationX(0.05f);
		// mask
		for (int i = 0; i < 10; i++) {
			dartBoardMaskArray[i]->SetRotationX(0.05f);
		}
		
		// ダーツが台に当たったかどうかを判定
		if (dart.whereToHitDartBoard(hitAreaArray[0])
			|| dart.whereToHitDartBoard(hitAreaArray[1])
			|| dart.whereToHitDartBoard(hitAreaArray[2])
			|| dart.whereToHitDartBoard(hitAreaArray[3])
			|| dart.whereToHitDartBoard(hitAreaArray[4])
			|| dart.whereToHitDartBoard(hitAreaArray[5])
			|| dart.whereToHitDartBoard(hitAreaArray[6])
			|| dart.whereToHitDartBoard(hitAreaArray[7])
			|| dart.whereToHitDartBoard(hitAreaArray[8])
			|| dart.whereToHitDartBoard(hitAreaArray[9])) {
			
			// ダーツが当たったフラグを立てる
			dart.setHitDartBoard(true);
			dartBoard.setHitDart(true);

			if (!dart.getHitOnce()) {

				for (int i = 0; i < 10; i++) {
					// どこにダーツが当たったかを判定する
					if (dart.whereToHitDartBoard(hitAreaArray[i])) {
						std::cout << "hit " << i << "point zone" << std::endl;
						point = i;
					}
				}

				// 1回当たったことを示すフラグをtrueにする
				dart.setHitOnce(true);
			}
		} else {
			// ダーツが当たっていないフラグを立てる
			dart.setHitDartBoard(false);
			// dartBoard.setHitDart(false);
		}
		
		// ダーツが台にあたっている時の処理
		if (dart.getHitDartBoard()) {
			std::cout << "hit dart board" << std::endl; 
		} else {
			std::cout << "not hit dart board" << std::endl;
		}
				
		// ダーツを動かす
		dart.react(&hitArea_Hand_and_Dart);
		dart.move();

		std::cout << "Now you get " << point << " Point" << std::endl;
		
		// マスクウインドウのダーツの動きもさせる
		// maskDart.SetPosition(xDart, yDart, zDart);
		
		bg_subtract(&mainScreen, &stored, &source, 0x20202020);

		//for debug(2/2)
		//debug = hitArea;
		//arsgd.Draw(&debug);
		
		g.Draw();
		// maskG.Draw();
	}
	
	d.StopGraph();

	// 動的に確保したメモリを開放する
	for (int i = 0; i < 10; i++) {
		delete hitAreaArray[i];
	}

	
	for (int i = 0; i < 10; i++) {
		delete dartBoardMaskArray[i];
	}
	
	return 0;
}

inline void subtract_maskf(Texture* result, Texture* backgrnd, Texture* src, DWORD border)
{
	ARSC::diff(result,backgrnd,src,border);
	ARSC::monochrome(result,result);
	ARSC::createmaskf(result,result,border);
}

inline bool Touchable::whereToHitDartBoard(Texture *hitAreaMask) {
	
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
	if (c.y > sizey-50 && vy<0)	// vy *= -1.0f;

	
	if (c.y > sizey - 50 && vy < 0.03f) {
		// vy = 0;
	}
	else{
		// vy -= 0.03f;
	}
	//空気抵抗
	vx *= 0.8f;
	// vy *= 0.8f;

   // SetPosition(vx, vy, 0.0f, GL_RELATIVE);
	SetPosition(vx, 0.0f, 0.0f, GL_RELATIVE);
}

inline void Dart::react(Texture* _hitArea) {

	int gx,gy;
	bool overlapping = get_overlapping_center(_hitArea, &gx, &gy,100);
	
	if (overlapping) {
		overlappingOnce = true;
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

inline void Dart::move() {

	VECTOR2D c;
	GetARSG()->Convert3Dto2D(&c, GetPosition());

	if (overlappingOnce){

		//枠の反射
		if (c.x < 0 || c.x > sizex)	{
			vx *= -1.0f;
		}
		if (c.y > sizey - 50 && vy<0)	{
			vy *= -1.0f;
		}

		//自由落下または停止
		if (c.y > sizey - 50 && vy < 0.03f) {
			vy = 0;

			// 途中で画面下で停止したらスタート位置に戻るよう
			SetPosition(6.0f, 3.0f, 0.0f);
		}
		else {
			vy -= 0.03f;
		}

		//空気抵抗
		vx *= 0.8f;
		vy *= 0.8f;

		if (yDart > restartDartPosition) {
			SetPosition(vx, vy, 0.0f, GL_RELATIVE);
		} else {
			
			// 落ちるのが終わったときの処理
			if (hitDartBoard) {
				
				std::cout << "落ちるのおわりー" << std::endl;
				
				// 状態を示すフラグをリセット
				overlappingOnce = false;
				hitDartBoard = false;
				hitOnce = false;

				// スタート位置に移動させる
				SetPosition(6.0f, 3.0f, 0.0f);
			}else {
				SetPosition(vx, vy, 0.0f, GL_RELATIVE);
			}
		}
	}
	else {
		SetPosition(0.0f, 0.0f, 0.0f, GL_RELATIVE);
	}
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




//#pragma once
//
//#include <windows.h>
//#include <tchar.h>
//#define D3D_DEBUG_INFO
//#include <stdlib.h>
//#include <math.h>
//
//
//#include "../WML/WindowManager.h"
//#include "arstest.h"
//
////-----------------------------------------------------------------------------
//// Name: WinMain()
//// Desc: The application's entry point
////-----------------------------------------------------------------------------
//
//const int sizex  = 640;
//const int sizey = 480;

//UINT MainLoop(WindowManager *winmgr)
//{
//	ShowDebugWindow();
//	Window window;
//	winmgr->RegisterWindow(&window);
//
//	ARSG g(window.hWnd, sizex, sizey, true);
//	g.SetBackgroundColor(255,0,0,0);
//
//	Light light;
//	g.CreateLight(&light);
//	g.RegisterLight(&light);
//
//	Texture screen;
//	g.CreateTexture(&screen, L"beach.jpg");
//	screen.SetDrawMode(TRUE);
//	g.RegisterShape(&screen);
//
//	Mesh sphere;
//	g.CreateMesh(&sphere, L"ball.x");	
//	sphere.SetPosition(0.0f, 5.0f, 0.0f,GL_ABSOLUTE);
//	g.RegisterShape(&sphere);
//	
//	while (!winmgr->WaitingForTermination()){
//		sphere.SetRotationY(0.01f);						
//		sphere.SetPosition(0.01f,0.0f,0.0f,GL_RELATIVE);
//		g.Draw();
//	}
//
//	return 0;
//}

//const int sizex  = 640;
//const int sizey = 480;
//
//UINT MainLoop(WindowManager *winmgr)
//{
////	ShowDebugWindow();
//	Window window;
//	winmgr->RegisterWindow(&window);
//
//	ARSG g(window.hWnd, sizex, sizey, true);
//	g.SetBackgroundColor(255,0,0,0);
//
//	Light light;
//	g.CreateLight(&light);
//	g.RegisterLight(&light);
//
//	Texture screen;
//	g.CreateTexture(&screen, sizex, sizey);
//	screen.SetDrawMode(TRUE);
//	g.RegisterShape(&screen);
//
//	Mesh sphere;
//	g.CreateMesh(&sphere, L"ball.x");	
//	g.RegisterShape(&sphere);
//
//	ARSD d;
//	d.Init();
//	d.AttachCam(0);
//	d.StartGraph();
//	g.SetEnvironmentTexture(&screen);
//	
//	while (!winmgr->WaitingForTermination()){
//		sphere.SetRotationY(0.01f);						
//		d.GetCamImage(&screen);
//		g.Draw();
//	}
//
//	return 0;
//}

//
//int APIENTRY _tWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPTSTR lpCmdLine, int nCmdShow)
//{
//	WindowManager program(hInstance, &MainLoop);
//#ifdef DEBUG
//    MessageBox(NULL,L"OK?",TEXT(APPNAME), NULL);
//#endif
//    return 0;
//}

//#pragma once
//
//#include "arstest.h"
//#include "../WML/WindowManager.h"
//#include <windows.h>
//#include <tchar.h>
//#define D3D_DEBUG_INFO
//
//#include <stdlib.h>
//#include <math.h>
//
////-----------------------------------------------------------------------------
//// Name: WinMain()
//// Desc: The application's entry point
////-----------------------------------------------------------------------------
//
//
//void subtract_maskf(Texture* result, Texture* bg, Texture* src, DWORD border);
//unsigned int sizex, sizey, depth;
//
//
//UINT MainLoop(WindowManager *winmgr)
//{
////	ShowDebugWindow();
//	Window window;
//	winmgr->RegisterWindow(&window);
//
//
//	sizex = 640;
//	sizey = 480;
//	depth = 4;
//
//	ARSG g(window.hWnd, sizex, sizey, true);
//	ARSD d;
//	Light light;
//	
//	g.CreateLight(&light);
//	g.RegisterLight(&light);
//	Texture sourceTex;
//	g.CreateTexture(&sourceTex, sizex, sizey);
////	g.CreateTexture(&sourceTex, L".\\fan.png");
//
//	d.Init();
//	d.AttachCam(0);
//	d.StartGraph();
//
//	g.SetBackgroundColor(255,0,0,0);
//
//	//sourceTex.SetPosition(0,0,0);
//	Texture hitArea, stored, source;
//	g.CreateTexture(&hitArea,640,480);
//	g.CreateTexture(&stored,640,480);
//	g.CreateTexture(&source,640,480);
//
//	source.SetDrawMode(TRUE);
//	g.RegisterShape(&source);
//
//	Touchable ball(&g);
//	g.CreateMesh(&ball, L"ball.x");
//	ball.SetScale(2.0f, 2.0f, 2.0f);
//	ball.SetPosition(0.0f, 5.0f, 0.0f,GL_ABSOLUTE);
//
//	g.RegisterShape(&ball);
//
//	source.SetScale(1,1,1);
//
//	ARSI *keyIn = window.GetInputHandler();
//	while(d.GetCamImage(&source) < 5);
//	d.GetCamImage(&stored);
//	
//	while (!winmgr->WaitingForTermination()){
//		if (keyIn->GetKeyTrig('A')) {
//			d.GetCamImage(&stored);
//		}
//
//		d.GetCamImage(&source);
//			
//		if (keyIn->GetKeyTrig('Q')) break;
//			
//		subtract_maskf(&hitArea,&stored,&source,0x20202020);	
//
//       
//		ball.react(&hitArea);
//		ball.move();
//
//		g.Draw();
//		
//	}
//	d.StopGraph();
//	return 0;
//}
//
//inline void subtract_maskf(Texture* result, Texture* backgrnd, Texture* src, DWORD border)
//{
//	ARSC::diff(result,backgrnd,src,border);
//	ARSC::monochrome(result,result);
//	ARSC::createmaskf(result,result,border);
//}
//
//
//inline bool Touchable::check_overlap(Texture* hitArea, unsigned int threshold)
//{
//	Texture txtr;
//	int pixel_count;
//
//	g->CreateTexture(&txtr, sizex, sizey);
//	g->Draw(this,&txtr);
//	ARSC::and(hitArea, hitArea, &txtr, 0x10101010);
//
//	ARSC::getCG(&gx, &gy, &pixel_count, hitArea);	
//	return pixel_count > threshold;
//}
//
//
//inline void Touchable::react(Texture* _hitArea)
//{
//	bool overlap = check_overlap(_hitArea,100);
//
//	VECTOR2D c;	
//	g->Convert3Dto2D(&c, GetPosition());
//
//	switch (state) {
//		case ACTIVE:
//			if (overlap) {
//				vx = (c.x - gx) * 0.05f;
//				vy = -(c.y - gy) * 0.05f;
//				state = INACTIVE;
//			}
//			break;
//		case INACTIVE:
//			if (!overlap)
//				state = ACTIVE;
//			break;
//		default:
//			break;
//	}
//}
//
//inline void Touchable::move()
//{
//	VECTOR2D c;
//
//	g->Convert3Dto2D(&c, GetPosition());
//		
//	//枠の反射
//	if (c.x < 0 || c.x > sizex)	vx *= -1.0f;
//	if (c.y > sizey-50 && vy<0)	vy *= -1.0f;
//
//	//自由落下または停止
//	if (c.y > sizey-50 && vy<0.03f) 
//		vy = 0;
//	else
//		vy -= 0.03f;
//
//	//空気抵抗
//	vx *= 0.8f;
//	vy *= 0.8f;
//
//   SetPosition(vx, vy, 0.0f, GL_RELATIVE);
//}
//
//inline void Touchable::setARSG(ARSG* _g)
//{
//	g = _g;
//}
//
//
//int APIENTRY _tWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPTSTR lpCmdLine, int nCmdShow)
//{
//	WindowManager program(hInstance, &MainLoop);
//#ifdef DEBUG
//    MessageBox(NULL,L"OK?",TEXT(APPNAME), NULL);
//#endif
//    return 0;
//}
