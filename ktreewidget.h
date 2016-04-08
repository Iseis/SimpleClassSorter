#ifndef KTREEWIDGET
#define KTREEWIDGET
#include <QTreeWidget>
#include <QMimeData>
#include <QListWidget>
#include <QTextBrowser>
#include "class_list.h"
#include "student.h"
#include <QDropEvent>
#include <QDebug>

class KTreeWidget: public QTreeWidget{
 Q_OBJECT

public slots:
 void on_treeWidget_itemClicked(QTreeWidgetItem *item, int column);

public:
 KTreeWidget(QWidget* w = NULL, classList *list = NULL, QListWidget *widg = NULL, QTextBrowser *text = NULL, QStringList *studentList=NULL);

private:
 void dropEvent(QDropEvent *event);
 void itemClicked(QTreeWidgetItem *item);
 QString printPrefrences(team *t);
 classList *list;
 QListWidget *students;
 QTreeWidgetItem* _header;
 QTextBrowser *text;
 QStringList *studentList;
};

#endif // KTREEWIDGET

