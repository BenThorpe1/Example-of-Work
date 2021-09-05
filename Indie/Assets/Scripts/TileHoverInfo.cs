using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using UnityEngine.UI;

/// <summary>
/// Controls the display for tile info
/// </summary>
public class TileHoverInfo : MonoBehaviour
{

    //Fields for tile info display
    [SerializeField]
    private Text tileName;
    [SerializeField]
    private Text tileDescription;

    /// <summary>
    /// Update the details 
    /// </summary>
    /// <param name="a_name"></param>
    /// <param name="a_des"></param>
    public void UpdateTile(string a_name, string a_des)
    {
        tileName.text = a_name;
        tileDescription.text = a_des;
    }

    /// <summary>
    /// Toggle display
    /// </summary>
    /// <param name="a_active"></param>
    public void Display(bool a_active)
    {
        gameObject.SetActive(a_active);
    }
}
