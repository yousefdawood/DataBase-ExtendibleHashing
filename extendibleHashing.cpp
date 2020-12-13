#include "readfile.h"


int modeHashCode(int key)
{
    return key % HASHNUMBER;
}


// Covert decimal to binary given a decimal number
int* decToBinary(int n) //4->[0,0,1]
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
    //reverse array
    int start = 0;
    int end = BITSCOUNT-1;
    while (start < end)
    {
        int temp = binaryNum[start]; 
        binaryNum[start] = binaryNum[end];
        binaryNum[end] = temp;
        start++;
        end--;
    } 


    return binaryNum; 
}

// covert binary to decimal given binary number in array form
int binaryToDecimal(int* n, int size) //[1,0,0] -> 4
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

// Insert a new key 
/* fdh: file handle of directory file
   fbh: file handle of buckets file
   key: key to be inserted

   returns offset where key is inserted
*/
int insertItem(int fdh, int fbh, int key){

}

// Search for a key 
/* fdh: file handle of directory file
   fbh: file handle of buckets file
   key: key to be searched for 

   returns offset where key is found
   else -1;
*/
int searchItem(int fdh, int fbh, int key){


}


/* Function to colapse the directory file if possible 
   fdh: file handle of directory file

   returns depth of directory file (global depth)
*/
void colapseDirectory(int fdh){
    struct Directory read_dir_data;
    Directory write_dir_data;
    ssize_t result_r = pread(fdh, &read_dir_data, sizeof(Directory), 0);
    int new_depth = read_dir_data.depth;
    bool can_colapse = true;
    for(int i =0; i<read_dir_data.records.size()/2; i++){
        if(read_dir_data.records[2*i].offset != read_dir_data.records[2*i+1].offset){
            can_colapse = false;
            break;
        }
    }

    if(can_colapse){
        new_depth = read_dir_data.depth - 1;
        vector<DirectoryRecord> new_records;
        
        for(int i =0; i<read_dir_data.records.size()/2; i++){
            struct DirectoryRecord record;
            record.id = read_dir_data.records[2*i].id / 2;
            record.offset = read_dir_data.records[2*i].offset;
            new_records.push_back(record);
        }

        write_dir_data.depth = new_depth;
        write_dir_data.records = new_records;
        int result_w = pwrite(fdh, &write_dir_data, sizeof(Directory), 0);
        if (result_w < sizeof(Directory)){
            perror("some error occurred in pwrite while wrting the new directory file.");
        }
    }
    DisplayDirectoriesFile(fdh);
}


// delete a key if exist 
/* fdh: file handle of directory file
   fbh: file handle of buckets file
   key: key to be searched for 

   returns -1 if key not found
   else 1;
*/
int deleteItem(int fdh, int fbh, int key){


}


int DisplayBucketsFile(int fbh){


}

int DisplayDirectoriesFile(int fdh, int fbh){

    Directory d;
    pread(fdh, &d, sizeof(Directory), 0);
    cout <<"Global Depth = "<<d.depth<<endl;
    cout<<"Directory size = "<< (int)pow(2,d.depth)<<endl;
    for (int i = 0; i < (int)pow(2,d.depth); i++)
    {
        // print each record
        cout<<"Record ID = "<<d.records[i].id<<";  Bucket Offset = "<<d.records[i].offset<<endl;
        
        Bucket b;
        pread(fbh, &b, sizeof(Bucket), d.records[i].offset);
        
        cout<<"Bucket depth = "<<b.depth<<endl;
        for (int j = 0; j < RECORDSPERBUCKET; j++)
        {
            if (b.dataItem[j].valid == 1)
            {
                cout<<b.dataItem[j].key<<" ";
            }
            
        }
        cout<<endl;
        
    }
    

}