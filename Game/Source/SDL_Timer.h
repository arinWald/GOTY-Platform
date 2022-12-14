#ifndef __SDL_TIMER_H__
#define __SDL_TIMER_H__	

#include "SDL/include/SDL.h"

enum estadoTimerP {
	ENCENDIDO,
	EJECUTANDO,
	FIN,
	APAGADO,
};


class SDL_Timer
{
public:
	void Start(float m_duration);
	int Test();

private:
	uint32_t startTime;
	estadoTimerP estadoTimer = APAGADO;
	uint32_t duration; //segundos de cronómetro
};

#endif // !__SDL_TIMER_H__