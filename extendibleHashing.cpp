#include "readfile.h"
using namespace std;

int modeHashCode(int key)
{
    return key % HASHNUMBER;
}

int offsetOfNewBucket(int fbh)
{
    int offset = 0;
    bool found = false;
    while (!found)
    {
        Bucket bucket;
        ssize_t result = pread(fbh, &bucket, sizeof(Bucket), offset);
        if (result <= 0) //error or unallocated space
        {
            found = true;
            return offset;
        }
        else if (bucket.valid == 0) // empty bucket
        {
            found = true;
            return offset;
        }
        else
        {
            offset += sizeof(Bucket);
        }
    }
}

// Covert decimal to binary given a decimal number
int *decToBinary(int n) //4->[0,0,1]
{
    // array to store binary number
    int *binaryNum = new int[BITSCOUNT];

    // counter for binary array
    int i = 0;
    while (n > 0)
    {

        // storing remainder in binary array
        binaryNum[i] = n % 2;
        n = n / 2;
        i++;
    }
    //reverse array
    int start = 0;
    int end = BITSCOUNT - 1;
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
int binaryToDecimal(int *n, int size) //[1,0,0] -> 4
{
    int dec_value = 0;

    // Initializing base value to 1, i.e 2^0
    int base = 1;
    int i = size - 1;
    while (i >= 0)
    {

        dec_value += n[i] * base;

        base = base * 2;
        i--;
    }

    return dec_value;
}

// get directory number that is budy with deleted bucket

int getBudyBucket(int dirId, int depth)
{
    int *dirBits = decToBinary(dirId);

    // for (int i = 0; i < BITSCOUNT; i++)
    // {
    //     cout << dirBits[i] << " ";
    // }
    // cout << endl;

    dirBits[BITSCOUNT - 1] = 1 - dirBits[BITSCOUNT - 1];

    // for (int i = 0; i < BITSCOUNT; i++)
    // {
    //     cout << dirBits[i] << " ";
    // }
    // cout << endl;

    return binaryToDecimal(dirBits, 5);
}

// Insert a new key
/* fdh: file handle of directory file
   fbh: file handle of buckets file
   key: key to be inserted

   returns offset where key is inserted
*/

int insertItem(int fdh, int fbh, int key)
{
    // hash the key
    int newKey = modeHashCode(key);
    //Read Directory
    Directory d;

    ssize_t result = pread(fdh, &d, sizeof(Directory), 0);
    if (result <= 0) //either an error happened in the pread or it hit an unallocated space
    {                // perror("some error occurred in pread");
        return -1;
    }

    // get key in bit representation
    int *keyInBits = decToBinary(newKey);
    for (int i = 0; i < BITSCOUNT; i++)
    {
        //cout << keyInBits[i] << " ";
    }
    //cout << endl;
    // get the bucket that the key will be inserted into

    //get the bucket record in the directory table
    int bucketRecordId = binaryToDecimal(keyInBits, d.depth);

    DirectoryRecord bucketRecord = d.records[bucketRecordId];

    //get the bucket
    Bucket bucket;
    result = pread(fbh, &bucket, sizeof(Bucket), bucketRecord.offset);

    // insert into the bucket
    // if place is avilabe
    for (size_t i = 0; i < RECORDSPERBUCKET; i++)
    { // if there is avialbe space in bucket
        if (bucket.dataItem[i].valid == 0)
        {
            //insert key
            bucket.dataItem[i].valid = 1;
            bucket.dataItem[i].key = key;
            //write in file
            pwrite(fbh, &bucket, sizeof(Bucket), bucketRecord.offset);
            // return success
            return 1;
        }
    }

    // if no space availabe then need to expand
    //first find offset for new bucket
    int newBucketOffset = offsetOfNewBucket(fbh);
    //cout << "newBucketOffset = " << newBucketOffset << endl;
    //check on global and local depth
    if (d.depth == bucket.depth) // new to expand the directory
    {
        // first allocate the new bucket
        Bucket newBucket;
        newBucket.depth = bucket.depth + 1;
        newBucket.valid = 1;
        for (size_t i = 0; i < RECORDSPERBUCKET; i++)
        {
            DataItem d;
            d.valid = 0;
            d.key = -1;
            newBucket.dataItem[i] = d;
        }

        pwrite(fbh, &newBucket, sizeof(Bucket), newBucketOffset);

        //second get all keys of the old bucket plus the new key
        //also empty the old bucket
        int keys[RECORDSPERBUCKET + 1];
        for (size_t i = 0; i < RECORDSPERBUCKET; i++)
        {
            keys[i] = bucket.dataItem[i].key;
            bucket.dataItem[i].valid = 0;
        }
        bucket.depth += 1;
        //add modified bucket to the file
        pwrite(fbh, &bucket, sizeof(Bucket), bucketRecord.offset);
        //add the new key to the list of keys

        keys[RECORDSPERBUCKET] = key;

        //third double directory size
        Directory newDirectory;
        // increase depth by 1
        newDirectory.depth = d.depth + 1;
        newDirectory.records = new DirectoryRecord[(int)pow(2, newDirectory.depth)];

        //handle the pointers
        int dSize = (int)pow(2, d.depth);
        for (int i = 0; i < dSize; i++)
        {
            if (d.records[i].id != bucketRecordId) // not the bucket we working with
            {
                newDirectory.records[2 * i].offset = d.records[i].offset;
                newDirectory.records[2 * i].id = d.records[i].id * 2;

                newDirectory.records[2 * i + 1].offset = d.records[i].offset;
                newDirectory.records[2 * i + 1].id = d.records[i].id * 2 + 1;
            }
            else
            {
                newDirectory.records[2 * i].offset = d.records[i].offset;
                newDirectory.records[2 * i].id = d.records[i].id * 2;

                newDirectory.records[2 * i + 1].offset = newBucketOffset;
                newDirectory.records[2 * i + 1].id = d.records[i].id * 2 + 1;
            }
        }

        // fourth write the new directory in the file
        ssize_t r = pwrite(fdh, &newDirectory, sizeof(Directory), 0);

        //fifth reinsert the keys
        for (int i = 0; i < RECORDSPERBUCKET + 1; i++)
        {
            insertItem(fdh, fbh, keys[i]);
        }
    }
    else
    { // no need for directory expansion

        // first allocate the new bucket
        Bucket newBucket;
        newBucket.depth = bucket.depth + 1;
        newBucket.valid = 1;
        for (size_t i = 0; i < RECORDSPERBUCKET; i++)
        {
            DataItem d;
            d.valid = 0;
            d.key = -1;
            newBucket.dataItem[i] = d;
        }

        pwrite(fbh, &newBucket, sizeof(Bucket), newBucketOffset);

        //second get all keys of the old bucket plus the new key
        //also empty the old bucket
        int keys[RECORDSPERBUCKET + 1];
        for (size_t i = 0; i < RECORDSPERBUCKET; i++)
        {
            keys[i] = bucket.dataItem[i].key;
            bucket.dataItem[i].valid = 0;
        }
        bucket.depth += 1;
        //add modified bucket to the file
        pwrite(fbh, &bucket, sizeof(Bucket), bucketRecord.offset);
        //add the new key to the list of keys
        keys[RECORDSPERBUCKET] = key;

        //third adjust the directory file: adjust the offset value of the records

        // number of recordes point to the bucket
        // # = 2^(global depth - local depth (old one))
        int recordsNum = (int)pow(2, (d.depth - (bucket.depth - 1)));
        bool firstHalfOfRecords = true;
        int aux = 0; //to check if i finished the fisrt half
        for (int i = 0; i < (int)pow(2, d.depth); i++)
        {
            if (aux >= recordsNum / 2)
            {
                firstHalfOfRecords = false;
            }
            if (aux >= recordsNum)
            {
                break;
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
        for (int i = 0; i < RECORDSPERBUCKET + 1; i++)
        {
            insertItem(fdh, fbh, keys[i]);
        }
    }

    return 1;
}

// Search for a key
/* fdh: file handle of directory file
   fbh: file handle of buckets file
   key: key to be searched for 

   returns <Id of directory pointing to the bucket, offset of bucket, index of data item in the array>
   else <-1, -1, -1>;
*/
vector<int> searchItem(int fdh, int fbh, int key)
{
    // hash the key
    int newKey = modeHashCode(key);
    // cout << "newkey = " + to_string(newKey) + "\n";
    //ReadDirectory
    Directory d;
    vector<int> p;

    ssize_t result = pread(fdh, &d, sizeof(Directory), 0);
    if (result <= 0) //either an error happened in the pread or it hit an unallocated space
    {                // perror("some error occurred in pread");
        return p;
    }
    // get key in bit representation
    int *keyInBits = decToBinary(newKey);
    // get the bucket that the key will be inserted into

    //get the bucket record in the directory table
    int bucketRecordId = binaryToDecimal(keyInBits, d.depth);

    // cout << "record id = " + to_string(bucketRecordId) + "\n";

    DirectoryRecord bucketRecord = d.records[bucketRecordId];

    //get the bucket
    Bucket bucket;
    result = pread(fbh, &bucket, sizeof(Bucket), bucketRecord.offset);

    for (size_t i = 0; i < RECORDSPERBUCKET; i++)
    { // Check if this is the key we are searching for
        if (bucket.dataItem[i].valid == 1 && bucket.dataItem[i].key == key)
        {
            p.push_back(bucketRecordId);      //id of directory
            p.push_back(bucketRecord.offset); //bucket offset holding the record
            p.push_back(i);                   // index of the record in the data items array

            return p;
        }
    }

    p.push_back(-1);
    p.push_back(-1);
    p.push_back(-1);

    return p;
}

/* Function to colapse the directory file if possible 
   fdh: file handle of directory file

   updates the directory file after decreasing the depth
   and returns the new global depth
*/

void searchResult(int fdh, int fbh, int key)
{
    // search for the given key
    vector<int> searchedKey = searchItem(fdh, fbh, key);

    if (searchedKey[0] == -1) // check that key exist
    {
        // key doesnot exist
        cout << " item with Key ->  " + to_string(key) + " is not found\n";
        return;
    }

    // get key location
    int dirId = searchedKey[0], offset = searchedKey[1], bucketIdx = searchedKey[2];

    cout << " item with Key ->  " + to_string(key) + " is found at directory = " << dirId << "  offset = " << offset << "  bucket Index = " << bucketIdx << endl;
}

int colapseDirectory(int fdh)
{
    Directory read_dir_data;
    Directory write_dir_data;
    pread(fdh, &read_dir_data, sizeof(Directory), 0);
    int new_depth = read_dir_data.depth;
    if (new_depth == 1)
        return 1;
    int size = (int)pow(2, read_dir_data.depth);
    bool can_colapse = true;
    for (int i = 0; i < size / 2; i++)
    {
        if (read_dir_data.records[2 * i].offset != read_dir_data.records[2 * i + 1].offset)
        {
            can_colapse = false;
            break;
        }
    }

    if (can_colapse)
    {
        new_depth = read_dir_data.depth - 1;

        write_dir_data.records = new DirectoryRecord[size / 2];
        for (int i = 0; i < size / 2; i++)
        {
            DirectoryRecord record;
            record.id = read_dir_data.records[2 * i].id / 2;
            record.offset = read_dir_data.records[2 * i].offset;
            write_dir_data.records[i] = record;
        }

        write_dir_data.depth = new_depth;

        int result_w = pwrite(fdh, &write_dir_data, sizeof(Directory), 0);
        if (result_w < sizeof(Directory))
        {
            perror("some error occurred in pwrite while wrting the new directory file.");
        }
    }

    return new_depth;
}

// delete a key if exist
/* fdh: file handle of directory file
   fbh: file handle of buckets file
   key: key to be searched for 

   returns -1 if key not found
   else 1;
*/
int deleteItem(int fdh, int fbh, int key)
{
    // search for the given key
    vector<int> searchedKey = searchItem(fdh, fbh, key);

    if (searchedKey[0] == -1) // check that key exist
    {
        // key doesnot exist
        cout << " Delete failed ....  Key ->  " + to_string(key) + " not found\n";
        return -1;
        // cout << " Key ->  " + to_string(key) + " deleted successfully\n ";
    }

    // get key location
    int dirId = searchedKey[0], offset = searchedKey[1], bucketIdx = searchedKey[2];

    // get the bucket
    Bucket bucket;
    ssize_t result = pread(fbh, &bucket, sizeof(Bucket), offset);

    // mark data item as invlid
    bucket.dataItem[bucketIdx].valid = 0;

    // write the change in bucket
    pwrite(fbh, &bucket, sizeof(Bucket), offset);

    cout << " Key ->  " + to_string(key) + " deleted successfully\n ";

    // check if buckets can be merged

    // 1 - get the remaining keys in the first bucket

    vector<int> keysINbuckets;
    int count = 0;
    for (size_t i = 0; i < RECORDSPERBUCKET; i++)
    {
        if (bucket.dataItem[i].valid == 1)
        {
            count++;
            keysINbuckets.push_back(bucket.dataItem[i].key);
        }
    }
    /*
    // if (count == 0)
    // {
    //     // if bucket become empty delete bucket and adjust directory point to its budy bucket
    //     bucket.valid = 0;
    //     pwrite(fbh, &bucket, sizeof(Bucket), offset);
    //     int budyBucket = getBudyBucket(dirId, bucket.depth);

    //     Directory d;

    //     ssize_t result = pread(fdh, &d, sizeof(Directory), 0);
    //     if (result <= 0) //either an error happened in the pread or it hit an unallocated space
    //     {                // perror("some error occurred in pread");
    //         return -3;
    //     }

    //     // change offset of the dir that point to the deleted record

    //     Directory newDirectory;
    //     newDirectory.depth = d.depth;
    //     newDirectory.records = d.records;

    //     cout << "first offset " << newDirectory.records[dirId].offset << "   second offset " << newDirectory.records[budyBucket].offset << endl;

    //     newDirectory.records[dirId].offset = newDirectory.records[budyBucket].offset;

    //     ssize_t r = pwrite(fdh, &newDirectory, sizeof(Directory), 0);

    //     colapseDirectory(fdh);
    // }
    //else // check if this bucket can be merged with its budy bucket
    */

    // 2- get budy bucket keys

    int budyBucket = getBudyBucket(dirId, bucket.depth);
    Directory d;

    result = pread(fdh, &d, sizeof(Directory), 0);
    if (result <= 0) //either an error happened in the pread or it hit an unallocated space
    {                // perror("some error occurred in pread");
        return -3;
    }
    int budyBucketOffset = d.records[budyBucket].offset;

    // get the bucket
    Bucket bucket2;
    result = pread(fbh, &bucket2, sizeof(Bucket), budyBucketOffset);

    for (int i = 0; i < RECORDSPERBUCKET; i++)
    {
        if (bucket2.dataItem[i].valid == 1)
        {
            keysINbuckets.push_back(bucket2.dataItem[i].key);
        }
    }

    // 3- check if the total keys is smaller than or equal the allowed records in bucket

    if (keysINbuckets.size() <= RECORDSPERBUCKET)
    {
        // 4 - merge buckets and delete first bucket

        bucket.valid = 0;
        pwrite(fbh, &bucket, sizeof(Bucket), offset);

        Directory d;

        result = pread(fdh, &d, sizeof(Directory), 0);
        if (result <= 0) //either an error happened in the pread or it hit an unallocated space
        {                // perror("some error occurred in pread");
            return -3;
        }

        // 5 - change offset of the dir that point to the deleted record to the budy bucket

        Directory newDirectory;
        newDirectory.depth = d.depth;
        newDirectory.records = d.records;

        newDirectory.records[dirId].offset = newDirectory.records[budyBucket].offset;

        // 6- change depth of the body bucket and merge content

        bucket2.depth--;

        for (int i = 0; i < RECORDSPERBUCKET; i++)
        {
            if (bucket2.dataItem[i].valid == 1)
            {
                bucket2.dataItem[i].valid = 0;
            }
        }

        for (int i = 0; i < keysINbuckets.size(); i++)
        {

            bucket2.dataItem[i].valid = 1;
            bucket2.dataItem[i].key = keysINbuckets[i];
        }

        // 7 -  write the change in the file

        pwrite(fbh, &bucket2, sizeof(Bucket), budyBucketOffset);
        ssize_t r = pwrite(fdh, &newDirectory, sizeof(Directory), 0);
    }

    // 8 - check if I can collapse the directory file

    colapseDirectory(fdh);

    return 1;
}

int DisplayDirectoriesFile(int fdh, int fbh)
{

    Directory d;
    pread(fdh, &d, sizeof(Directory), 0);
    cout << "Global Depth = " << d.depth << endl;
    cout << "Directory size = " << (int)pow(2, d.depth) << endl;
    for (int i = 0; i < (int)pow(2, d.depth); i++)
    {
        // print each record
        cout << "Record ID = " << d.records[i].id << ";  Bucket Offset = " << d.records[i].offset << endl;

        Bucket b;
        pread(fbh, &b, sizeof(Bucket), d.records[i].offset);

        cout << "Bucket depth = " << b.depth << endl;
        for (int j = 0; j < RECORDSPERBUCKET; j++)
        {
            if (b.dataItem[j].valid == 1)
            {
                cout << b.dataItem[j].key << " ";
            }
        }
        cout << endl;
    }
}