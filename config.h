#ifndef CONFIG_H
#define CONFIG_H

// Configuration data

#include <QList>


const QList<int> LEDS = { 18, 23, 24, 25 }; // Output
const QList<int> BUTTONS = { 22, 27, 17 }; // Input


const int LFLAGS = 0;
const int CHIP = 0;

const int T_UPDATE = 75;

#endif
