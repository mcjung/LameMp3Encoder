#include <iostream>
#include <fstream>
#include <string.h>

#include "WavParser.h"
#include "LameMp3Encoder.h"

using namespace std;

LameMp3Encoder::LameMp3Encoder() {}
LameMp3Encoder::~LameMp3Encoder() {}

bool LameMp3Encoder::encode_wav(std::string &input)
{
	WavParser parser(input);

	const size_t IN_SAMPLERATE = parser.get_sample_rate();
	const size_t PCM_SIZE = 8192;
	const size_t MP3_SIZE = 8192;
	const size_t LAME_GOOD = 5;
	const size_t NUM_CHANNELS = parser.get_channels();
	const size_t BITS_PER_SAMPLE = parser.get_bits_per_sample();
	int16_t *pcm_buffer16 = new int16_t[PCM_SIZE * NUM_CHANNELS];
	unsigned char *pcm_buffer8 = new unsigned char[PCM_SIZE * NUM_CHANNELS];
	unsigned char *mp3_buffer = new unsigned char [MP3_SIZE];
	const size_t bytes_per_sample = NUM_CHANNELS * BITS_PER_SAMPLE / 8;
	const string ext = { "mp3" };

	string output(input);
	output.replace(output.end() - ext.length(), output.end(), ext);
	std::ifstream wav;
	std::ofstream mp3;

	try {	
		wav.open(input, std::ios_base::binary | std::ios_base::in);
		wav.seekg(44, ios_base::beg); //to remove click sound 
		mp3.open(output, std::ios_base::binary | std::ios_base::out);
	}
	catch (std::ifstream::failure e) {
		cout << "Failed to open input/output file " << strerror(errno) << std::endl;
		return false;
	}

	lame_t lame = lame_init();
	lame_set_in_samplerate(lame, IN_SAMPLERATE);
	lame_set_VBR(lame, vbr_default);
	lame_set_VBR_q(lame, LAME_GOOD);
	if (NUM_CHANNELS == 1) {
		lame_set_num_channels(lame, 1);
		lame_set_mode(lame, MONO);
	}
	else if (NUM_CHANNELS == 2) {
		lame_set_num_channels(lame, 2);
		lame_set_mode(lame, STEREO);
	}
	else {
		std::cout << "Unsupported channel" << std::endl;
	}

	if (lame_init_params(lame) >= 0) {
		while (wav.good()) {
			int write = 0;
			int read = 0;
			if (BITS_PER_SAMPLE == 8) {
				wav.read(reinterpret_cast<char*>(pcm_buffer8), sizeof(pcm_buffer8));
				read = wav.gcount() / bytes_per_sample;

				for (int i = 0; i < read; i++) {
					pcm_buffer16[i] = (short)(pcm_buffer8[i] - 0x80) << 8;
				}
			} 
			else if (BITS_PER_SAMPLE == 16) {
				wav.read(reinterpret_cast<char*>(pcm_buffer16), sizeof(pcm_buffer16));
				read = wav.gcount() / bytes_per_sample;
			}
			else {
				std::cout << "Unsupported bits_per_sample" << std::endl;
			}

 			if (read == 0)
				write = lame_encode_flush(lame, mp3_buffer, MP3_SIZE);
			else
			{
				if (NUM_CHANNELS == 1) {
					write = lame_encode_buffer(lame, pcm_buffer16, NULL, read, mp3_buffer, MP3_SIZE);
				}
				else if (NUM_CHANNELS == 2) {
					write = lame_encode_buffer_interleaved(lame, pcm_buffer16, read, mp3_buffer, MP3_SIZE);
				}
				else {
					std::cout << "Unsupported channel" << std::endl;
				}
			}
				
			mp3.write(reinterpret_cast<char*>(mp3_buffer), write);
		}
	}

	wav.close();
	mp3.close();

	lame_close(lame);

	delete [] pcm_buffer16;
	delete[] pcm_buffer8;
	delete [] mp3_buffer;
	return true;
}