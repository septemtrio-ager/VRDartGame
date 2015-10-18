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
const float restartDartPosition = -4.0f;

UINT MainLoop(WindowManager *winmgr)
{
	ShowDebugWindow();

	//for debug(1/2)
	//Window window2;
	//winmgr->RegisterWindow(&window2);

	//ARSG arsgd(window2.hWnd, sizex, sizey, true);
	//Texture debug(&arsgd, sizex, sizey);
	//debug.SetDrawMode(true);
	
	// �_�[�c�ƃ_�[�c��̍��W���i�[����
	// float xDart, yDart, zDart;
	// float xDartBoart, yDartBoard, zDartBoard;

	int pGx, pGy;

	int point = 0;
	int totalPoint = 0;
	
	Window window;
	winmgr->RegisterWindow(&window);

	// ���C����ARSG�N���X�I�u�W�F�N�g
	// ����g�����C���Ŏg��
	ARSG g(window.hWnd, sizex, sizey, true);
	g.SetBackgroundColor(255,0,0,0);

	Light light(&g);
	light.SetLightIntensity(5.0f);
	g.Register(&light);
	
	// �w�i�摜��ݒ肷��
	Texture backgroundImage(&g, L"../../../material/background.jpg");
	backgroundImage.SetDrawMode(TRUE);
	g.Register(&backgroundImage);
	
	
	// mask window�ɕ\�������I�u�W�F�N�g
	// Window maskWindow;
	// winmgr->RegisterWindow(&maskWindow);
	
	// ARSG maskG(maskWindow.hWnd, sizex, sizey, true);
	// maskG.SetBackgroundColor(255, 0, 0, 0);
	
	// Light maskLight(&maskG);
	// maskLight.SetLightIntensity(5.0f);
	// maskG.Register(&maskLight);
	
	// �E�B���h�E�ɕ\�������f��
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
	
	// �ڐG���Ă���̈���i�[����
	// ��ƃ_�[�c���ڐG���Ă���̈���i�[����
	Texture hitArea_Hand_and_Dart(&g,sizex,sizey);
	// �_�[�c�̖�ƃ_�[�c�䂪�ڐG���Ă���̈���i�[�̂���
	Texture hitArea_Dart_and_DartBoard0(&g, sizex, sizey);
	hitArea_Dart_and_DartBoard0.SetDrawMode(TRUE);
	Texture hitArea_Dart_and_DartBoard1(&g, sizex, sizey);
	hitArea_Dart_and_DartBoard1.SetDrawMode(TRUE);
	Texture hitArea_Dart_and_DartBoard2(&g, sizex, sizey);
	hitArea_Dart_and_DartBoard2.SetDrawMode(TRUE);
	Texture hitArea_Dart_and_DartBoard3(&g, sizex, sizey);
	hitArea_Dart_and_DartBoard3.SetDrawMode(TRUE);
	Texture hitArea_Dart_and_DartBoard4(&g, sizex, sizey);
	hitArea_Dart_and_DartBoard4.SetDrawMode(TRUE);
	Texture hitArea_Dart_and_DartBoard5(&g, sizex, sizey);
	hitArea_Dart_and_DartBoard5.SetDrawMode(TRUE);
	Texture hitArea_Dart_and_DartBoard6(&g, sizex, sizey);
	hitArea_Dart_and_DartBoard6.SetDrawMode(TRUE);
	Texture hitArea_Dart_and_DartBoard7(&g, sizex, sizey);
	hitArea_Dart_and_DartBoard7.SetDrawMode(TRUE);
	Texture hitArea_Dart_and_DartBoard8(&g, sizex, sizey);
	hitArea_Dart_and_DartBoard8.SetDrawMode(TRUE);
	Texture hitArea_Dart_and_DartBoard9(&g, sizex, sizey);
	hitArea_Dart_and_DartBoard9.SetDrawMode(TRUE);
	
	// ����Ă���t���[�����ꎞ�I�ɕۑ�����
	Texture stored (&g,sizex,sizey);
	// �g�̉f����؂蔲���ۂɔw�i�摜�Ƃ��ė��p����
	Texture source (&g,sizex,sizey);
	source.SetDrawMode(TRUE);

	// mask
	Texture maskSource(&g, sizex, sizey);
	maskSource.SetDrawMode(TRUE);
	
	// �_�[�c�̖�
	Dart dart(&g, L"../../../material/dart.x");
	dart.SetScale(2.0f, 2.0f, 2.0f);
	dart.SetPosition(6.0f, 3.0f, 0.0f);
	g.Register(&dart);
	
	// �_�[�c��
	DartBoard dartBoard(&g, L"../../../material/dartBoard.x");
	dartBoard.SetScale(0.8f, 0.8f, 0.8f);
	dartBoard.SetPosition(-6.5f, 0.0f, 0.0f);
	g.Register(&dartBoard);

	// mask
	// �}�X�N�ƂȂ�_�[�c
	Dart maskDart(&g, L"../../../material/white_dart.x");
	maskDart.SetScale(2.0f, 2.0f, 2.0f);
	maskDart.SetPosition(6.0f, 3.0f, 0.0f);
	// g.Register(&maskDart);
		
	// �_�[�c��̓��_�͈̔͂̃}�X�N
	DartBoard dartBoardmask00(&g, L"../../../material/mask/0mask.x");
	dartBoardmask00.SetScale(0.8f, 0.8f,0.8f);
	dartBoardmask00.SetPosition(-6.5f, 0.0f, 0.0f);
	DartBoard dartBoardmask01(&g, L"../../../material/mask/1mask.x");
	dartBoardmask01.SetScale(0.8f, 0.8f, 0.8f);
	dartBoardmask01.SetPosition(-6.5f, 0.0f, 0.0f);
	DartBoard dartBoardmask02(&g, L"../../../material/mask/2mask.x");
	dartBoardmask02.SetScale(0.8f, 0.8f, 0.8f);
	dartBoardmask02.SetPosition(-6.5f, 0.0f, 0.0f);
	DartBoard dartBoardmask03(&g, L"../../../material/mask/3mask.x");
	dartBoardmask03.SetScale(0.8f, 0.8f, 0.8f);
	dartBoardmask03.SetPosition(-6.5f, 0.0f, 0.0f);
	DartBoard dartBoardmask04(&g, L"../../../material/mask/4mask.x");
	dartBoardmask04.SetScale(0.8f, 0.8f, 0.8f);
	dartBoardmask04.SetPosition(-6.5f, 0.0f, 0.0f);
	DartBoard dartBoardmask05(&g, L"../../../material/mask/5mask.x");
	dartBoardmask05.SetScale(0.8f, 0.8f, 0.8f);
	dartBoardmask05.SetPosition(-6.5f, 0.0f, 0.0f);
	DartBoard dartBoardmask06(&g, L"../../../material/mask/6mask.x");
	dartBoardmask06.SetScale(0.8f, 0.8f, 0.8f);
	dartBoardmask06.SetPosition(-6.5f, 0.0f, 0.0f);
	DartBoard dartBoardmask07(&g, L"../../../material/mask/7mask.x");
	dartBoardmask07.SetScale(0.8f, 0.8f, 0.8f);
	dartBoardmask07.SetPosition(-6.5f, 0.0f, 0.0f);
	DartBoard dartBoardmask08(&g, L"../../../material/mask/8mask.x");
	dartBoardmask08.SetScale(0.8f, 0.8f, 0.8f);
	dartBoardmask08.SetPosition(-6.5f, 0.0f, 0.0f);
	DartBoard dartBoardmask09(&g, L"../../../material/mask/9mask.x");
	dartBoardmask09.SetScale(0.8f, 0.8f, 0.8f);
	dartBoardmask09.SetPosition(-6.5f, 0.0f, 0.0f);
		
	ARSI *keyIn = window.GetInputHandler();
	
	while(!d.GetCamImage(&stored));

	// �A�j���[�V�����̐��䕔��
	while (!winmgr->WaitingForTermination()){

		// �_�[�c�̍��W���擾����
		// dart.GetPosition(&xDart, &yDart, &zDart);
		// dart.setYDart(yDart);

		// std::cout << "xDart = " << xDart << " yDart = " << yDart << " zDart = " << zDart << std::endl;
		
		// A�{�^�������������̋���
		if (keyIn->GetKeyTrig('A')) {
			
			d.GetCamImage(&stored);
			
			// �󋵂����Z�b�g�����̂�false�ɐݒ�
			dart.setOverlappingOnce(false);
			dart.setHitOnce(false);
			dart.SetPosition(6.0f, 3.0f, 0.0f);
			
			// �}�X�N�_�[�c�̓��������Z�b�g
			// maskDart.SetPosition(xDart, yDart, zDart);

			// �l�������|�C���g�����Z�b�g
			point = 0;
			
			std::cout << "A�{�^����������܂���" << endl;
		}
		
		d.GetCamImage(&source);

		
		if (keyIn->GetKeyTrig('Q')) break;
			
		// �_�[�c�̖�Ǝ肪�ڐG���Ă���̈�̃}�X�N�摜���쐬
		subtract_maskf(&hitArea_Hand_and_Dart,&stored,&source,0x20202020);
		// subtract_maskf(&hitArea_Dart_and_DartBoard0, &g, &g, 0x20202020);

		// �_�[�c�̖�ƃ_�[�c�䂪�ڐG���Ă���̈�̃}�X�N�摜���쐬
		g.Draw(&dartBoardmask00, &hitArea_Dart_and_DartBoard0);
		g.Draw(&dartBoardmask01, &hitArea_Dart_and_DartBoard1);
		g.Draw(&dartBoardmask02, &hitArea_Dart_and_DartBoard2);
		g.Draw(&dartBoardmask03, &hitArea_Dart_and_DartBoard3);
		g.Draw(&dartBoardmask04, &hitArea_Dart_and_DartBoard4);
		g.Draw(&dartBoardmask05, &hitArea_Dart_and_DartBoard5);
		g.Draw(&dartBoardmask06, &hitArea_Dart_and_DartBoard6);
		g.Draw(&dartBoardmask07, &hitArea_Dart_and_DartBoard7);
		g.Draw(&dartBoardmask08, &hitArea_Dart_and_DartBoard8);
		g.Draw(&dartBoardmask09, &hitArea_Dart_and_DartBoard9);
		
		// �_�[�c�����]������
		dartBoard.SetRotationX(0.05f);
		// mask
		dartBoardmask00.SetRotationX(0.05f);
		dartBoardmask01.SetRotationX(0.05f);
		dartBoardmask02.SetRotationX(0.05f);
		dartBoardmask03.SetRotationX(0.05f);
		dartBoardmask04.SetRotationX(0.05f);
		dartBoardmask05.SetRotationX(0.05f);
		dartBoardmask06.SetRotationX(0.05f);
		dartBoardmask07.SetRotationX(0.05f);
		dartBoardmask08.SetRotationX(0.05f);
		dartBoardmask09.SetRotationX(0.05f);
		
		// �_�[�c����ɓ����������ǂ����𔻒�
		if (dart.whereToHitDartBoard(&hitArea_Dart_and_DartBoard1)
			|| dart.whereToHitDartBoard(&hitArea_Dart_and_DartBoard2)
			|| dart.whereToHitDartBoard(&hitArea_Dart_and_DartBoard3)
			|| dart.whereToHitDartBoard( &hitArea_Dart_and_DartBoard4)
			|| dart.whereToHitDartBoard( &hitArea_Dart_and_DartBoard5)
			|| dart.whereToHitDartBoard( &hitArea_Dart_and_DartBoard6)
			|| dart.whereToHitDartBoard( &hitArea_Dart_and_DartBoard7)
			|| dart.whereToHitDartBoard( &hitArea_Dart_and_DartBoard8)
			|| dart.whereToHitDartBoard( &hitArea_Dart_and_DartBoard9)) {
			
			// �_�[�c�����������t���O�𗧂Ă�
			dart.setHitDartBoard(true);
			dartBoard.setHitDart(true);

			if (!dart.getHitOnce()) {
				
				// �ǂ��Ƀ_�[�c�������������𔻒肷��
				if (dart.whereToHitDartBoard(&hitArea_Dart_and_DartBoard0)) {
					std::cout << "hit 0 point zone" << std::endl;
					point = 0;
				
				} else if (dart.whereToHitDartBoard(&hitArea_Dart_and_DartBoard1)) {
					std::cout << "hit 1 point zone" << std::endl;
					point = 1;
				
				} else if (dart.whereToHitDartBoard(&hitArea_Dart_and_DartBoard2)) {
					std::cout << "hit 2 point zone" << std::endl;
					point = 2;
				
				} else if (dart.whereToHitDartBoard(&hitArea_Dart_and_DartBoard3)) {
					std::cout << "hit 3 point zone" << std::endl;
					point = 3;
				
				} else if (dart.whereToHitDartBoard(&hitArea_Dart_and_DartBoard4)) {
					std::cout << "hit 4 point zone" << std::endl;
					point = 4;
				
				} else if (dart.whereToHitDartBoard(&hitArea_Dart_and_DartBoard5)) {
					std::cout << "hit 5 point zone" << std::endl;
					point = 5;
				
				} else if (dart.whereToHitDartBoard(&hitArea_Dart_and_DartBoard6)) {
					std::cout << "hit 6 point zone" << std::endl;
					point = 6;
				
				} else if (dart.whereToHitDartBoard(&hitArea_Dart_and_DartBoard7)) {
					std::cout << "hit 7 point zone" << std::endl;
					point = 7;
				
				} else if (dart.whereToHitDartBoard(&hitArea_Dart_and_DartBoard8)) {
					std::cout << "hit 8 point zone" << std::endl;
					point = 8;
				
				} else if (dart.whereToHitDartBoard(&hitArea_Dart_and_DartBoard9)) {
					std::cout << "hit 9 point zone" << std::endl;
					point = 9;
				
				}

				// 1�񓖂��������Ƃ������t���O��true�ɂ���
				dart.setHitOnce(true);
			}
		} else {
			// �_�[�c���������Ă��Ȃ��t���O�𗧂Ă�
			dart.setHitDartBoard(false);
			// dartBoard.setHitDart(false);
		}
		
		// �_�[�c����ɂ������Ă��鎞�̏���
		if (dart.getHitDartBoard()) {
			std::cout << "hit dart board" << std::endl; 
		} else {
			std::cout << "not hit dart board" << std::endl;
		}
				
		// �_�[�c�𓮂���
		dart.react(&hitArea_Hand_and_Dart);
		dart.move();

		std::cout << "Now you get " << point << " Point" << std::endl;
		
		// �}�X�N�E�C���h�E�̃_�[�c�̓�����������
		// maskDart.SetPosition(xDart, yDart, zDart);
		
		bg_subtract(&mainScreen, &stored, &source, 0x20202020);

		//for debug(2/2)
		//debug = hitArea;
		//arsgd.Draw(&debug);
		
		g.Draw();
		// maskG.Draw();
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

inline bool Touchable::whereToHitDartBoard(Texture *hitAreaMask) {
	
	int pGx, pGy;
	static Texture txtr;
	ARSG* g = GetARSG();
	txtr.Init(g, sizex, sizey);

	int pixel_count;

	g->Draw(this, &txtr);
	ARSC::and(&txtr, &txtr, hitAreaMask, 0x10101010);
	ARSC::getCG(&pGx, &pGy, &pixel_count, &txtr);

	return pixel_count > 50;
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
		
	//�g�̔���
	if (c.x < 0 || c.x > sizex)	vx *= -1.0f;
	if (c.y > sizey-50 && vy<0)	// vy *= -1.0f;

	
	if (c.y > sizey - 50 && vy < 0.03f) {
		// vy = 0;
	}
	else{
		// vy -= 0.03f;
	}
	//��C��R
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

		//�g�̔���
		if (c.x < 0 || c.x > sizex)	{
			vx *= -1.0f;
		}
		if (c.y > sizey - 50 && vy<0)	{
			vy *= -1.0f;
		}

		//���R�����܂��͒�~
		if (c.y > sizey - 50 && vy < 0.03f) {
			vy = 0;

			// �r���ŉ�ʉ��Œ�~������X�^�[�g�ʒu�ɖ߂�悤
			SetPosition(6.0f, 3.0f, 0.0f);
		}
		else {
			vy -= 0.03f;
		}

		//��C��R
		vx *= 0.8f;
		vy *= 0.8f;

		if (yDart > restartDartPosition) {
			SetPosition(vx, vy, 0.0f, GL_RELATIVE);
		} else {
			
			// ������̂��I������Ƃ��̏���
			if (hitDartBoard) {
				
				std::cout << "������̂����[" << std::endl;
				
				// ��Ԃ������t���O�����Z�b�g
				overlappingOnce = false;
				hitDartBoard = false;
				hitOnce = false;

				// �X�^�[�g�ʒu�Ɉړ�������
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
//	//�g�̔���
//	if (c.x < 0 || c.x > sizex)	vx *= -1.0f;
//	if (c.y > sizey-50 && vy<0)	vy *= -1.0f;
//
//	//���R�����܂��͒�~
//	if (c.y > sizey-50 && vy<0.03f) 
//		vy = 0;
//	else
//		vy -= 0.03f;
//
//	//��C��R
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
