#ifndef POINSTSDATAMODEL_H
#define POINSTSDATAMODEL_H

#include <QAbstractTableModel>
#include <QVector>

class PoinstsDataModel : public QAbstractTableModel
{
    Q_OBJECT

public:
    PoinstsDataModel( QObject* parent = 0 );
    virtual~PoinstsDataModel() {};

    int rowCount( const QModelIndex& parent ) const;
    int columnCount( const QModelIndex& parent ) const;
    QVariant data( const QModelIndex& index, int role ) const;
    bool setData( const QModelIndex& index, const QVariant& value, int role );
    QVariant headerData( int section, Qt::Orientation orientation, int role ) const;
    Qt::ItemFlags flags( const QModelIndex& index ) const;
    bool removeRows( int position, int rows, const QModelIndex &index );

    void appendPoint(double x, double y);
    void appendPoint(QVector<QVector<double>> points);
    QVector<QVector<double>> getPoints();

public slots:

private:
    enum Column {X,Y}; // колонки

    typedef QHash< Column, QVariant > Point; // определяем свой тип данных - Точка
    typedef QList< Point > Points; // опредеяем тип данных - Лист из точек
    Points m_points; // тут храняться точки
};

#endif // POINSTSDATAMODEL_H
