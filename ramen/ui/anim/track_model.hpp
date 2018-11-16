// Copyright (c) 2010 Esteban Tovagliari
// Licensed under the terms of the CDDL License.
// See CDDL_LICENSE.txt for a copy of the license.

#pragma once

#include <QAbstractItemModel>

#include <ramen/anim/track_fwd.hpp>

namespace ramen
{
namespace ui
{
class track_model_t : public QAbstractItemModel
{
    Q_OBJECT

public:
    track_model_t();
    ~track_model_t() override;

    anim::track_t* root_track();
    anim::track_t* node_track();

    QModelIndex index(int row, int column, const QModelIndex& parent) const override;
    QModelIndex parent(const QModelIndex& index) const override;

    int rowCount(const QModelIndex& parent) const override;
    int columnCount(const QModelIndex& parent) const override;

    QVariant      data(const QModelIndex& index, int role) const override;
    Qt::ItemFlags flags(const QModelIndex& index) const override;
    QVariant      headerData(int section, Qt::Orientation orientation, int role) const override;

private:
    anim::track_t* tracks_;
};

}  // namespace
}  // namespace

