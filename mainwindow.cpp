#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    createMenu();
    initializePointsDataModel();
    initializeTaskDataModel();
    initializeStatusBar();

    connect(ui->addTaskBtn, SIGNAL(clicked()), this, SLOT(startAddTaskDialog()));
    connect(ui->plotBtn, SIGNAL(clicked()), this, SLOT(plot()));
    connect(ui->appPointsBtn, SIGNAL(clicked()), this, SLOT(addPoint()));
    connect(ui->tasksTableView, SIGNAL(customContextMenuRequested(QPoint)),
            this, SLOT(slotCustomMenuRequestedTasks(QPoint)));
    connect(ui->pointsTableView, SIGNAL(customContextMenuRequested(QPoint)),
            this, SLOT(slotCustomMenuRequestedPoints(QPoint)));
}

/* =====================Слоты для управелния меню==================*/
void MainWindow::createMenu(){
    pm_fileMenu = menuBar()->addMenu("&Файл");
    pm_fileMenu->addAction("&Новый", this, SLOT(newFile()), Qt::CTRL+Qt::Key_N);
    pm_fileMenu->addAction("&Открыть", this, SLOT(openFile()), Qt::CTRL+Qt::Key_O);
    pm_fileMenu->addAction("&Сохранить", this, SLOT(saveFile()), Qt::CTRL+Qt::Key_S);
    pm_fileMenu->addAction("&Сохранить как", this, SLOT(saveAsFile()), Qt::CTRL+Qt::ALT+Qt::Key_S);
    pm_fileMenu->addSeparator();
    pm_fileMenu->addAction("&Выйти", qApp, SLOT(quit()), Qt::CTRL+Qt::ALT+Qt::Key_Q);

    pm_settingMenu = menuBar()->addMenu("&Настройки");
    pm_isDynamicAction = new QAction("Динамическое поле", this);
    pm_isDynamicAction->setCheckable(true);
    pm_isDynamicAction->setChecked(false);
    pm_settingMenu->addAction(pm_isDynamicAction);
    connect(pm_isDynamicAction, SIGNAL(triggered()), this, SLOT(changeDynamic()));

    pm_aboutMenu = menuBar()->addMenu("&Справка");
}

bool MainWindow::savefileDialog()
{
    QMessageBox* pmb = new QMessageBox(QMessageBox::Warning,
                                       "MessageBox",
                                       "Сохранить текущий файл?",
                                       QMessageBox::Yes | QMessageBox::No |
                                       QMessageBox::Cancel);
    int answer = pmb->exec();
    delete pmb;

    if(answer == QMessageBox::Yes)
        saveFile();
    else if(answer == QMessageBox::Cancel)
        return false;
    return true;
}

void MainWindow::newFile()
{
    if(!savefileDialog()) return void();
    m_currentFile = "";
    pm_currentFileLabel->setText("Файл не сохранен");
    initializePointsDataModel();
}

void MainWindow::openFile()
{
    if(!savefileDialog()) return void();
    QString openedFile = QFileDialog::getOpenFileName(this,
                                                      "Открыть файл с данными",
                                                      "",
                                                      "*.txt");
    m_currentFile = openedFile;
    pm_currentFileLabel->setText(m_currentFile);
    initializePointsDataModel();
    qDebug()<<m_currentFile;
    pm_pointsDataModel->appendPoint(FileManager::getDataFromFile(m_currentFile));
}

void MainWindow::saveAsFile()
{
    QString fileFormat;
    QString savedFile = QFileDialog::getSaveFileName(this,
                                                     "Сохранить данные",
                                                     "mydata.txt",
                                                     "*.txt",
                                                     &fileFormat);

    m_currentFile = savedFile;
    pm_currentFileLabel->setText(m_currentFile);
    qDebug()<<m_currentFile;
    saveFile();
}

void MainWindow::saveFile()
{
    if(m_currentFile == "") saveAsFile();
    else{
        FileManager::writeDataToFile(pm_pointsDataModel->getPoints(),m_currentFile);
    }
}

/*==============================================================================*/


/* ===========Инициализируем модели данных========== */
void MainWindow::initializePointsDataModel(){
    delete pm_pointsDataModel;
    pm_pointsDataModel = new PoinstsDataModel();
    ui->pointsTableView->setModel(pm_pointsDataModel);
    ui->pointsTableView->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    ui->pointsTableView->setContextMenuPolicy(Qt::CustomContextMenu);
}
void MainWindow::initializeTaskDataModel(){
    delete pm_tasksDataModel;
    pm_tasksDataModel = new TasksDataModel();
    ui->tasksTableView->setModel(pm_tasksDataModel);
    ui->tasksTableView->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    ui->tasksTableView->setContextMenuPolicy(Qt::CustomContextMenu);
}
/*=================================================*/


