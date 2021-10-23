#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QSettings>
#include <QFileDialog>
#include <QJsonDocument>

static const char * LINKNEWS = "https://flaxengine.com/";
static const char * LINKSTORE = "https://flaxengine.com/download/";
static const char * LINKLEARN = "https://flaxengine.com/learn/";

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    QObject::connect(ui->bNews, SIGNAL(clicked()), this, SLOT(OpenNewsLink()));
    QObject::connect(ui->bStore, SIGNAL(clicked()), this, SLOT(OpenStoreLink()));
    QObject::connect(ui->bLearn, SIGNAL(clicked()), this, SLOT(OpenLearnLink()));
    QObject::connect(ui->bEngine, SIGNAL(clicked()), this, SLOT(ShowEngines()));
    QObject::connect(ui->btAddEngine, SIGNAL(clicked()), this, SLOT(AddInstalledEngine()));

    OpenLink(LINKNEWS);
    QSettings settings(QCoreApplication::organizationName(), QCoreApplication::applicationName());
    int nEngines = settings.beginReadArray("engines");
    for (int i = 0; i < nEngines; i++)
    {
        settings.setArrayIndex(i);
        enginePaths.append(QDir(settings.value("enginePath").toString()));
    }
    settings.endArray();
    for (int i = 0; i < enginePaths.size(); i++)
    {
        ui->listEngines->addItem(enginePaths.at(i).path());
    }
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::OpenLink(QString link)
{
    ui->engines->hide();
    ui->webView->show();
    ui->statusbar->showMessage(link, 3000);
    ui->webView->load(QUrl(link));
}

void MainWindow::OpenNewsLink()
{
    OpenLink(LINKNEWS);
}

void MainWindow::OpenStoreLink()
{
    OpenLink(LINKSTORE);
}

void MainWindow::OpenLearnLink()
{
    OpenLink(LINKLEARN);
}

void MainWindow::ShowEngines()
{
    ui->webView->hide();
    ui->engines->show();
}

void MainWindow::SaveSettings()
{
    QSettings settings(QCoreApplication::organizationName(), QCoreApplication::applicationName());
    settings.beginWriteArray("engines");
    for (int i = 0; i < enginePaths.size(); i++)
    {
        settings.setArrayIndex(i);
        settings.setValue("enginePath", enginePaths.at(i).path());
    }
    settings.endArray();
    settings.sync();
}

void MainWindow::AddInstalledEngine()
{
    QDir dir = QFileDialog::getExistingDirectory(this, "Select Engine", ".", QFileDialog::ShowDirsOnly);
    if (!enginePaths.contains(dir))
    {
        if (dir.exists("Flax.flaxproj"))
        {
            QFile projectFile(dir.path() + "/Flax.flaxproj");
            if (projectFile.open(QIODevice::ReadOnly))
            {
                QByteArray content = projectFile.readAll();
                QJsonDocument jsonObject(QJsonDocument::fromJson(content));
                enginePaths.append(dir);
                ui->listEngines->addItem(dir.path());
                SaveSettings();
            }
        }
    }
}
