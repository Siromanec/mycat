// This is a personal academic project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++, C#, and Java: http://www.viva64.com


#ifndef MYCAT_STDFILE_FACTORY_H
#define MYCAT_STDFILE_FACTORY_H
#include "Ifile.h"
#include <memory>
class stdfile_factory {
public:
//    virtual stdfile_factory() = default;
//    ~stdfile_factory() = default;

    [[nodiscard]] virtual std::unique_ptr<Ifile> create_stdin() const = 0;
    [[nodiscard]] virtual std::unique_ptr<Ifile> create_stdout() const = 0;
    [[nodiscard]] virtual std::unique_ptr<Ifile> create_stderr() const = 0;
    virtual ~stdfile_factory() = default;

};


#endif //MYCAT_STDFILE_FACTORY_H
