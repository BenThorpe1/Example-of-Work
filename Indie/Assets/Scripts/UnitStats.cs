
[System.Serializable]
/// <summary>
/// Contains stats for units
/// </summary>
public struct UnitStats
{
    //Who has control
    public UNIT_ALIGMENT aligment;
    //Movement range
    public float movement;
    //HP
    public float currentHP;
    //MP
    public float currentMP;
    //Class
    public CLASS_ID classID;
    //Level
    public float level;
    public float exp;

    //base stats
    public BaseStats bases;
};

[System.Serializable]
//Base stats of units
public struct BaseStats
{
    public float maxHP;
    public float maxMP;
    //Power of physic attacks
    public float str;
    //power of magic attacks
    public float intl;
    //Determines hit rates
    public float dex;
    //Reduces physic attacks
    public float def;
    //Reduces magic attacks
    public float res;
    //Determines turn order
    public float spd;
};

[System.Serializable]
//Who has control over unit
public enum UNIT_ALIGMENT
{
    PLAYER = 0,
    ENEMY = 1,
    //No contorl
    NEUTRAL = 2,
    COUNT = 3
};

