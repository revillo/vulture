#pragma once
#include <memory>

class VultureService {

public:
	virtual bool update() { return false; };

protected:

	VultureService();
	~VultureService();
	
};

typedef std::shared_ptr<VultureService> VultureServiceRef;