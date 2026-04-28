#include <iostream>
#include <fstream>
#include <vector>
#include <algorithm>
#include <cstring>

using namespace std;

/*================ PRIMARY INDEX CLASS =================*/

class PrimaryIndex
{
public:
    int id;
    int offset;

    PrimaryIndex() : id(0), offset(0) {}
    PrimaryIndex(int id, int offset) : id(id), offset(offset) {}

    int getId() const { return id; }
    int getOffset() const { return offset; }

    bool operator<(const PrimaryIndex &p) const
    {
        return id < p.id;
    }
};

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

/*================ AVAIL NODE CLASS =================*/

class AvailNode
{
public:
    int offset;
    short size;

    AvailNode() : offset(0), size(0) {}
    AvailNode(int offset, short size) : offset(offset), size(size) {}

    int getOffset() const { return offset; }
    short getSize() const { return size; }

    bool operator<(const AvailNode &a) const
    {
        return size < a.size;
    }
};

/*================ STUDENT CLASS =================*/

class Student
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
    Student() : id(0), gpa(0.0), age(0), level(0)
    {
        firstName[0] = lastName[0] = city[0] = '\0';
    }

    void readStudent(bool update, class StudentSystem &system); // Important

    void displayStudent() const
    {
        cout << "Student id        : " << id << endl;
        cout << "Student First Name: " << firstName << endl;
        cout << "Student Last Name : " << lastName << endl;
        cout << "Student City      : " << city << endl;
        cout << "Student GPA       : " << gpa << endl;
        cout << "Student Age       : " << age << endl;
        cout << "Student Level     : " << level << endl;
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
            if (primaryIndexList[i].id == id)
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
            if (primaryIndexList[mid].id == id)
                return primaryIndexList[mid].offset;
            if (primaryIndexList[mid].id < id)
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
            file.seekg(p.offset);
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
            file.seekg(p.offset);
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

int main()
{
    // Corrected file opening to ensure file exists
    fstream dataFile("file.txt", ios::binary | ios::in | ios::out | ios::app);
    dataFile.close();
    dataFile.open("file.txt", ios::binary | ios::in | ios::out);

    StudentSystem program;
    program.readPrimaryIndexFile();
    program.readSecondaryIndexFile();
    program.readAvailFile();

    int choice;
    do
    {
        cout << "===========================\n"
             << "1.Add New Student To File.\n"
             << "2.Display All Students.\n"
             << "3.Display Top 5 Students.\n"
             << "4.Find Student By Id.\n"
             << "5.Find Student By Name.\n"
             << "6.Delete Student By Id.\n"
             << "7.Delete Student By Name.\n"
             << "8.Update Student By Id.\n"
             << "9.Update Student By Name.\n"
             << "0.Exit.\n"
             << "===========================\n"
             << "Enter Choice : ";
        cin >> choice;

        switch (choice)
        {
        case 1:
        {
            Student s;
            s.readStudent(false, program);
            program.addNewStudentToFile(dataFile, s);
            break;
        }
        case 2:
            program.displayAllStudents(dataFile);
            break;
        case 3:
            program.displayTopFiveStudents(dataFile);
            break;
        case 4:
        {
            int id;
            cout << "Enter id: ";
            cin >> id;
            program.findStudentById(dataFile, id);
            break;
        }
        case 5:
        {
            char name[14];
            cout << "Enter name: ";
            cin.ignore();
            cin.getline(name, 14);
            program.findStudentByName(dataFile, name);
            break;
        }
        case 6:
        {
            int id;
            cout << "Enter id: ";
            cin >> id;
            program.deleteStudentById(dataFile, id);
            break;
        }
        case 7:
        {
            char name[14];
            cout << "Enter name: ";
            cin.ignore();
            cin.getline(name, 14);
            program.deleteStudentByName(dataFile, name);
            break;
        }
        case 8:
        {
            int id;
            cout << "Enter id: ";
            cin >> id;
            program.updateStudentDataById(dataFile, id);
            break;
        }
        case 9:
        {
            char name[14];
            cout << "Enter name: ";
            cin.ignore();
            cin.getline(name, 14);
            program.updateStudentDataByName(dataFile, name);
            break;
        }

        case 0:
            program.writePrimaryIndexFile();
            program.writeSecondaryIndexFile();
            program.writeAvailFile();
            cout << "Exit...\n";
            break;
        }
        if (choice != 0)
        {
            system("pause");
            system("cls");
        }
        dataFile.clear();
    } while (choice != 0);

    dataFile.close();
    return 0;
}
