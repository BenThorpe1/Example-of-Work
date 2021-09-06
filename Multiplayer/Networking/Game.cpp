#include "stdafx.h"
const unsigned int	SHDR_1 = 3;
const float BALL_VOLECITY_MAX = 0.1f;
const float BALL_VOECITY_MIN = 0.05f;
const unsigned int	SHDR = 0;
const float PLAYER_SPEED = 0.10f;
bool isStarted = false;

int score[2];

Game::Game()
{
	m_numPlayers = 0;
	m_turnOrder = 0;
	m_ballXPos = 0;
	m_ballYPos = 0;
	m_ID = 0;

	for (int i = 0; i < 2; i++)
	{
		score[i] = 0;
	}

}
Game::~Game()
{

}

void Game::Init(Scene* a_scene, bool a_isSever)
{
	this->GameState::Init(a_scene, a_isSever);
	m_numPlayers = 0;
	InitSceneData();
	m_turnOrder = 0;
	for (int i = 0; i < MAX_PLAYERS; i++)
	{
		m_yPos[i] = m_players[i].GetPos().y;
	}
	m_textS1.init("numerals.obj", 3, SHDR_1, vec3(0.5f, 0.9f, -0.9), vec3(1, 0, 0), 180.0f, vec3(0.04f, 0.2f, 1.0f), 32);
	m_textS2.init("numerals.obj", 3, SHDR_1, vec3(-0.5f, 0.9f, -0.9), vec3(1, 0, 0), 180.0f, vec3(0.04f, 0.2f, 1.0f), 32);
	glBindVertexArray(0);

	char temp[] = "0";
	int i = strlen(temp) + 1;
	strcpy_s(m_textBuffer2, i, temp);
	strcpy_s(m_textBuffer, i, temp);

}

void Game::DeInit()
{
}

void Game::Update(float a_deltaTime)
{
	//Update all players
	for (int i = 0; i < m_numPlayers; i++)
	{
	m_players[i].Update(m_yPos[i]);
	}

	if (m_isSever)
	{
		//Update ball
		m_ball.Update(&m_players[m_turnOrder], 0.0, a_deltaTime);
		m_ballYPos = m_ball.GetYPos();
		m_ballXPos = m_ball.GetXPos();
		Message message;
		message.SetType(Message::MessageType::Upadate);
		UpdateData data(m_yPos,m_ballXPos,m_ballYPos,score);
		message.SetData(data);
		m_scene->SendData(&message);
	}
	else
	{
		m_ball.SetPos(m_ballXPos,m_ballYPos);
	}

	m_textS1.Update(0.0);
	m_textS2.Update(0.0);
}

void Game::RenderScene(class cShaderInfo* pShaderInfo)
{
	// update the camera matrix
	m_camera.update();

	glEnable(GL_DEPTH_TEST);
	glCullFace(GL_BACK);

	// Clear the rendering window
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	// Render ALL mesh objects here..

	// Players
	for (int i = 0; i < m_numPlayers; i++)
	{
		m_players[i].Render(pShaderInfo);
	}
	// ball object
	m_ball.Render(pShaderInfo);
	//m_score->Render(pShaderInfo);
	//////////////////////////////////////////////////////////////////////////////////////////////////////

	m_textS1.render(pShaderInfo, m_textBuffer, false);
	m_textS2.render(pShaderInfo, m_textBuffer2, false);




	glBindVertexArray(0);       // Not necessary, but a good idea
	check_for_opengl_errors();
}

// SetupGeometries() - load all the mesh objects
void Game::SetupGeometries()
{
	for (int i = 0; i < MAX_PLAYERS; i++)
	{
		vec3 pos = vec3(-0.75f, 0.0f, 0.0f);
		if ((i + 1) % 2 == 0)
		{
			pos.x *= -1;
		}
		m_players[i].Init("player1.obj", 0, SHDR, pos, vec3(1, 0, 0), 180.0f, vec3(0.05f, 0.2f, 1.0));
	}
	// Ball args: velocity, mesh_name, meshID,	shdrID,	pos, rot_axis, rot_angle, scale
	m_ball.Init(RandomBallDirection(), "torus.obj", 1, SHDR, vec3(0.0f, 0.25f, 0.0f), vec3(1, 1, 1), NULL, vec3(0.05f), this);
		
	// Backdrop image
	// create a splashImage ("quad.obj") - set the z positon to 0.2f to prevent z-fighting 
	glBindVertexArray(0);
	check_for_opengl_errors();
}

