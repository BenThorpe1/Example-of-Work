#include "stdafx.h"

GameState::GameState()
{
}

GameState::~GameState()
{
}

void GameState::Init(Scene* a_scene,bool a_isSever)
{
	m_scene = a_scene;
	m_isSever = a_isSever;
}
void GameState::DeInit()
{
}
void GameState::Update(float a_deltaTime)
{
}
void GameState::RenderScene(class cShaderInfo* pShaderInfo)
{

}
void GameState::RecieveKeyInput(int a_key) {}
void GameState::RecieveData(Message* a_message)
{
	
}

void GameState::Connection() 
{
}
void GameState::Disconnected()
 {
 }