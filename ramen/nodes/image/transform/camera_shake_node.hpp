// Copyright (c) 2010 Esteban Tovagliari

#ifndef RAMEN_IMAGE_CAMERA_SHAKE_NODE_HPP
#define RAMEN_IMAGE_CAMERA_SHAKE_NODE_HPP

#include <ramen/nodes/image/xform2d_node.hpp>

namespace ramen
{
namespace image
{
class camera_shake_node_t : public xform2d_node_t
{
public:
    static const node_metaclass_t& camera_shake_node_metaclass();
    const node_metaclass_t*        metaclass() const override;

    camera_shake_node_t();

protected:
    camera_shake_node_t(const camera_shake_node_t& other);
    void operator=(const camera_shake_node_t&);

private:
    node_t* do_clone() const override { return new camera_shake_node_t(*this); }

    void do_create_params() override;

    void do_calc_hash_str(const render::context_t& context) override;

    matrix3_type do_calc_transform_matrix_at_frame(float frame, int subsample = 1) const override;
};

}  // namespace
}  // namespace

#endif
