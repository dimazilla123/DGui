#ifndef DGUI_EVENT_H
#define DGUI_EVENT_H

namespace DGui
{
    struct Event
    {
        enum EventType
        {
            MouseButtonEvent,
            MouseMoveEvent,

            FocusGainEvent,
            FocusLoseEvent,

            KeyboardButtonEvent,

            CloseEvent,
            //Count
        } type;

        enum EventProcStatus
        {
            Unprocessed,
            Processed,
            Passed,
        };

        struct MouseMoveStatus
        {
            int x;
            int y;
        };

        struct MouseButtonStatus
        {
            enum MouseButtonState
            {
                Clicked,
                Pressed,
                Released,
            } state;
            int x;
            int y;
        };

        struct KeyboardButtonStatus
        {

        };

        union 
        {
            MouseButtonStatus mouse_button_event;
            MouseMoveStatus   mouse_move_event;
        };
    };
};

#endif