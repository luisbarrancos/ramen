// Copyright (c) 2010 Esteban Tovagliari
// Licensed under the terms of the CDDL License.
// See CDDL_LICENSE.txt for a copy of the license.

#include<ramen/ui/viewer.hpp>

#include<QHBoxLayout>
#include<QVBoxLayout>
#include<QLabel>
#include<QComboBox>
#include<QStackedWidget>
#include<QToolButton>
#include<QDoubleSpinBox>

#include<ramen/nodes/composition_node.hpp>

#include<ramen/qwidgets/ocio_display_combo.hpp>
#include<ramen/qwidgets/ocio_view_combo.hpp>

#include<ramen/ui/image_view.hpp>

namespace ramen
{
namespace ui
{

viewer_t::viewer_t() : QWidget()
{
    setWindowTitle( "Viewer");

    QVBoxLayout *layout = new QVBoxLayout();
    layout->setContentsMargins( 5, 5, 5, 5);

    QWidget *status_bar = new QWidget();
    QHBoxLayout *status_layout = new QHBoxLayout();
    status_layout->setContentsMargins( 0, 0, 0, 0);
    QLabel *status_ = new QLabel();
    status_->setText( "Status");
    status_layout->addWidget( status_);
    QSize label_size = status_->sizeHint();
    status_bar->setMinimumSize( 0, label_size.height());
    status_bar->setMaximumSize( QWIDGETSIZE_MAX, label_size.height());
    status_bar->setLayout( status_layout);
    layout->addWidget( status_bar);

    img_view_ = new image_view_t();
    layout->addWidget( img_view_);

    // TODO: don't harcode this. Get it from some widgets
    const int toolbar_height = 30;

    QStackedWidget *active_toolbar_ = new QStackedWidget();
    active_toolbar_->setMinimumSize( 0, toolbar_height);
    active_toolbar_->setMaximumSize( QWIDGETSIZE_MAX, toolbar_height);
    active_toolbar_->setSizePolicy( QSizePolicy::Preferred, QSizePolicy::Fixed);
    layout->addWidget( active_toolbar_);

    QFrame *separator = new QFrame();
    separator->setFrameStyle( QFrame::HLine | QFrame::Raised);
    separator->setLineWidth( 1);
    layout->addWidget( separator);

    QWidget *viewer_controls = new QWidget();
    viewer_controls->setMinimumSize( 0, toolbar_height);
    viewer_controls->setMaximumSize( QWIDGETSIZE_MAX, toolbar_height);
    viewer_controls->setSizePolicy( QSizePolicy::Preferred, QSizePolicy::Fixed);
    QHBoxLayout *horizontalLayout = new QHBoxLayout( viewer_controls);
    horizontalLayout->setContentsMargins( 0, 0, 0, 0);

    QComboBox *result_combo_ = new QComboBox();
    result_combo_->insertItems( 0, QStringList() << "Active" << "Context");
    QSize s = result_combo_->sizeHint();

    QToolButton *update_btn_ = new QToolButton();
    update_btn_->setFocusPolicy( Qt::NoFocus);
    update_btn_->setCheckable( true);
    update_btn_->setChecked( true);
    update_btn_->setText( "U");
    update_btn_->setToolTip( "Auto-update");
    update_btn_->setMaximumWidth( s.height());
    update_btn_->setMaximumHeight( s.height());
    //connect( update_btn_, SIGNAL( toggled( bool)), this, SLOT( autoupdate_toggle( bool)));
    horizontalLayout->addWidget( update_btn_);

    result_combo_->setFocusPolicy( Qt::NoFocus);
    result_combo_->setToolTip( "Show result / context node");
    //connect( result_combo_, SIGNAL( activated( int)), this, SLOT( change_active_context_view( int)));
    horizontalLayout->addWidget(result_combo_);

    separator = new QFrame();
    separator->setFrameStyle( QFrame::VLine | QFrame::Raised);
    separator->setLineWidth( 1);
    horizontalLayout->addWidget( separator);

    qwidgets::ocio_display_combo_t *ocio_display_combo_ = new qwidgets::ocio_display_combo_t();
    ocio_display_combo_->setFocusPolicy( Qt::NoFocus);
    ocio_display_combo_->setToolTip( "OpenColorIO Display");
    //connect( ocio_device_combo_, SIGNAL( activated( int)), this, SLOT( change_display_device( int)));
    horizontalLayout->addWidget( ocio_display_combo_);

    QComboBox *ocio_view_combo_ = new qwidgets::ocio_view_combo_t();
    ocio_view_combo_->setFocusPolicy( Qt::NoFocus);
    ocio_view_combo_->setToolTip( "OpenColorIO View");
    //connect( ocio_transform_combo_, SIGNAL( activated( int)), this, SLOT( change_display_transform( int)));
    horizontalLayout->addWidget( ocio_view_combo_);

    connect( ocio_display_combo_, SIGNAL( display_changed( QString)),
             ocio_view_combo_, SLOT( update_views( QString)));

    QDoubleSpinBox *exposure_input_ = new QDoubleSpinBox();
    exposure_input_->setMinimum(-30.0);
    exposure_input_->setMaximum( 30.0);
    exposure_input_->setSingleStep( 0.1);
    exposure_input_->setDecimals( 3);
    exposure_input_->setToolTip( "Viewer Exposure");
    //connect( exposure_input_, SIGNAL( valueChanged( double)), this, SLOT( change_exposure( double)));
    //connect( exposure_input_, SIGNAL( spinBoxDragged( double)), this, SLOT( change_exposure( double)));
    horizontalLayout->addWidget( exposure_input_);

    QDoubleSpinBox *gamma_input_ = new QDoubleSpinBox();
    gamma_input_->setMinimum( 0);
    gamma_input_->setMaximum( 4);
    gamma_input_->setValue( 1);
    gamma_input_->setSingleStep( 0.1);
    gamma_input_->setDecimals( 3);
    gamma_input_->setToolTip( "Viewer Gamma");
    //connect( gamma_input_, SIGNAL( valueChanged( double)), this, SLOT( change_gamma( double)));
    //connect( gamma_input_, SIGNAL( spinBoxDragged( double)), this, SLOT( change_gamma( double)));
    horizontalLayout->addWidget( gamma_input_);

    separator = new QFrame();
    separator->setFrameStyle( QFrame::VLine | QFrame::Raised);
    separator->setLineWidth( 1);
    horizontalLayout->addWidget( separator);

    QStackedWidget *viewer_toolbar_ = new QStackedWidget();
    viewer_toolbar_->setMinimumSize( 0, toolbar_height);
    viewer_toolbar_->setMaximumSize( QWIDGETSIZE_MAX, toolbar_height);
    viewer_toolbar_->setSizePolicy( QSizePolicy::Preferred, QSizePolicy::Fixed);
    horizontalLayout->addWidget( viewer_toolbar_);

    viewer_controls->setLayout( horizontalLayout);
    layout->addWidget( viewer_controls);
    setLayout( layout);

    composition_node_t::node_deleted.connect( boost::bind( &viewer_t::node_released, this, _1));
}

viewer_t::~viewer_t() {}

void viewer_t::set_active_node( node_t *n) {}
void viewer_t::set_context_node( node_t *n) {}

void viewer_t::node_released( node_t *n) {}

void viewer_t::frame_changed() {}

void viewer_t::begin_interaction() {}
void viewer_t::end_interaction() {}

} // ui
} // ramen
