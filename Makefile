all:
	g++ ./Src/*.cpp -std=c++11 -Wall -I./Src -DLINUX -L./Lib/linux -lmp3lame -pthread -o ./Output/Linux/LameMp3Encoder

clean:
	rm -rf ./Src/*.o ./Output/Linux/LameMp3Encoder