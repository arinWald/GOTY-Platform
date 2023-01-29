#include "App.h"
#include "Input.h"
#include "Textures.h"
#include "Audio.h"
#include "Render.h"
#include "Window.h"
#include "Scene.h"
#include "EntityManager.h"
#include "Map.h"
#include "Player.h"
#include "Pathfinding.h"
#include "GuiManager.h"
#include"ModuleUI.h"
#include "Item.h"
#include"GuiSlider.h"
#include <iostream>
using namespace std;

#include "Defs.h"
#include "Log.h"
#include "DynArray.h"

#include "SDL_mixer/include/SDL_mixer.h"

Scene::Scene() : Module()
{
	name.Create("scene");
}

// Destructor
Scene::~Scene()
{}

// Called before render is available
bool Scene::Awake(pugi::xml_node& config)
{
	LOG("Loading Scene");
	bool ret = true;

	// iterate all objects in the scene
	// Check https://pugixml.org/docs/quickstart.html#access
	for (pugi::xml_node itemNode = config.child("item"); itemNode; itemNode = itemNode.next_sibling("item"))
	{
		Item* item = (Item*)app->entityManager->CreateEntity(EntityType::ITEM);
		item->parameters = itemNode;
	}

	player = (Player*)app->entityManager->CreateEntity(EntityType::PLAYER);
	player->parameters = config.child("player");

	bat = (Bat*)app->entityManager->CreateEntity(EntityType::BAT);
	bat->parameters = config.child("bat");

	walkEnemy = (WalkEnemy*)app->entityManager->CreateEntity(EntityType::WALKENEMY);
	walkEnemy->parameters = config.child("walkenemy");

	//iterate all enemies
	/*for (pugi::xml_node enemyNode = config.child("walkenemy"); enemyNode; enemyNode = enemyNode.next_sibling("walkenemy"))
	{
		WalkEnemy* enemy = (WalkEnemy*)app->entityManager->CreateEntity(EntityType::WALKENEMY);
		enemy->parameters = enemyNode;
	}*/
	
	pugi::xml_node logo = config.child("logo");
	logotexturePath = logo.attribute("texturepath").as_string();

	pugi::xml_node intro = config.child("intro");
	introtexturePath = intro.attribute("texturepath").as_string();
	
	pugi::xml_node game_over = config.child("game_over");
	game_over_texturePath = game_over.attribute("texturepath").as_string();

	pugi::xml_node win_screen = config.child("win_screen");
	win_screen_texturePath = win_screen.attribute("texturepath").as_string();

	pugi::xml_node level_1_song = config.child("level_1_song");
	level1SongPath = level_1_song.attribute("audiopath").as_string();

	pugi::xml_node silence_song = config.child("silence_song");
	silenceSongPath = silence_song.attribute("audiopath").as_string();

	pugi::xml_node victory_song = config.child("victory_song");
	victorySongPath = victory_song.attribute("audiopath").as_string();
	return ret;
}

// Called before the first frame
bool Scene::Start()
{

	// L04: DONE 7: Set the window title with map/tileset info
	SString title("Peepee's Adventure - Map:%dx%d Tiles:%dx%d Tilesets:%d",
		app->map->mapData.width,
		app->map->mapData.height,
		app->map->mapData.tileWidth,
		app->map->mapData.tileHeight,
		app->map->mapData.tilesets.Count());

	app->win->SetTitle(title.GetString());

	
	logo = app->tex->Load(logotexturePath);
	intro = app->tex->Load(introtexturePath);
	game_over = app->tex->Load(game_over_texturePath);
	win_screen = app->tex->Load(win_screen_texturePath);

	godMode = false;

	bool retLoad = app->map->Load();

	// L12 Create walkability map
	if (retLoad) {
		int w, h;
		uchar* data = NULL;

		bool retWalkMap = app->map->CreateWalkabilityMap(w, h, &data);
		if (retWalkMap) app->pathfinding->SetMap(w, h, data);

		RELEASE_ARRAY(data);

	}

	//Sets the camera to be centered in isometric map
	if (app->map->mapData.type == MapTypes::MAPTYPE_ISOMETRIC) {
		uint width, height;
		app->win->GetWindowSize(width, height);
		app->render->camera.x = width / 2;

		// Texture to highligh mouse position 
		mouseTileTex = app->tex->Load("Assets/Maps/path.png");

		// Texture to show path origin 
		originTex = app->tex->Load("Assets/Maps/x.png");
	}

	if (app->map->mapData.type == MapTypes::MAPTYPE_ORTHOGONAL) {

		// Texture to highligh mouse position 
		mouseTileTex = app->tex->Load("Assets/Maps/path_square.png");

		// Texture to show path origin 
		originTex = app->tex->Load("Assets/Maps/x_square.png");
	}

	checkPoint = app->physics->CreateRectangleSensor(1382, 256, 32, 64, bodyType::STATIC);

	// L15: DONE 2: Declare a GUI Button and create it using the GuiManager
	uint w, h;
	app->win->GetWindowSize(w, h);



	return true;
}

