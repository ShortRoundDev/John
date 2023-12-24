#include "SectionHeader.h"

SectionHeader::SectionHeader(uint16_t x, uint16_t y, std::string path) : UINode(
	{
		x,   y,
		128, 32,
		path
	}
)
{

}