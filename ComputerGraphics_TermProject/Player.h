#pragma once

#include "global.h"

class Player
{
public:
	Player();
	~Player();

private:
	float m_Xpos;
	float m_Ypos;
	float m_Zpos;

	float m_velX;
	float m_velZ;

	float m_accelX;
	float m_accelZ;

	float m_forceX;
	float m_forceZ;

	float m_Mass;
	float m_Friction;

	float m_Lleg_rot;
	float m_Rleg_rot;
	float m_Larm_rot;
	float m_Rarm_rot;

	float m_red;
	float m_green;
	float m_blue;

	float m_rotDir;

	int m_leg_dir;
	int m_arm_dir;

	int m_score;

	bool b_Dead = false;
	bool b_moving = false;

public:
	void GetPosition(float* x, float* y, float* z);
	void SetPosition(float x, float y, float z);

	void GetVelocity(float* x, float* z);
	void SetVelocity(float x, float z);

	void GetAccel(float* x, float* z);
	void SetAccel(float x, float z);

	void GetForce(float* x, float* y);
	void SetForce(float x, float y);

	void GetMass(float* m);
	void SetMass(float m);

	void GetFriction(float* x);
	void SetFriction(float x);

	void GetLegRot(float* r, float* l);
	void SetLegRot(float r, float l);

	void GetArmRot(float* r, float* l);
	void SetArmRot(float r, float l);

	void GetLegDir(int* n);
	void SetLegDir(int n);

	void GetArmDir(int* n);
	void SetArmDir(int n);

	void GetColor(float* r, float*g, float*b);
	void SetColor(float r, float g, float b);

	float GetRotDir();
	void SetRotDir(float n);

	void ApplyForce(float x, float z);

	void Update();

	void SetState();
	bool GetState();

	bool GetDead();
	void SetDead(bool n);

	void SetMove(bool n);
	bool GetMove();

	int GetScore();
	void SetScore(int n);

	void Draw();
	void Animate();
};

