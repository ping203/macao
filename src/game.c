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
#include <cherry/list.h>
#include <cherry/memory.h>
#include <cherry/string.h>
#include <cherry/stdio.h>
#include <cherry/map.h>
#include <cherry/math/math.h>
#include <cherry/graphic/node_manager.h>
#include <cherry/graphic/camera.h>
#include <cherry/graphic/node.h>
#include <cherry/graphic/texture.h>
#include <cherry/graphic/render_pass.h>
#include <cherry/graphic/texture_atlas.h>
#include <cherry/graphic/font_frame.h>
#include <cherry/graphic/texture_frame.h>
#include <cherry/graphic/light.h>
#include <cherry/xml/xml.h>
#include <cherry/graphic/action.h>
#include <cherry/graphic/dae.h>
#include <smartfox/smartfox.h>
#include <cherry/bytes.h>
#include <cherry/array.h>
#include <smartfox/data.h>
#include <smartfox/network_data.h>
#include <smartfox/smartfox.h>

struct node *test_node;
struct node *game_1;
struct node *game_2;

static void create_game_1(struct game *p)
{
        // struct dae_context *dae = dae_context_alloc();
        // dae_context_parse(dae, "res/images/cube_1.dae", sizeof("res/images/cube_1.dae")-1);
        // dae_context_free(dae);

        int i;
        game_1 = node_alloc(p->manager_game);
        node_set_size(game_1, (union vec3){video_width, video_height, 1});
        node_set_origin(game_1, (union vec3){0.5, 0.5, 0.5});
        node_set_position(game_1, (union vec3){video_width/2, video_height/2, 0});
        node_set_rotation(game_1, quat_angle_axis(DEG_TO_RAD(-30), (float[3]){1, 0, 0}));

        {
                struct node *child = node_alloc(p->manager_game);
                struct texture *tex = texture_alloc_file("res/images/macao/Floor-pattern2.png", FILE_INNER);
                texture_set_type(tex, TEXTURE_REPEAT);
                struct texture_frame *frame = texture_frame_alloc(tex);
                node_show_sprite(child, SHADER_3D_TEXTURE_COLOR, frame, 8, GFX_SPRITE_NORMAL);
                node_set_size(child, (union vec3){video_width*1.3, child->original_size.y / child->original_size.x * video_width * 1.3, 1});
                node_set_position(child, (union vec3){video_width/2, video_height/2, 0});
                node_set_origin(child, (union vec3){0.5, 0.5, 0.5});
                node_add_child(game_1, child);
                node_set_bright(child, 0.7);
                texture_frame_free(frame);
        }
        {
                struct node *child = node_alloc(p->manager_game);
                struct texture_frame *frame = texture_frame_alloc(texture_alloc_file("res/images/macao/table-poker.png", FILE_INNER));
                node_show_sprite(child, SHADER_3D_TEXTURE_COLOR, frame, 1, GFX_SPRITE_NORMAL);
                float width = video_width * 4/6;
                node_set_size(child, (union vec3){width, child->original_size.y / child->original_size.x * width, 1});
                node_set_position(child, (union vec3){video_width/2, video_height/2-15, 100});
                node_set_origin(child, (union vec3){0.5, 0.5, 0.5});
                node_add_child(game_1, child);
                node_set_bright(child, 1.25);
                texture_frame_free(frame);
        }

        char *cards[] = {
                "card_4.png","card_3.png","card_2.png","card_1.png","card_0.png"
        };
        for_i(i, 5) {
                struct node *child = node_alloc(p->manager_game);
                struct texture_frame *frame = texture_atlas_get_texture_frame("res/images/card.xml", FILE_INNER, sizeof("res/images/card.xml")-1, cards[i], strlen(cards[i]));
                node_show_sprite(child, SHADER_3D_TEXTURE_COLOR, frame, 1, GFX_SPRITE_NORMAL);
                float width =  video_width * 3/5 / 9;
                node_set_size(child, (union vec3){width, child->original_size.y / child->original_size.x * width, 1});
                node_set_position(child, (union vec3){video_width/2 + (child->size.x-5) * (i - 2), video_height/2, 101});
                node_set_origin(child, (union vec3){0, 0.5, 0});
                node_add_child(game_1, child);
                node_set_bright(child, 1.25);

                node_run_action(child, action_repeat(-1,
                        action_parallel(
                                node_delay(child, 3.5),
                                action_sequence(
                                        node_delay(child, (4 - i) * 0.09),
                                        node_rotate_by(child, DEG_TO_RAD(-180), (union vec3){0, 1, 0}, 0.75, EASE_QUADRATIC_OUT, 0),
                                        node_delay(child, i * 0.09),

                                        node_delay(child, 0.45),

                                        node_delay(child, i * 0.09),
                                        node_rotate_by(child, DEG_TO_RAD(180), (union vec3){0, 1, 0}, 0.75, EASE_QUADRATIC_OUT, 0),
                                        node_delay(child, (4 - i) * 0.09),
                                        node_delay(child, 0.45),
                                        NULL
                                ),
                                NULL
                        ),
                        NULL
               ), NULL);
                {
                        struct node *back = node_alloc(p->manager_game);
                        struct texture_frame *frame = texture_atlas_get_texture_frame("res/images/card.xml", FILE_INNER, sizeof("res/images/card.xml")-1, "card_52.png", sizeof("card_52.png")-1);
                        node_show_sprite(back, SHADER_3D_TEXTURE_COLOR, frame, 1, GFX_SPRITE_NORMAL);
                        float width =  video_width * 3/5 / 9;
                        node_set_size(back, (union vec3){width, back->original_size.y / back->original_size.x * width, 1});
                        node_set_position(back, (union vec3){back->size.x/2, back->size.y/2, -0.1});
                        node_set_origin(back, (union vec3){0.5, 0.5, 0});
                        node_set_rotation(back, quat_angle_axis(DEG_TO_RAD(180), (float[3]){0, 1, 0}));
                        node_add_child(child, back);
                        node_set_bright(back, 1.25);
                }
        }
}

