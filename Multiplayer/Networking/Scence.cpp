#include "stdafx.h"

bool isSever;
int playerStart = 0;
int playerCount = 0;

enum StateSetting
{
	GAME = 0,
	MENU = 1,
	CHANGE = 2,
};
StateSetting currentState;

Scene::Scene()
{
	currentState = StateSetting::CHANGE;
	isSever = false;
}
Scene::~Scene()
{
	DeInit();
}

void Scene::DeInit()
{
	m_menu.DeInit();
	m_game.DeInit();
}

void Scene::Init(bool a_isSever)
{


	// load the list of shaders required within this scene
	m_shaderInfo.create();
	isSever = a_isSever;
	
	m_menu.Init(this, isSever);
	m_game.Init(this, isSever);
	currentState = StateSetting::MENU;
}

void Scene::Update(float a_deltaTime)
{
	switch (currentState)
	{
	case StateSetting::GAME:
	{
		m_game.Update(a_deltaTime);
		break;
	}
	case StateSetting::MENU:
	{
		m_menu.Update(a_deltaTime);
		break;
	}
	case StateSetting::CHANGE:
	{
		if (isSever)
		{
			if (playerStart == playerCount)
			{
				m_game.Start(playerCount, 0);
				currentState = StateSetting::GAME;
			}
			else
			{
				Message startMessage;
				startMessage.SetType(Message::MessageType::START);
				UpdateData data;
				data.playerPos[0] = playerCount;
				startMessage.SetData(data);
				SendData(&startMessage);
			}
		}
		break;
	}
	}
}

// renderScene() - 
void Scene::RenderScene()
{
	switch (currentState)
	{
	case StateSetting::GAME:
	{
		m_game.RenderScene(&m_shaderInfo);
		break;
	}
	case StateSetting::MENU:
	{
		m_menu.RenderScene(&m_shaderInfo);
		break;
	}
	}
}

void Scene::ReciveMouseInput(vec2 a_mouse)
{
	switch (currentState)
	{
	case StateSetting::GAME:
	{	
		if (isSever)
		{
			return;
		}
		m_game.SetPlayerPosition(a_mouse.y);
		break;
	}
	default:
	{
		break;
	}
	}
}

void Scene::StartGame(int a_numPlayer,int a_id)
{
	if (currentState != StateSetting::MENU)
	{
		return;
	}
	playerCount = a_numPlayer;

	currentState = StateSetting::CHANGE;
	m_menu.DeInit();	

	if (isSever)
	{
		Message startMessage;
		startMessage.SetType(Message::MessageType::START);
		UpdateData data;
		data.playerPos[0] = a_numPlayer;
		startMessage.SetData(data);
		SendData(&startMessage);
	}
	else
	{
		printf(" cleint is %i \n", a_id);
		m_game.Start(playerCount, a_id);
		currentState = StateSetting::GAME;
		Message startMessage;
		startMessage.SetType(Message::MessageType::START);
		SendData(&startMessage);
	}
}

void Scene::ReciveKeyBoardInput(int a_key)
{
	switch (currentState)
	{
	case StateSetting::GAME:
	{
		m_game.RecieveKeyInput(a_key);
		break;
	}
	case StateSetting::MENU:
	{
		m_menu.RecieveKeyInput(a_key);
		break;
	}
	}
}

void Scene::RecieveData(Message* a_message, int a_id)
{

	switch (currentState)
	{
	case StateSetting::GAME:
	{
		m_game.RecieveData(a_message, a_id);
		break;
	}
	case StateSetting::MENU:
	{
		m_menu.RecieveData(a_message);
		break;
	}
	case StateSetting::CHANGE:
	{

		if (a_message->GetType() == Message::MessageType::START)
		{
			playerStart++;
			break;
		}
	}
	}


}

void Scene::AddConnection()
{
	if (currentState == StateSetting::CHANGE)
	{
		return;
	}

	switch (currentState)
	{
	case StateSetting::GAME:
	{
		m_game.Connection();
		break;
	}
	case StateSetting::MENU:
	{
		m_menu.Connection();	
		break;
	}
	}

}
void Scene::Disconnection()
{
	if (currentState == StateSetting::CHANGE)
	{
		return;
	}

	switch (currentState)
	{
	case StateSetting::GAME:
	{
		m_game.Disconnected();
		break;
	}
	case StateSetting::MENU:
	{
		m_menu.Disconnected();
		break;
	}
	}
}
void Scene::SendData(Message* a_message)
{
	if (isSever)
	{
		m_sever->SetMessage(a_message);
	}
	else
	{
		m_client->SetMessage(a_message);	
	}
}

void Scene::SetSever(cServer* a_server)
{
	m_sever = a_server;

}
void Scene::SetClient(cClient* a_client)
{
	m_client = a_client;
}