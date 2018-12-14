#pragma once

#include "quadtree/QuadtreeNode.h"

#include <iterator>
#include <vector>

namespace optimizer {

class SegmentIterator
{
    using SegmentContainer = quadtree::QuadtreeNode::SegmentContainer;
    using segment_iterator = SegmentContainer::const_iterator;
public:
    SegmentIterator(const quadtree::QuadtreeNode* root, bool skip_root = false);

    SegmentIterator& operator++();
    SegmentIterator operator++(int);

private:
    const quadtree::QuadtreeNode* m_end{nullptr};
    const quadtree::QuadtreeNode* m_current{nullptr};
    segment_iterator m_segment;

    void skip_empty_nodes();
    void next_node();
};

} // namespace optimizer
