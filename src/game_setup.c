/*
 * Copyright (C) 2017 Manh Tran
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 */
#include <macao/game.h>
#include <cherry/memory.h>
#include <cherry/math/math.h>
#include <cherry/list.h>
#include <cherry/array.h>
#include <cherry/string.h>
#include <cherry/map.h>
#include <cherry/graphic/camera.h>
#include <cherry/graphic/uniform.h>
#include <cherry/graphic/node_manager.h>
#include <cherry/graphic/shader/shader_2d_texture_color.h>
#include <cherry/graphic/shader/shader_3d_texture_color.h>
#include <cherry/graphic/render_pass.h>
#include <cherry/graphic/light.h>
#include <cherry/stdio.h>
#include <smartfox/smartfox.h>
#include <smartfox/data.h>
#include <smartfox/network_data.h>

static void __setup_game_camera(struct game *p)
{
        union mat4 project      = mat4_new_perspective(
                DEG_TO_RAD(45),
                (float)video_width / (float)video_height,
                100,
                100000
        );

        union mat4 lookat       = mat4_new_look_at(
                video_width/2, video_height/2, video_height/2 / tanf(DEG_TO_RAD(45.0/2)),
                video_width/2, video_height/2, 0,
                0, 1, 0
        );

        p->game_camera          = camera_alloc(project, lookat);
}

static void __setup_hud_camera(struct game *p)
{
        union mat4 project      = mat4_new_ortho(
                0, video_width,
                0, video_height,
                0, 2
        );

        union mat4 lookat       = mat4_new_look_at(
                0, 0, 1,
                0, 0, 0,
                0, 1, 0
        );

        p->hud_camera          = camera_alloc(project, lookat);
}

static void __setup_game_light(struct game *p)
{
        p->game_light           = light_alloc(3, 0, 0);
        light_set_direction_light_direction(p->game_light, 0, (union vec3){0, 0, -1});
        light_set_direction_light_ambient(p->game_light, 0, (union vec3){0, 0, 0});
        light_set_direction_light_diffuse(p->game_light, 0, (union vec3){1, 1, 1});
        light_set_direction_light_specular(p->game_light, 0, (union vec3){0, 0, 0});

        light_set_direction_light_direction(p->game_light, 1, (union vec3){1, 0, 0});
        light_set_direction_light_ambient(p->game_light, 1, (union vec3){0, 0, 0});
        light_set_direction_light_diffuse(p->game_light, 1, (union vec3){0.5, 0.5, 0.5});
        light_set_direction_light_specular(p->game_light, 1, (union vec3){0, 0, 0});

        light_set_direction_light_direction(p->game_light, 2, (union vec3){-1, 0, 0});
        light_set_direction_light_ambient(p->game_light, 2, (union vec3){0, 0, 0});
        light_set_direction_light_diffuse(p->game_light, 2, (union vec3){0.5, 0.5, 0.5});
        light_set_direction_light_specular(p->game_light, 2, (union vec3){0, 0, 0});
}

static void __setup_game_manager(struct game *p)
{
        struct node_manager *m  = node_manager_alloc();
        p->manager_game         = m;

        /*
         * setup shader for 2d
         */
        {
                struct shader *s        = shader_2d_texture_color_get();
                u32 key                 = SHADER_2D_TEXTURE_COLOR;
                map_set(m->shaders, &key, sizeof(key), &s);
        }
        /*
         * setup shader for 3d
         */
        {
                struct shader *s        = shader_3d_texture_color_get();
                u32 key                 = SHADER_3D_TEXTURE_COLOR;
                map_set(m->shaders, &key, sizeof(key), &s);
        }

        u32 key                     = UNIFORM_CAMERA;
        map_set(m->common_uniform_buffers, &key, sizeof(key), &p->game_camera->buffer);
        p->game_camera->buffer->ref++;

        key                     = UNIFORM_LIGHT;
        map_set(m->common_uniform_buffers, &key, sizeof(key), &p->game_light->buffer);
        p->game_light->buffer->ref++;
}

static void __setup_hud_manager(struct game *p)
{
        struct node_manager *m  = node_manager_alloc();

        struct shader *s        = shader_2d_texture_color_get();
        u32 key                 = SHADER_2D_TEXTURE_COLOR;
        map_set(m->shaders, &key, sizeof(key), &s);

        key                     = UNIFORM_CAMERA;
        map_set(m->common_uniform_buffers, &key, sizeof(key), &p->hud_camera->buffer);
        p->hud_camera->buffer->ref++;

        p->manager_hud          = m;
}

static void __setup_main_pass(struct game *p)
{
        p->main_pass            = render_pass_main_alloc();
}

static void __setup_game_1_pass(struct game *p)
{
        p->game_1_pass          = render_pass_texture_alloc(video_width, video_height);
}

static void __setup_game_2_pass(struct game *p)
{
        p->game_2_pass          = render_pass_texture_alloc(video_width, video_height);
}

void game_setup(struct game *p)
{
        __setup_main_pass(p);
        __setup_game_1_pass(p);
        __setup_game_2_pass(p);

        __setup_game_light(p);

        __setup_game_camera(p);
        __setup_hud_camera(p);

        __setup_game_manager(p);
        __setup_hud_manager(p);
}
