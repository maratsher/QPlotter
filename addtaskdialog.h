#ifndef ADDTASKDIALOG_H
#define ADDTASKDIALOG_H

#include <QDialog>
#include <QLabel>
#include <QComboBox>
#include <QStringList>
#include <QGridLayout>
#include <QPushButton>
#include <QDebug>
#include <QColorDialog>

#include <namespace.h>

using namespace base;

class AddTaskDialog : public QDialog
{
    Q_OBJECT

private:
    QComboBox* typeCombo;
    QComboBox* methodCombo;
    QPushButton* colorCombo;

    QString selectedMethod = Lmethod_app[LINEAR];
    QString selectedType = Ltypes[APPROXIMATION];
    QColor selectedColor = QColor("blue");

public:
    QString getType() const {return selectedType;}
    QString getMethod() const {return selectedMethod;}
    QColor getColor() const {return selectedColor;}
    AddTaskDialog(QWidget* wdg = nullptr);

public slots:
    void setType(const QString& sel);
    void setMethod(const QString& sel);
    void setColor();
};

#endif // ADDTASKDIALOG_H
