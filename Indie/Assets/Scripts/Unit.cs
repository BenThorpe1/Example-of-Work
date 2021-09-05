using UnityEngine;

/// <summary>
/// Repersent a unit, in game
/// </summary>
public class Unit : MonoBehaviour
{
    [SerializeField]
    private string unitName;
    [SerializeField]
    private UnitStats stats;
    [SerializeField]
    private ClassType classes;
    [SerializeField]
    private Attack[] attacks;

    //Tile position of unit
    private int posX;
    private int posZ;
    //Whether the unit has moved or not
    private bool moved;
    private bool acted;
    //Current selected attack
    private int selectedAttack = 0;

    #region Getters
    #region Info    
    public UNIT_ALIGMENT GetAligment() { return stats.aligment; }
    public CLASS_ID GetClassID() { return stats.classID; }
    public ATTACK_TYPE GetAttackType() { return attacks[selectedAttack].GetAttackType(); }
    public string GetName() { return unitName; }
    public int GetX() { return posX; }
    public int GetZ() { return posZ; }
    public float GetMovement() { return stats.movement; }
    public float GetCurrentHP() { return stats.currentHP; }
    public float GetCurrentMP() { return stats.currentMP; }
    public float GetEXP() { return stats.exp; }
    public float GetLevel() { return stats.level; }
    public bool GetActed() { return acted; }
    public bool GetMoved() { return moved; }
    #endregion
    #region Base Stats
    public float GetMaxHP() { return stats.bases.maxHP; }
    public float GetMaxMP() { return stats.bases.maxMP; }
    public float GetSTR() { return stats.bases.str; }
    public float GetINT() { return stats.bases.intl; }
    public float GetDEX() { return stats.bases.dex; }
    public float GetSPD() { return stats.bases.spd; }
    public float GetDEF() { return stats.bases.def; }
    public float GetRES() { return stats.bases.res; }
    #endregion
    //Returns calaculted states based on base stats and equiment
    #region Caluacted stats

    public float GetATK()
    {
        if (attacks[selectedAttack].GetAttackType() == ATTACK_TYPE.PHY)
        {
            return stats.bases.str + attacks[selectedAttack].GetMight();
        }
        else
        {
            return stats.bases.str;
        }
    }

    public float GetMAG()
    {
        if (attacks[selectedAttack].GetAttackType() == ATTACK_TYPE.MAG)
        {
            return stats.bases.intl + attacks[selectedAttack].GetMight();
        }
        else
        {
            return stats.bases.intl;
        }
    }

    public float GetPRT()
    {
        return stats.bases.def;
    }

    public float GetRSL()
    {
        return stats.bases.res;
    }

    public float GetHIT()
    {
        return stats.bases.dex + attacks[selectedAttack].GetHit();
    }

    public float GetAVD()
    {
        return stats.bases.spd;
    }

    public float GetAttackRangeMax()
    {
        return attacks[selectedAttack].GetRange();
    }
    #endregion
    #endregion

    #region Setters
    public void SetX(int a_x) { posX = a_x; }
    public void SetZ(int a_z) { posZ = a_z; }
    public void SetMoved(bool a_t) { moved = a_t; }
    public void SetActed(bool a_t) { acted = a_t; }
    #endregion

    /// <summary>
    /// Used to inilise 
    /// </summary>
    private void Start()
    {
        //Sets bases
     //   stats.bases = classes.GetClassBase((int)GetClassID());
        stats.currentHP = stats.bases.maxHP;
        stats.currentMP = stats.bases.maxMP;
        posX = (int)transform.localPosition.x;
        posZ = (int)transform.localPosition.z;
    }

    /// <summary>
    /// Reset unit move and acted
    /// </summary>
    public void RefreshAction()
    {
        moved = false;
        acted = false;
    }

    /// <summary>
    /// Unit takes damages, returns if unit dies
    /// </summary>
    /// <param name="a_value"></param>
    /// <returns></returns>
    public bool TakeDamage(float a_value)
    {
        stats.currentHP -= a_value;
        if (stats.currentHP <= 0)
        {
            return true;
        }
        else
        {
            return false;
        }
    }

    /// <summary>
    /// Set unit position
    /// </summary>
    /// <param name="a_pos"></param>
    public void MoveUnit(Vector3 a_pos)
    {
        transform.localPosition = a_pos;
        posX = (int)transform.localPosition.x;
        posZ = (int)transform.localPosition.z;
    }

    /// <summary>
    /// Wait with unit, sets act and move to true
    /// </summary>
    public void Wait()
    {
        acted = true;
        moved = true;
    }
}  

