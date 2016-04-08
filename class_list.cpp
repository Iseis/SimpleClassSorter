/**
Eric Schultz
11-5-15
Joey McMahon
11-28-15
*/

///Class list implementation.

#include <iostream>
#include <stdlib.h>
#include <stdio.h>
#include "class_list.h"
#include "helper.h"
#include "team.h"

using namespace std;

///Constructor and Destructor for class objects
classList::classList(){}
classList::~classList(){}

/**
    This function takes in a student object and adds it to the linked list of students within a class list object.
    Students are added according to alphabetical order. This function uses the addStudent function to add the student.
    This function uses updateClassSkills to update the numBestSkills array in the class list, which keeps track of information
    regarding the class's programming skills. This function also updates the number of students in a class. If a student is
    loaded into a class list but is already there, the later one will replace the old one.
    st: the student to be added.
*/
void classList::addStudentToClass(student *st){
int a = (this->addStudent(&(this->head), st));
(this->numStudents) += a;
if (a == 1)
{
updateClassSkills(st, true);
}
}

/**
    Function for adding a student to the current linked list of students in a class list.
    Keeps linked list of students ordered by alphabet. If the user adds a student that has already been added,
    the student gets his/her information updated.
    *headNode: the head node of the class list
    *stud: the student to be added to the class list
    returns 1 if a new student was added.
    returns 0 otherwise.
*/
int classList::addStudent(student **headNode, student *st) {
    if ((*headNode) == NULL)
    {
        (*headNode) = st;

        return 1;
    }
   int comp = strCompare((*headNode)->lastName.toStdString(), st->lastName.toStdString());
    if (comp == 0) {
        comp = strCompare((*headNode)->firstName.toStdString(), st->firstName.toStdString());
    }
    if (comp > 0) // head node's name comes first. Add after.
    {
        if ((*headNode)->next == NULL) //if next is null, add it.
        {
            (*headNode)->next = st; //student added.
            st->previous = (*headNode);
            return 1;
        }
        else
        {
            return this->addStudent(&((*headNode)->next), st);
        }
    }
    else if (comp < 0)// (*headNode) comes second. Add student before it.
    {
        if ((*headNode) == this->head){ //replacing head
            student *temp = this->head; //save the head
            this->head = st;  //change head to be pointing at added student
            st->next = temp; //connect student to old head.
            temp->previous = st; //connect head to stud
            return 1;
        }
        else //insert before (*headNode)
        {
            student *temp = (*headNode)->previous;
            (*headNode)->previous = st;
            student *temp2 = (*headNode);
            temp->next = st; //connect previous node to stud
            st->next = temp2; //connect stud to rest of list
            st->previous = temp;

            return 1;
        }
    }
    else // Names are identical. User is probably updating student info.
    {
        st->next = (*headNode)->next; //replace node by using its pointers and then freeing it.
        st->previous = (*headNode)->previous;
        student *temp = (*headNode)->previous;
        student *temp2 = (*headNode)->next;
        if (temp != NULL)
        {
            temp->next = st;
        }
        if (temp2 != NULL)
        {
            temp2->previous = st;
        }
        if (this->head == (*headNode))
        {
            this->head = st;
        }
        freeStudent(&(*headNode));
        return 0;
    }
}


/**
    This function removes a student object from a class list and returns the student object.
    If the given student object was not found, it returns NULL.
    Helper function to swap functions and delete function.
    **headNode: the head node of the class linked list
    *st: the student to be removed.

*/
student *classList::removeStudentFromClass(student **headNode, student *st){
    if (*headNode == NULL)
    {
        return NULL;
    }
    if (st == NULL)
    {
        return NULL;
    }
    int comp = strCompare((*headNode)->lastName.toStdString(), st->lastName.toStdString());
    if (comp == 0) {
        comp = strCompare((*headNode)->firstName.toStdString(), st->firstName.toStdString());
    }
    if (comp > 0) // head node's name comes first. Search after.
    {
       return (this->removeStudentFromClass(&((*headNode)->next),st));
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
                this->head->previous = NULL;
                this->updateClassSkills(st, false);
                st->next = NULL;
                st->previous = NULL;
                return st;
            }
            else
            {
                this->updateClassSkills(st, false);
                this->head = NULL;
                st->next = NULL;
                st->previous = NULL;
                return st;
            }
        }
        else if ((*headNode)->next == NULL) //node is at end of list.
        {
            (*headNode)->previous->next = NULL;
            this->updateClassSkills(st, false);
            st->next = NULL;
            st->previous = NULL;
            return st;
        }
        else //node to be deleted is in between two nodes.
        {
            student *temp = ((*headNode)->previous);
            student *temp2 = ((*headNode)->next);
            temp->next = temp2;
            temp2->previous = temp;
            this->updateClassSkills(st, false);
            st->next = NULL;
            st->previous = NULL;
            return st;
        }
    }
    return NULL;
}

