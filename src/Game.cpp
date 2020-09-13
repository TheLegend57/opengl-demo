#include "Game.h"
Game::Game(){
	
}

Game::~Game(){
	this->CleanUp();
}

void Game::InitCommandLineOptions(int argc, char** argv){
	std::string CurrentLine;
	this->ExecPath = argv[0];
	for (int i = this->ExecPath.length() - 1; this->ExecPath[i] != '/'; --i)
		this->ExecPath.erase(i, 1);
	for (int i = 1; i < argc; ++i){
		CurrentLine = argv[i];
		if (CurrentLine == "--enable-debug")
			this->Debug = true;
		else if (CurrentLine == "--enable-logging"){
			this->Logging = true;
			this->MainLogger = new Logger();
		}
		else if (CurrentLine.find("--log-path=") && this->Logging){
			CurrentLine.erase(0, 11);
			std::ofstream TestPath;
			TestPath.open(CurrentLine);
			if (TestPath.is_open()){
				this->MainLogger->SetLogFilePath(CurrentLine);
				this->MainLogger->StartLogger();
				TestPath.close();
			}
			else {
				std::time_t CurrentTime = time(NULL);
				std::string DateTime = std::ctime(&CurrentTime);
				DateTime.erase(DateTime.length() - 1, 1);
				this->MainLogger->SetLogFilePath(this->ExecPath + "Logs/" + DateTime + ".log");
				this->MainLogger->StartLogger();
				this->MainLogger->AddToLog("Failed to set custom log path");
			}
		} else if (CurrentLine == "--enable-something"){
			
		}
	}
	if (this->Logging)
		if (!this->MainLogger->IsLoggerStarted()){
			std::time_t CurrentTime = time(NULL);
			std::string DateTime = std::ctime(&CurrentTime);
			DateTime.erase(DateTime.length() - 1, 1);
			this->MainLogger->SetLogFilePath(this->ExecPath + "Logs/" + DateTime + ".log");
			this->MainLogger->StartLogger();
		}
	if (this->Logging)
		this->MainLogger->AddToLog("Logging started");
	else
		this->MainLogger = NULL;
}

