/**
Joey McMahon
11/25/15
*/

#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include "helper.h"
#include "team.h"

using namespace std;
///Constructor and Destructor for team objects
team::team(){}
team::~team(){}
/**
Creates a blank new team and initializes team size to zero, with pointer to next team at null.
teamName: desired team name. Should be system defined, not user.
teamNumber: Should be based off of teamName (i.e., team0's teamNumber is 0)
*/
team *createTeam(QString teamName, int teamNumber){
    team *newTeam = new team;
    newTeam->teamName = teamName;
    newTeam->teamNumber = teamNumber;
    newTeam->teamSize = 0;
    newTeam->head = NULL;
    newTeam->next = NULL;
    newTeam->progSkillOfChoice = 0;
    for (int i = 0; i < numRoles; i++)
    {
    newTeam->preferenceNames2[i] = "";
    newTeam->preferenceNames[i] = "";
    newTeam->highestPreferences[i][0] = 0;
    newTeam->highestPreferences[i][1] = 0;
    }
    return newTeam;
}

/**
Adds newStud to the linked list of students referenced by headNode.
headNode: pointer to the front of the linked list of student objects
newStud: pointer to new student node
Only use this function to add somebody to a team if they are not already on one.
*/
void team::addToTeam(student **headNode, student *newStud){
 if (*headNode == NULL)
 {
     (*headNode) = newStud;
     (this->teamSize)++;
     return;
 }
    int comp = strCompare((*headNode)->lastName.toStdString(), newStud->lastName.toStdString());
    if (comp == 0) {
        comp = strCompare((*headNode)->firstName.toStdString(), newStud->firstName.toStdString());
    }
    if (comp > 0) // head node's name comes first. Add after.
    {
        if ((*headNode)->next == NULL) //if next is null, add it.
        {
            (*headNode)->next = newStud;//student added.
            newStud->previous = (*headNode);
            (this->teamSize)++;
        }
        else
        {
            this->addToTeam(&((*headNode)->next), newStud);
        }
    }
    else if (comp < 0)// (*headNode) comes second. Add student before it.
    {
        if ((*headNode) == (this->head)){ //replacing head
            this->head->previous = newStud; //point head's previous to new node
            newStud->next = this->head;  //point new node to head
            this->head = newStud;           //update head node.
        }
        else //insert before (*headNode)
        {
            student *temp = (*headNode); //save (*headNode)
            (*headNode)->previous->next = newStud; //connect previous node to stud
            newStud->next = temp; //connect stud to rest of list
            newStud->previous = (*headNode)->previous;
            (*headNode)->previous = newStud;
        }
        (this->teamSize)++;
    }
    else // Names are identical. User probably updating somebody.
    {
        newStud->next = (*headNode)->next; //replace node by using its pointers and then freeing it.
        newStud->previous = (*headNode)->previous;
        student *temp = (*headNode)->previous;
        student *temp2 = (*headNode)->next;
        if (temp != NULL)
        {
            temp->next = newStud;
        }
        if (temp2 != NULL)
        {
            temp2->previous = newStud;
        }
        if (this->head == (*headNode))
        {
            this->head = newStud;
        }
        freeStudent(&(*headNode));
    }
}
/**
Removes a student object from a team using that team's linked list of students.
Thus headNode should always be the head variable found in each team.
headNode: the head of the linked list of student objects in a team object
studRem: the stud object to be removed from the list.
*/
student *team::removeStudentFromTeam(student **headNode, student *studRem){
    if ((*headNode) == NULL)
    {
        return NULL;
    }
    if (studRem == NULL)
    {
        return NULL;
    }
    int comp = strCompare((*headNode)->lastName.toStdString(), studRem->lastName.toStdString());
    if (comp == 0) {
        comp = strCompare((*headNode)->firstName.toStdString(), studRem->firstName.toStdString());
    }
    if (comp > 0) // head node's name comes first. Search after.
    {
       return  (this->removeStudentFromTeam(&((*headNode)->next),studRem));
       //return del;
    }
    else if (comp < 0)// (*headNode) comes second. But we already searched previous names.
    {
       return NULL;
    }
    else // Names are identical. Remove node
    {
        if (this->head == (*headNode))
        {
            if (((*headNode)->next) != NULL)
            {
                this->head = (*headNode)->next;
                (*headNode)->previous = NULL;
                this->teamSize--;
                studRem->previous = NULL;
                studRem->next = NULL;
                return studRem;
            }
            else
            {
                this->head = NULL;
                this->teamSize--;
                studRem->next = NULL;
                studRem->previous = NULL;
                return studRem;
            }
        }
        else if ((*headNode)->next == NULL) //node is at end of list.
        {
            student *temp = (*headNode)->previous;
            temp->next = NULL;
            this->teamSize--;
            studRem->next = NULL;
            studRem->previous = NULL;
            return studRem;
        }
        else //node to be deleted is in between two nodes.
        {
            student *temp = (*headNode)->previous;
            student *temp2 = (*headNode)->next;
            temp->next = temp2;
            temp2->previous = temp;
            (*headNode)->next = NULL;
            (*headNode)->previous = NULL;
            this->teamSize--;
            studRem->next = NULL;
            studRem->previous = NULL;
            return studRem;
        }
    }
}

