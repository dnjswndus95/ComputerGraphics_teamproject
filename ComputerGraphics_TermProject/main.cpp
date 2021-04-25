#include "global.h"
#pragma comment(lib, "winmm")
#include <playsoundapi.h>
#include <mmsystem.h>

Board* board[MAX_OBJECTS];

GLvoid drawScene(GLvoid);
GLvoid Reshape(int w, int h);
void TimerFunction(int);
void KeyBoard(unsigned char key, int x, int y);
void KeyboardUp(unsigned char key, int x, int y);
void Mouse(int button, int state, int x, int y);
void ReadPattern();
void InitPlayer_one();
void InitPlayer_two();
void BitmapInit();
void IntroInit();
void GameOverInit();
void Collision_Check(Player*, Board*);
bool Collision_Check_Player(Player*, Player*);

int g_BoardIndex = 0;
int g_GameTime = 0;
bool game_start = false;
bool game_dead = false;
bool one_player = false;
float g_x, g_y, g_z = 0.f;
float g_xpos, g_ypos = 0.f;
bool w_key, a_key, s_key, d_key = false;
bool i_key, j_key, k_key, l_key = false;

Player* m_player;
Player* m_player_second;

float player_rot = 0.f;
GLubyte *pBytes; // 데이터를 가리킬 포인터
BITMAPINFO *info; // 비트맵 헤더 저장할 변수
GLuint texture_object[4];
GLubyte * LoadDIBitmap(const char *filename, BITMAPINFO **info);

int main(int argc, char *argv[])
{
	glutInit(&argc, argv);
	PlaySound(TEXT(SOUND_FILE_OPENING), NULL, SND_FILENAME | SND_ASYNC);
	ReadPattern();
	for (int i = 0; i < MAX_OBJECTS; ++i)
	{
		board[i] = new Board(0.f, 0.f, -500.f, 30.f);
		board[i]->SetHole();
		board[i]->ShadowInit();
	}

	glutInitDisplayMode(GLUT_SINGLE | GLUT_RGBA); //디스플레이 모드 설정
	glutInitWindowPosition(100, 100);             //윈도우 위치지정
	glutInitWindowSize(800, 600);                 //윈도우의 크기 지정
	glutCreateWindow("Example");				  //윈도우 생성
	srand(unsigned(time(NULL)));
	BitmapInit();
	glutDisplayFunc(drawScene);                   //출력 함수의 지정
	glutReshapeFunc(Reshape);                     //다시 그리기 함수의 지정
	glutKeyboardFunc(KeyBoard);
	glutKeyboardUpFunc(KeyboardUp);
	glutMouseFunc(Mouse);
	glutTimerFunc(100, TimerFunction, 1);         //타이머 함수 설정 
	glutMainLoop();

	return 0;
}

void BitmapInit()
{
	glGenTextures(4, texture_object);

	glBindTexture(GL_TEXTURE_2D, texture_object[0]);   //텍스처와 객체를 결합한다.
	pBytes = LoadDIBitmap("intro.bmp", &info);   //이미지 로딩을 한다
	glTexImage2D(GL_TEXTURE_2D, 0, 3, 600, 337, 0, GL_BGR_EXT, GL_UNSIGNED_BYTE, pBytes);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	// 텍스처 모드 설정 
	glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_COLOR, GL_MODULATE);
	// 텍스처 매핑 활성화
	//glEnable(GL_TEXTURE_2D);

	glBindTexture(GL_TEXTURE_2D, texture_object[1]);   //텍스처와 객체를 결합한다.
	pBytes = LoadDIBitmap("1p.bmp", &info);   //이미지 로딩을 한다
	glTexImage2D(GL_TEXTURE_2D, 0, 3, 243, 238, 0, GL_BGR_EXT, GL_UNSIGNED_BYTE, pBytes);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	// 텍스처 모드 설정 
	glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_COLOR, GL_MODULATE);
	// 텍스처 매핑 활성화
	//glEnable(GL_TEXTURE_2D);

	glBindTexture(GL_TEXTURE_2D, texture_object[2]);   //텍스처와 객체를 결합한다.
	pBytes = LoadDIBitmap("2p.bmp", &info);   //이미지 로딩을 한다
	glTexImage2D(GL_TEXTURE_2D, 0, 3, 243, 238, 0, GL_BGR_EXT, GL_UNSIGNED_BYTE, pBytes);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	// 텍스처 모드 설정 
	glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_COLOR, GL_MODULATE);
	// 텍스처 매핑 활성화

	glBindTexture(GL_TEXTURE_2D, texture_object[3]);   //텍스처와 객체를 결합한다.
	pBytes = LoadDIBitmap("gameover.bmp", &info);   //이미지 로딩을 한다
	glTexImage2D(GL_TEXTURE_2D, 0, 3, 600, 337, 0, GL_BGR_EXT, GL_UNSIGNED_BYTE, pBytes);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	// 텍스처 모드 설정 
	glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_COLOR, GL_MODULATE);
}

