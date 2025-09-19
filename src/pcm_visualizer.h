#pragma once
#include <stdint.h>
#include <obs-module.h>

struct pcmvis {
	obs_source_t *source;
	float t;
	void *vbuf;
	void *effect;
	void *ep_color;
	uint32_t color;
};

extern struct obs_source_info pcmvis_info;
