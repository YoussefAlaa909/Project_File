#include <iostream>
#include <fstream>
#include <vector>
#include <algorithm>
#include <cstring>
using namespace std;

#include "Student/StudentSystem.h"
#include "Prof/ProfSystem.h"

void StudentProgram()
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
}

void ProfProgram()
{
    // Corrected file opening to ensure file exists
    fstream dataFile("ProfFile.txt", ios::binary | ios::in | ios::out | ios::app);
    dataFile.close();
    dataFile.open("ProfFile.txt", ios::binary | ios::in | ios::out);

    ProfSystem program;
    program.readPrimaryIndexFile();
    program.readSecondaryIndexFile();
    program.readAvailFile();

    int choice;
    do
    {
        cout << "===========================\n"
             << "1.Add New Prof To File.\n"
             << "2.Display All Profs.\n"
             << "3.Display Top 5 Profs.\n"
             << "4.Find Prof By Id.\n"
             << "5.Find Prof By Name.\n"
             << "6.Delete Prof By Id.\n"
             << "7.Delete Prof By Name.\n"
             << "8.Update Prof By Id.\n"
             << "9.Update Prof By Name.\n"
             << "0.Exit.\n"
             << "===========================\n"
             << "Enter Choice : ";
        cin >> choice;

        switch (choice)
        {
        case 1:
        {
            Prof s;
            s.readProf(false, program);
            program.addNewProfToFile(dataFile, s);
            break;
        }
        case 2:
            program.displayAllProfs(dataFile);
            break;
        case 3:
            program.displayTopFiveProfs(dataFile);
            break;
        case 4:
        {
            int id;
            cout << "Enter id: ";
            cin >> id;
            program.findProfById(dataFile, id);
            break;
        }
        case 5:
        {
            char name[14];
            cout << "Enter name: ";
            cin.ignore();
            cin.getline(name, 14);
            program.findProfByName(dataFile, name);
            break;
        }
        case 6:
        {
            int id;
            cout << "Enter id: ";
            cin >> id;
            program.deleteProfById(dataFile, id);
            break;
        }
        case 7:
        {
            char name[14];
            cout << "Enter name: ";
            cin.ignore();
            cin.getline(name, 14);
            program.deleteProfByName(dataFile, name);
            break;
        }
        case 8:
        {
            int id;
            cout << "Enter id: ";
            cin >> id;
            program.updateProfDataById(dataFile, id);
            break;
        }
        case 9:
        {
            char name[14];
            cout << "Enter name: ";
            cin.ignore();
            cin.getline(name, 14);
            program.updateProfDataByName(dataFile, name);
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
}

int main()
{
    int choice;
    do
    {
        cout << "Choose Entity:\n1:Student 2:Prof 0:Exit\nSystem: ";
        cin >> choice;
        switch (choice)
        {
        case 1:
        {
            system("cls");
            StudentProgram();
            break;
        }
        case 2:
            system("cls");
            ProfProgram();
            break;

        case 0:
            cout << "Exit...\n";
            break;
        }
        if (choice != 0)
        {
            system("pause");
            system("cls");
        }
    } while (choice != 0);

    return 0;
}
