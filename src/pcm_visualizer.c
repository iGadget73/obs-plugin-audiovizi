#include "pcm_visualizer.h"

/* GS / GFX */
#include <graphics/graphics.h>
#include <graphics/vec3.h>
#include <graphics/vec4.h>
#include <math.h>

static const char *pcmvis_get_name(void *unused)
{
	UNUSED_PARAMETER(unused);
	return "AudioVizi (Skeleton)";
}

static void pcmvis_update(void *data, obs_data_t *settings)
{
	struct pcmvis *s = (struct pcmvis *)data;
	uint8_t r = (uint8_t)obs_data_get_int(settings, "r");
	uint8_t g = (uint8_t)obs_data_get_int(settings, "g");
	uint8_t b = (uint8_t)obs_data_get_int(settings, "b");
	s->color = (0xFFu << 24) | (r << 16) | (g << 8) | b;
}

static void *pcmvis_create(obs_data_t *settings, obs_source_t *source)
{
	struct pcmvis *s = bzalloc(sizeof(*s));
	s->source = source;
	s->t = 0.0f;

	/* Built-in solid effect */
	s->effect = obs_get_base_effect(OBS_EFFECT_SOLID);
	s->ep_color = gs_effect_get_param_by_name((gs_effect_t *)s->effect, "color");

	pcmvis_update(s, settings);
	return s;
}

static void pcmvis_destroy(void *data)
{
	struct pcmvis *s = (struct pcmvis *)data;
	if (s->vbuf)
		gs_vertexbuffer_destroy((gs_vertbuffer_t *)s->vbuf);
	bfree(s);
}

static uint32_t pcmvis_get_width(void *data)  { UNUSED_PARAMETER(data); return 800; }
static uint32_t pcmvis_get_height(void *data) { UNUSED_PARAMETER(data); return 200; }

/* Hilfsfunktion: neuen VB aus Rohdaten bauen (API-konform) */
static gs_vertbuffer_t *build_vb_from_arrays(struct vec3 *pts, uint32_t *cols, size_t n)
{
	struct gs_vb_data *vb = gs_vbdata_create();
	if (!vb)
		return NULL;

	vb->num = (uint32_t)n;
	vb->points = bmemdup(pts, sizeof(struct vec3) * n);
	vb->colors = bmemdup(cols, sizeof(uint32_t) * n);

	gs_vertbuffer_t *vbuf = gs_vertexbuffer_create(vb, GS_DYNAMIC);
	gs_vbdata_destroy(vb);
	return vbuf;
}

static void pcmvis_video_render(void *data, gs_effect_t *effect)
{
	UNUSED_PARAMETER(effect);

	struct pcmvis *s = (struct pcmvis *)data;
	const uint32_t W = pcmvis_get_width(s);
	const uint32_t H = pcmvis_get_height(s);

	const size_t N = 1024;

	/* Punkte + Farben vorbereiten (CPU) */
	struct vec3 *pts = (struct vec3 *)bzalloc(sizeof(struct vec3) * N);
	uint32_t *cols = (uint32_t *)bzalloc(sizeof(uint32_t) * N);

	const float mid = (float)H * 0.5f;
	const float amp = (float)H * 0.4f;
	s->t += 0.03f;

	for (size_t i = 0; i < N; i++) {
		float x = (float)i / (float)(N - 1);
		float y = sinf((x + s->t) * 6.28318f * 2.0f); /* 2 cycles */
		pts[i].x = x * (float)W;
		pts[i].y = mid + y * amp;
		pts[i].z = 0.0f;
		cols[i] = s->color; /* uint32_t erwartet */
	}

	/* alten VB weg, neuen aus Arrays bauen (einfach & CI-sicher) */
	if (s->vbuf)
		gs_vertexbuffer_destroy((gs_vertbuffer_t *)s->vbuf);
	s->vbuf = build_vb_from_arrays(pts, cols, N);

	bfree(pts);
	bfree(cols);

	if (!s->vbuf)
		return;

	gs_reset_blend_state();
	gs_matrix_push();
	gs_matrix_identity();

	gs_load_vertexbuffer((gs_vertbuffer_t *)s->vbuf);

	gs_technique_t *tech = gs_effect_get_technique((gs_effect_t *)s->effect, "Solid");
	gs_technique_begin(tech);
	gs_technique_begin_pass(tech, 0);

	/* erwartet uint32_t */
	gs_effect_set_color((gs_eparam_t *)s->ep_color, s->color);

	gs_draw(GS_LINESTRIP, 0, (uint32_t)N);

	gs_technique_end_pass(tech);
	gs_technique_end(tech);

	gs_matrix_pop();
}

static obs_properties_t *pcmvis_properties(void *data)
{
	UNUSED_PARAMETER(data);
	obs_properties_t *p = obs_properties_create();
	obs_properties_add_int(p, "r", "Red", 0, 255, 1);
	obs_properties_add_int(p, "g", "Green", 0, 255, 1);
	obs_properties_add_int(p, "b", "Blue", 0, 255, 1);
	return p;
}

static void pcmvis_defaults(obs_data_t *s)
{
	obs_data_set_default_int(s, "r", 255);
	obs_data_set_default_int(s, "g", 255);
	obs_data_set_default_int(s, "b", 255);
}

struct obs_source_info pcmvis_info = {
	.id = "audiovizi",
	.type = OBS_SOURCE_TYPE_INPUT,
	.output_flags = OBS_SOURCE_VIDEO,
	.get_name = pcmvis_get_name,
	.create = pcmvis_create,
	.destroy = pcmvis_destroy,
	.update = pcmvis_update,
	.get_width = pcmvis_get_width,
	.get_height = pcmvis_get_height,
	.video_render = pcmvis_video_render,
	.get_properties = pcmvis_properties,
	.get_defaults = pcmvis_defaults,
	/* .icon_type weggelassen – nicht überall vorhanden */
};
