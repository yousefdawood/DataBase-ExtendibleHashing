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

#define BITSCOUNT 5        //Maximum number of bits any number will be represented with
#define HASHNUMBER 10                       //Number of BUCKETS
#define RECORDSPERBUCKET 2                //No. of records inside each Bucket
#define BUCKETSIZE sizeof(Bucket)         //Size of the bucket (in bytes)
#define FILESIZE BUCKETSIZE *MBUCKETS     //+ OVERFLOWLISTSIZE*sizeof(DataItem)*MBUCKETS // size of file
#define NULL 0   // predefined null;                                                            

//Data Record inside the file
struct DataItem
{
   int valid; //) means invalid record, 1 = valid record
   int data;
   int nextOffset = NULL;
};

//Each bucket contains number of records
struct Bucket
{
   struct DataItem dataItem[RECORDSPERBUCKET];
};

//Check the create File
int createFile(int size, char *);

//check the extendibleHashing.cpp File
int deleteItem(int fdh, int fbh, int key);
int insertItem(int fdh, int fbh, DataItem item);
int DisplayBucketsFile(int fbh);
int DisplayDirectoriesFile(int fdh);
int searchItem(int fdh, int fbh, int key, int *count);

#endif /* READFILE_H_ */
