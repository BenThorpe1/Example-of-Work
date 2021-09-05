using UnityEngine;

/// <summary>
/// Data needed for tiles
/// </summary>
[System.Serializable]
[CreateAssetMenu(fileName = "Tile", menuName = "ScriptableObjects/Tile")]
public class TileData : ScriptableObject
{
    [SerializeField]
    private string tileName;
    //Description of the tile and effects
    [SerializeField]
    private string description;
    //Cost to leave tile
    [SerializeField]
    private float cost;
    //Default materail of tile
    [SerializeField]
    private Material material;

    //Prefab to spawn
    [SerializeField]
    private GameObject tilePrefab;

    //Getters
    public string GetName() { return name; }
    public string GetDescription() { return description; }
    public float GetCost() { return cost; }
    public Material GetMaterail() { return material; }
    public GameObject GetTilePrefab() { return tilePrefab; }

}

