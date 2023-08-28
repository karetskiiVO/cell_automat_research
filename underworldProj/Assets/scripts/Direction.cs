using System;
using UnityEngine;

public class Direction {
    public static readonly Vector3Int[] directions = {
        new Vector3Int(1, 0, 0), new Vector3Int(0, 0, 1),
        new Vector3Int(-1, 0, 0), new Vector3Int(0, 0, -1)
    };
    private int dir;
    
    public Direction (int dir = 0) { this.dir = Math.Abs(dir) % 4; }

    public int getCurentNumber () { return dir;}
    
    public Vector3Int getDir () { return directions[dir]; }
    public Direction getReverse () { return new Direction(dir + 2); }
    public Direction getCopy () { return new Direction(dir); }
    public Direction getLeft (int num = 1) { return new Direction((dir + 3 * num) % 4); }
    public Direction getRight (int num = 1) { return new Direction((dir + 1 * num) % 4); }
    
    
    public void left (int num = 1) { dir = (dir + 3 * num) % 4; }
    public void right (int num = 1) { dir = (dir + 1 * num) % 4; }
    public void reverse () { dir = (dir + 2) % 4; }
}