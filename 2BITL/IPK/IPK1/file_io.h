// IPK 2017
// Project 1
// Author: David Bolvansky xbolva00

#ifndef FILE_IO_H
#define FILE_IO_H

// Size of command output for shell command
const int COMMAND_OUTPUT_SIZE = 1024;

class Storage {
    std::string file;
    bool file_dir_exists;
    bool is_dir;
    public:
    Storage() {}
    Storage(std::string file);
    std::string get_absolute_path();
    bool exists();

    bool is_file();

    bool is_directory();

    std::string get_directory_base_name();

    bool is_user_root_directory();

    bool is_empty_directory();

    int remove_directory();

    int remove_file();

    bool create_directory();

    std::string get_current_directory();

    std::string get_content_type();

    std::string list_directory();
    std::string list_directory_filtered(std::string filter);

    std::string read_file();

    bool write_file(std::string content);

    bool copy_file(Storage to);
    bool move_file(Storage to);

    bool remove_duplicated_files();

private:
    static std::string run_command(std::string command);
    static std::string trim(std::string& str);
};


#endif // FILE_IO_H
