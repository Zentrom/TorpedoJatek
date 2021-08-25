#pragma once

//Globális változók,amikre sok osztályba szükség van
namespace TorpedoGLOBAL {
	const bool CameraBounds = true; //Legyen-e bekorlátozva a kamera a játéktérre.
	const float Scale = 1.0f;	//globális grafikai skálázás
	const bool Debug = true;	//Ha ez igaz,akkor a kliens DEBUG módba fut,nem csatlakozik szerverhez. Jó grafikai teszteléskor.
}