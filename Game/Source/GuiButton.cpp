#include "GuiButton.h"
#include "App.h"
#include "Scene.h"
#include "GuiManager.h"
#include "Audio.h"
#include "Player.h"
#include "Window.h"

GuiButton::GuiButton(uint32 id, SDL_Rect bounds, SDL_Texture* tex) : GuiControl(GuiControlType::BUTTON, id)
{
    this->id = id;
    this->bounds = bounds;
    this->texture = tex;
}

GuiButton::~GuiButton()
{
}

bool GuiButton::Update(Input* input, float dt)
{
    if (app->scene->continueButtonDisabled && id == 1 && app->scene->gameplayState == app->scene->GameplayState::TITLE_SCREEN)
        state = GuiControlState::DISABLED;

    if (state != GuiControlState::DISABLED)
    {
        int mouseX, mouseY;
        input->GetMousePosition(mouseX, mouseY);

        // Check collision between mouse and button bounds
        if ((mouseX > bounds.x) && (mouseX < (bounds.x + bounds.w)) &&
            (mouseY > bounds.y) && (mouseY < (bounds.y + bounds.h)))
        {
            state = GuiControlState::FOCUSED;

            if (input->GetMouseButtonDown(SDL_BUTTON_LEFT) == KeyState::KEY_REPEAT)
            {
                state = GuiControlState::PRESSED;
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

bool GuiButton::Draw(Render* render)
{
    int yOffset = 4;

    // Draw the right button depending on state
    if (app->scene->gameplayState == app->scene->GameplayState::TITLE_SCREEN)
    {
        switch (id)
        {
            //continue
        case 1:
            switch (state)
            {
            case GuiControlState::NORMAL:
                app->scene->continueButtonAnim.loop = false;
                break;
            case GuiControlState::FOCUSED:
                if (app->guiManager->lastId != id) playFxOnce = true;

                if (playFxOnce)
                {
                    app->audio->PlayFx(app->guiManager->hoverButtonFx, 0);
                    playFxOnce = false;
                    app->guiManager->lastId = id;
                }
                app->scene->continueButtonAnim.loop = true;
                break;

            case GuiControlState::SELECTED:
                app->audio->PlayFx(app->guiManager->pressButtonFx, 0);
                NotifyObserver();
                break;

            default:
                break;
            }
            break;
            //new game
        case 2:
            switch (state)
            {
            case GuiControlState::NORMAL:
                app->scene->newGameButtonAnim.loop = false;
                break;

            case GuiControlState::FOCUSED:
                if (app->guiManager->lastId != id) playFxOnce = true;

                if (playFxOnce)
                {
                    app->audio->PlayFx(app->guiManager->hoverButtonFx, 0);
                    playFxOnce = false;
                    app->guiManager->lastId = id;
                }
                app->scene->newGameButtonAnim.loop = true;
                break;

            case GuiControlState::SELECTED:
                app->audio->PlayFx(app->guiManager->pressButtonFx, 0);
                NotifyObserver();
                break;

            default:
                break;
            }
            break;
            //settings
        case 3:
            switch (state)
            {
            case GuiControlState::NORMAL:
                app->scene->settingsButtonAnim.loop = false;
                break;

            case GuiControlState::FOCUSED:
                if (app->guiManager->lastId != id) playFxOnce = true;

                if (playFxOnce)
                {
                    app->audio->PlayFx(app->guiManager->hoverButtonFx, 0);
                    playFxOnce = false;
                    app->guiManager->lastId = id;
                }
                app->scene->settingsButtonAnim.loop = true;
                break;

            case GuiControlState::SELECTED:
                app->audio->PlayFx(app->guiManager->pressButtonFx, 0);
                NotifyObserver();
                break;

            default:
                break;
            }
            break;
            //credits
        case 4:
            switch (state)
            {
            case GuiControlState::NORMAL:
                app->scene->creditsButtonAnim.loop = false;
                break;

            case GuiControlState::FOCUSED:
                if (app->guiManager->lastId != id) playFxOnce = true;

                if (playFxOnce)
                {
                    app->audio->PlayFx(app->guiManager->hoverButtonFx, 0);
                    playFxOnce = false;
                    app->guiManager->lastId = id;
                }
                app->scene->creditsButtonAnim.loop = true;
                break;

            case GuiControlState::SELECTED:
                app->audio->PlayFx(app->guiManager->pressButtonFx, 0);
                NotifyObserver();
                break;

            default:
                break;
            }
            break;
            //exit
        case 5:
            switch (state)
            {
            case GuiControlState::NORMAL:
                app->scene->exitButtonAnim.loop = false;
                break;

            case GuiControlState::FOCUSED:
                if (app->guiManager->lastId != id) playFxOnce = true;

                if (playFxOnce)
                {
                    app->audio->PlayFx(app->guiManager->hoverButtonFx, 0);
                    playFxOnce = false;
                    app->guiManager->lastId = id;
                }
                app->scene->exitButtonAnim.loop = true;
                break;

            case GuiControlState::SELECTED:
                app->audio->PlayFx(app->guiManager->pressButtonFx, 0);
                NotifyObserver();
                break;

            default:
                break;
            }
            break;

        default:
            break;
        }
    }
    else if (app->scene->gameplayState == app->scene->GameplayState::PLAYING)
    {
        switch (state)
        {
        case GuiControlState::DISABLED:
            break;

        case GuiControlState::NORMAL:
            break;

        case GuiControlState::FOCUSED:
            if (app->guiManager->lastId != id) playFxOnce = true;

            if (playFxOnce)
            {
                app->audio->PlayFx(app->guiManager->hoverButtonFx, 0);
                playFxOnce = false;
                app->guiManager->lastId = id;
            }
            render->DrawTexture(texture, bounds.x, bounds.y - yOffset + bounds.h / 2, &SDL_Rect({ 0,0,9,7 }), 0, 0, 0, 0, false);
            render->DrawTexture(texture, bounds.x + bounds.w - 9, bounds.y - yOffset + bounds.h / 2, &SDL_Rect({ 18,0,9,7 }), 0, 0, 0, 0, false);
            break;

        case GuiControlState::PRESSED:
            render->DrawTexture(texture, bounds.x, bounds.y - yOffset + bounds.h / 2, &SDL_Rect({ 9,0,9,7 }), 0, 0, 0, 0, false);
            render->DrawTexture(texture, bounds.x + bounds.w - 9, bounds.y - yOffset + bounds.h / 2, &SDL_Rect({ 27,0,9,7 }), 0, 0, 0, 0, false);
            break;

        case GuiControlState::SELECTED:
            app->audio->PlayFx(app->guiManager->pressButtonFx, 0);
            NotifyObserver();
            break;

        default:
            break;
        }
    }
    return false;
}

bool GuiButton::DrawDebug(Render* render)
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