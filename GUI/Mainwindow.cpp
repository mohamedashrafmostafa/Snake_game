//
// Created by DELL on 5/6/2026.
//

// You may need to build the project (run Qt uic code generator) to get "ui_Mainwindow1.h" resolved

#include "Mainwindow.h"
#include "ui_Mainwindow1.h"


Mainwindow1::Mainwindow1(QWidget *parent) : QWidget(parent), ui(new Ui::Mainwindow1) {
    ui->setupUi(this);
}

Mainwindow1::~Mainwindow1() {
    delete ui;
}