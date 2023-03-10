#ifndef INCLUDE_SPACE_PARTITIONING_OCTREE_HPP
#define INCLUDE_SPACE_PARTITIONING_OCTREE_HPP

#include <vector>
#include <cmath>
#include <exception>
#include <iterator>
#include <utility>
#include <tuple>

#include "shape.hpp"
#include "node.hpp"
#include "primitive_traits.hpp"

namespace yLab
{

namespace geometry
{

template<typename> class Octree;

namespace detail
{

template<std::input_iterator it>
auto calculate_octree_parameters (it first, it last)
{
    using distance_type = it::value_type::distance_type;
    using size_type = typename Octree<distance_type>::size_type;
    
    std::array<distance_type, 3> min{};
    std::array<distance_type, 3> max{};
    
    std::size_t n_shapes = 0;
    for (; first != last; ++first, ++n_shapes)
    {
        auto &bounding_volume = (*first).bounding_volume();
        auto &center = bounding_volume.center();

        for (auto i = 0; i != 3; ++i)
        {
            auto left_border_i = center[i] - bounding_volume.halfwidth(i);
            auto right_border_i = center[i] + bounding_volume.halfwidth(i);

            if (left_border_i < min[i])
                min[i] = left_border_i;

            if (right_border_i > max[i])
                max[i] = right_border_i;
        }
    }

    auto min_elem = std::min_element (min.begin(), min.end());
    auto max_elem = std::max_element (max.begin(), max.end());

    auto pt_coord = (*min_elem + *max_elem) / distance_type{2};
    auto halfwidht = (*max_elem - *min_elem) / distance_type{2};

    // Linear size of a shape is of scale 2 * halfwidth / cbrt (n_shapes)
    auto calc_height = static_cast<size_type>(std::ceil (cbrt (n_shapes) / 2.0));
    auto height = std::min (Octree<distance_type>::max_height(), calc_height);

    return std::tuple{Point_3D{pt_coord, pt_coord, pt_coord}, halfwidht, height};
}

} // namespace detail

struct Empty_Octree final: public std::runtime_error
{
    Empty_Octree () : std::runtime_error{"Constructing octree of height 0 is forbidden"} {}
};

template<typename T>
class Octree final
{
public:

    using distance_type = T;
    using point_type = typename Primitive_Traits<T>::point_type;
    using node_type = Octree_Node<distance_type>;
    using shape_type = typename node_type::shape_type;
    using size_type = std::size_t;

private:

    std::vector<node_type> nodes_;
    size_type height_;

public:
    
    template<std::input_iterator it>
    Octree (it first, it last)
    {
        auto [center, halfwidth, height] = detail::calculate_octree_parameters (first, last);
        if (height == 0)
            throw Empty_Octree{};
        else
            height_ = height;

        nodes_.reserve (max_size());
        build_subtree (center, halfwidth, height);

        insert (first, last);
    }

    size_type height () const noexcept { return height_; }
    static constexpr size_type max_height () noexcept { return 6; }

    size_type size () const noexcept { return nodes_.size(); }
    size_type max_size () const
    {
        return (static_cast<size_type>(std::exp2 (3 * height())) - 1) / (8 - 1);
    }

    // Modifiers

    void insert (const shape_type &shape)
    {
        detail::insert_shape (std::addressof (root()), shape);
    }

    template<std::input_iterator it>
    requires std::same_as<std::iter_value_t<it>, shape_type>
    void insert (it first, it last)
    {
        for (; first != last; ++first)
            insert (*first);
    }

    const node_type &root () const { return nodes_.front(); }
    node_type &root () { return nodes_.front(); }

private:

    node_type *build_subtree (const point_type &center, distance_type halfwidth, unsigned stop_depth)
    {
        node_type &subroot = nodes_.emplace_back(center, halfwidth);
        
        if (stop_depth > 1)
        {
            auto step = halfwidth * distance_type{0.5};
            for (int child_i = 0; child_i != 8; ++child_i)
            {
                point_type offset {(child_i & 1) ? step : -step,
                                   (child_i & 2) ? step : -step,
                                   (child_i & 4) ? step : -step};

                point_type new_center{center.x() + offset.x(), center.y() + offset.y(),
                                      center.z() + offset.z()};

                subroot.child (child_i) = build_subtree (new_center, step, stop_depth - 1);
            }
        }

        return std::addressof (subroot);
    }
};

} // namespace geometry

} // namespace yLab

#endif // INCLUDE_SPACE_PARTITIONING_OCTREE_HPP
