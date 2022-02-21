#pragma once

//Glob�lis v�ltoz�k,amikre sok oszt�lyba sz�ks�g van
namespace TorpedoGLOBAL {
	const bool CameraBounds = true; //Legyen-e bekorl�tozva a kamera a j�t�kt�rre.
	const float Scale = 1.0f;	//glob�lis grafikai sk�l�z�s
	const bool Debug = false;	//Ha ez igaz,akkor a kliens DEBUG m�dba fut,nem csatlakozik szerverhez. J� grafikai tesztel�skor.
}

//J�t�k kliens �llapota
enum GameState {
	INITIAL = 1,
	SHIP_SIZE_INPUT,
	PLACING_SHIP,
	SHOOTING_AT_ENEMY,
	GETTING_SHOT,
	//WON_GAME,
	//LOST_GAME,
};

//Van-e k��rand� error
//enum ErrorState {
//	NONE = 0,
//	NO_UNPLACED_SHIPS_LEFT,
//};