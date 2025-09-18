#pragma once
#include <stdint.h>
#include <obs-module.h>

struct pcmvis {
	obs_source_t *source;
	float t;
	void *vbuf;       /* gs_vertbuffer_t* */
	void *effect;     /* gs_effect_t* */
	void *ep_color;   /* gs_eparam_t* */
	uint32_t color;   /* RGBA */
};

extern struct obs_source_info pcmvis_info;
