/** Eric Schultz
    11/8/15
    Joey McMahon
    11/28/15
    Student class and functions header
*/

#include <iostream>
#include <QString>
#ifndef STUDENT_HPP_INCLUDED
#define STUDENT_HPP_INCLUDED

/**
Student Class Definition

Most importantly, each class object of type student contains student object variables for the next student
in a list and the previous student in a list. These variables are called next and previous. Thus a doubly
linked list is implemented for student objects. This class implements four simple though useful functions
for student objects. The print function prints out all student information which was very useful for testing
purposes. The find function finds a particular student in any linked list, useful for GUI implementation
if one needs to retrieve a particular student's information. The create function makes a new student object
and initialized all of its variables, which is obviously useful and necessary. The free function frees memory
allocated to a student object.

This class lends the most modifiability to the code. It accomplishes this with predefined constants
that can be easily changed. If one wanted to include more programming skills, one would simply add
one to the integer that is currently being used (6), and then add a corresponding program skill
variable with the next value of the array (6), as seen in the predefined programming skill variables
(prog_skillName). Removing skills would work in the opposite way. If the program is ever referencing
the number of programming skills, the number of personal skills, or the number of roles, the program
uses these corresponding variables so that when they are changed, code does not need to be added or
modified. All programming skills, personal skills, and roles have variables and integer values assigned
to them so that values can be correctly stored and used. Programming skills have a "prog" prefix;
personal skills have an "sk" prefix; and roles have a "role" prefix.

The student class also has variables for other meaningful data such as a QString for storing professional
experience, and a QString for any other programming language that the student is good at. The bestProgrammingSkill
variable stores the integer that references the student's best programming language. The numBest variable, tells
how many programming skills the student has a highest rating for. This is useful for the sorting algorithm since
it searches first based on which students have only one best skill, and then searches for those who have more than
one best skill. The team variable stores the integer reference for which team the student is on. If the student is on
no team, the value is -1 which is stored in the variable no_team for ease of use and greater modifiability.
*/

#define no_team -1
#define numProgSkills 6
#define numSkills 13
#define numRoles 4

///These are for accessing the correct element in the student object arrays prgm_skills, skills, and rolePreference
//May be helpful for GUI implementation.
//Program Skills
#define prog_c 0
#define prog_cpp 1 //c++
#define prog_java 2
#define prog_python 3
#define prog_android 4
#define prog_webDesign 5
//Skills
#define sk_probSolv 0
#define sk_analytic 1
#define sk_newIdeas 2
#define sk_publicSpeak 3
#define sk_planOrganize 4
#define sk_bigPicture 5
#define sk_powerPoint 6
#define sk_teamBuild 7
#define sk_goodWithDetails 8
#define sk_spokenEnglish 9
#define sk_writtenEnglish 10
#define sk_techWriting 11
#define sk_outgoingShy 12
//Role Preferences
#define role_manager 0
#define role_programmer 1
#define role_documenting 2
#define role_tester 3


using namespace std;
/* Implementation of student class

We are using a class instead of a struct for the class list because of the relatively large amount of information stored in each student object.
Most important fields included thus far.
Each student has a pointer to the next student. Thus we are using a linked list.
We are using a struct for the team lists because we will only need student names and linkers in this data structure.
*/


using namespace std;
class student {
    public:
    //constructor and destructor.
    //necessary for class. Don't really use these.
    student();
    ~student();
    // Basic information
    QString lastName;
    QString firstName;
    QString wholeName;
    int student_id;
    //QString email;
    int bestProgramSkill;
    int numBest;
    int rolePreference[numRoles]; //roles preferences numbered 1-4. 4 being most preferred, 1 being least.
    //0 = manager, 1 = programmer, 2 = documenting, 3 = testing
    int prgm_skills[numProgSkills];
    // skills[0] indicates proficiency in c, skills[1] in c++, skills[2] in java, 3 = python, 4 = android, 5 = web design
    int skills[numSkills];
    /*
    skills[i]: 0: problem solving, 1: Analytic thinking, 2: Thinking up new ideas, 3: Public speaking, 4: Planning/organizing,
    5: good with big picture, 6: Powerpoint, 7: team building, 8: good with details, 9: spoken English, 10: Written English,
    11: Technical Writing, 12: Outgoing (5), shy (1)
    Rating of zero, means no skill, 5 means high proficiency.
    */

    QString other_prgm;
    // Extra space for students to write in other programming languages they know

    QString prof_exp;
    // Space for students to describe any professional programming experience they may have

    //char avg_grade[2];
    // The student's average grade in CIS classes (I figure this is better than asking for grades they got in each class?)

    int team;
    // Indicates what team this student is on, if any

    student *next;
    student *previous;
    // Pointer to next student in list/team (TODO: studentList or team??)

    void printInfo();
};

// Student class helper functions:
///Function for creating complete student object with all required fields.
student *createStudent(QString lastname, QString firstname, int id, int prgmSkills[numProgSkills], int skills[numSkills], int rolePreference[numRoles], QString othrPrgmLg);
///Function for creating simpler student object for teams.
student *findStudent(student *head, QString firstName, QString lastName);
void freeStudent(student **st);
#endif // STUDENT_HPP_INCLUDED
