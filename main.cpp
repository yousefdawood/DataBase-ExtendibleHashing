#include "readfile.h"
#include <iostream>

using namespace std;

int main(){
    int fdh = createFile(DIRECTORYFILESIZE, "DirectoryFile", 0);
    int fbh = createFile(DATAFILESIZE, "DataFile", 1);
    DisplayDirectoriesFile(fdh,fbh);
    return 0;
}
