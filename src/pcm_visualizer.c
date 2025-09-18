#include "pcm_visualizer.h"

static const char *pcmvis_get_name(void *)
{
	return "PCM Visualizer (Skeleton)";
}

static void pcmvis_update(void *data, obs_data_t *settings)
{
	auto *s = static_cast<pcmvis *>(data);
	uint8_t r = (uint8_t)obs_data_get_int(settings, "r");
	uint8_t g = (uint8_t)obs_data_get_int(settings, "g");
	uint8_t b = (uint8_t)obs_data_get_int(settings, "b");
	s->color = (0xFFu << 24) | (r << 16) | (g << 8) | b;
}

static void *pcmvis_create(obs_data_t *settings, obs_source_t *src)
{
	auto *s = new pcmvis();
	s->source = src;
	s->effect = obs_get_base_effect(OBS_EFFECT_SOLID);
	s->ep_color = gs_effect_get_param_by_name(s->effect, "color");
	pcmvis_update(s, settings);
	return s;
}

static void pcmvis_destroy(void *data)
{
	auto *s = static_cast<pcmvis *>(data);
	if (s->vbuf)
		gs_vertexbuffer_destroy(s->vbuf);
	delete s;
}

static uint32_t pcmvis_get_width(void *)
{
	return 800;
}
static uint32_t pcmvis_get_height(void *)
{
	return 200;
}

static void ensure_vbuf(pcmvis *s, size_t n)
{
	if (s->vbuf && gs_vertexbuffer_get_num_vertices(s->vbuf) == n)
		return;
	if (s->vbuf)
		gs_vertexbuffer_destroy(s->vbuf);

	gs_vertex_t *verts = (gs_vertex_t *)bzalloc(sizeof(gs_vertex_t) * n);
	s->vbuf = gs_vertexbuffer_create(verts, n, GS_DYNAMIC);
	bfree(verts);
}

static void pcmvis_video_render(void *data, gs_effect_t *)
{
	auto *s = static_cast<pcmvis *>(data);
	const uint32_t W = pcmvis_get_width(s);
	const uint32_t H = pcmvis_get_height(s);

	const size_t N = 1024;
	ensure_vbuf(s, N);

	gs_vb_data *map = gs_vertexbuffer_map(s->vbuf);
	if (!map)
		return;

	const float mid = (float)H * 0.5f;
	const float amp = (float)H * 0.4f;
	s->t += 0.03f;

	vec4 col;
	vec4_from_rgba(&col, s->color);
	for (size_t i = 0; i < N; i++) {
		float x = (float)i / (float)(N - 1);
		float y = sinf((x + s->t) * 6.28318f * 2.0f);
		map->points[i].pos.x = x * (float)W;
		map->points[i].pos.y = mid + y * amp;
		map->points[i].pos.z = 0.0f;
		map->points[i].color = col;
	}
	gs_vertexbuffer_unmap(s->vbuf);

	gs_reset_blend_state();
	gs_matrix_push();
	gs_matrix_identity();

	gs_load_vertexbuffer(s->vbuf);
	gs_technique_t *tech = gs_effect_get_technique(s->effect, "Solid");
	gs_technique_begin(tech);
	gs_technique_begin_pass(tech, 0);
	gs_effect_set_color(s->ep_color, &col);
	gs_draw(GS_LINESTRIP, 0, N);
	gs_technique_end_pass(tech);
	gs_technique_end(tech);

	gs_matrix_pop();
}

static obs_properties_t *pcmvis_properties(void *)
{
	obs_properties_t *p = obs_properties_create();
	obs_properties_add_int(p, "r", "Red", 0, 255, 1);
	obs_properties_add_int(p, "g", "Green", 0, 255, 1);
	obs_properties_add_int(p, "b", "Blue", 0, 255, 1);
	return p;
}

struct obs_source_info pcmvis_info = {.id = "pcm_visualizer",
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
				      .icon_type = OBS_ICON_TYPE_AUDIOWAVEFORM};
