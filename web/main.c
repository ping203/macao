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
#include <emscripten.h>
#include <emscripten/html5.h>
#include <cherry/stdio.h>
#include <cherry/list.h>
#include <cherry/map.h>
#include <cherry/string.h>
#include <cherry/math/math.h>
#include <native_ui/view.h>
#include <native_ui/parser.h>
#include <native_ui/manager.h>
#include <native_ui/view_controller.h>
#include <smartfox/data.h>
#include <macao/exec.h>

static struct nview *root = NULL;

EMSCRIPTEN_KEEPALIVE
void macao_resize(int width, int height)
{
        nview_set_size(root, (union vec2){(float)width, (float)height});
        nview_set_position(root, (union vec2){root->size.width / 2, root->size.height/2});
        nview_update_layout(root);
}

static void do_frame()
{
        nmanager_update(nmanager_shared(), 1.0f / 60);
}

int main(int argc, char **argv)
{
        nexec_set_fnf(mc_nexec_alloc);
        root = nview_alloc();
        nview_set_layout_type(root, NATIVE_UI_LAYOUT_RELATIVE);
        nview_set_user_interaction_enabled(root, 1);

        struct nparser *parser = nparser_alloc();
        nparser_parse_file(parser, "res/layout/root.xml", NULL);

        struct nview *view = (struct nview *)
                ((char *)parser->view.next - offsetof(struct nview, parser));

        nview_add_child(root, view);

        EM_ASM_({
                var obj =  __shared_object_get($0);
                var rt = document.getElementById("root");
                rt.appendChild(obj);
                _macao_resize(rt.offsetWidth, rt.offsetHeight);
        }, root->ptr);

        EM_ASM(
                var fs = require('fs');
              FS.mkdir('/work');
              FS.mount(NODEFS, {root : '.'}, '/work');
        );

        //emscripten_wget("data.txt", "/work/data.txt");

        struct string *f = file_read_string("/work/data.txt", FILE_INNER);

        debug("%s\n", f->ptr);

        string_free(f);

        emscripten_set_main_loop(do_frame, 60, 1);
        return 0;
}