void IntroInit()
{
	// 텍스처를 객체에 맵핑 
	//if (game_start)
	//	glTranslatef(2000.f,  0.f, 2000.f);
	glBindTexture(GL_TEXTURE_2D, texture_object[0]);
	glBegin(GL_QUADS);
	glColor3f(1.0, 1.0, 1.0);
	glTexCoord2f(0.0f, 1.0f);         //텍스처위치
	glVertex3f(-600, 550, 0);
	glTexCoord2f(0.0f, 0.0f);         //텍스처위치
	glVertex3f(-600, -550, 0);
	glTexCoord2f(1.0f, 0.0f);         //텍스처 위치
	glVertex3f(600, -550, 0);
	glTexCoord2f(1.0f, 1.0f);         //텍스처 위치
	glVertex3f(600, 550, 0);               //앞 왼상단부터 반시계.
	glEnd();

	glBindTexture(GL_TEXTURE_2D, texture_object[1]);
	glBegin(GL_QUADS);
	glColor3f(1, 1, 1);
	glTexCoord2f(0.0f, 1.0f);         //텍스처위치
	glVertex3f(-400, -100, 10);
	glTexCoord2f(0.0f, 0.0f);         //텍스처위치
	glVertex3f(-400, -300, 10);
	glTexCoord2f(1.0f, 0.0f);         //텍스처 위치
	glVertex3f(-200, -300, 10);
	glTexCoord2f(1.0f, 1.0f);         //텍스처 위치
	glVertex3f(-200, -100, 10);
	glEnd();

	glBindTexture(GL_TEXTURE_2D, texture_object[2]);
	glBegin(GL_QUADS);
	glColor3f(1, 1, 1);
	glTexCoord2f(0.0f, 1.0f);         //텍스처위치
	glVertex3f(200, -100, 10);
	glTexCoord2f(0.0f, 0.0f);         //텍스처위치
	glVertex3f(200, -300, 10);
	glTexCoord2f(1.0f, 0.0f);         //텍스처 위치
	glVertex3f(400, -300, 10);
	glTexCoord2f(1.0f, 1.0f);         //텍스처 위치
	glVertex3f(400, -100, 10);
	glEnd();

	glEnable(GL_TEXTURE_2D);
}

void GameOverInit()
{
	//glDeleteTextures(3, texture_object);
	glBindTexture(GL_TEXTURE_2D, texture_object[3]);
	glBegin(GL_QUADS);
	glColor4f(1.0, 1.0, 1.0, 0);
	glTexCoord2f(0.0f, 1.0f);         //텍스처위치
	glVertex3f(-600, 550, 0);
	glTexCoord2f(0.0f, 0.0f);         //텍스처위치
	glVertex3f(-600, -550, 0);
	glTexCoord2f(1.0f, 0.0f);         //텍스처 위치
	glVertex3f(600, -550, 0);
	glTexCoord2f(1.0f, 1.0f);         //텍스처 위치
	glVertex3f(600, 550, 0);               //앞 왼상단부터 반시계.
	glEnd();
	glEnable(GL_TEXTURE_2D);
}