/**
    This function deletes the student st from the classList and frees the memory allocated to this student.
    This function uses removeStudentFromClass to remove the student object from the class list and then deletes
    the student.
    **st: the student to be deleted from the class list.
*/
void classList::deleteStudentFromClass(student **st){
if (*st == NULL)
{
    return;
}
(this->removeStudentFromClass(&(this->head), (*st)));
if (st != NULL)
{
freeStudent(st);
}
this->numStudents--;
}

/**
    Function for freeing all memory allocated to a class list.
    Frees student linked list using deleteStudentFromClass
    Frees all students in teams and all teams using freeTeams
    *cL: class list to be freed.
*/
void freeClassList(classList *cL){
if (cL == NULL)
{
    return;
}
while (cL->head != NULL)
{
student *oldHead = cL->head;
if (cL->head->next != NULL)
{
cL->head = cL->head->next;
}
cL->deleteStudentFromClass(&oldHead);

}
if (cL->teams != NULL)
{
    cL->freeTeams(&(cL->teams));
}
cL->teams = NULL;
cL->head = NULL;
free(cL);
}

/**
Not currently in use.
*/
void classList::freeStudentList(student **st){
if (*st == NULL)
{
    return;
}
student *temp = (*st)->next;
student *temp2 = (*st)->previous;
if (*st != NULL)
{
    this->deleteStudentFromClass(st);
}
if (temp != NULL)
{
    this->freeStudentList(&temp);
}
if (temp2 != NULL)
{
    this->freeStudentList(&temp2);
}
}

/**
Function for creating and initializing a new classList object.
className: name of the class
*/
classList *createClassList(QString className) {

    classList *newClass = new classList();
    // Initialize classList values
    newClass->className = className;
    newClass->numStudents = 0;
    newClass->numTeams = 0;
    newClass->head = NULL;
    newClass->teams = NULL;
    for (int i = 0; i < numProgSkills; i++)
    {
        newClass->numBestSkills[i] = 0;
    }
    return newClass;
}

/**
Prints all students names and all their information to the console.
Must be called on a class list object
*/
void classList::printClass(student *headNode){
    if (headNode == NULL)
    {
       // cout << "EMPTY CLASS";
        return;
    }
    if (headNode == this->head)
    {
    //cout << this->className << endl;
    }
    if (headNode != NULL)
    {
        headNode->printInfo();
        //cout << endl;
    }
    if (headNode->next != NULL)
    {
        this->printClass(headNode->next);
    }
    else
    {

    }
}

/**
Prints only student names of a class in order.
Must be called on a classList object.
*/
void classList::printClassNames(student *headNode){
    if (headNode == NULL)
    {
       // cout << "NO STUDENTS" << endl;
        return;
    }
    if (this->head == headNode)
    {
    //cout << this->className << endl;
    }
    if (headNode != NULL)
    {
       // cout << headNode->firstName << " " << headNode->lastName << endl;
    }
    if (headNode->next != NULL)
    {
        this->printClassNames(headNode->next);
    }
}

/**
This function removes the given student from the classList and adds the student
to the team with the given team number if that team exists and the student is in the
classList. This function uses findTeam to find the team based on the team number. It then uses
removeStudentFromClass and addToTeam to swap the student from the class list to the team. This
function also updates class skills so as to reflect the given student's skills not being available
to the class anymore. This helps with team forming. Once the student is added to a team, their team
number is adjusted to teamNumb. If updateTeamSkills is true, then the team's skills are updated.
teamNumb: the number representing the team
st: the student to be removed from the class list and added to the corresponding team
bool updateTeamSkills: true if we want to updateTeam skills.
*/
void classList::addStudentToTeam(int teamNumb, student *st, bool updateTeamSkills){
team *s = findTeam(this->teams,teamNumb);
if (s == NULL)
{
    return;
}
///NOTE: TEAM SIZE UPDATED IN addToTeam. DO NOT DO IT HERE.
student *stu = removeStudentFromClass(&(this->head), st);
if (stu == NULL)
{
    return;
}
s->addToTeam(&(s->head),stu); //add the stud object to the team.
st->team = teamNumb;
this->updateClassSkills(st, false); //their skills are no longer available to the class
if (updateTeamSkills)
{
    calcTeamSkills(s);
}
//updateTeamStats(s,st);
}

