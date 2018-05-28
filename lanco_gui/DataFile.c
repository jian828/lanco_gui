#include <stdio.h>


#include <stdio.h>
#include <sys/types.h>
#include <fcntl.h>
#include <strings.h>
#include <stdlib.h>
#include <unistd.h>


//int main(int argc, char**argv)
int test_data_file_rw(void)
{
	int fd = -1;
	char buf[512] = {0};
	int read_len = 0;

	if (access("/data/lanco", 0) == -1) {
		if (mkdir("/data/lanco", 0766) == -1) {
			printf("===syw=== mkdir lanco failed.\n");
		} else {
			printf("===syw=== mkdir lanco succ.\n");
		}
	}

	if ((fd = open("/data/lanco/lanco_gui_test", O_RDWR | O_CREAT, 0666)) == -1) {
		printf("===syw=== open lanco_gui failed.\n");
		return 0;	
	} else {
		printf("===syw=== open lanco_gui succ fd= %d.\n", fd);
	}

	write(fd, "hello wrold", strlen("hello wrold")); //返回值是写入的字节数
	close(fd);

	//read
	if ((fd = open("/data/lanco/lanco_gui_test", O_RDWR | O_CREAT, 0666)) == -1) {
                printf("===syw=== open lanco_gui failed.\n");
                return 0;
        } else {
                printf("===syw=== open lanco_gui succ fd= %d.\n", fd);
        }
	memset(buf, 0, 512);
	read_len = read(fd, buf, 512);
	printf("===syw=== READ: len = %d, %s\n", read_len, buf);
	close(fd);

	return 0;
}



void gu_check_lanco_folder()
{
 	if (access("/data/lanco", 0) == -1) 
	{
		if (mkdir("/data/lanco", 0766) == -1) 
		{
			printf("mkdir lanco failed.\n");
		} 
		else 
		{
			printf("mkdir lanco succ.\n");
		}
	}
	else
	{
        printf("folder lanco is existed.\n");
	}
}
extern unsigned char g_ot_buffer[];
#define	TFNET_ID_MAX		 (125+2 +10) //otid 0 is not ever used.
#define	TFNET_FILE_SIZE  	 (TFNET_ID_MAX*1024)
unsigned char gu_load_data_from_flash()
{
	int fd = -1;
	memset(g_ot_buffer,0,TFNET_FILE_SIZE);
	int read_len = 0;


	//read
	if ((fd = open("/data/lanco/lanco_gui", O_RDWR | O_CREAT, 0666)) == -1) 
	{
        printf("ropen lanco_gui failed.\n");
        return 0;
    } 
	else 
	{
        printf("ropen lanco_gui succ fd= %d.\n", fd);
    }
	memset(g_ot_buffer,0,TFNET_FILE_SIZE);
	
	read_len = read(fd, g_ot_buffer, TFNET_FILE_SIZE);
	printf("READ: len = %d\n", read_len);
	close(fd);

	return read_len;
}


unsigned char gu_save_data_2_flash()
{
	int fd = -1;
	int write_len =0;


	if ((fd = open("/data/lanco/lanco_gui", O_RDWR | O_CREAT, 0666)) == -1) 
	{
		printf("wopen lanco_gui failed.\n");
		return 0;	
	} 
	else 
	{
		printf("wopen lanco_gui succ fd= %d.\n", fd);
	}

	write_len = write(fd, g_ot_buffer, TFNET_FILE_SIZE); //返回值是写入的字节数
	close(fd);


	
    return write_len;
}