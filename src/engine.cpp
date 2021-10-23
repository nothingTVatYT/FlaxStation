#include "engine.h"
#include <QDir>
#include <QDebug>

Engine::Engine(QString name, QString path, QString version)
{
    this->name = name;
    this->path = path;
    this->version = version;
    if (name.isEmpty())
    {
        this->name = this->path.mid(this->path.lastIndexOf('/')+1);
    }
}

QString Engine::editorPath()
{
#if defined(Q_OS_LINUX)
    QString osname = "Linux";
    QString executable = "FlaxEditor";
#elif defined(Q_OS_WIN)
    QString osname = "Windows";
    QString executable = "FlaxEditor.exe";
#elif defined(Q_OS_MACOS)
    QString osname = "MacOSX";
    QString executable = "FlaxEditor";
#endif
    if (QFile(path + "/Binaries/Editor/" + osname + "/Release/" + executable).exists())
    {
        return path + "/Binaries/Editor/" + osname + "/Release/" + executable;
    }
    if (QFile(path + "/Binaries/Editor/" + osname + "/Development/" + executable).exists())
    {
        return path + "/Binaries/Editor/" + osname + "/Development/" + executable;
    }
    if (QFile(path + "/Binaries/Editor/" + osname + "/Debug/" + executable).exists())
    {
        return path + "/Binaries/Editor/" + osname + "/Debug/" + executable;
    }
    qDebug() << "No FlaxEditor at " << path << "/Binaries/Editor/*/" << osname;
    return "";
}
