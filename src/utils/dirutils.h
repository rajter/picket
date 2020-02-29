#include <string>
#include <iostream>
#include <unistd.h>
#include <sys/stat.h>

using namespace std;

namespace DirUtilities
{
    static int DirectoryExists(string path)
    {
        struct stat info;

        int statRC = stat( path.c_str(), &info );
        if( statRC != 0 )
        {
            if (errno == ENOENT)  { return 0; } // something along the path does not exist
            if (errno == ENOTDIR) { return 0; } // something in path prefix is not a dir
            return -1;
        }

        return ( info.st_mode & S_IFDIR ) ? 1 : 0;
    }

    static bool CreateUserDirectory(string directory)
    {
        if (mkdir(directory.c_str(), 0777) == -1)
        {
            cerr << "Error in creating user directory:  " << strerror(errno) << endl;
            return false;
        }
        else
        {
            return true;
        }
    }
};