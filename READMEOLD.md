## TorpedoJatek (TorpedoGame)

**TorpedoJatek** is a 1vs1 torpedo game  i created from scratch. It was the 3rd game i ever created and also my thesis at the university.  
It is based upon server-client architecture. It was thoroughly tested, so the latest version(v0.1.1) should not crash at all.

It uses:
- **OpenGL** API for 3d graphics and shaders. 
- **SDL** for input handling, drawing letters, playing audio and tcp connection between server and client.
- **glm** for vector/matrix types and calculations.

![Gameplay image](https://raw.githubusercontent.com/Zentrom/TorpedoJatek-TorpedoGame/master/Bstuff/BuildPics/V007.png)

### Prerequisites

**Build:** _Visual Studio 2015_ or newer having the _MSCV_ compiler.  
The _Windows 8 SDK_ or newer.

**Play:** In case of a runtime error, you have to install the appropriate _Visual C++ redistributable_.

### Installation

You can download the latest pre-built `.zip`-s in [release](https://github.com/Zentrom/TorpedoJatek-TorpedoGame/releases), built on Windows 7.

...or build it:

1. Clone the repository
 ```bash
 git clone Zentrom/TorpedoJatek-TorpedoGame
 ```
2. Open `TorpedoJatek.sln` with Visual Studio and build both the client and the server project in **Release** mode, **x64** platform.
3. Copy the respective _.dll_ files from `OGLPack/bin`, inside the build directories where the server/client is located.

### Usage

1. Start the server executable. Make sure you have the selected port forwarded if you plan to play over the internet. (or use a VPN)
2. Both players start the client executable and connect to the server host.

In the client, you can also change some options like resolution or volume.  
The game controls are listed inside the **Play** menu.  
Currently the game does not have an AI opponent, so there is **no singleplayer** mode. You can run two client instances on the same pc though. There is also the **Debug** option in the main menu, which lets you check the 3d scene without the need to connect to a game server.

#### Contributions

This project is currently **_stalled_**. I might return to it in the future to optimize it or even add new content, but that might also never happen. This also means that I will not review pull requests, unless i start actively developing this again.

#### License

Commercial use of this project and its derivatives is not allowed.  
Details in the [License](https://github.com/Zentrom/TorpedoJatek-TorpedoGame/blob/master/LICENSE.md) file.