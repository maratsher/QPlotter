#ifndef TASKSDATAMODEL_H
#define TASKSDATAMODEL_H

#include <QAbstractTableModel>
#include <QVector>
#include <QBrush>
#include <QColor>

class TasksDataModel : public QAbstractTableModel
{
    Q_OBJECT

public:
    TasksDataModel( QObject* parent = 0 );
    virtual~TasksDataModel() {};

    int rowCount( const QModelIndex& parent ) const;
    int columnCount( const QModelIndex& parent ) const;
    QVariant data( const QModelIndex& index, int role ) const;
    bool setData( const QModelIndex& index, const QVariant& value, int role );
    QVariant headerData( int section, Qt::Orientation orientation, int role ) const;
    Qt::ItemFlags flags( const QModelIndex& index ) const;
    bool removeRows( int position, int rows, const QModelIndex &index );

    void appendTask(QString type, QString method, QString color);
    QList<QList<QString>> getTasks();

public slots:

private:
    enum Column {
        TYPE,
        METHOD,
        COLOR,
    }; // колонки

    typedef QHash< Column, QVariant > Task; // определяем свой тип данных - Задача
    typedef QList< Task > Tasks; // опредеяем тип данных - Лист из задач
    Tasks m_tasks; // тут храняться задачи
};

#endif // TASKSDATAMODEL_H
