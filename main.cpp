#include "readfile.h"
#include <iostream>

#define DATAFILESIZE HASHNUMBER*sizeof(Bucket)
#define DIRECTORYFILESIZE (HASHNUMBER+10)*sizeof(DirectoryRecord)
using namespace std;

int main(){

    int dataf = createFile(DATAFILESIZE, "DataFile", 1);
    int directoryf = createFile(DIRECTORYFILESIZE, "DirectoryFile", 0);

    return 0;
}
