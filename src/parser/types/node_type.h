//
// Part of the o2 Project, under the Apache License v2.0 with o2 Project Exceptions.
// See the LICENSE file in the project root for license terms
//

#pragma once

#include "../node_symbol.h"

namespace o2
{
	/**
	 * \brief how compatible one type is from another
	 */
	enum class compatibility : int
	{
		// the two types are identical
		identical = 0,

		// type is upcast-compatible. For example a byte array is upcast compatible with a pointer
		// to byte memory or a struct can be upcasted to it's inherited interface.
		//
		// upcasting is done automatically by the compiler
		upcast,

		// there are, potentially, a way in which a type can be casted to the new type. For example, an
		// interface can be downcast to a struct that implements that interface.
		//
		// this is not done automatically by the compiler but has, instead, to be done by the developer
		downcast,

		// not compatible
		incompatible,
	};

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
		[[nodiscard]] int get_size() const
		{
			assert(_size >= 0 && "resolve_size has not be called yet");
			return _size;
		}

		/**
		 * \return true if the size is already known
		 */
		[[nodiscard]] bool has_known_size() const
		{
			return _size >= 0;
		}

		/**
		 * \param rhs
		 * \return information on if the supplied type is compatible with this type. I
		 */
		virtual compatibility is_compatible_with(node_type* rhs) const
		{
			return compatibility::incompatible;
		}

	protected:
		int _size;
	};
}