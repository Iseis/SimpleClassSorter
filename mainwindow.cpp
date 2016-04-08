#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    setWindowTitle("Class Sorting Hat");
    ui->classListWidget->setDragEnabled(true); //tell the listWidget to allow dragging
    ui->amountOfTeams->setValue(2);
    list = createClassList("Class");
    teamCount = 0;
    treeWidget = new KTreeWidget(parent, list, ui->classListWidget, ui->selectedStudTextBrowser, &studentList);
    //Connect signal to the custom slot.
    treeWidget->connect(treeWidget, SIGNAL(itemClicked(QTreeWidgetItem*,int)), treeWidget, SLOT(on_treeWidget_itemClicked(QTreeWidgetItem*,int)));
    ui->gridLayout->addWidget(treeWidget, 1, 1);
}

MainWindow::~MainWindow()
{
    delete ui;
}

//opens new dialog box to find student files
//can take multiple files to expedite student input.
void MainWindow::on_addStudentsButton_clicked()
{
    //Set reading in multiple files to build the student list
    QStringList files;  //List of files
    QFileDialog fileDialog(this); //Create the dialog for getting the files
    fileDialog.setDirectory(QDir::homePath());
    fileDialog.setFileMode(QFileDialog::ExistingFiles);

    if(fileDialog.exec())
        files = fileDialog.selectedFiles();

    if(!files.isEmpty())
    {
        student *temp;
        int filesRead = 1; //used to send file read error information
        //read in the students information
        foreach (QString str, files)
        {
           temp = parseStudentFile(str);
           //the parsing of the student info goes bad send an error message
           if(temp != NULL)
           {
                studentList += temp->firstName + " " +temp->lastName;
                list->addStudentToClass(temp);//add the student to the class.
           }
           else
               QMessageBox::information(this, "Error reading student file.", "Could parse student file number: " + filesRead);
           filesRead++;
        }
    }
    ui->classListWidget->clear();
    ui->classListWidget->addItems(studentList);
}

//Saves the teams and there formation to a file for ease of access or posting for instructor
void MainWindow::on_saveTeamButton_clicked()
{
    QString file = QFileDialog::getSaveFileName(this,"Save Teams to a file.");
    if(!file.isEmpty())
    {
        int teams = list->numTeams;
        team *nTeam = list->teams;
        QFile sFile(file);
        if(sFile.open(QFile::WriteOnly | QFile::Text))
        {
            QTextStream out(&sFile);
            for(int i =0; i < teams; i++)
            {
                out << nTeam->teamName + "\n";
                student *temp = nTeam->head;
                for(int j = 0; j < nTeam->teamSize; j++)
                {
                   out << temp->wholeName + "\n";
                   temp = temp->next;
                }
                 out << "\n\n";
                nTeam = nTeam->next;
            }
            sFile.flush();
            sFile.close();
        }
    }
}


//Creates a blank team with no students entered used for drag and drop
void MainWindow::on_createBlankTeam_clicked()
{
    addRoot("Team " + QString::number(teamCount, 10));
    teamCount++;

}

// Creates and adds a new team
QTreeWidgetItem *MainWindow::addRoot(QString name)
{
    team *temp = createTeam(name, teamCount);
    list->addTeam(&(list->teams), temp);
    QTreeWidgetItem *itm = new QTreeWidgetItem(treeWidget);
    itm->setText(0, name);
    treeWidget->addTopLevelItem(itm);
    return itm;

}

//Adds a child to a team
// used when sorting team members using the sort button
void MainWindow::addChild(QTreeWidgetItem *parent, team *tm)
{
    int numMembers = tm->teamSize;
    student *std = tm->head;
    for(int i = 0; i < numMembers; i++)
    {
        QTreeWidgetItem *itm = new QTreeWidgetItem();
        itm->setText(0, std->wholeName);
        parent->addChild(itm);
        std = std->next;
    }
}

//Sorts the class into teams
void MainWindow::on_sortButton_clicked()
{
    //Disable the create blank team button
    ui->createBlankTeam->setEnabled(false);

    //clear widget
    treeWidget->clear();


    int error = list->sortTeams(ui->amountOfTeams->value());
    // sortTeams returns an error depending on the error display a message
    if(error == 0)
    {
        QMessageBox::information(this, "Need more Teams!" ,"Must have more than one team.");
        return;
    }
    else if(error == 1)
    {
        QMessageBox::information(this, "More Students Needed.", "Each team must have at least 2 people on it.");
        return;
    }
    else if(error == 2)
    {
        QMessageBox::information(this, "Not all Students added.", "Not all students added to team.");
        return;
    }

    int teams = list->numTeams;
    team *nTeam = list->teams;

    for(int i =0; i < teams; i++)
    {
        //if we have already sorted team then no need to add new root
        //QTreeWidgetItem *itm = addRoot("Team " + QString::number(i));

        QTreeWidgetItem *itm = new QTreeWidgetItem(treeWidget);
        itm->setText(0, "Team " + QString::number(i));
        treeWidget->addTopLevelItem(itm);
        addChild(itm, nTeam);

        nTeam = nTeam->next;
    }

    //clear all the diplay fields
    studentList.clear();
    ui->classListWidget->clear();
    ui->selectedStudTextBrowser->clear();
}

