//
// Part of the o2 Project, under the Apache License v2.0 with o2 Project Exceptions.
// See the LICENSE file in the project root for license terms
//

#pragma once

#include "json.h"

namespace o2
{
	/**
	 * \brief base class for serializing json types
	 */
	class json_serializable
	{
	public:
		virtual ~json_serializable()
		{
		}

		/**
		 * \brief write this object down as a json object
		 * \param j where to put the json content into
		 */
		void write_json(json& j);

		/**
		 * \brief write this object's properties, but ignore children
		 * \param j
		 */
		virtual void write_json_properties(json& j);

		/**
		 * \return a string that represents this type
		 */
		[[nodiscard]] virtual string_view get_json_type() const
		{
			return { "json_serializable" };
		}
	};
}