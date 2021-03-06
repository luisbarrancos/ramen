// Copyright (c) 2010 Esteban Tovagliari
// Licensed under the terms of the CDDL License.
// See CDDL_LICENSE.txt for a copy of the license.

#include <ramen/params/parameterised.hpp>

#include <ramen/app/composition.hpp>

#include <ramen/anim/track.hpp>

#include <ramen/serialization/yaml_oarchive.hpp>

#include <ramen/util/string.hpp>

namespace ramen
{
parameterised_t::parameterised_t()
  : dont_persist_params_(false)
{
    params_.set_parent(this);
    parent_ = 0;
}

parameterised_t::parameterised_t(const parameterised_t& other)
  : params_(other.params_)
{
    name_ = other.name_;
    dont_persist_params_ = other.dont_persist_params_;
    params_.set_parent(this);
    parent_ = 0;
}

parameterised_t::~parameterised_t() { deleted(this); }

parameterised_t* parameterised_t::clone() const { return do_clone(); }

void parameterised_t::create_params()
{
    do_create_params();

    for (auto& p : param_set())
        p.init();
}

void parameterised_t::set_parent(parameterised_t* parent)
{
    parent_ = parent;
    do_set_parent(parent);
}

void parameterised_t::do_set_parent(parameterised_t* parent) {}

const node_t* parameterised_t::node() const
{
    const parameterised_t* p = this;

    while (p != 0)
    {
        if (const node_t* node = dynamic_cast<const node_t*>(p))
            return node;

        p = p->parent();
    }

    return 0;
}

node_t* parameterised_t::node()
{
    parameterised_t* p = this;

    while (p != 0)
    {
        if (node_t* node = dynamic_cast<node_t*>(p))
            return node;

        p = p->parent();
    }

    return 0;
}

// composition
const composition_t* parameterised_t::composition() const
{
    const node_t* n = node();

    if (n)
        return n->composition();

    return 0;
}

composition_t* parameterised_t::composition()
{
    node_t* n = node();

    if (n)
        return n->composition();

    return 0;
}

void parameterised_t::set_name(const std::string& n)
{
    assert(util::is_string_valid_identifier(n));

    if (!util::is_string_valid_identifier(n))
        throw std::runtime_error("Invalid name for parameterised_t");

    name_ = n;
}

bool parameterised_t::autokey() const
{
    if (const composition_t* c = composition())
        return c->autokey();

    return false;
}

bool parameterised_t::track_mouse() const { return true; }

const param_t& parameterised_t::param(const std::string& identifier) const
{
    return param_set().find(identifier);
}

param_t& parameterised_t::param(const std::string& identifier)
{
    return param_set().find(identifier);
}

void parameterised_t::for_each_param(const std::function<void(param_t*)>& f)
{
    param_set().for_each_param(f);
}

// anim & time
void parameterised_t::create_tracks(anim::track_t* root)
{
    std::auto_ptr<anim::track_t> top(new anim::track_t(name()));

    for (param_t& p : param_set())
    {
        p.create_tracks(top.get());
    }

    do_create_tracks(top.get());
    root->add_child(top);
}

void parameterised_t::set_frame(float f)
{
    for (auto& p : param_set())
        p.set_frame(f);

    do_set_frame(f);
}

void parameterised_t::evaluate_params(float frame)
{
    for (auto& p : param_set())
        p.evaluate(frame);
}

void parameterised_t::update_widgets()
{
    for (auto& p : param_set())
        p.update_widgets();

    do_update_widgets();
}

parameterised_t* new_clone(const parameterised_t& other)
{
    return other.clone();
}

}  // namespace ramen
