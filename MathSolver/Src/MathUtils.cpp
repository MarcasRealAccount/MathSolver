#include "MathUtils.h"

namespace MathUtils
{
	std::uint64_t GCD(std::uint64_t x, std::uint64_t y)
	{
		if (x <= 1 || y <= 1)
			return 1;

		while (x != y)
		{
			if (x > y)
				x -= y;
			else
				y -= x;
		}

		return x;
	}
} // namespace MathUtils