static void create_game_2(struct game *p)
{
        int i;
        game_2 = node_alloc(p->manager_game);
        node_set_size(game_2, (union vec3){video_width, video_height, 1});
        node_set_origin(game_2, (union vec3){0.5, 0.5, 0.5});
        node_set_position(game_2, (union vec3){video_width/2, video_height/2, 0});
        node_set_rotation(game_2, quat_angle_axis(DEG_TO_RAD(-30), (float[3]){1, 0, 0}));

        {
                struct node *child = node_alloc(p->manager_game);
                struct texture *tex = texture_alloc_file("res/images/macao/Floor-pattern.png", FILE_INNER);
                texture_set_type(tex, TEXTURE_REPEAT);
                struct texture_frame *frame = texture_frame_alloc(tex);
                node_show_sprite(child, SHADER_3D_TEXTURE_COLOR, frame, 8, GFX_SPRITE_NORMAL);
                node_set_size(child, (union vec3){video_width*1.3, child->original_size.y / child->original_size.x * video_width * 1.3, 1});
                node_set_position(child, (union vec3){video_width/2, video_height/2, 0});
                node_set_origin(child, (union vec3){0.5, 0.5, 0.5});
                node_add_child(game_2, child);
                node_set_bright(child, 0.7);
                texture_frame_free(frame);
        }
        {
                struct node *child = node_alloc(p->manager_game);
                struct texture_frame *frame = texture_frame_alloc(texture_alloc_file("res/images/macao/table-tai-xiu.png", FILE_INNER));
                node_show_sprite(child, SHADER_3D_TEXTURE_COLOR, frame, 1, GFX_SPRITE_NORMAL);
                float width = video_width * 4/6;
                node_set_size(child, (union vec3){width, child->original_size.y / child->original_size.x * width, 1});
                node_set_position(child, (union vec3){video_width/2, video_height/2-15, 100});
                node_set_origin(child, (union vec3){0.5, 0.5, 0.5});
                node_add_child(game_2, child);
                node_set_bright(child, 1.25);
                texture_frame_free(frame);
        }

        char *cards[] = {
                "card_4.png","card_3.png","card_2.png","card_1.png","card_0.png"
        };
        for_i(i, 5) {
                struct node *child = node_alloc(p->manager_game);
                struct texture_frame *frame = texture_atlas_get_texture_frame("res/images/card.xml", FILE_INNER, sizeof("res/images/card.xml")-1, cards[i], strlen(cards[i]));
                node_show_sprite(child, SHADER_3D_TEXTURE_COLOR, frame, 1, GFX_SPRITE_NORMAL);
                float width =  video_width * 3/5 / 9;
                node_set_size(child, (union vec3){width, child->original_size.y / child->original_size.x * width, 1});
                node_set_position(child, (union vec3){video_width/2 + (child->size.x-5) * (i - 2), video_height/2, 101});
                node_set_origin(child, (union vec3){0.5, 0.5, 0});
                node_add_child(game_2, child);
                node_set_bright(child, 1.25);
                node_set_rotation(child, quat_angle_axis(DEG_TO_RAD(-180), (float[3]){0, 1, 0}));
                node_run_action(child, action_repeat(-1,
                        action_parallel(
                                node_delay(child, 3.5),
                                action_sequence(
                                        node_delay(child, (4 - i) * 0.09),
                                        action_parallel(
                                                action_sequence(
                                                        node_move_by(child, (union vec3){0, 0, 60}, 0.375, EASE_QUADRATIC_OUT, 0),
                                                        node_move_by(child, (union vec3){0, 0, -60}, 0.375, EASE_QUADRATIC_OUT, 0),
                                                        NULL
                                                ),
                                                node_rotate_by(child, DEG_TO_RAD(180), (union vec3){0, 1, 0}, 0.75, EASE_QUADRATIC_OUT, 0),
                                                NULL
                                        ),
                                        node_delay(child, i * 0.09),

                                        node_delay(child, 0.45),

                                        node_delay(child, i * 0.09),
                                        action_parallel(
                                                action_sequence(
                                                        node_move_by(child, (union vec3){0, 0, 60}, 0.375, EASE_QUADRATIC_OUT, 0),
                                                        node_move_by(child, (union vec3){0, 0, -60}, 0.375, EASE_QUADRATIC_OUT, 0),
                                                        NULL
                                                ),
                                                node_rotate_by(child, DEG_TO_RAD(-180), (union vec3){0, 1, 0}, 0.75, EASE_QUADRATIC_OUT, 0),
                                                NULL
                                        ),

                                        node_delay(child, (4 - i) * 0.09),
                                        node_delay(child, 0.45),
                                        NULL
                                ),
                                NULL
                        ),
                        NULL
               ), NULL);
                {
                        struct node *back = node_alloc(p->manager_game);
                        struct texture_frame *frame = texture_atlas_get_texture_frame("res/images/card.xml", FILE_INNER, sizeof("res/images/card.xml")-1, "card_52.png", sizeof("card_52.png")-1);
                        node_show_sprite(back, SHADER_3D_TEXTURE_COLOR, frame, 1, GFX_SPRITE_NORMAL);
                        float width =  video_width * 3/5 / 9;
                        node_set_size(back, (union vec3){width, back->original_size.y / back->original_size.x * width, 1});
                        node_set_position(back, (union vec3){back->size.x/2, back->size.y/2, -0.1});
                        node_set_origin(back, (union vec3){0.5, 0.5, 0});
                        node_set_rotation(back, quat_angle_axis(DEG_TO_RAD(180), (float[3]){0, 1, 0}));
                        node_add_child(child, back);
                        node_set_bright(back, 1.25);
                }
        }
}

