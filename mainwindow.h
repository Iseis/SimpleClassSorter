#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QTreeWidgetItem>
#include <QStringList>
#include <QDropEvent>
#include <QDropEvent>
#include <QFileDialog>
#include <QListWidgetItem>
#include <QMessageBox>
#include <QDebug>
#include <QHash>
#include"ktreewidget.h"
#include "student.h"
#include "team.h"
#include "class_list.h"

class QDropEvent;

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private slots:
    void on_addStudentsButton_clicked();

    void on_saveTeamButton_clicked();

    void on_createBlankTeam_clicked();

    void on_sortButton_clicked();

    void on_classListWidget_itemClicked(QListWidgetItem *item);

    void on_actionNew_triggered();

    void on_deleteStudenButton_clicked();

private:
    Ui::MainWindow *ui;
    classList *list;
    QStringList studentList;
    QTreeWidgetItem *addRoot(QString name);
    void addChild(QTreeWidgetItem *parent, team *tm);
    student *parseStudentFile(QString file);
    int teamCount;
    KTreeWidget *treeWidget;
    bool teamEdited;
};

#endif // MAINWINDOW_H
