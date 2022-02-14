#include "filemanager.h"

FileManager::FileManager(){}

/*?плохой код, т.к ограничен оперативой памятью?*/
int FileManager::getCountRowInFile(QFile &file)
{
    return QTextStream(&file).readAll().split('\n').count();
}

QVector<QVector<double>> FileManager::getDataFromFile(QString filePath)
{
    QFile file(filePath);
    QVector<QVector<double>> tempMatrix = {};

    if (!file.open(QIODevice::ReadOnly)) qWarning("Cannot open file for reading");
    QTextStream stream(&file);

    while (!stream.atEnd()) {
        QVector<double> tempVector;
        QStringList lststr = stream.readLine().split(QLatin1Char(' '));
        tempVector = {lststr[0].toDouble(), lststr[1].toDouble()};
        tempMatrix.append(tempVector);
    }
    file.close();
    return tempMatrix;
}
void FileManager::writeDataToFile(QVector<QVector<double>> data, QString filePath)
{
    QFile file(filePath);
    if (!file.open(QIODevice::WriteOnly)) qWarning("Cannot open file for reading");
    QTextStream stream(&file);

    for(int i = 0; i < data.count(); i++){
        QVector<double> line = data[i];
        stream<<QString::number(line[0])<<" "<<QString::number(line[1])<<endl;
    }

    file.close();
}

