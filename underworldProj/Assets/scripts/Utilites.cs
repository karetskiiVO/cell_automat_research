using System;
using UnityEngine;

public static class Utilites {
    public static class rand  {
        private static readonly System.Random rndGen = new System.Random();
        
        public static int getInt () {
            return rndGen.Next();
        }
        
        // returns integer in diapazone [frpm...to)
        public static int getInt (int from, int to) {
            if (to <= from) {
                // exception
            }

            return from + getUInt() % (to - from);
        }
        
        public static int getUInt () {
            return Math.Abs(rndGen.Next());
        }

        public static bool getProbabilityBool (float probability) {
            const int BigConst = 1000000;
            return  getUInt() % BigConst <= probability * (float)BigConst;
        }
        
        public static float normalDistribution (float center, float radius) {
            var point = Vector2.zero;
            while (point.magnitude == 0 || point.magnitude >= 1) {
                point = UnityEngine.Random.insideUnitCircle;
            }

            return center + radius * point.x * Mathf.Sqrt(-4 * Mathf.Log(point.magnitude)) / point.magnitude;
        }
    }
}