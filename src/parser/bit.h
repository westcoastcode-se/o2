//
// Part of the o2 Project, under the Apache License v2.0 with o2 Project Exceptions.
// See the LICENSE file in the project root for license terms
//

#pragma once

namespace o2
{
	static constexpr bool bit_isset(int bits, int bit)
	{
		return (bits & bit) == bit;
	}

	static constexpr int bit_unset(int bits, int bit)
	{
		return (bits & ~bit);
	}

	static constexpr int bit_set(int bits, int bit)
	{
		return (bits | bit);
	}
}