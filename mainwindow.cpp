#include "mainwindow.h"
#include "./ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    setWindowTitle("ExistOS Updater");
    ui->label_version->setText(VERSION);
    ui->output->setText("Connect your calculator to computer while pressing [ON/C] key.\nThen press FLASH button.");
    if(edbINFO->isFile() == false || sbloaderINFO->isFile() == false){
        QMessageBox::critical(this, tr("Error"), tr("Files are missing! Please reinstall this application."));
        ui->button_flash->setEnabled(false);
    }
    connect(optionsWindow, SIGNAL(returnPage(int, int)), this, SLOT(getReturnPage(int, int)));
}

MainWindow::~MainWindow()
{
    delete ui;
    delete p1;
    delete p2;
    delete path_OSLoader;
    delete path_System;
    delete edbINFO;
    delete sbloaderINFO;
    delete optionsWindow;
}

void MainWindow::doStage(int stage){
    if(stage == 1){
        ui->output->clear();
        addLine("Prepare to flash...");
        addLine("OSLoader: PAGE " + QString::number(page_OSLoader));
        addLine("System: PAGE " + QString::number(page_System));
        QStringList argu;
        argu.append("-f");
        argu.append(*path_OSLoader);
        openProcess(stage, QDir::currentPath() + "/sb_loader.exe", argu);
    }else{
        if(stage == 2){
            addLine("Installing system...");
            QStringList argu;
            argu.append("-r");
            argu.append("-s");
            argu.append("-f");
            argu.append(*path_OSLoader);
            argu.append(QString::number(page_OSLoader));
            argu.append("b");
            argu.append("-f");
            argu.append(*path_System);
            argu.append(QString::number(page_System));
            openProcess(stage, QDir::currentPath() + "/edb.exe", argu);
        }
    }
}

void MainWindow::openProcess(const int & stage, const QString & path, const QStringList & argu){
    if(stage == 1){
        p1->start(path, argu);
        connect(p1, SIGNAL(finished(int)), this, SLOT(readResult(int)));
    }else{
        if(stage == 2){
            p2->start(path, argu);
            connect(p2, SIGNAL(finished(int)), this, SLOT(readResult(int)));
        }
    }
}

void MainWindow::addLine(const QString & texts){
    ui->output->setText(ui->output->toPlainText() + texts + "\n");
    ui->output->moveCursor(QTextCursor::End);
}

void MainWindow::readResult(int exitCode){
    QByteArray result;
    if(stage == 1){
        result = p1->readAll();
    }else{
        if(stage == 2){
            result = p2->readAll();
        }
    }

    addLine(QString::fromLocal8Bit(result));
    stage++;
    if(stage <= 2){
        doStage(stage);
    }else{
        addLine("Done!");
        ui->button_flash->setEnabled(true);
        stage = 1;
        p1->close();
        p2->close();
    }
}

void MainWindow::on_button_OSLoader_path_clicked()
{
    ui->OSLoader_path->setText(QFileDialog::getOpenFileName(this, tr("Select a file"), tr(""), tr("OS Loader File(*.sb)")));
}


void MainWindow::on_button_System_path_clicked()
{
    ui->System_path->setText(QFileDialog::getOpenFileName(this, tr("Select a file"), tr(""), tr("System File(*.sys)")));
}


void MainWindow::on_button_flash_clicked()
{
    *path_OSLoader = ui->OSLoader_path->text();
    *path_System = ui->System_path->text();
    QFileInfo osloaderINFO;
    QFileInfo systemINFO;
    osloaderINFO.setFile(*path_OSLoader);
    systemINFO.setFile(*path_System);
    if(*path_OSLoader == "" || *path_System == ""){
        QMessageBox::warning(this, tr("Warning"), tr("File(s) unselected!"));
    }else{
        if(osloaderINFO.isFile() && systemINFO.isFile()){
            if(QMessageBox::question(this, tr("Question"), tr("Are you sure you want to update your calculator?\n")) == QMessageBox::Yes){
                ui->button_flash->setEnabled(false);
                doStage(stage);
            }
        }else{
            QMessageBox::critical(this, tr("Error"), tr("File does not exist!"));
        }
    }
}


void MainWindow::on_button_Option_clicked()
{
    optionsWindow->setPage(page_OSLoader, page_System);
    optionsWindow->show();
}

void MainWindow::getReturnPage(int OSLoader, int System){
    page_OSLoader = OSLoader;
    page_System = System;
}

