using System.Collections.Generic;
using architectors;
using tiles;
using UnityEngine;

public class Dungeon : MonoBehaviour {
    public Vector3Int size;
    public List<Location> locations;
    public int locationId;
    
    public List<List<DungeonTile>> tileMap;

    private void Start() {
        init();
        generate();
        optimize();
        place();
    }
    
    private void optimize () {
        for (var x = 0; x < size.x; x++) {
            for (var z = 0; z < size.z; z++) {
                tileMap[x][z] = tileMap[x][z].optimize(new Vector3Int(x, 0, z));
            }
        }
    }
    
    private void generate() {
        Architector firstArchitector = new HallBuilder(this);
        firstArchitector.spawn(new Vector3Int(size.x / 2, size.y / 2, size.z / 2), new Direction());
        
        var architectors = new List<Architector> { firstArchitector };

        while (step(architectors)) { }
    }

    private static bool step (List<Architector> architectors) {
        var architectorsAccumulator = new List<Architector>();
        foreach (var architector in architectors) {
            var bufList = architector.step();

            if (bufList != null) {
                architectorsAccumulator.AddRange(bufList);
            }
        }

        for (var i = 0; i < architectors.Count; i++) {
            if (!architectors[i].isAlive()) {
                architectors.Remove(architectors[i]);
                i--;
            }
        }
        architectors.AddRange(architectorsAccumulator);

        return architectors.Count != 0;
    }
    
    private void init () {
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
    
    public void place () {
        transform.position += Vector3.up - 0.5f * (Vector3)size * locations[locationId].blokSize;
        
        for (var x = 0; x < size.x; x++) {
            for (var z = 0; z < size.z; z++) {
                tileMap[x][z].place(new Vector3Int(x, 0, z));
            }
        }
    }
}