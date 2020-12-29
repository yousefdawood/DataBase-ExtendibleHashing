#include "readfile.h"
#include <iostream>
#include <string.h>

using namespace std;
void InsertSimpleTestCase(int fdh, int fbh);
void InsertAdvancedTestCase1(int fdh, int fbh);
void InsertDeleteTestCase1(int fdh, int fbh);
void TestCase(int fdh, int fbh);

int main()
{

    int fdh = createFile(DIRECTORYFILESIZE, "DirectoryFile", 0);
    int fbh = createFile(DATAFILESIZE, "DataFile", 1);
    DisplayDirectoriesFile(fdh, fbh);
    printf("---------------------------------------------------------------------\n");

    int type;

    do
    {
        printf("\n\nChoose Test Case\n\n");
        printf("1. InsertSimpleTestCase\n");
        printf("2. InsertAdvancedTestCase1\n");
        printf("3. InsertDeleteTestCase1\n");
        printf("4. Sheet Test Case\n");
        printf("5. Exit\n");
        scanf("%d", &type);

        switch (type)
        {
        case 1:

            InsertSimpleTestCase(fdh, fbh);
            fdh = createFile(DIRECTORYFILESIZE, "DirectoryFile", 0);
            fbh = createFile(DATAFILESIZE, "DataFile", 1);
            break;
        case 2:

            InsertAdvancedTestCase1(fdh, fbh);
            fdh = createFile(DIRECTORYFILESIZE, "DirectoryFile", 0);
            fbh = createFile(DATAFILESIZE, "DataFile", 1);
            break;

        case 3:
            InsertDeleteTestCase1(fdh, fbh);
            fdh = createFile(DIRECTORYFILESIZE, "DirectoryFile", 0);
            fbh = createFile(DATAFILESIZE, "DataFile", 1);
            break;

        case 4:
            TestCase(fdh, fbh);
            //fdh = createFile(DIRECTORYFILESIZE, "DirectoryFile", 0);
            //fbh = createFile(DATAFILESIZE, "DataFile", 1);
            break;

        case 5:
            printf("Goodbye\n");

            break;
        case 6:
             DisplayDirectoriesFile(fdh, fbh);
            insertItem(fdh,fbh,20);
             DisplayDirectoriesFile(fdh, fbh);
            break;
        case 7:
            fdh = createFile(DIRECTORYFILESIZE, "DirectoryFile", 0);
            fbh = createFile(DATAFILESIZE, "DataFile", 1);
             DisplayDirectoriesFile(fdh, fbh);
            close(fdh);
            close(fbh);
            break;
        default:
            printf("Wrong Choice. Enter again\n");
            break;
        }

    } while (type != 5);

    return 0;

    return 0;
}

void TestCase(int fdh, int fbh)
{

    //Sheet Example
    insertItem(fdh, fbh, 2369); // 00001
    insertItem(fdh, fbh, 3760); // 10000
    insertItem(fdh, fbh, 4692); // 10100
    insertItem(fdh, fbh, 4871); // 00111
    printf("----------------------------AFTER FIRST SPLIT--------------------------------------\n");
    DisplayDirectoriesFile(fdh, fbh);
    printf("---------------------------------------------------------------------\n");
    insertItem(fdh, fbh, 5659); // 11011
    printf("----------------------------Double DIRECTORY Size --------------------------------------\n");
    DisplayDirectoriesFile(fdh, fbh);
    printf("---------------------------------------------------------------------\n");
    insertItem(fdh, fbh, 1821); // 11101
    insertItem(fdh, fbh, 1074); // 10010
    insertItem(fdh, fbh, 7115); // 01011
    insertItem(fdh, fbh, 1620); // 10100
    // insertItem(fdh, fbh, 2428); // 11100

    DisplayDirectoriesFile(fdh, fbh);
    printf("----------------------------Search for key 1620-----------------------------------------\n\n");

    searchResult(fdh, fbh, 1620);
    printf("----------------------------Delete key 1620-----------------------------------------\n\n");

    deleteItem(fdh, fbh, 1620);
    printf("---------------------------------------------------------------------\n");
    DisplayDirectoriesFile(fdh, fbh);
    printf("----------------------------Search for key 1620-----------------------------------------\n\n");

    searchResult(fdh, fbh, 1620);

    printf("----------------------------Delte key 1074 and Merge buckets then collapse-----------\n");

    deleteItem(fdh, fbh, 1074);
    printf("---------------------------------------------------------------------\n");
    DisplayDirectoriesFile(fdh, fbh);

    // close file handlers and remove files

    close(fdh);
    close(fbh);

    //remove("DataFile");
    //remove("DirectoryFile");
}
void InsertSimpleTestCase(int fdh, int fbh)
{
    insertItem(fdh, fbh, 0);
    insertItem(fdh, fbh, 31);
    insertItem(fdh, fbh, 5);
    insertItem(fdh, fbh, 16);
    insertItem(fdh, fbh, 8);
    insertItem(fdh, fbh, 3);
    insertItem(fdh, fbh, 7);
    insertItem(fdh, fbh, 24);
    insertItem(fdh, fbh, 12);
    insertItem(fdh, fbh, 13);
    insertItem(fdh, fbh, 25);
    insertItem(fdh, fbh, 1);
    DisplayDirectoriesFile(fdh, fbh);

    // close file handlers and remove files

    close(fdh);
    close(fbh);

    remove("DataFile");
    remove("DirectoryFile");
}
void InsertAdvancedTestCase1(int fdh, int fbh)
{
    insertItem(fdh, fbh, 1);
    insertItem(fdh, fbh, 2);
    insertItem(fdh, fbh, 3);
    insertItem(fdh, fbh, 31);
    insertItem(fdh, fbh, 30);
    insertItem(fdh, fbh, 16);
    DisplayDirectoriesFile(fdh, fbh);

    // close file handlers and remove files

    close(fdh);
    close(fbh);

    remove("DataFile");
    remove("DirectoryFile");
}
void InsertDeleteTestCase1(int fdh, int fbh)
{
    insertItem(fdh, fbh, 1);
    insertItem(fdh, fbh, 2);
    insertItem(fdh, fbh, 3);
    insertItem(fdh, fbh, 31);
    insertItem(fdh, fbh, 30);
    insertItem(fdh, fbh, 16);
    printf("---------------------------------------------------------------------\n");

    DisplayDirectoriesFile(fdh, fbh);
    printf("---------------------------------------------------------------------\n");

    deleteItem(fdh, fbh, 31);
    deleteItem(fdh, fbh, 30);
    deleteItem(fdh, fbh, 16);
    printf("---------------------------------------------------------------------\n");
    DisplayDirectoriesFile(fdh, fbh);

    printf("---------------------------------------------------------------------\n");

    deleteItem(fdh, fbh, 2);

    printf("---------------------------------------------------------------------\n");
    DisplayDirectoriesFile(fdh, fbh);

    // close file handlers and remove files

    close(fdh);
    close(fbh);

    remove("DataFile");
    remove("DirectoryFile");
}