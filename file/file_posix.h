//
// Created by ADMIN on 25-Sep-23.
//

#ifndef MYCAT_FILE_POSIX_H
#define MYCAT_FILE_POSIX_H

#include "Ifile.h"
class file_posix: public Ifile {
    size_t fd;
    size_t interpret_mode(open_mode mode) override;

public:
    explicit file_posix(size_t fd): fd{fd} {};
    file_posix (const std::string &file_name, open_mode mode);
    ~file_posix();
    virtual int read(std::string &buff, size_t buffer_max_size, int *status, size_t *true_buff_size) override;
    virtual int write(const std::string &buff, size_t buffer_max_size, int *status, size_t *true_buff_size) override;
};


#endif //MYCAT_FILE_POSIX_H
