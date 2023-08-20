#include "game.h"
#include "player.h"

int main() {
	int max_score = 0;

	while (1) {
		int init_status = init();

		if (init_status == 0) {
			if (run(max_score)) { // end game after game over
				break;
			}
		}
	}

	close();

	return 0;
}
