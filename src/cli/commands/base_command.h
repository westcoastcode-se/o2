//
// Part of the o2 Project, under the Apache License v2.0 with o2 Project Exceptions.
// See the LICENSE file in the project root for license terms
//

#pragma once

namespace o2
{
	class base_command
	{
	public:
		virtual ~base_command()
		{
		}

		/**
		 * \brief abort this command
		 */
		virtual void abort() = 0;
	};
}