#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "WavParser.h"

// WAVE wav_header structure
unsigned char buffer4[4];
unsigned char buffer2[2];

WavParser::WavParser(std::string &input) {
	parse_wave(input);
	//debug_info();
}

WavParser::~WavParser() {}

unsigned int WavParser::get_channels() {
	return wav_header.channels;
}

unsigned int WavParser::get_sample_rate() {
	return wav_header.sample_rate;
}

unsigned int WavParser::get_bits_per_sample() {
	return wav_header.bits_per_sample;
}

long WavParser::get_num_samples() {
	return num_samples;
}

void WavParser::parse_wave(std::string &input) {
	try {
		wav.open(input, std::ios_base::binary | std::ios_base::in);

		// read wav_header parts
		wav.read(reinterpret_cast<char*>(wav_header.riff), sizeof(wav_header.riff));

		// convert little endian to big endian 4 byte int
		wav.read(reinterpret_cast<char*>(buffer4), sizeof(buffer4));
		wav_header.overall_size = buffer4[0] |
			(buffer4[1] << 8) |
			(buffer4[2] << 16) |
			(buffer4[3] << 24);

		wav.read(reinterpret_cast<char*>(wav_header.wave), sizeof(wav_header.wave));
		wav.read(reinterpret_cast<char*>(wav_header.fmt_chunk_marker), sizeof(wav_header.fmt_chunk_marker));

		// convert little endian to big endian 4 byte integer
		wav.read(reinterpret_cast<char*>(buffer4), sizeof(buffer4));
		wav_header.length_of_fmt = buffer4[0] |
			(buffer4[1] << 8) |
			(buffer4[2] << 16) |
			(buffer4[3] << 24);

		wav.read(reinterpret_cast<char*>(buffer2), sizeof(buffer2));
		wav_header.format_type = buffer2[0] | (buffer2[1] << 8);
		if (wav_header.format_type == 1)
			strcpy(format_name, "PCM");
		else if (wav_header.format_type == 6)
			strcpy(format_name, "A-law");
		else if (wav_header.format_type == 7)
			strcpy(format_name, "Mu-law");

		wav.read(reinterpret_cast<char*>(buffer2), sizeof(buffer2));
		wav_header.channels = buffer2[0] | (buffer2[1] << 8);

		wav.read(reinterpret_cast<char*>(buffer4), sizeof(buffer4));
		wav_header.sample_rate = buffer4[0] |
			(buffer4[1] << 8) |
			(buffer4[2] << 16) |
			(buffer4[3] << 24);

		wav.read(reinterpret_cast<char*>(buffer4), sizeof(buffer4));
		wav_header.byterate = buffer4[0] |
			(buffer4[1] << 8) |
			(buffer4[2] << 16) |
			(buffer4[3] << 24);

		wav.read(reinterpret_cast<char*>(buffer2), sizeof(buffer2));
		wav_header.block_align = buffer2[0] |
			(buffer2[1] << 8);

		wav.read(reinterpret_cast<char*>(buffer2), sizeof(buffer2));
		wav_header.bits_per_sample = buffer2[0] |
			(buffer2[1] << 8);

		wav.read(reinterpret_cast<char*>(wav_header.data_chunk_header), sizeof(wav_header.data_chunk_header));

		wav.read(reinterpret_cast<char*>(buffer4), sizeof(buffer4));
		wav_header.data_size = buffer4[0] |
			(buffer4[1] << 8) |
			(buffer4[2] << 16) |
			(buffer4[3] << 24);

		// calculate no.of samples
		num_samples = (8 * wav_header.data_size) / (wav_header.channels * wav_header.bits_per_sample);

		size_of_each_sample = (wav_header.channels * wav_header.bits_per_sample) / 8;

		// calculate duration of file
		duration_in_seconds = (float)wav_header.overall_size / wav_header.byterate;
	}
	catch (std::ifstream::failure e) {
		cout << "Failed to open input/output file " << strerror(errno) << std::endl;
	}

	wav.close();
}

void WavParser::debug_info() {
	printf("(1-4): %s \n", wav_header.riff);
	printf("(5-8) Overall size: bytes:%u, Kb:%u \n", wav_header.overall_size, wav_header.overall_size / 1024);
	printf("(9-12) Wave marker: %s\n", wav_header.wave);
	printf("(13-16) Fmt marker: %s\n", wav_header.fmt_chunk_marker);
	printf("(17-20) Length of Fmt wav_header: %u \n", wav_header.length_of_fmt);
	printf("(21-22) Format type: %u %s \n", wav_header.format_type, format_name);
	printf("(23-24) Channels: %u \n", wav_header.channels);
	printf("(25-28) Sample rate: %u\n", wav_header.sample_rate);
	printf("(29-32) Byte Rate: %u , Bit Rate:%u\n", wav_header.byterate, wav_header.byterate * 8);
	printf("(33-34) Block Alignment: %u \n", wav_header.block_align);
	printf("(35-36) Bits per sample: %u \n", wav_header.bits_per_sample);
	printf("(37-40) Data Marker: %s \n", wav_header.data_chunk_header);
	printf("(41-44) Size of data chunk: %u \n", wav_header.data_size);
	printf("Number of samples:%lu \n", num_samples);
	printf("Size of each sample:%ld bytes\n", size_of_each_sample);
	printf("Approx.Duration in seconds=%f\n", duration_in_seconds);
}