/**
Helper function for freeing all teams.
Frees all students on a team.
*/
void classList::freeTeam(team **t){
    if (*t == NULL)
    {
        return;
    }
    student *temp  = NULL;
    if ((*t)->head != NULL)
    {
    temp = (*t)->removeStudentFromTeam(&((*t)->head),(*t)->head);
    }
    while (temp != NULL)
    {
    freeStudent(&temp);
    this->numStudents--;
    //I believe removing the head first is the fastest way to delete all the stud objects.
    temp = (*t)->removeStudentFromTeam(&((*t)->head),(*t)->head);
    }
    (*t)->head = NULL;
    (*t)->next = NULL;
    (*t)->previous = NULL;
    (*t) = NULL;
    this->numTeams--;
    free(*t);
}


/**
Use only for end of program execution
Frees all teams in a linked list of teams, meaning it frees all students on all teams as well.
Something we need when the user wants to shut the app down.
s: the head pointer of the list of teams, all to be freed.
*/
void classList::freeTeams(team **s){
    while (*s != NULL)
    {
       team *a = *s; //save the team
       (*s) = (*s)->next; //move down the linked list.
       this->teams = (*s);
       freeTeam(&a); //free the old head node.
    }
    for (int i = 0; i < numProgSkills; i++)
    {
    this->numBestSkills[i] = 0;
    }
}

/**
May not use this function. Will be used if we want the user to be able to delete a team.
*/
void classList::removeTeamFromClass(team *tm){
     if (this->teams == NULL)
     {
         return;
     }
     if (tm == this->teams)
     {
        if (tm->next != NULL)
        {
            this->teams = tm->next;
            tm->previous = NULL;
            this->freeTeam(&tm);
        }
        else
        {
            freeTeam(&tm);
        }
     }
     else if (tm->previous != NULL)
     {
         team *temp = tm->previous;
         team *temp2 = tm->next;
         (temp->next) = temp2;
         if (tm->next != NULL)
         {
         temp2->previous = temp;
         }
         this->freeTeam(&tm);
     }
 }

/**
Adds a team to the current linked list of team objects in a class list.
Updates the number of teams.
Adds teams in order of their team number.
origin: head pointer of team linked list.
newTeam: new team to be added to list of teams.
*/
 void classList::addTeam(team **origin, team *newTeam){
  if (*origin == NULL)
  {
      (*origin) = newTeam;
      this->numTeams++;
      return;
  }
  else if (newTeam == NULL)
  {
      return;
  }
  else
  {
    if (newTeam->teamNumber > (*origin)->teamNumber)
    {
        if ((*origin)->next == NULL)
        {
            (*origin)->next = newTeam;
            newTeam->previous = (*origin);
            this->numTeams++;
        }
        else
        {
            this->addTeam(&((*origin)->next), newTeam);
            this->numTeams++;
        }

    }
    else if((*origin)->teamNumber > newTeam->teamNumber)
        {
            newTeam->next = (*origin);
            newTeam->previous = (*origin)->previous;
            if ((*origin)->previous != NULL)
            {
                (*origin)->previous->next = newTeam;
            }
            (*origin)->previous = newTeam;
            this->numTeams++;
        }
    else
    {
        return;
    }
  }
 }

 /**
 prints all team names and member names.
 */
 void classList::printTeams(team *headNode){
     if (this->teams == NULL)
     {
         //cout << "No teams currently available" << endl;
         return;
     }
     if (headNode == NULL)
     {
         return;
     }
     if (headNode != NULL)
     {
     //cout << "TeamName: " << headNode->teamName << endl;
     //cout << "TeamNumber: " << headNode->teamNumber << endl;
     //cout << "TeamSize: " << headNode->teamSize << endl;
     //cout << "Team Members:" << endl;
     }
     if (headNode->head == NULL)
     {
         //cout << "EMPTY TEAM" << endl;
     }
     else
     {
     headNode->printTeam(headNode->head);
     }
     if (headNode->next != NULL)
     {
     this->printTeams(headNode->next);
     }
 }

