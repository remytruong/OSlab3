
/*
*
*	The Kernel
*
*/

#include <stddef.h>
#include <stdbool.h>
#include <stdint.h>

#include "kernel.h"
#include "hal/hal.h"
#include "string.h"

#include "drivers/stdio/emb-stdio.h"			// Needed for printf
#include "drivers/sdcard/SDCard.h"

void kernel_init(void);
void input_output_init(void);
void sys_info( uint8_t* );
void sd_card_fs_demo();

char buffer[500];
void DisplayDirectory(const char*);

/*
 *		Kernel's entry point
**/
void main(uint32_t r0, uint32_t r1, uint32_t atags){

  //Init
  kernel_init();
  input_output_init();
  sdInitCard (&printf_serial, &printf_serial, true);

//  sd_card_fs_demo();   //<<-- Uncomment this to show File System/SD Card demo

  //Welcome Msg Video
  hal_io_video_puts( "\n\r\n\rWelcome to MiniOS Pi Zero\n\r", 3, VIDEO_COLOR_GREEN );
  hal_io_serial_puts( SerialA, "\n\r\n\rWelcome to MiniOS Pi Zero\n\r" );
  hal_io_video_puts( "\n\r$ ", 2, VIDEO_COLOR_GREEN );
  hal_io_serial_puts( SerialA, "\n\r$ " );

  uint8_t c;

  char input[64];
  int i = 0;

    while (1){
        c = hal_io_serial_getc( SerialA );
        if(c == '\r') {
            input[i++] = '\0';
			char* token = strtok(input, " ");
			if (!strcmp(token, "cat")) {
				token = strtok(NULL, " ");
				printf_serial("\n\r%s", token);
				printf_serial("\n\n");
				printf_serial("Opening %s \n", token);

				HANDLE fHandle = sdCreateFile(token, GENERIC_READ, 0, 0, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, 0);
				if (fHandle != 0) {
					uint32_t bytesRead;

					if ((sdReadFile(fHandle, &buffer[0], 500, &bytesRead, 0) == true)) {
						buffer[bytesRead - 1] = '\0';  ///insert null char
						printf_serial("File Contents: %s", &buffer[0]);
						printf_video("\n\r%s\n\r", &buffer[0]);
					}
					else {
						printf_serial("Failed to read");
					} 

					// Close the file
					sdCloseHandle(fHandle);

				}
				else {
					printf_serial("Unable to find file %s", token);
					printf_video("Unable to find file %s", token);
				}
				printf_serial("\n");
			}
			if (!strcmp(input, "cd")) {
				printf_serial("yeet");
			}
            if(!strcmp(input, "ls")) {
//                printf_serial("\n%s\n", "LS called");
                /* Display root directory */
                printf_serial("\n\nDirectory (/): \n");
                printf_video("\n\r");
                DisplayDirectory("\\*.*");
            //} else if (!strcmp(input, "cd")) {
                //printf_serial("\n\n%s\n", "CD called");
            } else if(!strcmp(input, "sysinfo")) {
                printf_serial("\n\n%s\n", "OS Name: O OS\nVersion: 0.0");
                printf_video("\n\r%s\n\r", "OS Name: O OS\n\rVersion: 0.0");
//                printf_serial("\n\n");
//                sdInitCard (&printf_serial, &printf_serial, true);
            } else if(!strcmp(input, "dump")) {
                printf_serial("\n\n%s\n", "DUMP called");
            };
            hal_io_video_puts( "\n\r$ ", 2, VIDEO_COLOR_GREEN );
            printf_serial("\n\r$ " );
            i = 0;
        } else {
            input[i++] = c;
            printf_video( "%c", c );  //<<--- We also have printfs
            printf_serial( "%c", c );
        }
    }

}

/*
* Initializes the kernel
*/
void kernel_init(void){

  hal_io_init();
  //console_init();
  //system_calls_init();
  //scheduler_init();
  //faults_init();

}

