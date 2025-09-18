#pragma once
#include <obs-module.h>
struct pcmvis {
    obs_source_t* source = nullptr;
    float t = 0.0f;
    gs_vertbuffer_t* vbuf = nullptr;
    gs_effect_t* effect = nullptr;
    gs_eparam_t* ep_color = nullptr;
    uint32_t color = 0xFFFFFFFF; // RGBA
};
extern struct obs_source_info pcmvis_info;
