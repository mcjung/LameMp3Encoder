#ifndef WAV_FILTER_H
#define WAV_FILTER_H

#include <algorithm>
#include <iostream>
#include <fstream>
#include <vector>
#include <cstring>
#include <string>
#include <sys/stat.h>

#ifdef WIN32
#include <windows.h>
#else
#include <sys/types.h>
#include <dirent.h>
#endif 

#ifdef WIN32
#define PATH_DELIMETER "\\"
#define PATH_MAX 260
#else
#define PATH_DELIMETER "/"
#define PATH_MAX 4096
#endif

class WavFilter {
public:
	WavFilter();
	~WavFilter();

	std::vector<std::string> read_directory(const std::string &path, const std::string &extension);

private:
#ifdef WIN32
	LPWSTR mbs_to_wcs(const char *mbstring);
	char* wcs_to_mbs(const LPWSTR wcstring);
#endif
};

#endif //WAV_FILTER_H_