// Called each loop iteration
bool Scene::PreUpdate()
{
	return true;
}

// Called each loop iteration
bool Scene::Update(float dt)
{
	if (app->input->GetKey(SDL_SCANCODE_RETURN) == KEY_DOWN && gameplayState == LOGO_SCREEN)
	{
		FadeToNewState(TITLE_SCREEN);
	}
	/*if (app->input->GetKey(SDL_SCANCODE_0) == KEY_DOWN && gameplayState == PLAYING)
	{
		FadeToNewState(GAME_OVER_SCREEN);
	}*/

	if (gameplayState == TITLE_SCREEN && app->input->GetKey(SDL_SCANCODE_RETURN) == KEY_DOWN)
	{
		app->scene->FadeToNewState(PLAYING);
		LOG("LOAD REQUESTED");
	}
	/*if (gameplayState == GAME_OVER_SCREEN && app->input->GetKey(SDL_SCANCODE_RETURN) == KEY_DOWN)
	{
		app->scene->FadeToNewState(TITLE_SCREEN);
	}*/
	//if (gameplayState == WIN_SCREEN && app->input->GetKey(SDL_SCANCODE_RETURN) == KEY_DOWN)
	//{
	//	app->scene->FadeToNewState(TITLE_SCREEN);
	//}
	if (player->playerlives <= 0) {
		app->scene->FadeToNewState(app->scene->GAME_OVER_SCREEN);

	}

	if (player->isWin && gameplayState == PLAYING)
	{
		player->playerlives = 3;
		app->scene->FadeToNewState(app->scene->WIN_SCREEN);
	}

	if (gameplayState == PLAYING) {
		if (app->input->GetKey(SDL_SCANCODE_F5) == KEY_DOWN)
			app->SaveGameRequest();

		if (app->input->GetKey(SDL_SCANCODE_F6) == KEY_DOWN)
			app->LoadGameRequest();

		//if (app->input->GetKey(SDL_SCANCODE_UP) == KEY_REPEAT)
		//	app->render->camera.y += 3;

		//if (app->input->GetKey(SDL_SCANCODE_DOWN) == KEY_REPEAT)
		//	app->render->camera.y -= 3;

		//if (app->input->GetKey(SDL_SCANCODE_LEFT) == KEY_REPEAT)
		//	app->render->camera.x += 3;

		//if (app->input->GetKey(SDL_SCANCODE_RIGHT) == KEY_REPEAT)
		//	app->render->camera.x -= 3;


		if (app->input->GetKey(SDL_SCANCODE_F1) == KEY_DOWN)
		{
			player->playerlives = 3;
			player->ChangePosition(player->initialPosX, player->initialPosY);
		}
		/*if (app->input->GetKey(SDL_SCANCODE_F2) == KEY_DOWN)
			player->playerlives = 0;*/
		// Start from the beginning of the current level
		if (app->input->GetKey(SDL_SCANCODE_F3) == KEY_DOWN)
		{
			player->ChangePosition(player->initialPosX, player->initialPosY);
		}
		if (app->input->GetKey(SDL_SCANCODE_F10) == KEY_DOWN)
		{
			if (!godMode) godMode = true;
			else godMode = false;
		}
	}

	if (app->input->GetKey(SDL_SCANCODE_F11) == KEY_DOWN)//caps fps to 30 or 60
	{
		if (app->FPS == 60)
		{
			app->FPS = 30;
		}
		else
		{
			app->FPS = 60;
		}
	}

	if (gameplayState != targetState)
	{
		//Abans era 0.05f
		currentFade += 0.02f;
		if (currentFade >= 0.0f)
		{
			currentFade = 0.0f;
			ChangeGameplayState(targetState);
		}
	}
	else if (currentFade > 0.0f)
	{
		currentFade -= 0.02f;
	}
	else if (currentFade <= 0.0f)
	{
		currentFade = 0.0f;
		fading = false;
	}

	app->render->DrawTexture(game_over, 0, 0);
	
	// Draw map
	if (app->scene->gameplayState == PLAYING)
	{
		app->map->Draw();
	}

	app->guiManager->Draw();

	//PATHFINDING WALK ENEMY
	if (walkEnemy->currentMoveState == WalkEnemy::MoveState::CHASING)
	{
		origin.x = walkEnemy->pbody->body->GetPosition().x;
		origin.y = walkEnemy->pbody->body->GetPosition().y;
		iPoint destination;
		destination.x = player->pbody->body->GetPosition().x;
		destination.y = player->pbody->body->GetPosition().y;
		app->pathfinding->ClearLastPath();
		app->pathfinding->CreatePath(origin, destination);
		// Get the latest calculated path and draw
		const DynArray<iPoint>* path = app->pathfinding->GetLastPath();
		for (uint i = 0; i < path->Count(); ++i)
		{
			iPoint pos = app->map->MapToWorld(path->At(i)->x, path->At(i)->y);
			if (i == 1)
			{
				walkEnemy->objective.x = PIXEL_TO_METERS(pos.x);
				walkEnemy->objective.y = PIXEL_TO_METERS(pos.y);
			}
			if (app->physics->debug) app->render->DrawTexture(mouseTileTex, pos.x, pos.y);
		}

		// Debug pathfinding
		if (app->physics->debug)
		{
		iPoint originScreen = app->map->MapToWorld(origin.x, origin.y);
		if (app->physics->debug) app->render->DrawTexture(originTex, originScreen.x, originScreen.y);
	    }
	}

	return true;
}

