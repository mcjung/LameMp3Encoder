#include "ThreadPool.h"
#include "WavFilter.h"
#include "ThreadPool.h"
#include "LameMp3Encoder.h"
#include <iostream>

using namespace std;

const int MAX_THREADS = 4;

struct thread_params {
	std::string name;
};

void thread_func(void* arg)
{
	thread_params *params = (thread_params*)arg;
	cout << params->name << endl;
	LameMp3Encoder::encode_wav(params->name);
}

int main(int argc, char* argv[])
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

	ThreadPool tp(MAX_THREADS);
	int ret = tp.initialize_threadpool();
	if (ret == -1) {
		cerr << "Failed to initialize thread pool!" << endl;
		return 0;
	}

	for (std::vector<std::string>::iterator it = files.begin(); it != files.end(); ++it) {
		thread_params *param = new thread_params();
		param->name = *it;

		Task* t = new Task(&thread_func, (void*)param);
		tp.add_task(t);
	}

	tp.destroy_threadpool();

	// timestamp
	clock_t time_to_end = clock();
	std::cout << "Total elapsed time to convert: " << double(time_to_end - time_to_begin) / CLOCKS_PER_SEC << "s." << endl;

	return 0;
}
