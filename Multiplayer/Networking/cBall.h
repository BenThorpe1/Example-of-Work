#pragma once

#include "stdafx.h"

class Game;
class cPlayer;

class cBall :public cMesh
{
public:
	cBall();
	~cBall();
	void Init(vec3 a_velocity, const char* a_meshName, int a_meshID, int a_shaderID, vec3 a_pos, 
				vec3 a_rotAxis, float a_rotAngle, vec3 a_scale, Game* a_game);
	void Update(cPlayer* a_players, float a_spinSpeed, float a_deltaTime);
	void SetVelocity(vec3 a_velocity)	{m_velocity = a_velocity;}
	void SetPos(float a_xPos,float a_yPos);
	float GetYPos() { return m_transform.m_pos.y; }
	float GetXPos() { return m_transform.m_pos.x; }

private:
	bool Collision(cPlayer* a_currPlayer);
	void CollideWall(bool a_isLeft);
	vec3	m_velocity;
	float	m_speed;
	Game* m_game;
	vec3 m_centerPosition;
};