int Game::Initialize(int argc, char** argv){
	InitCommandLineOptions(argc, argv);
	//GLFW Init Start
	if (this->Logging)
		this->MainLogger->AddToLog("Initializing GLFW...");
	if (!glfwInit()){
        if (this->Logging)
			this->MainLogger->AddToLog("GLFW failed to initialize");
		return 1;
	}
	const GLFWvidmode *vidmode = glfwGetVideoMode(glfwGetPrimaryMonitor());
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);
	glfwWindowHint(GLFW_RED_BITS, vidmode->redBits);
	glfwWindowHint(GLFW_GREEN_BITS, vidmode->greenBits);
	glfwWindowHint(GLFW_BLUE_BITS, vidmode->blueBits);
	glfwWindowHint(GLFW_REFRESH_RATE, vidmode->refreshRate);
	if (this->Logging)
		this->MainLogger->AddToLog("GLFW successfully initialized");
	//GLFW Init Complete
	
	//GLFW Window Creation
	if (this->Logging)
		this->MainLogger->AddToLog("Creating main window...");
	MainWindow = glfwCreateWindow(vidmode->width, vidmode->height, "[ALPHA 0.0.1]", glfwGetPrimaryMonitor(), NULL);
	//MainWindow = glfwCreateWindow(1280, 720, "[ALPHA 0.0.1]", NULL, NULL);
	if (!MainWindow){
		glfwTerminate();
		if (this->Logging)
			this->MainLogger->AddToLog("GLFW failed to create MainWindow");
		return 2;
	}
	glfwMakeContextCurrent(MainWindow);
	if (this->Logging)
		this->MainLogger->AddToLog("Main window successfully created");
	//GlFW Windows Created
	
	//Initialize GLAD
	if (this->Logging)
		this->MainLogger->AddToLog("Initializing glad...");
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)){
        glfwTerminate();
		if (this->Logging)
			this->MainLogger->AddToLog("glad failed to initialize");
        return 3;
    }
	if (this->Logging)
		this->MainLogger->AddToLog("glad successfully initialized");
	//GLAD Initializing finished
	
	//Check OpenGL version
	if (this->Logging)
		this->MainLogger->AddToLog("Checking OpenGL version...");
	if ((GLVersion.major < 3) || ((GLVersion.major == 3) && (GLVersion.minor < 3))){
		if (this->Logging)
			this->MainLogger->AddToLog("OpenGL versiong too low. Should be at least 3.3");
		return 4;
	}
	if (this->Logging)
		this->MainLogger->AddToLog("OpenGL version OK");
	//OpenGL version checked
	
	//OpenGL Settings
	if (this->Logging)
		this->MainLogger->AddToLog("Configuring OpenGL settings...");
	try {
		glEnable(GL_DEPTH_TEST);
		glEnable(GL_CULL_FACE);
		glCullFace(GL_BACK);
		glFrontFace(GL_CCW);
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		glViewport(0, 0, vidmode->width, vidmode->height); 
	} catch (...){
		if (this->Logging)
			this->MainLogger->AddToLog("OpenGL settings failed to configure");
		return 5;
	}
	if (this->Logging)
		this->MainLogger->AddToLog("OpenGL settings successfully configured");
	//OpenGL Settings Configured 
	
	//Check if the textures will fit on GPU
	if (this->Logging)
		this->MainLogger->AddToLog("Testing maximum texture size...");
	GLint MaxTexSize;
	glGetIntegerv(GL_MAX_TEXTURE_SIZE, &MaxTexSize);
	if (MaxTexSize < 3840){
		if (this->Logging)
			this->MainLogger->AddToLog("Maximum texture size too low, should be at least 3840");
		return 6;
	}
	if (this->Logging)
		this->MainLogger->AddToLog("Maximum texture size is ok");
	//
	
	//Load Main Font
	if (this->Logging)
		this->MainLogger->AddToLog("Loading main font...");
	if (this->Logging)
		this->MainFont = new Font("../Textures/fonts/MainFont.png", this->MainLogger);
	else
		this->MainFont = new Font("../Textures/fonts/MainFont.png");
	if (!MainFont->IsLoaded()){
		if (this->Logging)
			this->MainLogger->AddToLog("Main font failed to load");
		return 7;
	}
	if (this->Logging)
		this->MainLogger->AddToLog("Main font successfully loaded");
	//Finish loading font
	this->MainCamera = new Camera(vidmode->height, vidmode->width);
	return 0;
}

