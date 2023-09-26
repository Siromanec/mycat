// This is a personal academic project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++, C#, and Java: http://www.viva64.com

#include <iostream>
#include <ctype.h>

#include <memory>
#include <vector>

#include "options_parser.h"
#include "file/Ifile.h"
//#define CNTRL_SPACE_TEST
#define PRINT_SPACE_TEST
#define RESERVE_FOR_NULL 1
#define BUFF_MAX_SIZE (1024)


#ifdef POSIX

#include "file/stdfile_factory_posix.h"
#include <fcntl.h>
#else
#ifdef WIN32
// must assert that buffer size is <= MAX_DWORD
 #include "file/stdfile_factory_win.h"
 #if BUFF_MAX_SIZE > MAXDWORD
  #warning "BUFF_MAX_SIZE size bigger than MAXDWORD"
 #endif
#endif

#endif






#define HEX_CHAR_SIZE 4

enum return_codes {
    OK,
    NOT_OK_FILE_OPEN,
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
    return new_sstream.str();
}

int main(int argc, char *argv[]) {
    command_line_options_t command_line_options{argc, argv};
    const std::vector<std::string> &file_names = command_line_options.get_filenames();
    std::vector<std::shared_ptr<Ifile>> files;
    size_t file_count = file_names.size();
    if (file_count == 0) {
        return 0;
    }
    files.reserve(file_count); // to avoid reallocation
    try {
        for (const auto &file_name: file_names) {
        #ifdef POSIX
                    files.emplace_back(new file_posix {file_name, FILE_READ});
        #else
                    #ifdef WIN32
                files.emplace_back(new file_win {file_name, FILE_READ});
            #endif
        #endif
        }

    }
    catch (std::exception &ex) {
        std::cerr<<ex.what() <<std::endl;
        return NOT_OK_FILE_OPEN;
    }


    std::string buffer;
    buffer.reserve(BUFF_MAX_SIZE + RESERVE_FOR_NULL);
    int read_status = 0;
    int write_status = 0;
    int read_was_err = 0;
    int write_was_err = 0;

    size_t true_buff_size = 0;
    std::unique_ptr<stdfile_factory> stdfile_factory;
#ifdef POSIX
    stdfile_factory = std::make_unique<stdfile_factory_posix>();
#else
 #ifdef WIN32
    stdfile_factory = std::make_unique<stdfile_factory_win>();
  #endif
#endif
    const std::unique_ptr<Ifile> stdout_file = stdfile_factory->create_stdout();
    for (const auto file: files) { //reference on ptr, wtf clang-tidy; the shared_ptr should be passed by value
        // while buffer is not enough -> write to stout and try again
        // buffer is not enough when it is full
        // regular buff.size will not work from the box
        while (true_buff_size < BUFF_MAX_SIZE) {
            read_was_err = file->read(buffer, BUFF_MAX_SIZE, &read_status,  &true_buff_size);
            if(read_was_err != 0) {
                std::cerr << "*** error occurred while reading a file with error code " << read_status << " ***";
                return read_status;
            }
            //  if buffer not full -> continue
            buffer[true_buff_size + -1 + RESERVE_FOR_NULL] = '\0';

            if (true_buff_size < BUFF_MAX_SIZE)
                break;
            if (command_line_options.has_A_flag()) {
                std::string hexified = nonprint_to_hex(buffer.c_str());
                true_buff_size = hexified.size();
                write_was_err = stdout_file->write(hexified, BUFF_MAX_SIZE, &write_status,  &true_buff_size);
            }
            else {
                write_was_err = stdout_file->write(buffer, BUFF_MAX_SIZE, &write_status,  &true_buff_size);
            }

            if(write_was_err != 0) {
                std::cerr << "*** error occurred while writing to a file with error code " << write_status << " ***";
                return write_status;
            }
        }

    }
    if (command_line_options.has_A_flag()) {
        std::string hexified = nonprint_to_hex(buffer.c_str());
        true_buff_size = hexified.size();
        write_was_err = stdout_file->write(hexified, BUFF_MAX_SIZE, &write_status,  &true_buff_size);
    }
    else {
        write_was_err = stdout_file->write(buffer, BUFF_MAX_SIZE, &write_status,  &true_buff_size);
    }
    if(write_was_err != 0) {
        std::cerr << "*** error occurred while writing to a file with error code " << write_status << " ***";
        return write_status;
    }

    return OK;
}
