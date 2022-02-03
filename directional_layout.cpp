#include "directional_layout.h"

namespace DGui
{
    void HorizontalLayout::refit()
    {
        int child_cnt = 0;
        int total_width = 0;
        int max_height = 0;

        for (Widget::Iterator it = begin(); it != end(); ++it)
        {
            ++child_cnt;
            Size sz = (*it)->getMinimumSize();
            total_width += sz.w;
            if (max_height < sz.h)
                max_height = sz.h;
        }

        int border = 10;
        position.y_length = max_height;

        //fprintf(stderr, "border = %d, hborder = %d\n", border, hborder);
        if (border < 0) border = 0;

        int width_passed = border;
        for (Widget::Iterator it = begin(); it != end(); ++it)
        {
            (*it)->setMinimumSize();
            (*it)->setPosition(position.x0 + width_passed, position.y0);
            width_passed += border + (*it)->getMinimumSize().w;
        }
        position.x_length = width_passed;
    }

    void VerticalLayout::refit()
    {
        int child_cnt = 0;
        int total_height = 0;
        int max_width = 0;

        for (Widget::Iterator it = begin(); it != end(); ++it)
        {
            ++child_cnt;
            Size sz = (*it)->getMinimumSize();
            total_height += sz.h;
            if (max_width < sz.w)
                max_width = sz.w;
        }

        int border = 10;
        position.x_length = max_width;

        //fprintf(stderr, "border = %d, hborder = %d\n", border, hborder);
        if (border < 0) border = 0;

        int height_passed = border;
        for (Widget::Iterator it = begin(); it != end(); ++it)
        {
            (*it)->setMinimumSize();
            (*it)->setPosition(position.x0, position.y0 + height_passed);
            height_passed += border + (*it)->getMinimumSize().h;
        }
        position.y_length = height_passed;
    }
};