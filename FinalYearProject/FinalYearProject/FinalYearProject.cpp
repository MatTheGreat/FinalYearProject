

#include "CoreLoop.h"

int main()
{
	CoreLoop core = CoreLoop();
	while (core.isRunning == true)
	{
		core.Loop();
	}
	return 0;
}

