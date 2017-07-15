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

static void on_linked(struct nexec *p);
static void on_removed(struct nexec *p);
static void on_net(struct smart_listener *lis, struct smart_network_data_head *head);

static void on_connect(struct nexec *p, struct smart_connect_output *out);
static void on_login(struct nexec *p, struct smart_login_output *out);
static void on_join_room(struct nexec *p, struct smart_join_room_output *out);

static void on_extension(struct nexec *p, struct smart_extension_in_out *out);
static void on_user_info(struct nexec *p, struct smart_extension_in_out *out);
static void on_play_slot_machine(struct nexec *p, struct smart_extension_in_out *out);
static void on_slotmachine_get_table_info(struct nexec *p, struct smart_extension_in_out *out);

/*
 * data
 */
struct exec_data {
        struct smart_listener sfs_listener;
        struct nexec *controller;
        struct map *cmds;
        struct string *room_name;
};

static void exec_data_free(struct exec_data *p)
{
        smart_listener_release(&p->sfs_listener);
        string_free(p->room_name);
        map_free(p->cmds);
        sfree(p);
}

static struct exec_data *exec_data_alloc(struct nexec *controller)
{
        struct exec_data *p     = smalloc(sizeof(struct exec_data), exec_data_free);
        smart_listener_init(&p->sfs_listener);
        p->sfs_listener.delegate = on_net;
        p->controller = controller;
        p->room_name = string_alloc(0);
        p->cmds = map_alloc(sizeof(extension_response));

        map_set(p->cmds, qlkey("user_info"),
                &(extension_response){on_user_info});
        map_set(p->cmds, qlkey("play_slotmachine"),
                &(extension_response){on_play_slot_machine});
        map_set(p->cmds, qlkey("slotmachine_get_table_info"),
                &(extension_response){on_slotmachine_get_table_info});

        return p;
}


/*
 * exec
 */
struct nexec *mc_slot_exec_alloc()
{
        struct nexec *p                         = nexec_alloc();
        p->on_linked                            = on_linked;
        p->on_removed                           = on_removed;
        p->custom_data                          = exec_data_alloc(p);
        p->custom_data_free                     = exec_data_free;

        return p;
}

static void on_net(struct smart_listener *lis, struct smart_network_data_head *head)
{
        struct exec_data *d = (struct exec_data *)
                ((char *)lis - offsetof(struct exec_data, sfs_listener));
        struct nexec *p = d->controller;

        switch (head->type) {
                case SMART_CONNECT_OUT:
                        on_connect(p, (struct smart_connect_output *)
                                ((char *)head - offsetof(struct smart_connect_output, head)));
                        break;
                case SMART_LOGIN_OUT:
                        on_login(p, (struct smart_login_output *)
                                ((char *)head - offsetof(struct smart_login_output, head)));
                        break;
                case SMART_JOIN_ROOM_OUT:
                        on_join_room(p, (struct smart_join_room_output *)
                                ((char *)head - offsetof(struct smart_join_room_output, head)));
                        break;
                case SMART_EXTENSION_IN_OUT:
                        on_extension(p, (struct smart_extension_in_out *)
                                ((char *)head - offsetof(struct smart_extension_in_out, head)));
                        break;
        }
}

static void on_connect(struct nexec *p, struct smart_connect_output *out)
{
        struct sobj *uf = user_info_shared();

        struct smart_login_input *sli;
        struct smartfox_client *cli = sfs_client_shared();

        if(strcmp(out->result->ptr, "connected") == 0) {
                sli = smart_login_input_alloc();
                sobj_set_str(uf, qlkey("account"), qlkey("manh321321"));
                string_cat(sli->user_name, qlkey("manh321321"));
                string_cat(sli->password, qlkey("123123"));
                sli->partner_id = 30;
                sli->type = 2;
                string_cat(sli->imei, qlkey("imei_web"));
                string_cat(sli->version_code, qlkey("14/07/2017"));
                smartfox_client_send(cli, &sli->head);
                game_set_state(GAME_LOGINING);
        } else {
                game_set_state(GAME_NONE);
        }
}

static void on_login(struct nexec *p, struct smart_login_output *out)
{
        struct sobj *uf = user_info_shared();
        struct string *stmp = NULL;

        u8 st = game_get_state();
        switch (st) {
                case GAME_LOGINING:
                        if(strcmp(out->result->ptr, "success") == 0) {
                                game_set_state(GAME_LOGINED);

                                stmp = sobj_get_str(out->object, qlkey("$FS_NEW_LOGIN_NAME"), RPL_TYPE);
                                sobj_set_str(uf, qlkey("nick_name"), qskey(stmp));

                                net_get_user_info();
                        } else {
                                game_set_state(GAME_NONE);
                        }
                        break;
        }
}

static void on_join_room(struct nexec *p, struct smart_join_room_output *out)
{
        struct smartfox_client *sci = sfs_client_shared();
        struct smart_extension_in_out *ex;
        struct exec_data *d = (struct exec_data *)p->custom_data;
        if(strcmp(d->room_name->ptr, out->room_name->ptr) == 0) {
                d->sfs_listener.room = out->room_id;

                ex = smart_extension_in_out_alloc();
                ex->send_room = 1;
                ex->room_id = d->sfs_listener.room;
                string_cat(ex->cmd, qlkey("slotmachine.slotmachine_get_table_info"));

                smartfox_client_send(sci, &ex->head);
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

static void on_user_info(struct nexec *p, struct smart_extension_in_out *out)
{
        struct smartfox_client *sci = sfs_client_shared();

        struct smart_extension_in_out *ex = smart_extension_in_out_alloc();
        string_cat(ex->cmd, qlkey("lobby.play_slotmachine"));
        smartfox_client_send(sci, &ex->head);
}

static void on_play_slot_machine(struct nexec *p, struct smart_extension_in_out *out)
{
        struct exec_data *d = (struct exec_data *)p->custom_data;
        struct string *stmp;
        struct smartfox_client *sci = sfs_client_shared();

        struct smart_join_room_input *jri = smart_join_room_input_alloc();
        jri->join_type = SMART_JOIN_ROOM_AND_STAY;
        jri->room_to_leave = -1;
        stmp = sobj_get_str(out->object, qlkey("room_name"), RPL_TYPE);
        string_cat(jri->room_name, qskey(stmp));
        /*
         * store room_name for future checking
         */
        string_cat(d->room_name, qskey(stmp));

        smartfox_client_send(sci, &jri->head);
}

static void on_slotmachine_get_table_info(struct nexec *p, struct smart_extension_in_out *out)
{

}

static void on_linked(struct nexec *p)
{
        struct exec_data *d = (struct exec_data *)p->custom_data;

        struct smartfox_client *cli = sfs_client_shared();
        smartfox_client_add_listener(cli, &d->sfs_listener);

        // /*
        //  * quick login
        //  */
        // struct smart_connect_input *sci = smart_connect_input_alloc();
        // string_cat(sci->host, qlkey("117.0.33.17"));
        // sci->port = 8889;
        // smartfox_client_send(cli, &sci->head);
        // game_set_state(GAME_CONNECTING_TO_LOGIN);
}

static void on_removed(struct nexec *p)
{

}