// Called each loop iteration
bool Scene::PostUpdate()
{
	bool ret = true;

	if (app->input->GetKey(SDL_SCANCODE_ESCAPE) == KEY_DOWN || (gameplayState == WIN_SCREEN && app->input->GetKey(SDL_SCANCODE_RETURN) == KEY_DOWN)
		|| (gameplayState == GAME_OVER_SCREEN && app->input->GetKey(SDL_SCANCODE_RETURN) == KEY_DOWN))
	{
		ret = false;

	}

	if (gameplayState == LOGO_SCREEN)
	{
		app->render->DrawTexture(logo, 0, 0);
	}

	if (gameplayState == TITLE_SCREEN)
	{
		app->render->DrawTexture(intro, 0, 0);

		SDL_Rect continueRect;

		if (continueButtonDisabled)
			continueRect = SDL_Rect({ 0,120,100,20 });
		else
			continueRect = continueButtonAnim.GetCurrentFrame();

		app->render->DrawTexture(continueButtonTex, buttonsPosX, buttonsPosY, &continueRect, 0, 0, 0, 0, false);

		SDL_Rect newGameRect = newGameButtonAnim.GetCurrentFrame();
		app->render->DrawTexture(newGameButtonTex, buttonsPosX, buttonsPosY + 24, &newGameRect, 0, 0, 0, 0, false);

		SDL_Rect settingsRect = settingsButtonAnim.GetCurrentFrame();
		app->render->DrawTexture(settingsButtonTex, buttonsPosX, buttonsPosY + 48, &settingsRect, 0, 0, 0, 0, false);

		SDL_Rect creditsRect = creditsButtonAnim.GetCurrentFrame();
		app->render->DrawTexture(creditsButtonTex, buttonsPosX, buttonsPosY + 72, &creditsRect, 0, 0, 0, 0, false);

		SDL_Rect exitRect = exitButtonAnim.GetCurrentFrame();
		app->render->DrawTexture(exitButtonTex, buttonsPosX, buttonsPosY + 96, &exitRect, 0, 0, 0, 0, false);

	}

	if (gameplayState == GAME_OVER_SCREEN)
	{
		app->render->DrawTexture(game_over, 0, 0);

	}
	if (gameplayState == WIN_SCREEN)
	{
		app->render->DrawTexture(win_screen, 0, 0);

	}
	return ret;
}


bool Scene::LoadState(pugi::xml_node& data)
{
	player->ChangePosition(data.child("player").attribute("x").as_int() , data.child("player").attribute("y").as_int());

	walkEnemy->ChangePosition(data.child("walkEnemy").attribute("x").as_int(), data.child("walkEnemy").attribute("y").as_int());
	walkEnemy->ChangeMoveState(data.child("walkEnemy").attribute("moveState").as_int());

	return true;
}