struct game *game_alloc()
{
        struct game *p = smalloc(sizeof(struct game), game_free);
        game_setup(p);

        p->frame = 0;
        p->touching = 0;
        p->current_angle = 0;

        // create_game_1(p);
        // create_game_2(p);

        test_node = node_alloc(p->manager_game);
        node_set_origin(test_node, (union vec3){0.5, 0.5, 0.5});
        node_set_position(test_node, (union vec3){video_width/2, video_height/2, -video_height/2});
        // node_set_position(test_node, (union vec3){video_width/2, video_height/2, 0});

        // {
                // struct node *layout     = node_alloc(p->manager_game);
                // node_add_child(test_node, layout);
                // node_set_origin(layout, (union vec3){0.5, 0.5, 0.5});
                // node_set_position(layout, (union vec3){0, 0, 0});
                //
                // {
                //         struct node *child = node_alloc(p->manager_game);
                //         struct texture_frame *frame = texture_frame_alloc(texture_alloc_file("res/images/macao/table-poker.png", FILE_INNER));
                //         node_show_sprite(child, SHADER_3D_TEXTURE_COLOR, frame, 1, GFX_SPRITE_NORMAL);
                //         float width = video_width * 4/6;
                //         node_set_size(child, (union vec3){width, child->original_size.y / child->original_size.x * width, 1});
                //         node_set_position(child, (union vec3){0, 0, video_height/2});
                //         node_set_origin(child, (union vec3){0.5, 0.5, 0.5});
                //         node_add_child(layout, child);
                //         node_set_bright(child, 1.25);
                //         texture_frame_free(frame);
                // }

