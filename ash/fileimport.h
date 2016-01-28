#ifndef ASHVM_FILEIMPORT_H
#define ASHVM_FILEIMPORT_H

#include <stdint.h>
#include <memory>

namespace ash
{
	struct programData
	{
		uint32_t* program;
		uint32_t size;
	};

	programData loadProgramFromFile(const char* filename);
}

#endif