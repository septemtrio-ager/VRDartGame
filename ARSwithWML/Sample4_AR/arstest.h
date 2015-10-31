#pragma once

#ifndef _DEBUG
#pragma comment(lib, "../lib/ARS.lib")
#pragma comment(lib, "../lib/WML.lib")
#else
#pragma comment(lib, "../lib/ARSd.lib")
#pragma comment(lib, "../lib/WMLd.lib")
#endif



#define _CRT_SECURE_NO_WARNINGS

#define CLASSNAME "ARSTEST"
#define APPNAME "ARSTEST"

// new design branch

class Touchable : public Mesh{
protected:
   float vx,vy;	
   bool get_overlapping_center(Texture* hitArea, int *gx, int *gy, unsigned int threshold);
   void state_action(bool condition);
public:   
   static enum {ACTIVE,INACTIVE};
   int state;

   Touchable(ARSG* _g, wchar_t fln[]) : Mesh(_g,fln), vx(0.2f),vy(-0.2f),state(ACTIVE){ }
   Touchable(void) : vx(0.2f),vy(-0.2f),state(ACTIVE){ }
   void react(Texture *hitArea);
   void move(void);
   

};

class Dart:public Touchable {
protected:
	// ダーツボードに当たった時の座標を格納する
	float hitXPoint;
	float hitYPoint;
	float hitZPoint;
	// 手に当たったかどうかを格納する
	bool hitHand;
	// ボードに当たったかどうかを格納する
	bool hitDartBoard;
	// ダーツを回転させるときの回転角
	float angle;
	float beforeAngle;
	float afterAngle;

public:
	
Dart(ARSG* _g, wchar_t fln[]):Touchable(_g, fln),hitXPoint(0.0f), hitYPoint(0.0f), hitZPoint(0.0f),
		hitHand(false), hitDartBoard(false), angle(0.0f){}

	void react(Texture *hitArea);
	void move(void);
	bool whereToHitDartBoard(Texture *hitAreaMask);

	float getHitXPoint() {
		return hitXPoint;
	}
	void setHitXPoint(float hxp) {
		hitXPoint = hxp;
	}

	float getHitYPoint() {
		return hitYPoint;
	}
	void setHitYPoint(float hyp) {
		hitYPoint = hyp;
	}

	float getHitZPoint() {
		return hitZPoint;
	}
	void setHitZPoint(float hzp) {
		hitZPoint = hzp;
	}

	void setHitPoint(float hxp, float hyp, float hzp) {
		hitXPoint = hxp;
		hitYPoint = hyp;
		hitZPoint = hzp;
	}

	bool getHitHand() {
		return hitHand;
	}
	void setHitHand(bool hh) {
		hitHand = hh;
	}

	bool getHitDartBoard() {
		return hitDartBoard;
	}
	void setHitDartBoard(bool hdb) {
		hitDartBoard = hdb;
	}

	float getAngle() {
		return angle;
	}
	void setAngle(float a) {
		angle = a;
	}

	float getBeforeAngle() {
		return beforeAngle;
	}
	void setBeforeAngle(float ba) {
		beforeAngle = ba;
	}

	float getAfterAngle() {
		return afterAngle;
	}
	void setAfterAngle(float aa) {
		afterAngle = aa;
	}

	float calcDeltaAngle() {
		return (angle - beforeAngle);
	}
		
};

class DartBoard:public Touchable {
protected:

public:
DartBoard(ARSG* _g, wchar_t fln[]) : Touchable(_g, fln) {}
	/* void react(Texture* hitArea); */
	/* void move(void); */
};
