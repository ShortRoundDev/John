#include <Windows.h>
#include "App.h"

//int WinMain(
//	HINSTANCE hInstance,
//	HINSTANCE hPrevInstance,
//	LPSTR     lpCmdLine,
//	int       nShowCmd
//)
int main(int argc, char** argv)
{
	App::init("John");
	while (!APP->shouldQuit)
	{
		APP->events();
		APP->update();
		APP->draw();
	}
	return 1;
}