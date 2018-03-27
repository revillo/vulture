#include "TestApp.h"

int main() {

	auto app = TestApp();
	auto service = make_shared<TestService>();
	app.runService(service, 300);

	//app.run();


	return 0;
}