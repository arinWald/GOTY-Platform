#include "GuiManager.h"
#include "App.h"
#include "Textures.h"

#include "GuiButton.h"
#include "Audio.h"

GuiManager::GuiManager() :Module()
{
	name.Create("guiManager");
}

GuiManager::~GuiManager() {}

bool GuiManager::Start()
{


	pressButtonFxPath = "Assets/Audio/Fx/pressed_button.wav";
	hoverButtonFxPath = "Assets/Audio/Fx/hover_button.wav";

	hoverButtonFx = app->audio->LoadFx(hoverButtonFxPath);
	pressButtonFx = app->audio->LoadFx(pressButtonFxPath);
	

	return true;
}



bool GuiManager::Update(float dt)
{
	accumulatedTime += dt;
	if (accumulatedTime >= updateMsCycle) doLogic = true;

	UpdateAll(dt, doLogic);

	if (doLogic == true)
	{
		accumulatedTime = 0.0f;
		doLogic = false;
	}
	

	return true;
}

bool GuiManager::Draw() {

	ListItem<GuiControl*>* control = controls.start;

	while (control != nullptr)
	{
		
			control->data->Draw(app->render);
			control = control->next;
		
	}

	return true;

}

bool GuiManager::CleanUp()
{
	ListItem<GuiControl*>* control = controls.start;

	while (control != nullptr)
	{
		RELEASE(control);
	}

	return true;

	return false;
}

GuiControl* GuiManager::CreateGuiControl(GuiControlType type, int x, int y, SDL_Rect bounds, int id)
{
	GuiControl* control = nullptr;

	switch (type)
	{
	case GuiControlType::BUTTON:
		control = new GuiButton(id, bounds, arrowMenuTex);
		
		break;
	//case GuiControlType::CHECKBOX:
	//	control = new GuiCheckBox(id, bounds, checkBoxTex);
	//	control->SetObserver(app->scene);
	//	break;
	//case GuiControlType::SLIDER:
	//	control = new GuiSlider(id, bounds, sliderTex);
	//	control->SetObserver(app->scene);
	//	break;

	default: break;
	}
	id++;

	// Created entities are added to the list
	if (control != nullptr) controls.Add(control);

	return control;
}

void GuiManager::DestroyGuiControl(GuiControl* entity)
{
	int i = controls.Find(entity);
	ListItem<GuiControl*>* c = controls.At(i);
	controls.Del(c);
}

void GuiManager::DestroyAllGuiControls()
{
	int u = controls.Count();

	for (int i = 0; i < u; i++)
	{
		delete controls.At(0)->data;
		controls.Del(controls.At(0));
	}
}

void GuiManager::AddGuiControl(GuiControl* entity)
{
	if (entity != nullptr) controls.Add(entity);
}

void GuiManager::UpdateAll(float dt, bool doLogic)
{
	if (doLogic)
	{
		for (int i = 0; i < controls.Count(); i++)
		{
			controls[i]->Update(app->input, dt);
		}
	}
}

void GuiManager::DrawAll()
{
	for (int i = 0; i < controls.Count(); i++)
	{
		if (showDebug)
			controls[i]->DrawDebug(app->render);
		controls[i]->Draw(app->render);
	}
}



