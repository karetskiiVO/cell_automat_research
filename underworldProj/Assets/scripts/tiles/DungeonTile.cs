using UnityEngine;

public abstract class DungeonTile {
    protected readonly Dungeon dungeon;

    public enum type {
        Room,
        Door,
        Wall,
        Hall,
        Empty,
        Ground,
    }
    
    protected DungeonTile (Dungeon dungeon) {
        this.dungeon = dungeon;
    }
    public abstract void place (Vector3Int tilePos);
    public abstract type getType ();
    public abstract void setupAngel ();
    public abstract void setDirection (Direction dir);
    
    public virtual DungeonTile optimize (Vector3Int pos) {
        return this;
    }
}
