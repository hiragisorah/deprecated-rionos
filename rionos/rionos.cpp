#include "rionos.h"

void Rionos::Initialize(void)
{
}

bool Rionos::Run(void)
{
	return true;
}

void Rionos::Finalize(void)
{
}

void Rionos::Destroy(void)
{
	delete this;
}

Game * const Create(Graphics * const graphics)
{
	return new Rionos(graphics);
}