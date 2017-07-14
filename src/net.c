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
#include <macao/net.h>
#include <cherry/array.h>
#include <cherry/map.h>
#include <cherry/string.h>
#include <cherry/memory.h>
#include <smartfox/smartfox.h>
#include <smartfox/data.h>
#include <smartfox/network_data.h>

static struct smartfox_client *__cli = NULL;
static struct sobj *__user_info = NULL;
static u8 __game_state = GAME_NONE;
static struct sarray *__slot_info = NULL;

struct smartfox_client *sfs_client_shared()
{
        if(!__cli) {
                __cli = smartfox_client_alloc(qlkey("117.0.33.17"),8889,qlkey("BaiIBigZone"));
        }
        return __cli;
}

void net_get_user_info()
{
        struct string *stmp;
        i32 itmp;
        user_info_shared();

        struct smart_extension_in_out *ex = smart_extension_in_out_alloc();
        ex->send_room = 0;
        string_cat(ex->cmd, qlkey("lobby.user_info"));

        stmp = sobj_get_str(__user_info, qlkey("nick_name"), RPL_TYPE);
        sobj_set_str(ex->object, qlkey("nick_name"), qskey(stmp));

        stmp = sobj_get_str(__user_info, qlkey("avatar"), RPL_TYPE);
        sobj_set_str(ex->object, qlkey("avatar"), qskey(stmp));

        stmp = sobj_get_str(__user_info, qlkey("package_game"), RPL_TYPE);
        sobj_set_str(ex->object, qlkey("package_game"), qskey(stmp));

        stmp = sobj_get_str(__user_info, qlkey("version"), RPL_TYPE);
        sobj_set_str(ex->object, qlkey("version"), qskey(stmp));

        stmp = sobj_get_str(__user_info, qlkey("platform"), RPL_TYPE);
        sobj_set_str(ex->object, qlkey("platform"), qskey(stmp));

        itmp = sobj_get_i32(__user_info, qlkey("partner_id"), RPL_TYPE);
        sobj_set_i32(ex->object, qlkey("partner_id"), itmp);

        itmp = sobj_get_i32(__user_info, qlkey("app_type"), RPL_TYPE);
        sobj_set_i32(ex->object, qlkey("type"), itmp);

        stmp = sobj_get_str(__user_info, qlkey("protocol"), RPL_TYPE);
        sobj_set_str(ex->object, qlkey("protocol"), qskey(stmp));

        smartfox_client_send(__cli, &ex->head);
}

struct sobj *user_info_shared()
{
        if(!__user_info) {
                __user_info = sobj_alloc();

                sobj_set_str(__user_info, qlkey("package_game"), qlkey("com.gamebai.bai69html"));
                sobj_set_str(__user_info, qlkey("version"), qlkey("14/07/2017"));
                sobj_set_str(__user_info, qlkey("platform"), qlkey("web"));
                sobj_set_str(__user_info, qlkey("imei"), qlkey("imei_web"));
                sobj_set_i32(__user_info, qlkey("partner_id"), 30);
                sobj_set_i32(__user_info, qlkey("app_type"), 0);
                sobj_set_str(__user_info, qlkey("protocol"), qlkey("tcp"));
                sobj_set_i32(__user_info, qlkey("use_payment"), 0);
                sobj_set_i32(__user_info, qlkey("use_refcode"), 0);
        }
        return __user_info;
}

u8 game_get_state()
{
        return __game_state;
}

void game_set_state(u8 state)
{
        __game_state = state;
}

struct sarray *slot_info_shared()
{
        if(!__slot_info) {
                __slot_info  = sarray_alloc();
        }
        return __slot_info;
}

void slot_info_update(struct sarray *s)
{
        if(__slot_info) {
                sarray_free(__slot_info);
        }

        struct string *j = sarray_to_json(s);
        __slot_info = sarray_from_json(j, j->len, &(int){0});
        string_free(j);
}