/**
Needed for sort function. Returns all students from all teams in a class list back to the class list.
Uses returnStudentsFromTeam to return students from one team at a time.
Also frees all teams in a classList.
head: head node of the team list in a class list object.
*/
void classList::returnAllStudentsFromTeams(team **head){
    while (*head != NULL)
    {
       team *a = *head; //save the team
       (*head) = (*head)->next; //move down the linked list.
       this->teams = (*head);
       returnStudentsFromTeam(&a);
    }
    this->numTeams = 0;
}

/**
Returns all students on a particular team back to the classList and then frees this team.
*/
void classList::returnStudentsFromTeam(team **tm){
    if ((*tm) == NULL)
    {
        return;
    }
    while ((*tm)->head != NULL)
    {
        student *st = (*tm)->removeStudentFromTeam(&((*tm)->head),(*tm)->head);
        st->team = no_team;
        this->addStudent(&(this->head),st);
        this->updateClassSkills(st,true);
    }
    freeTeam(tm);
}


/**
Returns all students from teams back to the class list using returnAllStudentsFromTeams, and then
sorts all students into numTeams teams, if numTeams is a valid number that makes it so there is at least
2 people on each team. Creates all teams and uses chooseStudents to form teams.
numTeams: the number of teams the user wants.
*/
int classList::sortTeams(int numTeams){
    if (numTeams <= 1)
    {
        return 0;
    }
    int studPerTeam = (this->numStudents)/numTeams;
    if (studPerTeam < 2)
    {
        return 1;
    }
    if (this->numTeams > 0) //Remove all students from all teams and remove all teams and start from scratch.
    {
        this->returnAllStudentsFromTeams(&(this->teams));
    }
    int leftOverStudents = this->numStudents - studPerTeam*numTeams;
    int studAdded = 0;
    QString teamS;
    for (int i = 0; i < numTeams; i++)
    {
    teamS = '0' + (char)i;
    teamS = "Team " + teamS;
    team *T = createTeam(teamS, i);
    this->addTeam(&(this->teams), T);
    int numChosen;
    numChosen = (this->chooseStudents(T,studPerTeam));
    if (leftOverStudents > 0)//This should always take care of all remaining students
                             //Because R(numStudents/numTeams) < numTeams always if numStudents > numTeams
    {
        student *S = this->mustGetStudent(this->head, T->progSkillOfChoice);
        this->addStudentToTeam(T->teamNumber,S,false);
        numChosen++;
        leftOverStudents--;
    }
    calcTeamSkills(T);
    studAdded += numChosen;
    }
    if (studAdded < this->numStudents)
    {
        return 2;
    }
    return 3;
}

/**
Chooses studPerTeam students to go on team tm.
Finds the most prevalent programming skill in the class list and chooses team members based on that. These
students are added to the team. This function finds students with the same best programming skill first and
then finds students with compatible programming skills if there are no matching results at first. This function
should always fill up a team with studPerTeam students.
This helps keep the sorting algorithm simple and should make reasonable enough teams to begin with, as
the customer wanted.
tm: team to add students to
studPerTeam: number of student to add to team tm.
Returns the number of students added.
*/
int classList::chooseStudents(team *tm, int studPerTeam){
int mostBestSkill = 0; //variable to store the program skill index
                        //with the most amount of people who are best at it.
int highestSkill = 0;
    for (int i = 0; i < numProgSkills; i++) //6 programming skills
    {
        if (this->numBestSkills[i] > highestSkill) //finding the max
        {
            highestSkill = this->numBestSkills[i];
            mostBestSkill = i;
        }
    }
    tm->progSkillOfChoice = mostBestSkill;
    int studentsChosen = 0;
    while (studentsChosen < studPerTeam)
    {
        student *st = findStudentBySkill(this->head, mostBestSkill, true);
        if (st == NULL)
        {
            st = findStudentBySkill(this->head, mostBestSkill, false);
            if (st == NULL) //still didn't find a student to add
            {
                st = mustGetStudent(this->head, mostBestSkill);
            }
        }
        this->addStudentToTeam(tm->teamNumber, st, false);
        studentsChosen++;
    }
    return studentsChosen;
}


