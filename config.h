#ifndef CONFIG_H
#define CONFIG_H
#include <QString>
class Config
{
public:
    int displayID = 0;
    int logs = 10;
    int portNumber = 15000;
    bool isEmulator = false;
    QString programVersion;
};
#endif // CONFIG_H
