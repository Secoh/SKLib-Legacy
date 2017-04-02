// Functions related to file/directory tree, in Windows/Linux platform-independent way
//
// Copyright [2016-2017] Secoh
//
// Licensed under the Apache License, Version 2.0 (the "License") - see http://www.apache.org/licenses/LICENSE-2.0
// You may not use this file except in compliance with the License.
// Software is distributed on "AS IS" BASIS, WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
//


/* Lets not release it yet


__GNUC__


#include<sys/stat.h>


#define DIR_SEP	'/'


#define x_mkdir(name) (mkdir((name), (S_IRWXU | S_IRGRP | S_IXGRP | S_IROTH | S_IXOTH)))
#define x_isftty(file) (isatty(fileno(file)))






(_WIN32)

#define DIR_SEP	'\\'




#include<direct.h>
#define x_mkdir(name) (_mkdir((name)))
//
#include<io.h>
#define x_isftty(file) (_isatty(_fileno(file)))		// use with stdin/out/err


*/


