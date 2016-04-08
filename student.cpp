/** Eric Schultz
 11-8-15
 Joey McMahon
 11/28/15
 Student implementation
 */

#include <stdlib.h>
#include <stdio.h>
#include <iostream>
#include "student.h"
#include "helper.h"

using namespace std;

///Constructor and Destructor for student objects
student::student(){}
student::~student(){}

/**
prints all of a student's information.
*/
void student::printInfo() {
    /*cout << this->lastName << ", " << this->firstName << endl;
    cout << this->student_id << endl;
    cout << "Student programming skills:\n" << "C: " << this->prgm_skills[0] << "  C++: " << this->prgm_skills[1]
         << "  Java: " << this->prgm_skills[2] << "  Python: " << this->prgm_skills[3] << "  Android: "
         << this->prgm_skills[4] << " Web Design: " << this->prgm_skills[5] << endl;
    if((this->other_prgm).length() > 0) cout << "Other student programming languages: " << this->other_prgm << endl;
    if((this->prof_exp).length() > 0) cout << "Student's professional experience: " << this->prof_exp << endl;
    cout << "Skills: ";
    for (int i = 0; i < 13; i++)
    {
        cout << this->skills[i] << " ";
    }
    cout << endl;
    //cout << "Student's average grade in CIS classes: " << this->avg_grade << "\n";
    if(team != -1) cout << "Student is on team " << this->team << "\n";*/
}

/**
Creates a student object, initializes it, and returns it.
lastname: student's last name
firstname: student's first name
id: student id
prgmSkills[5]: an array of integer rated program language skills formatted in the correct order. See student.hpp
int skills[13]; an array of integer rated personal skills formatted in the correct order. See student.hpp
int rolePreferences[numRoles]; an array of integer rated role preferences formatted in the correct order. See student.hpp
othrPrgmLg: other programming language the student may have familiarity with.
*/
student *createStudent(QString lastname, QString firstname, int id, int prgmSkills[numProgSkills], int skills[numSkills],int rolePreference[numRoles], QString othrPrgmLg){

    student *s = new student;
    s->lastName = lastname;
    s->firstName = firstname;
    s->wholeName = firstname + " " + lastname;
    s->student_id = id;
    s->bestProgramSkill = 0;
    int highestRating = 0;
    for (int i = 0; i < numProgSkills; i++){
        s->prgm_skills[i] = prgmSkills[i];
        if (prgmSkills[i] > highestRating)
        {
            highestRating = prgmSkills[i];
            s->bestProgramSkill = i;
            s->numBest = 1;
        }
        if (prgmSkills[i] == highestRating)
        {
            s->numBest++;
        }
    }
    for (int j = 0; j < numSkills; j++){
        s->skills[j] = skills[j];
    }
    for (int k = 0; k < numRoles; k++)
    {
        s->rolePreference[k] = rolePreference[k];
    }
    s->team = no_team; //currently not on a team.
    s->other_prgm = othrPrgmLg;
    s->next = NULL;
    s->previous = NULL;
    return s;
}

 /**
 This function finds a student based on name in a linked list of students and returns the student object
 if it is found.
 Returns NULL otherwise.
 head: pointer to head of the linked list of students.
 firstName/lastName: name of desired student to be found.
 */
 student *findStudent(student *head, QString firstName, QString lastName){
     int comp = strCompare((head->lastName).toStdString(), lastName.toStdString());
     if (comp == 0){
     comp = strCompare(head->firstName.toStdString(), firstName.toStdString());
     }
     if (comp == 0)
     {
         return head;
     }
     else if (comp < 0) //student's name comes alphabetically first but we have already checked previous names
     {
         return NULL;
     }
     else //comp > 0
     {
         return findStudent(head->next,firstName,lastName);
     }
}




/**
frees a student object from memory
st: double pointer to student object to be deleted.
*/
void freeStudent(student **st){
(*st)->next = NULL;
(*st)->previous = NULL;
(*st) = NULL;
free((*st));
free(st);
}
