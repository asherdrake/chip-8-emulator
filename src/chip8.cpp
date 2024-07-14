#include "chip8.h"
#include <fstream>

const unsigned int START_ADDRESS = 0x200;

Chip8::Chip8() {

}

void Chip8::LoadRom(char const* filename) {
    //creates 'file' object
    //two flags mean: open file as stream of binary, and move file pointer to the end
    std::ifstream file(filename, std::ios::binary | std::ios::ate);
    
    if (file.is_open())
	{
		// Get size of file and allocate a buffer to hold the contents
		std::streampos size = file.tellg();
		char* buffer = new char[size];

		// Go back to the beginning of the file and fill the buffer
		file.seekg(0, std::ios::beg);
		file.read(buffer, size);
		file.close();

		// Load the ROM contents into the Chip8's memory, starting at 0x200
		for (long i = 0; i < size; ++i)
		{
			memory[START_ADDRESS + i] = buffer[i];
		}

		// Free the buffer
		delete[] buffer;
	}
}