                // {
                //         struct node *child = node_alloc(p->manager_game);
                //         struct texture_frame *frame = texture_frame_alloc(render_pass_texture_get_texture(p->game_1_pass));
                //         node_show_sprite(child, SHADER_3D_TEXTURE_COLOR, frame, 1, GFX_SPRITE_FBO);
                //         node_set_size(child, (union vec3){video_width/2, video_height/2, 1});
                //         node_set_position(child, (union vec3){-video_width/2 + video_width/4, -video_height/2 + video_height/4, video_height/2});
                //         node_set_origin(child, (union vec3){0.5, 0.5, 0.5});
                //         node_add_child(layout, child);
                //         union vec2 offset = (union vec2){0, 0};
                //         texture_frame_free(frame);
                // }
        //         {
        //                 struct node *child = node_alloc(p->manager_game);
        //                 struct texture_frame *frame = texture_frame_alloc(render_pass_texture_get_texture(p->game_1_pass));
        //                 node_show_sprite(child, SHADER_3D_TEXTURE_COLOR, frame, 1, GFX_SPRITE_FBO);
        //                 node_set_size(child, (union vec3){video_width/2, video_height/2, 1});
        //                 node_set_position(child, (union vec3){video_width/4, video_height/4, video_height/2});
        //                 node_set_origin(child, (union vec3){0.5, 0.5, 0.5});
        //                 node_add_child(layout, child);
        //                 union vec2 offset = (union vec2){video_width/2, video_height/2};
        //                 texture_frame_free(frame);
        //         }
        //         {
        //                 struct node *child = node_alloc(p->manager_game);
        //                 struct texture_frame *frame = texture_frame_alloc(render_pass_texture_get_texture(p->game_2_pass));
        //                 node_show_sprite(child, SHADER_3D_TEXTURE_COLOR, frame, 1, GFX_SPRITE_FBO);
        //                 node_set_size(child, (union vec3){video_width/2, video_height/2, 1});
        //                 node_set_position(child, (union vec3){-video_width/2 + video_width/4, video_height/4, video_height/2});
        //                 node_set_origin(child, (union vec3){0.5, 0.5, 0.5});
        //                 node_add_child(layout, child);
        //                 texture_frame_free(frame);
        //         }
        //         {
        //                 struct node *child = node_alloc(p->manager_game);
        //                 struct texture_frame *frame = texture_frame_alloc(render_pass_texture_get_texture(p->game_2_pass));
        //                 node_show_sprite(child, SHADER_3D_TEXTURE_COLOR, frame, 1, GFX_SPRITE_FBO);
        //                 node_set_size(child, (union vec3){video_width/2, video_height/2, 1});
        //                 node_set_position(child, (union vec3){video_width/4, -video_height/2 + video_height/4, video_height/2});
        //                 node_set_origin(child, (union vec3){0.5, 0.5, 0.5});
        //                 node_add_child(layout, child);
        //                 texture_frame_free(frame);
        //         }
        // }
        // {
        //         struct node *layout     = node_alloc(p->manager_game);
        //         node_add_child(test_node, layout);
        //         node_set_origin(layout, (union vec3){0.5, 0.5, 0.5});
        //         node_set_position(layout, (union vec3){0, 0, 0});
        //
        //         {
        //                 struct node *child = node_alloc(p->manager_game);
        //                 struct texture_frame *frame = texture_frame_alloc(render_pass_texture_get_texture(p->game_1_pass));
        //                 node_show_sprite(child, SHADER_3D_TEXTURE_COLOR, frame, 1, GFX_SPRITE_FBO);
        //                 node_set_size(child, (union vec3){video_width/2, video_height/2, 1});
        //                 node_set_position(child, (union vec3){-video_width/2 + video_width/4, -video_height/2 + video_height/4, video_height/2});
        //                 node_set_origin(child, (union vec3){0.5, 0.5, 0.5});
        //                 node_add_child(layout, child);
        //                 union vec2 offset = (union vec2){0, 0};
        //                 texture_frame_free(frame);
        //         }
        //         {
        //                 struct node *child = node_alloc(p->manager_game);
        //                 struct texture_frame *frame = texture_frame_alloc(render_pass_texture_get_texture(p->game_1_pass));
        //                 node_show_sprite(child, SHADER_3D_TEXTURE_COLOR, frame, 1, GFX_SPRITE_FBO);
        //                 node_set_size(child, (union vec3){video_width/2, video_height/2, 1});
        //                 node_set_position(child, (union vec3){video_width/4, video_height/4, video_height/2});
        //                 node_set_origin(child, (union vec3){0.5, 0.5, 0.5});
        //                 node_add_child(layout, child);
        //                 union vec2 offset = (union vec2){video_width/2, video_height/2};
        //                 texture_frame_free(frame);
        //         }
        //         {
        //                 struct node *child = node_alloc(p->manager_game);
        //                 struct texture_frame *frame = texture_frame_alloc(render_pass_texture_get_texture(p->game_2_pass));
        //                 node_show_sprite(child, SHADER_3D_TEXTURE_COLOR, frame, 1, GFX_SPRITE_FBO);
        //                 node_set_size(child, (union vec3){video_width/2, video_height/2, 1});
        //                 node_set_position(child, (union vec3){-video_width/2 + video_width/4, video_height/4, video_height/2});
        //                 node_set_origin(child, (union vec3){0.5, 0.5, 0.5});
        //                 node_add_child(layout, child);
        //                 texture_frame_free(frame);
        //         }
        //         {
        //                 struct node *child = node_alloc(p->manager_game);
        //                 struct texture_frame *frame = texture_frame_alloc(render_pass_texture_get_texture(p->game_2_pass));
        //                 node_show_sprite(child, SHADER_3D_TEXTURE_COLOR, frame, 1, GFX_SPRITE_FBO);
        //                 node_set_size(child, (union vec3){video_width/2, video_height/2, 1});
        //                 node_set_position(child, (union vec3){video_width/4, -video_height/2 + video_height/4, video_height/2});
        //                 node_set_origin(child, (union vec3){0.5, 0.5, 0.5});
        //                 node_add_child(layout, child);
        //                 texture_frame_free(frame);
        //         }
        //         node_set_rotation(layout, quat_angle_axis(DEG_TO_RAD(90), (float[3]){1, 0, 0}));
        // }
        // {
        //         struct node *layout     = node_alloc(p->manager_game);
        //         node_add_child(test_node, layout);
        //         node_set_origin(layout, (union vec3){0.5, 0.5, 0.5});
        //         node_set_position(layout, (union vec3){0, 0, 0});
        //
        //         {
        //                 struct node *child = node_alloc(p->manager_game);
        //                 struct texture_frame *frame = texture_frame_alloc(render_pass_texture_get_texture(p->game_1_pass));
        //                 node_show_sprite(child, SHADER_3D_TEXTURE_COLOR, frame, 1, GFX_SPRITE_FBO);
        //                 node_set_size(child, (union vec3){video_width/2, video_height/2, 1});
        //                 node_set_position(child, (union vec3){-video_width/2 + video_width/4, -video_height/2 + video_height/4, video_height/2});
        //                 node_set_origin(child, (union vec3){0.5, 0.5, 0.5});
        //                 node_add_child(layout, child);
        //                 union vec2 offset = (union vec2){0, 0};
        //                 texture_frame_free(frame);
        //         }
        //         {
        //                 struct node *child = node_alloc(p->manager_game);
        //                 struct texture_frame *frame = texture_frame_alloc(render_pass_texture_get_texture(p->game_1_pass));
        //                 node_show_sprite(child, SHADER_3D_TEXTURE_COLOR, frame, 1, GFX_SPRITE_FBO);
        //                 node_set_size(child, (union vec3){video_width/2, video_height/2, 1});
        //                 node_set_position(child, (union vec3){video_width/4, video_height/4, video_height/2});
        //                 node_set_origin(child, (union vec3){0.5, 0.5, 0.5});
        //                 node_add_child(layout, child);
        //                 union vec2 offset = (union vec2){video_width/2, video_height/2};
        //                 texture_frame_free(frame);
        //         }
        //         {
        //                 struct node *child = node_alloc(p->manager_game);
        //                 struct texture_frame *frame = texture_frame_alloc(render_pass_texture_get_texture(p->game_2_pass));
        //                 node_show_sprite(child, SHADER_3D_TEXTURE_COLOR, frame, 1, GFX_SPRITE_FBO);
        //                 node_set_size(child, (union vec3){video_width/2, video_height/2, 1});
        //                 node_set_position(child, (union vec3){-video_width/2 + video_width/4, video_height/4, video_height/2});
        //                 node_set_origin(child, (union vec3){0.5, 0.5, 0.5});
        //                 node_add_child(layout, child);
        //                 texture_frame_free(frame);
        //         }
        //         {
        //                 struct node *child = node_alloc(p->manager_game);
        //                 struct texture_frame *frame = texture_frame_alloc(render_pass_texture_get_texture(p->game_2_pass));
        //                 node_show_sprite(child, SHADER_3D_TEXTURE_COLOR, frame, 1, GFX_SPRITE_FBO);
        //                 node_set_size(child, (union vec3){video_width/2, video_height/2, 1});
        //                 node_set_position(child, (union vec3){video_width/4, -video_height/2 + video_height/4, video_height/2});
        //                 node_set_origin(child, (union vec3){0.5, 0.5, 0.5});
        //                 node_add_child(layout, child);
        //                 texture_frame_free(frame);
        //         }
        //         node_set_rotation(layout, quat_angle_axis(DEG_TO_RAD(180), (float[3]){1, 0, 0}));
        // }
        // {
        //         struct node *layout     = node_alloc(p->manager_game);
        //         node_add_child(test_node, layout);
        //         node_set_origin(layout, (union vec3){0.5, 0.5, 0.5});
        //         node_set_position(layout, (union vec3){0, 0, 0});
        //
        //         {
        //                 struct node *child = node_alloc(p->manager_game);
        //                 struct texture_frame *frame = texture_frame_alloc(render_pass_texture_get_texture(p->game_1_pass));
        //                 node_show_sprite(child, SHADER_3D_TEXTURE_COLOR, frame, 1, GFX_SPRITE_FBO);
        //                 node_set_size(child, (union vec3){video_width/2, video_height/2, 1});
        //                 node_set_position(child, (union vec3){-video_width/2 + video_width/4, -video_height/2 + video_height/4, video_height/2});
        //                 node_set_origin(child, (union vec3){0.5, 0.5, 0.5});
        //                 node_add_child(layout, child);
        //                 union vec2 offset = (union vec2){0, 0};
        //                 texture_frame_free(frame);
        //         }
        //         {
        //                 struct node *child = node_alloc(p->manager_game);
        //                 struct texture_frame *frame = texture_frame_alloc(render_pass_texture_get_texture(p->game_1_pass));
        //                 node_show_sprite(child, SHADER_3D_TEXTURE_COLOR, frame, 1, GFX_SPRITE_FBO);
        //                 node_set_size(child, (union vec3){video_width/2, video_height/2, 1});
        //                 node_set_position(child, (union vec3){video_width/4, video_height/4, video_height/2});
        //                 node_set_origin(child, (union vec3){0.5, 0.5, 0.5});
        //                 node_add_child(layout, child);
        //                 union vec2 offset = (union vec2){video_width/2, video_height/2};
        //                 texture_frame_free(frame);
        //         }
        //         {
        //                 struct node *child = node_alloc(p->manager_game);
        //                 struct texture_frame *frame = texture_frame_alloc(render_pass_texture_get_texture(p->game_2_pass));
        //                 node_show_sprite(child, SHADER_3D_TEXTURE_COLOR, frame, 1, GFX_SPRITE_FBO);
        //                 node_set_size(child, (union vec3){video_width/2, video_height/2, 1});
        //                 node_set_position(child, (union vec3){-video_width/2 + video_width/4, video_height/4, video_height/2});
        //                 node_set_origin(child, (union vec3){0.5, 0.5, 0.5});
        //                 node_add_child(layout, child);
        //                 texture_frame_free(frame);
        //         }
        //         {
        //                 struct node *child = node_alloc(p->manager_game);
        //                 struct texture_frame *frame = texture_frame_alloc(render_pass_texture_get_texture(p->game_2_pass));
        //                 node_show_sprite(child, SHADER_3D_TEXTURE_COLOR, frame, 1, GFX_SPRITE_FBO);
        //                 node_set_size(child, (union vec3){video_width/2, video_height/2, 1});
        //                 node_set_position(child, (union vec3){video_width/4, -video_height/2 + video_height/4, video_height/2});
        //                 node_set_origin(child, (union vec3){0.5, 0.5, 0.5});
        //                 node_add_child(layout, child);
        //                 texture_frame_free(frame);
        //         }
        //         node_set_rotation(layout, quat_angle_axis(DEG_TO_RAD(270), (float[3]){1, 0, 0}));
        // }


