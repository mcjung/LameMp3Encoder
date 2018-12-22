Multithreading Lame Mp3 Encoder
===============================

PREREQUISITES
-------------
* LAME static library v3.100
  * Linux:   liblamemp3.a
  * Windows: libmp3lame-static.lib, libmpghip-static.lib

HOW TO BUILD
------------
* Linux:
  * $ make
  * (output: ./Output/Linux/LameMp3Encoder)
        
* Windows:
  * Run Visual Studio 2017
  * Open LameMp3Encoder.sln
  * Build the solution
  * (output: .\Output\Windows\LameMp3Encoder.exe)

USAGE
-----
* Linux:
  * $ ./LameMp3Encoder <wav_file_path>
        
* Windows:
  * C:\\>LameMp3Encoder.exe <wav_file_path>
