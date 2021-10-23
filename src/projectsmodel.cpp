#include "projectsmodel.h"

ProjectsModel::ProjectsModel(QObject *parent) : QAbstractTableModel(parent)
{
}

int ProjectsModel::rowCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent);
    return entries.size();
}

int ProjectsModel::columnCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent);
    return 3;
}

QVariant ProjectsModel::data(const QModelIndex &index, int role) const
{
    if (!index.isValid())
        return QVariant();

   if (role == Qt::DisplayRole)
   {
       FlaxProject project = entries[index.row()];
       switch (index.column())
       {
       case 0: return project.name;
       case 1: return project.path;
       case 2: return project.engineVersion;
       }
   }
   return QVariant();
}

QVariant ProjectsModel::headerData(int section, Qt::Orientation orientation, int role) const
{
    if (role == Qt::DisplayRole)
    {
        if (orientation == Qt::Horizontal)
        {
            switch (section)
            {
            case 0: return tr("Name");
            case 1: return tr("Path");
            case 2: return tr("Engine");
            }
        }
    }
    return QVariant();
}

FlaxProject ProjectsModel::item(int index)
{
    return entries.at(index);
}

void ProjectsModel::add(const FlaxProject project)
{
    FlaxProject *existing = getProjectByPath(project.path);
    if (existing) return;
    int newIndex = entries.size();
    beginInsertRows(QModelIndex(), newIndex, newIndex);
    entries.append(project);
    endInsertRows();
}

FlaxProject *ProjectsModel::getProjectByPath(QString path)
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
