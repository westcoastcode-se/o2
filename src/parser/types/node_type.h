//
// Part of the o2 Project, under the Apache License v2.0 with o2 Project Exceptions.
// See the LICENSE file in the project root for license terms
//

#pragma once

#include "../node_symbol.h"

namespace o2
{
	/**
	 * \brief base class for types, such as primitives and other types
	 */
	class node_type
			: public node_symbol
	{
	public:
		explicit node_type(const source_code_view& view);

		node_type(const source_code_view& view, int size);

		/**
		 * \return the actual type
		 */
		virtual node_type* get_type()
		{
			return this;
		}

		/**
		 * \brief resolve the size of this type
		 */
		virtual int resolve_size(const recursion_detector* rd) = 0;

		/**
		 * \return the size of this type
		 */
		inline int get_size() const
		{
			assert(_size >= 0 && "resolve_size has not be called yet");
			return _size;
		}

		/**
		 * \return true if the size is already known
		 */
		bool has_known_size() const
		{
			return _size >= 0;
		}

	protected:
		int _size;
	};
}