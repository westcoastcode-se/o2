//
// Part of the o2 Project, under the Apache License v2.0 with o2 Project Exceptions.
// See the LICENSE file in the project root for license terms
//

#pragma once

#include "node_type.h"
#include "../primitive_value.h"
#include <llvm/IR/Type.h>

namespace o2
{
	class node_type_primitive final
			: public node_type
	{
	public:
		node_type_primitive(const vector<string_view>& names, int size,
				primitive_type pttype,
				llvm::Type* type);

		/**
		 * \return the name of the primitive
		 */
		[[nodiscard]] string_view get_name() const
		{
			return _names[0];
		}

		/**
		 * \return all names that matches this primitive
		 */
		[[nodiscard]] array_view<string_view> get_names() const
		{
			return _names;
		}

#pragma region node_type

		compatibility is_compatible_with(node_type* rhs) const final;

#pragma endregion

#pragma region node_symbol

		[[nodiscard]] string get_id() const final;

#pragma endregion

#pragma region node

		void debug(debug_ostream& stream, int indent) const final;

		void write_json_properties(json& j) final;

#pragma endregion

#pragma region json_serializable

		[[nodiscard]] string_view get_json_type() const override
		{
			return { "primitive" };
		}

#pragma endregion

	private:
		vector<string_view> _names;
		const primitive_type _primitive_type;
		llvm::Type* const _llvm_type;
	};

}