#ifndef ENGINE_H
#define ENGINE_H

#include <QString>

class Engine
{
public:
    explicit Engine(QString name, QString path, QString version);
    QString path;
    QString name;
    QString version;

    QString editorPath();

    static QString osName();
private:
};

#endif // ENGINE_H
