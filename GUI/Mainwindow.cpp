#include "Mainwindow.h"
#include "Mainwindow.ui"
#include <QIcon>

Mainwindow1::Mainwindow1(QWidget *parent) : QWidget(parent), ui(new Ui::Mainwindow1) {
    ui->setupUi(this);

    setWindowIcon(QIcon(":/snake++.png"));
}

Mainwindow1::~Mainwindow1() {
    delete ui;
}