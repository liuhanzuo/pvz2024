#include"game.h"

int main() {
	Game g;
	g.gameInit();
	int time = 0;
	bool flag = true;
	start();
	view();
	ready();
	while (true) {
		userclick();
		time += GetDelay();
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
