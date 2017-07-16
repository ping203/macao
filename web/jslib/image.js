function encode (input) {
    var keyStr = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/=";
    var output = "";
    var chr1, chr2, chr3, enc1, enc2, enc3, enc4;
    var i = 0;

    while (i < input.length) {
        chr1 = input[i++];
        chr2 = i < input.length ? input[i++] : Number.NaN; // Not sure if the index
        chr3 = i < input.length ? input[i++] : Number.NaN; // checks are needed here

        enc1 = chr1 >> 2;
        enc2 = ((chr1 & 3) << 4) | (chr2 >> 4);
        enc3 = ((chr2 & 15) << 2) | (chr3 >> 6);
        enc4 = chr3 & 63;

        if (isNaN(chr2)) {
            enc3 = enc4 = 64;
        } else if (isNaN(chr3)) {
            enc4 = 64;
        }
        output += keyStr.charAt(enc1) + keyStr.charAt(enc2) +
                  keyStr.charAt(enc3) + keyStr.charAt(enc4);
    }
    return output;
}

function isRetinaDisplay() {
        if (window.matchMedia) {
            var mq = window.matchMedia("only screen and (min--moz-device-pixel-ratio: 1.3), only screen and (-o-min-device-pixel-ratio: 2.6/2), only screen and (-webkit-min-device-pixel-ratio: 1.3), only screen  and (min-device-pixel-ratio: 1.3), only screen and (min-resolution: 1.3dppx)");
            return (mq && mq.matches || (window.devicePixelRatio > 1));
        }
        return false;
}

function native_image()
{
        this.alloc();
}

native_image.prototype.alloc = function()
{
        __register_shared_object(this);
        this.image = new Image();
        this.width = 0;
        this.height = 0;
}

native_image.prototype.set_native_ptr = function(p)
{
        this.native_ptr = p;
}

native_image.prototype.free = function()
{
        this.image = null;
        __remove_shared_object(this);
}

native_image.prototype.load = function(f)
{
        var str = FS.readFile(f);
        var data = 'data:image/png;base64,'+encode(str);
        var self = this;
        this.image.onload = function() {
                console.log("ok");
                self.width = self.image.width;
                self.height = self.image.height;
        };
        this.image.src = data;
}

native_image.prototype.get_width = function()
{
        return this.width;
}

native_image.prototype.get_height = function()
{
        return this.height;
}

native_image.prototype.load_texture = function()
{
        var canvas = document.getElementById("canvas");
        gl = canvas.getContext("experimental-webgl");

        gl.texImage2D(gl.TEXTURE_2D, 0, gl.RGBA, gl.RGBA, gl.UNSIGNED_BYTE, this.image);
}
