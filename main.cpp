// This is a personal academic project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++, C#, and Java: http://www.viva64.com

#include <iostream>
#include <sys/stat.h> // Ñòðóêòóðè (òèïè) äëÿ fstat(), lstat (), stat ().
#include <fcntl.h> // Òèïè i êîíñòàíòè äëÿ fcntl() i open().
#include <unistd.h> // POSIX header.
#include <stdlib.h>
#include <ctype.h>

#include <vector>
#include <sstream>
#include "options_parser.h"
#include "file.h"
#include <cstdint>
//#define CNTRL_SPACE_TEST
#define PRINT_SPACE_TEST
#define RESERVE_FOR_NULL 1
#define BUFF_MAX_SIZE (1024)

#define HEX_CHAR_SIZE 4

enum return_codes {
    OK,
    NOFILE,
    NOTOPEN,
};

std::string char_to_hex(char ch) {
    std::stringstream ss{};
//    char buffer[50];
//    sprintf(buffer, "%02X", ch);
    ss << "\\x" << std::hex << std::uppercase << (0xFF & (unsigned int) ch); // gets rid of 0xFFXY and makes it 0x00XY. idk why FFXY appears
//    return std::string(buffer);
    return  ss.str();
}

std::string nonprint_to_hex(const char *string) {
//        std::stringstream new_sstream;
    size_t overhead_size = 0;
    size_t hex_char_overhead = 3; // #(\,x,A,B) - #(ch)

#ifdef CNTRL_SPACE_TEST
    // returns true if nonprint
    auto isnonprint = [](char ch) {
        return std::iscntrl(ch) && !std::isprint(ch);
    };
#else
 #ifdef PRINT_SPACE_TEST
    // returns true if nonprint
    auto isnonprint = [](char ch) {
        return !(std::isspace(ch) || std::isprint(ch));
    };
 #endif
#endif
    size_t str_index = 0;
    while ( string[str_index] != '\0') {
        char ch = string[str_index];
        if (isnonprint(ch)) {
            overhead_size += hex_char_overhead;
        }
        str_index += 1;
    }

    size_t str_size = str_index;
//    new_sstream.
//    new_sstream.reserve(string.size() + overhead_size; // it must know the new total size
    std::stringstream new_sstream(std::string(str_size + overhead_size,
                                              '\0')); //preallocation may not be worth it since a copy is made, but it should be tested in comparison without preallocation to make any conclusions
    for (size_t i = 0; i < str_size; ++i) {
        char ch = string[i];
        if (isnonprint(ch)) {
            new_sstream << char_to_hex(ch);
        } else {
            new_sstream << ch;
        }
    }
//    for (auto ch: string) {
//
//    }
    return new_sstream.str();
}

void closeall(std::vector<size_t> &file_descriptors) {
    //TODO Handling close errors
    while (!file_descriptors.empty()) {
        close(file_descriptors.back());
        file_descriptors.pop_back();
    }
}

//int writebuffer(int fd, const std::string &buffer, int *status) {
//    ssize_t written_bytes = 0;
//    while (written_bytes < buffer.size()) {
//        ssize_t written_now = write(fd, buffer.c_str() + written_bytes,
//                                    buffer.size() - written_bytes);
//        if (written_now == -1) {
//            if (errno == EINTR)
//                continue;
//            else {
//                *status = errno;
//                return -1;
//            }
//        } else
//            written_bytes += written_now;
//    }
//    return 0;
//}


int main(int argc, char *argv[]) {
    command_line_options_t command_line_options{argc, argv};
//    std::cout << "A flag value: " << command_line_options.get_A_flag() << std::endl;
    const std::vector<std::string> &file_names = command_line_options.get_filenames();
    std::vector<file> files;
    size_t file_count = file_names.size();
    files.reserve(file_count); // to avoid reallocation

    for (const auto &file_name: file_names) {
        files.emplace_back(file_name, O_RDONLY);
    }


    std::string buffer;
    buffer.reserve(BUFF_MAX_SIZE + RESERVE_FOR_NULL);
    int status = 0;
    size_t true_buff_size = 0;
    file stdout_file{STDOUT_FILENO};
//    buffer.size();
    for (auto& file: files) {
        // while buffer is not enough -> write to stout and try again
        // buffer is not enough when it is full
        // regular buff.size will not work from the box
        while (true_buff_size < BUFF_MAX_SIZE) {
            file.read(buffer, BUFF_MAX_SIZE, &status,  &true_buff_size);
            //  if buffer not full -> continue
            buffer.data()[true_buff_size + -1 + RESERVE_FOR_NULL] = '\0';

            if (true_buff_size < BUFF_MAX_SIZE)
                break;
//            stdout_file.write(buffer, BUFF_MAX_SIZE, &status,  &true_buff_size);

//            std::cout << buffer.data();
//            buffer.clear();
            if (command_line_options.get_A_flag() == true) {
                std::string hexified = nonprint_to_hex(buffer.c_str());
                stdout_file.write(hexified.c_str(), BUFF_MAX_SIZE, &status,  hexified.size());
            }
            else {
                stdout_file.write(buffer, BUFF_MAX_SIZE, &status,  true_buff_size);
            }
            true_buff_size = 0;
        }

    }
    if (command_line_options.get_A_flag() == true) {
        std::string hexified = nonprint_to_hex(buffer.c_str());
        stdout_file.write(hexified.c_str(), BUFF_MAX_SIZE, &status,  hexified.size());
    }
    else {
        stdout_file.write(buffer, BUFF_MAX_SIZE, &status,  true_buff_size);
    }
//    std::cout << buffer.data();



// for some reason last newline vanishes in the stdout and in original file, despite opening in readonly mode

    return OK;
}