/**
This function finds a student in a linked list of students based on a the given skill mostBestSkill. If onlyOneBest is true,
that means only students who have mostBestSkill as their only best skill will be selected. If onlyOneBest is false,
students who have mostBestSkill as one of their best skills will be chosen.
head: head of the linked list of students in a class list.
mostBestSkill: the most prevalent programming skill in the class list.
onlyOneBest: true if searching for students with one best prog skill. False otherwise.
Returns student that matches criteria.
*/
student *findStudentBySkill(student *head, int mostBestSkill, bool onlyOneBest){
    if (head != NULL)
    {
        if (onlyOneBest)
        {
            if (head->team == no_team) //student not on a team
            {
                if (head->bestProgramSkill == mostBestSkill) //choosing a student from the most prevalent program skill
                {
                    if (head->numBest == 1) //this program skill ranking is not matched by any other program skill
                    {
                        return head;
                    }
                }
            }
            else
            {
                return findStudentBySkill(head->next, mostBestSkill, onlyOneBest);
            }
        }
        else //not only one best skill
        {
            if (head->team == no_team) //student not on a team
            {
               // cout << "BESTSKILL: " << head->bestProgramSkill << " mostBestSkill: " << mostBestSkill << endl;
                if (head->bestProgramSkill == mostBestSkill) //choosing a student from the most prevalent program skill
                {
                    return head;
                }
                else
                {
                    return findStudentBySkill(head->next, mostBestSkill, onlyOneBest);
                }
            }
        }
    }
    else
    {
    return NULL;
    }
    return NULL;
}

/**
This function should always return a student unless head is null.
It uses findSkillMax to find the most qualified student in the list of students in a class list.
If score of found student is 0 for particular prog skill, student is searched again based on compatible
programming skills.
head: the head of the linked list of students in a class list.
mostBestSkill: the skill to be searched for.
*/
student *classList::mustGetStudent(student *head, int mostBestSkill){
    if (head == NULL)
    {
        return NULL;
    }
    student *st = this->findSkillMax(head, mostBestSkill, 0);
    if (st == NULL)
    {
        return NULL;
    }
    if (st->prgm_skills[mostBestSkill] == 0)
    {
        if (mostBestSkill == prog_c)
        {
            st = this->findSkillMax(st, prog_cpp, 0);
            if (st->prgm_skills[prog_cpp] == 0)
            {
                st = this->findSkillMax(st, prog_webDesign, 0);
            }
            return st;
        }
        else if (mostBestSkill == prog_cpp)
        {
            st = this->findSkillMax(st, prog_c, 0);
            if (st->prgm_skills[prog_c] == 0)
            {
                st = this->findSkillMax(st, prog_webDesign, 0);
            }
            return st;
        }
        else if (mostBestSkill == prog_java)
        {
            st = this->findSkillMax(st, prog_android, 0);
            return st;
        }
        else if (mostBestSkill == prog_python)
        {
            st = this->findSkillMax(st, prog_webDesign, 0);
            return st;
        }
        else if (mostBestSkill == prog_android)
        {
            st = this->findSkillMax(st, prog_java, 0);
            return st;
        }
        else if (mostBestSkill == prog_webDesign)
        {
            st = this->findSkillMax(st, prog_cpp, 0);
            if (st->prgm_skills[mostBestSkill] == 0)
            {
            st = this->findSkillMax(st, mostBestSkill, 0);
            }
            return st;
        }
        else
        {
        return st;
        }
    }
    else
    {
        return st;
    }
    return st;
}

/**
Find the student with the highest skill rating for a given programming skill out of the
entire list of students in st. Returns this student. If a student is found to have
a rating of 4 or higher for this skill, that student is immediately chosen.
st: the list of students to be searched.
skill: the skill to search for and maximize
topRating: the highest skill rating found so far.
*/
student *classList::findSkillMax(student *st, int skill, int topRating){
    if (st == NULL)
    {
        return NULL;
    }
    if (st->team == no_team) //student not on a team yet.
    {
        if ( (st->prgm_skills[skill]) > topRating)
        {
             if (st->prgm_skills[skill] >= 4)
             {
                 return st;
             }
             topRating = st->prgm_skills[skill];
         }
         if ( (st->prgm_skills[skill] <= topRating) || (st->prgm_skills[skill] < 4) )
         {
             if (st->next != NULL)
             {
                  student *st2 = findSkillMax(st->next, skill, topRating);
                  if (st2 != NULL)
                  {
                      if (st2->prgm_skills[skill] >= topRating)
                      {
                          return st2;
                      }
                  }
                  if (st2->prgm_skills[skill] > topRating)
                  {
                      topRating = st2->prgm_skills[skill];
                  }
             }
         }
         if (st->prgm_skills[skill] >= topRating)
         {
            return st;
         }
         else
         {
             return NULL;
         }
    }
    else
    {
        if (st->next != NULL)
        {
        return this->findSkillMax(st->next, skill, topRating);
        }
        else
        {
            return NULL;
        }
    }
}