/*
* Initializes All IO
*/
void input_output_init(void){
  uint32_t video_init_res = HAL_FAILED;

#ifdef VIDEO_PRESENT
  video_init_res = hal_io_video_init();
#endif

#ifdef SERIAL_PRESENT
  hal_io_serial_init();
#endif
//NOTE: PAST THIS POINT WE CAN USE printf
//      (printf needs both serial and video inits to be called first)

  if ( video_init_res == HAL_SUCCESS )
    sys_info( "Video Initialized\n\r" );
  else
    sys_info( "Video Init Failed [x]\n\r" );

    sys_info( "Serial Initialized\n\r" );
}

void sys_info( uint8_t* msg ){
  printf_video( msg );
  printf_serial( msg );
}

/////////////////////////////////////////////////////////////////
////////////////    D E M O    C O D E    ///////////////////////
/////////////////////////////////////////////////////////////////

//char buffer[500];
//void DisplayDirectory(const char*);


void sd_card_fs_demo(){
  printf_serial("\n\n");
  sdInitCard (&printf_serial, &printf_serial, true);

  /* Display root directory */
  printf_serial("\n\nDirectory (/): \n");
  DisplayDirectory("\\*.*");

  printf_serial("\n");
  printf_serial("Opening Alice.txt \n");

  HANDLE fHandle = sdCreateFile("Alice.txt", GENERIC_READ, 0, 0, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, 0);
  if (fHandle != 0) {
    uint32_t bytesRead;

    if ((sdReadFile(fHandle, &buffer[0], 500, &bytesRead, 0) == true))  {
        buffer[bytesRead-1] = '\0';  ///insert null char
        printf_serial("File Contents: %s", &buffer[0]);
    }
    else{
      printf_serial("Failed to read" );
    }

    // Close the file
    sdCloseHandle(fHandle);

  }

}

void DisplayDirectory(const char* dirName) {
	HANDLE fh;
	FIND_DATA find;
	char* month[12] = { "Jan", "Feb", "Mar", "Apr", "May", "Jun", "Jul", "Aug", "Sep", "Oct", "Nov", "Dec" };
	fh = sdFindFirstFile(dirName, &find);							// Find first file
	do {
		if (find.dwFileAttributes == FILE_ATTRIBUTE_DIRECTORY)
			printf_serial("%s <DIR>\n", find.cFileName);
		else {
            printf_serial("%c%c%c%c%c%c%c%c.%c%c%c Size: %9lu bytes, %2d/%s/%4d, LFN: %s\n",
                  find.cAlternateFileName[0], find.cAlternateFileName[1],
                  find.cAlternateFileName[2], find.cAlternateFileName[3],
                  find.cAlternateFileName[4], find.cAlternateFileName[5],
                  find.cAlternateFileName[6], find.cAlternateFileName[7],
                  find.cAlternateFileName[8], find.cAlternateFileName[9],
                  find.cAlternateFileName[10],
                  (unsigned long) find.nFileSizeLow,
                  find.CreateDT.tm_mday, month[find.CreateDT.tm_mon],
                  find.CreateDT.tm_year + 1900,
                  find.cFileName);                                        // Display each entry
            printf_video("%s\n\r",
//                          find.cAlternateFileName[0], find.cAlternateFileName[1],
//                          find.cAlternateFileName[2], find.cAlternateFileName[3],
//                          find.cAlternateFileName[4], find.cAlternateFileName[5],
//                          find.cAlternateFileName[6], find.cAlternateFileName[7],
//                          find.cAlternateFileName[8], find.cAlternateFileName[9],
//                          find.cAlternateFileName[10],
//                          (unsigned long) find.nFileSizeLow,
//                          find.CreateDT.tm_mday, month[find.CreateDT.tm_mon],
//                          find.CreateDT.tm_year + 1900,
                          find.cFileName);
        }
		} while (sdFindNextFile(fh, &find) != 0);						// Loop finding next file
	sdFindClose(fh);												// Close the serach handle
}
