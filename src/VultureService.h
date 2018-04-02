#pragma once
#include <memory>
#include <atomic>

class VultureService {

public:
	virtual bool update() { return false; };
	
	void kill();
	
protected:

	std::atomic<bool> _stayAlive;

	VultureService();
	~VultureService();

};

typedef std::shared_ptr<VultureService> VultureServiceRef;