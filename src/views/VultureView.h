#pragma once
#include "graphics\VultureGPUService.h"
#include "graphics\VultureMesh.h"
#include "graphics\VultureMeshRender.h"

class View : public OOP::Interface {
public:

	virtual void show() = 0;

	virtual void hide() = 0;

protected:
};