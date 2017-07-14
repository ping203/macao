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
#include "game.h"

static void on_process(struct nexec *p, float d);
static void on_net(struct smart_listener *lis, struct smart_network_data_head *head);

static void on_extension(struct nexec *p, struct smart_extension_in_out *out);
static void on_slotmachine_update_jackpot(struct nexec *p, struct smart_extension_in_out *out);
static void on_user_info(struct nexec *p, struct smart_extension_in_out *out);

struct exec_data {
        struct smart_listener sfs_listener;
        struct nexec *controller;
        struct map *cmds;
};

static void exec_data_free(struct exec_data *p)
{
        smart_listener_release(&p->sfs_listener);
        map_free(p->cmds);
        sfree(p);
}

static struct exec_data *exec_data_alloc(struct nexec *controller)
{
        struct exec_data *p     = smalloc(sizeof(struct exec_data), exec_data_free);
        smart_listener_init(&p->sfs_listener);
        p->sfs_listener.delegate = on_net;
        p->controller = controller;
        p->cmds = map_alloc(sizeof(extension_response));

        map_set(p->cmds, qlkey("slotmachine_update_jackpot"),
                &(extension_response){on_slotmachine_update_jackpot});
        map_set(p->cmds, qlkey("user_info"),
                &(extension_response){on_user_info});

        struct smartfox_client *cli = sfs_client_shared();
        smartfox_client_add_listener(cli, &p->sfs_listener);
        return p;
}

/*
 * exec
 */
struct nexec *mc_root_exec_alloc()
{
        struct nexec *p         = nexec_alloc();
        p->custom_data          = exec_data_alloc(p);
        p->custom_data_free     = exec_data_free;

        nexec_reg(p, qlkey("update"), -1, on_process);

        return p;
}

static void on_net(struct smart_listener *lis, struct smart_network_data_head *head)
{
        struct exec_data *d = (struct exec_data *)
                ((char *)lis - offsetof(struct exec_data, sfs_listener));
        struct nexec *p = d->controller;

        switch (head->type) {
                case SMART_EXTENSION_IN_OUT:
                        on_extension(p, (struct smart_extension_in_out *)
                                ((char *)head - offsetof(struct smart_extension_in_out, head)));
                        break;
        }
}

static void on_extension(struct nexec *p, struct smart_extension_in_out *out)
{
        struct exec_data *d = (struct exec_data *)p->custom_data;

        extension_response *cmd = map_get_pointer(d->cmds, qskey(out->cmd));

        if(*cmd) {
                (*cmd)(p, out);
        }
}

static void on_slotmachine_update_jackpot(struct nexec *p, struct smart_extension_in_out *out)
{
        struct sarray *jp = sobj_get_arr(out->object, qlkey("arr_jackpot"), RPL_TYPE);
        slot_info_update(jp);
}

static void on_user_info(struct nexec *p, struct smart_extension_in_out *out)
{
        struct sobj *uf = user_info_shared();
        i64 ltmp = 0;
        i32 itmp = 0;
        struct string *stmp = NULL;

        stmp = sobj_get_str(out->object, qlkey("nick_name"), RPL_TYPE);
        sobj_set_str(uf, qlkey("nick_name"), qskey(stmp));

        stmp = sobj_get_str(out->object, qlkey("avatar"), RPL_TYPE);
        sobj_set_str(uf, qlkey("avatar"), qskey(stmp));

        ltmp = sobj_get_i64(out->object, qlkey("money"), RPL_TYPE);
        sobj_set_i64(uf, qlkey("money"), ltmp);

        itmp = sobj_get_i32(out->object, qlkey("vip_level"), RPL_TYPE);
        sobj_set_i32(uf, qlkey("vip_level"), itmp);

        ltmp = sobj_get_i64(out->object, qlkey("free_chip_money"), RPL_TYPE);
        sobj_set_i64(uf, qlkey("free_chip_money"), ltmp);

        stmp = sobj_get_str(out->object, qlkey("phone"), RPL_TYPE);
        sobj_set_str(uf, qlkey("phone"), qskey(stmp));

        itmp = sobj_get_i32(uf, qlkey("app_type"), RPL_TYPE);
        switch (itmp) {
                case APP_ANDROID:
                case APP_WINPHONE:
                        itmp = sobj_get_i32(out->object, qlkey("open_money"), RPL_TYPE);
                        sobj_set_i32(uf, qlkey("use_payment"), itmp);
                        break;
                case APP_IOS:
                        itmp = sobj_get_i32(out->object, qlkey("app_store_open"), RPL_TYPE);
                        sobj_set_i32(uf, qlkey("use_payment"), itmp);
                        break;
                case APP_WEB:
                        sobj_set_i32(uf, qlkey("use_payment"), 1);
                        break;
        }

        stmp = sobj_get_str(out->object, qlkey("card_types"), RPL_TYPE);
        sobj_set_str(uf, qlkey("card_types"), qskey(stmp));
        stmp = sobj_get_str(out->object, qlkey("card_types"), RPL_TYPE);
        sobj_set_str(uf, qlkey("card_values"), qskey(stmp));
}

static void on_process(struct nexec *p, float d)
{
        struct smartfox_client *cli = sfs_client_shared();

        smartfox_client_process_send(cli);
        smartfox_client_process_response(cli);
}
