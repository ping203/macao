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
#ifndef __MACAO_TYPES_H__
#define __MACAO_TYPES_H__

#ifdef __cplusplus
extern "C" {
#endif

#include <native_ui/types.h>
#include <smartfox/types.h>
#include <cherry/graphic/types.h>

#define SET_STRING(str) {       \
        .len = sizeof(str) - 1, \
        .ptr = str              \
};

enum game_state {
        GAME_NONE,
        GAME_CONNECTING_TO_LOGIN,
        GAME_LOGINING,
        GAME_LOGINED
};

enum app_type {
        APP_WEB = 0,
        APP_JAVA = 1,
        APP_ANDROID = 2,
        APP_IOS = 3,
        APP_WINPHONE = 4
};

struct game {
        union {
                struct {
                        struct node_manager     *manager_game;
                        struct node_manager     *manager_hud;
                };
                struct node_manager             *manager[2];
        };
        union {
                struct {
                        struct camera           *game_camera;
                        struct camera           *hud_camera;
                };
                struct camera                   *camera[2];
        };
        union {
                struct {
                        struct render_pass      *main_pass;
                        struct render_pass      *game_1_pass;
                        struct render_pass      *game_2_pass;
                };
                struct render_pass              *pass[3];
        };
        union {
                struct {
                        struct light            *game_light;
                };
                struct light                    *light[1];
        };

        u8                                      frame;
        u8                                      touching;
        float                                   current_angle;
        union vec2                              last_touch;
};


#ifdef __cplusplus
}
#endif

#endif
