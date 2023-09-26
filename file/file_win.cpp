//
// Created by ADMIN on 25-Sep-23.
//

#include <sstream>
#include "file_win.h"

file_win::file_win(const std::string &file_name, open_mode mode) {

    HANDLE inputFh = CreateFile(file_name.c_str(), interpret_mode(mode),
            FILE_SHARE_READ, NULL, OPEN_EXISTING,
            FILE_ATTRIBUTE_NORMAL, NULL);
    if(inputFh == INVALID_HANDLE_VALUE) {
        int errno_copy = GetLastError();
        std::cerr << "*** Failed to open file " << file_name << " with errno = "
                                                 << errno_copy << " ***"<< std::endl;
        throw std::exception();
    }

    fh = inputFh;

}

size_t file_win::interpret_mode(open_mode mode) {
    switch (mode) {
        case FILE_READ:
            return GENERIC_READ;
        case FILE_WRITE:
            return GENERIC_WRITE;
        case FILE_READWRITE:
            return GENERIC_READ | GENERIC_WRITE;
        default:
            std::cerr << "*** not implemented ***" << std::endl;
            throw
            std::exception();
    }
    return 0;
}

int file_win::read(std::string &buff, size_t buffer_max_size, int *status, size_t *true_buff_size) {
    DWORD read_bytes = 0;
    DWORD size = buffer_max_size - *true_buff_size;
    BOOL read_success = ReadFile(fh, buff.data() + *true_buff_size, size, &read_bytes, NULL);
    if (!read_success) { // considered adding  || read_bytes!=size, but it becomes dumb when reading the ending of a file
//        if(ERROR_IO_PENDING == GetLastError()){
//            continue;
//        }
        *status=GetLastError();
        return -1;
    }

    *true_buff_size += read_bytes;

    return 0;
}

int file_win::write(const std::string &buff, size_t buffer_max_size, int *status, size_t *true_buff_size) {
    DWORD written_bytes = 0;
    DWORD size = *true_buff_size;
    BOOL read_success = WriteFile(fh,  buff.data(), size, &written_bytes, NULL);
    if (!read_success || written_bytes!=size) {
        *status=GetLastError();
        return -1;
    }

    *true_buff_size = 0 ;

    return 0;
}
