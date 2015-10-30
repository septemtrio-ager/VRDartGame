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
private:
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
