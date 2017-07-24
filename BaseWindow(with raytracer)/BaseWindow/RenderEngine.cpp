#include "RenderEngine.h"



RenderEngine::RenderEngine(HWND* _windowHandle)
{
	RTgraphics = new RTGraphics(_windowHandle);
	GPURTgraphics = new GPURTGraphics(_windowHandle);
}

RenderEngine::~RenderEngine()
{
	RTgraphics->release();
	GPURTgraphics->release();
	SAFE_DELETE(GPURTgraphics);
	SAFE_DELETE(RTgraphics);
}

void RenderEngine::Render(float _deltaTime)
{
	RTgraphics->updateTogglecb(buttonInput->GetIsVPressed(), buttonInput->GetIsVPressed(), buttonInput->GetBPressed());

	if (buttonInput->GetMPressed() && modeChange == false)
	{
		modeChange = true;
		renderMethod++;
		if (renderMethod > 1)
		{
			renderMethod = 0;
		}
	}
	else if (buttonInput->GetMPressed() == false)
	{
		modeChange = false;
	}

	GPURTgraphics->UpdateCamera(_deltaTime);

	//render
	if (renderMethod == Offline)
	{
		RTgraphics->Update(_deltaTime);
		RTgraphics->Render(_deltaTime);
	}
	else if (renderMethod == GPUGen)
	{
		GPURTgraphics->Update(_deltaTime);
		GPURTgraphics->Render(_deltaTime);
	}
	Cam->update();
}
