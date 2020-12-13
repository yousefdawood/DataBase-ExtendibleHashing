#include "readfile.h"
#include <iostream>

#define DATAFILESIZE HASHNUMBER*sizeof(Bucket)
#define DIRECTORYFILESIZE (HASHNUMBER+10)*sizeof(DirectoryRecord)
using namespace std;

int main(){

    int fdh = createFile(DIRECTORYFILESIZE, "DirectoryFile", 0);
    int fbh = createFile(DATAFILESIZE, "DataFile", 1);
    DisplayDirectoriesFile(fdh,fbh);
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
    insertItem(fdh,fbh,1);
    insertItem(fdh,fbh,2);
    insertItem(fdh,fbh,3);
    insertItem(fdh,fbh,31);
    insertItem(fdh,fbh,30);
    insertItem(fdh,fbh,16);
    DisplayDirectoriesFile(fdh,fbh);
    return 0;
}
