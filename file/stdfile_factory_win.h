//
// Created by ADMIN on 25-Sep-23.
//

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
