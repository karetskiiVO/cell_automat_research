using System.Collections.Generic;
using tiles;
using UnityEngine;

namespace architectors {
    public class HallBuilder : Architector {
        public HallBuilder (Dungeon dungeon) : base(dungeon) { }

        public override void spawn (Vector3Int pos, Direction dir) {
            this.pos = pos;
            this.dir = dir;
            alive = true;
        }
    
        public override List<Architector> step () {
            if (!alive) return null;

            var len = Mathf.Max(5, Mathf.RoundToInt(Mathf.Abs(Utilites.rand.normalDistribution(8, 2))));
            var bufPos = pos + dir.getDir() * len;
        
            if (bufPos.x <= 0       || bufPos.z <= 0 ||
                bufPos.x >= dungeon.size.x - 1 || bufPos.z >= dungeon.size.z - 1) {
                kill();
                return null;
            }
        
            for (var i = 0; i < len; i++) {
                pos += dir.getDir();
                switch (dungeon.tileMap[pos.x][pos.z].getType()) {
                    case DungeonTile.type.Ground: break;
                    case DungeonTile.type.Empty: break;
                    case DungeonTile.type.Wall:
                        if (dungeon.tileMap[(pos + dir.getDir()).x][(pos + dir.getDir()).z].getType() == DungeonTile.type.Room)
                            dungeon.tileMap[pos.x][pos.z] = new Door(dungeon);
                        kill();
                        return null;
                    case DungeonTile.type.Room:
                        kill();
                        return null;
                    case DungeonTile.type.Door:
                        kill();
                        return null;
                    case DungeonTile.type.Hall:
                        kill();
                        return null;
                }

                dungeon.tileMap[pos.x][pos.z] = new Hall(dungeon);
            }

            var resList = new List<Architector>();
        
            var bufDir = dir.getReverse();
            dir.left(Utilites.rand.getInt(1, 4) + 2);

            for (var i = 0; i < 3; i++) {
                bufDir.left();
            
                if (bufDir == dir) continue;

                if (Utilites.rand.getProbabilityBool(0.8f)) {
                    resList.Add(setNewArchitector(pos, bufDir.getCopy(), new float[] {4, 40}));
                }
            }
        
            return resList;
        }
    }
}