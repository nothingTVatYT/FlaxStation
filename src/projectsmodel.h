#ifndef PROJECTSMODEL_H
#define PROJECTSMODEL_H

#include <QAbstractTableModel>
#include "flaxproject.h"

class ProjectsModel : public QAbstractTableModel
{
    Q_OBJECT

public:
    ProjectsModel(QObject *parent = nullptr);

    int rowCount(const QModelIndex &parent = QModelIndex()) const override;
    int columnCount(const QModelIndex &parent = QModelIndex()) const override;
    QVariant headerData(int section, Qt::Orientation orientation, int role = Qt::DisplayRole) const override;
    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;

    void add(const FlaxProject engine);
    FlaxProject item(int index);
    FlaxProject * getProjectByPath(QString path);
private:
    QList<FlaxProject> entries;
};

#endif // PROJECTSMODEL_H
