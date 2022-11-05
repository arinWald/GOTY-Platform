#ifndef __SCENE_H__
#define __SCENE_H__

#include "Module.h"
#include "Player.h"
#include "Item.h"

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

	GameplayState gameplayState = TITLE_SCREEN;
	bool fading = false;
	SDL_Texture* intro;
	const char* introtexturePath;

	SDL_Texture* game_over;
	const char* game_over_texturePath;

	Animation* screenDisplayAnim;
private:
	SDL_Texture* img;
	float currentFade = 0.0f;
	GameplayState targetState = gameplayState;
};

#endif // __SCENE_H__