#ifndef ARGUMENTPARSER_H
#define ARGUMENTPARSER_H

#include <QString>



 class ArgumentParser
{
public:
     ArgumentParser();
     static bool Parse(int argc, char *argv[], int* portNumber, int* displayID, bool* emulator, int* logs, QString programVersion);
};

#endif // ARGUMENTPARSER_H
