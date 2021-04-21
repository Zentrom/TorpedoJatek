#pragma once

#include <array>

//Kliens �s szervern�l is sz�ks�ges glob�lis dolgok

//Elv�rt adatot k�ld�nk-e �t vagy m�st
enum class MessageType {
	ESTIMATED,
	QUIT,
};

//J�t�k�llapot v�lasz
enum class ResponseState {
	HIT_ENEMY_SHIP = 1,
	CONTINUE_MATCH,
	START_OF_GAME,
	WIN_PLAYER_ONE,
	WIN_PLAYER_TWO,
};

//Hajoszam a palyanagysag fuggvenyeben(1x1,2x2,stb.)
namespace ShipCount {
	const std::array<int, 4> Five = { 3,1,1,0 };
	const std::array<int, 4> Seven = { 4,3,2,0 };
	const std::array<int, 4> Nine = { 6,4,3,1 };
}