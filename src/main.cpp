#include "game.h"
#include "player.h"

int main() {
	int max_score = 0;

	while (1) {
		int init_status = init();

		if (init_status == 0) {
			int return_val = run(&max_score);
			if (return_val == 1) { // end game after game over
				break;
			}
		}
	}

	close();

	return 0;
}
