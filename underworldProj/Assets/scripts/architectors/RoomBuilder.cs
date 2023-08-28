using System.Collections.Generic;
using tiles;
using UnityEngine;
using UnityEngine.Audio;

public class RoomBuilder : Architector {
    public RoomBuilder (Dungeon dungeon) : base(dungeon) { }

    public override void spawn (Vector3Int pos, Direction dir) {
        this.pos = pos;
        this.dir = dir;
        alive = true;
    }
    
    public override List<Architector> step() {
        var roomSize = new Vector3Int(
            x: Utilites.rand.getInt(3, 5) + Utilites.rand.getInt(3, 5),
            z: Utilites.rand.getInt(3, 5) + Utilites.rand.getInt(3, 5),
            y: 0
        );

        var freeSpace = searchFreeSpace(roomSize);
        int leftL = freeSpace.x, rightL = freeSpace.z;
        var dirLeft  = dir.getLeft();
        var dirRight = dir.getRight();
        
        var leftPos  = pos + dir.getDir() + dirLeft.getDir();
        var rightPos = pos + dir.getDir();
        
        if (rightL == 0 || leftL + rightL < roomSize.x) {
            kill();
            return null;
        }

        var d = (leftL + rightL - roomSize.x > 0) ? (Utilites.rand.getUInt() % (leftL + rightL - roomSize.x)) : 0;
        leftPos  = pos + dir.getDir() + dirLeft.getDir();
        rightPos = pos + dir.getDir();

        if (leftL - d == 0 || roomSize.x + d - leftL == 1) {
            kill();
            return null;
        }
        
        // draw room

        for (var i = 0; i < leftL - d; i++, leftPos += dirLeft.getDir()) {
            var buf = leftPos;

            for (var j = 0; j < roomSize.z; j++, buf += dir.getDir()) {
                if (i == leftL - d - 1 || j == 0 || j == roomSize.z - 1) {
                    dungeon.tileMap[buf.x][buf.z] = new Wall(dungeon);
                } else {
                    dungeon.tileMap[buf.x][buf.z] = new Room(dungeon);
                }
            }
        }
        for (var i = 0; i < roomSize.x + d - leftL; i++, rightPos += dirRight.getDir()) {
            var buf = rightPos;

            for (var j = 0; j < roomSize.z; j++, buf += dir.getDir()) {
                if (i == roomSize.x + d - leftL - 1 || j == 0 || j == roomSize.z - 1) {
                    dungeon.tileMap[buf.x][buf.z] = new Wall(dungeon);
                } else {
                    dungeon.tileMap[buf.x][buf.z] = new Room(dungeon);
                }
            }
        }

        // draw door

        var doorPos = pos + dir.getDir();
        dungeon.tileMap[doorPos.x][doorPos.z] = new Door(dungeon);
        
        // spawn new objects

        var listArch = new List<Architector>();
        
        var aroundDir = dir.getRight();
        var curDoorPos = pos + dir.getDir();
        for (var i = 0; i < 2 * (roomSize.x + roomSize.z - 4); i++) {
            var centerDir = aroundDir.getLeft();
            curDoorPos += aroundDir.getDir();
            
            dungeon.tileMap[curDoorPos.x][curDoorPos.z].setDirection(centerDir);
            if (dungeon.tileMap[(centerDir.getDir() + curDoorPos).x][(centerDir.getDir() + curDoorPos).z].getType()
                == DungeonTile.type.Wall) {
                dungeon.tileMap[curDoorPos.x][curDoorPos.z].setupAngel();
                
                aroundDir.left();
                curDoorPos += aroundDir.getDir();
                continue;
            }

            if (Utilites.rand.getProbabilityBool(0.15f)) {
                listArch.Add(setNewArchitector(curDoorPos, centerDir.getReverse(), new float[] {1, 60}));
                dungeon.tileMap[curDoorPos.x][curDoorPos.z] = new Door(dungeon);
            }
        }

        kill();
        return listArch;
    }

    private Vector3Int searchFreeSpace (Vector3Int roomSize) {
        int leftL = 0, rightL = 0;
        var dirLeft  = dir.getLeft();
        var dirRight = dir.getRight();
        
        var leftPos  = pos + dir.getDir() + dirLeft.getDir();
        var rightPos = pos + dir.getDir();
        
        for (var i = 0; i < roomSize.x - 1; i++, leftPos  += dirLeft.getDir()) {
            var buf = leftPos;
            var isSpaceFree = true;

            for (var j = 0; j < roomSize.z; j++, buf += dir.getDir()) {
                if (buf.x < 0 || buf.z < 0  ||
                    buf.x >= dungeon.size.x || buf.z >= dungeon.size.z) {
                    isSpaceFree = false;
                    break;
                }

                if (dungeon.tileMap[buf.x][buf.z].getType() != DungeonTile.type.Empty &&
                    dungeon.tileMap[buf.x][buf.z].getType() != DungeonTile.type.Ground) {
                    isSpaceFree = false;
                    break;
                }
            }

            if (isSpaceFree) {
                leftL++;
            } else {
                break;
            }
        }
        for (var i = 0; i < roomSize.x;     i++, rightPos += dirRight.getDir()) {
            var buf = rightPos;
            var isSpaceFree = true;

            for (var j = 0; j < roomSize.z; j++, buf += dir.getDir()) {
                if (buf.x < 0 || buf.z < 0  ||
                    buf.x >= dungeon.size.x || 
                    buf.z >= dungeon.size.z) {
                    isSpaceFree = false;
                    break;
                }

                if (dungeon.tileMap[buf.x][buf.z].getType() != DungeonTile.type.Empty &&
                    dungeon.tileMap[buf.x][buf.z].getType() != DungeonTile.type.Ground) {
                    isSpaceFree = false;
                    break;
                }
            }

            if (isSpaceFree) {
                rightL++;
            } else {
                break;
            }
        }
        
        return new Vector3Int(leftL, 0, rightL);
    }
}