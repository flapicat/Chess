#include "core/App.h"
#include "core/Log.h"
#include "ChessLayer.h"

int main()
{
	Log::init();
	App* app = new App;
	app->PushLayer(new ChessLayer());
	app->Run();
	delete app;
}