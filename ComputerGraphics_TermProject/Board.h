#pragma once

#include "global.h"
#pragma comment(lib, "winmm")
#include <playsoundapi.h>
#include <mmsystem.h>

typedef struct Pattern
{
	int m_pattern[5][5];
};

typedef struct Shadow
{
	float m_xpos = 0.f;
	float m_ypos = 2.f;
	float m_zpos = -400.f;

	float max_zpos;

	float m_velocity;

	bool b_real = false;
};


class Board
{

private:
	float Xpos;
	float Ypos;
	float Zpos;
	float Rotate;
	bool b_Ready = false;
	bool b_Collision = false;
	int Hole[5][5] = { 0 };
	int Hole_Level = 0;
	Pattern m_Hole;
	Shadow m_shadow[5][5];
	bool b_DrawShadow = false;

public:
	Board();
	Board(float x, float y, float z, float r) : Xpos(x), Ypos(y), Zpos(z), Rotate(r)
	{
		Xpos = x;
		Ypos = y;
		Zpos = z;
		Rotate = r;
	}
	~Board();

public:
	static Pattern pattern[18];

public:
	void GetPos(float*, float*, float*);
	void SetPos(float, float, float);
	float GetRotate();
	void SetReady();
	void SetState();
	bool GetReady();
	bool GetState();
	void SetHole();
	void GetPattern(Pattern*);
	void SetShadow();
	bool GetShadow();

public:
	void Update();
	void Draw();
	void ShadowInit();
	void Shadow_Update();
	void Shadow_Draw();
};
