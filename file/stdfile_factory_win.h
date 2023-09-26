// This is a personal academic project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++, C#, and Java: http://www.viva64.com


#ifndef MYCAT_STDFILE_FACTORY_WIN_H
#define MYCAT_STDFILE_FACTORY_WIN_H

#include "stdfile_factory.h"
#include "file_win.h"

class stdfile_factory_win: public stdfile_factory {
    std::unique_ptr<Ifile> create_stdin() const override {
        return std::make_unique<file_win>(GetStdHandle(STD_INPUT_HANDLE));
    };
    std::unique_ptr<Ifile> create_stdout() const override {
        return std::make_unique<file_win>(GetStdHandle(STD_OUTPUT_HANDLE));
    };
    std::unique_ptr<Ifile> create_stderr() const override {
        return std::make_unique<file_win>(GetStdHandle(STD_ERROR_HANDLE));
    };
};


#endif //MYCAT_STDFILE_FACTORY_WIN_H
