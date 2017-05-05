# NAME

**ftrestd** - a server which supports RESTful APIs

# SYNOPSIS

**ftrestd [-r ROOT-FOLDER] [-p PORT]**

# DESCRIPTION

**ftrestd** is a simple server, which supports RESTful APIs and provides various functionality related to operations with file or directories. A user can upload, download or remove file, create or remove (empty) directory or list contents of directory, remove duplicated files, remote copying or moving files using the client and the server performs this action.

# IMPLEMENTATION

The server is implemented as IPv6 server. It simply waits for any new client in the infinite loop. If there is one, the server reads message from client and parses it to get information from header. Firstly, we read header char by char, until double CLRF is found. We find out content length from header and then we read content using buffer. According to REST headers, the server detects action (PUT, GET, DELETE) and type of object (file or directory) and performs a appropriate action - e.g. creates directory, etc. In this phrase, there are some checks to verify validity of correctness of the action - ie. check if file of directory exists, check if valid file or directory, check if empty directory, etc...A message is sent back to client indicating the result of this action - if any error, the message contains information about error, which is printed on the client side. Server is implemented as a concurrent server, so it starts a new thread for every new client. File operations which modify files are protected using mutexes. The server ends when Ctrl-C is used.

# EXTENSIONS

The sever provides basic functionality like create or removing directories, uploading, downloading and removing files, listing content of directory and as an extensions we implemented support for copying and moving files on the server and also it is possible to apply regular expression when listing content of directory. We also added support to remove duplicated files in any directory, one unique copy is always left.

# OPTIONS

**-r**
:   Set a root folder, which is base folder for all actions. Users have their directories here. If not specified, current directory is used as a root folder.

**-p**
:   Set a port, which is used to initiate a connection. If not specified, default implicit port 6677 is used.

# ERRORS

In case of wrong arguments or network problems (bind, connect, etc...), a program exits with code 1. If something goes wrong when performing operations with files or directories, an error message is sent back to a client.

# EXAMPLES

**ftrestd -p 5566**
:   initiate a connection with port 5566 and a root folder is set as current directory.

**ftrestd -r /home/test**
:   initiate a connection with default implicit port 6677 and a root folder is set as /home/test.

**ftrestd -r /home/test -p 5566**
:   initiate a connection with port 5566 and a root folder is set as /home/test.


#  AUTHOR

Dávid Bolvanský (xbolva00)

# SEE ALSO

**ftrest** - a client


# NAME

**ftrest** - a client which communicates with a server using RESTful APIs


# SYNOPSIS

**ftrest COMMAND REMOTE-PATH [LOCAL-PATH]**

# DESCRIPTION

**ftrest** is a simple client which uses RESTful APIs to communicate with a server. A user can use this client to upload, download or remove file, create or remove (empty) directory or list contents of directory, remove duplicated files, remote copying or moving files using the client and the server performs this action.

# IMPLEMENTATION
The client processes command line options and creates request message to be sent to server. The client waits for reply message from the server which contains HTTP status code, message, content, etc. If HTTP status code is not 200, the error got from content of the reply message is printed to standard error stream and the client ends with code 1. If HTTP status code is 200, scripts ends with code 0 and nothing else is done unless the command requires it - e.g. the received content from the server is printed to standard output stream (lst) or to the file (get).

# EXTENSIONS

The user can use additional commands or extended current ones in the client. Using commands copy and move user can copy or move files directly on the server. The user can also apply regular expression on the result of command lst. Protocol in URL does not need to be specified, we use HTTP as the default one. Also, if port is not specified, port 6677 is used as the default port. Also, we detect the right mime type of files - which is used in the field Content-Type when uploading files (put action).

# OPTIONS

**ftrest** provides various options to comunicate with the server to perform I/O operations with files or directories.

## COMMAND

**put**
:   Copy file defined by *LOCAL-PATH* to directory defined by *REMOTE-PATH* on the server

**del**
:   Delete file defined by *REMOTE-PATH* on the server

**get**
:   Copy file defined by *REMOTE-PATH* on the server to current local directory or at the place defined by *LOCAL-PATH*, if set

**lst**
:   Print content of directory defined by *REMOTE-PATH* to standard output stream (same format as *ls*). A filter can be defined by *LOCAL-PATH* (regular expression).

