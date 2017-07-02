#include <cherry/stdio.h>
#include <emscripten.h>
#include <native_ui/view.h>

static u8 __view_touched = 0;
static u8 __view_touched_move = 0;
static u8 __view_touched_end = 0;

pthread_mutex_t __shared_nview_touch_lock;

EMSCRIPTEN_KEEPALIVE
void js_nview_touch_began(struct nview *v)
{
        if(__view_touched || !v) return;

        __view_touched = 1;

        // nview_touch_began(v, (union vec2){x, y});
}

EMSCRIPTEN_KEEPALIVE
void js_nview_touch_moved(struct nview *v)
{
        // debug("touch %f %f\n", x, y);
        debug("native touch\n");
        if(!__view_touched || !v) return;

        // nview_touch_moved(v, (union vec2){x, y});
}

EMSCRIPTEN_KEEPALIVE
void js_nview_touch_ended(struct nview *v)
{
        if(!__view_touched || !v) return;

        if(__view_touched_end == 0) {
                __view_touched_end = 1;
                // nview_touch_ended(v, (union vec2){x, y});
        }
        __view_touched_end = 0;
        __view_touched = 0;
}

EMSCRIPTEN_KEEPALIVE
void js_nview_touch_cancelled(struct nview *v)
{
        if(!__view_touched || !v) return;

        if(__view_touched_end == 0) {
                __view_touched_end = 1;
                // nview_touch_cancelled(v, (union vec2){x, y});
        }
        __view_touched_end = 0;
        __view_touched = 0;
}

EMSCRIPTEN_KEEPALIVE
void test_native_code()
{
        native_ui_test();
}

static void do_frame()
{

}

int main(int argc, char **argv)
{
        debug("Macao Web\n");
        emscripten_set_main_loop(do_frame, 0, 1);
        return 0;
}
