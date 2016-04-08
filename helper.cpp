/**
Joey McMahon
11/14/15
*/
#include "helper.h"
using namespace std;


///returns true if a character is upper case.
bool isUpperCase(char c)
{
    if (c >= 'A' && c <= 'Z')
    {
        return true;
    }
    else
    {
        return false;
    }
}

///takes a string a makes all of its characters lower case
string toLowerCase(const string toLow)
{
string low = toLow;
int j = low.size();
int i;
for (i = 0; i < j; i++)
    {
    if (isUpperCase(low[i]))
    {
    low[i] += 'a' - 'A';
    }
    }
    return low;
}

/**returns 0 if they are the same
returns > 0 if one comes alphabetically first.
returns < 0 if two come alphabetically first. */
int strCompare(const string one,const string two)
{
    string oneC = toLowerCase(one);
    string twoC = toLowerCase(two);
    bool decided = false;
    int sizeOne = one.size();
    int sizeTwo = two.size();
    //std::cout << one << " \'" << two << "\'" << std::endl;
    //std::cout << sizeOne << " " << sizeTwo << std::endl;
    int currChar = 0;
    bool oneGreater = false;
    bool twoGreater = false;
    while ( ((currChar < sizeOne) && (currChar < sizeTwo) && (!decided)) )
    {
        if (oneC[currChar] > twoC[currChar])
        {
            oneGreater = true;
        }
        else if (twoC[currChar] > oneC[currChar])
        {
            twoGreater = true;
        }
        if (oneGreater || twoGreater)
        {
            decided = true;
        }
        else
        {
            currChar++;
        }
    }
    if (decided && oneGreater)
    {
        return -1;
    }
    else if (decided && twoGreater)
    {
        return 1;
    }
    //not decided
    else
    {
        //std::cout << sizeOne << " " << sizeTwo << std::endl;
        //if all characters present are matching but oneC is shorter, then it comes first.
        if (sizeOne < sizeTwo)
        {
            return 1;

        }
        else if (sizeOne > sizeTwo)
        {
            return -1;
        }
        //they are equal
        else
        {
            return 0;
        }
    }
}