        // {
        //         struct node *n = node_alloc(p->manager_game);
        //         node_set_origin(n, (union vec3){0.5, 0.5, 0.0});
        //         node_set_position(n, (union vec3){video_width/2, video_height/2, 0});
        //
        //         struct xml_element *xml = xml_parse("res/plist/string.xml", FILE_INNER);
        //         struct xml_element *message = xml_find(xml, "string", 0);
        //         struct string * s = string_alloc(0);
        //         string_cat_string(s, message->value);
        //         string_cat_string(s, message->value);
        //         string_cat_string(s, message->value);
        //         string_cat_string(s, message->value);
        //         string_cat_string(s, message->value);
        //         string_cat_string(s, message->value);
        //         string_cat_string(s, message->value);
        //         string_cat_string(s, message->value);
        //         string_cat_string(s, message->value);
        //         string_cat_string(s, message->value);
        //         node_show_text(n, SHADER_3D_TEXTURE_COLOR,
        //                 s->ptr, s->len,
        //                 "res/font/cjk/font_regular_24/font_regular_24.fnt",
        //                 FILE_INNER,
        //                 sizeof("res/font/cjk/font_regular_24/font_regular_24.fnt")-1,
        //                 video_width-20,
        //                 24,
        //                 TEXT_ALIGN_LEFT);
        //         xml_free(xml);
        //         string_free(s);
        //         node_add_child(test_node, n);
        // }
        //
        // int i;
        // for_i(i, 10) {
        //         struct node *child = node_alloc(p->manager_game);
        //         node_show_spine(child, SHADER_3D_TEXTURE_COLOR, "res/spine/raptor.skel", FILE_INNER, "res/spine/raptor.atlas", FILE_INNER, 0.05);
        //         node_spine_set_animation(child, 0, "walk", 1);
        //         node_spine_add_animation(child, 1, "gungrab", 0, 2);
        //
        //         node_set_origin(child, (union vec3){0, 0, 0});
        //         node_set_position(child, (union vec3){rand_rf(0, video_width), rand_rf(0, video_height), 0});
        //         float size = rand_rf(1.0, 5.0);
        //         node_set_size(child, (union vec3){size, size, 1});
        //         node_add_child(test_node, child);
        //
        //         node_spine_run_animation(child);
        // }
        return p;
}

