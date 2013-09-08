// Copyright (c) 2010 Esteban Tovagliari

#ifndef RAMEN_STATIC_PARAM_HPP
#define	RAMEN_STATIC_PARAM_HPP

#include<ramen/params/param.hpp>

namespace ramen
{

class RAMEN_API static_param_t : public param_t
{
    Q_OBJECT

public:

    static_param_t();

protected:

    static_param_t( const static_param_t& other);
    void operator=( const static_param_t& other);

    virtual core::auto_ptr_t<undo::command_t> do_create_command();
};

} // namespace

#endif
