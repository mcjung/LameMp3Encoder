#pragma once

#include <iostream>
#include <fstream>
#include <string>

using namespace std;

// WAVE file header format
struct HEADER {
	unsigned char riff[4];                     // RIFF string
	unsigned int overall_size;                 // overall size of file in bytes
	unsigned char wave[4];                     // WAVE string
	unsigned char fmt_chunk_marker[4];         // fmt string with trailing null char
	unsigned int length_of_fmt;                // length of the format data
	unsigned int format_type;                  // format type. 1-PCM, 3- IEEE float, 6 - 8bit A law, 7 - 8bit mu law
	unsigned int channels;                     // no.of channels
	unsigned int sample_rate;                  // sampling rate (blocks per second)
	unsigned int byterate;                     // SampleRate * NumChannels * BitsPerSample/8
	unsigned int block_align;                  // NumChannels * BitsPerSample/8
	unsigned int bits_per_sample;              // bits per sample, 8- 8bits, 16- 16 bits etc
	unsigned char data_chunk_header[4];        // DATA string or FLLR string
	unsigned int data_size;                    // NumSamples * NumChannels * BitsPerSample/8 - size of the next chunk that will be read
};

class WavParser {
public:
	WavParser(std::string &input);
	~WavParser();
	unsigned int get_channels();
	unsigned int get_sample_rate();
	unsigned int get_bits_per_sample();
	long get_num_samples();
private:
	HEADER wav_header;
	std::ifstream wav;
	unsigned int channels;
	unsigned int sample_rate;
	unsigned int bit_per_sample;
	char format_name[10];
	long num_samples;
	long size_of_each_sample;
	float duration_in_seconds;

	void parse_wave(std::string &input);
	void debug_info();
};