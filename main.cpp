#include "readfile.h"
#include <iostream>

#define DATAFILESIZE HASHNUMBER*sizeof(Bucket)
#define DIRECTORYFILESIZE (HASHNUMBER+10)*sizeof(DirectoryRecord)
using namespace std;

int main(){

    int fdh = createFile(DIRECTORYFILESIZE, "DirectoryFile", 0);
    int fbh = createFile(DATAFILESIZE, "DataFile", 1);
    DisplayDirectoriesFile(fdh,fbh);
    return 0;
}
