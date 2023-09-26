// This is a personal academic project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++, C#, and Java: http://www.viva64.com


#ifndef MYCAT_FILE_POSIX_H
#define MYCAT_FILE_POSIX_H

#include "Ifile.h"
class file_posix: public Ifile {
    int fd;
    int interpret_mode(open_mode mode) const;

public:
    explicit file_posix(int fd): fd{fd} {};
    file_posix (const std::string &file_name, open_mode mode);
    ~file_posix();
    virtual int read(std::string &buff, size_t buffer_max_size, int *status, size_t *true_buff_size) override;
    virtual int write(const std::string &buff, size_t buffer_max_size, int *status, size_t *true_buff_size) override;
};


#endif //MYCAT_FILE_POSIX_H
