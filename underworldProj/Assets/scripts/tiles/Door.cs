using UnityEngine;

namespace tiles {
    public class Door : DungeonTile {
        public Door( Dungeon dungeon) : base(dungeon) { }
        
        public override void setupAngel () { }
        public override void setDirection (Direction dir) { }

        public override type getType() {
            return type.Door;
        }
        public override void place (Vector3Int tilePos) {
            var spawnedTile = 
                Object.Instantiate(dungeon.locations[dungeon.locationId].DoorTiles[0], dungeon.transform);
            spawnedTile.transform.position += dungeon.locations[dungeon.locationId].blokSize * (Vector3)tilePos;
        }
    }
}