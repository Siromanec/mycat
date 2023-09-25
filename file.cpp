//
// Created by ADMIN on 23-Sep-23.
//

#include "file.h"

#include <iostream>
#include <sys/stat.h> // Ñòðóêòóðè (òèïè) äëÿ fstat(), lstat (), stat ().
#include <fcntl.h> // Òèïè i êîíñòàíòè äëÿ fcntl() i open().
#include <unistd.h> // POSIX header.
#include <stdlib.h>
#include <ctype.h>

#include <vector>
#include <sstream>

file::file(const std::string &file_name, int mode) : total_read_bytes{0} {
    do {
        fd = open(file_name.c_str(), mode);

        if (fd == -1) {
            if (errno == EINTR) {
                continue;
            }
            if (errno == ENOENT) {
                std::cerr << "file '" << file_name << "' does not exist. Aborting..." << std::endl;
                throw std::exception();

            } else if (errno != 0) {
                std::cerr << "Error opening file '" << file_name << "'. Aborting..." << std::endl;
                throw std::exception();

            }

        }
        break;
    } while (true);
}

file::~file() {
    if (fd != STDIN_FILENO && fd != STDOUT_FILENO && fd != STDERR_FILENO)
        close(fd);
}

int file::read(std::string &buff, size_t buffer_max_size, int *status, size_t *true_buff_size) {
    ssize_t read_bytes = 0;

    size_t size = buffer_max_size - *true_buff_size;
    ssize_t read_now = 0;
    do {
        read_now = ::read(fd, buff.data() + *true_buff_size + read_bytes, // the size shouldn't change up until the end of the function
                                  size - read_bytes);
        if (read_now == -1) {
            if (errno == EINTR)
                continue;
            else {
                *status = errno;
                return -1;
            }
        } else
            read_bytes += read_now;
    }
    while (read_bytes < size && read_now != 0); // i hope read now == 0 means that it has finished reading file and not some quirky undefined behavior
//    buff.resize(buff.size() + read_bytes);
//buff.resize()
    *true_buff_size += read_bytes;
    total_read_bytes += read_bytes;
    return 0;
}
int file::write(const std::string &buff, size_t buffer_max_size, int *status, size_t true_buff_size) {
    ssize_t written_bytes = 0;

    size_t size = true_buff_size;
    ssize_t written_now = 0;
    while (written_bytes  < size) {
        written_now = ::write(fd, buff.data() + written_bytes ,
                                  size - written_bytes );
        if (written_now == -1) {
            if (errno == EINTR)
                continue;
            else {
                *status = errno;
                return -1;
            }
        } else
            written_bytes  += written_now;
    }
//    *true_buff_size += written_bytes ;
    return 0;
}
;