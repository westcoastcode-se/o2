//
// Part of the o2 Project, under the Apache License v2.0 with o2 Project Exceptions.
// See the LICENSE file in the project root for license terms
//

#pragma once

#include "token.h"
#include "error.h"
#include "source_code_view.h"
#include "collections/vector.h"
#include "memory.h"
#include "bit.h"
#include "recursion_detector.h"
#include <sstream>

namespace o2
{
	class recursion_detector;

	/**
	 * \brief a helper structure that ensures that memory is de-allocated when errors occurs
	 * \tparam T the type
	 */
	template<class T>
	struct memory_guard
	{
		T* ptr;

		memory_guard() : ptr()
		{
		}

		memory_guard(memory_guard<T>&& rhs) noexcept: ptr(rhs.done())
		{
		}

		memory_guard(T* ptr) : ptr(ptr)
		{
		}

		~memory_guard()
		{
			delete ptr;
		}

		/**
		 * \brief we are done with the object and don't want to delete it
		 * \return a pointer to the object
		 */
		T* done()
		{
			auto temp = ptr;
			ptr = nullptr;
			return temp;
		}

		/**
		 * \return the pointer
		 */
		T* get() const
		{
			return ptr;
		}

		/**
		 * \param new_value the new value
		 */
		void set(T* new_value)
		{
			ptr = new_value;
		}

		memory_guard<T>& operator=(memory_guard<T>& rhs)
		{
			ptr = rhs.done();
			return *this;
		}

		template<class H>
		memory_guard<T>& operator=(const memory_guard<H>& rhs)
		{
			ptr = const_cast<memory_guard<H>*>(&rhs)->done();
			return *this;
		}

		T* operator->() const
		{
			return ptr;
		}
	};

	class node;

	/**
	 * \brief optimizer interface
	 */
	class node_optimizer
	{
	public:
		virtual ~node_optimizer()
		{
		}

		/**
		 * \brief check to see if the supplied node is accepted by this optimizer
		 * \param n the node
		 * \return true if this optimizer is able to optimize the supplied node
		 */
		virtual bool accept(const node* n) = 0;

		/**
		 * \brief optimize the supplied node and return one or more nodes to replace this node
		 * \param n 
		 * \return 
		 */
		virtual vector<node*> optimize(node* n) = 0;
	};

	/**
	 * \brief visitor pattern interface for
	 */
	class query_node_visitor
	{
	public:
		virtual ~query_node_visitor() = default;

		/**
		 * \brief method called when a node is visited while querying for nodes
		 * \param n the node we are visiting
		 * \return what to do next
		 */
		virtual void visit(node* n) = 0;
	};