/**
Updates a class list student skills according to whether or not that student is being added to a class list
or being removed.
st: student being removed or added.
add: true if student is being added. False otherwise.
*/
void classList::updateClassSkills(student *st, bool add){
    if (st == NULL)
    {
        return;
    }
    int bestSkill = st->bestProgramSkill;
    if (add)
    {
        for (int i = 0; i < numProgSkills; i++)
        {
            if (st->prgm_skills[i] == st->prgm_skills[bestSkill])
            {
                (this->numBestSkills[i])++; //keep track of all best skills.
            }
        }
    }
    else
    {
        for (int i = 0; i < numProgSkills; i++)
        {
            if (st->prgm_skills[i] == st->prgm_skills[bestSkill])
            {
                (this->numBestSkills[i])--; //keep track of all best skills.
            }
        }
    }
}

/**
If a student st is on a team, this function finds that student, removes the student from
their team and adds the student to the class list. Updates team skills. Updates class list programming skills
information. Important function for drag and drop; namely, for dragging students from teams back to the class list.
drag and drop.
*/
void classList::swapStudentTeamToClass(student *st){
    if (st == NULL)
    {
        return;
    }
    if (st->team == no_team)
    {
        return;
    }
    team *T = findTeam(this->teams, st->team);
    if (T == NULL)
    {
        return;
    }
    student *S = T->removeStudentFromTeam(&(T->head),st);
    if (S == NULL)
    {
        return;
    }
    calcTeamSkills(T);
    st->team = no_team;
    this->addStudentToClass(S);
    this->numStudents--; //I do this because addStudent adds one to numStudents but I don't want this
                         //because numStudents is only supposed to be adjusted when a student is deleted.
}

/**
If the given student is on a team, it deletes this student. Useful if a user wants to delete a student
but that student is currently on a team.
st: student to be removed.
*/
void classList::deleteStudentOnTeam(student *st){
    if (st == NULL)
    {
        return;
    }
    if (st->team == no_team)
    {
        return;
    }
    //Do not need to update class skills as they are already were when the student was added to the team.
    deleteStudentFromTeam((this->teams), st->team, st->lastName, st->firstName);
    this->numStudents--;
}

/**
Swaps a student from one team to another team with team int otherTeam.
Updates team skills for both teams. Also very important for drag and drop.
*/
 void classList::swapStudentTeamToTeam(student *st, int otherTeam){

     if (st == NULL)
     {
         return;
     }
     if (st->team == no_team)
     {
         return;
     }
     team *current = findTeam(this->teams, st->team);
     if (current == NULL)
     {
         return;
     }
     team *other = findTeam(this->teams, otherTeam);
     if (other == NULL)
     {
         return;
     }
     student *S = current->removeStudentFromTeam(&(current->head), st);
     if (S != NULL)
     {
        calcTeamSkills(current);
        other->addToTeam(&(other->head), S);
        calcTeamSkills(other);
     }
     else
     {
         return;
     }
 }


 student *classList::findStudentByName(QString nameFirst, QString nameLast)
 {
     int comp = strCompare((head->lastName.toStdString()), nameLast.toStdString());
     if (comp == 0){
     comp = strCompare(head->firstName.toStdString(), nameFirst.toStdString());
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
         return findStudent(head->next, nameFirst, nameLast);
     }
 }

 //Find student on team
 student *classList::findStudentOnTeam(int teamNum, QString firstName, QString lastName)
 {
     team *T= findTeam(this->teams, teamNum);
     if(T == NULL)
         return NULL;

     student *stud = findStudent(T->head, firstName, lastName);
     return stud;
 }
