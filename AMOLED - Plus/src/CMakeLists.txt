SET(SOURCES screens/ui_Main.c
    screens/ui_PPM.c
    ui.c
    components/ui_comp_hook.c
    ui_helpers.c
    ui_events.cpp)

add_library(ui ${SOURCES})
