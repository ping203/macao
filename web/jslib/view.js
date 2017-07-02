var native_type = {
        VIEW            : 0,
        IMAGE           : 1,
        LABEL           : 2,
        TEXTFIELD       : 3,
        TEXTVIEW        : 4,
        LISTVIEW        : 5,
        PAGEVIEW        : 6
}

function view_helper()
{

}

view_helper.prototype.base_alloc = function()
{
        var view = document.createElement('div');
        view.is_native = 1;
        view.style["position"] = "absolute";
        view.style["width"]     = "0px";
        view.style["height"]     = "0px";
        view.native_anchor = {
                x : 0.5,
                y : 0.5
        };
        view.style["transform"] = "perspective(600px)";
        view.style["transform-origin"] = "50% 50%";
        view.native_pos = {
                x : 0,
                y : 0
        };
        view.native_rotation = {
                x : 0,
                y : 0,
                z : 0
        };
        view.native_scale = {
                x : 1,
                y : 1
        };
        view.native_size = {
                width : 0,
                height : 0
        };

        __register_shared_object(view);

        return view;
}

view_helper.prototype.view_alloc = function()
{
        var v = this.base_alloc();
        v.native_type = native_type.VIEW;
        return v;
}

view_helper.prototype.image_alloc = function()
{
        var v = this.base_alloc();
        v.native_type           = native_type.IMAGE;
        v.custom_content        = document.createElement('img');
        v.custom_content.style["position"] = "absolute";
        v.custom_content.style["width"] = "100%";
        v.custom_content.style["height"] = "100%";
        v.custom_content.style["overflow-x"] = "hidden";
        v.custom_content.style["overflow-y"] = "hidden";
        v.custom_content.src = "anhchup.png";
        v.appendChild(v.custom_content);
        this.view_set_clip(v, 1);
        return v;
}

view_helper.prototype.label_alloc = function()
{
        var v = this.base_alloc();
        v.native_type = native_type.LABEL;
        v.innerHTML = "";
        v.style["font-size"] = "12px";
        return v;
}

view_helper.prototype.view_free = function(v)
{
        __remove_shared_object(v);
        v.remove();
}

view_helper.prototype.view_set_font_size = function(v, s)
{
        if(v.native_type == native_type.LABEL) {
                v.style["font-size"] = s + "px";
        }
}

view_helper.prototype.view_set_text_align = function(v, a)
{
        if(v.native_type == native_type.LABEL) {
                switch (a) {
                        case 0:
                                v.style["text-align"] = "left";
                                break;
                        case 1:
                                v.style["text-align"] = "right";
                                break;
                        case 2:
                                v.style["text-align"] = "center";
                                break;
                        case 3:
                                break;
                        default:
                                break;
                }
        }
}

view_helper.prototype.view_set_text_color = function(v, r, g, b, a)
{
        if(v.native_type == native_type.LABEL) {
                v.style["color"] = "rgb(" + (r * 255) + ", " + (g * 255) + ", " + (b * 255) +")";
        }
}

view_helper.prototype.view_set_text = function(v, t)
{
        if(v.native_type == native_type.LABEL) {
                v.innerHTML = t;
                this.view_set_size(v, v.native_size.width, v.native_size.height);
        }
}

view_helper.prototype.request_transform = function(v)
{
        var t = "perspective(600px) "
                + "rotateX(" + (v.native_rotation.x) + "deg) "
                + "rotateY(" + (v.native_rotation.y) + "deg) "
                + "rotateZ(" + (v.native_rotation.z) + "deg) "
                + "scale(" + v.native_scale.x + "," + v.native_scale.y + ") ";
        v.style["transform"] = t;
        v.style["-ms-transform"] = t;
        v.style["-webkit-transform"] = t;
}

view_helper.prototype.view_calculate_size = function(v)
{
        var tmp = document.createElement('div');
        tmp.style["font-size"]          = v.style["font-size"];
        tmp.style["font-family"]        = v.style["font-family"];
        tmp.style["position"]           = v.style["position"];
        tmp.style["width"]              = v.style["width"];
        tmp.style["max-width"]          = v.style["max-width"];
        tmp.style["height"]             = v.style["height"];
        tmp.innerHTML                   = v.innerHTML;
        document.getElementById("root").appendChild(tmp);
        v.native_size.height    = tmp.offsetHeight;
        tmp.remove();
}

view_helper.prototype.view_set_size = function(v, w, h)
{
        v.native_size.width     = w;
        v.native_size.height    = h;
        v.style["max-width"]    = w + "px";
        v.style["width"]        = w + "px";
        if(v.native_type == native_type.LABEL) {
                v.style["height"]    = "auto";
                this.view_calculate_size(v);
        } else {
                v.style["height"]       = h + "px";
        }
        this.view_set_position(v, v.native_pos.x, v.native_pos.y);
}

view_helper.prototype.view_set_position = function(v, x, y)
{
        v.native_pos.x = x;
        v.native_pos.y = y;
        v.style.left = (x - v.native_size.width * v.native_anchor.x) + "px";
        v.style.top = (y - v.native_size.height * v.native_anchor.y) + "px";
}

view_helper.prototype.view_set_rotation = function(v, x, y, z)
{
        v.native_rotation.x = x;
        v.native_rotation.y = y;
        v.native_rotation.z = z;
        this.request_transform(v);
}

view_helper.prototype.view_set_anchor = function(v, x, y)
{
        v.native_anchor.x = x;
        v.native_anchor.y = y;
        v.style["transform-origin"] = (x * 100) + "%" + (y * 100) + "%";
        this.view_set_position(v, v.native_pos.x, v.native_pos.y);
        this.request_transform(v);
}

view_helper.prototype.view_set_scale = function(v, x, y)
{
        v.native_scale.x = x;
        v.native_scale.y = y;
        this.request_transform(v);
}

view_helper.prototype.view_set_color = function(v, r, g, b, a)
{
        if(r >= 0) {
                v.style["background-color"] = "rgb(" + (r * 255) + ", " + (g * 255) + ", " + (b * 255) +")";
        } else {
                v.style["background-color"] = "transparent";
        }
}

view_helper.prototype.view_set_border_color = function(v, r, g, b ,a)
{
        if(r >= 0) {
                v.style["border-color"] = "rgb(" + (r * 255) + ", " + (g * 255) + ", " + (b * 255) +")";
                v.style["border-width"]  = 2 + "pt";
                v.style["border-style"] = "solid";
        } else {
                v.style["border-color"] = "transparent";
                v.style["border-width"]  = 2 + "pt";
                v.style["border-style"] = "none";
        }
}

view_helper.prototype.view_set_border = function(v, b)
{
        v.style["border-radius"] = b + "px";
}

view_helper.prototype.view_set_alpha = function(v, a)
{
        v.style["opacity"] = a + "";
}

view_helper.prototype.view_set_clip = function(v, b)
{
        if(b == 1) {
                v.style["overflow-x"] = "hidden";
                v.style["overflow-y"] = "hidden";
        } else {
                v.style["overflow-x"] = "initial";
                v.style["overflow-y"] = "initial";
        }
}
