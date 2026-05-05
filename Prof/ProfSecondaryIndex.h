#include <iostream>
#include <vector>
#include <cstring>
#include <algorithm>
using namespace std;
/*================ SECONDARY INDEX CLASS =================*/

class ProfSecondaryIndex
{
private:
    char firstName[14];
    vector<int> ids;

public:
    ProfSecondaryIndex()
    {
        firstName[0] = '\0';
    }

    ProfSecondaryIndex(const char *name, int firstId)
    {
        strncpy(firstName, name, 13);
        firstName[13] = '\0';
        ids.push_back(firstId);
    }

    const char *getFirstName() const { return firstName; }
    vector<int> &getIds() { return ids; }

    bool operator<(const ProfSecondaryIndex &s) const
    {
        return strcmp(firstName, s.firstName) < 0;
    }
};