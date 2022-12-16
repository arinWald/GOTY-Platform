#ifndef __SCENE_H__
#define __SCENE_H__

#include "Module.h"
#include "Player.h"
#include "Item.h"
#include "Mushroom.h"

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

public:

	//L02: DONE 3: Declare a Player attribute 
	Player* player;

	Mushroom* enemy1;

	GameplayState gameplayState = LOGO_SCREEN;
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

	Animation* screenDisplayAnim;

	bool godMode;

private:
	SDL_Texture* img;
	float currentFade = 0.0f;
	GameplayState targetState = gameplayState;

	SDL_Texture* mouseTileTex = nullptr;
	SDL_Texture* originTex = nullptr;

	// L12: Debug pathfing
	iPoint origin;
	bool originSelected = false;
};

#endif // __SCENE_H__