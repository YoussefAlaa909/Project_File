#include <iostream>
#include <fstream>
#include <vector>
#include <algorithm>
#include <cstring>
using namespace std;

#include "PrimaryIndex.h"
#include "SecondaryIndex.h"
#include "AvailNode.h"
#include "Student.h"

/*================ STUDENT SYSTEM CLASS =================*/

class StudentSystem
{
private:
    vector<PrimaryIndex> primaryIndexList;
    vector<SecondaryIndex> secondaryIndexList;
    vector<AvailNode> availList;

public:
    StudentSystem() {}

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
        ifstream in("AvailList.txt", ios::binary);
        if (!in)
            return;
        AvailNode a;
        while (in.read((char *)&a, sizeof(a)))
            availList.push_back(a);
        in.close();
    }

    void writeAvailFile()
    {
        ofstream out("AvailList.txt", ios::binary | ios::trunc);
        for (const auto &a : availList)
            out.write((char *)&a, sizeof(a));
        out.close();
    }

    /*================ Primary Index =================*/

    void writePrimaryIndexFile()
    {
        ofstream out("PrimaryIndex.txt", ios::binary | ios::trunc);
        for (const auto &pi : primaryIndexList)
            out.write((char *)&pi, sizeof(pi));
        cout << "Primary Index saved successfully.\n";
        out.close();
    }

    void readPrimaryIndexFile()
    {
        ifstream in("PrimaryIndex.txt", ios::binary);
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
        ofstream out("SecondaryIndex.txt", ios::binary | ios::trunc);
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
        ifstream in("SecondaryIndex.txt", ios::binary);
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

    /*=============== Student ============*/

    // READ & WRITE
    void writeStudentToStream(fstream &file, const Student &s)
    {
        int id = s.getId();
        file.write((char *)&id, sizeof(id));
        short len;
        const char *fields[] = {s.getFirstName(), s.getLastName(), s.getCity()};
        for (int i = 0; i < 3; i++)
        {
            len = strlen(fields[i]);
            file.write((char *)&len, sizeof(len));
            file.write(fields[i], len);
        }
        float gpa = s.getGpa();
        file.write((char *)&gpa, sizeof(gpa));
        int age = s.getAge();
        file.write((char *)&age, sizeof(age));
        int level = s.getLevel();
        file.write((char *)&level, sizeof(level));
    }

    void readStudentFromStream(fstream &file, Student &s)
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
        float gpa;
        file.read((char *)&gpa, sizeof(gpa));
        s.setGpa(gpa);
        int age;
        file.read((char *)&age, sizeof(age));
        s.setAge(age);
        int level;
        file.read((char *)&level, sizeof(level));
        s.setLevel(level);
    }

    // INSERT WITH RECLAIMING 1
    void addNewStudentToFile(fstream &file, const Student &s)
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
        writeStudentToStream(file, s);
    }

    // DISPLAY 2,3
    void displayAllStudents(fstream &file)
    {
        Student s;
        cout << "====== Students ======\n";
        for (const auto &p : primaryIndexList)
        {
            file.seekg(p.getOffset());
            readStudentFromStream(file, s);
            s.displayStudent();
            cout << "\n---------------------------\n";
        }
    }

    void displayTopFiveStudents(fstream &file)
    {
        int cnt = 5;

        Student s;
        cout << "====== Students ======\n";
        for (const auto &p : primaryIndexList)
        {
            file.seekg(p.getOffset());
            readStudentFromStream(file, s);
            s.displayStudent();
            cout << "\n---------------------------\n";

            if (!(--cnt))
                break;
        }
    }

    // SEARCH 4,5
    void findStudentById(fstream &file, int id)
    {
        int offset = searchInPrimaryIndexList(id);
        if (offset == -1)
        {
            cout << "Not found\n";
            return;
        }
        file.seekg(offset);
        Student s;
        readStudentFromStream(file, s);
        cout << "Student Found.\n";
        s.displayStudent();
        cout << "\n--------------------------\n";
    }

    void findStudentByName(fstream &file, const char *name)
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
        cout << "Students Found.\n";
        for (int id : secondaryIndexList[pos].getIds())
        {
            int offset = searchInPrimaryIndexList(id);
            if (offset != -1)
            {
                file.seekg(offset);
                Student s;
                readStudentFromStream(file, s);
                s.displayStudent();
                cout << "\n--------------------------\n";
            }
        }
    }

    // DELETE + AVAIL 6,7
    void deleteStudentById(fstream &file, int id)
    {
        int offset = searchInPrimaryIndexList(id);
        if (offset == -1)
        {
            cout << "Not found\n";
            return;
        }
        file.seekg(offset);
        Student s;
        readStudentFromStream(file, s);
        short size = s.calcLength();
        availList.push_back(AvailNode(offset, size));
        deleteFromPrimaryIndexList(id);
        deleteFromSecondaryIndexList(s.getFirstName(), id);
        cout << "Student deleted successfully.\n";
    }

    void deleteStudentByName(fstream &file, char name[])
    {
        while (searchSecondaryName(name) != -1)
        {
            int pos = searchSecondaryName(name);

            int id = secondaryIndexList[pos].getIds()[0];

            deleteStudentById(file, id);
        }

        cout << "All records deleted.\n";
    }

    // UPDATE 8,9
    void updateStudentDataById(fstream &file, int id)
    {
        int offset = searchInPrimaryIndexList(id);
        if (offset == -1)
        {
            cout << "Not found\n";
            return;
        }
        file.seekg(offset);
        Student oldS;
        readStudentFromStream(file, oldS);
        short oldLen = oldS.calcLength();
        cout << "Current Data:\n";
        oldS.displayStudent();
        cout << "\nNew Data:\n";
        Student newS;
        newS.setId(id);
        newS.readStudent(true, *this);
        short newLen = newS.calcLength();
        if (newLen <= oldLen)
        {
            file.seekp(offset);
            writeStudentToStream(file, newS);
            int pad = oldLen - newLen;
            while (pad--)
                file.put('#');
        }
        else
        {
            deleteStudentById(file, id);
            addNewStudentToFile(file, newS);
        }
    }

    void updateStudentDataByName(fstream &file, char name[])
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
            updateStudentDataById(file, ids[0]);
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

        updateStudentDataById(file, chosenId);
    }
};

void Student::readStudent(bool update, StudentSystem &system)
{
    if (!update)
    {
        while (true)
        {
            int temp;
            cout << "Enter Student id: ";
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
    cout << "Enter Student first name: ";
    solveGetLineProblem();
    cin.getline(firstName, 14);
    cout << "Enter Student last name: ";
    cin.getline(lastName, 14);
    cout << "Enter Student city: ";
    cin.getline(city, 20);
    cout << "Enter Student gpa: ";
    cin >> gpa;
    cout << "Enter Student age: ";
    cin >> age;
    cout << "Enter Student level: ";
    cin >> level;
}
