#include "KTreeWidget.h"

//Reimplement treeWidget to do different drap and drop things.
KTreeWidget::KTreeWidget(QWidget* w, classList *list, QListWidget *widg, QTextBrowser *text, QStringList *studentList):QTreeWidget(w){
     setColumnCount(1);
     _header = new QTreeWidgetItem(NULL);
     _header->setText(0,"Teams");
     this->setDefaultDropAction(Qt::MoveAction);
     setHeaderItem(_header);
     setDragEnabled(true);
     setAcceptDrops(true);
     this->list = list;
     this->studentList = studentList;
     students = widg;
     this->text = text;
}

//custom dropEvent for when we drag from classListWidget into this widget
void KTreeWidget::dropEvent(QDropEvent *event)
{
    //so if no team name then don't drop anything
    if(this->itemAt(event->pos()) != 0x0)
    {
        //if not a parent item in tree don't drop also if drag originated from this widget
        if(this->itemAt(event->pos())->parent() == NULL && event->source() == this)
        {
            //get name of current item
            QString name = this->currentItem()->text(0);
            QString firstName = name.section(' ', 0, 0);
            QString lastName = name.section(' ', 1);
            int stuTeamNum = this->currentItem()->parent()->text(0).section(" ", 1).toInt();
            qDebug() << stuTeamNum;
            int toTeamNum = this->itemAt(event->pos())->text(0).section(" ", 1).toInt();
            qDebug() << toTeamNum;
            student *temp = list->findStudentOnTeam(stuTeamNum, firstName, lastName);
            list->swapStudentTeamToTeam(temp, toTeamNum);
            QTreeWidget::dropEvent(event);

        }
        //if not a parent item in tree don't drop
        else if(this->itemAt(event->pos())->parent() == NULL)
        {
            QString name = students->currentItem()->text();
            qDebug() << name;
            //now split the name appart
            QStringList listName = name.split(" ");
            int itr = 0;
            //store first and last name in each
            QString test[2];
            foreach (QString str, listName)
            {
                test[itr] = str;
                itr++;
            }
            QString firstName = test[0];
            QString lastName = test[1];
            //find student in class and add them to a team.
            student *temp = list->findStudentByName(firstName, lastName);
            QString teamInt = this->itemAt(event->pos())->text(0).section(' ', 1);
            int teamNum = teamInt.toInt();
            list->addStudentToTeam(teamNum, temp, true);

            //remove student from class list
            QListWidgetItem *item = students->currentItem();
            int index = studentList->indexOf(item->text());
            studentList->removeAt(index);
            students->takeItem(students->row(item));
            QTreeWidget::dropEvent(event);
        }

    }
    else
        event->ignore();
}

void KTreeWidget::on_treeWidget_itemClicked(QTreeWidgetItem *item, int column)
{
    QString info;
    //if we clicked on a team name then display information
    if(item->parent() == NULL)
    {
        int teamNum = item->text(column).section(" ", 1).toInt();
        team *temp = findTeam(list->teams, teamNum);

        calcTeamSkills(temp);
        info = printPrefrences(temp);
    }
    else
    {
        QString firstName, lastName;
        text->clear();//clear the textBrowser to display new info
        //get the first and last names to search for
        firstName = item->text(column).section(" ", 0, 0);
        lastName = item->text(column).section(" ", 1);

        QString parent = item->parent()->text(0);
        qDebug() << parent;
        int teamNum = parent.section(" ", 1).toInt();
        qDebug()<< teamNum;

        student *temp = list->findStudentOnTeam(teamNum, firstName, lastName);
        qDebug() << temp->team;

        int id = temp->student_id;
        //Long Qstring to display information on selected student.
        info = firstName + " " + lastName + "\nID#: " + QString::number(id) + "\n\nRole Prefrences"
                + "\nManager: " + QString::number(temp->rolePreference[role_manager]) + "\nDocumenting: " + QString::number(temp->rolePreference[role_documenting])
                + "\nProgrammer: " + QString::number(temp->rolePreference[role_programmer]) + "\nTester: " + QString::number(temp->rolePreference[role_tester])
                +"\n\nSkills: " + "\nC: " + QString::number(temp->prgm_skills[0]) + "\nC++: " + QString::number(temp->prgm_skills[1]) + "\nJava: "
                + QString::number(temp->prgm_skills[2]) + "\nPython: " + QString::number(temp->prgm_skills[3]) + "\nAndroid: "
                + QString::number(temp->prgm_skills[4]) + "\nWeb Design: " + QString::number(temp->prgm_skills[5]) + "\nProblem Solving: "
                + QString::number(temp->skills[0]) + "\nAnalytic thinking: " + QString::number(temp->skills[1]) + "\nThinking up new ideas: "
                + QString::number(temp->skills[2]) + "\nPublic speaking: " + QString::number(temp->skills[3]) + "\nPlanning/organizing: "
                + QString::number(temp->skills[4]) + "\nGood with big picture: " + QString::number(temp->skills[5]) + "\nPowerpoint: "
                + QString::number(temp->skills[6]) + "\nTeam building: " + QString::number(temp->skills[7]) + "\nGood with details: "
                + QString::number(temp->skills[8]) + "\nSpoken English: " + QString::number(temp->skills[9])
                + "\nWritten English: " + QString::number(temp->skills[10]) + "\nTechnical Writing: " + QString::number(temp->skills[11])
            + "\nOutgoing(5)Shy(1): " + QString::number(temp->skills[12]);

    }
    text->clear();
    text->setText(info);
}

QString KTreeWidget::printPrefrences(team *t)
{
    QString info = "";
    QString roles[] = {"Manager: ", "Programmer: ", "Documenting: ", "Tester: "};

    for(int i = 0; i < numRoles; i++)
    {
        info.append(roles[i]);
        if(t->highestPreferences[i][0] > 2)
         {
            info.append(t->preferenceNames[i]);
            if(t->highestPreferences[i][1] > 2)
            {
                info.append(", ");
                info.append(t->preferenceNames2[i]);
            }
        }
        else
            info.append("Unfullfilled");
        info.append("\n");
    }
    return info;
}
