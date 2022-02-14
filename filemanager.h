#ifndef FILEMANAGER_H
#define FILEMANAGER_H

#include <QString>
#include <QFile>
#include <QVector>
#include <QTextStream>

class FileManager
{
public:
    FileManager();
    static QVector<QVector<double>> getDataFromFile(QString filePath);
    static void writeDataToFile(QVector<QVector<double>>, QString filePath);
    static int getCountRowInFile(QFile &file);
};

#endif // FILEMANAGER_H