/*======================СЛОТЫ===========================*/
void MainWindow::startAddTaskDialog(){
    AddTaskDialog* addtskDialog = new AddTaskDialog;
    if(addtskDialog->exec() == QDialog::Accepted){
        pm_tasksDataModel->appendTask(addtskDialog->getType(),
                                      addtskDialog->getMethod(),
                                      addtskDialog->getColor().name());

    }
}

void MainWindow::addPoint(){
    pm_pointsDataModel->appendPoint(
                ui->lineX->text().toDouble(),
                ui->lineY->text().toDouble());
}

void MainWindow::plot(){
    QVector<QVector<double>> points = pm_pointsDataModel->getPoints();
    Solver solver = Solver(points);
    QList<QVector<QVector<double>>> lines;
    QList<QList<QString>> tasks = pm_tasksDataModel->getTasks();

    ui->area->clearGraphs();
    ui->area->clearItems(); 

    /* Отрисовываем точки */
    ui->area->addGraph();
    ui->area->graph(0)->setData(solver.getX(), solver.getY());
    ui->area->graph(0)->setPen(QColor(50, 50, 50, 255));
    ui->area->graph(0)->setLineStyle(QCPGraph::lsNone);
    ui->area->graph(0)->setScatterStyle(QCPScatterStyle(QCPScatterStyle::ssCircle, 6));
    ui->area->graph(0)->setName("Точки");
    /*================================================================================*/

    /* Отрисовываем графики */
    for(int i = 0; i < tasks.size(); i++){
        lines.append(solver.getLine(tasks[i][0], tasks[i][1]));
        QPen pen;
        pen.setColor(QColor(tasks[i][2]));
        pen.setWidthF(2);
        ui->area->addGraph();
        ui->area->graph(i+1)->setData(lines[i][0], lines[i][1]);
        ui->area->graph(i+1)->setPen(pen);
        ui->area->graph(i+1)->setName(tasks[i][1]);
        //ui->area->legend->addElement(i+1, 1, ui->area->legend->item(i+1));
    }
    ui->area->legend-> setVisible (true);
    ui->area->xAxis->setRange(solver.getXmin()-5, solver.getXmax()+5);
    ui->area->yAxis->setRange(solver.getYmin()-3, solver.getYmax()+3);
    ui->area->replot();

    qDebug()<<solver.getCorr();
    qDebug()<<solver.getMSD();

    ui->corrLbl->setText("R="+QString::number(solver.getCorr(),'g',4));
    ui->msdLbl->setText("D="+QString::number(solver.getMSD(), 'g', 2));
}

void MainWindow::slotCustomMenuRequestedTasks(QPoint pos)
{
    QMenu * contextMenu = new QMenu(this);
    QAction * deleteRows = new QAction("Удалить", this);
    connect(deleteRows, SIGNAL(triggered()), this, SLOT(slotRemoveRowsTasks()));
    contextMenu->addAction(deleteRows);
    contextMenu->popup(ui->tasksTableView->viewport()->mapToGlobal(pos));
}


void MainWindow::slotRemoveRowsTasks()
{
    int countSelectedRows = ui->tasksTableView->selectionModel()->selectedRows().count();
    qDebug() <<countSelectedRows;
    if(countSelectedRows){
    for( int i = countSelectedRows-1; i >= 0; i--)
        ui->tasksTableView->model()->removeRow( ui->tasksTableView->selectionModel()->selectedRows().at( i).row(), QModelIndex());
    }
}

void MainWindow::slotCustomMenuRequestedPoints(QPoint pos)
{
    QMenu * contextMenu = new QMenu(this);
    QAction * deleteRows = new QAction("Удалить", this);
    connect(deleteRows, SIGNAL(triggered()), this, SLOT(slotRemoveRowsPoints()));
    contextMenu->addAction(deleteRows);
    contextMenu->popup(ui->pointsTableView->viewport()->mapToGlobal(pos));
}


void MainWindow::slotRemoveRowsPoints()
{
    int countSelectedRows = ui->pointsTableView->selectionModel()->selectedRows().count();
    if(countSelectedRows){
    for( int i = countSelectedRows-1; i >= 0; i--)
        ui->pointsTableView->model()->removeRow( ui->pointsTableView->selectionModel()->selectedRows().at(i).row(), QModelIndex());
    }
}

/*==================================================================================*/

void MainWindow::initializeStatusBar()
{
    pm_currentFileLabel = new QLabel("Файл не сохранен", this);
    statusBar()->addWidget(pm_currentFileLabel);
}

void MainWindow::changeDynamic()
{
    ui->area->setInteraction(QCP::iRangeDrag, pm_isDynamicAction->isChecked());
}

MainWindow::~MainWindow()
{
}

