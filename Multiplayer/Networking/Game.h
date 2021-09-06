#pragma once
#include "stdafx.h"


class cMesh;
class cBall;
class cPlayer;
class cCounter;
class cCamera;

class Game : public GameState
{
public:
	Game();
	~Game();
	void Init(Scene* a_scene,bool a_isSever)override;
	void DeInit() override;
	void Update(float a_deltaTime) ;
	void RenderScene(class cShaderInfo* pShaderInfo);
	void RecieveKeyInput(int a_key) ;
	void SetPlayerPosition(float a_posY);
	void UpdateScore(bool a_isLeft);
	void UpdateTurnOrder();
	void Start(int a_players,int a_id);
	 void RecieveData(Message* a_message, int a_i);
	 void Connection();
	 void Disconnected();

private:

	void SetupGeometries();
	void InitSceneData();
	vec3 RandomBallDirection();
	float RandomFloat();
	void Increase(int a_value);
	void AddPlayer();

	cMesh m_splashImage;
	cBall m_ball;
	cPlayer m_players[MAX_PLAYERS];
	cCamera m_camera;
	//y position for the players
	float m_yPos[MAX_PLAYERS];
	float m_ballYPos;
	float m_ballXPos;
	int m_turnOrder;
	int m_numPlayers;

	cText m_textS1;
	cText m_textS2;

	char m_textBuffer[MAX_TEXT_SIZE];
	char m_textBuffer2[MAX_TEXT_SIZE];

};