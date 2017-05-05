// IPK 2017
// Project 1
// Author: David Bolvansky xbolva00

#include <iostream>
#include <fstream>
#include <sstream>
#include <mutex>
#include <algorithm>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <linux/limits.h>
#include <dirent.h>
#include <string.h>
#include "file_io.h"

// Constuctor with file parameter
Storage::Storage(std::string file) {
        this->file = file;
        this->file_dir_exists = (access(file.c_str(), 0) == 0);
        struct stat status;
        memset(&status, 0, sizeof(status));
        stat(file.c_str(), &status );
        this->is_dir = status.st_mode & S_IFDIR;
}

// Get absolute path
std::string Storage::get_absolute_path() {
        return this->file;
}
// Checks if file or folder exists
bool Storage::exists() {
        return this->file_dir_exists;
}

// Checks if object is file
bool Storage::is_file() {
        return !this->is_dir;
}

// Get directory base name
std::string Storage::get_directory_base_name() {
  std::string dirname = this->file;
  size_t pos = dirname.find_last_of("/\\");
  return dirname.substr(0, pos);
}

// Check if object is directory
bool Storage::is_directory() {
        return this->is_dir;
}

// Check if object is a root directory for user on server
bool Storage::is_user_root_directory() {
        std::string f = this->file;
        size_t n = std::count(f.begin(), f.end(), '/');
        return f.find('/') == std::string::npos || (f.substr(f.size()-1,1) == "/" && n == 1);
}

// Create directory
bool Storage::create_directory() {
        return mkdir(file.c_str(), S_IRWXU | S_IRWXG | S_IROTH | S_IXOTH);
}

// Checks if directory is empty
bool Storage::is_empty_directory() {
        struct dirent *ent;
        bool is_empty = true;

        DIR *d = opendir(file.c_str());
        if (!d) {
                return false;
        }

        while ((ent = readdir(d))) {
                if (!strcmp(ent->d_name, ".") || !(strcmp(ent->d_name, "..")))
                        continue;
                is_empty = false;
                break;
        }

        closedir(d);
        return is_empty;
}

// Remove directory
int Storage::remove_directory() {
        return rmdir(file.c_str());
}

// Remove file
int Storage::remove_file() {
        return remove(file.c_str());
}

bool Storage::remove_duplicated_files() {
        std::string duplicated_files = "find '" + this->file + "' -type f -exec md5sum {} \\; | sort -k1 | uniq -w32 -D | tail -n +2 | awk '{print substr($0, index($0, $2))}'";
        if (duplicated_files.empty()) return true;  // no duplicates
        std::istringstream dups_stream(run_command(duplicated_files));
        std::string file;
        while (getline(dups_stream, file )) {
                Storage(file).remove_file();
        }

        return true;
}

// Get current working directory
std::string Storage::get_current_directory() {
        char buff[PATH_MAX];
        getcwd (buff, PATH_MAX);
        std::string cwd(buff);
        return cwd;
}

// Get file mime type
std::string Storage::get_content_type() {
        std::string mime = "file -b --mime-type '" + this->file + "'";
        std::string output = run_command(mime);
        size_t pos = output.find_last_not_of('\n') + 1;
        if (pos != std::string::npos) {
                output = output.substr(0, pos);
        }
        return output;
}

// List contents of directory
std::string Storage::list_directory() {
        std::string ls = "ls '" + this->file + "'";
        return run_command(ls);
}

// List contents of directory with filter
std::string Storage::list_directory_filtered(std::string filter) {
        std::string ls = "ls '" + this->file + "' | egrep '" + filter + "'";
        return run_command(ls);
}

// Read file to string
std::string Storage::read_file() {
        std::ifstream in(file.c_str(), std::ios::binary);
        std::ostringstream content;
        content << in.rdbuf();
        return content.str();
}

// Write string to file
bool Storage::write_file(std::string content) {
        static std::mutex mutex;
        std::lock_guard<std::mutex> lock(mutex);

        std::ofstream out;
        out.open(file.c_str(), std::ios::binary);
        if (!out) return false;
        out << content;
        out.close();
        return true;
}

// Copy files
bool Storage::copy_file(Storage to) {
        static std::mutex mutex;
        std::lock_guard<std::mutex> lock(mutex);

        std::ifstream src(get_absolute_path(), std::ios::binary);
        if (!src) return false;
        std::ofstream dst(to.get_absolute_path(),   std::ios::binary);
        if (!dst) return false;
        dst << src.rdbuf();
        src.close();
        dst.close();
        return true;
}

// Move files
bool Storage::move_file(Storage to) {
        if (!copy_file(to)) return false;
        return (remove_file() == 0);
}

// Run shell command
std::string Storage::run_command(std::string command) {
        FILE *p = popen(command.c_str(), "r");

        if (!p) return "";

        char out[COMMAND_OUTPUT_SIZE+1];

        std::string output;
        while (fgets(out, COMMAND_OUTPUT_SIZE, p)) {
                output += out;
        }

        pclose(p);
        return output;
}

// Trim spaces
std::string Storage::trim(std::string& str) {
        size_t first = str.find_first_not_of(' ');
        if (first == std::string::npos)
                return "";
        size_t last = str.find_last_not_of(' ');
        return str.substr(first, (last-first+1));
}
