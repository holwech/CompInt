/*
##########################################################################################
Student Functions:

- Implement the bodies of the problem prototypes within this file!
- The import rl_homework_lib.h is sufficient to solve all the problems.

##########################################################################################
*/

#include "rl_homework_lib.h"


// Function of the Agent:
Action getNextAction(Agent* a, State s_t) {
    double randNum = (rand() % 101) / 100.0;
	if (randNum <= a->epsilon) {
		return getRandomAction(a, s_t);
	} else {
		return getGreedyAction(a, s_t);
	}
}

Action getRandomAction() {
	int randVal = rand() % 4;
	switch(randVal) {
		case 0:
			return UP;
		case 1:
			return DOWN;
		case 2:
			return LEFT;
		case 3:
			return RIGHT;
	}
    return UP;
}

Action getGreedyAction(Agent* a, State s_t) {
    double maxQ = a->QValues[stateAction2idx(s_t, UP)];
    Action nextAction = UP;
    if (a->QValues[stateAction2idx(s_t, DOWN)] >= maxQ) {
        maxQ = a->QValues[stateAction2idx(s_t, DOWN)];
		nextAction = DOWN;
    }
    if (a->QValues[stateAction2idx(s_t, LEFT)] >= maxQ) {
        maxQ = a->QValues[stateAction2idx(s_t, LEFT)];
		nextAction = LEFT;
    }
    if (a->QValues[stateAction2idx(s_t, RIGHT)] >= maxQ) {
        maxQ = a->QValues[stateAction2idx(s_t, RIGHT)];
		nextAction = RIGHT;
	}
    return nextAction;
}