static inline void __game_update_task(struct game *p)
{
        int i;
        struct node_manager **manager    = &p->manager[0];
        for_i(i, sizeof(p->manager) / sizeof(p->manager[0])) {
                node_manager_update_tasks(*manager, 1.0f / 60, p->frame);
                manager++;
        }
}

static inline void __game_update_actions(struct game *p)
{
        int i;
        struct node_manager **manager    = &p->manager[0];
        for_i(i, sizeof(p->manager) / sizeof(p->manager[0])) {
                node_manager_update_action(*manager, 1.0f / 60);
                manager++;
        }
}

static inline void __game_update_transform(struct game *p)
{
        int i;
        struct node_manager **manager    = &p->manager[0];
        for_i(i, sizeof(p->manager) / sizeof(p->manager[0])) {
                node_manager_update_transform(*manager);
                manager++;
        }
}

float deg = 0;

void game_update(struct game *p)
{
        __game_update_task(p);
        __game_update_actions(p);
        __game_update_transform(p);
}

void game_render(struct game *p)
{
        // render_pass_begin(p->game_1_pass, p->frame);
        // node_render(game_1, p->frame);
        // render_pass_end();
        //
        // render_pass_begin(p->game_2_pass, p->frame);
        // node_render(game_2, p->frame);
        // render_pass_end();

        render_pass_begin(p->main_pass, p->frame);
        node_render(test_node, p->frame);
        // node_render(game_1, p->frame);
        render_pass_end();

        p->frame++;
        if(p->frame == BUFFERS) p->frame = 0;
}

