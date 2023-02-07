#include "app.hpp"

int main() {
	App app;
	app.init();
	app.loop();
	app.exit();
	return 0;
}
