#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget* parent) :
        QMainWindow(parent),
        ui(new Ui::MainWindow) {
    ui->setupUi(this);
    connect(ui->actionLoadObj, &QAction::triggered, ui->openGLWidget, &MainGLWidget::loadObjHandler);
    connect(ui->actionLoadTexture, &QAction::triggered, ui->openGLWidget, &MainGLWidget::loadTextureHandler);
}

MainWindow::~MainWindow() {
    delete ui;
}
