#pragma once
#include "stdafx.h"

class GameState;
class Message;
class cClient;
class cServer;
class Game;
class Menu;

class Scene
{
	public:
	Scene();
	~Scene();
	void Init(bool a_isSever);
	void DeInit();
	void Update(float a_deltaTime);

	void SetSever(cServer* a_server);
	void SetClient(cClient* a_client);

	void RenderScene();
	void ReciveMouseInput(vec2 a_mouse);
	void ReciveKeyBoardInput(int a_key);
	void StartGame(int a_numPlayer,int a_id);

	void RecieveData(Message* a_message,int a_id);
	void AddConnection();
	void Disconnection();
	void SendData(Message* a_message);

private:

	Game m_game;
	Menu m_menu;
	cClient* m_client;
	cServer* m_sever;
	cShaderInfo m_shaderInfo;
};