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

// develop branch file

void subtract_maskf(Texture* result, Texture* bg, Texture* src, DWORD border);
void bg_subtract(Texture* result, Texture* bg, Texture* src, DWORD border);

const unsigned int sizex = 640; 
const unsigned int sizey = 480;

UINT MainLoop(WindowManager *winmgr)
{
	//ShowDebugWindow();

	//for debug(1/2)
	//Window window2;
	//winmgr->RegisterWindow(&window2);

	//ARSG arsgd(window2.hWnd, sizex, sizey, true);
	//Texture debug(&arsgd, sizex, sizey);
	//debug.SetDrawMode(true);
	
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

	// ウインドウに表示される映像
	Texture mainScreen(&g, sizex, sizey);
	mainScreen.SetDrawMode(TRUE);
	g.Register(&mainScreen);
	
	ARSD d;
	d.Init();
	d.AttachCam(0);
	d.StartGraph();
	
	// 手とダーツが接触している領域を格納する
	Texture hitArea_Hand_and_Dart(&g,sizex,sizey);
	// 流れてくるフレームを一時的に保存する
	Texture stored (&g,sizex,sizey);
	// 身体映像を切り抜く際に背景映像として利用する
	Texture source (&g,sizex,sizey);
	source.SetDrawMode(TRUE);

	// ダーツオブジェクト
	Dart dart(&g, L"../../../material/dart.x");
	dart.SetScale(2.0f, 2.0f, 2.0f);
	dart.SetPosition(6.0f, 3.0f, 0.0f);
	g.Register(&dart);

	// ダーツボードオブジェクト
	DartBoard dartBoard(&g, L"../../../material/dartBoard.x");
	dartBoard.SetScale(0.8f, 0.8f, 0.8f);
	dartBoard.SetPosition(-6.5f, 0.0f, 0.0f);
	g.Register(&dartBoard);
	
	ARSI *keyIn = window.GetInputHandler();
	
	while(!d.GetCamImage(&stored));

	while (!winmgr->WaitingForTermination()){
		
		if (keyIn->GetKeyTrig('A')){
			d.GetCamImage(&stored);
		}
		
		d.GetCamImage(&source);
		
		if (keyIn->GetKeyTrig('Q')) break;
			
		subtract_maskf(&hitArea_Hand_and_Dart,&stored,&source,0x20202020);	
		
		bg_subtract(&mainScreen, &stored, &source, 0x20202020);

		//for debug(2/2)
		//debug = hitArea;
		//arsgd.Draw(&debug);
		g.Draw();
		
	}
	d.StopGraph();
	return 0;
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

inline void bg_subtract(Texture* result, Texture* backgrnd, Texture* src, DWORD border)
{
	subtract_maskf(result,backgrnd,src,border);
	ARSC::maskFilter(result,src,result);
}

int APIENTRY _tWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPTSTR lpCmdLine, int nCmdShow)
{
	WindowManager program(hInstance, &MainLoop);
#ifdef DEBUG
    MessageBox(NULL,L"OK?",TEXT(APPNAME), NULL);
#endif
    return 0;
}
