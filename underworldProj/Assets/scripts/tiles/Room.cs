using UnityEngine;

namespace tiles {
    public class Room : DungeonTile {
        public Room (Dungeon dungeon) : base(dungeon) { }
        
        public override void setupAngel () { }
        public override void setDirection (Direction dir) { }
        
        public override type getType() {
            return type.Room;
        }
        public override void place(Vector3Int tilePos) { }
    }
}