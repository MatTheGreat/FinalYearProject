

#include "CoreLoop.h"

int main()
{
	CoreLoop coreLoop = CoreLoop();
	while (coreLoop.isRunning == true)
	{
		coreLoop.Loop();
	}
	return 0;
}

