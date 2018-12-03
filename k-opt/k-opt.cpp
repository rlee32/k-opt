#include "fileio/PointSet.h"
#include "fileio/Tour.h"
#include "primitives.h"
#include "quadtree/Quadtree.h"
#include "quadtree/morton_keys.h"

#include <iostream>
#include <vector>

int main(int argc, char** argv)
{
    if (argc < 2)
    {
        std::cout << "Usage: point_set_file_path optional_tour_file_path" << std::endl;
        return 0;
    }
    fileio::PointSet point_set(argv[1]);
    quadtree::Quadtree quadtree;
    const auto keys = quadtree::morton_keys::compute_point_morton_keys(point_set.x(), point_set.y());
    // Initialize tour.
    std::vector<primitives::point_id_t> tour;
    if (argc > 2)
    {
        fileio::Tour initial_tour(argv[2]);
        tour = initial_tour.point_ids();
    }
    else
    {
        for (primitives::point_id_t i{0}; i < point_set.count(); ++i)
        {
            tour.push_back(i);
        }
    }
    const auto length = point_set.cycle_length(tour);
    std::cout << "Initial tour length: " << length << std::endl;
    // Initialize quadtree with segments.
    auto prev = tour.back();
    for (auto id : tour)
    {
        Segment s(prev, id, distance_functions::euc2d(point_set.x(), point_set.y(), prev, id));
        quadtree.insert(s, keys);
        prev = id;
    }
    bool local_optimum{false};
    while (not local_optimum)
    {
        const auto segments = quadtree.suboptimal_segments();
        local_optimum = segments.empty();
    }
    quadtree.iterate();
    return 0;
}
