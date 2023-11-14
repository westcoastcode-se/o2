//
// Part of the o2 Project, under the Apache License v2.0 with o2 Project Exceptions.
// See the LICENSE file in the project root for license terms
//

#pragma once

#include <utility>

#include "token.h"

namespace o2
{
	/**
	 * \brief a read-only view of the supplied source code
	 */
	class source_code
	{
	public:
		explicit source_code(string text)
				: _text(std::move(text)), _filename()
		{
		}

		source_code(string text, string filename)
				: _text(std::move(text)), _filename(std::move(filename))
		{
		}

		/**
		 * \return the actual source code
		 */
		inline string_view get_text() const
		{
			return _text;
		}

		/**
		 * \return a name that represents the source code. Usually it's filename
		 */
		inline string_view get_filename() const
		{
			return _filename;
		}

	private:
		const string _text;
		const string _filename;
	};
}