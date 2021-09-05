using UnityEngine;

/// <summary>
///Repersents a tile
/// </summary>
public class Tile : MonoBehaviour
{
    [SerializeField]
    private TILE_TYPE tileID;
    public TILE_TYPE GetID() { return tileID; }
    public void SetID(TILE_TYPE a_i) { tileID = a_i; }
}

//Type of tiles
public enum TILE_TYPE
{
//No specail funtion, cost 1 to leave
 GROUND =0,
//Unbreakable and unpassable wall
 WALL =1,
 //Grass ground, cost 2 to leave
 GRASS =2,
 //Invalid tile type
 NULL = 3,
 COUNT =4
};

