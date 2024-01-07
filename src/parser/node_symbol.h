//
// Part of the o2 Project, under the Apache License v2.0 with o2 Project Exceptions.
// See the LICENSE file in the project root for license terms
//

#pragma once

#include "node.h"
#include "node_attribute.h"

namespace o2
{
	/**
	 * \brief base class for symbols that are not allowed to exist multiple times in the same scope
	 *
	 * all symbols have a unique id. For example a type might have a symbol id ".mycompany.MyType" or
	 * ".mycompany.MyMacroType<mycompany.Node>"
	 */
	class node_symbol
			: public node
	{
	public:
		// perform deeper collision test during the resolve phase
		static constexpr int phase_deep_collision_test = phase_next_bit << 0;

		enum accessor
		{
			// allow everyone to access this symbol
			accessor_public = 1,
			// only access to this symbol from the current module
			accessor_module,
			// only allow access from the same package
			accessor_private
		};

		/**
		 * \param view
		 * \param name the name of this symbol
		 */
		explicit node_symbol(const source_code_view& view)
				: node_symbol(view, accessor_public)
		{
		}

		/**
		 * \param view
		 * \param name the name of this symbol
		 * \param accessor whom are allowed to access this symbol
		 */
		node_symbol(const source_code_view& view, accessor accessor)
				: node(view), _accessor(accessor), _attributes()
		{
		}

		/**
		 * \return all attributes attached to this symbol
		 */
		[[nodiscard]] node_attributes* get_attributes() const
		{
			return _attributes;
		}

		/**
		 * \return a unique id for this symbol
		 */
		[[nodiscard]] virtual string get_id() const = 0;

		/**
		 * \return whom are allowed to access this symbol
		 */
		[[nodiscard]] accessor get_accessor() const
		{
			return _accessor;
		}

		/**
		 * \return true if the current node is allowed to access this symbol
		 */
		bool is_allowed(node* n) const;

		/**
		 * \brief helper type used when searching for collisions
		 * \tparam T
		 */
		template<class T>
		class superficial_collision_test_visitor final
				: public query_node_visitor
		{
		public:
			T* const _this;
			// TODO allow for stop querying in a more controlled manner
			bool _done;

			explicit superficial_collision_test_visitor(T* t)
					: _this(t), _done(false)
			{
			}

			void visit(node* n) final
			{
				if (n == _this || _done)
					return;

				const auto tt = dynamic_cast<const T*>(n);
				if (tt == nullptr)
					return;

				if (_this->superficial_test_symbol_collision(tt))
				{
					_done = true;
					_this->add_phases_left(phase_deep_collision_test);
				}
			}
		};

		/**
		 * \brief helper function for performing a superficial collision test
		 * \param t send in this from the inherited class
		 * \tparam T the type of node we want to test
		 * \param U the type of base class that we are allowing, must be at least node_symbol
		 *
		 * This method is normally called during the parse phase. It helps with, quickly, figuring out if there is a
		 * collision. For example, if there's no function with the same name then we know, for sure, that no collision
		 * occurs.
		 *
		 * You must implement the method "bool superficial_test_symbol_collision(const T*) const". Returning true will
		 * force the compiler to perform deeper collision detection during the resolve phase. You are also allowed to
		 * throw an exception in this method if a collision can be detected during the parse phase.
		 *
		 * If superficial_test_symbol_collision is allowed to return true, then you must call deep_collision_test
		 * in the resolve phase and implement the necessary methods related to it
		 */
		template<class T, class U = T>
		void superficial_collision_test(T* t)
		{
			superficial_collision_test_visitor<U> v(t);
			const auto parent = get_parent_of_type<node_symbol>();
			parent->query(&v, query_flag_children | query_flag_downwards);
		}

		/**
		 * \brief helper type used when searching for collisions
		 * \tparam T
		 */
		template<class T>
		class deep_collision_test_visitor final
				: public query_node_visitor
		{
		public:
			const T* const _this;

			explicit deep_collision_test_visitor(const T* t)
					: _this(t)
			{
			}

			void visit(node* n) final
			{
				if (n == _this)
					return;

				const auto tt = dynamic_cast<const T*>(n);
				if (tt == nullptr)
					return;

				_this->deep_test_symbol_collision(tt);
			}
		};

		/**
		 * \brief helper function for a deep collision detection test
		 * \tparam T
		 * \tparam U
		 * \param t
		 *
		 * This method is normally called during the resolve phase. This method is expected to be called during the
		 * resolve phase if "phase_deep_collision_test" is set.
		 *
		 * You must implement the method "void deep_test_symbol_collision(const T*) const". You are expected to
		 * throw an exception in this method if a collision is detected
		 */
		template<class T, class U = T>
		void deep_collision_test(T* t)
		{
			if (!has_phase_left(phase_deep_collision_test))
				return;
			deep_collision_test_visitor<U> v(t);
			const auto parent = get_parent_of_type<node_symbol>();
			parent->query(&v, query_flag_children | query_flag_downwards);
			t->remove_phases_left(phase_deep_collision_test);
		}

#pragma region node

		node* on_child_added(node* n) override;

		void on_child_removed(node* n) override;

		void write_json_properties(json& j) override;

#pragma endregion

	private:
		accessor _accessor;
		node_attributes* _attributes;
	};
}
