
#include "Player.h"

Player::Player()
{
}


Player::~Player()
{
}

void Player::GetPosition(float* x, float* y, float* z)
{
	*x = m_Xpos;
	*y = m_Ypos;
	*z = m_Zpos;
}

void Player::SetPosition(float x, float y, float z)
{
	m_Xpos = x;
	m_Ypos = y;
	m_Zpos = z;
}

void Player::GetVelocity(float* x, float* z)
{
	*x = m_velX;
	*z = m_velZ;
}

void Player::SetVelocity(float x, float z)
{
	m_velX = x;
	m_velZ = z;
}

void Player::GetAccel(float* x, float* z)
{
	*x = m_accelX;
	*z = m_accelZ;
}

void Player::SetAccel(float x, float z)
{
	m_accelX = x;
	m_accelZ = z;
}

void Player::GetForce(float* x, float* z)
{
	*x = m_forceX;
	*z = m_forceZ;
}

void Player::SetForce(float x, float z)
{
	m_forceX = x;
	m_forceZ = z;
}

void Player::GetMass(float* m)
{
	*m = m_Mass;
}

void Player::SetMass(float m)
{
	m_Mass = m;
}

void Player::GetFriction(float* x)
{
	*x = m_Friction;
}

void Player::SetFriction(float x)
{
	m_Friction = x;
}

void Player::GetLegRot(float* r, float* l)
{
	*r = m_Rleg_rot;
	*l = m_Lleg_rot;
}

void Player::SetLegRot(float r, float l)
{
	m_Rleg_rot = r;
	m_Lleg_rot = l;
}

void Player::GetArmRot(float* r, float* l)
{
	*r = m_Rarm_rot;
	*l = m_Larm_rot;
}

void Player::SetArmRot(float r, float l)
{
	m_Rarm_rot = r;
	m_Larm_rot = l;
}

void Player::GetArmDir(int* n)
{
	*n = m_arm_dir;
}

void Player::SetArmDir(int n)
{
	m_arm_dir = n;
}

void Player::GetLegDir(int* n)
{
	*n = m_leg_dir;
}

void Player::SetLegDir(int n)
{
	m_leg_dir = n;
}

float Player::GetRotDir()
{
	return m_rotDir;
}

void Player::SetRotDir(float n)
{
	m_rotDir = n;
}

int Player::GetScore()
{
	return m_score;
}

void Player::SetScore(int n)
{
	m_score = n;
}

bool Player::GetDead()
{
	return b_Dead;
}

void Player::SetDead(bool n)
{
	b_Dead = n;
}

void Player::ApplyForce(float x, float z)
{
	m_accelX = x / m_Mass;
	m_accelZ = z / m_Mass;

	m_velX = m_velX + m_accelX;
	m_velZ = m_velZ + m_accelZ;

	m_accelX = 0.f;
	m_accelZ = 0.f;
}

void Player::Update()
{
	float gz = m_Mass * 9.8;
	float friction = m_Friction * gz;

	float VelMag = sqrtf(m_velX*m_velX + m_velZ * m_velZ);

	if (VelMag < FLT_EPSILON)
	{
		m_velX = 0.f;
		m_velZ = 0.f;
	}
	else
	{
		float fx = -friction * m_velX / VelMag;
		float fz = -friction * m_velZ / VelMag;

		float accX = fx / m_Mass;
		float accZ = fz / m_Mass;

		float newVelX = m_velX + accX;
		float newVelZ = m_velZ + accZ;

		if (newVelX*m_velX < 0.f)
		{
			m_velX = 0.f;
		}
		else
		{
			m_velX = newVelX;
		}
		if (newVelZ*m_velZ < 0.f)
		{
			m_velZ = 0.f;
		}
		else
		{
			m_velZ = newVelZ;
		}

		m_velX = m_velX + m_accelX;
		m_velZ = m_velZ + m_accelZ;

		m_Xpos = m_Xpos + m_velX;
		m_Zpos = m_Zpos + m_velZ;
		if (m_Xpos >= 280 )
			m_Xpos = 280;
		if (m_Xpos <= -280)
			m_Xpos = -280;
		if (m_Zpos >= 480)
			m_Zpos = 480;
		if (m_Zpos <= -480)
			m_Zpos = -480;
	}
}

