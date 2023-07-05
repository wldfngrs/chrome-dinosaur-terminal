#include "game.h"
#include "player.h"

int main(int argv, char** argc) {
	int max_score = 0;

	while (1) {
		int init_status = init();

		if (init_status == 0) {
			if (!run(&max_score)) {
				break;
			};
		}
	}

	close();

	return 0;
}