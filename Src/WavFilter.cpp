#include "WavFilter.h"

#ifdef WIN32
#define PATH_DELIMETER "\\"
#define MAX_DIR_LEN 260
#else
#define PATH_DELIMETER "/"
#endif

using namespace std;

WavFilter::WavFilter() {}
WavFilter::~WavFilter() {}

std::vector<std::string> WavFilter::read_directory(const std::string &path, const std::string &extension)
{
	std::vector<std::string> result;
#ifndef WIN32
	auto str_to_lower = [](const string& input) {
		string result(input.length(), 0);
		transform(input.begin(), input.end(), result.begin(), ::tolower);
		return result;
	};
	auto get_file_size = [](const string& filename) {
		struct stat stat_buf;
		int rc = stat(filename.c_str(), &stat_buf);
		return rc == 0 ? stat_buf.st_size : -1;
	};
	string lcExtension(str_to_lower(extension));

	auto *dir = opendir(path.c_str());
	if (!dir)
		throw std::runtime_error("Unable to open directory.");

	struct dirent *ent;
	while ((ent = readdir(dir)) != nullptr)
	{
		string entry(ent->d_name);
		string lcEntry(str_to_lower(entry));

		// Check extension matches (case insensitive)
		auto pos = lcEntry.rfind(lcExtension);
		if (pos != string::npos && pos == lcEntry.length() - lcExtension.length()) {
			auto full_path = path + '/' + entry;
			if (get_file_size(full_path) > 0)
				result.push_back(full_path);
			else
				cout << "Truncated wav file: " << full_path << '\n';
		}
	}

	if (closedir(dir) != 0)
		throw std::runtime_error("Unable to close directory.");
#else
		std::string pattern(path);
		pattern.append("\\*");
		WIN32_FIND_DATA data;

		HANDLE hFind = FindFirstFile(mbs_to_wcs(pattern.c_str()), &data);
		if (hFind == INVALID_HANDLE_VALUE)
		{
			printf("FindFirstFile failed (%d)\n", GetLastError());
			return result;
		}

		do {
			if (data.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)
				continue;
			if (!wcsstr(data.cFileName, mbs_to_wcs(extension.c_str())))
				continue;

			result.push_back(path + std::string(PATH_DELIMETER) + std::string(wcs_to_mbs(data.cFileName)));
		} while (FindNextFile(hFind, &data) != 0);

		FindClose(hFind);
#endif
	return result;
}

#ifdef WIN32
LPWSTR WavFilter::mbs_to_wcs(const char *mbstr) {
	size_t length;
	static wchar_t wtext[MAX_DIR_LEN] = { 0, };
	mbstowcs_s(&length, wtext, (size_t)MAX_DIR_LEN, mbstr, (size_t)MAX_DIR_LEN);
	return wtext;
}

char* WavFilter::wcs_to_mbs(const LPWSTR wcstr) {
	size_t   length;
	static char text[MAX_DIR_LEN] = { 0, };
	wcstombs_s(&length, text, (size_t)MAX_DIR_LEN, wcstr, (size_t)MAX_DIR_LEN);
	return text;
}
#endif 