#ifndef __GUIBUTTON_H__
#define __GUIBUTTON_H__

#include "GuiControl.h"

#include "Point.h"
#include "SString.h"

struct SDL_Texture;

class GuiButton : public GuiControl
{
public:

    GuiButton(uint32 id, SDL_Rect bounds, SDL_Texture* tex);
    virtual ~GuiButton();

    bool Update(Input* input, float dt);
    bool Draw(Render* render);
    bool DrawDebug(Render* render);

    bool playFxOnce = true;

	int lastState = 0;

	bool canClick = true;
	bool drawBasic = false;
	bool playFxOnce = true;

	int audioFxId;
};

#endif // __GUIBUTTON_H__
