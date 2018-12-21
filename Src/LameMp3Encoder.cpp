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
	int16_t *pcm_buffer = new int16_t[PCM_SIZE * parser.get_channels()];
	unsigned char *mp3_buffer = new unsigned char [MP3_SIZE];
	const size_t bytes_per_sample = parser.get_channels() * parser.get_bits_per_sample() / 8;
	const string ext = { "mp3" };

	string output(input);
	output.replace(output.end() - ext.length(), output.end(), ext);
	std::ifstream wav;
	std::ofstream mp3;

	try {	
		wav.open(input, std::ios_base::binary | std::ios_base::in);
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

	if (lame_init_params(lame) >= 0) {
		while (wav.good()) {
			int write = 0;
			wav.read(reinterpret_cast<char*>(pcm_buffer), sizeof(pcm_buffer));
			int read = wav.gcount() / bytes_per_sample;
			if (read == 0)
				write = lame_encode_flush(lame, mp3_buffer, MP3_SIZE);
			else
				write = lame_encode_buffer_interleaved(lame, pcm_buffer, read, mp3_buffer, MP3_SIZE);
			mp3.write(reinterpret_cast<char*>(mp3_buffer), write);
		}
	}

	wav.close();
	mp3.close();

	lame_close(lame);

	delete [] pcm_buffer;
	delete [] mp3_buffer;
	return true;
}