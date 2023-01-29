#ifndef __SCENE_H__
#define __SCENE_H__

#include "Module.h"
#include "Player.h"
#include "WalkEnemy.h"
#include "Bat.h"
#include "Item.h"
#include "GuiButton.h"

struct SDL_Texture;

class Scene : public Module
{
public:

	enum GameplayState
	{
		TITLE_SCREEN,
		PLAYING,
		GAME_OVER_SCREEN,
		LOGO_SCREEN,
		WIN_SCREEN,
	};

	Scene();

	// Destructor
	virtual ~Scene();

	// Called before render is available
	bool Awake(pugi::xml_node& config);

	// Called before the first frame
	bool Start();

	// Called before all Updates
	bool PreUpdate();

	// Called each loop iteration
	bool Update(float dt);

	// Called before all Updates
	bool PostUpdate();

	// Called before quitting
	bool CleanUp();

	bool LoadState(pugi::xml_node&);
	bool SaveState(pugi::xml_node&);

	void ChangeGameplayState(GameplayState newState);
	void FadeToNewState(GameplayState newState);

	bool Scene::OnGuiMouseClickEvent(GuiControl* control);

public:

	Player* player;
	Bat* bat;
	WalkEnemy* walkEnemy;

	GameplayState gameplayState = LOGO_SCREEN;

	SDL_Texture* newGameButtonTex = nullptr;
	SDL_Texture* settingsButtonTex = nullptr;
	SDL_Texture* creditsButtonTex = nullptr;
	SDL_Texture* exitButtonTex = nullptr;
	SDL_Texture* creditsTex = nullptr;

	bool fading = false;

	SDL_Texture* logo;
	const char* logotexturePath;

	SDL_Texture* intro;
	const char* introtexturePath;

	SDL_Texture* game_over;
	const char* game_over_texturePath;

	SDL_Texture* win_screen;
	const char* win_screen_texturePath;

	const char* level1SongPath;

	const char* silenceSongPath;

	const char* victorySongPath;

	const char* musicPath;
	const char* screenTexturePath;
	const char* titleMenuPath;
	const char* titleButtonsPath;
	const char* creditsPath;

	Animation* screenDisplayAnim;
	Animation continueButtonAnim;
	Animation newGameButtonAnim;
	Animation settingsButtonAnim;
	Animation creditsButtonAnim;
	Animation exitButtonAnim;

	bool godMode;
	bool continueButtonPressed = false;
	bool continueButtonDisabled = true;
	bool exit = false;
	bool ret = true;

	int buttonsPosX = 280;
	int buttonsPosY = 355;

	PhysBody* checkPoint;

private:
	SDL_Texture* img;
	float currentFade = 0.0f;
	GameplayState targetState = gameplayState;

	SDL_Texture* mouseTileTex = nullptr;
	SDL_Texture* originTex = nullptr;

	iPoint origin;
	bool originSelected = false;

	GuiButton* button1;
	GuiButton* button2;
};

#endif // __SCENE_H__