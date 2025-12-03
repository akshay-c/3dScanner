#ifndef __FILE_HANDLING_H
#define __FILE_HANDLING_H

extern char    *ResultFileName;

void WriteHeader(char* Name, unsigned long vertex, unsigned long face);
void FileWriteLine(const void *bytes, int len);
void FileWriteLine2(const void *bytes, int len);
void FileWrite(const void *bytes, int len);
#endif