void Game::UpdateInput(){
    double xpos, ypos;
    glfwGetCursorPos(this->MainWindow, &xpos, &ypos);
    float curPosX = static_cast<float>(xpos);
    float curPosY = static_cast<float>(ypos);
    if (cameraFirstTime)
        cameraFirstTime = false;
    else {
        MainCamera->rotate((curPosY - prevPosY)*7.f*dt, (curPosX - prevPosX)*7.f*dt, 0.f);
    }
    prevPosX = curPosX;
    prevPosY = curPosY;
    float curTime = glfwGetTime();
    if (timeFirstTime)
        timeFirstTime = false;
    else {
        dt = curTime - lastTime;
    }
    lastTime = curTime;
	float speed = 10;
    if (glfwGetKey(this->MainWindow, GLFW_KEY_ESCAPE) == GLFW_PRESS){
        glfwSetWindowShouldClose(MainWindow, GLFW_TRUE);
    }
    if (glfwGetKey(this->MainWindow, GLFW_KEY_W) == GLFW_PRESS){
        MainCamera->move(0.0f, 0.0f, speed*dt);
    }
    if (glfwGetKey(this->MainWindow, GLFW_KEY_A) == GLFW_PRESS){
        MainCamera->move(-speed*dt, 0.0f, 0.0f);
    }
    if (glfwGetKey(this->MainWindow, GLFW_KEY_S) == GLFW_PRESS){
        MainCamera->move(0.0f, 0.0f, -speed*dt);
    }
    if (glfwGetKey(this->MainWindow, GLFW_KEY_D) == GLFW_PRESS){
        MainCamera->move(speed*dt, 0.0f, 0.0f);
    }
    if (glfwGetKey(this->MainWindow, GLFW_KEY_SPACE) == GLFW_PRESS){
        MainCamera->move(0.0f, speed*dt, 0.0f);
    }
    if (glfwGetKey(this->MainWindow, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS){
        MainCamera->move(0.0f, -speed*dt, 0.0f);
    }
}

void Game::ShowMainMenu(){
ReopenMainMenu:
	//Load MenuShader
	Shader *MenuShader;
	if (this->Logging)
		MenuShader = new Shader("../Shaders/vertex_menu.glsl", "../Shaders/fragment_menu.glsl", "", this->MainLogger);
	else
		MenuShader = new Shader("../Shaders/vertex_menu.glsl", "../Shaders/fragment_menu.glsl", "");
	if (MenuShader->ID == 0)
		_Exit(1);
	MenuShader->SetUsed(true);
	//Finish loading MenuShader
		
	//Create Main Menu
	Menu *MainMenu = new Menu("../Textures/menu/MenuTextures.png");
	MenuImage MainLogo;
	/*
	MainLogo.x = -0.9375f;
	MainLogo.y = 0.9f;
	MainLogo.width = 1.875f;
	MainLogo.height = 0.5093f;
	*/
	MainLogo.x = -0.8138125f;
	MainLogo.y = 0.9f;
	MainLogo.width = 1.627625f;
	MainLogo.height = 0.442125f;
	
	MainLogo.uvx = 0.f;
	MainLogo.uvy = 0.f;
	MainLogo.uvwidth = 1.f;
	MainLogo.uvheight = 0.4783f;
	MainMenu->AddImage(MainLogo);
	
	/*0*/std::u32string ButtonString = U"Debuging screen";
	float ButtonHeight = 0.1f;
	Button *NewButton = new Button(-0.9f, 0.3f, /*0.52f*/MainFont->GetStringWidth(ButtonString, ButtonHeight*0.9f)/0.9f, ButtonHeight, 0.f, 0.4782f, 1.f, 0.1304f, ButtonString);
	//NewButton->state = 3;
	MainMenu->AddButton(NewButton);
	
	/*1*/ButtonString = U"Load Game";
	ButtonHeight = 0.1f;
	NewButton = new Button(-0.9f, 0.15f, /*0.52f*/MainFont->GetStringWidth(ButtonString, ButtonHeight*0.9f)/0.9f, ButtonHeight, 0.f, 0.4782f, 1.f, 0.1304f, ButtonString);
	NewButton->state = 3;
	MainMenu->AddButton(NewButton);
	
	/*2*/ButtonString = U"Multiplayer";
	ButtonHeight = 0.1f;
	NewButton = new Button(-0.9f, 0.0f, /*0.52f*/MainFont->GetStringWidth(ButtonString, ButtonHeight*0.9f)/0.9f, ButtonHeight, 0.f, 0.4782f, 1.f, 0.1304f, ButtonString);
	NewButton->state = 3;
	MainMenu->AddButton(NewButton);
	
	/*3*/ButtonString = U"Map editor";
	ButtonHeight = 0.1f;
	NewButton = new Button(-0.9f, -0.15f, /*0.52f*/MainFont->GetStringWidth(ButtonString, ButtonHeight*0.9f)/0.9f, ButtonHeight, 0.f, 0.4782f, 1.f, 0.1304f, ButtonString);
	NewButton->state = 3;
	MainMenu->AddButton(NewButton);
	
	/*4*/ButtonString = U"Settings";
	ButtonHeight = 0.1f;
	NewButton = new Button(-0.9f, -0.3f, /*0.52f*/MainFont->GetStringWidth(ButtonString, ButtonHeight*0.9f)/0.9f, ButtonHeight, 0.f, 0.4782f, 1.f, 0.1304f, ButtonString);
	NewButton->state = 3;
	MainMenu->AddButton(NewButton);
	
	/*5*/ButtonString = U"Quit";
	ButtonHeight = 0.1f;
	NewButton = new Button(-0.9f, -0.45f, /*0.52f*/MainFont->GetStringWidth(ButtonString, ButtonHeight*0.9f)/0.9f, ButtonHeight, 0.f, 0.4782f, 1.f, 0.1304f, ButtonString);
	MainMenu->AddButton(NewButton);
	
	GLuint MenuVAO = 0;
	GLuint MenuTextVAO = 0;
	unsigned long int MenuIndexCount = 0, MenuTextIndexCount = 0;
	MenuIndexCount = MainMenu->GenerateVAOs(MenuVAO, MainFont, MenuTextVAO, MenuTextIndexCount);
	//Finished creating menu
	
	int Next = -1;//Next action
	
	//MainLoop
	bool pressed = false;
	glDisable(GL_DEPTH_TEST);
	while (!glfwWindowShouldClose(MainWindow) && (Next == -1)){
		glfwPollEvents();
		//this->UpdateInput();//Update input
		
		double xpos = 0, ypos = 0;
		glfwGetCursorPos(this->MainWindow, &xpos, &ypos);
		int width, height;
		glfwGetWindowSize(this->MainWindow, &width, &height);
		float curPosX = (static_cast<float>(xpos)/width*2.f - 1.f);
		float curPosY = (static_cast<float>(ypos)/height*2.f - 1.f)*-1.f;
		unsigned int ButtonID;
		if (MainMenu->CheckButtons(curPosX, curPosY, ButtonID)){
			int LMBs = glfwGetMouseButton(this->MainWindow, GLFW_MOUSE_BUTTON_LEFT);
			if (LMBs == GLFW_PRESS){	
				if (!pressed){
					pressed = true;
					if (MainMenu->GetButtonState(ButtonID) == 1)
						MainMenu->ChangeButtonState(ButtonID, 2);
				}
			} else if (LMBs == GLFW_RELEASE){
				if (pressed){
					if (MainMenu->GetButtonState(ButtonID) == 2){
						if (ButtonID == 5)
							glfwSetWindowShouldClose(this->MainWindow, GLFW_TRUE);
						else
							Next = ButtonID;
						MainMenu->ChangeButtonState(ButtonID, 1);
					}
				}
				pressed = false;
				if (MainMenu->GetButtonState(ButtonID) == 0)
					MainMenu->ChangeButtonState(ButtonID, 1);
			}
		} else {
			MainMenu->ResetPressedOrHoveredButton();
		}
		MainMenu->UpdateButtonsAndImages(MenuVAO);
		
		glClearColor(0.f, 0.f, 0.f, 1.f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
		MenuShader->SetUsed(true);
		
		MainMenu->MenuTexture->Bind(0);
		MenuShader->SetUniform1i("menuTexture", MainMenu->MenuTexture->GetSlot());
		glBindVertexArray(MenuVAO);
		glDrawElements(GL_TRIANGLES, MenuIndexCount, GL_UNSIGNED_INT, 0);
		
		MainFont->FontTexture->Bind(0);
		MenuShader->SetUniform1i("menuTexture", MainFont->FontTexture->GetSlot());
		glBindVertexArray(MenuTextVAO);
		glDrawElements(GL_TRIANGLES, MenuTextIndexCount, GL_UNSIGNED_INT, 0);
		
		glfwSwapBuffers(MainWindow);
	}
	glEnable(GL_DEPTH_TEST);
	
	//Menu cleanup
	delete MenuShader;
	delete MainMenu;
	//Cleanup completed
	
	//Next menu switcher
	switch (Next){
		case 0: {//New game menu
			this->GameDebugWindow();
			goto ReopenMainMenu;
			break;
		}
		case 1:{//Load game menu
			
			goto ReopenMainMenu;
			break;
		}
		case 2:{//Online game menu
			
			goto ReopenMainMenu;
			break;
		}
		case 3:{//Launch map editor
			
			goto ReopenMainMenu;
			break;
		}
		case 4:{//Open settings menu
			
			goto ReopenMainMenu;
			break;
		}
		default: {
			CleanUp();
			break;
		} 
	}
}

void Game::CleanUp(){
	this->MainLogger->AddToLog("Terminating...");
	this->MainLogger->StopLogger();
	glfwTerminate();
}

void Game::GameDebugWindow(){
	glfwSetInputMode(this->MainWindow, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
	if (glfwRawMouseMotionSupported())
		glfwSetInputMode(this->MainWindow, GLFW_RAW_MOUSE_MOTION, GLFW_TRUE);
	MaterialObject *TestObject = new MaterialObject();
	TestObject->LoadObject("../Objects/untitled.obj");
	TestObject->GenerateVAOs();
	Shader* MaterialShader = new Shader("../Shaders/vertex_material.glsl", "../Shaders/fragment_material.glsl", "../Shaders/geometry_material.glsl", this->MainLogger);
	TestObject->SetUpShader(MaterialShader);
	glm::mat4 ModelMatrix(1.f);
    ModelMatrix = glm::translate(ModelMatrix, glm::vec3(0.f, 0.f, 0.f));
    ModelMatrix = glm::rotate(ModelMatrix, glm::radians(0.f), glm::vec3(1.f, 0.f, 0.f));
    ModelMatrix = glm::rotate(ModelMatrix, glm::radians(0.f), glm::vec3(0.f, 1.f, 0.f));
    ModelMatrix = glm::rotate(ModelMatrix, glm::radians(0.f), glm::vec3(0.f, 0.f, 1.f));
    ModelMatrix = glm::scale(ModelMatrix, glm::vec3(1.f));
	DirectionalLight Light;
	Light.Direction = glm::vec3(0.f, -1.f, 0.f);
	Light.Strength = 1.f;
	Light.LightColor = glm::vec3(1.f, 1.f, 1.f);
	this->MainCamera->move(0.f, 0.f, -10.f);
	
	
	while (!glfwWindowShouldClose(this->MainWindow)){
		glfwPollEvents();
		this->UpdateInput();//Update input
		glClearColor(1.f, 1.f, 1.f, 1.f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
		MaterialShader->SetUsed(true);
		ModelMatrix = glm::mat4(1.f);
		ModelMatrix = glm::translate(ModelMatrix, glm::vec3(0.f, 0.f, 0.f));
		ModelMatrix = glm::rotate(ModelMatrix, glm::radians(0.f), glm::vec3(1.f, 0.f, 0.f));
		ModelMatrix = glm::rotate(ModelMatrix, glm::radians(0.f), glm::vec3(0.f, 1.f, 0.f));
		ModelMatrix = glm::rotate(ModelMatrix, glm::radians(0.f), glm::vec3(0.f, 0.f, 1.f));
		ModelMatrix = glm::scale(ModelMatrix, glm::vec3(1.f));
		TestObject->DrawObject(ModelMatrix, this->MainCamera, Light);
		ModelMatrix = glm::mat4(1.f);
		ModelMatrix = glm::translate(ModelMatrix, glm::vec3(15.f, 0.f, 0.f));
		ModelMatrix = glm::rotate(ModelMatrix, glm::radians(0.f), glm::vec3(1.f, 0.f, 0.f));
		ModelMatrix = glm::rotate(ModelMatrix, glm::radians(0.f), glm::vec3(0.f, 1.f, 0.f));
		ModelMatrix = glm::rotate(ModelMatrix, glm::radians(0.f), glm::vec3(0.f, 0.f, 1.f));
		ModelMatrix = glm::scale(ModelMatrix, glm::vec3(1.f));
		TestObject->DrawObject(ModelMatrix, this->MainCamera, Light);
		glfwSwapBuffers(MainWindow);
		//glfwSetWindowShouldClose(this->MainWindow, GLFW_TRUE);
	}
	
	delete MaterialShader;
	delete TestObject;
}
