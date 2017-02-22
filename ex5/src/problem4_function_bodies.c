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
        s_t.y--;
        if (s_t.y <= 0) {
            s_t.y++;
        }
        break;
    case DOWN:
        s_t.y++;
        if (s_t.y >= world_height - 1) {
            s_t.y--;
        }
        break;
    case RIGHT:
        s_t.x++;
        if (s_t.x >= world_width - 1) {
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
    if (s_t.x == (world_width - 2) && s_t.y == (world_height - 2)) {
        return goalReward;
    } else if (s_t.y == world_height - 2 && s_t.x != 1) {
        return dropOffPenalty;
    } else {
        return stepPenalty;
    }
}

int isTerminalState(State s_t) {
    if (s_t.y == (world_height - 2) && s_t.x != 1) {
        return 1;
    }
  return 0;
}



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




Agent* trainAgent(int nIter, int verbose, Agent* a) {
    for (int nIt = 0; nIt < nIter; nIt++) {
        playEpisode(0, 0, a);
    }
    a->avgReward = a->accumReward / nIter;
    return a;
}

Agent* playEpisode(int nEpisode, int verbose, Agent* a) {
    State s_t1 = {1, world_height - 2};
    while(isTerminalState(s_t1) == 0) {
        Action a_t1 = getNextAction(a, s_t1);
        State s_t2 = getNextState(s_t1, a_t1);
        float r_t = getNextReward(s_t2);
        if (a->type == QLearning) {
            qLearningUpdate(a, s_t1, s_t2, a_t1, r_t);
        } else {
            sarsaUpdate(a, s_t1, s_t2, a_t1, r_t);
        }
        s_t1 = s_t2;
    }
    return a;
}


// SARSA and Q-Learning Updates:

Agent* sarsaUpdate(Agent* a, State s_t1, State s_t2, Action a_t1, float r_t) {
    float Q = a->QValues[stateAction2idx(s_t1, a_t1)];
    Action a_t2 = getNextAction(a, s_t2);
    float QNext = a->QValues[stateAction2idx(s_t2, a_t2)];
    float newQ = Q + a->alpha * (r_t + a->gamma * QNext - Q);
    a->QValues[stateAction2idx(s_t1, a_t1)] = newQ;
    a->accumReward += r_t;
    return a;
}

Agent* qLearningUpdate(Agent* a, State s_t1, State s_t2, Action a_t1, float r_t) {
    float Q = a->QValues[stateAction2idx(s_t1, a_t1)];
    Action a_t2 = getGreedyAction(a, s_t2);
    float QNext = a->QValues[stateAction2idx(s_t2, a_t2)];
    float newQ = Q + a->alpha * (r_t + a->gamma * QNext - Q);
    a->QValues[stateAction2idx(s_t1, a_t1)] = newQ;
    a->accumReward += r_t;
    return a;
}


