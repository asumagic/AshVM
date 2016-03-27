#include "fileimport.h"

#include <cstdio>

namespace ash
{
	programData loadProgramFromFile(const char* filename)
	{
		FILE* bcfile = fopen(filename, "rb");
		if (!bcfile)
		{
			perror("Could not load bytecode ");
			return programData{ nullptr, 0 };
		}

		fseek(bcfile, 0, SEEK_END); // Seek to EOF
		long int length = (ftell(bcfile) / 4); // Get the file length
		fseek(bcfile, 0, SEEK_SET); // Seek to the beginning back

		uint32_t* program = new uint32_t[length];
		fread(program, sizeof(uint32_t), length, bcfile);
		fclose(bcfile);

		return programData{ program, static_cast<uint32_t>(length / 2) };
	}
}

