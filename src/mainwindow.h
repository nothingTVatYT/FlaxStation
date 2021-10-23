#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QDir>

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
    void AddInstalledEngine();

private:
    Ui::MainWindow *ui;
    void OpenLink(QString link);
    void SaveSettings();
    QList<QDir> enginePaths;
};
#endif // MAINWINDOW_H
