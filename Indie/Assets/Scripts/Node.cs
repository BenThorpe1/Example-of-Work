using System.Collections;
using System.Collections.Generic;
using UnityEngine;

/// <summary>
/// Reperents a node for pathing
/// </summary>
public class Node
{
    //Tile of the node
    private Tile tile;
    //Cost to travel to node
    private float cost;
    //The prevous node, that is part of the cheapest path
    private Node prevousNode;

    //Position in array
    private int xPos;
    private int zPos;

    //Max value for cost, set as defulat
    private const float MAX_VALUE = float.MaxValue;

    //Series of one line getters of properties
    #region Getters
    public float GetCost() { return cost; }
    public Tile GetTile() { return tile; }
    public TILE_TYPE GetID() { return tile.GetID(); }
    public int GetX() { return xPos; }
    public int GetZ() { return zPos; }
    public Node GetPervious() { return prevousNode; }
    public Vector3 GetPos() { return new Vector3(xPos, 0, zPos); }
    #endregion

    /// <summary>
    /// Set nodes with paramaters
    /// </summary>
    /// <param name="a_tile"></param>
    /// <param name="a_x"></param>
    /// <param name="a_z"></param>
    public Node(Tile a_tile, int a_x, int a_z)
    {
        tile = a_tile;
        xPos = a_x;
        zPos = a_z;
        ResetNode();
    }

    /// <summary>
    /// Set cost of node if cheaper
    /// return true, if cost is cheaper
    /// </summary>
    /// <param name="a_cost"></param>
    /// <param name="a_prevous"></param>
    public bool SetCost(float a_cost, Node a_prevous)
    {
        //Check if new path is cheaper then current
        if (a_cost < cost)
        {
            cost = a_cost;
            prevousNode = a_prevous;
            return true;
        }
        return false;
    }

    /// <summary>
    /// Reset nodes values for pathing
    /// </summary>
    public void ResetNode()
    {
        prevousNode = null;
        cost = MAX_VALUE;
    }
}
