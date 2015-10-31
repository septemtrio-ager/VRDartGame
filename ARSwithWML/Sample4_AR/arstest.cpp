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

// A�{�^���������ꂽ���Ȃǂŏ󋵂����Z�b�g����Ƃ��ɌĂяo��
void reset(int *point, int *totalPoint, int *threwCount, int *lastPoint, int *lastTotalPoint, int *lastThrewCount, int *gameoverCount, Dart *dart[]);
// A�{�^���������ꂽ���ɕ\�������Z�b�g����Ƃ��ɌĂяo��
void resetDisplay(int *point, int *totalPoint, int *threwCount, int *lastPoint, int *lastTotalPoint, int *lastThrewCount, ARSG *g, Dart *dart[], Texture *totalPointArray[], Texture *pointArray[], Texture *threwNumberArray[]);

// �E�B���h�E�T�C�Y
const unsigned int sizex = 640; 
const unsigned int sizey = 480;

// �_�[�c���X�^�[�g����ʒu
const float START_X_POINT = 6.0f;
const float START_Y_POINT = 3.0f;
const float START_Z_POINT = 2.0f;

// �_�[�c�{�[�h�̏ꏊ
const float DARTBOARD_X_POINT = -6.5f;
const float DARTBOARD_Y_POINT = 0.0f;
const float DARTBOARD_Z_POINT = 0.0f;

// �v���C�ł����
const int NUMBER_OF_PLAYS = 3;

// �_�[�c�ƃ{�[�h�̓����蔻����J�n����X�����W
const float START_JUDGEMENT_X_POINT = -4.0f;

// �_�[�c�̃X�P�[��
const float DART_SCALE = 2.3f;
// �_�[�c�{�[�h�̃X�P�[��
const float DARTBOARD_SCALE = 0.8f;
// ���̃_�[�c���Z�b�g�����܂ł̃_�[�c�̉�]��
const float DART_ROTATION = PI;// ����]�����玟�̃_�[�c���Z�b�g����悤�ɂ���

