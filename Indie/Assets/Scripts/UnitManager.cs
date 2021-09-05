using System.Collections;
using System.Collections.Generic;
using UnityEngine;

/// <summary>
/// Controls all units
/// </summary>
public class UnitManager : MonoBehaviour
{

    [SerializeField]
    private GridMangaer gridMangaer;
    [SerializeField]
    private UiManager uiManager;
    [SerializeField]
    private Controlller playerController;
    [SerializeField]
    private AICOntroller aIController;
    //Colour for unit
    [SerializeField]
    private Material playerColour;
    [SerializeField]
    private Material enemeyColour;
    [SerializeField]
    private Material waitedColour;

    List<Unit> playerUnit;
    List<Unit> enemyUnits;
    List<Unit> obstacles;
    private bool playerTurn;

    /// <summary>
    /// Use this for initialization
    /// </summary>
    void Start()
    {
        //Initilize varaibles
        playerTurn = true;
        playerUnit = new List<Unit>();
        enemyUnits = new List<Unit>();
        obstacles = new List<Unit>();

        //Convert child units into units
        Transform[] children = GetComponentsInChildren<Transform>();

        foreach (Transform child in children)
        {
            //Get children returns parent, not required
            if (child == transform || child.GetComponent<Unit>() == null)
            {
                continue;
            }

            //Add unit to apporitate list
            Unit currentUnit = child.GetComponent<Unit>();

            switch (currentUnit.GetAligment())
            {
                case UNIT_ALIGMENT.PLAYER:
                    {
                        playerUnit.Add(currentUnit);
                        currentUnit.GetComponent<MeshRenderer>().material = playerColour;
                        break;
                    }
                case UNIT_ALIGMENT.ENEMY:
                    {
                        enemyUnits.Add(currentUnit);
                        currentUnit.GetComponent<MeshRenderer>().material = enemeyColour;
                        break;
                    }
                case UNIT_ALIGMENT.NEUTRAL:
                    {
                        obstacles.Add(currentUnit);
                        break;
                    }
                default:
                    {
                        Debug.Log("Unit without aligment");
                        break;

                    }
            }
        }
    }

    /// <summary>
    /// Check if unit is on current tile
    /// </summary>
    public Unit CheckTile(int a_x, int a_z, bool playerOnly)
    {
        //Check players
        foreach (Unit currentUnit in playerUnit)
        {
            if (currentUnit.GetX() == a_x && currentUnit.GetZ() == a_z)
            {
                return currentUnit;
            }
        }

        //no players where found
        if (playerOnly)
        {
            return null;
        }

        //Check enemy
        foreach (Unit currentUnit in enemyUnits)
        {
            if (currentUnit.GetX() == a_x && currentUnit.GetZ() == a_z)
            {
                return currentUnit;
            }
        }

        //Check netrual
        foreach (Unit currentUnit in obstacles)
        {
            if (currentUnit.GetX() == a_x && currentUnit.GetZ() == a_z)
            {
                return currentUnit;
            }
        }

        return null;
    }

    /// <summary>
    /// Check if a unit is being hoover overed
    /// </summary>
    public void HooverTile(int a_x, int a_z)
    {
        Unit unit = CheckTile(a_x, a_z, false);

        if (unit != null)
        {
            gridMangaer.HighlightTileUnit(a_x, a_z, unit.GetMovement());
            uiManager.UpdateUnitHoverInfo(unit);
        }
        else
        {
            uiManager.HideUnitHoverInfo();
        }
    }

    /// <summary>
    /// Check if unit is selected to display unit states
    /// </summary>
    /// <param name="a_x"></param>
    /// <param name="a_z"></param>
    /// <returns></returns>
    public bool UnitStats(int a_x, int a_z)
    {
        Unit unit = CheckTile(a_x, a_z, false);
        if (unit != null)
        {
            uiManager.UnitStats(unit);
            return true;
        }
        return false;
    }

