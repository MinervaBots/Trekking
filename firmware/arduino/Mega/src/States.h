#ifndef STATES_H
#define STATES_H


void changeState(void (*nextState)(unsigned long));

void reset(unsigned long deltaTime);
void idle(unsigned long deltaTime);
void search(unsigned long deltaTime);
void refinedSearch(unsigned long deltaTime);
void targetFound(unsigned long deltaTime);
void rotateCamera(unsigned long deltaTime);

#endif //STATES_H