**mkd**
:   Create directory defined by *REMOTE-PATH* on the server. Creating user directories. is not allowed.

**rmd**
:   Delete directory defined by *REMOTE-PATH* on the server (directory must be empty). Removing user directories is not allowed.

**copy**
:   Copy file defined by *REMOTE-PATH* to file defined by *LOCAL-PATH* - remote copying on the server side

**move**
:   Move file defined by *REMOTE-PATH* to file defined by *LOCAL-PATH* - remote moving on the server side

**rdl**
:   Remove duplicated files (leave one unique file) in a directory defined by *REMOTE-PATH*


## REMOTE PATH

**Definition**

:   **[ protocol :// ] hostname [ : port ] / path**

:   if protocol is not defined, http is used as a default protocol

:   if port is not defined, 6677 is used as a default protocol

**Example**
:   http://localhost:12345/tonda/foo/bar
:   protocol is http, hostname is localhost, port is 12345 and path is tonda/foo/bar

## LOCAL PATH

**Meanings**

:   local file for put or get operations

:   filter for lst operation

:   remote file for copy or move operations


## ERRORS

**Not a directory.**
:   Occurs when *REMOTE-PATH* is a file when processing operation lst, mkd or rmd

**Directory not found.**
:   Occurs when directory defined *REMOTE-PATH* does not exist when processing operation lst, mkd, rmd or put.

**Directory not empty.**
:   Occurs when directory defined *REMOTE-PATH* is not empty when processing operation rmd

**Already exists.**
:   Occurs when directory  or file defined *REMOTE-PATH* already exists when processing operation mkd or put

**Not a file.**
:   Occurs when *REMOTE-PATH* is a directory when processing operation put or get

**File not found.**
:   Occurs when file defined by*REMOTE-PATH* does not exist when processing operation put or get

**User Account Not Found**
:   Occurs when operation works with user directory which does not exists

**Unknown error.**
:   For other errors when creating/removing/copying/moving operations failed on the server side.

In case of wrong arguments or network problems (bind, connect, etc...), the client exits with code 1. If something goes wrong on the server when performing operations with files or directories, the client will get message from server with more details about the error and prints it to the standard error stream.

# EXAMPLES

**ftrest mkd http://localhost:12345/tonda/foo/bar**
:   create the directory *foo* in the directory *bar* which is the subdirectory of the root directory of the user *tonda*

**ftrest put http://localhost:12345/tonda/foo/bar/doc.pdf ~/doc.pdf**
:   upload the local file *doc.pdf* into the directory *bar* on the server

**ftrest get http://localhost:12345/tonda/foo/bar/doc.pdf**
:   download the remote file *doc.pdf* into the current local directory

**ftrest get http://localhost:12345/tonda/foo/bar/doc.pdf ./bar**
:   download the remote file *doc.pdf* into the directory *bar*

**ftrest get http://localhost:12345/tonda/foo/bar/doc.pdf file.pdf**
:   download the remote file *doc.pdf* into the current local directory as *file.pdf*

**ftrest del http://localhost:12345/tonda/foo/bar/doc.pdf**
:   delete the remote file *doc.pdf* on the server

**ftrest rmd http://localhost:12345/tonda/foo/bar**
:   delete the directory *foo* on the server

**ftrest lst http://localhost:12345/tonda**
:   print content of the directory *tonda*

**ftrest lst http://localhost:12345/tonda "*.cc$"**
:   print files of the directory *tonda* which has extentions *.cc*

**ftrest copy http://localhost:12345/tonda/foo/bar/doc.pdf /tonda/foo/file.pdf**
:   copy the remote file *doc.pdf* to the directory *foo* as *file.pdf*

**ftrest move http://localhost:12345/tonda/foo/bar/doc.pdf /tonda/presentation.pdf**
:   move the remote file *doc.pdf* to the directory *foo* as *presentation.pdf*

**ftrest rdf http://localhost:12345/tonda/foo/bar**
:   remove duplicated files in the directory *bar*, leave one unique file


#  AUTHOR

Dávid Bolvanský (xbolva00)


# SEE ALSO

**ftrestd** - a server
