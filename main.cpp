#include"game.h"
#include"gameinit.h"
int main() {
	gameInit();
	int time = 0;
	bool flag = true;
	start();
	view();
	ready();
	while (true) {
		userclick();
		time += getdelay();
		if (time > 50) {
			flag = true;
			time = 0;
		}
		if (flag) {
			flag = false;
			updateimage();
			updategame();
		}
		if (!win()) {
			break;
		}
	}
	system("pause");
	return 0;
}
