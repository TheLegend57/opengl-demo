#ifndef GAME_H
#define GAME_H

#include "Logger.h"
#include "Shader.h"
#include "Texture.h"
#include "../Dependencies/GLFW/include/GLFW/glfw3.h"

#include "Menu.h"
#include "Object.h"

class Game{
public:
	Game();
	~Game();
	int Initialize(int, char**);
	void ShowMainMenu();
	void ShowNewGameMenu();
	void GameDebugWindow();
	void ShowMultiplayerMenu();
	void ShowSettingsMenu();
	void StartOfflineGame();
	void CleanUp();
private:
	//Used variables
	GLFWwindow *MainWindow;
	Camera *MainCamera = new Camera(1080, 1920);
	Font *MainFont;
	bool cameraFirstTime = false, timeFirstTime = false;
	float prevPosX = 0.f, prevPosY = 0.f, dt = 0.f, lastTime = 0.f;
	
	//Settings and data
	bool Debug = false;
	bool Logging = false;
	std::string ExecPath;
	
	//Hidden functions
	void InitCommandLineOptions(int, char**);
	void UpdateInput();
	
	//Logger
	Logger *MainLogger = NULL;
};

#endif // GAME_H
