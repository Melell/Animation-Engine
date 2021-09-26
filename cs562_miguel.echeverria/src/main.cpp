#include "pch.h"
#include "Application/Engine.h"


int main(void)
{
	cs460::Engine engine;

	if (!engine.initialize())
		return -1;
	
	engine.update();
	engine.close();

	return 0;
}