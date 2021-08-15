#include "FileCrc32.h"

#include <string>
#include <cassert>
#include <iostream>
#include <sstream>
#include <iomanip>

#ifndef __x86_64__
#ifndef __aarch64__

bool FileCrc32::tableCreated = false;

FileCrc32::FileCrc32()
{
	if (!FileCrc32::tableCreated)
	{
		uint32_t crc;
		for (int i = 0; i < 256; i++)
		{
			crc = i;
			for (int j = 0; j < 8; j++)
			{
				if (crc & 1)
				{
					crc = (crc >> 1) ^ 0x82F63B78; // 多项式0x1EDC6F41
				}
				else
				{
					crc >>= 1;
				}
			}
			FileCrc32::crcTable[i] = crc;
		}
		FileCrc32::tableCreated = true;
	}
}
#endif
#endif

#if defined(__x86_64__) || defined(__aarch64__)
FileCrc32::FileCrc32()
{
}
#endif

std::string FileCrc32::GetFileCrc(const std::string &filePath)
{
	inFile.open(filePath);
	assert(inFile.is_open());

	uint32_t crc = 0xffffffff;
	uint8_t unChar;

	inFile >> std::noskipws;

	inFile.seekg(0, std::ios::end);

	int fileLength = inFile.tellg();

	std::cout << std::dec;
	std::cout << "Length of this file is:" << fileLength << std::endl;

	inFile.seekg(0, std::ios::beg);

#ifdef __linux__
	fileLength += fileLength > 0 ? -1 : 0;
#endif

	for (int i = 1; i <= fileLength; i++)
	{
		inFile >> unChar;
		crc = Crc32_u8(crc, unChar);
		std::cout << i << "    " << unChar << std ::endl;
	}
	
	inFile.close();

	std::stringstream strCrc;
	strCrc << std::hex << std::setiosflags(std::ios::uppercase);
	strCrc << (crc ^ 0xffffffff);
	return strCrc.str();
}