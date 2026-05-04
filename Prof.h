#include <iostream>
#include <fstream>
#include <vector>
#include <algorithm>
#include <cstring>
using namespace std;



/*================ Prof CLASS =================*/

class Prof
{
private:
    int id, age, level;
    char firstName[14];
    char lastName[14];
    char city[20];
    float gpa;

    void solveGetLineProblem()
    {
        if (cin.peek() == '\n')
            cin.ignore(100, '\n');
    }

public:
    Prof() : id(0), gpa(0.0), age(0), level(0)
    {
        firstName[0] = lastName[0] = city[0] = '\0';
    }

    void readProf(bool update, class ProfSystem &system); // Important

    void displayProf() const
    {
        cout << "Prof id        : " << id << endl;
        cout << "Prof First Name: " << firstName << endl;
        cout << "Prof Last Name : " << lastName << endl;
        cout << "Prof City      : " << city << endl;
        cout << "Prof GPA       : " << gpa << endl;
        cout << "Prof Age       : " << age << endl;
        cout << "Prof Level     : " << level << endl;
    }

    short calcLength() const
    {
        // Fixed: Added string indicators (3 * 2 bytes) to the calculation
        short fixed = 3 * sizeof(int) + sizeof(float);
        short var = strlen(firstName) + strlen(lastName) + strlen(city);
        short indicators = 3 * sizeof(short);
        return fixed + var + indicators;
    }

    int getId() const { return id; }
    const char *getFirstName() const { return firstName; }
    const char *getLastName() const { return lastName; }
    const char *getCity() const { return city; }
    float getGpa() const { return gpa; }
    int getAge() const { return age; }
    int getLevel() const { return level; }

    void setId(int val) { id = val; }
    void setFirstName(const char *val)
    {
        strncpy(firstName, val, 13);
        firstName[13] = '\0';
    }
    void setLastName(const char *val)
    {
        strncpy(lastName, val, 13);
        lastName[13] = '\0';
    }
    void setCity(const char *val)
    {
        strncpy(city, val, 19);
        city[19] = '\0';
    }
    void setGpa(float val) { gpa = val; }
    void setAge(int val) { age = val; }
    void setLevel(int val) { level = val; }
};

