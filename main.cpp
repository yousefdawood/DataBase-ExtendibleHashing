#include "readfile.h"
#include <iostream>
#include <string.h>

using namespace std;

int main()
{

    int fdh = createFile(DIRECTORYFILESIZE, "DirectoryFile", 0);
    int fbh = createFile(DATAFILESIZE, "DataFile", 1);
    DisplayDirectoriesFile(fdh, fbh);
    printf("---------------------------------------------------------------------");
    /*insertItem(fdh,fbh,0);
    insertItem(fdh,fbh,31);
    insertItem(fdh,fbh,5);
    insertItem(fdh,fbh,16);
    insertItem(fdh,fbh,8);
    insertItem(fdh,fbh,3);
    insertItem(fdh,fbh,7);
    insertItem(fdh,fbh,24);
    insertItem(fdh,fbh,12);
    insertItem(fdh,fbh,13);
    insertItem(fdh,fbh,25);
    insertItem(fdh,fbh,1);*/
    // insertItem(fdh, fbh, 1);
    // insertItem(fdh, fbh, 2);
    // insertItem(fdh, fbh, 3);
    // insertItem(fdh, fbh, 31);
    // insertItem(fdh, fbh, 30);
    // insertItem(fdh, fbh, 16);

    //Sheet Example
    insertItem(fdh, fbh, 2369); // 00001
    insertItem(fdh, fbh, 3760); // 10000
    insertItem(fdh, fbh, 4692); // 10100
    //insertItem(fdh, fbh, 4871); // 00111
    //insertItem(fdh, fbh, 1821); // 11101

    DisplayDirectoriesFile(fdh, fbh);
    printf("---------------------------------------------------------------------");

    deleteItem(fdh, fbh, 2369);
    // deleteItem(fdh, fbh, 2369);

    DisplayDirectoriesFile(fdh, fbh);
    printf("---------------------------------------------------------------------");

    insertItem(fdh, fbh, 2369);

    DisplayDirectoriesFile(fdh, fbh);

    // vector<int> searchedKey = searchItem(fdh, fbh, 4871);

    // for (int i = 0; i < searchedKey.size(); i++)
    // {
    //     cout << searchedKey[i] << endl;
    // }
    // close file handlers and remove files
    close(fdh);
    close(fbh);

    remove("DataFile");
    remove("DirectoryFile");

    return 0;
}
