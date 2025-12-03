#include <opencv2/opencv.hpp>
#include <gtk/gtk.h>
#include <stdio.h>
#include <string.h>

#include "filehandling.hpp"
#include "main.hpp"

char    *ResultFileName = (char*)"Output.ply";
char    *Format = (char*)"ascii 1.0";
char    *Comment = (char*)"Created by 3dScanner";
FILE    *fd,*fd2;
std::string data;

void FileWrite(const void *bytes, int len)
{
    fwrite(bytes, sizeof(char), len, fd);
}

void FileWriteLine(const void *bytes, int len)
{
    char NewLine = '\n';
    fwrite(bytes, sizeof(char), len, fd);
    fwrite(&NewLine, sizeof(char), 1, fd);
    printf("writing %s\n",(char*)bytes);
}
void FileWriteLine2(const void *bytes, int len)
{
    char NewLine = '\n';
    fwrite(bytes, sizeof(char), len, fd2);
    fwrite(&NewLine, sizeof(char), 1, fd2);
    printf("writing %s\n",(char*)bytes);
}

void WriteHeader(char* Name, unsigned long vertex, unsigned long face)
{
    fd = fopen(Name, "w");
    fd2 = fopen("Reference.txt","w");
    time_t tm;
    time( &tm );
    struct tm *t2 = localtime( &tm );
    char buf[1024];
    std::string temp;
    data = "ply";
    FileWriteLine(data.c_str(), data.size());
    data = "format ascii 1.0";
    FileWriteLine(data.c_str(), data.size());
    data = "comment made using 3dScanner";
    FileWriteLine(data.c_str(), data.size());
    data = "comment FileName: ";
    FileWrite(data.c_str(), data.size());
    data = ResultFileName;
    FileWriteLine(data.c_str(), data.size());
    data = "comment DateCreated: ";
    FileWrite(data.c_str(), data.size());
    strftime( buf, sizeof(buf)-1, "%c", t2);
    data = buf;
    FileWriteLine(data.c_str(), data.size());
    
    sprintf(buf, "element vertex %ld",vertex );
    data = buf;
    FileWriteLine(data.c_str(), data.size());
    data = "property double x";
    FileWriteLine(data.c_str(), data.size());
    data = "property double y";
    FileWriteLine(data.c_str(), data.size());
    data = "property double z";
    FileWriteLine(data.c_str(), data.size());

    sprintf(buf, "element face %ld", face);
    data = buf;
    FileWriteLine(data.c_str(), data.size());

    data = "property list int int vertex_indices";
    FileWriteLine(data.c_str(), data.size());
    data = "end_header";
    FileWriteLine(data.c_str(), data.size());

    // fs << "ply";
    // fs << "format" << Format;
    // fs << "comment" << Comment;
    // fs << "comment File Name" << Name;
}