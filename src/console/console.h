#ifndef CONSOLE_H
#define CONSOLE_H

//void run(char*);
void sys_info( uint8_t*);
void DisplayDirectory(char*, char*);
void changeDirectory(const char* token, char* dest);
void printText(char*, const char*, char*);
void printBinary(char*, const char*, char*);
void runFile(char*, char*);
void runTextEditor(uint8_t);
#endif