// �_�[�c�{�[�h�̉�]��
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

	// �w�i�摜��ݒ肷��
	Texture backgroundImage(&g, L"../../../material/background.jpg");
	backgroundImage.SetDrawMode(TRUE);
	g.Register(&backgroundImage);

	// �E�B���h�E�ɕ\�������f��
	Texture mainScreen(&g, sizex, sizey);
	mainScreen.SetDrawMode(TRUE);
	g.Register(&mainScreen);
	
	ARSD d;
	d.Init();
	d.AttachCam(0);
	d.StartGraph();
	
	// ��ƃ_�[�c���ڐG���Ă���̈���i�[����
	Texture hitArea_Hand_and_Dart(&g,sizex,sizey);
	// �_�[�c�ƃ{�[�h���ڐG���Ă���̈���i�[����
	Texture *hitAreaArray[10];
	for (int i = 0; i < 10; i++) {
		hitAreaArray[i] = new Texture(&g, sizex, sizey);
		hitAreaArray[i]->SetDrawMode(TRUE);
	}
	// ����Ă���t���[�����ꎞ�I�ɕۑ�����
	Texture stored (&g,sizex,sizey);
	// �g�̉f����؂蔲���ۂɔw�i�摜�Ƃ��ė��p����
	Texture source (&g,sizex,sizey);
	source.SetDrawMode(TRUE);
	
	// �_�[�c��NUMBER_OF_PLAYS�������p�ӂ���
	Dart *dart[NUMBER_OF_PLAYS];
	for (int i = 0; i < NUMBER_OF_PLAYS; i++) {
		dart[i] = new Dart(&g, L"../../../material/dart.x");
		dart[i]->SetScale(DART_SCALE, DART_SCALE, DART_SCALE);
		dart[i]->SetPosition(START_X_POINT, START_Y_POINT, START_Z_POINT);
	}
	
	// �_�[�c�{�[�h
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

	// �_���\���̉摜��ǂݍ���
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

	// ���v���_�\���̉摜��ǂݍ���
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

	// �������񐔂�\��������
	Texture *threwNumberArray[NUMBER_OF_PLAYS];
	threwNumberArray[0] = new Texture(&g, L"../../../material/threw/1.png");
	threwNumberArray[1] = new Texture(&g, L"../../../material/threw/2.png");
	threwNumberArray[2] = new Texture(&g, L"../../../material/threw/3.png");
	for (int i = 0; i < 3; i++) {
		threwNumberArray[i]->SetDrawMode(TRUE);
	}
	g.Register(threwNumberArray[0]);
	
	// Game Over��ʂ�ǂݍ���
	Texture gameover(&g, L"../../../material/gameover.png");
	gameover.SetDrawMode(TRUE);

	ARSI *keyIn = window.GetInputHandler();
	
	while(!d.GetCamImage(&stored));

	// �A�j���[�V�������䕔��
	while (!winmgr->WaitingForTermination()){

		cout << "Now you threw[" << threwCount << "]dart," << endl;
		dart[threwCount]->GetPosition(&xDart, &yDart, &zDart);
		cout << "x = " << xDart << ", y = " << yDart << ", z = " << zDart << endl;
		cout << "Game Over Count = " << gameoverCount << endl;
		cout << "Angle = " << dart[0]->getAngle() << endl;
		
		// 3�񓊂������ǂ������肷��
		if (gameoverCount < NUMBER_OF_PLAYS) {
			
			// ������_�[�c��`�悷��悤�ɓo�^
			g.Register(dart[threwCount]);

			// A�{�^�������������̏���
			if (keyIn->GetKeyTrig('A')){
				
				d.GetCamImage(&stored);
				
				resetDisplay(&point, &totalPoint, &threwCount, &lastPoint, &lastTotalPoint, &lastThrewCount, &g, dart, totalPointArray, pointArray, threwNumberArray);
				reset(&point, &totalPoint, &threwCount, &lastPoint, &lastTotalPoint, &lastThrewCount, &gameoverCount, dart);
			}
		
			d.GetCamImage(&source);
		
			if (keyIn->GetKeyTrig('Q')) break;
			
			// �_�[�c�Ǝ肪�ڐG���Ă���̈�̃}�X�N�摜���쐬
			subtract_maskf(&hitArea_Hand_and_Dart,&stored,&source,0x20202020);
			// �_�[�c�ƃ{�[�h���ڐG���Ă���̈�̃}�X�N�摜���쐬
			for (int i = 0; i < 10; i++) {
				g.Draw(dartBoardPoint[i], hitAreaArray[i]);
			}
			
			// �_�[�c�{�[�h����]������
			for (int i = 0; i < 10; i++) {
				dartBoardPoint[i]->SetRotationX(DARTBOARD_ROTATION);
			}
			
			// START_JUDGEMENT_X_POINT�̒l�܂ł͓����蔻������Ȃ��悤�ɂ��Ă���
			// �_�[�c���{�[�h�ɓ������Ă���ꍇ�͔��肵�Ȃ��悤�ɂ���
			if (xDart < START_JUDGEMENT_X_POINT && !dart[threwCount]->getHitDartBoard()) {

				// �_�[�c���{�[�h�ɓ����������ǂ����𔻒肷��
				// �_�[�c���{�[�h�ɓ������Ă��鎞�̏���
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

					// �{�[�h�ɓ��������͂��߂�1�񂾂���������镔��
					if (!dart[threwCount]->getHitDartBoard()) {

						// �����������̍��W���擾����
						dart[threwCount]->GetPosition(&xDart, &yDart, &zDart);
						dart[threwCount]->setHitPoint(xDart, yDart, zDart);
						// �����������̏�����]�p���v�Z����
						dart[threwCount]->setAngle(-atan2(yDart, zDart));
						dart[threwCount]->setBeforeAngle(-atan2(yDart, zDart));
						
						// �O��̂̓��_������
						g.Unregister(pointArray[lastPoint]);
						g.Unregister(totalPointArray[lastTotalPoint]);

						// �{�[�h�̂ǂ��Ƀ_�[�c�������������𔻒肷��
						for (int i = 0; i < 10; i++) {
							if (dart[threwCount]->whereToHitDartBoard(hitAreaArray[i])) {
								cout << "Hit " << i << "point zone" << endl;
								point = i;
							}
						}

						// ���v�|�C���g���v�Z����
						totalPoint += point;

						// ���������|�C���g��ޔ����Ă���
						lastPoint = point;
						lastTotalPoint = totalPoint;

						// �O�ɓ��������|�C���g�̕\�����猻�݂̃|�C���g�\���ɕύX����
						g.Register(pointArray[point]);
						g.Register(totalPointArray[totalPoint]);

					}// �{�[�h�ɓ��������͂��߂�1�񂾂���������镔���I��
					
					// �_�[�c���{�[�h�ɓ��������t���O�𗧂Ă�
					dart[lastThrewCount]->setHitDartBoard(true);
					
				} else {// �_�[�c���{�[�h�ɓ������Ă��Ȃ����̏���
					// �_�[�c���{�[�h�ɓ������Ă��Ȃ��t���O�𗧂Ă�
					dart[threwCount]->setHitDartBoard(false);
				}
				
			}// START_JUDGEMENT_X_POINT��if���I��

			// ��O�̐��l���i�[���Ă���
			lastThrewCount = threwCount;
			if (dart[threwCount]->calcDeltaAngle() > DART_ROTATION) {

				// �Q�[���I���܂ł̃J�E���g��+1����
				gameoverCount++;
				
				if (threwCount < NUMBER_OF_PLAYS - 1) {
					// �������񐔂�+1����
					threwCount++;	
				}
			}
			// �������񐔂�\��������
			g.Unregister(threwNumberArray[lastThrewCount]);
			g.Register(threwNumberArray[threwCount]);
			
			// �_�[�c�𓮂���
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

			// �g�̉f���̐؂蔲�����s��
			bg_subtract(&mainScreen, &stored, &source, 0x20202020);
			
			//for debug(2/2)
			//debug = hitArea;
			//arsgd.Draw(&debug);
			
		} else {// 3�񓊂����ꍇ�̏���

			// Game Over��ʂ�\��������
			g.Register(&gameover);

			// A�{�^�������������̏���
			if (keyIn->GetKeyTrig('A')) {

				g.Unregister(&gameover);
				
				resetDisplay(&point, &totalPoint, &threwCount, &lastPoint, &lastTotalPoint, &lastThrewCount, &g, dart, totalPointArray, pointArray, threwNumberArray);
				reset(&point, &totalPoint, &threwCount, &lastPoint, &lastTotalPoint, &lastThrewCount, &gameoverCount, dart);
			}

			if (keyIn->GetKeyTrig('Q')) {
				break;
			}
			
		}// 3�񓊂����ꍇ�̏����I��

		g.Draw();// ��ʂ�`�悷��
		
	}// �A�j���[�V�������䕔���I��
	
	// ���I�Ɋm�ۂ������������������
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

	// ���ꂼ��̃_�[�c�̏�������������
	for (int i = 0; i < NUMBER_OF_PLAYS; i++) {
		dart[i]->setHitHand(false);
		dart[i]->setHitDartBoard(false);
		dart[i]->setHitPoint(0.0f, 0.0f, 0.0f);
		dart[i]->setAngle(0.0f);
		dart[i]->SetPosition(START_X_POINT, START_Y_POINT, START_Z_POINT);
	}
	
	// ���ꂼ��̐���ϐ�������������
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

	// �������_�[�c���\���ɂ���
	g->Unregister(dart[1]);
	g->Unregister(dart[2]);
	
	// ���܂ŕ\�����Ă������_�\�����\���ɂ���
	g->Unregister(totalPointArray[*totalPoint]);
	g->Unregister(pointArray[*point]);
	g->Unregister(threwNumberArray[*threwCount]);

	// ���_�\����0�ɖ߂�
	g->Register(pointArray[0]);
	g->Register(totalPointArray[0]);
	g->Register(threwNumberArray[0]);
}

