#include "Board.h"

Pattern Board::pattern[18] = { 0 };

Board::Board()
{
}


Board::~Board()
{
}

void Board::Update()
{
	if (b_Ready)
	{
		if (Rotate < 180.f)
		{
			if (Rotate >= 90.f)
			{
				b_DrawShadow = true;
				Shadow_Update();
			}

			Rotate += 5.f;
		}
		else
		{
			b_Collision = true;
			PlaySound(TEXT(SOUND_FILE_BOARDCRUSH), NULL, SND_FILENAME | SND_ASYNC);
		}

	}

}

void Board::SetPos(float x, float y, float z)
{
	Xpos = x;
	Ypos = y;
	Zpos = z;
}

void Board::GetPos(float* x, float* y, float* z)
{
	*x = Xpos;
	*y = Ypos;
	*z = Zpos;
}

float Board::GetRotate()
{
	return Rotate;
}

void Board::SetReady()
{
	b_Ready = true;
}

void Board::SetState()
{
	b_Collision = true;
}

bool Board::GetReady()
{
	return b_Ready;
}

bool Board::GetState()
{
	return b_Collision;
}

void Board::Draw()
{
	glPushMatrix();
	{
		glTranslatef(0.f, 0.f, -500.f);
		glRotatef(Rotate, 1.f, 0.f, 0.f);

		for (int i = 0; i < 5; ++i)
		{
			for (int j = 0; j < 5; ++j)
			{
				if (m_Hole.m_pattern[i][j] == 0)
				{
					glPushMatrix();
					{
						glColor3f(0.f, 1.f, 0.f);
						glBegin(GL_QUADS);		// x축 -300 ~ 300, z축 -500 ~ 500
						glVertex3f((j - 2.5) * WIDTH_SIZE, 0.f, (i - 2.5) * DEPTH_SIZE - 500.f);
						glVertex3f((j - 1.5) * WIDTH_SIZE, 0.f, (i - 2.5) * DEPTH_SIZE - 500.f);
						glVertex3f((j - 1.5) * WIDTH_SIZE, 0.f, (i - 1.5) * DEPTH_SIZE - 500.f);
						glVertex3f((j - 2.5) * WIDTH_SIZE, 0.f, (i - 1.5) * DEPTH_SIZE - 500.f);
						glEnd();
					}
					glPopMatrix();

					glPushMatrix();
					{
						glColor3f(0.f, 0.f, 0.f);
						glLineWidth(10.f);

						glBegin(GL_LINES);
						glVertex3f((j - 2.5) * WIDTH_SIZE, 0.f, (i - 2.5) * DEPTH_SIZE - 500.f);
						glVertex3f((j - 2.5) * WIDTH_SIZE, 0.f, (i - 1.5) * DEPTH_SIZE - 500.f);
						glEnd();

						glBegin(GL_LINES);
						glVertex3f((j - 1.5) * WIDTH_SIZE, 0.f, (i - 2.5) * DEPTH_SIZE - 500.f);
						glVertex3f((j - 1.5) * WIDTH_SIZE, 0.f, (i - 1.5) * DEPTH_SIZE - 500.f);
						glEnd();

						glBegin(GL_LINES);
						glVertex3f((j - 2.5) * WIDTH_SIZE, 0.f, (i - 2.5) * DEPTH_SIZE - 500.f);
						glVertex3f((j - 1.5) * WIDTH_SIZE, 0.f, (i - 2.5) * DEPTH_SIZE - 500.f);
						glEnd();

						glBegin(GL_LINES);
						glVertex3f((j - 2.5) * WIDTH_SIZE, 0.f, (i - 1.5) * DEPTH_SIZE - 500.f);
						glVertex3f((j - 1.5) * WIDTH_SIZE, 0.f, (i - 1.5) * DEPTH_SIZE - 500.f);
						glEnd();

					}
					glPopMatrix();
				}
			}
		}
	}
	glPopMatrix();
}

void Board::SetHole()
{
	//랜드값 받아서 패턴 index로 접근해서 패턴 얻는걸로 할 예정.
	Hole_Level = rand() % 18;

	for (int i = 0; i < 5; ++i)
	{
		for (int j = 0; j < 5; ++j)
		{
				m_Hole.m_pattern[i][j] = pattern[Hole_Level].m_pattern[i][j];
			if (m_Hole.m_pattern[i][j] == 0)
				m_shadow[4-i][j].b_real = true;
		}
	}
}

void Board::GetPattern(Pattern* p)
{
	*p = m_Hole;
}

void Board::SetShadow()
{
	b_DrawShadow = true;
}

bool Board::GetShadow()
{
	return b_DrawShadow;
}

void Board::ShadowInit()
{
	for (int i = 0; i < 5; ++i)
	{
		for (int j = 0; j < 5; ++j)
		{
			if (m_shadow[i][j].b_real)
			{
				m_shadow[i][j].m_xpos = -240.f + (j * WIDTH_SIZE);
				m_shadow[i][j].max_zpos = -400.f + (i * DEPTH_SIZE);
				if (i == 0)
					m_shadow[i][j].m_velocity = 0;
				else if (i == 1)
					m_shadow[i][j].m_velocity = 13;
				else if (i == 2)
					m_shadow[i][j].m_velocity = 23;
				else if (i == 3)
					m_shadow[i][j].m_velocity = 36;
				else if (i == 4)
					m_shadow[i][j].m_velocity = 44;
			}
		}
	}
}

void Board::Shadow_Update()
{
	for (int i = 0; i < 5; ++i)
	{
		for (int j = 0; j < 5; ++j)
		{
			if (m_shadow[4 - i][j].b_real)
			{
				if (m_shadow[4 - i][j].m_zpos < m_shadow[4 - i][j].max_zpos)
					m_shadow[4 - i][j].m_zpos += m_shadow[4 - i][j].m_velocity;
				else
					m_shadow[4 - i][j].m_zpos = m_shadow[4 - i][j].max_zpos;
			}
		}
	}
}

void Board::Shadow_Draw()
{
	if (Rotate >= 90.f)
	{
		for (int i = 0; i < 5; ++i)
		{
			for (int j = 0; j < 5; ++j)
			{
				if (m_shadow[4-i][j].b_real)
				{
					glPushMatrix();
					{
					glColor4f(0.f, 0.f, 0.f, 0.6f);
					glBegin(GL_QUADS);
					{
						glVertex3f(m_shadow[4 - i][j].m_xpos - 58.f, 5.f, m_shadow[4 - i][j].m_zpos - 98.f);
						glVertex3f(m_shadow[4 - i][j].m_xpos - 58.f, 5.f, m_shadow[4 - i][j].m_zpos + 98.f);
						glVertex3f(m_shadow[4 - i][j].m_xpos + 58.f, 5.f, m_shadow[4 - i][j].m_zpos + 98.f);
						glVertex3f(m_shadow[4 - i][j].m_xpos + 58.f, 5.f, m_shadow[4 - i][j].m_zpos - 98.f);
					}
					glEnd();
					}
					glPopMatrix();
				}
			}
		}
	}
}