void Collision_Check(Player* player, Board* board)
{
	float player_x, player_y, player_z;
	player->GetPosition(&player_x, &player_y, &player_z);
	float board_y = (player_z + 500.f) * sin((180 - board->GetRotate()) * PI / 180.f);		// 플레이어 z값에 해당하는 보드의 높이
	Pattern p;
	board->GetPattern(&p);
	int score;

	for (int i = 0; i < 5; ++i)
	{
		for (int j = 0; j < 5; ++j)
		{
			if (p.m_pattern[i][j] == 0)
			{
				if (player_x > -300 + (WIDTH_SIZE * j) && player_x < -300 + WIDTH_SIZE * (j + 1))
				{
					if (player_z > -500 + (DEPTH_SIZE * (4 - i)) && player_z < -500 + DEPTH_SIZE * (5 - i))
					{
						if (player_y + 90 > board_y)
						{
							player->SetState();
							board->SetState();
							PlaySound(TEXT(SOUND_FILE_PLAYERDEAD), NULL, SND_FILENAME | SND_ASYNC);
							score = player->GetScore();
							player->SetScore(score - 10);
							player->SetDead(true);
						}
					}
				}
			}
		}
	}
	//cout << player->GetState() << endl;
}

bool Collision_Check_Player(Player* player1, Player* player2)
{
	float x1, y1, z1;
	float x2, y2, z2;

	player1->GetPosition(&x1, &y1, &z1);
	player2->GetPosition(&x2, &y2, &z2);

	if (x1 - 30.f > x2 + 30.f)
		return false;
	if (x1 + 30 < x2 - 30.f)
		return false;
	if (z1 - 15.f > z2 + 15.f)
		return false;
	if (z1 + 15.f < z2 - 15.f)
		return false;

	cout << "충돌" << endl;
	return true;
}

void InitPlayer_one()
{
	m_player = new Player();
	m_player->SetPosition(0, 0, 0);
	m_player->SetVelocity(0, 0);
	m_player->SetAccel(0, 0);
	m_player->SetForce(0, 0);
	m_player->SetMass(5.f);
	m_player->SetFriction(0.8);
	m_player->SetArmDir(0);
	m_player->SetLegDir(0);
	m_player->SetArmRot(0, 0);
	m_player->SetLegRot(0, 0);
	m_player->SetRotDir(0);
	m_player->SetColor(0, 1, 1);
	m_player->SetDead(false);
	m_player->SetScore(0);
}

void InitPlayer_two()
{
	m_player = new Player();
	m_player->SetPosition(-40, 0, 0);
	m_player->SetVelocity(0, 0);
	m_player->SetAccel(0, 0);
	m_player->SetForce(0, 0);
	m_player->SetMass(5.f);
	m_player->SetFriction(0.8);
	m_player->SetArmDir(0);
	m_player->SetLegDir(0);
	m_player->SetArmRot(0, 0);
	m_player->SetLegRot(0, 0);
	m_player->SetRotDir(0);
	m_player->SetColor(0, 1, 1);
	m_player->SetScore(0);
	m_player->SetDead(false);

	m_player_second = new Player();
	m_player_second->SetPosition(40, 0, 0);
	m_player_second->SetVelocity(0, 0);
	m_player_second->SetAccel(0, 0);
	m_player_second->SetForce(0, 0);
	m_player_second->SetMass(5.f);
	m_player_second->SetFriction(0.8);
	m_player_second->SetArmDir(0);
	m_player_second->SetLegDir(0);
	m_player_second->SetArmRot(0, 0);
	m_player_second->SetLegRot(0, 0);
	m_player_second->SetRotDir(0);
	m_player_second->SetColor(0.75, 0.75, 0.75);
	m_player_second->SetScore(0);
	m_player_second->SetDead(false);
}

void ReadPattern()
{
	ifstream in;

	in.open("Pattern1.txt");

	if (!in.is_open())
		cout << "읽지 못하였습니다." << endl;
	else
	{
		for (int k = 0; k < 18; ++k)
		{
			for (int i = 0; i < 5; ++i)
			{
				for (int j = 0; j < 5; ++j)
				{
					in >> Board::pattern[k].m_pattern[i][j];
				}				
			}
		}
	}
	if (in.is_open())
		in.close();
}