inline void Dart::react(Texture* _hitArea)
{
	int gx,gy;
	bool overlapping = get_overlapping_center(_hitArea, &gx, &gy,100);

	// �肪�_�[�c�ɐڐG�������ǂ����𔻒肷��
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
	
	// �肪�_�[�c�ɓ���������_�[�c�𓮂���
	if (hitHand) {
		
		//�g�̔���
		if (c.x < 200 || c.x > sizex){
			// vx *= -1.0f;
			vx = 0;
		}
		if (c.y > sizey-50 && vy<0){
			vy *= -1.0f;
		}

		//���R�����܂��͒�~
		if (c.y > sizey-50 && vy<0.03f) 
			vy = 0;
		else
			vy -= 0.03f;

		//��C��R
		vx *= 0.8f;
		vy *= 0.8f;

		// �_�[�c�{�[�h�ɓ������Ă���Ƃ��̏���
		if (hitDartBoard) {

			// �_�[�c�{�[�h�̒��S�Ɠ��������Ƃ���̒������v�Z����
			r = sqrt(hitYPoint * hitYPoint + START_Z_POINT *START_Z_POINT);
			
			// �~�^��������
			SetPosition(-6.5f, r * (-sin(angle)), r * cos(angle) + START_Z_POINT);

			// ��]�p�𑝂₷
			angle += 0.05f;
			
		} else {// �_�[�c�{�[�h�ɓ������Ă��Ȃ������炻�̂܂ܕ��ʂɓ�����
			SetPosition(vx, vy, 0.0f, GL_RELATIVE);
		}
		
	} else {// �肪�_�[�c�ɓ������Ă��Ȃ��ꍇ�͓������Ȃ�

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
		
	//�g�̔���
	if (c.x < 0 || c.x > sizex)	vx *= -1.0f;
	if (c.y > sizey-50 && vy<0)	vy *= -1.0f;

	//���R�����܂��͒�~
	if (c.y > sizey-50 && vy<0.03f) 
		vy = 0;
	else
		vy -= 0.03f;

	//��C��R
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
