// This is a personal academic project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++, C#, and Java: http://www.viva64.com


#include <sstream>
#include "file_win.h"

file_win::file_win(const std::string &file_name, open_mode mode) {

    HANDLE inputFh = CreateFile(file_name.c_str(), interpret_mode(mode),
            FILE_SHARE_READ, NULL, OPEN_EXISTING,
            FILE_ATTRIBUTE_NORMAL, NULL);
    if(inputFh == INVALID_HANDLE_VALUE) {
        DWORD errno_copy = GetLastError();
        std::cerr << "*** Failed to open file " << file_name << " with errno = "
                                                 << errno_copy << " ***"<< std::endl;
        throw std::exception();
    }

    fh = inputFh;

}

size_t file_win::interpret_mode(open_mode mode) const {
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
}

int file_win::read(std::string &buff, size_t buffer_max_size, int *status, size_t *true_buff_size) {
    DWORD read_bytes = 0;
    DWORD read_now = 0;

    DWORD size = buffer_max_size - *true_buff_size;
    BOOL read_success;
    do {
        read_success = ReadFile(fh, buff.data() + *true_buff_size + read_bytes, size - read_bytes, &read_now, NULL);
        if (!read_success) {
            if (GetLastError() == ERROR_IO_PENDING)
                continue;
            else {
                *status = GetLastError();
                return -1;
            }
        } else
            read_bytes += read_now;
    }
    while (read_bytes < size && read_now != 0);

    *true_buff_size += read_bytes;

    return 0;
}

int file_win::write(const std::string &buff, size_t buffer_max_size, int *status, size_t *true_buff_size) {
    DWORD written_bytes = 0;
    DWORD written_now = 0;
    DWORD size = *true_buff_size;
    BOOL write_success;
    while (written_bytes  < size) {
        write_success = WriteFile(fh,  buff.data() + written_bytes, size - written_bytes, &written_now, NULL);
        if (!write_success) {
            if (GetLastError() == ERROR_IO_PENDING)
                continue;
            else {
                *status = GetLastError();
                return -1;
            }
        } else
            written_bytes  += written_now;
    }

    *true_buff_size = 0 ;

    return 0;
}
