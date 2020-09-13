#define STB_IMAGE_IMPLEMENTATION
#include "Game.h"
int main(int argc, char** argv){
	Game MainGame = Game();
	int InitRes = MainGame.Initialize(argc, argv);
	if (InitRes == 0){
		MainGame.ShowMainMenu();
		MainGame.CleanUp();
	} else
		return InitRes;
	return 0;
}
