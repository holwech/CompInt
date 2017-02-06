/*
##########################################################################################
Student Functions:

- Implement the bodies of the problem prototypes within this file!
- The import rl_homework_lib.h is sufficient to solve all the problems.

##########################################################################################
*/

#include "rl_homework_lib.h"

// Function of Gameplay:
Agent* trainAgent(int nIter, int verbose, Agent* a) {

    return a;
}

Agent* playEpisode(int nEpisode, int verbose, Agent* a) {
    for (int episode = 0; episode < nEpisode; episode++) {
        State s_t1 = {1, world_height - 2};
        while(isTerminalState(s_t) == 0) {
            Action a_t1 = getNextAction(s_t1);
            State s_t2 = getNextState(s_t1, a_t1);
            qLearningUpdate(a, s_t1, s_t2, a_t1, )
        }
    }
    return a;
}


// SARSA and Q-Learning Updates:

Agent* sarsaUpdate(Agent* a, State s_t1, State s_t2, Action a_t1, float r_t) {
    float newQ = getNextReward(s_t1) + r_t + a->gamma * getNextReward(s_t2);
    return a;
}

Agent* qLearningUpdate(Agent* a, State s_t1, State s_t2, Action a_t1, float r_t) {
		float Q = a->QValues[stateAction2idx(s_t1, a_t1)];
		Action a_t2 = getGreedyAction(a, s_t2);
		float QNext = a->QValues[stateAction2idx(s_t2, a_t2)];
    float newQ = Q + a->alpha * (r_t + gamma * QNext - Q);
    a->QValues[stateAction2idx(s_t1, a_t1)] = newQ;
    a->accumReward += newQ;
	return a;
}


