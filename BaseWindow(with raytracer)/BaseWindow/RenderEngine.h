#pragma once
#include "stdafx.h"
#include "ButtonInput.h"
#include "RTGraphics.h"
#include "GPURTGraphics.h"


extern Camera* Cam;
extern ButtonInput* buttonInput;

enum { GPUGen, Offline };


class RenderEngine
{
public:
	RenderEngine(HWND* _windowHandle);
	~RenderEngine();
	void Render(float _deltaTime);
private:
	RTGraphics *RTgraphics;
	GPURTGraphics *GPURTgraphics;
	int renderMethod = Offline;
	bool modeChange = false;
};

