using UnityEngine;

/// <summary>
/// Contians collection of all tiles
/// </summary>
[CreateAssetMenu(fileName = "TileList", menuName = "ScriptableObjects/TileList")]
public class TileTypes : ScriptableObject
{
    [SerializeField]
    private TileData[] tileData;

    //Getters
    public string GetName(int a_i) { return tileData[a_i].GetName(); }
    public string GetDescription(int a_i) { return tileData[a_i].GetDescription(); }
    public float GetCost(int a_i) { return tileData[a_i].GetCost(); }
    public Material GetMaterail(int a_i) { return tileData[a_i].GetMaterail(); }
    public GameObject GetPrefab(int a_i) { Debug.Log(a_i + " " + tileData.Length); return tileData[a_i].GetTilePrefab(); }
}

