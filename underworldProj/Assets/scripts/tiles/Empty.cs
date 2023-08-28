using UnityEngine;

namespace tiles {
    public class Empty : DungeonTile {
        public Empty (Dungeon dungeon) : base(dungeon) { }
        
        public override void setupAngel () { }
        public override void setDirection (Direction dir) { }
        
        public override type getType() {
            return type.Empty;
        }
        public override void place (Vector3Int tilePos) { }
    }
}