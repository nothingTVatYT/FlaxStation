#include "enginesmodel.h"
#include <QSize>

EnginesModel::EnginesModel(QObject *parent)
    : QAbstractTableModel(parent)
{
}

int EnginesModel::rowCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent);
    return entries.size();
}

int EnginesModel::columnCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent);
    return 3;
}

QVariant EnginesModel::data(const QModelIndex &index, int role) const
{
    if (!index.isValid())
        return QVariant();

   if (role == Qt::DisplayRole)
   {
       Engine engine = entries[index.row()];
       switch (index.column())
       {
       case 0: return engine.name;
       case 1: return engine.version;
       case 2: return engine.path;
       }
   }
   return QVariant();
}

QVariant EnginesModel::headerData(int section, Qt::Orientation orientation, int role) const
{
    if (role == Qt::DisplayRole)
    {
        if (orientation == Qt::Horizontal)
        {
            switch (section)
            {
            case 0: return tr("Name");
            case 1: return tr("Version");
            case 2: return tr("Path");
            }
        }
    }
    return QVariant();
}

Engine EnginesModel::item(int index)
{
    return entries.at(index);
}

void EnginesModel::add(const Engine engine)
{
    Engine *existing = getEngineByPath(engine.path);
    if (existing) return;
    int newIndex = entries.size();
    beginInsertRows(QModelIndex(), newIndex, newIndex);
    entries.append(engine);
    endInsertRows();
}

Engine *EnginesModel::getEngineByPath(QString path)
{
    for (int i = 0; i < entries.size(); i++)
    {
        if (entries[i].path == path)
        {
            return &entries[i];
        }
    }
    return NULL;
}

int EnginesModel::getEnginesByVersion(QString version, QList<Engine> *result)
{
    for (int i = 0; i < entries.size(); i++)
    {
        if (entries[i].version == version)
        {
            result->append(entries[i]);
        }
    }
    return result->size();
}

bool versionIsHigherOrEqual(QString version1, QString version2)
{
    QStringList v1 = version1.split('.');
    QStringList v2 = version2.split('.');
    for (int i = 0; i < v1.length(); i++)
    {
        if (v2.length() < i)
        {
            int v1i = v1[i].toInt();
            int v2i = v2[i].toInt();
            if (v2i > v1i)
            {
                return false;
            } else if (v2i < v1i)
            {
                return true;
            }
        }
    }
    return true;
}

int EnginesModel::getEnginesByMinimalVersion(QString version, QList<Engine> *result)
{
    for (int i = 0; i < entries.size(); i++)
    {
        if (versionIsHigherOrEqual(entries[i].version, version))
        {
            result->append(entries[i]);
        }
    }
    return result->size();
}
