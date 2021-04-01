#include "Window.h"

int main()
{
	Window window = Window(1920, 1080, "Physarum Simulation", 0, false);
	window.run();

	return 0;
}