GLvoid drawScene(GLvoid)
{
	glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);       // 설정된 색으로 전체를 칠하기
	glEnable(GL_DEPTH_TEST);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	if (game_start == false && game_dead == false)
		IntroInit();
	if (game_start == false && game_dead == true)
		GameOverInit();

	if (game_start == true)
	{
		glDisable(GL_TEXTURE_2D);
		glPushMatrix();								// 전체				
		{
			glTranslatef(g_xpos, g_ypos - 150.f, -200.f);
			glRotatef(20.f, 1.f, 0.f, 0.f);
			glPushMatrix();							// 맵
			{
				glPushMatrix();
				{
					glColor3f(1.f, 0.f, 0.f);
					for (int i = 0; i < 5; ++i)
					{
						for (int j = 0; j < 5; ++j)
						{
							glBegin(GL_QUADS);		// x축 -300 ~ 300, z축 -500 ~ 500
							glVertex3f((i - 2.5) * WIDTH_SIZE, 0.f, (j - 2.5) * DEPTH_SIZE);
							glVertex3f((i - 1.5) * WIDTH_SIZE, 0.f, (j - 2.5) * DEPTH_SIZE);
							glVertex3f((i - 1.5) * WIDTH_SIZE, 0.f, (j - 1.5) * DEPTH_SIZE);
							glVertex3f((i - 2.5) * WIDTH_SIZE, 0.f, (j - 1.5) * DEPTH_SIZE);
							glEnd();
						}
					}
				}
				glPopMatrix();

				glPushMatrix();
				{
					for (int i = 0; i < 6; ++i)
					{
						glColor3f(0.f, 0.f, 0.f);
						glLineWidth(10.f);
						glBegin(GL_LINES);
						glVertex3f(-300.f, 2.f, (i - 2.5) * DEPTH_SIZE);
						glVertex3f(300.f, 2.f, (i - 2.5) * DEPTH_SIZE);
						glVertex3f((i - 2.5) * WIDTH_SIZE, 2.f, -500.f);
						glVertex3f((i - 2.5) * WIDTH_SIZE, 2.f, 500.f);
						glEnd();
					}
				}
				glPopMatrix();

			}
			glPushMatrix();
			{
				for (int i = 0; i < g_BoardIndex + 1; ++i)
					if (!board[i]->GetState()) //충돌체크 안됬을때만
					{
						board[i]->Draw();
						board[i]->Shadow_Draw();
					}
			}
			glPopMatrix();
		

			glPushMatrix();
			{
				if (one_player == true)
				{
					float x, y, z, rot;
					m_player->GetPosition(&x, &y, &z);
					glTranslatef(x, y + 120, z);
					glRotatef(m_player->GetRotDir(), 0, 1, 0);
					m_player->Draw();
					m_player->Update();
				}
				if (one_player == false)
				{
					glPushMatrix();
					{
						float x, y, z, rot;
						m_player->GetPosition(&x, &y, &z);
						glTranslatef(x, y + 120, z);
						glRotatef(m_player->GetRotDir(), 0, 1, 0);
						m_player->Draw();
						m_player->Update();
					}
					glPopMatrix();
					glPushMatrix();
					{
						float x1, y1, z1, rot1;
						m_player_second->GetPosition(&x1, &y1, &z1);
						glTranslatef(x1, y1 + 120, z1);
						glRotatef(m_player_second->GetRotDir(), 0, 1, 0);
						m_player_second->Draw();
						m_player_second->Update();
					}
					glPopMatrix();
				}
			}
			glPopMatrix();
		}
		glPopMatrix();
	}
	glutSwapBuffers();
}

GLvoid Reshape(int w, int h)
{
	glViewport(0, 0, w, h);
	glMatrixMode(GL_PROJECTION);
	gluPerspective(60.f, w / h, 1.0, 5000.0);
	glTranslated(0.f, 0.f, -1000);
}

