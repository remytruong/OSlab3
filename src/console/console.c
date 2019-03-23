#include <stddef.h>
#include <stdbool.h>
#include <stdint.h>
#include <ctype.h>

#include "string.h"

#include "../kernel.h"

#include "../hal/hal.h"

#include "../drivers/stdio/emb-stdio.h"			// Needed for printf
#include "../drivers/sdcard/SDCard.h"

#include "console.h"

//void run()
//{
//    char input[64];
//    int i = 0;
//    char folder[300];
//    strcpy(folder, "");
//    char dest[200];
//    //Welcome Msg Video
//    hal_io_video_puts( "\n\r\n\rWelcome to MiniOS Pi Zero\n\r", 3, VIDEO_COLOR_GREEN );
//    hal_io_serial_puts( SerialA, "\n\r\n\rWelcome to MiniOS Pi Zero\n\r" );
//    hal_io_video_puts( "\n\r$ ", 2, VIDEO_COLOR_GREEN );
//    printf_video("%s ", folder);
//    printf_serial("\n\r$ %s ", folder);
//    char runnable[4] = ".bin";
//    uint8_t c;
//
//    while (1){
//        c = hal_io_serial_getc( SerialA );
//        if(c == '\r') {
//            input[i++] = '\0';
//            char* token = strtok(input, " ");
//
//            if(!strcmp(input, "evit")) {
//                char buffer[32767];
//                int cursor = 0;
//                int eof = 1;
//                buffer[cursor] = '_';
//                buffer[eof] = '\0';
//                hal_io_clear_screen();
//                printf_video(buffer);
//
//                int terminate = 0;
//                while(!terminate) {
//                    c = hal_io_serial_getc( SerialA );
//                    if (c == 27) {
//                        c = hal_io_serial_getc( SerialA );
//                        c = hal_io_serial_getc( SerialA );
//                        if (c == 'C') {
//                            printf_serial(" >");
//                            if (cursor < eof - 1) {
//                                buffer[cursor] = buffer[cursor+1];
//                                buffer[++cursor] = '_';
//                            }
//                        }
//                        else if (c == 'D') {
//                            printf_serial(" <");
//                            if (cursor > 0) {
//                                buffer[cursor] = buffer[cursor-1];
//                                buffer[--cursor] = '_';
//                            }
//                        }
//                        else {
//                            terminate = 1;
//                        }
//                    }
//                    else if (c == 127) {
//                        printf_serial(" BS");
//                        for(int it = cursor; it <= eof; ++it) {
//                            buffer[it - 1] = buffer[it];
//                        }
//                        --cursor;
//                    }
//                    else {
//                        ++eof;
//                        for(int it = eof; it > cursor; --it) {
//                            buffer[it] = buffer[it-1];
//                        }
//                        buffer[cursor++] = c;
//                        if (c == '\r') {
//                            ++eof;
//                            for(int it = eof; it > cursor; --it) {
//                                buffer[it] = buffer[it-1];
//                            }
//                            buffer[cursor++] = '\n';
//                        }
//                        buffer[eof] = '\0';
//                    }
//                    hal_io_clear_screen();
//                    printf_video(buffer);
//                }
//            }
//
//            if (!strcmp(token, "cat")) {
//                token = strtok(NULL, " ");
//                printf_serial("\n\r%s", token);
//                printf_serial("\n\n");
//                printf_serial("Opening %s \n", token);
//                char temp[200];
//                strcpy(temp, dest);
//                strcat(temp, "\\");
//                strcat(temp, token);
//                HANDLE fHandle = sdCreateFile(temp, GENERIC_READ, 0, 0, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, 0);
//                if (fHandle != 0) {
//                    uint32_t bytesRead;
//
//                    if ((sdReadFile(fHandle, &buffer[0], 500, &bytesRead, 0) == true)) {
//                        buffer[bytesRead - 1] = '\0';  ///insert null char
//                        printf_serial("File Contents: %s", &buffer[0]);
//                        printf_video("\n\r%s\n\r", &buffer[0]);
//                    }
//                    else {
//                        printf_serial("Failed to read");
//                    }
//
//                    // Close the file
//                    sdCloseHandle(fHandle);
//
//                }
//                else {
//                    printf_serial("Unable to find file %s", token);
//                    printf_video("\n\r Unable to find file %s", token);
//                }
//                printf_serial("\n");
//            }
//
//            if (!strcmp(input, "cd")) {
//                token = strtok(NULL, " ");
//                if (!strcmp(token, "..")) {
//                    size_t startpoint = strlen(dest);
//                    for (size_t i = startpoint; i >= 0; --i) {
//                        if (dest[i] == '\\') {
//                            char test[500];
//                            strcpy(test, dest);
//                            strcpy(dest, "");
//                            for (size_t d = 0; d < i; d++) {
//                                dest[d] = test[d];
//                            }
//                            dest[i] = '\0';
//                            break;
//                        }
//                        if (i == 0) {
//                            strcpy(dest, "");
//                        }
//                    }
//                }
//                else {
//                    strcat(dest, "\\");
//                    strcat(dest, token);
//                    printf_serial("\n\n");
//                    printf_serial("cding to %s \n", token);
//                }
//            }
//
//            if(!strcmp(input, "ls")) {
//                printf_serial("\n\nDirectory (%s): \n", folder);
//                printf_video("\n\r");
//                char directory[500];
//                strcpy(directory, "");
//                strcat(directory, dest);
//                strcat(directory, "\\*.*");
//                DisplayDirectory(directory);
//            }
//
//            if(!strcmp(input, "sysinfo")) {
//                sys_info(OS_NAME);
//                sys_info(OS_VERSION);
//            }
//
//            if(!strcmp(input, "dump")) {
//                token = strtok(NULL, " ");
//                printf_serial("\n\r%s", token);
//                printf_serial("\n\n");
//                printf_serial("Dumping %s \n", token);
//
//                char temp[200];
//                strcpy(temp, dest);
//                strcat(temp, "\\");
//                strcat(temp, token);
//
//                HANDLE fHandle = sdCreateFile(temp, GENERIC_READ, 0, 0, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, 0);
//                if (fHandle != 0) {
//                    uint32_t bytesRead;
//
//                    if ((sdReadFile(fHandle, &buffer[0], 500, &bytesRead, 0) == true)) {
//                        buffer[bytesRead - 1] = '\0';  ///insert null char
//                        int bin_i = 0;
//
//                        while(buffer[bin_i] != '\0'){
//                            if(bin_i % 16 == 0) {
//                                printf_serial("\n");
//                                printf_video("\n\r");
//                            }
//                            printf_serial("%02x ", buffer[bin_i]);
//                            printf_video("%02x ", buffer[bin_i]);
//                            ++bin_i;
//                        }
//                    }
//                    else {
//                        printf_serial("Failed to read");
//                    }
//
//                    // Close the file
//                    sdCloseHandle(fHandle);
//
//                }
//                else {
//                    printf_serial("Unable to find file %s", token);
//                    printf_video("\n\r Unable to find file %s", token);
//                }
//                printf_serial("\n");
//            }
//            else if(!strcmp(&input[strlen(input)-4], runnable)) {
//                printf_serial("\n\n");
//                printf_serial("Running %s: \n", input);
//
//                HANDLE fHandle = sdCreateFile(input, GENERIC_READ, 0, 0, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, 0);
//                if (fHandle != 0) {
//                    uint32_t bytesRead;
//
//                    if ((sdReadFile(fHandle, &buffer[0], 500, &bytesRead, 0) == true)) {
//                        int ret = ((int(*)(void))(buffer))();
//                        printf_serial("%d",ret);
//                    }
//                    else {
//                        printf_serial("Failed to read");
//                    }
//
//                    // Close the file
//                    sdCloseHandle(fHandle);
//
//                }
//                else {
//                    printf_serial("Unable to find file %s", input);
//                    printf_video("\n\r Unable to find file %s", input);
//                }
//                printf_serial("\n");
//            }
//            hal_io_video_puts( "\n\r$ ", 2, VIDEO_COLOR_GREEN );
//            printf_video("%s ", folder);
//            printf_serial("\n\r$ %s ", folder );
//            i = 0;
//        }
//        else {
//            input[i++] = c;
//            printf_video( "%c", c );  //<<--- We also have printfs
//            printf_serial( "%c", c );
//        }
//
//    }
//}

void sys_info( uint8_t* msg ){
    printf_video( "\n\r%s\r", msg  );
    printf_serial( "\n\r%s\r", msg );
}

void DisplayDirectory(const char* dirName) {
    HANDLE fh;
    FIND_DATA find;
    char* month[12] = { "Jan", "Feb", "Mar", "Apr", "May", "Jun", "Jul", "Aug", "Sep", "Oct", "Nov", "Dec" };
    fh = sdFindFirstFile(dirName, &find);							// Find first file
    do {
        if (find.dwFileAttributes == FILE_ATTRIBUTE_DIRECTORY) {
            printf_serial("%s <DIR>\n", find.cFileName);
            printf_video("%s\n\r", find.cFileName);
        }
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
            printf_video("%s\n\r", find.cFileName);
        }
    } while (sdFindNextFile(fh, &find) != 0);						// Loop finding next file
    sdFindClose(fh);												// Close the serach handle
}