using UnityEngine;

/// <summary>
/// Controlls how UI elements work
/// </summary>
public class UiManager : MonoBehaviour
{
    #region Serialized Fields
    //Hover display
    [SerializeField]
    private TileHoverInfo tileHoverInfo;
    //Current tile type selected;
    private TILE_TYPE currentTile;
    //Controller of hover display
    [SerializeField]
    private UnitHoverDisplay unitHoverDisplay;
    //Unit stats controller
    [SerializeField]
    private UnitStatsUI statsUI;
    [SerializeField]
    private ActionUI actionUI;
    [SerializeField]
    private AttackPreviwUI attackPreviwUI;
    //List of all tiles and properites
    [SerializeField]
    private TileTypes tileTypes;
    [SerializeField]
    private Controlller controlller;
    #endregion

    /// <summary>
    /// Used to inilise 
    /// </summary>
    private void Start()
    {
        currentTile = TILE_TYPE.NULL;
    }

    /// <summary>
    /// Display the tile info 
    /// </summary>
    /// <param name="a_type"></param>
    public void DisplayTileInfo(TILE_TYPE a_type)
    {
        //Only update if tile type has changed
        if (a_type == currentTile)
        {
            return;
        }

        currentTile = a_type;
        string tileName = tileTypes.GetName((int)a_type);
        string tileDescription = tileTypes.GetDescription((int)a_type);
        tileHoverInfo.UpdateTile(tileName, tileDescription);
    }

    /// <summary>
    /// Display the hover info for the unit
    /// </summary>
    /// <param name="a_unit"></param>
    public void UpdateUnitHoverInfo(Unit a_unit)
    {
        unitHoverDisplay.DisplayUnit(a_unit.GetName(), a_unit.GetCurrentHP(), a_unit.GetMaxHP(),a_unit.GetAligment(),a_unit.GetCurrentMP(),a_unit.GetMaxMP(),a_unit.GetClassID());
 }

    /// <summary>
    /// Hide unit hover details
    /// </summary>
    public void HideUnitHoverInfo()
    {
        unitHoverDisplay.Display(false);
    }

    /// <summary>
    /// Displays stats of units
    /// </summary>
    /// <param name="a_unit"></param>
    public void UnitStats(Unit a_unit)
    {
        statsUI.DisplayStats(a_unit);
        unitHoverDisplay.Display(false);
        tileHoverInfo.Display(false);
    }

    /// <summary>
    /// Hides states UI
    /// </summary>
    public void HideStats()
    {
        statsUI.Close();
        tileHoverInfo.Display(true);
        unitHoverDisplay.Display(true);
    }

    /// <summary>
    /// Show action options
    /// </summary>
    /// <param name="a_false"></param>
    public void ShowAction(bool a_true)
    {
        actionUI.SetActive(a_true);
    }

    /// <summary>
    /// Display options for selected unit
    /// </summary>
    /// <param name="a_moved"></param>
    /// <param name="a_acted"></param>
    public void SelectUnit(bool a_moved, bool a_acted)
    {
        actionUI.SetActive(true);
        actionUI.SelectUnit(a_moved, a_acted);
    }

    /// <summary>
    /// Called when move button is clicked, infrom the player controller
    /// </summary>
    public void ClickMoved()
    {
        controlller.SelctedMove();
    }

    /// <summary>
    /// Called when attack button is clicked, infrom the player controller
    /// </summary>
    public void ClickedAttack()
    {
        controlller.SelectedAttack();
    }

    /// <summary>
    /// Called when move button is clicked, infrom the player controller
    /// </summary>
    public void ClickedWait()
    {
        controlller.Wait();
    }

    /// <summary>
    /// Display attack preview screen
    /// </summary>
    /// <param name="a_true"></param>
    public void DisplayAttackPreview(bool a_true)
    {
        attackPreviwUI.Display(a_true);
    }

    /// <summary>
    ///  Dispaly attack preview screen with values
    /// </summary>
    /// <param name="a_hit"></param>
    /// <param name="a_damage"></param>
    public void AttackPreviewStates(float a_hit, float a_damage)
    {
        attackPreviwUI.DisplayStats(a_hit, a_damage);
    }

    /// <summary>
    /// Display miss text
    /// </summary>
    public void MissDisplay()
    {
        attackPreviwUI.Miss();
    }
}
