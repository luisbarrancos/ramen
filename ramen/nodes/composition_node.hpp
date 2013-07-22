// Copyright (c) 2013 Esteban Tovagliari
// Licensed under the terms of the CDDL License.
// See CDDL_LICENSE.txt for a copy of the license.

#ifndef RAMEN_NODES_COMPOSITION_NODE_HPP
#define RAMEN_NODES_COMPOSITION_NODE_HPP

#include<ramen/nodes/node_fwd.hpp>

#include<ramen/nodes/composite_node.hpp>

#include<ramen/render/context.hpp>

namespace ramen
{

class composition_node_t : public composite_node_t
{
public:

    /// Constructor
    composition_node_t();

    // signals

    /// Emitted when a node is created.
    static boost::signals2::signal<void ( node_t*)> node_created;

    /// Emitted when a node is deleted.
    static boost::signals2::signal<void ( node_t*)> node_deleted;

    int start_frame() const;
    int end_frame() const;

    float frame() const;

    bool autokey() const;

    image::format_t default_format() const;

    int frame_rate() const;

    render::context_t current_context( render::render_mode mode = render::interface_render) const;

protected:

    composition_node_t( const composition_node_t& other);

private:

    void operator=( const composition_node_t& other);

    virtual node_t *do_clone() const;

    virtual void do_create_params();

    virtual void do_notify();
};

} // ramen

#endif