//used to parse the students file
//takes the file name to be parsed
student *MainWindow::parseStudentFile(QString file)
{
    //create file
    QFile sFile(file);
    //open it
    if(sFile.open(QFile::ReadOnly | QFile::Text))
    {
        int prgm_skills[numProgSkills];//used to store incoming student prgm skills
        int skills[numSkills];//other student skills
        int pref[numRoles];
        QString firstName, lastName, other;
        QStringList input; //used for splitting the strings
        int stuID; //students id#

        QTextStream in(&sFile); //used for getting the lines of the file
        //skip first to lines not important
        in.readLine();
        in.readLine();

        QString tempName = in.readLine().section(':', 1); //used to get full name of student

        //now split the name appart
        QStringList listName = tempName.split(" ");
        int itr = 0;
        //store first and last name in each
        QString test[2];
        foreach (QString str, listName)
        {
            test[itr] = str;
            itr++;
        }
        //Store values
        firstName = test[0];
        lastName = test[1];
        qDebug() << firstName + " " + lastName;

        //get the id number
        stuID = in.readLine().section(':', 1).toInt();

        //skip next 2 lines
        in.readLine();
        in.readLine();

        //start getting skills
        prgm_skills[0] = in.readLine().section(':',1).toInt();
        prgm_skills[1] = in.readLine().section(':',1).toInt();
        prgm_skills[2] = in.readLine().section(':',1).toInt();
        prgm_skills[3] = in.readLine().section(':',1).toInt();
        prgm_skills[4] = in.readLine().section(':',1).toInt();
        prgm_skills[5] = in.readLine().section(':',1).toInt();
        skills[0] = in.readLine().section(':',1).toInt();
        skills[1] = in.readLine().section(':',1).toInt();
        skills[2] = in.readLine().section(':',1).toInt();
        skills[3] = in.readLine().section(':',1).toInt();
        skills[4] = in.readLine().section(':',1).toInt();
        skills[5] = in.readLine().section(':',1).toInt();
        skills[6] = in.readLine().section(':',1).toInt();
        skills[7] = in.readLine().section(':',1).toInt();
        skills[8] = in.readLine().section(':',1).toInt();
        skills[9] = in.readLine().section(':',1).toInt();
        skills[10] = in.readLine().section(':',1).toInt();
        skills[11] = in.readLine().section(':',1).toInt();
        skills[12] = in.readLine().section(':',1).toInt();

        //skip next 2 lines
        in.readLine();
        in.readLine();

        //read in student role prefrences
        pref[role_manager] = in.readLine().section(':',1).toInt();
        pref[role_programmer] = in.readLine().section(':',1).toInt();
        pref[role_documenting] = in.readLine().section(':',1).toInt();
        pref[role_tester] = in.readLine().section(':',1).toInt();

        in.readLine();
        other = in.readLine().section(':',1);

        return createStudent(lastName, firstName, stuID, prgm_skills, skills, pref, other);
    }
    return NULL;
}


//If user clicks on  a item in the class widget display the information
void MainWindow::on_classListWidget_itemClicked(QListWidgetItem *item)
{
    QString firstName, lastName;
    ui->selectedStudTextBrowser->clear();//clear the textBrowser to display new info
    //get the first and last names to search for
    firstName = item->text().section(" ", 0, 0);
    lastName = item->text().section(" ", 1);

    student *temp = list->findStudentByName(firstName, lastName);
    int id = temp->student_id;
    //long Qstring to display information on selected student.
    QString info = firstName + " " + lastName + "\nID#: " + QString::number(id) + "\n\nRole Prefrences"
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

    ui->selectedStudTextBrowser->setText(info);

}



//opens a new app
void MainWindow::on_actionNew_triggered()
{
    MainWindow *temp = new MainWindow();
    temp->show();
}

//removes student from the class list or the team list
void MainWindow::on_deleteStudenButton_clicked()
{
    QListWidgetItem *listItem = ui->classListWidget->currentItem();
    QTreeWidgetItem *treeItem = treeWidget->currentItem();

    if(list->head != NULL)
        if(list->teams != NULL)
        {
            {
                if(listItem != NULL)
                {
                    QString stuName = listItem->text();

                    //remove from list
                    ui->classListWidget->takeItem(ui->classListWidget->row(listItem));

                    //get first and last name
                    QString first = stuName.section(" ", 0,0);
                    QString last = stuName.section(" ", 1);

                    //now remove the student
                    student *temp = list->findStudentByName(first, last);
                    list->deleteStudentFromClass(&temp);

                    ui->selectedStudTextBrowser->clear();
                    studentList.removeOne(stuName);
                }
                else
                {
                    if(treeItem->parent() != NULL)
                    {
                        QString stuName = treeItem->text(0);
                        //get first and last name
                        QString first = stuName.section(" ", 0,0);
                        QString last = stuName.section(" ", 1);

                        int teamNum = treeItem->parent()->text(0).section(" ", 1).toInt();

                        student *temp = list->findStudentOnTeam(teamNum, first, last);
                        list->deleteStudentOnTeam(temp);

                        //now remove the item from the widget
                        delete treeItem;
                        ui->selectedStudTextBrowser->clear();
                    }
                }
            }
    }
}
