#include "dialoghelp.h"
#include "ui_dialoghelp.h"

DialogHelp::DialogHelp(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DialogHelp)
{
    this->setFixedSize(400, 300);
    ui->setupUi(this);
}

DialogHelp::~DialogHelp()
{
    delete ui;
}
