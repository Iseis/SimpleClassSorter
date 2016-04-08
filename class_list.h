/**
Eric Schultz
11-5-2015
Joey McMahon
11-9-15
11-14-15
*/

#include <iostream>
#include <QString>
#include <QMessageBox>
#include "student.h"
#include "team.h"

#ifndef CLASS_LIST_H_INCLUDED
#define CLASS_LIST_H_INCLUDED

class classList{

    public:

    // Constructor and destructor
    classList();
    ~classList();

    QString className;
    int numStudents; // Number of students in list
    int numTeams; //number of teams.
    int numBestSkills[numProgSkills]; //for keeping track of the available best programming skills among remaining students that are
                      //not on teams.
    student* head; 	//Head of this linked list of students
    team *teams; //Head of this linked list of teams

    ///Functions: Named after their use. Not fully described here. See class_list.cpp for more information,
    void addStudentToClass(student *st); //add a student to a the linked list of students in the class list.
    int addStudent(student **headNode, student *st); //helper function for addStudentToClass
    student *removeStudentFromClass(student **headNode, student* st); //remove a student object for further use.
    void deleteStudentFromClass(student **st); //completely get rid of a student object.
    void freeStudentList(student **st); //get rid of an entire list of students using any student object in the list.
    void printClass(student *headNode); //prints all student names in order and their information.
    void printClassNames(student *headNode); //prints names of all students in class list in order.
    void addStudentToTeam(int teamNumb, student *st, bool updateTeamSkills); //adds a student to a particular team
    void freeTeams(team **s); //frees all teams in a linked list of teams, given the head pointer.
    void freeTeam(team **t); //frees a single team. Helper function for freeTeams.
    void removeTeamFromClass(team *tm); //deletes a team and its students from a class list.
    void addTeam(team **origin, team *newTeam); //adds a team to a linked list of teams.
    void printTeams(team *head); //prints team name, team size, and team members.
    int sortTeams(int numTeams); //sorts all students currently in class list (including students on teams) into teams.
    void swapStudentTeamToClass(student *st); //moves a student from a team back to the student list in the class list.
    void deleteStudentOnTeam(student *st); //completely removes a student that is on a team.
    void swapStudentTeamToTeam(student *st, int otherTeam); //moves a student from their current team to another one.
private:
    int chooseStudents(team *tm, int studPerTeam); //helper function for sort.
    void updateClassSkills(student *st, bool add); //updates numBestSkills
    student *findSkillMax(student *st, int skill, int topRating); //helper function for sort.
    student *mustGetStudent(student *head, int mostBestSkill); //helper function for sort.
    void returnAllStudentsFromTeams(team **head); //helper function for sort.
    void returnStudentsFromTeam(team **tm); //helper function for returnAllStudentsFromTeams.

public:
    student *findStudentByName(QString nameFirst, QString nameLast);
    student *findStudentOnTeam(int teamNum, QString firstName, QString lastName);
};
classList *createClassList(QString className);
void freeClassList(classList *cL);
student *findStudentBySkill(student *head, int mostBestSkill, bool onlyOneBest);
#endif // CLASS_LIST_H_INCLUDED
