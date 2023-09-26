//
// Created by ADMIN on 23-Sep-23.
//

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

    virtual size_t interpret_mode(open_mode mode) = 0;
public:

    virtual int read(std::string & buff, size_t buffer_max_size, int *status, size_t *true_buff_size) = 0;
    virtual int write(const std::string & buff, size_t buffer_max_size, int *status, size_t *true_buff_size) = 0;
//    virtual ~Ifile() = 0;

};

#endif //MYCAT_IFILE_H