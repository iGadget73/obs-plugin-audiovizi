/*
PCM Visualizer Plugin for OBS Studio
------------------------------------
Author: Günther Gadget (igadget.de)
Email: igadget73@gmail.com
Year: 2025

This plugin adds a simple PCM Audio Visualizer source to OBS Studio.
It is distributed under the terms of the GNU General Public License
as published by the Free Software Foundation, either version 2
of the License, or (at your option) any later version.
See <https://www.gnu.org/licenses/> for more details.
*/

#include <obs-module.h>
#include <plugin-support.h>
#include "pcm_visualizer.h"   /* unser Header */

OBS_DECLARE_MODULE();
OBS_MODULE_USE_DEFAULT_LOCALE(PLUGIN_NAME, "en-US");

/* Plugin-Beschreibung für OBS */
MODULE_EXPORT const char *obs_module_description(void)
{
	return "PCM Visualizer (Skeleton) – scrollende Waveform für OBS Studio";
}

bool obs_module_load(void)
{
	extern struct obs_source_info pcmvis_info;
	obs_register_source(&pcmvis_info);

	blog(LOG_INFO, "[pcmvis] loaded");
	return true;
}

void obs_module_unload(void)
{
	blog(LOG_INFO, "[pcmvis] plugin unloaded");
}
