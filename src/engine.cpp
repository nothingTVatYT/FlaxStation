#include "engine.h"

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

