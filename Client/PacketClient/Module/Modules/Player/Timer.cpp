#include "Timer.h"
#include "../pch.h"

Timer::Timer() : IModule(0, Category::PLAYER, "Modifies the games speed") {
	registerIntSetting("TPS", &timer, timer, 20, 50);
	//registerBoolSetting("Hold", &tmhold, tmhold);
}

const char* Timer::getModuleName() {
	return ("Timer");
}

void Timer::onTick(C_GameMode* gm) {
	g_Data.getClientInstance()->minecraft->setTimerSpeed(timer);
}

void Timer::onDisable() {
	g_Data.getClientInstance()->minecraft->setTimerSpeed(20.f);
}
/*
bool Timer::isHoldMode() {
	if (tmhold)
		return true;
	else {
		return false;
	}
}
*/