	/**
		 * \brief base class for all nodes in the syntax tree
	 */
	class node
			: public memory_tracked
	{
	public:
		node(const source_code_view& view)
				: node(view, 0)
		{
		}

		node(const source_code_view& view, int access_modifier)
				: _source_code(view), _parent(), _query_access_modifiers(access_modifier)
		{
		}

		virtual ~node();

		/**
		 * \return get the root node
		 */
		inline node* get_root_node()
		{
			if (_parent)
				return _parent->get_root_node();
			return this;
		}

		/**
		 * \return the parent node
		 */
		node* get_parent() const
		{
			return _parent;
		}

		/**
		 * \return all children under this node
		 */
		array_view<node*> get_children() const
		{
			return _children;
		}

		/**
		 * \brief get a child
		 * \param idx the child
		 * \return the child at the supplied index. nullptr if child node doesn't exist
		 */
		node* get_child(int idx) const
		{
			if (_children.size() > idx)
				return _children[idx];
			return nullptr;
		}

		/**
		 * \return number of children under this node
		 */
		int get_child_count() const
		{
			return _children.size();
		}

		/**
		 * \return all siblings that are younger
		 */
		vector<node*> get_younger_siblings() const;

		/**
		 * \return the source code this node is generated from
		 */
		const source_code_view& get_source_code() const
		{
			return _source_code;
		}

		/**
		 * \brief set a new parent for this node
		 * \param p  the parent node
		 */
		void set_parent(node* p);

		/**
		 * \brief method called when a new parent is set
		 * \param p
		 */
		virtual void on_parent_node(node* p)
		{
		}

		/**
		 * \brief method called when the supplied parent is removed
		 * \param p the node that we are removing
		 */
		virtual void on_removed_parent_node(node* p)
		{
		}

		/**
		 * \brief add a child node
		 * \param n
		 * \return the added child
		 */
		node* add_child(node* n);

		/**
		 * \brief add an array of children
		 * \param children
		 */
		void add_children(array_view<node*> children);

		/**
		 * \brief method called when a child node is added
		 * \param n the node
		 */
		virtual node* on_child_added(node* n)
		{
			return n;
		}

		/**
		 * \brief remove a child from this node
		 * \param n the child
		 */
		void remove_child(node* n);

		/**
		 * \brief method called when a child node is removed
		 * \param n the node
		 */
		virtual void on_child_removed(node* n)
		{
		}

		/**
		 * \brief replace the node at the supplied index and return the old node
		 * \param idx the index
		 * \param n the new node
		 */
		node* replace_child(int idx, node* n);

		/**
		 * \brief replace the node with the list of nodes
		 * \param idx 
		 * \param nodes 
		 */
		node* replace_children(int& idx, array_view<node*> nodes);

		/**
		 * \brief destroy all child-nodes
		 */
		void destroy_children();

		/**
		 * \brief debug output to the stdout
		 * \param stream
		 * \param indent
		 */
		virtual void debug(std::basic_ostream<char>& stream, int indent) const;

		/**
		 * \brief resolve all references
		 * \param rd
		 * \return
		 */
		virtual bool resolve(const recursion_detector* rd);

		/**
		 * \brief optimize this node using the supplied optimizer
		 * \param optimizer
		 * \return new nodes to replace this node
		 */
		virtual vector<node*> optimize(node_optimizer* optimizer);

		/**
		 * \brief querying nodes from a specific node's point-of-view
		 */
		enum query_flags
		{
			/**
			 * \brief querying is done upwards
			 */
			query_flag_upwards = 1 << 0,

			/**
			 * \brief querying is done downwards
			 */
			query_flag_downwards = 1 << 1,

			/**
			 * \brief include parents in their query
			 */
			query_flag_parents = 1 << 2,

			/**
			 * \brief query all siblings that's created before the one doing the query
			 * 
			 * this is usually done when querying inside a function body, where you look for
			 * declared identifiers made before
			 */
			query_flag_younger_siblings = 1 << 3,

			/**
			 * \brief visit the node's first line of children
			 */
			query_flag_children = 1 << 4,

			/**
			 * \brief query first line of children when reaching a root node
			 */
			query_flag_children_from_root = 1 << 5,

			/**
			 * \brief query directly from the root instead
			 */
			query_flag_from_root = 1 << 6,

			/**
			 * \brief follow resolved references when querying
			 */
			query_flag_follow_refs = 1 << 7,

			/**
			 * \brief mask that can be used by classes inheriting from the node
			 */
			query_flag_next_mask = 1 << 8,
		};

		/**
		 * \brief query nodes in the tree that this node is allowed to see
		 * \param visitor
		 * \param flags
		 * \return
		 */
		virtual void query(query_node_visitor* visitor, int flags);

		/**
		 * \brief query all children from this node's point of view
		 * \param visitor
		 * \param flags
		 * \return
		 */
		void query_children(query_node_visitor* visitor, int flags);

		/**
		 * \brief query all parents
		 * \param visitor
		 * \param flags
		 * \return
		 */
		void query_parents(query_node_visitor* visitor, int flags);

		/**
		 * \brief query all younger siblings
		 * \param visitor
		 * \param flags
		 * \return
		 */
		void query_younger_siblings(query_node_visitor* visitor, int flags);

		/**
		 * \brief visit this node all all it's children
		 * \param visitor
		 */
		void visit(query_node_visitor* visitor);

		/**
		 * \brief a way to modify query flags when querying
		 */
		enum query_access_modifiers
		{
			/**
			 * \brief stop querying siblings
			 *
			 * This is useful when you leave a function and only queries nodes in the entire scope
			 */
			query_access_modifier_no_siblings = 1 << 0,

			/**
			 * \brief this node allows for passthrough when querying downwards
			 */
			query_access_modifier_passthrough = 1 << 1,

			/**
			 * \brief mask that can be used by classes inheriting from the node
			 */
			query_access_modifier_next_mask = 1 << 2,
		};

		/**
		 * \return the flags used when preventing access during querying
		 */
		int get_query_access_modifiers() const
		{
			return _query_access_modifiers;
		}

		/**
		 * \param flags the flags used we want to modify using the access modifiers
		 */
		void set_query_access_flags(int flags)
		{
			_query_access_modifiers = flags;
		}

		/**
		 * \param query_flags
		 * \return converted query flags based on what is allowed using the locked flags
		 */
		int limit_query_flags(int query_flags) const
		{
			// you are no longer allowed to search for siblings from this point on
			if (bit_isset(_query_access_modifiers, query_access_modifier_no_siblings))
				query_flags = bit_unset(query_flags, query_flag_younger_siblings);
			// This node allows for downwards passthrough
			if (bit_isset(_query_access_modifiers, query_access_modifier_passthrough) &&
				bit_isset(query_flags, query_flag_downwards))
				query_flags = bit_set(query_flags, query_flag_children);
			return query_flags;
		}

		/**
		 * \tparam T the type
		 * \return the closest parent of a specific type
		 */
		template<class T>
		T* get_parent_of_type()
		{
			const auto p = dynamic_cast<T*>(_parent);
			if (p != nullptr || _parent == nullptr)
				return p;
			return _parent->get_parent_of_type<T>();
		}

	protected:
		static std::string in(int indent)
		{
			std::string s;
			for (int i = 0; i < indent; ++i)
				s += "  ";
			return s;
		}

	private:
		const source_code_view _source_code;
		node* _parent;
		vector<node*> _children;
		int _query_access_modifiers;
	};
}