#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QApplication>
#include <QMainWindow>
#include <QMenu>
#include <QMenuBar>
#include <QAction>
#include <QMessageBox>
#include <QFileDialog>
#include <QDebug>
#include <QTableView>
#include <QLayout>
#include <QHeaderView>
#include <QStatusBar>
#include <QPushButton>
#include <QLineEdit>

#include "filemanager.h"
#include "poinstsdatamodel.h"
#include "tasksdatamodel.h"
#include "addtaskdialog.h"
#include "solver.h"
//#include "namespace.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private:
    Ui::MainWindow *ui;
    QMenu* pm_fileMenu;
    QMenu* pm_settingMenu;
    QMenu* pm_aboutMenu;
    QString m_currentFile = "";
    PoinstsDataModel* pm_pointsDataModel = nullptr;
    TasksDataModel* pm_tasksDataModel = nullptr;
    QLabel* pm_currentFileLabel;
    QAction* pm_isDynamicAction;

    void createMenu();

private slots:
    void newFile();
    void openFile();
    void saveFile();
    void saveAsFile();
    void startAddTaskDialog();
    void plot();
    bool savefileDialog();
    void addPoint();
    void slotCustomMenuRequestedTasks(QPoint pos);
    void slotRemoveRowsTasks();
    void slotCustomMenuRequestedPoints(QPoint pos);
    void slotRemoveRowsPoints();
    void changeDynamic();

private:
    void initializePointsDataModel();
    void initializeTaskDataModel();
    void initializeStatusBar();

};
#endif // MAINWINDOW_H
