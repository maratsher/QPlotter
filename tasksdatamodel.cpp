#include "tasksdatamodel.h"

TasksDataModel::TasksDataModel(QObject* parent) : QAbstractTableModel(parent) {}

int TasksDataModel::rowCount( const QModelIndex& parent ) const {
    Q_UNUSED( parent )
    return m_tasks.count();
}

int TasksDataModel::columnCount( const QModelIndex& parent ) const {
    Q_UNUSED( parent )
    return COLOR+1;
}

QVariant TasksDataModel::data( const QModelIndex& index, int role ) const {

    if (role == Qt::BackgroundRole && index.column() == 2)
        return QBrush(QColor( m_tasks[ index.row() ][ Column(index.column()) ].toString() ));

    if(
            !index.isValid() ||
            m_tasks.count() <= index.row() ||
            (role != Qt::DisplayRole && role != Qt::EditRole)
    )
    {
        return QVariant();
    }

    return m_tasks[ index.row() ][ Column( index.column() ) ];
}

bool TasksDataModel::setData( const QModelIndex& index, const QVariant& value, int role ) {
    if( !index.isValid() || role != Qt::EditRole || m_tasks.count() <= index.row() ) {
        return false;
    }

    m_tasks[ index.row() ][ Column( index.column() ) ] = value;
    emit dataChanged( index, index ); // отправляем сигнал о том, что данные изменены

    return true;
}

QVariant TasksDataModel::headerData( int section, Qt::Orientation orientation, int role ) const {
    if( role != Qt::DisplayRole ) {
        return QVariant();
    }

    if( orientation == Qt::Vertical ) {
        return section+1;
    }

    switch( section ) {
    case TYPE:
        return QString( "Тип задачи" );
    case METHOD:
        return QString( "Метод решения" );
    case COLOR:
        return QString( "Цвет" );
    }

    return QVariant();
}

Qt::ItemFlags TasksDataModel::flags( const QModelIndex& index ) const {
    Qt::ItemFlags flags = QAbstractTableModel::flags( index );
    if( index.isValid() ) {
        flags |= Qt::ItemIsEditable;
    }

    return flags;
}

bool TasksDataModel::removeRows(int position, int rows, const QModelIndex &index)
{
    Q_UNUSED(index)
    beginRemoveRows(QModelIndex(), position, position + rows - 1);

    for (int i = 0; i < rows; i++)
    {
        m_tasks.removeAt(i);
    }
    endRemoveRows();
    return true;
}

void TasksDataModel::appendTask(QString type, QString method, QString color) {
    Task task;
    task[TYPE] = type;
    task[METHOD] = method;
    task[COLOR] = color;

    int row = m_tasks.count();
    beginInsertRows( QModelIndex(), row, row );
    m_tasks.append( task );
    endInsertRows();
}

QList<QList<QString>> TasksDataModel::getTasks()
{
    QList<QList<QString>> tempMatrix;
    for(int i = 0; i < m_tasks.count(); i++){
        QList<QString> tempVector = {
            (m_tasks[i][Column(TYPE)]).toString(),
            (m_tasks[i][Column(METHOD)]).toString(),
            (m_tasks[i][Column(COLOR)].toString())
        };
        tempMatrix.append(tempVector);
    }
    return tempMatrix;
}

