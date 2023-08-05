using System;
using System.Collections.Generic;
using UnityEngine;

public class Dungeon : MonoBehaviour {
    public Vector3Int size;
    public List<Location> locations;
    public int locationId;
    
    public List<List<DungeonTile>> tileMap;

    void init () {
        tileMap = new List<List<DungeonTile>>(size.x);
        for (var i = 0; i < size.x; i++) {
            tileMap.Add(new List<DungeonTile>(size.z));
        }

        for (var x = 0; x < size.x; x++) {
            for (var y = 0; y < size.z; y++) {
                tileMap[x].Add(new Ground(this));
            }
        }
    }

    private void Start() {
        init();
        transform.position -= 0.5f * (Vector3)size;
        place();
    }

    public void place () {
        for (var x = 0; x < size.x; x++) {
            for (var z = 0; z < size.z; z++) {
                tileMap[x][z].place(new Vector3Int(x, 0, z));
            }
        }
    }
}