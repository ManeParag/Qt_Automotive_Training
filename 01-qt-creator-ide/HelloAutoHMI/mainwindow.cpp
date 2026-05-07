#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QStatusBar>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    setWindowTitle("Hello AutoHMI");
    statusBar()->showMessage("Engine off");

    connect(ui->startButton, &QPushButton::clicked,
            this, &MainWindow::onStartEngineClicked);
}

MainWindow::~MainWindow() { delete ui; }

void MainWindow::onStartEngineClicked() {
    m_engineRunning = !m_engineRunning;
    if (m_engineRunning) {
        statusBar()->showMessage("Engine running");
        ui->startButton->setText("Stop Engine");
    } else {
        statusBar()->showMessage("Engine off");
        ui->startButton->setText("Start Engine");
    }
}
