#include "SDL_Timer.h"

void SDL_Timer::Start(float m_duration)
{
	
	if (startTime == 0)
	{
		estadoTimer = ENCENDIDO;
		startTime = SDL_GetTicks();
		duration = m_duration * 1000;

	}

}

int SDL_Timer::Test()
{
	if (estadoTimer == ENCENDIDO) {

		auto currentTime = SDL_GetTicks();

		if (currentTime - startTime >= duration)
		{
			startTime = 0;
			duration = 0;
			estadoTimer = APAGADO;
			return FIN;
		}

		return EJECUTANDO;
	}

	return APAGADO;

}