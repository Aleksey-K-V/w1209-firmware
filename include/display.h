#ifndef DISPLAY_H
#define DISPLAY_H

#ifndef bool
#define bool    _Bool
#define true    1
#define false   0
#endif

void initDisplay();
void refreshDisplay();
void setDisplayOff (bool val);
void setDisplayTestMode (bool);
void setDisplayStr (const unsigned char*);
void setDisplayInt (int);

#endif