bool Scene::SaveState(pugi::xml_node& data)
{
	pugi::xml_node playerNode = data.append_child("player");

	playerNode.append_attribute("x") = player->position.x;
	playerNode.append_attribute("y") = player->position.y;

	pugi::xml_node walkEnemyNode = data.append_child("walkEnemy");
	walkEnemyNode.append_attribute("x") = walkEnemy->position.x;
	walkEnemyNode.append_attribute("y") = walkEnemy->position.y;
	walkEnemyNode.append_attribute("moveState") = (int*)walkEnemy->currentMoveState;

	pugi::xml_node batNode = data.append_child("batEnemy");
	batNode.append_attribute("x") = bat->position.x;
	batNode.append_attribute("y") = bat->position.y;
	batNode.append_attribute("state") = (int*)bat->state;

	return true;
}

void Scene::FadeToNewState(GameplayState newState)
{
	if (gameplayState == newState)
		return;
	if (fading)
		return;
	targetState = newState;
	currentFade = 0.0f;
	fading = true;
}

void Scene::ChangeGameplayState(GameplayState newState)
{
	if (gameplayState == newState) return;

	switch (newState)
	{
	case LOGO_SCREEN:
		gameplayState = LOGO_SCREEN;
		app->render->camera.x = 0;
		app->render->camera.y = 0;
		break;
	case PLAYING:
		gameplayState = PLAYING;
		//app->map->Load();
		app->audio->PlayMusic(level1SongPath, 0);
		player->ChangePosition(player->initialPosX, player->initialPosY);
		break;
	case TITLE_SCREEN:
		gameplayState = TITLE_SCREEN;
		app->map->Load();
		player->isDead = false;
		player->isWin = false;
		player->playerlives = 3;
		screenDisplayAnim = &titleScreenAnim;
		gameplayState = TITLE_SCREEN;
		app->map->CleanUp();
		app->render->camera.x = 0;
		app->render->camera.y = 0;
		/*app->moduleUI->uiToRender = 0;*/
		app->guiManager->CreateGuiControl(GuiControlType::BUTTON, 0, 0, SDL_Rect({ buttonsPosX, buttonsPosY, 101, 24 }), 1);
		app->guiManager->CreateGuiControl(GuiControlType::BUTTON, 0, 0, SDL_Rect({ buttonsPosX, buttonsPosY + 24, 101, 24 }), 2);
		app->guiManager->CreateGuiControl(GuiControlType::BUTTON, 0, 0, SDL_Rect({ buttonsPosX, buttonsPosY + 48, 101, 24 }), 3);
		app->guiManager->CreateGuiControl(GuiControlType::BUTTON, 0, 0, SDL_Rect({ buttonsPosX, buttonsPosY + 72, 101, 24 }), 4);
		app->guiManager->CreateGuiControl(GuiControlType::BUTTON, 0, 0, SDL_Rect({ buttonsPosX, buttonsPosY + 96, 101, 24 }), 5);
		break;
	case GAME_OVER_SCREEN:
		gameplayState = GAME_OVER_SCREEN;
		app->audio->PlayMusic(silenceSongPath, 0);
		//app->map->CleanUp();
		app->render->camera.x = 0;
		app->render->camera.y = 0;
		break;
	case WIN_SCREEN:
		app->audio->PlayMusic(victorySongPath, 0);
		gameplayState = WIN_SCREEN;
		//app->map->CleanUp();
		app->render->camera.x = 0;
		app->render->camera.y = 0;
	}
}


// Called before quitting
bool Scene::CleanUp()
{
	LOG("Freeing scene");

	return true;
}

