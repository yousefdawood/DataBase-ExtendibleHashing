#include "readfile.h"
using namespace std;

int modeHashCode(int key)
{
    return key % HASHNUMBER;
}

int offsetOfNewBucket(int fbh){
    int offset = 0;
    bool found = false;
    while(!found){
    Bucket bucket;
    ssize_t result = pread(fbh, &bucket, sizeof(Bucket), offset);
    if (result <= 0)//error or unallocated space
    {
        found = true;
        return offset;
    }
    else if (bucket.valid == 0)// empty bucket
    {
        found = true;
        return offset;
    }
    else{
        offset += sizeof(Bucket);
    }
    
    }
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
/*
int insertItem(int fdh, int fbh, int key){
    // hash the key
    int newKey = modeHashCode(key);
    //ReadDirectory
    Directory d;
    ssize_t result = pread(fdh, &d, sizeof(Directory), 0);
	if (result <= 0) //either an error happened in the pread or it hit an unallocated space
	{				 // perror("some error occurred in pread");
		return -1;
	}
    // get key in bit representation
    int * keyInBits = decToBinary(newKey);
    // get the bucket that the key will be inserted into
    
    //get the bucket record in the directory table
    int bucketRecordId = binaryToDecimal(keyInBits,d.depth);
    DirectoryRecord bucketRecord = d.records[bucketRecordId];

    //get the bucket
    Bucket bucket;
    result = pread(fbh, &bucket, sizeof(Bucket), bucketRecord.offset);

    // insert into the bucket
    // if place is avilabe
    for (size_t i = 0; i < RECORDSPERBUCKET; i++)
    {   // if there is avialbe space in bucket
        if (bucket.dataItem[i].valid == 0)
        {
            //insert key 
            bucket.dataItem[i].valid = 1;
            bucket.dataItem[i].key = key;
            //write in file
            pwrite(fbh,&bucket,sizeof(Bucket),bucketRecord.offset);
            // return success
            return 1;
        }
        
    }
    // if no space availabe then need to expand
    //first find offset for new bucket
    int newBucketOffset = offsetOfNewBucket(fbh);

    //check on global and local depth
    if (d.depth == bucket.depth)// new to expand the directory
    {
        // first allocate the new bucket
        Bucket newBucket;
        newBucket.depth = bucket.depth+1;
        newBucket.valid = 1;
        for (size_t i = 0; i < RECORDSPERBUCKET; i++)
        {
            DataItem d;
            d.valid = 0;
            d.key = -1;
            newBucket.dataItem[i] = d;
        }

        pwrite(fbh,&newBucket,sizeof(Bucket),newBucketOffset);

        //second get all keys of the old bucket plus the new key
        //also empty the old bucket
        int keys [RECORDSPERBUCKET+1];
        for (size_t i = 0; i < RECORDSPERBUCKET; i++)
        {
            keys[i] = bucket.dataItem[i].key;
            bucket.dataItem[i].valid = 0;
            bucket.depth += 1;
        }
        //add the new key to the list of keys
        
        keys[RECORDSPERBUCKET] = key;

        //third double directory size
        Directory newDirectory;
        // increase depth by 1
        newDirectory.depth = d.depth+1;

        //handle the pointers
        int dSize = (int)pow(2,d.depth);
        for (int i = 0; i < dSize; i++)
        {
            if (d.records[i].id != bucketRecordId)// not the bucket we working with
            {
                newDirectory.records[2*i].offset = d.records[i].offset;
                newDirectory.records[2*i].id = d.records[i].id*2;

                newDirectory.records[2*i+1].offset = d.records[i].offset;
                newDirectory.records[2*i+1].id = d.records[i].id*2+1;
            }
            else
            {
                newDirectory.records[2*i].offset = d.records[i].offset;
                newDirectory.records[2*i].id = d.records[i].id*2;

                newDirectory.records[2*i+1].offset = newBucketOffset;
                newDirectory.records[2*i+1].id = d.records[i].id*2+1;
            }
            
            
        }

        // fourth write the new directory in the file
        ssize_t r = pwrite(fdh, &newDirectory, sizeof(Directory), 0);
        
        //fifth reinsert the keys
        for (int i = 0; i < RECORDSPERBUCKET+1; i++)
        {
            insertItem(fdh,fbh,keys[i]);
        }
        

    }
    else{// no need for directory expansion
        
        // first allocate the new bucket
        Bucket newBucket;
        newBucket.depth = bucket.depth+1;
        newBucket.valid = 1;
        for (size_t i = 0; i < RECORDSPERBUCKET; i++)
        {
            DataItem d;
            d.valid = 0;
            d.key = -1;
            newBucket.dataItem[i] = d;
        }

        pwrite(fbh,&newBucket,sizeof(Bucket),newBucketOffset);

        //second get all keys of the old bucket plus the new key
        //also empty the old bucket
        int keys [RECORDSPERBUCKET+1];
        for (size_t i = 0; i < RECORDSPERBUCKET; i++)
        {
            keys[i] = bucket.dataItem[i].key;
            bucket.dataItem[i].valid = 0;
            bucket.depth += 1;
        }
        //add the new key to the list of keys
        keys[RECORDSPERBUCKET] = key;

        //third adjust the directory file: adjust the offset value of the records

        // number of recordes point to the bucket
        // # = 2^(global depth - local depth)
        int recordsNum = (int)pow(2,(d.depth-bucket.depth));
        bool firstHalfOfRecords = true;
        int aux = 0;//to check if i finished the fisrt half
        for (int i = 0; i < d.records.size(); i++)
        {
            if (aux >= recordsNum/2)
            {
                firstHalfOfRecords = false;
            }
            
            if (d.records[i].offset == bucketRecord.offset)
            {
                aux++;
                if (!firstHalfOfRecords)
                {
                    d.records[i].offset = newBucketOffset;
                }
                
            }
            
        }

        // fourth write the modified directory in the file
        ssize_t r = pwrite(fdh, &d, sizeof(Directory), 0);
        
        //fifth reinsert the keys
        for (int i = 0; i < RECORDSPERBUCKET+1; i++)
        {
            insertItem(fdh,fbh,keys[i]);
        }
        

    }

    return 1;
    
}
*/
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