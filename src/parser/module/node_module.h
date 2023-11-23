//
// Part of the o2 Project, under the Apache License v2.0 with o2 Project Exceptions.
// See the LICENSE file in the project root for license terms
//

#pragma once

#include "../node_symbol.h"

namespace o2
{
	class module;

	/**
	 * \brief class used to connect a module to the syntax tree
	 */
	class node_module final
			: public node_symbol
	{
	public:
		explicit node_module(module* m);

		/**
		 * \return the name of this module
		 */
		[[nodiscard]] string_view get_name() const
		{
			return _name;
		}

		/**
		 * \return the version of this module
		 */
		[[nodiscard]] string_view get_version() const;

		/**
		 * \param import_path
		 * \return the relative path
		 */
		[[nodiscard]] string_view get_relative_path(string_view import_path) const;

		/**
		 * \return the module
		 */
		[[nodiscard]] module* get_module() const
		{
			return _module;
		}

#pragma region node_symbol

		[[nodiscard]] string get_id() const final;

		bool compare_with_symbol(const node_module* rhs) const;

#pragma endregion

#pragma region node

		void on_parent_node(node* p) final;

		void debug(debug_ostream& stream, int indent) const final;

		void write_json_properties(json& j) override;

#pragma endregion

#pragma region json_serializable

		[[nodiscard]] string_view get_json_type() const override
		{
			return { "module" };
		}

#pragma endregion

	private:
		const string_view _name;
		module* const _module;
	};
}
