#ifndef LAME_MP3_ENCODER_H
#define LAME_MP3_ENCODER_H

#include <stdio.h>
#include <string>
#include "lame/lame.h"

class LameMp3Encoder {
public:
	LameMp3Encoder();
	~LameMp3Encoder();

	static bool encode_wav(std::string &input);
private:

};

#endif //LAME_MP3_ENCODER_H