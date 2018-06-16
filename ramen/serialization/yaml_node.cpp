// Copyright (c) 2010 Esteban Tovagliari
// Licensed under the terms of the CDDL License.
// See CDDL_LICENSE.txt for a copy of the license.

#include <ramen/serialization/yaml_node.hpp>
#include <ramen/serialization/yaml_iarchive.hpp>

namespace ramen
{
namespace serialization
{
yaml_node_t::yaml_node_t(yaml_iarchive_t* parent, const YAML::Node* node, int version)
{
    assert(parent);
    assert(node);
    assert(version >= 0);

    parent_  = parent;
    node_    = node;
    version_ = version;
}

yaml_node_t::yaml_node_t(const yaml_node_t& parent, const YAML::Node* node)
{
    assert(node);
    parent_  = parent.parent_;
    node_    = node;
    version_ = parent.version_;
}

void yaml_node_t::set_version(int v) const
{
    assert(v >= 0);
    version_ = v;
}

yaml_node_t yaml_node_t::get_node(const std::string& key) const
{
    return yaml_node_t(parent_, &(*node_)[key], version());
}

boost::optional<yaml_node_t> yaml_node_t::get_optional_node(const std::string& key) const
{
    if (const YAML::Node* n = node_->FindValue(key))
        return yaml_node_t(parent_, n, version());

    return boost::optional<yaml_node_t>();
}

std::stringstream& yaml_node_t::error_stream() const
{
    assert(parent_);
    return parent_->error_stream();
}

}  // namespace
}  // namespace
