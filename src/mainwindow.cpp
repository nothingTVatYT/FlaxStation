#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QSettings>
#include <QFileDialog>
#include <QMessageBox>
#include <QJsonDocument>
#include <QJsonObject>

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
    QObject::connect(ui->bProjects, &QCommandLinkButton::pressed, this, &MainWindow::ShowProjects);
    QObject::connect(ui->btAddEngine, &QCommandLinkButton::pressed, this, &MainWindow::AddInstalledEngine);
    QObject::connect(ui->btDownloadEngine, &QCommandLinkButton::pressed, this, &MainWindow::DownloadEngine);
    QObject::connect(ui->btAddProject, &QCommandLinkButton::pressed, this, &MainWindow::AddProject);

    OpenLink(LINKNEWS);

    ReadSettings();

    ui->listEngines->setModel(&engines);
    ui->listEngines->horizontalHeader()->setSectionResizeMode(QHeaderView::ResizeToContents);
    ui->listProjects->setModel(&projects);
    ui->listProjects->horizontalHeader()->setSectionResizeMode(QHeaderView::ResizeToContents);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::OpenLink(QString link)
{
    ui->engines->hide();
    ui->projects->hide();
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
    ui->projects->hide();
    ui->engines->show();
}

void MainWindow::ShowProjects()
{
    ui->webView->hide();
    ui->projects->show();
    ui->engines->hide();
}

void MainWindow::ReadSettings()
{
    QSettings settings(QCoreApplication::organizationName(), QCoreApplication::applicationName());
    int nEngines = settings.beginReadArray("engines");
    for (int i = 0; i < nEngines; i++)
    {
        settings.setArrayIndex(i);
        AddInstalledEngineAt(QDir(settings.value("enginePath").toString()));
    }
    settings.endArray();
    int nProjects = settings.beginReadArray("projects");
    for (int i = 0; i < nProjects; i++)
    {
        settings.setArrayIndex(i);
        AddProjectAt(settings.value("projectPath").toString());
    }
    settings.endArray();
}

void MainWindow::SaveSettings()
{
    QSettings settings(QCoreApplication::organizationName(), QCoreApplication::applicationName());

    settings.beginWriteArray("engines");
    for (int i = 0; i < engines.rowCount(); i++)
    {
        settings.setArrayIndex(i);
        settings.setValue("enginePath", engines.item(i).path);
    }
    settings.endArray();

    settings.beginWriteArray("projects");
    for (int i = 0; i < projects.rowCount(); i++)
    {
        settings.setArrayIndex(i);
        settings.setValue("projectPath", projects.item(i).path);
    }
    settings.endArray();

    settings.sync();
}

void MainWindow::AddInstalledEngine()
{
    QString dir = QFileDialog::getExistingDirectory(this, tr("Select Engine"), ".", QFileDialog::ShowDirsOnly);
    if (!dir.isEmpty()) {
        AddInstalledEngineAt(QDir(dir));
        SaveSettings();
    }
}

void MainWindow::DownloadEngine()
{
    QMessageBox::warning(this, tr("Sorry"), tr("This function is not implemented yet."));
}

void MainWindow::AddInstalledEngineAt(QDir dir)
{
    if (dir.exists("Flax.flaxproj"))
    {
        QFile projectFile(dir.path() + "/Flax.flaxproj");
        if (projectFile.open(QIODevice::ReadOnly))
        {
            QByteArray content = projectFile.readAll();
            QJsonParseError parseError;
            QJsonDocument jsonObject(QJsonDocument::fromJson(content, &parseError));
            QJsonObject project = jsonObject.object();
            QJsonObject version = project["Version"].toObject();
            QString engineVersion = QString("%1.%2.%3").arg(version["Major"].toInt(0)).arg(version["Minor"].toInt(0)).arg(version["Build"].toInt(0));

            engines.add(Engine(dir.dirName(), dir.path(), engineVersion));
        } else {
            QMessageBox::warning(this, tr("Warning"), tr("Cannot read project file"));
        }
    } else {
        QMessageBox::warning(this, tr("Warning"), tr("This is not a project directory"));
    }
}

void MainWindow::AddProject()
{
    QString filePath = QFileDialog::getOpenFileName(this, tr("Select Project"), ".", tr("Flax Projects (*.flaxproj)"));
    if (!filePath.isEmpty()) {
        AddProjectAt(filePath);
    }
    SaveSettings();
}

void MainWindow::AddProjectAt(QString filepath)
{
    QFile projectFile(filepath);
    if (projectFile.exists())
    {
        FlaxProject project;
        project.path = filepath;
        if (projectFile.open(QIODevice::ReadOnly))
        {
            QByteArray content = projectFile.readAll();
            QJsonParseError parseError;
            QJsonDocument jsonObject(QJsonDocument::fromJson(content, &parseError));
            QJsonObject jsonProject = jsonObject.object();
            project.name = jsonProject["Name"].toString("<none>");
            project.engineVersion = jsonProject["MinEngineVersion"].toString("");

            projects.add(project);
        }
    } else {
        QMessageBox::warning(this, tr("Error loading project"), tr("Could not load project at ") + filepath);
    }
}