void TimerFunction(int value)
{
	if (game_start)
	{
		if (one_player == false)
			Collision_Check_Player(m_player, m_player_second);

		if (game_start == true)
		{
			++g_GameTime;

			if (g_GameTime % 30 == 0)
			{
				board[g_BoardIndex]->SetReady();
				++g_BoardIndex;
			}

			for (int i = 0; i < g_BoardIndex; ++i)
			{
				if (!board[i]->GetState())
					board[i]->Update();
			}
			if (one_player == true)
			{
				if (m_player->GetMove())
				{
					m_player->Animate();
				}
			}
			if (one_player == false)
			{
				if (m_player->GetMove())
				{
					m_player->Animate();
				}
				if (m_player_second->GetMove())
				{
					m_player_second->Animate();
				}
			}

			for (int i = 0; i < 50; ++i)
			{
				if (board[i]->GetReady())
				{
					if (!board[i]->GetState())
					{
						if (one_player == true)
						{
							Collision_Check(m_player, board[i]);
						}
						if (one_player == false)
						{
							Collision_Check(m_player, board[i]);
							Collision_Check(m_player_second, board[i]);
						}
					}
				}
			}
			if (one_player == false)
			{
				if (Collision_Check_Player(m_player, m_player_second) == true)
				{
					float x1, z1;
					float x2, z2;
					m_player->GetVelocity(&x1, &z1);
					m_player_second->GetVelocity(&x2, &z2);
					if (m_player->GetRotDir() == 90 && m_player_second->GetRotDir() == -90)
					{
						x1 -= 40;
						x2 += 40;
					}
					if (m_player->GetRotDir() == -90 && m_player_second->GetRotDir() == 90)
					{
						x1 += 40;
						x2 -= 40;
					}
					if (m_player->GetRotDir() == 180 && m_player_second->GetRotDir() == 0)
					{
						z1 += 40;
						z2 -= 40;
					}
					if (m_player->GetRotDir() == 0 && m_player_second->GetRotDir() == 180)
					{
						z1 -= 40;
						z2 += 40;
					}
					m_player->SetVelocity(x1, z1);
					m_player_second->SetVelocity(x2, z2);
				}
				if (m_player->GetDead() == true && m_player_second->GetDead() == true)
				{
				//	game_start = false;
				//	game_dead = true;    //무적
				}
			}
		}
	}
	glutPostRedisplay();
	glutTimerFunc(100, TimerFunction, 1);
}

void Mouse(int button, int state, int x, int y)
{
	if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN && (x-400>=-300 && x-400<=-200 && 300-y>=-300 && 300-y<=-60))
	{
		one_player = true;
		game_start = true;
		InitPlayer_one();
	}
	if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN && (x-400 >= 200 && x-400 <= 300 && 300-y >= -300 && 300-y <= -60))
	{
		one_player = false;
		game_start = true;
		InitPlayer_two();
	}
}