    /// <summary>
    /// Check if selected player unit can still move
    /// </summary>
    /// <param name="a_x"></param>
    /// <param name="a_z"></param>
    /// <returns></returns>
    public Unit UnitSelected(int a_x, int a_z)
    {
        Unit unit = CheckTile(a_x, a_z, true);

        if (unit == null)
        {
            return null;
        }

        //Found unit, check if unit can still act or move
        if (unit.GetMoved() == false || unit.GetActed() == false)
        {
            return unit;
        }
        return null;
    }

    //Unit has waited
    /// <summary>
    /// Unit has waited, check for turn change
    /// </summary>
    /// <param name="a_unit"></param>
    public void Wait(Unit a_unit)
    {
        a_unit.gameObject.GetComponent<MeshRenderer>().material = waitedColour;

        //Change turn if all units have completed there actions
        bool changeTurn = true;

        if (playerTurn)
        {
            foreach (Unit currentUnit in playerUnit)
            {
                if (!currentUnit.GetActed())
                {
                    changeTurn = false;
                }
            }
        }
        else
        {
            foreach (Unit currentUnit in enemyUnits)
            {
                if (!currentUnit.GetActed())
                {
                    changeTurn = false;
                }
            }
        }

        if (changeTurn)
        {
            ChangeTurn();
        }
    }

    /// <summary>
    /// Change turn
    /// </summary>
    private void ChangeTurn()
    {
        //Reset units states
        foreach (Unit currentUnit in playerUnit)
        {
            currentUnit.RefreshAction();
        }
        foreach (Unit currentUnit in enemyUnits)
        {
            currentUnit.RefreshAction();
        }

        //Change turn
        playerTurn = !playerTurn;
        if (playerTurn)
        {
            playerController.StartTurn();
        }
        else
        {
            playerController.GivePlayerControl(false);
            aIController.StartTurn();
        }
    }

    /// <summary>
    /// Calulcate unit damage
    /// </summary>
    /// <param name="a_attacker"></param>
    /// <param name="a_other"></param>
    public void TakeDamage(Unit a_attacker, Unit a_other)
    {
        //Calucate hit rate
        float hitRate = CalulacteHitChance(a_attacker, a_other);

        //Hit
        if (CheckRandomChance(hitRate))
        {
            a_other.TakeDamage(CalulcateDamage(a_attacker, a_other));
            uiManager.UpdateUnitHoverInfo(a_other);
        }
        //Miss
        else
        {
            uiManager.MissDisplay();
        }
    }

    /// <summary>
    /// Check if random change
    /// </summary>
    /// <param name="a_change"></param>
    /// <returns></returns>
    private bool CheckRandomChance(float a_change)
    {
        float ran = Random.Range(0, 100);
        if (a_change >= ran)
        {
            return true;
        }
        else
        {
            return false;
        }
    }

    /// <summary>
    /// Calulcate hit rate
    /// </summary>
    /// <param name="a_attacker"></param>
    /// <param name="a_other"></param>
    /// <returns></returns>
    public float CalulacteHitChance(Unit a_attacker, Unit a_other)
    {
        return a_attacker.GetHIT() - a_other.GetAVD();
    }

    /// <summary>
    /// Calulcate damage dealt
    /// </summary>
    /// <param name="a_attacker"></param>
    /// <param name="a_other"></param>
    /// <returns></returns>
    public float CalulcateDamage(Unit a_attacker, Unit a_other)
    {
        //Phyiscal damage
        if (a_attacker.GetAttackType() == ATTACK_TYPE.PHY)
        {
            return a_attacker.GetATK() - a_other.GetPRT();
        }
        //Magical damage
        else
        {
            return a_attacker.GetMAG() - a_other.GetRSL();
        }
    }

    /// <summary>
    /// End the current turn
    /// </summary>
    public void EndTurn()
    {
        ChangeTurn();
    }

}
