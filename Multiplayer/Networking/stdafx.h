#pragma once

#define _WINSOCK_DEPRECATED_NO_WARNINGS
#define MAX_PLAYERS 4


#include <winsock2.h>
#include <ws2tcpip.h>
#define WIN32_LEAN_AND_MEAN

#include <windows.h>
#include <iostream>
#include <string.h>
#include <stdio.h>
#include <math.h>

#include "Server.h"
#include "Client.h"
#include "NetworkManager.h"
#include "Message.h"
#include "OutputMemoryStream.h"
#include "InputMemoryStream.h"

#include "glm/glm.hpp"  
#include "glm/gtc/matrix_transform.hpp" 
#include "glm/gtc/type_ptr.hpp"
#include "core.h"

#include "cTransform.h"
#include "cMesh.h"
#include "cPlayer.h"
#include "cBall.h"
#include "cCounter.h"
#include "cText.h"
#include "cObjImport.h"
#include "cCamera.h"

#include "GameState.h"
#include "Game.h"
#include "Menu.h"

#include "Scence.h"