bool Scene::OnGuiMouseClickEvent(GuiControl* control)
{
	// L15: DONE 5: Implement the OnGuiMouseClickEvent method
	if (fading)
		return true;

	if (gameplayState == TITLE_SCREEN)
	{
		GuiSlider* g;
		switch (control->type)
		{
		case GuiControlType::BUTTON:
			switch (control->id)
			{
			case 1:
				app->guiManager->DestroyAllGuiControls();
				continueButtonPressed = true;
				break;

			case 2:
				app->guiManager->DestroyAllGuiControls();
				FadeToNewState(PLAYING);
				break;

			case 3:
				app->guiManager->DestroyAllGuiControls();
				/*FadeToNewState(TITLE_MENU);*/
				app->guiManager->CreateGuiControl(GuiControlType::SLIDER, 0, 0, SDL_Rect({ 220, 62, 116, 23 }), 1);
				app->guiManager->CreateGuiControl(GuiControlType::SLIDER, 0, 0, SDL_Rect({ 220, 105, 116, 23 }), 2);
				app->guiManager->CreateGuiControl(GuiControlType::CHECKBOX, 0, 0, SDL_Rect({ 316, 147, 20, 20 }), 1);
				app->guiManager->CreateGuiControl(GuiControlType::CHECKBOX, 0, 0, SDL_Rect({ 316, 186, 20, 20 }), 2);
				break;

			case 4:
				app->guiManager->DestroyAllGuiControls();
				/*FadeToNewState(CREDITS_SCREEN);*/
				break;

			case 5:
				exit = true;
				break;

			default:
				break;
			}
			break;

		default:
			break;
		}
	}
	else if (gameplayState == TITLE_MENU)
	{
		GuiSlider* g;
		int volume;
		switch (control->type)
		{
		case GuiControlType::CHECKBOX:
			switch (control->id)
			{
			case 1:
				/*app->win->SwitchFullScreen();*/
				break;

			case 2:
				LOG("VSYNC switched.");
				break;

			default:
				break;
			}
			break;
		case GuiControlType::SLIDER:
			g = (GuiSlider*)control;
			switch (control->id)
			{
			case 1:

				volume = (g->value / 100) * MIX_MAX_VOLUME;
				app->guiManager->musicVolume = g->value;
				Mix_VolumeMusic(volume);
				break;
			case 2:
				volume = (g->value / 100) * MIX_MAX_VOLUME;
				app->guiManager->fxVolume = g->value;
				Mix_Volume(-1, volume);
				break;
			}
			break;

		default:
			break;
		}
	}
	else if (gameplayState == PLAYING)
	{
		GuiSlider* g;
		int volume;
		switch (control->type)
		{
		case GuiControlType::BUTTON:
			switch (control->id)
			{
			case 1:
				app->guiManager->DestroyAllGuiControls();
				app->moduleUI->uiToRender = 0;
				break;

			case 2:
				app->guiManager->DestroyAllGuiControls();
				app->guiManager->CreateGuiControl(GuiControlType::SLIDER, 0, 0, SDL_Rect({ 188, 93, 116, 23 }), 1);
				app->guiManager->CreateGuiControl(GuiControlType::SLIDER, 0, 0, SDL_Rect({ 188, 141, 116, 23 }), 2);
				app->guiManager->CreateGuiControl(GuiControlType::CHECKBOX, 0, 0, SDL_Rect({ 275, 166, 20, 20 }), 1);
				app->guiManager->CreateGuiControl(GuiControlType::CHECKBOX, 0, 0, SDL_Rect({ 238, 189, 20, 20 }), 2);
				app->moduleUI->uiToRender = 2;
				break;

			case 3:
				app->guiManager->DestroyAllGuiControls();
				FadeToNewState(TITLE_SCREEN);
				break;

			case 4:
				app->guiManager->DestroyAllGuiControls();
				app->guiManager->CreateGuiControl(GuiControlType::BUTTON, 0, 0, SDL_Rect({ 199, 150, 41, 7 }), 5);
				app->guiManager->CreateGuiControl(GuiControlType::BUTTON, 0, 0, SDL_Rect({ 240, 150, 35, 7 }), 6);
				app->moduleUI->uiToRender = 3;
				break;

			case 5:
			/*	app->entities->GetPlayer()->saveOnce1 = true;
				app->entities->GetPlayer()->saveOnce2 = true;
				app->RequestSave();*/
				LOG("SAVE REQUESTED");
				exit = true;
				break;

			case 6:
				exit = true;
				break;

			default:
				break;
			}
			break;

		case GuiControlType::CHECKBOX:
			switch (control->id)
			{
			case 1:
				/*app->win->SwitchFullScreen();*/
				break;

			case 2:
				LOG("VSYNC switched.");
				break;

			default:
				break;
			}
			break;

		case GuiControlType::SLIDER:
			g = (GuiSlider*)control;
			switch (control->id)
			{
			case 1:
				volume = (g->value / 100) * MIX_MAX_VOLUME;
				app->guiManager->musicVolume = g->value;
				Mix_VolumeMusic(volume);
				break;
			case 2:
				volume = (g->value / 100) * MIX_MAX_VOLUME;
				app->guiManager->fxVolume = g->value;
				Mix_Volume(-1, volume);
				break;
			}
			break;

		default:
			break;
		}
	}
	return true;
}