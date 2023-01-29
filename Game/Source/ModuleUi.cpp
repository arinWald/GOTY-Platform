#include "App.h"
#include "Textures.h"
#include "Window.h"
#include "Textures.h"
#include "Render.h"
#include "ModuleUI.h"
#include "Player.h"
#include "Scene.h"
#include "Input.h"
#include "Audio.h"
#include "GuiManager.h"
#include "EntityManager.h"


#include <string.h>
#include <stdio.h>


ModuleUI::ModuleUI() : Module()
{
	name.Create("ui");
}

ModuleUI::~ModuleUI()
{}

// Load assets
bool ModuleUI::Awake(pugi::xml_node& config)
{
	bool ret = true;

	pugi::xml_node uiPathN = config.child("ui");

	fontPath = uiPathN.attribute("fontPath").as_string();

	livesTexturePath = uiPathN.attribute("livesTexturePath").as_string();

	optionsMenuPath = uiPathN.attribute("optionsMenu").as_string();
	settingsMenuPath = uiPathN.attribute("settingsMenu").as_string();
	saveMenuPath = uiPathN.attribute("saveMenu").as_string();

	menuArrowPath = uiPathN.attribute("menuArrow").as_string();

	score = 0;

	currentLevel = 1;
	uiToRender = 0;

	return ret;
}

bool ModuleUI::Start()
{
	bool ret = true;

	char lookupTable[] = { "0123456789.,\"!'-?ABCDEFGHIJKLMNOPQRSTUVWXYZ.:    " };

	font = Load(fontPath, lookupTable, 3);

	box = SDL_Rect({ 0, 0, app->render->camera.w, 30 });

	optionsMenuTex = app->tex->Load(optionsMenuPath);
	settingsMenuTex = app->tex->Load(settingsMenuPath);
	saveMenuTex = app->tex->Load(saveMenuPath);

	menuArrowTex = app->tex->Load(menuArrowPath);

	livesTexture = app->tex->Load(livesTexturePath);
	livesRect = SDL_Rect({ 0,0,12,10 });
	extraLivesRect = SDL_Rect({ 12,0,12,10 });

	return ret;
}

// Update: draw background
bool ModuleUI::Update(float dt)
{
	switch (uiToRender)
	{
	case 1:
		if (app->input->GetKey(SDL_SCANCODE_ESCAPE) == KEY_DOWN && app->scene->gameplayState == Scene::GameplayState::PLAYING)
		{
			app->guiManager->DestroyAllGuiControls();
			uiToRender = 0;
		}
		break;

	case 2:
		if (app->input->GetKey(SDL_SCANCODE_ESCAPE) == KEY_DOWN && app->scene->gameplayState == Scene::GameplayState::PLAYING)
		{
			app->guiManager->DestroyAllGuiControls();
			app->guiManager->CreateGuiControl(GuiControlType::BUTTON, 0, 0, SDL_Rect({ 204, 69, 73, 35 }), 1);
			app->guiManager->CreateGuiControl(GuiControlType::BUTTON, 0, 0, SDL_Rect({ 197, 105, 87, 35 }), 2);
			app->guiManager->CreateGuiControl(GuiControlType::BUTTON, 0, 0, SDL_Rect({ 177, 141, 127, 35 }), 3);
			app->guiManager->CreateGuiControl(GuiControlType::BUTTON, 0, 0, SDL_Rect({ 212, 177, 56, 35 }), 4);
			uiToRender = 1;
		}
		break;

	case 3:
		if (app->input->GetKey(SDL_SCANCODE_ESCAPE) == KEY_DOWN && app->scene->gameplayState == Scene::GameplayState::PLAYING)
		{
			app->guiManager->DestroyAllGuiControls();
			app->guiManager->CreateGuiControl(GuiControlType::BUTTON, 0, 0, SDL_Rect({ 204, 69, 73, 35 }), 1);
			app->guiManager->CreateGuiControl(GuiControlType::BUTTON, 0, 0, SDL_Rect({ 197, 105, 87, 35 }), 2);
			app->guiManager->CreateGuiControl(GuiControlType::BUTTON, 0, 0, SDL_Rect({ 177, 141, 127, 35 }), 3);
			app->guiManager->CreateGuiControl(GuiControlType::BUTTON, 0, 0, SDL_Rect({ 212, 177, 56, 35 }), 4);
			uiToRender = 1;
		}
		break;

	default:
		if (app->input->GetKey(SDL_SCANCODE_ESCAPE) == KEY_DOWN && app->scene->gameplayState == Scene::GameplayState::PLAYING)
		{
			app->guiManager->CreateGuiControl(GuiControlType::BUTTON, 0, 0, SDL_Rect({ 204, 69, 73, 35 }), 1);
			app->guiManager->CreateGuiControl(GuiControlType::BUTTON, 0, 0, SDL_Rect({ 197, 105, 87, 35 }), 2);
			app->guiManager->CreateGuiControl(GuiControlType::BUTTON, 0, 0, SDL_Rect({ 177, 141, 127, 35 }), 3);
			app->guiManager->CreateGuiControl(GuiControlType::BUTTON, 0, 0, SDL_Rect({ 212, 177, 56, 35 }), 4);
			uiToRender = 1;
		}
		break;
	}
	return true;
}

