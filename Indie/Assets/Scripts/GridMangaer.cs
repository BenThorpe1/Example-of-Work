using System.Collections.Generic;
using UnityEngine;

/// <summary>
/// Managers all tiles,
/// </summary>
public class GridMangaer : MonoBehaviour
{

    //Materail of highlighted tiles
    [SerializeField]
    private Material highlightTileMaterail;
    //Materail of move pathing 
    [SerializeField]
    private Material lineMaterail;
    //List of all tiles and properites
    [SerializeField]
    private TileTypes tileTypes;
    //Size of tiles
    [SerializeField]
    private GridBounds gridBounds;

    //Collection of all tiles
    private Node[,] tiles;

    //List of current highlighted tiles
    private List<Node> highlighTiles;
    //List of current attack range tiles
    private List<Node> attackRange;
    //List of current tiles in moving path
    private List<Node> linePath;


    /// <summary>
    /// Collect all tiles in array
    /// </summary>
    private void Start()
    {
     //   GridSpanwer spawner = GetComponent<GridSpanwer>();
     //   spawner.CreateTiles((int)gridBounds.xMax, (int)gridBounds.zMax);

        //Inilise variabvles
        tiles = new Node[(int)gridBounds.xMax, (int)gridBounds.zMax];
        highlighTiles = new List<Node>();
        linePath = new List<Node>();

        //Convert child tiles into nodes
        Transform[] children = GetComponentsInChildren<Transform>();

        foreach (Transform child in children)
        {
            //Get children returns parent, not required
            if (child == transform || child.GetComponent<Tile>() == null)
            {
                continue;
            }

            //Set tile position in array to match childs, relative position
            Vector2 pos = new Vector2();
            pos.x = child.localPosition.x;
            pos.y = child.localPosition.z;

            //Create new node for tile
            Node newNode = new Node(child.GetComponent<Tile>(), (int)pos.x, (int)pos.y);
            tiles[(int)pos.x, (int)pos.y] = newNode;
        }
    }

    /// <summary>
    /// Calucate all tiles in movement range
    /// </summary>
    /// <returns></returns>
    private List<Node> CalucateMovementRange(Node a_start, float a_range)
    {
        //All tiles in range
        List<Node> tilesInRange = new List<Node>();
        //Newly added tiles, needs to check nieghbours
        List<Node> unCheckTiles = new List<Node>();

        //Set start tile
        a_start.SetCost(0, a_start);
        unCheckTiles.Add(a_start);

        //Chech nieghbours to determine if in range
        while (unCheckTiles.Count != 0)
        {
            Node currentNode = unCheckTiles[0];
            float leaveCost = tileTypes.GetCost((int)currentNode.GetID());

            //Zero indicate non passable tile
            if (leaveCost == 0)
            {
                unCheckTiles.Remove(currentNode);
                continue;
            }

            float travelCost = currentNode.GetCost() + leaveCost;

            //Nieghbours are within range, get all nieghbours
            if (travelCost <= a_range)
            {
                //Add neighbours to un checked tiles
                Node neighbour;

                //Postive X
                int neighbourPos = currentNode.GetX() + 1;
                if (neighbourPos < gridBounds.xMax)
                {
                    neighbour = tiles[neighbourPos, currentNode.GetZ()];
                    //Re-check tile if cost changed
                    if (neighbour.SetCost(travelCost, currentNode))
                    {
                        unCheckTiles.Add(neighbour);
                    }
                }

                //Negative X
                neighbourPos = currentNode.GetX() - 1;
                if (neighbourPos >= gridBounds.xMin)
                {
                    neighbour = tiles[neighbourPos, currentNode.GetZ()];
                    //Re-check tile if cost changed
                    if (neighbour.SetCost(travelCost, currentNode))
                    {
                        unCheckTiles.Add(neighbour);
                    }
                }

                //Postive Z
                neighbourPos = currentNode.GetZ() + 1;
                if (neighbourPos < gridBounds.zMax)
                {
                    neighbour = tiles[currentNode.GetX(), neighbourPos];
                    //Re-check tile if cost changed
                    if (neighbour.SetCost(travelCost, currentNode))
                    {
                        unCheckTiles.Add(neighbour);
                    }
                }

                //Negative Z
                neighbourPos = currentNode.GetZ() - 1;
                if (neighbourPos >= gridBounds.zMin)
                {
                    neighbour = tiles[currentNode.GetX(), neighbourPos];
                    //Re-check tile if cost changed
                    if (neighbour.SetCost(travelCost, currentNode))
                    {
                        unCheckTiles.Add(neighbour);
                    }
                }

            }

            //Adds current tile to checked tiles
            tilesInRange.Add(currentNode);
            unCheckTiles.Remove(currentNode);

            //Highlight tile
            currentNode.GetTile().GetComponent<MeshRenderer>().material = highlightTileMaterail;
        }
        return tilesInRange;
    }

