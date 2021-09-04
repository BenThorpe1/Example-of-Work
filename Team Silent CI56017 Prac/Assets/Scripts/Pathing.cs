using System.Collections.Generic;
using UnityEngine;

/// <summary>
/// Repsernt an AI pathing
/// Ben Thorpe
/// </summary>
public class Pathing : MonoBehaviour
{
    //Sphre that shows partol points on the level
    private const float SPHERE_SIZE = 1f;

    //The current partol point the AI is on
    private int point = 0;
    //Wheter the AI is moving backwards 
    private bool backwardes = false;
    private List<Vector3> path;

    [SerializeField]
    private PARTOL_SETTING partolSetting;

    /// <summary>
    /// Used to inilize
    /// </summary>
    private void Start()
    {
        path = GetPath();
    }

    /// <summary>
    /// Calucate the path
    /// </summary>
    /// <returns></returns>
    public List<Vector3> GetPath()
    {
        List<Vector3> partolPoint = new List<Vector3>();
        Transform[] childern = GetComponentsInChildren<Transform>();
        foreach (Transform child in childern)
        {
            if (child == transform)
            {
                continue;
            }
            partolPoint.Add(child.position);
        }

        return partolPoint;
    }

    /// <summary>
    /// Draws the path on gizmo
    /// </summary>
    public void DrawPath()
    {
        List<Vector3> partolPoints;
        partolPoints = GetPath();

        int i = 0;
        foreach (Vector3 pos in partolPoints)
        {
            //Draw line from previous
            if (i > 0)
            {
                Gizmos.color = Color.blue;
                Gizmos.DrawLine(partolPoints[i - 1], pos);
            }
            i++;
            Gizmos.color = Color.red;
            //draw force application point
            Gizmos.DrawWireSphere(pos, SPHERE_SIZE);
        }

        if (partolSetting == PARTOL_SETTING.LOOP)
        {
            Gizmos.color = Color.blue;
            Gizmos.DrawLine(partolPoints[partolPoints.Count - 1], partolPoints[0]);
        }
    }

   
    /// <summary>
    /// Determine the next point
    /// </summary>
    /// <returns></returns>
    public Vector3 getNextPoint()
    {
        if (path == null)
        {
            path = GetPath();
        }

        //Reverse the path at the end
        if (backwardes)
        {
            point--;
            if (point < 0)
            {
                backwardes = false;
                point = 0;
            }
        }
        else
        {
            point++;
            
            if (point >= path.Count)
            {
                if (partolSetting == PARTOL_SETTING.BACK)
                {
                    backwardes = true;
                    point = path.Count - 1;
                }
                else
                {
                    point = 0;
                }
            }
        }

        return path[point];
    }

    /// <summary>
    /// Draws the partol line when selected
    /// </summary>
    public void OnDrawGizmosSelected()
    {
        DrawPath();
    }

    /// <summary>
    /// Controls how partol pathing works
    /// </summary>
    [System.Serializable]
    private enum PARTOL_SETTING
    {
        //THe path is one big loop
        LOOP,
        //The AI will turn around
        BACK
    }

}
