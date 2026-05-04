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
    int id, age;
    char firstName[14];
    char lastName[14];
    char city[20];
    char courseName[20];
    float salary;

    void solveGetLineProblem()
    {
        if (cin.peek() == '\n')
            cin.ignore(100, '\n');
    }

public:
    Prof() : id(0), salary(0.0), age(0)
    {
        firstName[0] = lastName[0] = city[0] = courseName[0] = '\0';
    }

    void readProf(bool update, class ProfSystem &system); // Important

    void displayProf() const
    {
        cout << "Prof id           : " << id << endl;
        cout << "Prof First Name   : " << firstName << endl;
        cout << "Prof Last Name    : " << lastName << endl;
        cout << "Prof City         : " << city << endl;
        cout << "Prof salary       : " << salary << endl;
        cout << "Prof Age          : " << age << endl;
        cout << "Prof Course Name  : " << courseName << endl;
    }

    short calcLength() const
    {
        // Fixed: Added string indicators (3 * 2 bytes) to the calculation
        short fixed = 2 * sizeof(int) + sizeof(float);
        short var = strlen(firstName) + strlen(lastName) + strlen(city) + strlen(courseName);
        short indicators = 4 * sizeof(short);
        return fixed + var + indicators;
    }

    int getId() const { return id; }
    const char *getFirstName() const { return firstName; }
    const char *getLastName() const { return lastName; }
    const char *getCity() const { return city; }
    float getSalary() const { return salary; }
    int getAge() const { return age; }
    const char *getCourseName() const { return courseName; }

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
    void setSalary(float val) { salary = val; }
    void setAge(int val) { age = val; }

    void setCourseName(const char *val)
    {
        strncpy(courseName, val, 19);
        courseName[19] = '\0';
    }
};
