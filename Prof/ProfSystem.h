#include <iostream>
#include <fstream>
#include <vector>
#include <algorithm>
#include <cstring>
using namespace std;

#include "ProfPrimaryIndex.h"
#include "ProfSecondaryIndex.h"
#include "ProfAvailNode.h"
#include "Prof.h"

/*================ Prof SYSTEM CLASS =================*/

class ProfSystem
{
private:
    vector<PrimaryIndex> primaryIndexList;
    vector<SecondaryIndex> secondaryIndexList;
    vector<AvailNode> availList;

public:
    ProfSystem() {}

    /*================ AVAIL =========*/

    int getFreeSpace(short needed)
    {
        for (int i = 0; i < (int)availList.size(); i++)
        {
            if (availList[i].size >= needed)
            {
                int offset = availList[i].offset;
                availList.erase(availList.begin() + i);
                return offset;
            }
        }
        return -1;
    }

    void readAvailFile()
    {
        ifstream in("Prof/ProfAvailList.txt", ios::binary);
        if (!in)
            return;
        AvailNode a;
        while (in.read((char *)&a, sizeof(a)))
            availList.push_back(a);
        in.close();
    }

    void writeAvailFile()
    {
        ofstream out("Prof/ProfAvailList.txt", ios::binary | ios::trunc);
        for (const auto &a : availList)
            out.write((char *)&a, sizeof(a));
        out.close();
    }

    /*================ Primary Index =================*/

    void writePrimaryIndexFile()
    {
        ofstream out("Prof/ProfPrimaryIndex.txt", ios::binary | ios::trunc);
        for (const auto &pi : primaryIndexList)
            out.write((char *)&pi, sizeof(pi));
        cout << "Primary Index saved successfully.\n";
        out.close();
    }

    void readPrimaryIndexFile()
    {
        ifstream in("Prof/ProfPrimaryIndex.txt", ios::binary);
        if (!in)
            return;
        PrimaryIndex pi;
        while (in.read((char *)&pi, sizeof(pi)))
            primaryIndexList.push_back(pi);
        cout << "Primary Index loaded successfully.\n";
        in.close();
    }

    void addToPrimaryIndexList(PrimaryIndex pi)
    {
        primaryIndexList.push_back(pi);
        sort(primaryIndexList.begin(), primaryIndexList.end());
    }

    void deleteFromPrimaryIndexList(int id)
    {
        for (int i = 0; i < primaryIndexList.size(); i++)
        {
            if (primaryIndexList[i].getId() == id)
            {
                primaryIndexList.erase(primaryIndexList.begin() + i);
                return;
            }
        }
    }

    int searchInPrimaryIndexList(int id)
    {
        int low = 0, high = (int)primaryIndexList.size() - 1;
        while (low <= high)
        {
            int mid = (low + high) / 2;
            if (primaryIndexList[mid].getId() == id)
                return primaryIndexList[mid].getOffset();
            if (primaryIndexList[mid].getId() < id)
                low = mid + 1;
            else
                high = mid - 1;
        }
        return -1;
    }

    /*================ Secondary Index =================*/

    void writeSecondaryIndexFile()
    {
        ofstream out("Prof/ProfSecondaryIndex.txt", ios::binary | ios::trunc);
        int size = secondaryIndexList.size();
        out.write((char *)&size, sizeof(size));
        for (auto &si : secondaryIndexList)
        {
            out.write(si.getFirstName(), 14);
            int idCount = si.getIds().size();
            out.write((char *)&idCount, sizeof(idCount));
            for (int id : si.getIds())
                out.write((char *)&id, sizeof(id));
        }
        cout << "Secondary Index saved successfully.\n";
        out.close();
    }

    void readSecondaryIndexFile()
    {
        ifstream in("Prof/ProfSecondaryIndex.txt", ios::binary);
        if (!in)
            return;
        secondaryIndexList.clear();
        int size;
        if (!in.read((char *)&size, sizeof(size)))
            return;
        for (int i = 0; i < size; i++)
        {
            char name[14];
            in.read(name, 14);
            int idCount;
            in.read((char *)&idCount, sizeof(idCount));
            int firstId;
            in.read((char *)&firstId, sizeof(firstId));
            SecondaryIndex si(name, firstId);
            for (int j = 1; j < idCount; j++)
            {
                int nextId;
                in.read((char *)&nextId, sizeof(nextId));
                si.getIds().push_back(nextId);
            }
            secondaryIndexList.push_back(si);
        }
        cout << "Secondary Index loaded successfully.\n";
        in.close();
    }

