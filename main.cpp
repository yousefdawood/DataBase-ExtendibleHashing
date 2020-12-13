#include "readfile.h"
#include <iostream>

#include"readfile.cpp"
#include"extendibleHashing.cpp"
using namespace std;

int main(){
    int fdh = createFile(DIRECTORYFILESIZE, "DirectoryFile", 0);
    int fbh = createFile(DATAFILESIZE, "DataFile", 1);
    DisplayDirectoriesFile(fdh,fbh);
    return 0;
}
