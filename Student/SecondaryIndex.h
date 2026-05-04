#include <iostream>
#include <vector>
#include <cstring>
#include <algorithm>
using namespace std;
/*================ SECONDARY INDEX CLASS =================*/

class SecondaryIndex
{
private:
    char firstName[14];
    vector<int> ids;

public:
    SecondaryIndex()
    {
        firstName[0] = '\0';
    }

    SecondaryIndex(const char *name, int firstId)
    {
        strncpy(firstName, name, 13);
        firstName[13] = '\0';
        ids.push_back(firstId);
    }

    const char *getFirstName() const { return firstName; }
    vector<int> &getIds() { return ids; }

    bool operator<(const SecondaryIndex &s) const
    {
        return strcmp(firstName, s.firstName) < 0;
    }
};