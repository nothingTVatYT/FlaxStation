#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QDir>
#include "enginesmodel.h"
#include "projectsmodel.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
public slots:
    void OpenNewsLink();
    void OpenStoreLink();
    void OpenLearnLink();
    void ShowEngines();
    void ShowProjects();
    void AddInstalledEngine();
    void AddInstalledEngineAt(QDir dir);
    void DownloadEngine();
    void AddProject();
    void AddProjectAt(QString filePath);

private:
    Ui::MainWindow *ui;
    void OpenLink(QString link);
    void ReadSettings();
    void SaveSettings();
    EnginesModel engines;
    ProjectsModel projects;

};
#endif // MAINWINDOW_H
