#if 1

#include "ThreadPool.h"
#include "WavFilter.h"
#include "ThreadPool.h"
#include "LameMp3Encoder.h"
#include <iostream>

using namespace std;

const int MAX_TASKS = 40;
const int MAX_THREADS = 4;

void thread_func(void* arg)
{
	int* x = (int*)arg;
	cout << "Hello " << *x << endl;
	//  cout << "\n";
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

	for (int i = 0; i < MAX_TASKS; i++) {
		int* x = new int();
		*x = i + 1;
		Task* t = new Task(&thread_func, (void*)x);
		//    cout << "Adding to pool, task " << i+1 << endl;
		tp.add_task(t);
		//    cout << "Added to pool, task " << i+1 << endl;
	}

	tp.destroy_threadpool();
	cout << "Exiting app..." << endl;

	return 0;
}



#if 0
#include <iostream>
#include <chrono>
#include "ThreadPool.h"
#include "WavFilter.h"
#include "ThreadPool.h"
#include "LameMp3Encoder.h"

#define MAX_THREAD 4

using namespace std;

void thread_func(void* arg)
{
	char *file_name = (char *)arg;
	cout << "dest: " << file_name << endl;
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

	ThreadPool tp(MAX_THREAD);
	int ret = tp.initialize_threadpool();
	if (ret == -1) {
		cerr << "Failed to initialize thread pool!" << endl;
		return 0;
	}

	for (std::vector<std::string>::iterator it = files.begin(); it != files.end(); ++it) {
		Task* t = new Task(&thread_func, (void *)(*it).c_str());
		tp.add_task(t);
	}

	tp.destroy_threadpool();

	// timestamp
	clock_t time_to_end = clock();
	std::cout << "Total elapsed time to convert: " << double(time_to_end - time_to_begin) / CLOCKS_PER_SEC << "s." << endl;

	return 0;
}
#endif 

#else

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
#endif 