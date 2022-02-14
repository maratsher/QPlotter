#include "addtaskdialog.h"

AddTaskDialog::AddTaskDialog(QWidget* wgt) : QDialog(wgt)
{

    QGridLayout *grid = new QGridLayout(this);

    QLabel* label_type = new QLabel("Тип задачи", this);
    QLabel* label_method = new QLabel("Метод решения", this);
    QLabel* label_color = new QLabel("Цвет линии", this);

    QPushButton* okBtn = new QPushButton("&Ок");
    QPushButton* cancelBtn = new QPushButton("&Cancel");

    typeCombo = new QComboBox();
    typeCombo->addItems(Ltypes);
    methodCombo = new QComboBox();
    methodCombo->addItems(Lmethod_app);
    colorCombo = new QPushButton("Выбрать цвет");

    grid->addWidget(label_type, 0, 0);
    grid->addWidget(label_method, 0, 1);
    grid->addWidget(label_color, 0, 2);
    grid->addWidget(typeCombo, 1, 0);
    grid->addWidget(methodCombo, 1, 1);
    grid->addWidget(colorCombo, 1, 2);
    grid->addWidget(okBtn, 2, 1);
    grid->addWidget(cancelBtn, 2, 2);

    connect(typeCombo, SIGNAL(activated(QString)),
        this, SLOT(setType(QString)));
    connect(methodCombo, SIGNAL(activated(QString)),
        this, SLOT(setMethod(QString)));
    connect(colorCombo, SIGNAL(clicked()),
        this, SLOT(setColor()));
    connect(okBtn, SIGNAL(clicked()), SLOT(accept()));
    connect(cancelBtn, SIGNAL(clicked()), SLOT(reject()));
}

void AddTaskDialog::setType(const QString& sel)
{
    selectedType = sel;
    methodCombo->clear();
    if(selectedType == Ltypes[APPROXIMATION]){
        methodCombo->addItems(Lmethod_app);
    }
    else if(selectedType == Ltypes[INTERPOLATION]){
        methodCombo->addItems(Lmethod_inter);
        selectedMethod = Lmethod_inter[LINEAT_SPLINE];
    }

}

void AddTaskDialog::setMethod(const QString& sel)
{
    selectedMethod = sel;
}

void AddTaskDialog::setColor()
{
    selectedColor = QColorDialog::getColor();
    colorCombo->setText(selectedColor.name());
    QPalette pal = colorCombo->palette();
    pal.setColor(QPalette::Button, selectedColor);
    colorCombo->setAutoFillBackground(true);
    colorCombo->setPalette(pal);
    colorCombo->update();
}