static void __game_process_point(struct game *p, union vec2 point, u8 state)
{
        switch (p->touching) {
                case 0:
                        if(state == MOUSE_DOWN && list_singular(&test_node->key.actions)) {
                                p->current_angle = 0;
                                p->touching = 1;
                                p->last_touch = point;
                        }
                        break;
                case 1:
                        if(state == MOUSE_MOVE) {
                                float offset_y = point.y - p->last_touch.y;
                                p->current_angle -= offset_y / 10.0f;
                                p->current_angle = MAX(-90, MIN(90, p->current_angle));
                                p->last_touch = point;
                                node_set_rotation(test_node, quat_angle_axis(DEG_TO_RAD(p->current_angle), (float[3]){1, 0, 0}));
                        } else if(state == MOUSE_CANCEL || state == MOUSE_UP) {
                                if(p->current_angle > -90 && p->current_angle < 90) {
                                        if(p->current_angle < 0) {
                                                if(p->current_angle > -10) {
                                                        node_run_action(test_node,
                                                                node_rotate_by(test_node, DEG_TO_RAD(0 - p->current_angle), (union vec3){1, 0, 0}, 0.25, EASE_QUADRATIC_OUT, 0), NULL);
                                                } else {
                                                        node_run_action(test_node,
                                                                node_rotate_by(test_node, DEG_TO_RAD(-90 - p->current_angle), (union vec3){1, 0, 0}, 1, EASE_QUADRATIC_OUT, 0), NULL);
                                                }
                                        } else {
                                                if(p->current_angle < 10) {
                                                        node_run_action(test_node,
                                                                node_rotate_by(test_node, DEG_TO_RAD(0 - p->current_angle), (union vec3){1, 0, 0}, 0.25, EASE_QUADRATIC_OUT, 0), NULL);
                                                } else {
                                                        node_run_action(test_node,
                                                                node_rotate_by(test_node, DEG_TO_RAD(90 - p->current_angle), (union vec3){1, 0, 0}, 1, EASE_QUADRATIC_OUT, 0), NULL);
                                                }
                                        }
                                        p->current_angle = 0;
                                        p->touching = 0;
                                }
                        }
                        break;
        }
}

