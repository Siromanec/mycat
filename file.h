//
// Created by ADMIN on 23-Sep-23.
//

#ifndef MYCAT_FILE_H
#define MYCAT_FILE_H

#include <iostream>
#include <sys/stat.h> // Ñòðóêòóðè (òèïè) äëÿ fstat(), lstat (), stat ().
#include <fcntl.h> // Òèïè i êîíñòàíòè äëÿ fcntl() i open().
#include <unistd.h> // POSIX header.
#include <stdlib.h>
#include <ctype.h>

#include <vector>
#include <sstream>

class file {
    size_t fd;
    ssize_t total_read_bytes;
    ssize_t total_written_bytes;


public:
    file(const std::string &file_name, int mode);
    file(size_t fd): fd{fd}, total_read_bytes{0}{};


    ~file();

    int read(std::string & buff, size_t buffer_size, int *status, size_t *true_buff_size);
    int write(const std::string & buff, size_t buffer_size, int *status, size_t true_buff_size);

    bool file_was_read();


};

#endif //MYCAT_FILE_H