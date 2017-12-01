#ifndef RASLLO_H
#define RASLLO_H

#include <QMainWindow>

namespace Ui {
class Rasllo;
}

class Rasllo : public QMainWindow
{
    Q_OBJECT

public:
    explicit Rasllo(QWidget *parent = 0);
    ~Rasllo();

private:
    Ui::Rasllo *ui;
};

#endif // RASLLO_H
