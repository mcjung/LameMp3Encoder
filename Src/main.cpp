#include <iostream>
#include <chrono>

#include "WavFilter.h"
#include "ThreadPool.h"
#include "LameMp3Encoder.h"

using namespace std;

int main(int argc, const char * argv[])
{
	if (argc != 2) {
		std::cout << "Usage: " << argv[0] << " <directory>" << std::endl;
		return 1;
	}

	std::cout << "LAME version: " << get_lame_version() << std::endl;
 
	const std::string path(argv[1]);
	std::vector<std::string> files;

	WavFilter wav_filter;
	files = wav_filter.read_directory(path, ".wav");
	
	// timestamp
	clock_t time_to_begin = clock();

	if (files.size()) {
		try {
			const size_t num_thread = thread::hardware_concurrency();
			ThreadPool pool(num_thread);
			pool.init();

			for (std::vector<std::string>::iterator it = files.begin(); it != files.end(); ++it)
			{
				pool.submit(LameMp3Encoder::encode_wav, *it);
			}
		}
		catch (std::runtime_error &e) {
			std::cout << e.what();
			exit(1);
		}
	}
	else {
		std::cout << "There is no valid WAV files in " << path << std::endl;
		return 1;
	}

	// timestamp
	clock_t time_to_end = clock();
	std::cout << "Total elapsed time to convert: " << double(time_to_end - time_to_begin) / CLOCKS_PER_SEC << "s." << endl;

	return 0;
}
