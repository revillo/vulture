#include "VultureService.h"

void VultureService::kill()
{
	_stayAlive = false;
}

VultureService::VultureService()
{
	_stayAlive = true;
}

VultureService::~VultureService()
{
}
