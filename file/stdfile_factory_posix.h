//
// Created by ADMIN on 25-Sep-23.
//

#ifndef MYCAT_STDFILE_FACTORY_POSIX_H
#define MYCAT_STDFILE_FACTORY_POSIX_H

#include <unistd.h>


#include "stdfile_factory.h"
#include "file_posix.h"
class stdfile_factory_posix: public stdfile_factory {


    std::unique_ptr<Ifile> create_stdin() const override {
        return std::make_unique<file_posix>(STDIN_FILENO);
    };
    std::unique_ptr<Ifile> create_stdout() const override {
        return std::make_unique<file_posix>(STDOUT_FILENO);
    };
    std::unique_ptr<Ifile> create_stderr() const override {
        return std::make_unique<file_posix>(STDERR_FILENO);
    };
};


#endif //MYCAT_STDFILE_FACTORY_POSIX_H
