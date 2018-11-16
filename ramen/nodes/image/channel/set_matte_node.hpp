// Copyright (c) 2010 Esteban Tovagliari

#pragma once

#include <ramen/nodes/image_node.hpp>

namespace ramen
{
namespace image
{
class set_matte_node_t : public image_node_t
{
public:
    static const node_metaclass_t& set_matte_node_metaclass();
    const node_metaclass_t*        metaclass() const override;

    set_matte_node_t();

    bool use_cache(const render::context_t& context) const override { return false; }

protected:
    set_matte_node_t(const set_matte_node_t& other)
    : image_node_t(other)
    {
    }
    void operator=(const set_matte_node_t&);

private:
    node_t* do_clone() const override { return new set_matte_node_t(*this); }

    void do_create_params() override;

    void do_calc_bounds(const render::context_t& context) override;

    void do_process(const render::context_t& context) override;
};

}  // namespace
}  // namespace

