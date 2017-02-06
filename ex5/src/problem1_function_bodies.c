/*
##########################################################################################
Student Functions:

- Implement the bodies of the problem prototypes within this file!
- The import rl_homework_lib.h is sufficient to solve all the problems.

##########################################################################################
*/

#include "rl_homework_lib.h"



// Function of the World
State getNextState(State s_t, Action a_t) {
	switch(a_t) {
	case UP:
		s_t.y++;
		if (s_t.y >= world_height) {
			s_t.y--;
		}
		break;
	case DOWN:
		s_t.y--;
		if (s_t.y <= 0) {
			s_t.y++;
		}
		break;
	case RIGHT:
		s_t.x++;
		if (s_t.x >= world_width) {
			s_t.x--;
		}
		break;
	case LEFT:
		s_t.x--;
		if (s_t.x <= 0) {
			s_t.x++;
		}
		break;
	}
	return s_t;
}

float getNextReward(State s_t) {
        return 0.0;
}

int isTerminalState(State s_t) {
	if (s_t.x == (world_width - 1) && s_t.y == (world_width - 1)) {
		return 1;
	}
  return 0;
}
