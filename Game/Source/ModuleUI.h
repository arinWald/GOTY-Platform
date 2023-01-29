#ifndef __ModuleUI_H__
#define __ModuleUI_H__

#include "Module.h"
#include "Animation.h"
#include "SDL\include\SDL_pixels.h"

#define MAX_FONTS 100
#define MAX_FONT_CHARS 256
#define DYNAMIC_TEXT_LEN 10

struct SDL_Texture;
struct Animation;

struct Font
{
	// Lookup table. All characters displayed in the same order as the texture
	char table[MAX_FONT_CHARS];

	// The font texture
	SDL_Texture* texture = nullptr;

	// Font setup data
	uint totalLength;
	uint rows, columns;
	uint char_w, char_h;
};


class ModuleUI : public Module
{

public:
	ModuleUI();
	~ModuleUI();

	bool Awake(pugi::xml_node& config);
	bool Start();
	bool Update(float dt);
	bool PostUpdate();
	bool CleanUp();

public:
	int score;
	float timer;
	int lifes;
	int currentLevel;

	int uiToRender;

	int font = -1;

	const char* fontPath;

	const char* optionsMenuPath;
	const char* settingsMenuPath;
	const char* saveMenuPath;

	const char* menuArrowPath;

	const char* livesTexturePath;

	bool showDebug = false;

	char scoreText[6] = { "00000" };
	char timerText[4] = { "300" };
	char shortNumberText[2] = { "0" };
	char lifeText[2] = { "0" };

	SDL_Rect box;

	SDL_Texture* pointSpritesheetTex;
	SDL_Rect pointsRect;

	SDL_Texture* livesTexture = nullptr;
	SDL_Rect livesRect;
	SDL_Rect extraLivesRect;

	SDL_Texture* optionsMenuTex = nullptr;
	SDL_Texture* settingsMenuTex = nullptr;
	SDL_Texture* saveMenuTex = nullptr;
	SDL_Texture* menuArrowTex = nullptr;

	// Loads a font file from a texture
	// Returns a font index from the fonts array
	// Param texturePath	- The path to the texture file
	// Param characters		- The lookup table. All characters displayed in the same order as the texture
	// Param rows			- The amount of character rows in the texture
	int Load(const char* texturePath, const char* characters, uint rows = 1);

	// Removes a font by its index
	// Unloads the texture and removes it from the fonts array
	void UnLoad(int fontIndex);

	// Create a surface from text
	void BlitText(int x, int y, int fontIndex, const char* text, bool useCamera) const;
	void IntToDynamicString(char* buffer, int k);
	void IntToString(char* buffer, int k, int length);

	int k = 0;

private:
	// An array to keep track and store all loaded fonts
	Font fonts[MAX_FONTS];
};

#endif 
