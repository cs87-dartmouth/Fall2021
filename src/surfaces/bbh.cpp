/*
    This file is part of darts – the Dartmouth Academic Ray Tracing Skeleton.

    Copyright (c) 2017-2021 by Wojciech Jarosz
*/

#include <darts/progress.h>
#include <darts/sampling.h>
#include <darts/surface_group.h>
#include <future>

/** \addtogroup group_surfaces
 *  @{
 */

struct BBHNode;

/// An axis-aligned bounding box hierarchy acceleration structure
struct BBH : public SurfaceGroup
{
    shared_ptr<BBHNode> root;
    enum class SplitMethod
    {
        SAH,
        Middle,
        Equal
    } split_method    = SplitMethod::Middle;
    int max_leaf_size = 1;

    BBH(const json &j = json::object());

    /// Construct the BBH (must be called before @ref intersect)
    void build() override;

    /// Intersect a ray against all surfaces registered with the Accelerator
    bool intersect(const Ray3f &ray, HitInfo &hit) const override;
};


/// A node of an axis-aligned bounding box hierarchy
struct BBHNode : public Surface
{
    Box3f               bbox;           ///< The bounding box of this node
    shared_ptr<Surface> left_child;     ///< Pointer to left child
    shared_ptr<Surface> right_child;    ///< Pointer to right child

    BBHNode(vector<shared_ptr<Surface>> surfaces, Progress &progress, int depth = 0);

    ~BBHNode();

    bool intersect(const Ray3f &ray, HitInfo &hit) const override;

    Box3f bounds() const override
    {
        return bbox;
    }
};

BBHNode::BBHNode(vector<shared_ptr<Surface>> surfaces, Progress &progress, int depth)
{
    // TODO: Implement BVH construction, following chapter 2 of the book.
    // Hints:
    //     -- To get a random number in [0, n-1], you can use int(randf()*n)
    //     -- To sort a list of surfaces based on some sorting criterion, you can use std::sort and lambdas, like so:
    //
    //          std::sort(surfaces.begin(), surfaces.end(), [&](const Surface *a, const Surface *b) {
    //              return isASmallerThanB(a, b);
    //          });
    //
    //     For example, to sort them based on centroid along an axis, you could do
    //
    //          return a->bounds().center()[axis] < b->bounds().center()[axis]
    //
    //     -- To split a list (in C++: a 'vector') of things into two lists so that the first list has the first
    //     k elements of the original list, and the other list has the rest of the elements, you can do
    //          std::vector<Surface*> listA, listB
    //          listA.insert(listA.end(), originalList.begin(), originalList.begin() + k);
    //          listB.insert(listB.end(), originalList.begin() + k, originalList.end();
    //
    //     -- After construction, you need to compute the bounding box of this BVH node and assign it to bbox
    //     -- You can get the bounding box of a surface using surf->bounds();
    //     -- To take the union of two boxes, look at Box2f::enclose()
    put_your_code_here("Insert your BVH construction code here");
}


BBHNode::~BBHNode()
{
}

bool BBHNode::intersect(const Ray3f &ray_, HitInfo &hit) const
{
    // TODO: Implement BVH intersection, following chapter 2 of the book.
    put_your_code_here("Insert your BVH intersection code here");
    return false;
}

BBH::BBH(const json &j) : SurfaceGroup(j)
{
    // These values aren't used in the base code right now - but you can use these for when you want to extend the basic
    // BVH functionality

    max_leaf_size = j.value("max_leaf_size", max_leaf_size);

    string sm = j.value("split_method", "equal");
    if (sm == "sah")
        // Surface-area heuristic
        split_method = SplitMethod::SAH;
    else if (sm == "middle")
        // Split at the center of the bounding box
        split_method = SplitMethod::Middle;
    else if (sm == "equal")
        // Split so that an equal number of objects are on either side
        split_method = SplitMethod::Equal;
    else
    {
        spdlog::error("Unrecognized split_method \"{}\". Using \"equal\" instead.", sm);
        split_method = SplitMethod::Equal;
    }
}

void BBH::build()
{
    pcg32 rng;
    Progress progress("Building BBH", m_surfaces.size());
    if (!m_surfaces.empty())
        root = make_shared<BBHNode>(m_surfaces, progress);
    else
        root = nullptr;
    progress.set_done();
    spdlog::info("BBH contains {} surfaces.", m_surfaces.size());
}

bool BBH::intersect(const Ray3f &ray, HitInfo &hit) const
{
    return root ? root->intersect(ray, hit) : false;
}

DARTS_REGISTER_CLASS_IN_FACTORY(SurfaceGroup, BBH, "bbh")


/**
    \file
    \brief Class #BBH, #BBHNode, and #BBHBuildRecord
*/

/** @}*/