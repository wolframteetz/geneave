#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "serial.h"
#include "ui_serial.h"
#include <qdebug>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    Serial *serial = new Serial();
    this->setCentralWidget(serial);
}

MainWindow::~MainWindow()
{
    delete ui;
}
