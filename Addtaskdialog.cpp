#include "addtaskdialog.h"

AddTaskDialog::addTaskDialog(QWidget* wgt) : QDialog(wgt)
{

    QGridLayout *grid = new QGridLayout(this);
    types = new QStringList({"Аппроксимация", "Интерполяция"});
    colors = new QStringList({"Цвет"});

    QLabel* label_type = new QLabel("Тип задачи", this);
    QLabel* label_method = new QLabel("Метод решения", this);
    QLabel* label_color = new QLabel("Цвет линии", this);

    typeCombo = new QComboBox();
    typeCombo->addItems(*types);
    methodCombo = new QComboBox();
    colorCombo = new QComboBox();
    colorCombo->addItems(*colors);

    grid->addWidget(label_type, 0, 0);
    grid->addWidget(label_method, 0, 1);
    grid->addWidget(label_color, 0, 2);
    grid->addWidget(typeCombo, 1, 0);
    grid->addWidget(methodCombo, 1, 1);
    grid->addWidget(colorCombo, 1, 2);

    connect(typeCombo, static_cast<void(QComboBox::*)(const QString &)>(&QComboBox::activated),
        this, SLOT(setType()));
    connect(methodCombo, static_cast<void(QComboBox::*)(const QString &)>(&QComboBox::activated),
        this, SLOT(setMethod()));
    connect(colorCombo, static_cast<void(QComboBox::*)(const QString &)>(&QComboBox::activated),
        this, SLOT(setColor()));
}

void addTaskDialog::setType()
{
    selectedType = typeCombo->currentText();
    if(selectedType == types->at(0)){
        methods = new QStringList({"Линейный", "Ньютона"});
    }
    else if(selectedType == types->at(1)){
        methods = new QStringList({"Кусочно", "Лейбница"});
    }
    methodCombo->addItems(*methods);

}

void addTaskDialog::setMethod()
{
    selectedMethod = methodCombo->currentText();
}

void addTaskDialog::setColor()
{
    selectedColor = colorCombo->currentText();
}