/**
Finds a team based on team number. This is useful since complete student objects have this piece of information
in their team variable.
s: a list of teams to be searched
teamNumber: team identification number, found in all team objects and complete
student objects.
Returns pointer to the desired team if found; otherwise, returns NULL
*/
team *findTeam(team *s, int teamNumber){
    if (s == NULL)
    {
        return NULL;
    }
    else
    {
        if (s->teamNumber == teamNumber)
        {
            return s;
        }
        else
        {
            if (s->next != NULL)
            {
            return findTeam(s->next, teamNumber);
            }
            else //no more options.
            {
            return NULL;
            }
        }
    }
}

/**
HELPER FUNCTION TO deleteStudentOnTeam in classList.cpp
Completely removes a student from a team.
Should not use this function but the other one in class_list.cpp since that one
adjusts the class_list student size.
*/
void deleteStudentFromTeam(team *s, int teamNumber,QString lastName,QString firstName){
    team *T = findTeam(s, teamNumber);
    if (T == NULL)
    {
        return;
    }
    student *st = findStudent(T->head,firstName,lastName);
    if (st == NULL)
    {
        return;
    }
    student *S = T->removeStudentFromTeam(&(T->head), st);
    calcTeamSkills(T);
    if (S != NULL)
    {
    freeStudent(&S);
    }
}

/**
Helpful function for testing if team functionality is working properly.
Prints team name, number, size, and members.
headNode: head student node of a linked list of students.
*/
void team::printTeam(student *headNode){
if (this == NULL)
{
    return;
}
if (headNode == NULL)
{
    return;
}
//cout << headNode->firstName << " " << headNode->lastName << endl;
if (headNode->next != NULL)
{
this->printTeam(headNode->next);
}
}

/**
Important function for determining the highest preferences for each role and who on the team has that preference.
I only keep track of the two highest preferences. This may need to be adjusted, as their could be more than 2 people
who strongly prefer a role.
*/
void calcTeamSkills(team *tm){
    if (tm == NULL)
    {
        return;
    }
    if (tm->head == NULL)
    {
        for (int i = 0; i < numRoles; i++)
        {
        tm->highestPreferences[i][0] = 0;
        tm->highestPreferences[i][1] = 0;
        tm->preferenceNames[i] = "";
        tm->preferenceNames2[i] = "";
        }
        return;
    }
    else
    {
        for (int i = 0; i < numRoles; i++)
        {
        tm->highestPreferences[i][0] = 0;
        tm->highestPreferences[i][1] = 0;
        tm->preferenceNames[i] = "";
        tm->preferenceNames2[i] = "";
        }
        student *temp = tm->head;
        while (temp != NULL)
        {
            for (int i = 0; i < numRoles; i++)
            {
                if (temp->rolePreference[i] > tm->highestPreferences[i][0])
                {
                    tm->highestPreferences[i][1] = tm->highestPreferences[i][0];
                    tm->preferenceNames2[i] = tm->preferenceNames[i];
                    tm->highestPreferences[i][0] = temp->rolePreference[i];
                    tm->preferenceNames[i] = temp->firstName + " " + temp->lastName;
                }
                else if (temp->rolePreference[i] > tm->highestPreferences[i][1])
                {
                    tm->highestPreferences[i][1] = temp->rolePreference[i];
                    tm->preferenceNames2[i] = temp->firstName + " " + temp->lastName;
                }
            }
            temp = temp->next;
        }
    }
}


