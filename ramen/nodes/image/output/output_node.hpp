// Copyright (c) 2010 Esteban Tovagliari.
// Licensed under the terms of the CDDL License.
// See CDDL_LICENSE.txt for a copy of the license.

#pragma once

#include <ramen/nodes/image/base_output_node.hpp>

namespace ramen
{
namespace image
{
class output_node_t : public base_output_node_t
{
  public:
    static const node_metaclass_t& output_node_metaclass();
    const node_metaclass_t*        metaclass() const override;

    output_node_t();

  protected:
    output_node_t(const output_node_t& other);
    void operator=(const output_node_t&);

  private:
    node_t* do_clone() const override { return new output_node_t(*this); }

    void do_create_params() override;

    void param_changed(param_t* p, param_t::change_reason reason);

    std::string extension_for_format(int format) const;
    void        set_format_for_extension(const std::string& ext);

    void do_calc_defined(const render::context_t& context) override;

    void write(const render::context_t& context) override;

    int priority() const override;
};

}  // namespace image
}  // namespace ramen
