/*
 * join room type
 */
var SFS_JOIN_ROOM_AND_LOG_OUT_LAST      = 0;
var SFS_JOIN_ROOM_AND_LEAVE             = 1;
var SFS_JOIN_ROOM_AND_STAY              = 2;

/*
 * define smartfox client
 */
function smartfox_client(host, port, zone)
{
        this.alloc(host, port, zone);
}

smartfox_client.prototype.alloc         = function(host, port, zone)
{
        var configSFS           = {};
        configSFS.host          = host;
        configSFS.port          = port;
        configSFS.zone          = zone;
        configSFS.debug         = true;

        this.sfs                = new SmartFox(configSFS);
        this.zone               = zone;
        __register_shared_object(this);

        /*
         * register server delegate
         */
        this.sfs.addEventListener(SFS2X.SFSEvent.CONNECTION, smartfox_client.prototype.on_connection, this);
        this.sfs.addEventListener(SFS2X.SFSEvent.CONNECTION_LOST, smartfox_client.prototype.on_connection_lost, this);
        this.sfs.addEventListener(SFS2X.SFSEvent.DISCONNECT, smartfox_client.prototype.on_disconnect, this);
        this.sfs.addEventListener(SFS2X.SFSEvent.LOGIN, smartfox_client.prototype.on_login, this);
        this.sfs.addEventListener(SFS2X.SFSEvent.LOGIN_ERROR, smartfox_client.prototype.on_login_error, this);
        this.sfs.addEventListener(SFS2X.SFSEvent.LOGOUT, smartfox_client.prototype.on_logout, this);
        this.sfs.addEventListener(SFS2X.SFSEvent.ROOM_JOIN, smartfox_client.prototype.on_join_room, this);
        this.sfs.addEventListener(SFS2X.SFSEvent.ROOM_JOIN_ERROR, smartfox_client.prototype.on_join_room_error, this);
        this.sfs.addEventListener(SFS2X.SFSEvent.EXTENSION_RESPONSE, smartfox_client.prototype.on_extension_response, this);
        this.sfs.addEventListener(SFS2X.SFSEvent.PUBLIC_MESSAGE, smartfox_client.prototype.on_public_message, this);
}

smartfox_client.prototype.set_native    = function(n)
{
        this.__native_ptr__ = n;
}

smartfox_client.prototype.get_native    = function()
{
        return this.__native_ptr__;
}

smartfox_client.prototype.free          = function()
{
        __remove_shared_object(this);
}

/*
 * send data to server
 */

smartfox_client.prototype.connect = function(host, port)
{
        this.sfs.connect(host, port);
}

smartfox_client.prototype.login = function(username, password, zone, params)
{
        this.sfs.send(new SFS2X.Requests.System.LoginRequest(username, password, params, zone));
}

smartfox_client.prototype.logout = function()
{
        this.sfs.send(new SFS2X.Requests.System.LogoutRequest());
}

smartfox_client.prototype.join_room = function(type, name, pass, room_leave)
{
        switch (type) {
                case 0:
                        this.sfs.send(new SFS2X.Requests.System.JoinRoomRequest(name, pass));
                        break;
                case 1:
                        this.sfs.send(new SFS2X.Requests.System.JoinRoomRequest(name, pass, room_leave));
                        break;
                case 2:
                        this.sfs.send(new SFS2X.Requests.System.JoinRoomRequest(name, pass, -1));
                        break;
                default:
                        break;
        }
}

smartfox_client.prototype.disconnect = function()
{
        this.sfs.disconnect();
}

smartfox_client.prototype.public_message = function(message, param, room)
{
        this.sfs.send(new SFS2X.Requests.System.PublicMessageRequest(message, param, room.room));
}

smartfox_client.prototype.send_extension = function(cmd, param)
{
        this.sfs.send(new SFS2X.Requests.System.ExtensionRequest(cmd, param, this.zone));
}

smartfox_client.prototype.send_extension_room = function(cmd, param, room)
{
        this.sfs.send(new SFS2X.Requests.System.ExtensionRequest(cmd, param, room.room));
}

