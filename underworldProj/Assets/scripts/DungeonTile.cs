using UnityEngine;

public abstract class DungeonTile {
    protected Dungeon dungeon;
    public abstract void place (Vector3Int tilePos);
}

public class Empty : DungeonTile {
    public Empty (Dungeon dungeon) {
        this.dungeon = dungeon;
    }
    
    public override void place (Vector3Int tilePos) { }
}

public class Ground : DungeonTile {
    public Ground (Dungeon dungeon) {
        this.dungeon = dungeon;
    }
    
    public override void place (Vector3Int tilePos) {
        var spawnedTile = 
            Object.Instantiate(dungeon.locations[dungeon.locationId].EmptyTiles[0], dungeon.transform);
        spawnedTile.transform.position += tilePos;
    }
}


public class Hall : DungeonTile {
    public Hall (Dungeon dungeon) {
        this.dungeon = dungeon;
    }
    
    public override void place (Vector3Int tilePos) { }
}