void Game::InitSceneData()
{
	SetupGeometries();
	check_for_opengl_errors();
}

void Game::SetPlayerPosition(float a_posY)
{
	m_yPos[m_ID] = a_posY;

	Message mess;
	mess.SetType(Message::MessageType::Upadate);
	UpdateData data;
	data.playerPos[m_ID] = m_yPos[m_ID];
	mess.SetData(data);
	m_scene->SendData(&mess);
}

void Game::Increase(int a_value)
{
	m_yPos[m_ID] = m_players[m_ID].GetPos().y + (a_value * PLAYER_SPEED);

	Message mess;
	mess.SetType(Message::MessageType::Upadate);
	UpdateData data;
	data.playerPos[m_ID] = m_yPos[m_ID];

	mess.SetData(data);
	m_scene->SendData(&mess);
}

void Game::UpdateScore(bool a_isLeft)
{
	//Convert number to char[]
	char numTemp[MAX_TEXT_SIZE];
	if (a_isLeft == false)
	{
		score[0]++;
		sprintf_s(numTemp, "%i", score[0]);
		int i = strlen(numTemp) + 1;
		strcpy_s(m_textBuffer, i, numTemp);
	}
	else
	{
		score[1]++;
		sprintf_s(numTemp, "%i", score[1]);
		//Insert number text into buffer
		int i = strlen(numTemp) + 1;
		strcpy_s(m_textBuffer2, i, numTemp);
	}
	
	

	m_ball.SetVelocity(RandomBallDirection());
}

void Game::UpdateTurnOrder()
{
	if (m_turnOrder == 0)
	{
		m_turnOrder = 1;
	}
	else
	{
		m_turnOrder = 0;
	}

}

vec3 Game::RandomBallDirection()
{
	vec3 direction = vec3(RandomFloat(), RandomFloat(), 0.0);
	if (direction.x > 0)
	{
		m_turnOrder = 1;
	}
	else
	{
		m_turnOrder = 0;
	}

	return direction;
}

float Game::RandomFloat()
{
	float random = ((float)rand()) / (float)RAND_MAX;

	// generate (in your case) a float between 0 and (4.5-.78)
	// then add .78, giving you a float between .78 and 4.5
	float range = BALL_VOLECITY_MAX - BALL_VOECITY_MIN;
	float value = (random * range) + BALL_VOECITY_MIN;
	float signRandom = (float)rand() / (float)RAND_MAX;
	if (signRandom >= 0.5f)
	{
		value *= -1.0f;
	}

	return value;
}

void Game::RecieveKeyInput(int a_key)
{
	if (m_isSever);
	{
		return;
	}
	// process keyboard inputs here..
	switch (a_key)
	{
	case GLFW_KEY_W:
	{
		Increase(1);
		break;
	}
	case GLFW_KEY_S:
	{
		Increase(-1);
		break;
	}
	default:
	{
		break;
	}
	}
}

void Game::RecieveData(Message* a_message,int a_i)
{
	if (a_message->GetType() != Message::MessageType::Upadate)
	{
		if (a_message->GetType() == Message::MessageType::START)
		return;
	}

	if (m_isSever)
	{
		m_yPos[a_i] = a_message->GetData().playerPos[a_i];
	}
	else
	{
		UpdateData data = a_message->GetData();
		m_ballXPos = data.ballXPos;
		m_ballYPos = data.ballYPos; 
		
		if (score[0] != data.score[0])
		{
			char numTemp[MAX_TEXT_SIZE];

			score[0] = data.score[0];
			sprintf_s(numTemp, "%i", score[0]);
			int i = strlen(numTemp) + 1;
			strcpy_s(m_textBuffer, i, numTemp);
		}
		if (score[1] != data.score[1])
		{
			char numTemp[MAX_TEXT_SIZE];

			score[1] = data.score[1];
			sprintf_s(numTemp, "%i", score[0]);
			int i = strlen(numTemp) + 1;
			strcpy_s(m_textBuffer2, i, numTemp);
		}
		
		for (int i = 0; i < m_numPlayers; i++)
		{
			if (i == m_ID)
			{
				continue;
			}
			m_yPos[i] = data.playerPos[i];
		}
	}

}

void Game::Connection()
{
}
void Game::Disconnected()
{}
void Game::Start(int a_players,int a_id)
{
	if (a_players >= MAX_PLAYERS)
	{
		m_numPlayers = MAX_PLAYERS;
	}
	else
	{
		m_numPlayers = a_players;
	}
	printf(" Client is player %i \n", a_id);
	m_ID = a_id;
}