/*
 * delegate response from server
 */
smartfox_client.prototype.on_connection = function(evt)
{
        _smart_native_on_connection(this.__native_ptr__);
}

smartfox_client.prototype.on_connection_lost = function(evt)
{
        _smart_native_on_connection_lost(this.__native_ptr__);
}

smartfox_client.prototype.on_disconnect = function(evt)
{
        _smart_native_on_disconnect(this.__native_ptr__);
}

smartfox_client.prototype.on_login = function(evt)
{
        console.log(evt);
        var str         = null;
        if(evt.data != null && evt.data != undefined) {
                str     = JSON.stringify(evt.data);
        } else {
                str     = "{}";
        }

        var buffer = Module._malloc(str.length + 1);
        Module.stringToUTF8(str, buffer, str.length + 1);

        _smart_native_on_login(this.__native_ptr__, buffer, str.length);

        Module._free(buffer);
        str             = null;

}

smartfox_client.prototype.on_login_error = function(evt)
{
        var str         = evt.errorMessage;
        if(str == null || str == undefined) {
                str = "Lỗi đăng nhập";
        }

        var buffer      = Module._malloc(str.length + 1);
        Module.stringToUTF8(str, buffer, str.length + 1);

        _smart_native_on_login_error(this.__native_ptr__, buffer, str.length);
        Module._free(buffer);
        str             = null;
}

smartfox_client.prototype.on_logout = function(evt)
{
        _smart_native_on_logout(this.__native_ptr__);
}

smartfox_client.prototype.on_join_room = function(evt)
{
        var room        = new smartfox_room(evt.room);

        var buffer      = Module._malloc(room.room.name.length + 1);
        Module.stringToUTF8(room.room.name, buffer, room.room.name.length + 1);

        _smart_native_on_join_room(this.__native_ptr__, room.__soi__, room.room.id, buffer, room.room.name.length);

        Module._free(buffer);
        room            = null;
        buffer          = null;
}

smartfox_client.prototype.on_join_room_error = function(evt)
{
        _smart_native_on_join_room_error(this.__native_ptr__);
}

smartfox_client.prototype.on_extension_response = function(evt)
{
        var cmd         = evt.cmd;
        var params      = JSON.stringify(evt.params);

        var sourceRoom  = evt.sourceRoom;
        if(sourceRoom == null || sourceRoom == undefined) {
                sourceRoom = -1;
        }

        var cmd_buffer      = Module._malloc(cmd.length + 1);
        Module.stringToUTF8(cmd, cmd_buffer, cmd.length + 1);

        var params_buffer      = Module._malloc(params.length + 1);
        Module.stringToUTF8(params, params_buffer, params.length + 1);

        _smart_native_on_extension_response(this.__native_ptr__, sourceRoom, cmd_buffer, cmd.length, params_buffer, params.length);

        Module._free(cmd_buffer);
        Module._free(params_buffer);
        cmd             = null;
        params          = null;
        cmd_buffer      = null;
        params_buffer   = null;
}

smartfox_client.prototype.on_public_message = function(evt)
{
        var message     = evt.message;
        var data        = evt.data;
        var send_name   = "";

        if(message == null || message == undefined) {
                message = "";
        }

        if(data != null && send_name != undefined) {
                send_name = data.send_name;
        }

        if(send_name == null || send_name == undefined) {
                send_name = "";
        }

        var message_buffer      = Module._malloc(message.length + 1);
        Module.stringToUTF8(message, message_buffer, message.length + 1);

        var send_name_buffer      = Module._malloc(send_name.length + 1);
        Module.stringToUTF8(send_name, send_name_buffer, send_name.length + 1);

        _smart_native_on_public_message(this.__native_ptr__, message_buffer, message.length, send_name_buffer, send_name.length);

        Module._free(message_buffer);
        Module._free(send_name_buffer);
        message                 = null;
        data                    = null;
        send_name               = null;
        message_buffer          = null;
        send_name_buffer        = null;
}