bool ModuleUI::PostUpdate()
{
	app->render->DrawRectangle(box, 33, 31, 48, 255, true, false);

	int uiposx = 10;
	BlitText(uiposx, 5, font, "LEVEL", false);
	IntToString(shortNumberText, currentLevel, 2);
	BlitText(uiposx + 55, 5, font, shortNumberText, false);

	BlitText(uiposx + 90, 5, font, "HEALTH", false);
	/*IntToString(shortNumberText, app->player->health, 2);
	BlitText(uiposx + 155, 5, font, shortNumberText, false);
	*/
	

	for (int i = 0; i < app->scene->player->playerlives; i++)
	{
		if (i < 3)
		{
			app->render->DrawTexture(livesTexture, uiposx + 150 + (i * 15), 3, &livesRect, 0, 0, 0, 0, false);
		}
		else
		{
			app->render->DrawTexture(livesTexture, uiposx + 150 + (i * 15), 3, &extraLivesRect, 0, 0, 0, 0, false);

		}
	}

	BlitText(uiposx + 320, 5, font, "SCORE", false);
	IntToDynamicString(scoreText, score);
	BlitText(uiposx + 375, 5, font, scoreText, false);

	switch (uiToRender)
	{
	case 1:
		app->render->DrawTexture(optionsMenuTex, 0, 0, NULL, 0, 0, 0, 0, false);
		break;

	case 2:
		app->render->DrawTexture(settingsMenuTex, 0, 0, NULL, 0, 0, 0, 0, false);
		break;

	case 3:
		app->render->DrawTexture(saveMenuTex, 0, 0, NULL, 0, 0, 0, 0, false);
		break;

	default:
		break;
	}

	return true;
}

bool ModuleUI::CleanUp()
{

	bool ret = true;

	UnLoad(font);

	return ret;
}


// Load new texture from file path
int ModuleUI::Load(const char* texture_path, const char* characters, uint rows)
{
	int id = -1;

	if (texture_path == nullptr || characters == nullptr || rows == 0)
	{
		return id;
	}

	SDL_Texture* tex = app->tex->Load(texture_path);

	if (tex == nullptr || strlen(characters) >= MAX_FONT_CHARS)
	{
		return id;
	}

	id = 0;
	for (; id < MAX_FONTS; ++id)
		if (fonts[id].texture == nullptr)
			break;

	if (id == MAX_FONTS)
	{
		return id;
	}

	Font& font = fonts[id];

	font.texture = tex;
	font.rows = rows;

	// TODO 1: Finish storing font data

	// totalLength ---	length of the lookup table
	// table ---------  All characters displayed in the same order as the texture
	// columns -------  Amount of chars per row of the texture
	// char_w --------	Width of each character
	// char_h --------	Height of each character

	strcpy_s(fonts[id].table, MAX_FONT_CHARS, characters);
	font.totalLength = strlen(characters);
	font.columns = fonts[id].totalLength / rows;

	uint tex_w, tex_h;
	app->tex->GetSize(tex, tex_w, tex_h);
	font.char_w = tex_w / font.columns;
	font.char_h = tex_h / font.rows;


	k++;

	return id;
}

void ModuleUI::UnLoad(int font_id)
{
	if (font_id >= 0 && font_id < MAX_FONTS && fonts[font_id].texture != nullptr)
	{
		app->tex->UnLoad(fonts[font_id].texture);
		fonts[font_id].texture = nullptr;
	}
	k--;
}

void ModuleUI::BlitText(int x, int y, int font_id, const char* text, bool useCamera) const
{
	if (text == nullptr || font_id < 0 || font_id >= MAX_FONTS || fonts[font_id].texture == nullptr)
	{
		return;
	}

	const Font* font = &fonts[font_id];
	SDL_Rect spriteRect;
	uint len = strlen(text);

	spriteRect.w = font->char_w;
	spriteRect.h = font->char_h;

	for (uint i = 0; i < len; ++i)
	{
		uint charIndex = 0;

		// Find the location of the current character in the lookup table
		for (uint j = 0; j < font->totalLength; ++j)
		{
			if (font->table[j] == text[i])
			{
				charIndex = j;
				break;
			}
		}

		// Retrieve the position of the current character in the sprite
		spriteRect.x = spriteRect.w * (charIndex % font->columns);
		spriteRect.y = spriteRect.h * (charIndex / font->columns);

		app->render->DrawTexture(font->texture, x, y, &spriteRect, 1.0f, 0.0f, INT_MAX, INT_MAX, false);

		// Advance the position where we blit the next character
		x += spriteRect.w;
	}
}

void ModuleUI::IntToDynamicString(char* buffer, int k)
{

	for (int i = 0; i < DYNAMIC_TEXT_LEN; i++) {
		buffer[i] = '0';
	}

	buffer[DYNAMIC_TEXT_LEN] = 0;

	int i = DYNAMIC_TEXT_LEN - 1;
	while (k != 0) {
		if (i < 0) break;
		buffer[i--] += k % 10;
		k /= 10;
	}
}

void ModuleUI::IntToString(char* buffer, int k, int length)
{

	for (int i = 0; i < length; i++) {
		buffer[i] = '0';
	}

	buffer[length] = 0;

	int i = length - 1;
	while (k != 0) {
		if (i < 0) break;
		buffer[i--] += k % 10;
		k /= 10;
	}
}