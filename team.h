/**
Joey McMahon
11/14/15

Team objects are implemented as a linked list as well. Within each one is a linked list of stud objects that only
include each student's name. This is so that we do not need to create two complete student objects for our program.
This is necessary because one student object is not enough if the students are in two linked lists; pointers would get
mixed up. This also has the benefit of not having to update student info if a student's information is changed in the class list,
so long as it does not involve a change to the student's name.
*/
#include <iostream>
#include <QString>
#include "student.h"
#ifndef TEAM_H_INCLUDED
#define TEAM_H_INCLUDED

class team{
//constructor and destructor
    //constructor and destructor
    public:
    team();
    ~team();
    //Team analysis variables
    int progSkillOfChoice;
    int highestPreferences[numRoles][2]; //two highest role preferences
    QString preferenceNames[numRoles];
    QString preferenceNames2[numRoles];
    int teamSize;
    int teamNumber;
    QString teamName;
    student *head;
    team *next; //pointer to next team object.
    team *previous; //pointer to the previous team object.
    void addToTeam(student **headNode, student *newStud);
    student *removeStudentFromTeam(student **head, student *st);
    void printTeam(student *headNode);
    };
    team *createTeam(QString teamName, int teamNumber);
    team *findTeam(team *s, int teamNumber);
    void deleteStudentFromTeam(team *s, int teamNumber, QString lastName, QString firstName);
    void calcTeamSkills(team *tm);
#endif // TEAM_H_INCLUDED
