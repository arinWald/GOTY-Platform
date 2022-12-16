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
#include "Mushroom.h"
#include "Entity.h"
#include "EntityManager.h"
#include "Pathfinding.h"
#include <iostream>
using namespace std;

#include "Defs.h"
#include "Log.h"
#include "DynArray.h"

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
	//L02: DONE 3: Instantiate the player using the entity manager

	player = (Player*)app->entityManager->CreateEntity(EntityType::PLAYER);
	player->parameters = config.child("player");
	
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
	// L03: DONE 3: Request App to Load / Save when pressing the keys F5 (save) / F6 (load)
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

	int mouseX, mouseY;
	app->input->GetMousePosition(mouseX, mouseY);

	iPoint mouseTile = iPoint(0, 0);

	//if (app->map->mapData.type == MapTypes::MAPTYPE_ISOMETRIC) {
	//	mouseTile = app->map->WorldToMap(mouseX - app->render->camera.x - app->map->mapData.tileWidth / 2,
	//		mouseY - app->render->camera.y - app->map->mapData.tileHeight / 2);
	//}
	//if (app->map->mapData.type == MapTypes::MAPTYPE_ORTHOGONAL) {
	//	mouseTile = app->map->WorldToMap(mouseX - app->render->camera.x,
	//		mouseY - app->render->camera.y);
	//}

	//QUAN CAMERA ES MOU, EL QUADRAT TMB AL DOBLE DE VELOCITAT. MULTIPLICAR O DIVIDIR PER SCALE?
	if (app->map->mapData.type == MapTypes::MAPTYPE_ISOMETRIC) {
		mouseTile = app->map->WorldToMap(mouseX - app->render->camera.x / app->win->GetScale() - app->map->mapData.tileWidth / 2,
			mouseY - app->render->camera.y / app->win->GetScale() - app->map->mapData.tileHeight / 2);
	}
	if (app->map->mapData.type == MapTypes::MAPTYPE_ORTHOGONAL) {
		mouseTile = app->map->WorldToMap(mouseX - app->render->camera.x,
			mouseY - app->render->camera.y);
	}

	//cout << "MouseX: " << mouseX << " CameraX: " << app->render->camera.x << endl;

	//Convert again the tile coordinates to world coordinates to render the texture of the tile
	iPoint highlightedTileWorld = app->map->MapToWorld(mouseTile.x, mouseTile.y);
	app->render->DrawTexture(mouseTileTex, highlightedTileWorld.x, highlightedTileWorld.y);

	//Test compute path function
	if (app->input->GetMouseButtonDown(SDL_BUTTON_LEFT) == KEY_DOWN)
	{
		if (originSelected == true && gameplayState == PLAYING)
		{
			app->pathfinding->CreatePath(origin, mouseTile, "land");
			originSelected = false;
		}
		else
		{
			origin = mouseTile;
			originSelected = true;
			app->pathfinding->ClearLastPath();
		}
	}

	// L12: Get the latest calculated path and draw
	const DynArray<iPoint>* path = app->pathfinding->GetLastPath();
	for (uint i = 0; i < path->Count(); ++i)
	{
		iPoint pos = app->map->MapToWorld(path->At(i)->x, path->At(i)->y);
		app->render->DrawTexture(mouseTileTex, pos.x, pos.y);
	}

	// L12: Debug pathfinding
	iPoint originScreen = app->map->MapToWorld(origin.x, origin.y);
	app->render->DrawTexture(originTex, originScreen.x, originScreen.y);

	return true;
}



bool Scene::LoadState(pugi::xml_node& data)
{
	player->ChangePosition(data.child("player").attribute("x").as_int() , data.child("player").attribute("y").as_int());

	return true;
}

bool Scene::SaveState(pugi::xml_node& data)
{
	pugi::xml_node playerNode = data.append_child("player");

	playerNode.append_attribute("x") = player->position.x;
	playerNode.append_attribute("y") = player->position.y;

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
		app->render->camera.x = 0;
		app->render->camera.y = 0;
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

// Called before quitting
bool Scene::CleanUp()
{
	LOG("Freeing scene");

	return true;
}