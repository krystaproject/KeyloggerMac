#ifndef KYRALOGGER_H_
#define KYRALOGGER_H_

#include <ApplicationServices/ApplicationServices.h>
#include <stdbool.h>
#include <string.h>

bool layout_switch;
bool autorun;
char layout[256];

extern char * keycodeToString (CGKeyCode);
#endif