//
// Part of the o2 Project, under the Apache License v2.0 with o2 Project Exceptions.
// See the LICENSE file in the project root for license terms
//

#pragma once

#include "token.h"
#include "source_code.h"

namespace o2
{
	/**
	 * \brief a read-only view of the supplied source code
	 */
	class source_code_view
	{
	public:
		source_code_view()
				: _source_code(), _line(), _line_offset(), _line_start(),
				  _offset()
		{
		}

		source_code_view(const source_code* src, const token* t)
				: _source_code(src), _line(t->line()), _line_offset(t->line_offset()), _line_start(t->line_start()),
				  _offset(t->offset())
		{
		}

		inline const source_code* get_source_code() const
		{
			return _source_code;
		}

		inline int get_line() const
		{
			return _line;
		}

		inline int get_line_offset() const
		{
			return _line_offset;
		}

		inline const char* get_line_start() const
		{
			return _line_start;
		}

		inline int get_offset() const
		{
			return _offset;
		}

	private:
		const source_code* _source_code;
		const int _line;
		const int _line_offset;
		const char* _line_start;
		const int _offset;
	};
}