void KeyBoard(unsigned char key, int x, int y)
{
	switch (key)
	{
	case 'w':
		w_key = true;
		m_player->SetVelocity(0, -30);
		break;
	case 'a':
		a_key = true;
		m_player->SetVelocity(-30, 0);
		break;
	case 's':
		s_key = true;
		m_player->SetVelocity(0, 30);
		break;
	case 'd':
		d_key = true;
		m_player->SetVelocity(30, 0);
		break;
	case 'i':
		i_key = true;
		m_player_second->SetVelocity(0, -30);
		break;
	case 'j':
		j_key = true;
		m_player_second->SetVelocity(-30, 0);
		break;
	case'k':
		k_key = true;
		m_player_second->SetVelocity(0, 30);
		break;
	case'l':
		l_key = true;
		m_player_second->SetVelocity(30, 0);
		break;
	case 'x':
		g_x += 10.f;
		break;
	case 'X':
		g_x -= 10.f;
		break;
	case 'y':
		g_y += 10.f;
		break;
	case 'Y':
		g_y -= 10.f;
	case 'z':
		g_z += 10.f;
		break;
	case 'Z':
		g_z -= 10.f;
		break;

	default:
		break;
	}
	float forceX = 0.f, forceZ = 0.f;
	float forceX1 = 0.f, forceZ1 = 0.f;
	float amount = 20.f;
	float amount1 = 20.f;

	if (one_player == true)
	{
		// 1 Player
		if (w_key)
		{
			forceZ -= amount;
			m_player->SetRotDir(180);
			m_player->SetMove(true);
		}
		if (a_key)
		{
			forceX -= amount;
			m_player->SetRotDir(-90);
			m_player->SetMove(true);
		}
		if (s_key)
		{
			forceZ += amount;
			m_player->SetRotDir(0);
			m_player->SetMove(true);
		}
		if (d_key)
		{
			forceX += amount;
			m_player->SetRotDir(90);
			m_player->SetMove(true);
		}

		// 시점 계산
		if (w_key)
		{
			if (a_key)
			{
				m_player->SetVelocity(-30, -30);
				m_player->SetRotDir(-135);
			}
			if (d_key)
			{
				m_player->SetVelocity(30, -30);
				m_player->SetRotDir(135);
			}
		}
		if (a_key)
		{
			if (w_key)
			{
				m_player->SetVelocity(-30, -30);
				m_player->SetRotDir(-135);
			}
			if (s_key)
			{
				m_player->SetVelocity(-30, 30);
				m_player->SetRotDir(-45);
			}
		}
		if (s_key)
		{
			if (a_key)
			{
				m_player->SetVelocity(-30, 30);
				m_player->SetRotDir(-45);
			}
			if (d_key)
			{
				m_player->SetVelocity(30, 30);
				m_player->SetRotDir(45);
			}
		}
		if (d_key)
		{
			if (s_key)
			{
				m_player->SetVelocity(30, 30);
				m_player->SetRotDir(45);
			}
			if (w_key)
			{
				m_player->SetVelocity(30, -30);
				m_player->SetRotDir(135);
			}
		}

		m_player->ApplyForce(forceX, forceZ);
	}

	if (one_player == false)
	{
		// 1 Player
		if (w_key)
		{
			forceZ -= amount;
			m_player->SetRotDir(180);
			m_player->SetMove(true);
		}
		if (a_key)
		{
			forceX -= amount;
			m_player->SetRotDir(-90);
			m_player->SetMove(true);
		}
		if (s_key)
		{
			forceZ += amount;
			m_player->SetRotDir(0);
			m_player->SetMove(true);
		}
		if (d_key)
		{
			forceX += amount;
			m_player->SetRotDir(90);
			m_player->SetMove(true);
		}
		// 2 Player
		if (i_key)
		{
			forceZ1 -= amount1;
			m_player_second->SetRotDir(180);
			m_player_second->SetMove(true);
		}
		if (j_key)
		{
			forceX1 -= amount1;
			m_player_second->SetRotDir(-90);
			m_player_second->SetMove(true);
		}
		if (k_key)
		{
			forceZ1 += amount1;
			m_player_second->SetRotDir(0);
			m_player_second->SetMove(true);
		}
		if (l_key)
		{
			forceX1 += amount1;
			m_player_second->SetRotDir(90);
			m_player_second->SetMove(true);
		}

		// 시점계산
		if (w_key)
		{
			if (a_key)
			{
				m_player->SetVelocity(-30, -30);
				m_player->SetRotDir(-135);
			}
			if (d_key)
			{
				m_player->SetVelocity(30, -30);
				m_player->SetRotDir(135);
			}
		}
		if (a_key)
		{
			if (w_key)
			{
				m_player->SetVelocity(-30, -30);
				m_player->SetRotDir(-135);
			}
			if (s_key)
			{
				m_player->SetVelocity(-30, 30);
				m_player->SetRotDir(-45);
			}
		}
		if (s_key)
		{
			if (a_key)
			{
				m_player->SetVelocity(-30, 30);
				m_player->SetRotDir(-45);
			}
			if (d_key)
			{
				m_player->SetVelocity(30, 30);
				m_player->SetRotDir(45);
			}
		}
		if (d_key)
		{
			if (s_key)
			{
				m_player->SetVelocity(30, 30);
				m_player->SetRotDir(45);
			}
			if (w_key)
			{
				m_player->SetVelocity(30, -30);
				m_player->SetRotDir(135);
			}
		}
		if (i_key)
		{
			if (j_key)
			{
				m_player_second->SetVelocity(-30, -30);
				m_player_second->SetRotDir(-135);
			}
			if (l_key)
			{
				m_player_second->SetVelocity(30, -30);
				m_player_second->SetRotDir(135);
			}
		}
		if (j_key)
		{
			if (i_key)
			{
				m_player_second->SetVelocity(-30, -30);
				m_player_second->SetRotDir(-135);
			}
			if (k_key)
			{
				m_player_second->SetVelocity(-30, 30);
				m_player_second->SetRotDir(-45);
			}
		}
		if (k_key)
		{
			if (j_key)
			{
				m_player_second->SetVelocity(-30, 30);
				m_player_second->SetRotDir(-45);
			}
			if (l_key)
			{
				m_player_second->SetVelocity(30, 30);
				m_player_second->SetRotDir(45);
			}
		}
		if (l_key)
		{
			if (k_key)
			{
				m_player_second->SetVelocity(30, 30);
				m_player_second->SetRotDir(45);
			}
			if (i_key)
			{
				m_player_second->SetVelocity(30, -30);
				m_player_second->SetRotDir(135);
			}
		}
		m_player->ApplyForce(forceX, forceZ);
		m_player_second->ApplyForce(forceX1, forceZ1);
	}
}

