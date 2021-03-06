// Copyright (c) 2011 Esteban Tovagliari

#pragma once

#include <string>
#include <vector>

#include <boost/scoped_array.hpp>

#include <cassert>

namespace ramen
{
namespace util
{
struct edit_distance_t
{
  public:
    edit_distance_t();

    int operator()(const std::string& src, const std::string& dst);

  private:
    void realloc_matrix(int rows, int cols);

    int matrix(int i, int j) const
    {
        assert(i >= 0 && i < rows_);
        assert(j >= 0 && j < cols_);
        return matrix_[i * cols_ + j];
    }

    int& matrix(int i, int j)
    {
        assert(i >= 0 && i < rows_);
        assert(j >= 0 && j < cols_);
        return matrix_[i * cols_ + j];
    }

    boost::scoped_array<int> matrix_;
    int                      rows_, cols_;
};

}  // namespace util
}  // namespace ramen
