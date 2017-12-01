#include "rasllo.h"
#include "ui_rasllo.h"

Rasllo::Rasllo(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::Rasllo)
{
    ui->setupUi(this);
}

Rasllo::~Rasllo()
{
    delete ui;
}
