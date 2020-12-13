#include "readfile.h"


int modeHashCode(int key)
{
    return key % HASHNUMBER;
}


int* decToBinary(int n) 
{ 
    // array to store binary number 
    int binaryNum[BITSCOUNT]={0}; 
  
    // counter for binary array 
    int i = 0; 
    while (n > 0) { 
  
        // storing remainder in binary array 
        binaryNum[i] = n % 2; 
        n = n / 2; 
        i++; 
    }
    return binaryNum; 
}


int binaryToDecimal(int* n, int size)
{
    int dec_value = 0;
 
    // Initializing base value to 1, i.e 2^0
    int base = 1;
    int i =size-1;
    while (i>=0) { 
        dec_value += n[i] * base;
 
        base = base * 2;
        i--;
    }
 
    return dec_value;
}

//This function returns the directory id given the key and the depth
int getDirectoryId(int key, int depth){
    int *binary = decToBinary(key);
    int i= BITSCOUNT-1;
    int j = 0;
    int binaryNum[depth];
    while(j<depth){
        binaryNum[j]=binary[i];
        j++;
    }

    int id = binaryToDecimal(binaryNum, depth);
    return id;
}

int insertItem(int fdh, int fbh, DataItem item){

}


int searchItem(int fdh, int fbh, int key, int *count){


}


int deleteItem(int fdh, int fbh, int key){


}


int DisplayBucketsFile(int fbh){


}

int DisplayDirectoriesFile(int fdh){


}