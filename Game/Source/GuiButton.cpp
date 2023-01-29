#include "GuiButton.h"
#include "Render.h"
#include "App.h"
#include "Audio.h"
#include"Scene.h"
#include"GuiManager.h"
#include "Log.h"

GuiButton::GuiButton(uint32 id, SDL_Rect bounds, const char* text) : GuiControl(GuiControlType::BUTTON, id)
{
	this->bounds = bounds;
	this->text = text;

	canClick = true;
	drawBasic = false;
    this->id = id;

	
}

GuiButton::~GuiButton()
{

}

bool GuiButton::Update(float dt)
{
	if (state != GuiControlState::DISABLED)
	{
		// L15: DONE 3: Update the state of the GUiButton according to the mouse position
		app->input->GetMousePosition(mouseX, mouseY);

		GuiControlState previousState = state;

		// I'm inside the limitis of the button
		if (mouseX >= bounds.x && mouseX <= bounds.x + bounds.w &&
			mouseY >= bounds.y && mouseY <= bounds.y + bounds.h) {

			state = GuiControlState::FOCUSED;
			if (previousState != state) {
				LOG("Change state from %d to %d", previousState, state);
				app->audio->PlayFx(audioFxId);
			}

			if (app->input->GetMouseButtonDown(SDL_BUTTON_LEFT) == KeyState::KEY_REPEAT) {
				state = GuiControlState::PRESSED;
			}

			//
			if (app->input->GetMouseButtonDown(SDL_BUTTON_LEFT) == KeyState::KEY_UP) {
				NotifyObserver();
			}
		}
		else {
			state = GuiControlState::NORMAL;
		}
	}

	return false;
}




bool GuiButton::Draw(Render* render)
{
	//L15: DONE 4: Draw the button according the GuiControl State
    if (app->guiManager->showDebug == true) {
        switch (state)
        {

        case GuiControlState::NORMAL:
            render->DrawRectangle(bounds, 0, 0, 255, 255, true, false);
            break;
        case GuiControlState::FOCUSED:
            render->DrawRectangle(bounds, 0, 0, 20, 255, true, false);
            break;
        case GuiControlState::PRESSED:
            render->DrawRectangle(bounds, 0, 255, 0, 255, true, false);
            break;
        }
    }
    if (app->scene->gameplayState == app->scene->GameplayState::TITLE_SCREEN)
    {
        switch (id)
        {
         
  
            //new game
        case 1:
            switch (state)
            {
            case GuiControlState::NORMAL:
                app->scene->newGameButtonAnim.loop = false;
                lastState = 1;
                break;

            case GuiControlState::FOCUSED:
                if (app->guiManager->lastId != id) playFxOnce = true;

               
                if(lastState!=2){
                    app->audio->PlayFx(app->guiManager->hoverButtonFx,0);
                    playFxOnce = false;
                    
                    app->guiManager->lastId = id;
                    lastState = 2;
                 
                 }
                
                app->scene->newGameButtonAnim.loop = true;
                break;

            case GuiControlState::SELECTED:
                app->audio->PlayFx(app->guiManager->pressButtonFx);
              
                NotifyObserver();
                break;

            default:
                break;
            }
            break;
            //settings
       
        case 2:
            switch (state)
            {
            case GuiControlState::NORMAL:
                app->scene->exitButtonAnim.loop = false;
                lastState = 3;
                break;

            case GuiControlState::FOCUSED:
                if (app->guiManager->lastId != id) playFxOnce = true;

                if (lastState != 4) {

                    app->audio->PlayFx(app->guiManager->hoverButtonFx, 0);
                    playFxOnce = false;
                    app->guiManager->lastId = id;
                    lastState = 4;
                }
                app->scene->exitButtonAnim.loop = true;
                break;

            case GuiControlState::SELECTED:
                app->audio->PlayFx(app->guiManager->pressButtonFx);
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
    if (app->scene->gameplayState == app->scene->GameplayState::GAME_OVER_SCREEN)
    {
        switch (id)
        {


            //new game
        case 1:
            switch (state)
            {
            case GuiControlState::NORMAL:
                app->scene->newGameButtonAnim.loop = false;
                lastState = 1;
                break;

            case GuiControlState::FOCUSED:
                if (app->guiManager->lastId != id) playFxOnce = true;


                if (lastState != 2) {
                    app->audio->PlayFx(app->guiManager->hoverButtonFx, 0);
                    playFxOnce = false;

                    app->guiManager->lastId = id;
                    lastState = 2;

                }

                app->scene->newGameButtonAnim.loop = true;
                break;

            case GuiControlState::SELECTED:
                app->audio->PlayFx(app->guiManager->pressButtonFx);

                NotifyObserver();
                break;

            default:
                break;
            }
            break;
            //settings

        case 2:
            switch (state)
            {
            case GuiControlState::NORMAL:
                app->scene->exitButtonAnim.loop = false;
                lastState = 3;
                break;

            case GuiControlState::FOCUSED:
                if (app->guiManager->lastId != id) playFxOnce = true;

                if (lastState != 4) {

                    app->audio->PlayFx(app->guiManager->hoverButtonFx, 0);
                    playFxOnce = false;
                    app->guiManager->lastId = id;
                    lastState = 4;
                }
                app->scene->exitButtonAnim.loop = true;
                break;

            case GuiControlState::SELECTED:
                app->audio->PlayFx(app->guiManager->pressButtonFx);
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
    if (app->scene->gameplayState == app->scene->GameplayState::WIN_SCREEN)
    {
        switch (id)
        {


            //new game
        case 1:
            switch (state)
            {
            case GuiControlState::NORMAL:
                app->scene->newGameButtonAnim.loop = false;
                lastState = 1;
                break;

            case GuiControlState::FOCUSED:
                if (app->guiManager->lastId != id) playFxOnce = true;


                if (lastState != 2) {
                    app->audio->PlayFx(app->guiManager->hoverButtonFx, 0);
                    playFxOnce = false;

                    app->guiManager->lastId = id;
                    lastState = 2;

                }

                app->scene->newGameButtonAnim.loop = true;
                break;

            case GuiControlState::SELECTED:
                app->audio->PlayFx(app->guiManager->pressButtonFx);

                NotifyObserver();
                break;

            default:
                break;
            }
            break;
            //settings

        case 2:
            switch (state)
            {
            case GuiControlState::NORMAL:
                app->scene->exitButtonAnim.loop = false;
                lastState = 3;
                break;

            case GuiControlState::FOCUSED:
                if (app->guiManager->lastId != id) playFxOnce = true;

                if (lastState != 4) {

                    app->audio->PlayFx(app->guiManager->hoverButtonFx, 0);
                    playFxOnce = false;
                    app->guiManager->lastId = id;
                    lastState = 4;
                }
                app->scene->exitButtonAnim.loop = true;
                break;

            case GuiControlState::SELECTED:
                app->audio->PlayFx(app->guiManager->pressButtonFx);
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

	app->render->DrawText(text.GetString(), bounds.x, bounds.y, bounds.w, bounds.h, { 255,255,255 });

	return false;
}