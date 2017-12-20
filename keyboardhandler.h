#ifndef KEYBOARDHANDLER_H
#define KEYBOARDHANDLER_H

#include <QtGui>
#include <iostream>

class KeyboardHandler: public QObject
{
    Q_OBJECT
public:
    KeyboardHandler():QObject()
    {};
    bool eventFilter(QObject* object,QEvent* event);
    void SetStartKey(int StartKey);
signals:
    void CardReaded(QString cardNumber);
private:
      int StartKey = 0; // `==96
      bool started = false;
      QString CardNumber;
};

#endif // KEYBOARDHANDLER_H
