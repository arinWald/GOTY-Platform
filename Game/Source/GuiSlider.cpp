#include "GuiSlider.h"
#include "App.h"
#include "Scene.h"
#include "GuiManager.h"
#include "Audio.h"
#include "Window.h"

GuiSlider::GuiSlider(uint32 id, SDL_Rect bounds, SDL_Texture* tex) : GuiControl(GuiControlType::SLIDER, id)
{
    this->bounds = bounds;
    this->unit = bounds.w / 100.0f;
    this->texture = tex;
    if (id == 1)
    {
        value = app->guiManager->musicVolume;
    }
    else if (id == 2)
    {
        value = app->guiManager->fxVolume;
    }

    value = round(value);
    sliderPosx = ((value * unit) + bounds.x) - unit;
}

GuiSlider::~GuiSlider()
{
}

bool GuiSlider::Update(Input* input, float dt)
{
    if (state != GuiControlState::DISABLED)
    {
        int mouseX, mouseY;
        input->GetMousePosition(mouseX, mouseY);

        // Check collision between mouse and button bounds
        if ((mouseX > bounds.x) && (mouseX < (bounds.x + bounds.w)) &&
            (mouseY > bounds.y) && (mouseY < (bounds.y + bounds.h)))
        {
            state = GuiControlState::FOCUSED;

            unit = bounds.w / 100.0f;
            value = (mouseX - bounds.x) / unit;
            value = round(value);

            if (input->GetMouseButtonDown(SDL_BUTTON_LEFT) == KeyState::KEY_REPEAT)
            {
                state = GuiControlState::PRESSED;
                sliderPosx = ((value * unit) + bounds.x) - unit - 5;
            }

            // If mouse button pressed -> Generate event!
            if (input->GetMouseButtonDown(SDL_BUTTON_LEFT) == KeyState::KEY_UP)
            {
                state = GuiControlState::SELECTED;
            }
        }
        else state = GuiControlState::NORMAL;
    }
    return false;
}

bool GuiSlider::Draw(Render* render)
{
    // Draw the right button depending on state
    if (app->scene->fading == false)
    {
        switch (state)
        {
        case GuiControlState::DISABLED:
            break;

        case GuiControlState::NORMAL:
            render->DrawTexture(texture, bounds.x, bounds.y, &SDL_Rect({ 0, 0, 116, 23 }), 0, 0, 0, 0, false);
            render->DrawTexture(texture, sliderPosx, bounds.y, &SDL_Rect({ 116, 0, 16, 23 }), 0, 0, 0, 0, false);
            break;

        case GuiControlState::FOCUSED:
            if (app->guiManager->lastId != id) playFxOnce = true;

            if (playFxOnce)
            {
                app->audio->PlayFx(app->guiManager->hoverButtonFx, 0);
                playFxOnce = false;
                app->guiManager->lastId = id;
            }
            render->DrawTexture(texture, bounds.x, bounds.y, &SDL_Rect({ 0, 23, 116, 23 }), 0, 0, 0, 0, false);
            render->DrawTexture(texture, sliderPosx, bounds.y, &SDL_Rect({ 116, 0, 16, 23 }), 0, 0, 0, 0, false);
            break;

        case GuiControlState::PRESSED:
            render->DrawTexture(texture, bounds.x, bounds.y, &SDL_Rect({ 0, 23, 116, 23 }), 0, 0, 0, 0, false);
            render->DrawTexture(texture, sliderPosx, bounds.y, &SDL_Rect({ 116, 23 , 16, 23 }), 0, 0, 0, 0, false);
            break;

        case GuiControlState::SELECTED:
            if (id == 2)
                app->audio->PlayFx(app->guiManager->pressButtonFx, 0);
            render->DrawTexture(texture, bounds.x, bounds.y, &SDL_Rect({ 0, 23, 116, 23 }), 0, 0, 0, 0, false);
            render->DrawTexture(texture, sliderPosx, bounds.y, &SDL_Rect({ 116, 23 , 16, 23 }), 0, 0, 0, 0, false);
            NotifyObserver();
            break;

        default:
            break;
        }
    } 
    return false;
}

bool GuiSlider::DrawDebug(Render* render)
{
    int scale = app->win->GetScale();

    SDL_Rect drawBounds = SDL_Rect({ bounds.x * scale, bounds.y * scale, bounds.w * scale, bounds.h * scale });

    switch (state)
    {
    case GuiControlState::DISABLED:
        render->DrawRectangle(drawBounds, 255, 0, 0, 128, true, false);
        break;
    case GuiControlState::FOCUSED:
        render->DrawRectangle(drawBounds, 0, 255, 0, 128, true, false);
        break;
    case GuiControlState::NORMAL:
        render->DrawRectangle(drawBounds, 0, 0, 255, 128, true, false);
        break;
    case GuiControlState::PRESSED:
        render->DrawRectangle(drawBounds, 255, 255, 0, 128, true, false);
        break;
    case GuiControlState::SELECTED:
        render->DrawRectangle(drawBounds, 0, 255, 255, 128, true, false);
        break;
    }

    return true;
}