static void __game_read_mouse_event(struct game *p, struct event *e)
{
        union mat4 proj         = camera_get_project(p->game_camera);
        union mat4 m            = mat4_mul(proj, camera_get_view(p->game_camera));
        m                       = mat4_invert(m);

        float x                 = 2.0f * e->mouse_x / video_width - 1.0f;
        float y                 = 1.0f - 2.0f * e->mouse_y / video_height;

        union vec3 start        = mat4_mul_project_vec3(m, (union vec3){x, y, -1});
        union vec3 end          = mat4_mul_project_vec3(m, (union vec3){x ,y, -3});
        union vec3 dir          = vec3_normalize(vec3_sub(end, start));
        union vec3 normal       = (union vec3){0, 0, 1};

        float r1                = vec3_dot(dir, normal);
        float r2                = vec3_dot(start, normal);

        float t                 = 0;
        if(r1 != 0) {
                t               = -r2 / r1;
        }
        union vec3 result       = vec3_add(vec3_mul_scalar(dir, t), start);
        __game_process_point(p, result.xy, e->mouse_state);
}

void game_read_event(struct game *p, struct event *e)
{
        switch (e->type) {
                case EVENT_MOUSE:
                case EVENT_TOUCH:
                        __game_read_mouse_event(p, e);
                        break;
        }
}

void game_free(struct game *p)
{
        int i;
        struct node_manager **manager   = &p->manager[0];
        for_i(i, sizeof(p->manager) / sizeof(p->manager[0])) {
                node_manager_free(*manager);
                manager++;
        }

        struct camera **camera          = &p->camera[0];
        for_i(i, sizeof(p->camera) / sizeof(p->camera[0])) {
                camera_free(*camera);
                camera++;
        }

        struct render_pass **pass       = &p->pass[0];
        for_i(i, sizeof(p->pass) / sizeof(p->pass[0])) {
                render_pass_free(*pass);
                pass++;
        }

        struct light **light            = &p->light[0];
        for_i(i, sizeof(p->light) / sizeof(p->light[0])) {
                light_free(*light);
                light++;
        }

        sfree(p);
}

void game_resize(struct game *p, int width, int height)
{
        // video_width = width;
        // video_height = height;
        // union mat4 project      = mat4_new_ortho(
        //         0, video_width,
        //         0, video_height,
        //         0, 2
        // );
        // camera_set_project(p->game_camera, project);
        // camera_set_project(p->hud_camera, project);
}
