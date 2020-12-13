/*
 * readfile.h
 *
 *  Created on: Sep 20, 2016
 *      Author: dina
 */

#ifndef READFILE_H_
#define READFILE_H_

#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <vector>

using namespace std;

#define BITSCOUNT 5        //Maximum number of bits any number will be represented with
#define HASHNUMBER 32                      //Number of BUCKETS
#define RECORDSPERBUCKET 2                //No. of records inside each Bucket
#define BUCKETSIZE sizeof(Bucket)         //Size of the bucket (in bytes)
#define FILESIZE BUCKETSIZE *MBUCKETS     //+ OVERFLOWLISTSIZE*sizeof(DataItem)*MBUCKETS // size of file
#define NULL 0   // predefined null;                                                            

//Data Record inside the file
struct DataItem
{
   int valid; //0 means invalid record, 1 = valid record
   int key;
   int nextOffset = NULL;
};

//Each bucket contains number of records
struct Bucket
{
   int valid; //0 means invalid bucket, 1 = valid bucket 
   int depth;
   struct DataItem dataItem[RECORDSPERBUCKET];
};


struct DirectoryRecord
{
   int id; //record id
   int offset; //in data file
};

struct Directory
{
   int depth; //Global depth
   vector<DirectoryRecord> records;
};



//Check the create File
int createFile(int size, char *, int type);//type 0: Directory, type 1: data file

//check the extendibleHashing.cpp File
int deleteItem(int fdh, int fbh, int key);
int insertItem(int fdh, int fbh, int key);
int DisplayBucketsFile(int fbh);
int DisplayDirectoriesFile(int fdh);
int searchItem(int fdh, int fbh, int key, int *count);

#endif /* READFILE_H_ */
