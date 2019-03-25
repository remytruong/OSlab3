
/*
*
*	The Kernel
*
*/

#include <stddef.h>
#include <stdbool.h>
#include <stdint.h>
#include <ctype.h>

#include "kernel.h"
#include "hal/hal.h"
#include "string.h"

#include "drivers/stdio/emb-stdio.h"			// Needed for printf
#include "drivers/sdcard/SDCard.h"

#include "console/console.h"

void kernel_init(void);
void input_output_init(void);
char buffer[500];

/*
 *		Kernel's entry point
**/
void main(uint32_t r0, uint32_t r1, uint32_t atags){

  //Init
  kernel_init();
  input_output_init();
  sdInitCard (&printf_serial, &printf_serial, true);

//  run(buffer);

  char input[64];
  int i = 0;
  char folder[300];
  strcpy(folder, "");
  char dest[200];
  //Welcome Msg Video
  hal_io_video_puts( "\n\r\n\rWelcome to MiniOS Pi Zero\n\r", 3, VIDEO_COLOR_GREEN );
  hal_io_serial_puts( SerialA, "\n\r\n\rWelcome to MiniOS Pi Zero\n\r" );
  hal_io_video_puts( "\n\r$ ", 2, VIDEO_COLOR_GREEN );
  printf_video("%s ", folder);
  printf_serial("\n\r$ %s ", folder);
  char runnable[4] = ".bin";
  uint8_t c;

    while (1){
        c = hal_io_serial_getc( SerialA );
        if(c == '\r') {
            input[i++] = '\0';
			char* token = strtok(input, " ");
			if(!strcmp(input, "evit")) {
			    runTextEditor(c);
			}
			if (!strcmp(token, "cat")) {
                printText(buffer, token, dest);
			}
			if (!strcmp(input, "cd")) {
			    changeDirectory(token, dest);
			}
            if(!strcmp(input, "ls")) {
                DisplayDirectory(folder, dest);
            }
            if(!strcmp(input, "sysinfo")) {
                sys_info(OS_NAME);
                sys_info(OS_VERSION);
            }
            if(!strcmp(input, "dump")) {
                printBinary(buffer, token, dest);
            }
            else if(!strcmp(&input[strlen(input)-4], runnable)) {
                runFile(buffer, input);
            }
            hal_io_video_puts( "\n\r$ ", 2, VIDEO_COLOR_GREEN );
			printf_video("%s ", folder);
            printf_serial("\n\r$ %s ", folder );
            i = 0;
        }
        else {
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
