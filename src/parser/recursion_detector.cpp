//
// Part of the o2 Project, under the Apache License v2.0 with o2 Project Exceptions.
// See the LICENSE file in the project root for license terms
//

#include "recursion_detector.h"
#include "node.h"

using namespace o2;

const node* recursion_detector::find(const node* n) const
{
	// search upwards in the syntax tree to see if we found the supplied tree node. If so, then
	// return the first child item that caused the recursion in the first place.
	//
	// minimal recursion is: A -> B -> A

	// no parent? Then no recursion is possible
	if (parent == nullptr)
		return nullptr;

	const recursion_detector* prev = this;
	const recursion_detector* rt = parent;
	while (rt != nullptr)
	{
		if (rt->n == n)
		{
			return prev->n;
		}
		prev = rt;
		rt = rt->parent;
	}
	return nullptr;
}

void recursion_detector::raise_error(const node* n) const
{
	if (find(n))
		throw resolve_error_recursion(n->get_source_code(), n, this);
}
