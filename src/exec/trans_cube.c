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
#include <macao/exec.h>
#include <native_ui/view_controller.h>
#include <native_ui/action.h>
#include <native_ui/touch_handle.h>
#include <native_ui/parser.h>
#include <native_ui/preferences.h>
#include <native_ui/manager.h>
#include <smartfox/data.h>
#include <cherry/stdio.h>
#include <native_ui/view.h>
#include <cherry/math/math.h>
#include <cherry/map.h>
#include <cherry/array.h>
#include <cherry/memory.h>
#include <cherry/string.h>

static void on_linked(struct nexec *p);
static void on_removed(struct nexec *p);
static void touch_3d(struct nexec *p, struct nview *sender, u8 type);

/*
 * data
 */
struct exec_data {
        union {
                float degree;
                union vec4 degree_expanded;
        };
        struct naction_key      action_key;
        u8                      can_rotate;
        u8                      current_part;
};

static void exec_data_free(struct exec_data *p)
{
        naction_key_clear(&p->action_key);
        sfree(p);
}

static struct exec_data *exec_data_alloc(struct nexec *controller)
{
        struct exec_data *p     = smalloc(sizeof(struct exec_data), exec_data_free);
        p->degree_expanded      = (union vec4){0, 0, 0, 0};
        p->can_rotate           = 0;
        p->current_part         = 0;
        naction_key_init(&p->action_key);

        return p;
}


/*
 * exec
 */
struct nexec *mc_trans_cube_exec_alloc()
{
        debug("native ui create cube\n");
        struct nexec *p                         = nexec_alloc();
        p->on_linked                            = on_linked;
        p->on_removed                           = on_removed;
        p->custom_data                          = exec_data_alloc(p);
        p->custom_data_free                     = exec_data_free;

        return p;
}

static void on_linked(struct nexec *p)
{
        struct nview *view = nexec_get_view(p);
        struct nparser *pr = nview_get_parser(view);

        struct ntouch *t = nparser_get_touch(pr, qlkey("touch_3d"));
        ntouch_set_f(t, touch_3d, p, NULL);
}

static void on_removed(struct nexec *p)
{

}

struct mov_area {
        float   d;
        int     part;
};

static struct mov_area process_3d(struct nexec *p)
{
        struct exec_data *data = (struct exec_data *)p->custom_data;

        struct nview *view = nexec_get_view(p);
        struct nparser *pr = nview_get_parser(view);

        struct nview *c1 = nparser_get_hash_view(pr, qlkey("cube_1"));
        struct nview *c2 = nparser_get_hash_view(pr, qlkey("cube_2"));
        struct nview *c3 = nparser_get_hash_view(pr, qlkey("cube_3"));
        struct nview *c4 = nparser_get_hash_view(pr, qlkey("cube_4"));

        struct nview *up, *down;
        struct mov_area ret;

        if(data->degree <= 90) {
                nview_set_visible(c1, 1);
                nview_set_visible(c2, 1);
                nview_set_visible(c3, 0);
                nview_set_visible(c4, 0);
                up = c1;
                down = c2;
                ret.d = data->degree;
                ret.part = 0;
        } else if(data->degree <= 180) {
                nview_set_visible(c1, 0);
                nview_set_visible(c2, 1);
                nview_set_visible(c3, 1);
                nview_set_visible(c4, 0);
                up = c2;
                down = c3;
                ret.d = data->degree - 90;
                ret.part = 1;
        } else if(data->degree <= 270) {
                nview_set_visible(c1, 0);
                nview_set_visible(c2, 0);
                nview_set_visible(c3, 1);
                nview_set_visible(c4, 1);
                up = c3;
                down = c4;
                ret.d = data->degree - 180;
                ret.part = 2;
        } else {
                nview_set_visible(c1, 1);
                nview_set_visible(c2, 0);
                nview_set_visible(c3, 0);
                nview_set_visible(c4, 1);
                up = c4;
                down = c1;
                ret.d = data->degree - 270;
                ret.part = 3;
        }

        nview_set_anchor(up, (union vec2){0.5, 1.0});
        nview_set_anchor(down, (union vec2){0.5, 0.0});

