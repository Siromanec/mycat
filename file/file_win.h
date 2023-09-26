//
// Created by ADMIN on 25-Sep-23.
//

#ifndef MYCAT_FILE_WIN_H
#define MYCAT_FILE_WIN_H

#include "Ifile.h"
#include <windows.h>

class file_win: public Ifile {
    HANDLE fh;

    size_t interpret_mode(open_mode mode) override;
public:

    file_win(const std::string &file_name, open_mode mode);
    explicit file_win(HANDLE fh): fh{fh} {};

    virtual int read(std::string &buff, size_t buffer_max_size, int *status, size_t *true_buff_size) override;
    virtual int write(const std::string &buff, size_t buffer_max_size, int *status, size_t *true_buff_size) override;

    ~file_win() {
        CloseHandle(fh);
    };
};


#endif //MYCAT_FILE_WIN_H
