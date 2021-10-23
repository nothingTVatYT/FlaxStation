#ifndef ENGINESMODEL_H
#define ENGINESMODEL_H

#include <QAbstractTableModel>
#include "engine.h"

class EnginesModel : public QAbstractTableModel
{
    Q_OBJECT

public:
    EnginesModel(QObject *parent = nullptr);

    int rowCount(const QModelIndex &parent = QModelIndex()) const override;
    int columnCount(const QModelIndex &parent = QModelIndex()) const override;
    QVariant headerData(int section, Qt::Orientation orientation, int role = Qt::DisplayRole) const override;
    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;

    void add(const Engine engine);
    Engine item(int index);
    Engine * getEngineByPath(QString path);
private:
    QList<Engine> entries;
};

#endif // ENGINESMODEL_H
