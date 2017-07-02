function native_helper()
{

}

native_helper.prototype.alloc = function()
{
        this.ep = {
                x : 0,
                y : 0
        };

        this.wp = {
                x : 0,
                y : 0
        };

        this.tch = 0;

        this.ctg = null;
        this.ovl = null;
}

native_helper.prototype.find_view = function(v)
{
        if(v.is_native !== undefined) return v;

        if(v.parentElement !== undefined && v.parentElement != null) return this.find_view(v.parentElement);

        return null;
}

native_helper.prototype.mousedown = function(e)
{
        if(this.ctg == null) {
                this.ctg = this.find_view(e.target);
                if(this.ctg != null) {
                        this.ovl = document.createElement('div');
                        this.ovl.style["position"] = "absolute";
                        this.ovl.style["width"] = "100%";
                        this.ovl.style["height"] = "100%";
                        this.ovl.addEventListener("mousedown", this.mousedown, false);
                        this.ovl.addEventListener("mouseup", this.handleEnd, false);
                        this.ovl.addEventListener("mouseout", this.handleCancel, false);
                        this.ovl.addEventListener("mousemove", this.handleMove, false);
                        document.getElementById("root").appendChild(this.ovl);
                        this.tch = 1;

                        this.ep.x = e.pageX - this.ctg.offsetLeft;
                        this.ep.y = e.pageY - this.ctg.offsetTop;
                        this.wp.x = e.pageX;
                        this.wp.y = e.pageY;

                        this.tch = 1;
                }
        }
}

native_helper.prototype.mousemove = function(e)
{
        if(this.tch == 1 && this.ctg != null) {
                var v = {};
                v.x = this.ep.x + e.pageX - this.wp.x;
                v.y = this.ep.y + e.pageY - this.wp.y;


        }
}

native_helper.prototype.mouseup = function(e)
{
        if(this.tch == 1 && this.ctg != null) {
                var v = {};
                v.x = this.ep.x + e.pageX - this.wp.x;
                v.y = this.ep.y + e.pageY - this.wp.y;


        }
}


native_helper.prototype.mousecancel = function(e)
{
        if(this.tch == 1 && this.ctg != null) {
                var v = {};
                v.x = this.ep.x + e.pageX - this.wp.x;
                v.y = this.ep.y + e.pageY - this.wp.y;


        }
}
