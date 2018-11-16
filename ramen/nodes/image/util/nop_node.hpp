// Copyright (c) 2011 Esteban Tovagliari

#pragma once

#include <ramen/nodes/image_node.hpp>

namespace ramen
{
namespace image
{
class nop_node_t : public image_node_t
{
  public:
    static const node_metaclass_t& nop_node_metaclass();
    const node_metaclass_t*        metaclass() const override;

    nop_node_t();

    bool use_cache(const render::context_t& context) const override
    {
        return false;
    }

  protected:
    nop_node_t(const nop_node_t& other)
      : image_node_t(other)
    {
    }
    void operator=(const nop_node_t&);

  private:
    node_t* do_clone() const override { return new nop_node_t(*this); }

    bool do_is_identity() const override { return true; }
};

}  // namespace image
}  // namespace ramen
