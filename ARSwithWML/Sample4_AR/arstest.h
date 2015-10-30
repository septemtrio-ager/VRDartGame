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

// develop branch file

class Touchable : public Mesh{
// private:
protected:
   float vx,vy;	

   bool overlappingOnce;

    // bool get_overlapping_center(Texture* hitArea, int *gx, int *gy, unsigned int threshold);
   void state_action(bool condition);
public:   
   static enum {ACTIVE,INACTIVE};
   int state;

   Touchable(ARSG* _g, wchar_t fln[]) : Mesh(_g,fln), vx(0.2f),vy(-0.2f),state(ACTIVE),overlappingOnce(false){ }
   Touchable(void) : vx(0.2f),vy(-0.2f),state(ACTIVE){ }
   bool get_overlapping_center(Texture* hitArea, int *gx, int *gy, unsigned int threshold);
   bool whereToHitDartBoard(Texture * hitAreaMask);
   void react(Texture *hitArea);
   void move(void);
   void setOverlappingOnce(bool tf){
	   overlappingOnce = tf;
   }
};

class Dart : public Touchable {
protected:
	bool hitDartBoard;
	bool hitOnce;
	float xDart = 0;
	float yDart = 0;
	int overlappingCount = 0;
	int threwCount = 0;

public:
Dart(ARSG* _g, wchar_t fln[]):Touchable(_g, fln),hitDartBoard(false), hitOnce(false){}
	void react(Texture* hitArea);
	void move(void);
	void setHitDartBoard(bool hdb) {
		hitDartBoard = hdb;
	}
	bool getHitDartBoard() {
		return hitDartBoard;
	}
	void setYDart(float yd) {
		yDart = yd;
	}
	bool getHitOnce() {
		return hitOnce;
	}
	void setHitOnce(bool ho) {
		hitOnce = ho;
	}
	int getOverlappingCount() {
		return overlappingCount;
	}
	void setOverlappingCount(int olc) {
		overlappingCount = olc;
	}
	int getThrewCount() {
		return threwCount;
	}
	void setThrewCount(int tc) {
		threwCount = tc;
	}
};

class DartBoard : public Touchable {
protected:
	bool hitDart;

public:
	DartBoard(ARSG* _g, wchar_t fln[]) : Touchable(_g, fln), hitDart(false) {}
	void react(Texture* hitArea);
	void move(void);
	bool getHitDart() {
		return hitDart;
	}
	void setHitDart(bool hd) {
		hitDart = hd;
	}

};
