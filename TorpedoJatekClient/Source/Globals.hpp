#pragma once

//Globális változók,amikre sok osztályba szükség van
namespace TorpedoGLOBAL {
	const bool CameraBounds = true; //Legyen-e bekorlátozva a kamera a játéktérre.
	const float Scale = 1.0f; //globális grafikai skálázás
	const bool Debug = false; //Ha ez igaz,akkor a kliens DEBUG módba fut,nem csatlakozik szerverhez. Jó grafikai teszteléskor.

	const float SeaTileScaleXZ = 2.0f; //Tengermezõ nagyítás
}

//Játék kliens állapota
enum GameState {
	INITIAL = 1,
	SHIP_SIZE_INPUT,
	PLACING_SHIP,
	STARTING_MATCH,
	SHOOTING_AT_ENEMY,
	GETTING_SHOT,
	CHECKING_VICTORY_CONDITION,
	MATCH_ENDING,
};
