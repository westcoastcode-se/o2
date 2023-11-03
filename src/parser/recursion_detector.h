//
// Part of the o2 Project, under the Apache License v2.0 with o2 Project Exceptions.
// See the LICENSE file in the project root for license terms
//

#pragma once

namespace o2
{
	class node;

	/**
	 * \brief helper for detecting if we have nodes that refer to each other in a circular fashion
	 */
	class recursion_detector
	{
	public:
		const recursion_detector* const root;
		const recursion_detector* const parent;
		const node* const n;

		recursion_detector()
				: root(), parent(), n()
		{
		}

		recursion_detector(const recursion_detector* parent, const node* n)
				: root(parent->root), parent(parent), n(n)
		{
		}

		// \brief search for the supplied node
		const node* find(const node* n) const;

		// \brief raise an error if the supplied is already being parsed
		void raise_error(const node* n) const;

		/**
		 * \brief get the first rd instance that contains the supplied type
		 * \tparam T
		 * \return
		 */
		template<class T>
		const recursion_detector* first_of_type() const
		{
			auto impl = dynamic_cast<const T*>(n);
			if (impl)
				return this;
			if (parent)
				return parent->first_of_type<T>();
			return nullptr;
		}

		/**
		 * \brief get the next rd instance that contains the supplied type
		 * \tparam T
		 * \return
		 */
		template<class T>
		const recursion_detector* next_of_type() const
		{
			if (parent)
				return parent->first_of_type<T>();
			return nullptr;
		}
	};
}
