#pragma once
#include "../../ModuleManager.h"
#include "../Module.h"

class Watermark : public IModule {
private:
	float scale = 1.f;
	int opacity = 150;
public:
	bool outlinec = true;

	virtual void onPostRender(C_MinecraftUIRenderContext* renderCtx);
	virtual const char* getModuleName();
	virtual void onEnable() override;
	Watermark();
};