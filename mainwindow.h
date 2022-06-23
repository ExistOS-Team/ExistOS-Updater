#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QFileDialog>
#include <QMessageBox>
#include <QProcess>
#include <QDir>
#include <QFileInfo>

#include <options.h>
#include <about.h>

constexpr auto VERSION = "ver 0.0.2";

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
    void addLine(const QString &texts);
    void openProcess(const int &stage, const QString &path, const QStringList &argu);
    void doStage(int stage);
    int stage = 1;
    int page_OSLoader = DEFAULT_OSLOADER_PAGE;
    int page_System = DEFAULT_SYSTEM_PAGE;

private slots:
    void on_button_OSLoader_path_clicked();

    void on_button_System_path_clicked();

    void on_button_flash_clicked();

    void readResult(int exitCode);

    void on_button_Option_clicked();

    void getReturnPage(int OSLoader, int System);

    void on_button_About_clicked();

private:
    Ui::MainWindow *ui;
    QProcess* p1 = new QProcess(this);
    QProcess* p2 = new QProcess(this);
    QString* path_System = new QString;
    QString* path_OSLoader = new QString;
    QFileInfo* edbINFO = new QFileInfo(QDir::currentPath() + "/edb.exe");
    QFileInfo* sbloaderINFO = new QFileInfo(QDir::currentPath() + "/sb_loader.exe");

    Options* optionsWindow = new Options(this);
    About* aboutWindow = new About(this);

};
#endif // MAINWINDOW_H