void KeyboardUp(unsigned char key, int x, int y)
{
	switch (key)
	{
	case 'w':
		w_key = false;
		m_player->SetMove(false);
		break;
	case 'a':
		a_key = false;
		m_player->SetMove(false);
		break;
	case 's':
		s_key = false;
		m_player->SetMove(false);
		break;
	case 'd':
		d_key = false;
		m_player->SetMove(false);
		break;
	case 'i':
		i_key = false;
		m_player_second->SetMove(false);
		break;
	case 'j':
		j_key = false;
		m_player_second->SetMove(false);
		break;
	case 'k':
		k_key = false;
		m_player_second->SetMove(false);
		break;
	case 'l':
		l_key = false;
		m_player_second->SetMove(false);
		break;
	}
}

GLubyte * LoadDIBitmap(const char *filename, BITMAPINFO **info)
{
	FILE *fp;
	GLubyte *bits;
	int bitsize, infosize;
	BITMAPFILEHEADER header;
	// 바이너리 읽기 모드로 파일을 연다  
	if ((fp = fopen(filename, "rb")) == NULL)
		return NULL;
	// 비트맵 파일 헤더를 읽는다. 
	if (fread(&header, sizeof(BITMAPFILEHEADER), 1, fp) < 1) {
		fclose(fp);
		return NULL;
	}
	// 파일이 BMP 파일인지 확인한다.  
	if (header.bfType != 'MB') {
		fclose(fp);
		return NULL;
	}
	// BITMAPINFOHEADER 위치로 간다.  
	infosize = header.bfOffBits - sizeof(BITMAPFILEHEADER);
	// 비트맵 이미지 데이터를 넣을 메모리 할당을 한다.  
	if ((*info = (BITMAPINFO *)malloc(infosize)) == NULL) {
		fclose(fp);
		exit(0);
		return NULL;
	}
	// 비트맵 인포 헤더를 읽는다.  
	if (fread(*info, 1, infosize, fp) < (unsigned int)infosize) {
		free(*info);
		fclose(fp);
		return NULL;
	}
	// 비트맵의 크기 설정  
	if ((bitsize = (*info)->bmiHeader.biSizeImage) == 0)
		bitsize = ((*info)->bmiHeader.biWidth*(*info)->bmiHeader.biBitCount + 7) / 8.0 *
		abs((*info)->bmiHeader.biHeight);
	// 비트맵의 크기만큼 메모리를 할당한다.  
	if ((bits = (unsigned char *)malloc(bitsize)) == NULL) {
		free(*info);
		fclose(fp);
		return NULL;
	}
	// 비트맵 데이터를 bit(GLubyte 타입)에 저장한다.  
	if (fread(bits, 1, bitsize, fp) < (unsigned int)bitsize) {
		free(*info);
		free(bits);
		fclose(fp);
		return NULL;
	}
	fclose(fp);
	return bits;
}