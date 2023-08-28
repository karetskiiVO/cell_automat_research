using UnityEngine;

namespace tiles {
    public class Hall : DungeonTile {
        public Hall (Dungeon dungeon) : base(dungeon) { }
    
        public override void setupAngel () { }
        public override void setDirection (Direction dir) { }
        
        public override type getType() {
            return type.Hall;
        }
        public override void place (Vector3Int tilePos) { }
    }
}