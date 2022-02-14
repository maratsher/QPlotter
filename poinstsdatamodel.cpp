#include "poinstsdatamodel.h"

PoinstsDataModel::PoinstsDataModel(QObject* parent) : QAbstractTableModel(parent) {}

int PoinstsDataModel::rowCount( const QModelIndex& parent ) const {
    Q_UNUSED( parent )
    return m_points.count();
}

int PoinstsDataModel::columnCount( const QModelIndex& parent ) const {
    Q_UNUSED( parent )
    return Y+1;
}

QVariant PoinstsDataModel::data( const QModelIndex& index, int role ) const {
    if(
            !index.isValid() ||
            m_points.count() <= index.row() ||
            (role != Qt::DisplayRole && role != Qt::EditRole)
    )
    {
        return QVariant();
    }

    return m_points[ index.row() ][ Column( index.column() ) ];
}

bool PoinstsDataModel::setData( const QModelIndex& index, const QVariant& value, int role ) {
    if( !index.isValid() || role != Qt::EditRole || m_points.count() <= index.row() ) {
        return false;
    }

    m_points[ index.row() ][ Column( index.column() ) ] = value;
    emit dataChanged( index, index ); // отправляем сигнал о том, что данные изменены

    return true;
}

QVariant PoinstsDataModel::headerData( int section, Qt::Orientation orientation, int role ) const {
    if( role != Qt::DisplayRole ) {
        return QVariant();
    }

    if( orientation == Qt::Vertical ) {
        return section+1;
    }

    switch( section ) {
    case X:
        return QString( "X" );
    case Y:
        return QString( "Y" );
    }

    return QVariant();
}

Qt::ItemFlags PoinstsDataModel::flags( const QModelIndex& index ) const {
    Qt::ItemFlags flags = QAbstractTableModel::flags( index );
    if( index.isValid() ) {
        flags |= Qt::ItemIsEditable;
    }

    return flags;
}

bool PoinstsDataModel::removeRows(int position, int rows, const QModelIndex &index)
{
    Q_UNUSED(index)
    beginRemoveRows(QModelIndex(), position, position + rows - 1);

    for (int i = 0; i < rows; i++)
    {
        m_points.removeAt(i);
    }
    endRemoveRows();
    return true;
}

void PoinstsDataModel::appendPoint(double x, double y) {
    Point point;
    point[X] = x;
    point[Y] = y;

    int row = m_points.count();
    beginInsertRows( QModelIndex(), row, row );
    m_points.append( point );
    endInsertRows();
}

void PoinstsDataModel::appendPoint(QVector<QVector<double>> points) {
    for(int i = 0; i < points.count(); i++){
        appendPoint(points[i][X], points[i][Y]);
    }
}

QVector<QVector<double>> PoinstsDataModel::getPoints()
{
    QVector<QVector<double>> tempMatrix;
    for(int i = 0; i < m_points.count(); i++){
        QVector<double> tempVector = { (m_points[i][Column(X)]).toDouble(),
                                       (m_points[i][Column(Y)]).toDouble()};
        tempMatrix.append(tempVector);
    }
    return tempMatrix;
}
