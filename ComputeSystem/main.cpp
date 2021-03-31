#include "Window.h"

int main()
{
	Window window = Window(1920, 1080, "Compute", 0, true);
	window.run();

	return 0;
}