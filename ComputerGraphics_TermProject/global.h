#pragma once

#include <gl/freeglut.h>
#include <iostream>
#include <random>
#include <math.h>
#include <time.h>
#include <typeinfo>
#include <fstream>
#include <math.h>
#include <float.h>
#include "Board.h"
#include "Player.h"


#define SOUND_FILE_OPENING "Opening.wav"
#define SOUND_FILE_BOARDCRUSH "BoardCrush.wav"
#define SOUND_FILE_CLICKSOUND "ClickSound.wav"
#define SOUND_FILE_PLAYERDEAD "Dead.wav"
#define MAX_OBJECTS 100
#define WIDTH_SIZE 120
#define DEPTH_SIZE 200
#define BOARD_CON 5
#define width 800
#define height 600
#define PI 3.141592
#define MAX_CIRCLE 5
#define MAX_CUBE 3
#define RADIUS 1000

using namespace std;

static int g_Stage = 1;


