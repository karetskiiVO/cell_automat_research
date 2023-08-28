using UnityEngine;

namespace tiles {
    public class Ground : DungeonTile {
        public Ground (Dungeon dungeon) : base(dungeon) { }
        
        public override void setupAngel () { }
        public override void setDirection (Direction dir) { }
        
        public override type getType() {
            return type.Ground;
        }
        public override void place (Vector3Int tilePos) {
            if (!tryGetHallAround(tilePos)) return;
            
            var spawnedTile = 
                Object.Instantiate(dungeon.locations[dungeon.locationId].GroundTiles[0], dungeon.transform);
            spawnedTile.transform.position += dungeon.locations[dungeon.locationId].blokSize * (Vector3)tilePos;
        }
        
        private bool tryGetHallAround (Vector3Int pos) {
            for (var i = 0; i < Direction.directions.Length; i++) {
                var vecDir = Direction.directions[i];
                if (tryGetHall(vecDir + pos)) {
                    return true;
                }
            }
            
            for (var i = 0; i < Direction.directions.Length; i++) {
                var vecDir = Direction.directions[i] + Direction.directions[(i + 1) % Direction.directions.Length];
                if (tryGetHall(vecDir + pos)) {
                    return true;
                }
            }
            
            return false;
        }
        private bool tryGetHall (Vector3Int pos) {
            return !(pos.x < 0 || pos.x >= dungeon.size.x ||
                    pos.z < 0 || pos.z >= dungeon.size.z) &&
                   dungeon.tileMap[pos.x][pos.z].getType() == type.Hall;
        }
    }
}