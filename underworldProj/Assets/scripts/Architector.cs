using System.Collections.Generic;

public abstract class Architector {
    private Dungeon dungeon;

    Architector (Dungeon dungeon) {
        this.dungeon = dungeon;
    }
    
    public abstract List<Architector> step ();
}