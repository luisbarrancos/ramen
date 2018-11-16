// Copyright (c) 2010 Esteban Tovagliari

#pragma once

#include <ramen/nodes/image/xform2d_node.hpp>

namespace ramen
{
namespace image
{
class move2d_node_t : public xform2d_node_t
{
public:
    static const node_metaclass_t& move2d_node_metaclass();
    const node_metaclass_t*        metaclass() const override;

    move2d_node_t();

protected:
    move2d_node_t(const move2d_node_t& other);
    void operator=(const move2d_node_t&);

private:
    node_t* do_clone() const override { return new move2d_node_t(*this); }

    void do_create_params() override;
    void param_changed(param_t* p, param_t::change_reason reason);

    void do_create_manipulators() override;

    matrix3_type do_calc_transform_matrix_at_frame(float frame, int subsample = 1) const override;
};

}  // namespace
}  // namespace

