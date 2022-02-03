#include "abstract_slider.h"

namespace DGui
{
    AbstractSlider::AbstractSlider(WidgetManager *manager, int w, int h)
        : Widget(manager, w, h),
          thumb(manager, this)
    {
        addWidget(&thumb);
    }
};