    /// <summary>
    /// Tile with a unit has been highlighted
    /// </summary>
    /// <param name="a_x"></param>
    /// <param name="a_z"></param>
    public void HighlightTileUnit(int a_x, int a_z, float a_range)
    {
        Node selectedNoded = tiles[a_x, a_z];
        highlighTiles = CalucateMovementRange(selectedNoded, a_range);
    }

    /// <summary>
    /// Unhighlight any tiles
    /// </summary>
    public void UnSelectTile()
    {
        //Restore tile materail and reset node cost
        foreach (Node currentNode in highlighTiles)
        {
            //Highlight tile
            currentNode.GetTile().GetComponent<MeshRenderer>().material = tileTypes.GetMaterail((int)currentNode.GetID());

            //Reset pathing for the node
            currentNode.ResetNode();
        }
        highlighTiles.Clear();
    }

    /// <summary>
    /// Get world position of given tile
    /// </summary>
    /// <param name="a_x"></param>
    /// <param name="a_z"></param>
    /// <returns></returns>
    public Vector3 GetWorldPost(int a_x, int a_z)
    {
        Vector3 worldPos = tiles[a_x, a_z].GetTile().transform.position;
        return worldPos;
    }

    /// <summary>
    /// Return the type of choosen tile
    /// </summary>
    /// <param name="a_x"></param>
    /// <param name="a_z"></param>
    /// <returns></returns>
    public TILE_TYPE GetTileType(int a_x, int a_z)
    {
        return tiles[a_x, a_z].GetID();
    }

    /// <summary>
    /// Get the bounds of the grid
    /// </summary>
    /// <returns></returns>
    public GridBounds getBounds()
    {
        return gridBounds;
    }

    /// <summary>
    /// Highlight selected path
    /// </summary>
    /// <param name="a_x"></param>
    /// <param name="a_z"></param>
    public void HighlightPath(int a_x, int a_z)
    {
        //Clear previous path
        Node target = null;
        ClearLinePath();

        //Find target tile
        foreach (Node currentNode in highlighTiles)
        {
            if (currentNode.GetX() == a_x && currentNode.GetZ() == a_z)
            {
                target = currentNode;
                break;
            }
        }
        //Ensure target is within movement range
        if (target == null)
        {
            return;
        }

        bool quit = false;

        linePath.Add(target);
        //Calulacte path
        while (!quit)
        {
            //Path completed
            if (target.GetPervious() == target)
            {
                quit = true;
            }
            //Add text to path, next next tile
            else
            {
                target = target.GetPervious();
                linePath.Add(target);
                target.GetTile().GetComponent<MeshRenderer>().material = lineMaterail;
            }
        }
    }

    /// <summary>
    /// Return current movement path
    /// </summary>
    /// <returns></returns>
    public Vector3[] GetPath()
    {
        //Ensure path exist
        if (linePath == null)
        {
            return null;
        }

        //Convert list to array in revser order
        Vector3[] path = new Vector3[linePath.Count];

        int j = 0;
        for (int i = linePath.Count - 1; i >= 0; i--)
        {
            path[j] = linePath[i].GetPos();
            j++;
        }

        return path;

    }

    /// <summary>
    /// Clear current line path
    /// </summary>
    public void ClearLinePath()
    {
        //Remove all prevous nodes
        foreach (Node currentNode in linePath)
        {
            currentNode.GetTile().GetComponent<MeshRenderer>().material = highlightTileMaterail;
        }
        linePath.Clear();
    }

    /// <summary>
    /// Clear attack range tiles
    /// </summary>
    public void ClearAttackRange()
    {
        //Restore tile materail and reset node cost
        foreach (Node currentNode in attackRange)
        {
            //Highlight tile
            currentNode.GetTile().GetComponent<MeshRenderer>().material = tileTypes.GetMaterail((int)currentNode.GetID());

            //Reset pathing for the node
            currentNode.ResetNode();
        }
        attackRange.Clear();
    }

    /// <summary>
    /// Display attack range of unit
    /// </summary>
    /// <param name="a_x"></param>
    /// <param name="a_z"></param>
    /// <param name="a_range"></param>
    public void AttackRange(int a_x, int a_z, float a_range)
    {
        Node selectedNoded = tiles[a_x, a_z];
        attackRange = CalucateMovementRange(selectedNoded, a_range);
    }
}

/// <summary>
/// Bounds of the grid
/// </summary>
[System.Serializable]
public struct GridBounds
{
    public float xMin;
    public float xMax;
    public float zMin;
    public float zMax;
}