        nview_request_margin(up, (union vec4){
                .top = -view->size.height * ret.d / 90
        });
        nview_request_margin(down, (union vec4){
                .top = view->size.height * (90 - ret.d) / 90
        });

        nview_request_rotation(up, (union vec3){ret.d, 0, 0});
        nview_request_rotation(down, (union vec3){ret.d - 90, 0, 0});

        return ret;
}

static void process_action(struct nexec *p)
{
        process_3d(p);
}

static void touch_3d(struct nexec *p, struct nview *s, u8 t)
{
        struct exec_data *data = (struct exec_data *)p->custom_data;
        switch (t) {
                case NATIVE_UI_TOUCH_BEGAN:
                        break;
                case NATIVE_UI_TOUCH_MOVED:
                case NATIVE_UI_TOUCH_ENDED:
                case NATIVE_UI_TOUCH_CANCELLED:
                {
                        if(!data->can_rotate) {
                                if(t == NATIVE_UI_TOUCH_MOVED) {
                                        if(fabsf(s->touch_moved_point.x - s->touch_began_point.x) >= 10
                                                && fabsf(s->touch_moved_point.y - s->touch_began_point.y) <= 10) {

                                                struct nview *view = nexec_get_view(p);
                                                struct nparser *pr = nview_get_parser(view);

                                                struct nview *c1 = nparser_get_hash_view(pr, qlkey("list_game_1"));
                                                struct nview *c2 = nparser_get_hash_view(pr, qlkey("list_game_2"));
                                                struct nview *c3 = nparser_get_hash_view(pr, qlkey("list_game_3"));
                                                struct nview *c4 = nparser_get_hash_view(pr, qlkey("list_game_4"));

                                                struct nview *target = NULL;
                                                switch (data->current_part) {
                                                        case 0:
                                                                target = c1;
                                                                break;
                                                        case 1:
                                                                target = c2;
                                                                break;
                                                        case 2:
                                                                target = c3;
                                                                break;
                                                        case 3:
                                                                target = c4;
                                                                break;
                                                        default:
                                                                target = c1;
                                                                break;
                                                }

                                                // struct nview *target = __find_intercept_horizontal_view(p);

                                                if(target) {
                                                        nview_touch_cancelled(s, s->touch_moved_point);
                                                        s->intercept_horizontal_target    = target;
                                                        nview_touch_began(target, nview_convert_point_to_view(s, s->touch_began_point, target));
                                                        break;
                                                }
                                        }
                                }
                                if(fabsf(s->touch_moved_point.y - s->touch_began_point.y) > 10) {
                                        data->can_rotate = 1;
                                }
                        }

                        if(!data->can_rotate) break;

                        data->degree -= s->touch_offset.y / 10;
                        if(data->degree > 360) {
                                data->degree -= 360;
                        }
                        if(data->degree < 0) {
                                data->degree += 360;
                        }
                        struct mov_area m = process_3d(p);
                        if(t == NATIVE_UI_TOUCH_ENDED || t == NATIVE_UI_TOUCH_CANCELLED) {
                                data->can_rotate = 0;
                                struct naction *act = NULL;
                                if(
                                        (m.d < 10 && s->touch_ended_point.y - s->touch_began_point.y < 0)
                                        || (m.d > 60 && m.d < 80 && s->touch_ended_point.y - s->touch_began_point.y > 0)
                                        ) {
                                        data->current_part = m.part;
                                        act = naction_alloc(&data->degree_expanded,
                                                vec4_sub((union vec4){
                                                        m.part * 90, 0, 0, 0
                                                }, data->degree_expanded),
                                                0.15, NATIVE_UI_EASE_CUBIC_OUT, 0,
                                                (nactionf)process_action, p);
                                } else {
                                        data->current_part = m.part + 1;
                                        act = naction_alloc(&data->degree_expanded,
                                                vec4_sub((union vec4){
                                                        (m.part+1) * 90, 0, 0, 0
                                                }, data->degree_expanded),
                                                0.15, NATIVE_UI_EASE_CUBIC_OUT, 0,
                                                (nactionf)process_action, p);
                                }
                                naction_key_add_action(&data->action_key, act);
                                nmanager_add_action_key(nmanager_shared(), &data->action_key);
                        }
                }
                        break;
                default:
                        break;
        }
}
