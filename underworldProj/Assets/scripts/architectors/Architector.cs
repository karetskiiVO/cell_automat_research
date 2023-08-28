using System.Collections.Generic;
using architectors;
using UnityEngine;

public abstract class Architector {
    protected readonly Dungeon dungeon;
    protected Vector3Int pos;
    protected Direction dir;
    protected bool alive;

    public const int architectorTypesNum = 2;

    protected Architector (Dungeon dungeon) {
        this.dungeon = dungeon;
        alive = false;
        pos = new Vector3Int(0, 0, 0);
        dir = new Direction();
    }
    public bool isAlive () {
        return alive;
    }

    protected void kill () {
        alive = false;
    }

    public abstract void spawn (Vector3Int pos, Direction dir); 
    public abstract List<Architector> step ();
    
    private Architector architectorFromNumber (int num) {
        switch (num) {
            case 0: return new HallBuilder(dungeon);
            case 1: return new RoomBuilder(dungeon);
            default: return null;
        }
    }

    protected Architector setNewArchitector (Vector3Int pos, Direction dir, float[] probabilities) {
        if (probabilities.Length != architectorTypesNum) {
            return null;
        }

        float sum = 0;
        var bufProb = (float[]) probabilities.Clone();

        for (var i = 0; i < architectorTypesNum; i++) {
            sum += bufProb[i];
        }

        if (sum == 0) {
            for (var i = 0; i < architectorTypesNum; i++) {
                bufProb[i] = 1f / architectorTypesNum;
            }
        }
        else {
            for (var i = 0; i < architectorTypesNum; i++) {
                bufProb[i] /= sum;

                if (i > 0) {
                    bufProb[i] += bufProb[i - 1];
                }
            }
        }
        
        bufProb[architectorTypesNum - 1] = 2f;
        var probSpawn = Random.value;
        for (var i = 0; i < architectorTypesNum; i++) {
            if (!(probSpawn < bufProb[i])) continue;
            
            var res = architectorFromNumber(i);
            res.spawn(pos, dir);
            return res;
        }

        return null;
    }
}
