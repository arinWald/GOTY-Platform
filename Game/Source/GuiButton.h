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

private:

    // Gui Button specific properties
    // Maybe some animation properties for state change?
};

#endif // __GUIBUTTON_H__
