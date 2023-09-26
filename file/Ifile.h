// This is a personal academic project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++, C#, and Java: http://www.viva64.com


#ifndef MYCAT_IFILE_H
#define MYCAT_IFILE_H

#include <iostream>
#include <ctype.h>

enum open_mode{
    FILE_READ,
    FILE_WRITE,
    FILE_READWRITE
};
class Ifile {

public:

    virtual int read(std::string & buff, size_t buffer_max_size, int *status, size_t *true_buff_size) = 0;
    virtual int write(const std::string & buff, size_t buffer_max_size, int *status, size_t *true_buff_size) = 0;
//    virtual ~Ifile() = 0;

    virtual ~Ifile() = default;


};

#endif //MYCAT_IFILE_H