function smartfox_room(room)
{
        this.alloc(room);
}

smartfox_room.prototype.alloc = function(room)
{
        this.room = room;
        __register_shared_object(this);
}

smartfox_room.prototype.free = function()
{
        this.room = null;
        __remove_shared_object(this);
}
