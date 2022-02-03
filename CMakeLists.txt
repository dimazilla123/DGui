find_package(SFML 2.5 COMPONENTS graphics audio network window system REQUIRED)

include_directories(.)
add_subdirectory(json)

add_library(dgui STATIC
                        widget.cpp
                        widget_manager.cpp
                        screen_widget.cpp
                        button.cpp
                        boundered_rect.cpp
                        window/window.cpp
                        window/title_bar.cpp
                        directional_layout.cpp
                        style.cpp
                        abstract_slider/thumb.cpp
                        slider.cpp
                        curve/curve.cpp
                        curve/vector2d.cpp
                        )
target_link_libraries(dgui json sfml-system sfml-window sfml-graphics)