    void addToSecondaryIndexList(const char *name, int id)
    {
        for (auto &si : secondaryIndexList)
        {
            if (strcmp(si.getFirstName(), name) == 0)
            {
                si.getIds().push_back(id);
                return;
            }
        }
        secondaryIndexList.push_back(SecondaryIndex(name, id));
        sort(secondaryIndexList.begin(), secondaryIndexList.end());
    }

    void deleteFromSecondaryIndexList(const char name[], int id)
    {
        for (int i = 0; i < secondaryIndexList.size(); i++)
        {
            if (strcmp(secondaryIndexList[i].getFirstName(), name) == 0)
            {
                vector<int> &ids = secondaryIndexList[i].getIds();
                ids.erase(remove(ids.begin(), ids.end(), id), ids.end());
                if (ids.empty())
                    secondaryIndexList.erase(secondaryIndexList.begin() + i);
                return;
            }
        }
    }

    int searchSecondaryName(char name[])
    {
        for (int i = 0; i < secondaryIndexList.size(); i++)
        {
            if (strcmp(secondaryIndexList[i].getFirstName(), name) == 0)
                return i;
        }
        return -1;
    }

    /*=============== Prof ============*/

    // READ & WRITE
    void writeProfToStream(fstream &file, const Prof &s)
    {
        int id = s.getId();
        file.write((char *)&id, sizeof(id));
        short len;
        const char *fields[] = {s.getFirstName(), s.getLastName(), s.getCity(),s.getCourseName()};
        for (int i = 0; i < 3; i++)
        {
            len = strlen(fields[i]);
            file.write((char *)&len, sizeof(len));
            file.write(fields[i], len);
        }
        float salary = s.getSalary();
        file.write((char *)&salary, sizeof(salary));
        int age = s.getAge();
        file.write((char *)&age, sizeof(age));

        len = strlen(fields[3]);
        file.write((char *)&len, sizeof(len));
        file.write(fields[3], len);
    }

    void readProfFromStream(fstream &file, Prof &s)
    {
        int id;
        file.read((char *)&id, sizeof(id));
        s.setId(id);

        short len;
        char buffer[21];

        file.read((char *)&len, sizeof(len));
        file.read(buffer, len);
        buffer[len] = '\0';
        s.setFirstName(buffer);

        file.read((char *)&len, sizeof(len));
        file.read(buffer, len);
        buffer[len] = '\0';
        s.setLastName(buffer);

        file.read((char *)&len, sizeof(len));
        file.read(buffer, len);
        buffer[len] = '\0';
        s.setCity(buffer);

        float salary;
        file.read((char *)&salary, sizeof(salary));
        s.setSalary(salary);

        int age;
        file.read((char *)&age, sizeof(age));
        s.setAge(age);

        file.read((char *)&len, sizeof(len));
        file.read(buffer, len);
        buffer[len] = '\0';
        s.setCourseName(buffer);
    }

    // INSERT WITH RECLAIMING 1
    void addNewProfToFile(fstream &file, const Prof &s)
    {
        short recSize = s.calcLength();
        int offset = getFreeSpace(recSize);
        if (offset != -1)
        {
            file.seekp(offset);
            cout << "Inserted in reclaimed space.\n";
        }
        else
        {
            file.seekp(0, ios::end);
            offset = (int)file.tellp();
            cout << "Inserted at end.\n";
        }
        addToPrimaryIndexList(PrimaryIndex(s.getId(), offset));
        addToSecondaryIndexList(s.getFirstName(), s.getId());
        writeProfToStream(file, s);
    }

    // DISPLAY 2,3
    void displayAllProfs(fstream &file)
    {
        Prof s;
        cout << "====== Profs ======\n";
        for (const auto &p : primaryIndexList)
        {
            file.seekg(p.getOffset());
            readProfFromStream(file, s);
            s.displayProf();
            cout << "\n---------------------------\n";
        }
    }

    void displayTopFiveProfs(fstream &file)
    {
        int cnt = 5;

        Prof s;
        cout << "====== Profs ======\n";
        for (const auto &p : primaryIndexList)
        {
            file.seekg(p.getOffset());
            readProfFromStream(file, s);
            s.displayProf();
            cout << "\n---------------------------\n";

            if (!(--cnt))
                break;
        }
    }

    // SEARCH 4,5
    void findProfById(fstream &file, int id)
    {
        int offset = searchInPrimaryIndexList(id);
        if (offset == -1)
        {
            cout << "Not found\n";
            return;
        }
        file.seekg(offset);
        Prof s;
        readProfFromStream(file, s);
        cout << "Prof Found.\n";
        s.displayProf();
        cout << "\n--------------------------\n";
    }