void Player::Draw()
{
	glPushMatrix();
	{
		//glTranslatef(0.f, 50.f, 0.f);
		// ¾ó±¼
		glPushMatrix();
		{
			glColor3f(0.9, 0.8, 0.6);
			glutSolidSphere(20, 20, 20);
			glPushMatrix();
			{
				glTranslatef(0, 0, 20);
				glColor3f(0.3, 0.3, 0.3);
				glutSolidSphere(5, 10, 10);
			}
			glPopMatrix();
		}
		glPopMatrix();

		// ¸öÅë
		glPushMatrix();
		{
			glTranslatef(0, -40, 0);
			glScalef(0.8, 1, 0.75);
			glColor3f(m_red, m_green, m_blue);
			glutSolidSphere(30, 30, 30);
		}
		glPopMatrix();

		// ÆÈ
		glPushMatrix();
		{
			// ¿À¸¥ÆÈ
			glPushMatrix();
			{
				glRotatef(m_Larm_rot, 1, 0, 0);
				glPushMatrix();
				{
					glTranslatef(30, -20, 0);
					glRotatef(90, 1, 0, 0);
					glScalef(1, 1, 1);
					glColor3f(0, 0, 1);
					glutSolidCylinder(10, 30, 10, 10);
				}
				glPopMatrix();
				glPushMatrix();
				{
					glTranslatef(30, -50, 0);
					glRotatef(55, 1, 0, 0);
					glScalef(1, 0.8, 1);
					glColor3f(0, 0, 1);
					glutSolidCylinder(10, 35, 10, 10);
				}
				glPopMatrix();
			}
			glPopMatrix();
			// ¿ÞÆÈ
			glPushMatrix();
			{
				glRotatef(m_Rarm_rot, 1, 0, 0);
				glPushMatrix();
				{
					glTranslatef(-30, -20, 0);
					glRotatef(90, 1, 0, 0);
					glScalef(1, 1, 1);
					glColor3f(0, 0, 1);
					glutSolidCylinder(10, 30, 10, 10);
				}
				glPopMatrix();
				glPushMatrix();
				{
					glTranslatef(-30, -50, 0);
					glRotatef(55, 1, 0, 0);
					glScalef(1, 0.8, 1);
					glColor3f(0, 0, 1);
					glutSolidCylinder(10, 35, 10, 10);
				}
				glPopMatrix();
			}
			glPopMatrix();
		}
		glPopMatrix();

		// ´Ù¸®
		glPushMatrix();
		{
			glTranslatef(0, -40, 0);
			// ¿À¸¥ÂÊ ´Ù¸®
			glPushMatrix();
			{
				glRotatef(m_Rleg_rot, 1, 0, 0);
				glPushMatrix();
				{
					glTranslatef(-15, -30, 0);
					glRotatef(90, 1, 0, 0);
					glScalef(1, 1, 1);
					glColor3f(1, 0, 1);
					glutSolidCylinder(10, 50, 10, 10);
				}
				glPopMatrix();
			}
			glPopMatrix();

			// ¿ÞÂÊ ´Ù¸®
			glPushMatrix();
			{
				glRotatef(m_Lleg_rot, 1, 0, 0);
				glPushMatrix();
				{
					glTranslatef(15, -30, 0);
					glRotatef(90, 1, 0, 0);
					glScalef(1, 1, 1);
					glColor3f(1, 0, 1);
					glutSolidCylinder(10, 50, 10, 10);
				}
				glPopMatrix();
			}
			glPopMatrix();
		}
		glPopMatrix();
	}
	glPopMatrix();
}

void Player::Animate()
{
	if (m_leg_dir == 0)
	{
		m_Lleg_rot -= 5;
		m_Rleg_rot += 5;
		if (m_Lleg_rot <= -15 && m_Rleg_rot >= 15)
			m_leg_dir = 1;
	}
	if (m_arm_dir == 0)
	{
		m_Larm_rot += 5;
		m_Rarm_rot -= 5;
		if (m_Larm_rot >= 15 && m_Rarm_rot <= -15)
			m_arm_dir = 1;
	}
	if (m_leg_dir == 1)
	{
		m_Lleg_rot += 5;
		m_Rleg_rot -= 5;
		if (m_Lleg_rot >= 15 && m_Rleg_rot <= -15)
			m_leg_dir = 0;
	}
	if (m_arm_dir == 1)
	{
		m_Larm_rot -= 5;
		m_Rarm_rot += 5;
		if (m_Rarm_rot >= 15 && m_Larm_rot <= -15)
			m_arm_dir = 0;
	}
	
}

void Player::GetColor(float* r, float* g, float* b)
{
	*r = m_red;
	*g = m_green;
	*b = m_blue;
}

void Player::SetColor(float r, float g, float b)
{
	m_red = r;
	m_green = g;
	m_blue = b;
}

void Player::SetState()
{
	b_Dead = true;
}

bool Player::GetState()
{
	return b_Dead;
}

void Player::SetMove(bool n)
{
	b_moving = n;
}

bool Player::GetMove()
{
	return b_moving;
}