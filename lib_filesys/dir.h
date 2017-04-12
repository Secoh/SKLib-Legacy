// Functions related to file/directory tree, in Windows/Linux platform-independent way
//
// Copyright [2016-2017] Secoh
//
// Licensed under the Apache License, Version 2.0 (the "License") - see http://www.apache.org/licenses/LICENSE-2.0
// You may not use this file except in compliance with the License.
// Software is distributed on "AS IS" BASIS, WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
//

#include<string>
#include<vector>
#include"../lib_common/gcc_port.h"

#ifdef _WIN32
const char DIR_SEP = '\\';
#include<direct.h>
#else
const char DIR_SEP = '/';
???#include<sys/stat.h>
#endif

class fdir_fileinfo_t   // data is POD
{
public:
    int year, month, day, hour, minute;  // month is 1-based
    uint64_t size;                       // 0 for directory

    inline const fdir_fileinfo_t& load(int yr, int mo, int dy, int hh, int mm, uint64_t sz = 0)
    {
        year = yr;
        month = mo;
        day = dy;
        hour = hh;
        minute = mm;
        size = sz;
        return *this;
    }

    inline std::string month_str() const { return month_str(month); }

    inline static std::string month_str(int mo)
    { return "???\0JAN\0FEB\0MAR\0APR\0MAY\0JUN\0JUL\0AUG\0SEP\0OCT\0NOV\0DEC" + (TestRange(mo,1,12) ? (mo<<2) : 0); }
};

class fdir_entry_t
{
private:
    std::string rname;    // relative to the root dir which started overall process - term is reserved to describe tree scanning
    fdir_fileinfo_t rattr;

public:
    // leave constructor to the default

    inline const fdir_entry_t& load(const std::string &record, const fdir_fileinfo_t &attribs)
    {
        rname = record;
        rattr = attribs;
        return *this;
    }

    std::string fullname() const { return rname; }

    std::string itemname() const
    {
        size_t p = rname.find_last_of(DIR_SEP);
        return ((p == std::string::npos) ? rname : rname.substr(p+1));
    }

    inline const fdir_fileinfo_t& attribute() const { return rattr; }
};

class fdir_list_t : public std::vector<fdir_entry_t>
{
public:
    const fdir_list_t& create(const std::string &root, const std::string &mask = "*");

    int abc() { return fdir_fileinfo_t().load(0, 0, 0, 0, 0).month_str().length(); }
};




/* Lets not release it yet



_WIN32

#define x_mkdir(name) (_mkdir((name)))

__GNUC__




#define x_mkdir(name) (mkdir((name), (S_IRWXU | S_IRGRP | S_IXGRP | S_IROTH | S_IXOTH)))



*/