    void findProfByName(fstream &file, const char *name)
    {
        int low = 0, high = (int)secondaryIndexList.size() - 1, pos = -1;
        while (low <= high)
        {
            int mid = low + (high - low) / 2;
            int cmp = strcmp(secondaryIndexList[mid].getFirstName(), name);
            if (cmp == 0)
            {
                pos = mid;
                break;
            }
            if (cmp < 0)
                low = mid + 1;
            else
                high = mid - 1;
        }
        if (pos == -1)
        {
            cout << "Not found\n";
            return;
        }
        cout << "Profs Found.\n";
        for (int id : secondaryIndexList[pos].getIds())
        {
            int offset = searchInPrimaryIndexList(id);
            if (offset != -1)
            {
                file.seekg(offset);
                Prof s;
                readProfFromStream(file, s);
                s.displayProf();
                cout << "\n--------------------------\n";
            }
        }
    }

    // DELETE + AVAIL 6,7
    void deleteProfById(fstream &file, int id)
    {
        int offset = searchInPrimaryIndexList(id);
        if (offset == -1)
        {
            cout << "Not found\n";
            return;
        }
        file.seekg(offset);
        Prof s;
        readProfFromStream(file, s);
        short size = s.calcLength();
        availList.push_back(AvailNode(offset, size));
        deleteFromPrimaryIndexList(id);
        deleteFromSecondaryIndexList(s.getFirstName(), id);
        cout << "Prof deleted successfully.\n";
    }

    void deleteProfByName(fstream &file, char name[])
    {
        while (searchSecondaryName(name) != -1)
        {
            int pos = searchSecondaryName(name);

            int id = secondaryIndexList[pos].getIds()[0];

            deleteProfById(file, id);
        }

        cout << "All records deleted.\n";
    }

    // UPDATE 8,9
    void updateProfDataById(fstream &file, int id)
    {
        int offset = searchInPrimaryIndexList(id);
        if (offset == -1)
        {
            cout << "Not found\n";
            return;
        }
        file.seekg(offset);
        Prof oldS;
        readProfFromStream(file, oldS);
        short oldLen = oldS.calcLength();
        cout << "Current Data:\n";
        oldS.displayProf();
        cout << "\nNew Data:\n";
        Prof newS;
        newS.setId(id);
        newS.readProf(true, *this);
        short newLen = newS.calcLength();
        if (newLen <= oldLen)
        {
            file.seekp(offset);
            writeProfToStream(file, newS);
            int pad = oldLen - newLen;
            while (pad--)
                file.put('#');
        }
        else
        {
            deleteProfById(file, id);
            addNewProfToFile(file, newS);
        }
    }

    void updateProfDataByName(fstream &file, char name[])
    {
        int pos = searchSecondaryName(name);
        if (pos == -1)
        {
            cout << "Name not found\n";
            return;
        }

        vector<int> ids = secondaryIndexList[pos].getIds();

        if (ids.size() == 1)
        {
            updateProfDataById(file, ids[0]);
            return;
        }

        cout << "Records Found:\n";

        for (int id : ids)
        {
            cout << "ID : " << id << endl;
        }

        int chosenId;

        cout << "Enter ID to update: ";
        cin >> chosenId;

        bool found = false;

        for (int id : ids)
        {
            if (id == chosenId)
            {
                found = true;
                break;
            }
        }

        if (!found)
        {
            cout << "Invalid ID\n";
            return;
        }

        updateProfDataById(file, chosenId);
    }
};

void Prof::readProf(bool update, ProfSystem &system)
{
    if (!update)
    {
        while (true)
        {
            int temp;
            cout << "Enter Prof id: ";
            cin >> temp;
            if (system.searchInPrimaryIndexList(temp) != -1)
                cout << "Error: ID " << temp << " already exists.\n";
            else
            {
                id = temp;
                break;
            }
        }
    }
    cout << "Enter Prof first name: ";
    solveGetLineProblem();
    cin.getline(firstName, 14);
    cout << "Enter Prof last name: ";
    cin.getline(lastName, 14);
    cout << "Enter Prof city: ";
    cin.getline(city, 20);
    cout << "Enter Prof Salary: ";
    cin >> salary;
    cout << "Enter Prof age: ";
    cin >> age;
    cout << "Enter Prof Course Name: ";
    solveGetLineProblem();
    cin.getline(courseName, 20);
}
