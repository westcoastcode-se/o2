//
// Part of the o2 Project, under the Apache License v2.0 with o2 Project Exceptions.
// See the LICENSE file in the project root for license terms
//

#pragma once

#include "../node_type.h"
#include "node_type_complex_field.h"
#include "node_type_complex_methods.h"
#include "node_type_complex_inherits.h"
#include "../static/node_type_static_scope.h"

namespace o2
{
	/**
	 * \brief complex types allowed by the language
	 */
	enum class complex_type
	{
		// the type is struct
		struct_,
		// the type is a class
		class_,
		// the type is an interface
		interface_,
		// the type is a trait
		trait_,
		// the type is unknown, for now.
		unknown_
	};

	enum class complex_modifiers
	{
		// abstract modifier
		abstract_ = 1 << 0
	};

	/**
	 * \brief a complex type
	 *
	 * Complex types can be one of the following sub-types:
	 * - struct
	 * - class
	 * - interface
	 * - traits
	 *
	 * There are some limitations though:
	 * - If the type is a struct then you are only allowed to inherit from other structs
	 * - If the type is a class, then you are only allowed to inherit from other classes and interfaces
	 * - If a class inherits from another class, then that class are not allowed to inherit from more classes
	 */
	class node_type_complex
			: public node_type
	{
	public:
		node_type_complex(const source_code_view& view, string_view name);

		/**
		 * \return the name of the struct
		 */
		[[nodiscard]] string_view get_name() const
		{
			return _name;
		}

		/**
		 * \return what type of complex type this is
		 */
		[[nodiscard]] complex_type get_comple_type() const
		{
			return _type;
		}

		/**
		 * \return true if this type is an interface
		 */
		[[nodiscard]] bool is_interface() const
		{
			return _type == complex_type::interface_;
		}

		/**
		 * \return true if this type is a trait
		 */
		[[nodiscard]] bool is_trait() const
		{
			return _type == complex_type::trait_;
		}

		/**
		 * \return true if this type is a struct
		 */
		[[nodiscard]] bool is_struct() const
		{
			return _type == complex_type::struct_;
		}

		/**
		 * \return true if this type is a class
		 */
		[[nodiscard]] bool is_class() const
		{
			return _type == complex_type::class_;
		}

		/**
		 * \return inheritances
		 */
		[[nodiscard]] node_type_complex_inherits* get_inherits() const
		{
			return _inherits;
		}

		/**
		 * \return fields part of this struct
		 */
		[[nodiscard]] node_type_complex_fields* get_fields() const
		{
			return _fields;
		}

		/**
		 * \return all fields part of this struct - included from all inherited structures
		 */
		[[nodiscard]] vector<node_type_complex_field*> get_all_fields() const;

		/**
		 * \return methods container
		 */
		[[nodiscard]] node_type_complex_methods* get_methods() const
		{
			return _methods;
		}

		/**
		 * \return static container
		 */
		[[nodiscard]] node_type_static_scope* get_static() const
		{
			return _static;
		}

		/**
		 * \brief search for an inherited type that matches the supplied symbol id
		 * \param id the unique symbol id
		 * \return
		 */
		[[nodiscard]] node_type* find_inherited_type(string_view id) const
		{
			if (_inherits == nullptr)
				return nullptr;
			return _inherits->find_inherited_type(id);
		}

		/**
		 * \param type
		 * \return true if this struct inherits from the supplied type
		 */
		[[nodiscard]] bool inherits_from_type(node_type* type) const
		{
			if (_inherits == nullptr)
				return false;
			return _inherits->inherits_from_type(type);
		}

		/**
		 * \brief set the complex type
		 * \param type the type we are assuming this type is
		 *
		 * this will be verified after the resolve phase
		 */
		void set_complex_type(complex_type type)
		{
			_type = type;
		}

#pragma region node_type

		compatibility is_compatible_with(node_type* rhs) const override;

#pragma endregion

#pragma region node_symbol

		[[nodiscard]] string get_id() const final;

		bool superficial_test_symbol_collision(const node_type_complex* rhs) const;

#pragma endregion

#pragma region node

		void debug(debug_ostream& stream, int indent) const final;

		node* on_child_added(node* n) final;

		void on_child_removed(node* n) final;

		void on_parent_node(node* p) final;

		void write_json_properties(json& j) final;

		void on_process_phase(const recursion_detector* rd, resolve_state* state, int phase) final;

#pragma endregion

#pragma region json_serializable

		[[nodiscard]] string_view get_json_type() const override
		{
			return { "type_struct" };
		}

#pragma endregion

	private:
		/**
		 * \brief put all fields part of this struct - included from all inherited structures
		 * \param dest where the fields are put
		 */
		void get_all_fields(vector<node_type_complex_field*>* dest) const;

	private:
		const string_view _name;
		complex_type _type;
		node_type_complex_inherits* _inherits;
		node_type_complex_fields* _fields;
		node_type_complex_methods* _methods;
		node_type_static_scope* _static;
	};
}

namespace std
{
	inline ostream& operator<<(ostream& os, const o2::complex_type& ct)
	{
		switch (ct)
		{
		case o2::complex_type::struct_:
			return os << "struct_";
		case o2::complex_type::class_:
			return os << "class_";
		case o2::complex_type::interface_:
			return os << "interface_";
		case o2::complex_type::trait_:
			return os << "trait_";
		case o2::complex_type::unknown_:
			return os << "unknown_";
		default:
			return os << "???";
		}
	}
}
