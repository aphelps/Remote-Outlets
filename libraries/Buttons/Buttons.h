#ifndef BUTTONS_H
#define BUTTONS_H


void buttons_init(const int buttons[], const int nbuttons);
int checkButtons();

typedef void (*button_action_t)(int button, int value);
void buttons_set_action(int button, void (*action)(int button, int value));

#endif
