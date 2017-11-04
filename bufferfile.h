#ifndef BUFFERFILE_H
#define BUFFERFILE_H
#include <fstream>
#include <iostream>
#include <stdio.h>


class BufferFile
{
public :
    std::string file_path_;
    int length_;
    char* buffer_;
    explicit BufferFile();
    void load(std::string file_path);
    int GetLength();
    char* GetBuffer();
    ~BufferFile() {
        if (buffer_) {
            delete[] buffer_;
            buffer_ = NULL;
        }
    }
};

#endif // BUFFERFILE_H
