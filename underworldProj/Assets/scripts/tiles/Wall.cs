using UnityEngine;

namespace tiles {
    public class Wall : DungeonTile {
        private Direction wallDir;
        private bool inAngel;

        public Wall (Dungeon dungeon) : base(dungeon) {
            wallDir = new Direction(0);
            inAngel = false;
        }
        public override type getType() {
            return type.Wall;
        }
        public override void place (Vector3Int tilePos) {
            var spawnedTile =
                Object.Instantiate(dungeon.locations[dungeon.locationId].WallTils[0], dungeon.transform);
            spawnedTile.transform.position += dungeon.locations[dungeon.locationId].blokSize * (Vector3)tilePos;
            return;
            /*if (!inAngel) {
                var spawnedTile =
                    Object.Instantiate(dungeon.locations[dungeon.locationId].WallTils[0], dungeon.transform);
                spawnedTile.transform.position += dungeon.locations[dungeon.locationId].blokSize * (Vector3)tilePos;
                
                spawnedTile.transform.Rotate(new Vector3(0, 0, 1), -90 * wallDir.getCurentNumber() + 90);
            } else {
                var spawnedTile =
                    Object.Instantiate(dungeon.locations[dungeon.locationId].WallTils[0], dungeon.transform);
                spawnedTile.transform.position += dungeon.locations[dungeon.locationId].blokSize * (Vector3)tilePos;
                
            }*/
        }
        public override void setupAngel () { inAngel = true; }
        public override void setDirection (Direction dir) { wallDir = dir.getCopy(); }
    }
}