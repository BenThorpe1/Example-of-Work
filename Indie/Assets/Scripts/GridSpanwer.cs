using UnityEngine;

/// <summary>
/// Create the grid
/// </summary>
public class GridSpanwer : MonoBehaviour
{
    //List of all tiles and properites
    [SerializeField]
    private TileTypes tileTypes;

    /// <summary>
    /// Create world level
    /// </summary>
    public void CreateTiles(int a_sizeX, int a_sizeZ)
    {
        for (int x = 0; x < a_sizeX; x++)
        {
            for (int z = 0; z < a_sizeZ; z++)
            {
                //Create a tile
                int ran = Random.Range(0, (int)TILE_TYPE.COUNT-1);
                SpawnTile(x, z, (TILE_TYPE)ran);
            }
        }
    }

    /// <summary>
    /// Spawn a tile in
    /// </summary>
    /// <param name="a_x"></param>
    /// <param name="a_z"></param>
    /// <param name="a_type"></param>
    private void SpawnTile(int a_x, int a_z, TILE_TYPE a_type)
    {
        //Get tile gameobject
        GameObject tile = Instantiate(tileTypes.GetPrefab((int)a_type));
        tile.transform.parent = transform;
        Vector3 pos = new Vector3(a_x,0,a_z);
        tile.transform.localPosition = pos;

        //Tile ID
        Tile tileID = tile.AddComponent<Tile>();
        tileID.SetID(a_type);
    }
}
