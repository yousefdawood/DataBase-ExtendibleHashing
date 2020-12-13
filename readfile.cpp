#include "readfile.h"

/* Create Database file or Open it if it already exists
 * Input:  
 *          size: the no. of bytes that should be allocated to the file
 *          name: the name o the file
 * Output: return the file handler on success or -1 on failure
 */
int createFile(int size,char * name, int type)
{

    int fd;
	int result;
    struct stat sb;

    if (stat(name, &sb) == -1) {
         printf("file doesn't exist, create it\n");
    fd = open(name, O_RDWR | O_CREAT , (mode_t)0600);
    if (fd == -1) {
	perror("Error opening file for writing");
        return 1;
    }

    /* Stretch the file size.
     * Note that the bytes are not actually written,
     * and cause no IO activity.
     * Also, the diskspace is not even used
     * on filesystems that support sparse files.
     * (you can verify this with 'du' command)
     */
    result = lseek(fd, size-1, SEEK_SET);
    if (result == -1) {
	close(fd);
	perror("Error calling lseek() to 'stretch' the file");
        return 1;
    }
    
    /* write just one byte at the end */
    result = write(fd, "", 1);
    if (result < 0) {
		close(fd);
		perror("Error writing a byte at the end of the file");
			return -1;
    	}
    }
    
    if(type == 0)// Directory
    {
        Directory d;
        d.depth = 1;
        DirectoryRecord r;
        r.id = 0;
        r.offset = 0;
        d.records.push_back(r);
        DirectoryRecord r2;
        r2.id = 1;
        r2.offset = sizeof(Bucket);
        d.records.push_back(r2);
        pwrite(fd, &d, sizeof(Directory), 0);
    }
    if (type == 1)// data file
    {
        Bucket b1;
        b1.valid = 1;
        b1.depth = 1;
        pwrite(fd, &b1, sizeof(Bucket),0);
        pwrite(fd, &b1, sizeof(Bucket),sizeof(Bucket));
    }
    


    else  {//file exists
    	//| O_EXCL
    	    fd = open(name, O_RDWR  , (mode_t)0600);
    	    if (fd == -1) {
    		perror("Error opening file for writing");
    	        return -1;
    	    }
    }
    return fd;
}

