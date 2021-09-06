#include "stdafx.h"

//Increament of the speed
const float SPEED_STEP = 0.50f;
const float START_SPEED = 1.0f;
const float MAX_SPEED = 5.0f;

//Empty 
cBall::cBall()
{
	m_speed = 0.0f;
	m_velocity = vec3(0.0f);
}

// Empty 
cBall::~cBall()
{

}

// Initise object
void cBall::Init(vec3 a_velocity, const char* a_meshName, int a_meshID, int a_shaderID, vec3 a_pos,
	vec3 a_rotAxis, float a_rotAngle, vec3 a_scale, Game* a_game)
{
	m_speed = START_SPEED;
	m_velocity = a_velocity;
	m_centerPosition = a_pos;
	m_game = a_game;
	this->cMesh::Init(a_meshName, a_meshID, a_shaderID, a_pos,
		a_rotAxis, a_rotAngle, a_scale);
}

// Moves the ball and check for collision 
void cBall::Update(cPlayer* a_players, float a_spinSpeed,float a_deltaTime)
{
	// add velocity to the balls current position
	m_transform.m_pos += m_velocity * m_speed * a_deltaTime;
	// calculate the matrix used to translate, scale and rotate this mesh object 
	this->cMesh::Update(a_spinSpeed);
	
 //Players collision check
	if (Collision(a_players) == true)
		{
			// increase difficulty level by increasing the velocity slightly
			m_velocity.x *= -1;
			if (m_speed <= MAX_SPEED)
			{
			m_speed += SPEED_STEP;
			}
			m_game->UpdateTurnOrder();
			return;
		}
		

	// checking collision against the the walls, ceiling and floor
	vec3 pos = m_transform.m_pos;
	//Walls
	if (pos.x >= 1.0f)
	{
		m_velocity.x *= -1;
		CollideWall(false);
	}
	if (pos.x <= -1.0f)
	{
		m_velocity.x *= -1;
		CollideWall(true);
	}
	//Ceilling collision
	if (pos.y >= 1.0f)
	{
		m_velocity.y *= -1;
	}
	if (pos.y <= -1.0f)
	{
		m_velocity.y *= -1;
	}
}

void cBall::SetPos(float a_xPos, float a_yPos)
{
	//Update the players position to match inputed
	vec3 position = vec3(a_xPos, a_yPos, m_transform.m_pos.z);
	m_transform.m_pos = position;
	this->cMesh::Update(0.0);
}

// collision() - checks for a collision with the current player - performs a simple sphere collision check
bool cBall::Collision(cPlayer* a_currPlayer)
{
	// get the currPlayer's object bounding box
	//sOBB bBox = currPlayer->getOBB();
	// apply model matrix to the bounding boxes min/max values
	//vec4 min1 = currPlayer->m_obj.modelMatrix * vec4(bBox.m_min,1.0f);
	//vec4 max1 = currPlayer->m_obj.modelMatrix * vec4(bBox.m_max,1.0f);
	
	vec4 min = a_currPlayer->GetMinOOB();
	vec4 max = a_currPlayer->GetMaxOOB();
	// get the ball position
	vec3 ballPos = m_transform.m_pos;

	// containment check - is ball within the min/max extents of the currPlayer's bounding box?
	if (ballPos.x >= min.x && ballPos.x <= max.x)
	{
		if (ballPos.y <= min.y && ballPos.y >= max.y)
		{
			return true;
		}
	}
	return false;
}

// Informs scene that the ball has hit a wall
void cBall::CollideWall(bool a_isLeft)
{
//Reset balls position
	m_speed = START_SPEED;
	m_transform.m_pos = m_centerPosition;
	m_game->UpdateScore(a_isLeft);
}
