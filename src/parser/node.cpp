//
// Part of the o2 Project, under the Apache License v2.0 with o2 Project Exceptions.
// See the LICENSE file in the project root for license terms
//

#include "node.h"

using namespace o2;

node::~node()
{
	node::destroy_children();
}

vector<node*> node::get_younger_siblings() const
{
	if (_parent == nullptr)
		return {};

	const auto children = _parent->get_children();
	if (children.size() < 2)
		return {};

	vector<node*> siblings;
	for (auto c: children)
	{
		if (c == this)
			break;
		siblings.add(c);
	}
	return siblings;
}

node* node::replace_child(int idx, node* n)
{
	const auto prev = _children[idx];
	on_child_removed(prev);
	// is this node already part of the tree somewhere, then remove it
	if (n->get_parent())
		n->get_parent()->remove_child(n);
	_children[idx] = n;
	on_child_added(n);
	n->set_parent(this);
	return prev;
}

node* node::replace_children(int& idx, array_view<node*> nodes)
{
	const auto prev = _children.remove_at(idx);
	on_child_removed(prev);
	idx += _children.insert(nodes, idx);
	for (auto c: nodes)
		on_child_added(c);
	return prev;
}

void node::destroy_children()
{
	for (auto n: _children)
		delete n;
	_children.clear();
}

void node::debug(debug_ostream& stream, int indent) const
{
	for (auto c: _children)
		c->debug(stream, indent + 1);
}

void node::process_phase(const recursion_detector* rd, resolve_state* state, int phase)
{
	if (!has_phase_left(phase))
	{
		// push the node so that it will be processed in the next phase
		if (has_phases_left())
			state->add_next_phase(this);
		return;
	}
	// check for recursions
	rd->raise_error(this);
	// actually perform the phase logic
	switch (phase)
	{
	case phase_resolve:
		resolve0(rd, state);
		break;
	default:
		on_process_phase(rd, state, phase);
		break;
	}
	// we are now done with processing a phase from this node
	_phases_left = bit_unset(_phases_left, phase);
	// push the node so that it will be processed in the next phase
	if (has_phases_left())
		state->add_next_phase(this);
}

void node::resolve0(const recursion_detector* rd, resolve_state* state)
{
	const recursion_detector rd0(rd, this);
	for (auto c: _children)
		c->process_phase(&rd0, state, phase_resolve);
}

vector<node*> node::optimize(node_optimizer* optimizer)
{
	auto children = get_children();
	for (int i = 0; i < children.size(); ++i)
	{
		const auto optimized = children[i]->optimize(optimizer);
		if (optimized.empty())
			continue;
		if (optimized.size() == 1)
			delete replace_child(i, optimized[0]);
		else
		{
			delete replace_children(i, optimized);
			i--;
			children = get_children();
		}
	}

	if (optimizer->accept(this))
		return optimizer->optimize(this);
	return {};
}

void node::set_parent(node* p)
{
	if (p == nullptr)
	{
		const auto temp = _parent;
		_parent = nullptr;
		on_removed_parent_node(temp);
		return;
	}

	_parent = p;
	on_parent_node(p);
}

node* node::add_child(node* n)
{
	if (n->get_parent())
		n->get_parent()->remove_child(n);
	_children.add(n);
	n = on_child_added(n);
	assert(n != nullptr);
	n->set_parent(this);
	return n;
}

void node::add_children(array_view<node*> children)
{
	for (auto c: children)
		add_child(c);
}

void node::remove_child(node* n)
{
	const auto idx = _children.find(n);
	_children.remove_at(idx);
	on_child_removed(n);
	n->set_parent(nullptr);
}

void node::query(query_node_visitor* visitor, int flags)
{
	visitor->visit(this);

	flags = limit_query_flags(flags);
	query_children(visitor, flags);
	query_younger_siblings(visitor, flags);
	query_parents(visitor, flags);
}

void node::query_children(query_node_visitor* visitor, int flags)
{
	if (bit_isset(flags, query_flag_children_from_root) && _parent == nullptr)
		flags = bit_set(flags, query_flag_children);

	if (!bit_isset(flags, query_flag_children))
		return;

	// if we are querying downwards then don't allow for querying more children, unless
	// that node, specifically allows for it - such as the import node
	if (bit_isset(flags, query_flag_downwards))
		flags = bit_unset(flags, query_flag_children);

	// we are querying all siblings since they are querying all children
	flags = bit_unset(flags, query_flag_younger_siblings);
	// prevent querying parents in case of infinite recursion
	flags = bit_unset(flags, query_flag_parents);
	// we are querying downwards now
	flags = bit_set(flags, query_flag_downwards);
	// we are not querying upwards anymore
	flags = bit_unset(flags, query_flag_upwards);
	for (const auto c: _children)
		c->query(visitor, flags);
}

void node::query_parents(query_node_visitor* visitor, int flags)
{
	if (_parent == nullptr)
		return;

	if (!bit_isset(flags, query_flag_parents))
		return;

	// do not allow for a parent to also query it's children. This is to prevent recursive querying
	flags = bit_unset(flags, query_flag_children);
	// we are querying upwards now
	flags = bit_set(flags, query_flag_upwards);
	// we are not querying downwards anymore
	flags = bit_unset(flags, query_flag_downwards);

	_parent->query(visitor, flags);
}

void node::query_younger_siblings(query_node_visitor* visitor, int flags)
{
	if (!bit_isset(flags, query_flag_younger_siblings))
		return;

	// don't query parents or siblings children
	flags = bit_unset(flags, query_flag_parents | query_flag_children);
	// we aren't querying down- or upwards
	flags = bit_unset(flags, query_flag_upwards | query_flag_downwards);
	// no need to query siblings since we are already visiting younger siblings
	flags = bit_unset(flags, query_flag_younger_siblings);
	// don't follow references
	flags = bit_unset(flags, query_flag_follow_refs);

	const auto siblings = get_younger_siblings();
	for (const auto s: siblings)
		s->query(visitor, flags);
}

void node::visit(query_node_visitor* visitor)
{
	visitor->visit(this);
	for (auto c: _children)
		c->visit(visitor);
}
