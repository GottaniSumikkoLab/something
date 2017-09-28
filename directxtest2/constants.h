//constants.h

// Programming 2D Games
// Copyright (c) 2011 by: 
// Charles Kelly
// Chapter 3 constants.h v1.0

#ifndef _CONSTANTS_H            // prevent multiple definitions if this 
#define _CONSTANTS_H            // ..file is included in more than one place
#define WIN32_LEAN_AND_MEAN
#define CLASS_NAME "DirectX9 API test"

#include<windows.h>

//-----------------------------------------------
// Useful macros
//-----------------------------------------------
// Safely delete pointer referenced item
#define SAFE_DELETE(ptr)       { if (ptr) { delete (ptr); (ptr)=NULL; } }
// Safely release pointer referenced item
#define SAFE_RELEASE(ptr)      { if(ptr) { (ptr)->Release(); (ptr)=NULL; } }
// Safely delete pointer referenced array
#define SAFE_DELETE_ARRAY(ptr) { if(ptr) { delete [](ptr); (ptr)=NULL; } }

#define SAFE_ON_LOST_DEVISE(ptr) {if(ptr){ptr->onLostDevice();}}

#define SAFE_ON_RESET_DEVICE(ptr) {if(ptr){ptr->onResetDevice();}}

#define TRANSCOLOR SETCOLOR_ARGB(0,255,255,255)
//-----------------------------------------------
//                  Constants
//-----------------------------------------------

//graphics images
const char BACKSCREEN_IMAGE[] = "pictures\\blackboard.bmp";
const char FOXTYANN_IMAGE[] = "pictures\\foxtyann.png";
const float FADEOUT_TIME = 3.0f;//second

//text
const char FONT[] = "Courier New";
const int POINT_SIZE = 20;

// window
const bool FULLSCREEN = false;              // windowed or FULLSCREEN
const UINT  GAME_WIDTH = 1280;              // width of game in pixels
const UINT  GAME_HEIGHT = 720;              // height of game in pixels
											// key mappings
											// In this game simple constants are used for key mappings. If variables were used
											// it would be possible to save and restore key mappings from a data file.
//game
const double PI = 3.14159265;
const float FRAME_RATE = 60.0f;//frame per second
const float MIN_FRAME_RATE = 20.0f;//min frame per second
const float MIN_FRAME_TIME = 1.0f / FRAME_RATE;//target second
const float MAX_FRAME_TIME = 1.0f / MIN_FRAME_RATE;//max second

//仮想キーコード
const UCHAR ESC_KEY = VK_ESCAPE;      // escape key
const UCHAR ALT_KEY = VK_MENU;     // Alt key
const UCHAR ENTER_KEY = VK_RETURN;   // Enter key

#endif
