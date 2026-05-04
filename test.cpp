#include <iostream>
#include <fstream>
#include <vector>
#include <algorithm>
#include <cstring>
using namespace std;

#include "StudentSystem.h"
// #include "ProfSystem.h"

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
