using System.Collections;
using UnityEngine;

/// <summary>
/// Controllers player interactivity
/// </summary>
public class Controlller : MonoBehaviour
{
    #region Serilaized components    
    [SerializeField]
    private GridMangaer gridMangaer;
    [SerializeField]
    private UnitManager unitManager;
    [SerializeField]
    private UiManager uiManager;
    #endregion

    #region Consts
    //Set position of player, for demo level
    private const int START_POS_X = 1;
    private const int START_POS_Z = 1;
    //Position of player y distance, for camera
    private const float POS_Y = 1f;
    //Button sensitivty
    private const float BUTTON_SEN = 0.3f;
    //Time between repeated movement
    private const float BUTTON_DELAY = 0.1f;
    //The amount the camera rotates by
    private const float ROTATION_STEP = 30f;
    //Movement time tile/second
    private const float MOVEMENT_TIME = 0.2f;
    #endregion

    #region Variables
    //Tile position of player
    private int posX;
    private int posZ;
    //Time inbetween button presses
    private float buttonTimer;
    //Bounds of the player
    [SerializeField]
    private GridBounds gridBounds;
    //Current state of the player
    private PLAYER_STATE state;
    //Current selected unit
    private Unit selectedUnit;
    //Where the player is viewing the battle forcast
    private bool attackPreviw;
    #endregion

    /// <summary>
    /// Used to inilize
    /// </summary>
    private void Start()
    {
        gridBounds = gridMangaer.getBounds();
        StartGame();
    }

    /// <summary>
    /// Update logic
    /// </summary>
    private void Update()
    {
        //Ensure button delay
        buttonTimer += Time.deltaTime;
        if (buttonTimer < BUTTON_DELAY)
        {
            return;
        }

        //State logic
        switch (state)
        {
            //Nothing happens, within player control
            case PLAYER_STATE.NO_CONTROL:
                {
                    break;
                }
            //Player is moving unselected unit
            case PLAYER_STATE.HOVER_MOVEMENT:
                {
                    //Move
                    if (PlayerMovement())
                    {
                        //Update selected tile info
                        gridMangaer.UnSelectTile();
                        HoverTile();
                    }

                    //Check for menu key press, display stats of selected unit
                    if (Input.GetAxis("Menu") > BUTTON_SEN)
                    {
                        if (unitManager.UnitStats(posX, posZ))
                        {
                            state = PLAYER_STATE.STATS;
                            buttonTimer = 0;
                        }
                    }

                    //Check for selection press  
                    if (Input.GetAxis("Selected") > BUTTON_SEN)
                    {
                        //Check for valid player
                        selectedUnit = unitManager.UnitSelected(posX, posZ);
                        if (selectedUnit != null)
                        {
                            //Change state
                            uiManager.SelectUnit(selectedUnit.GetMoved(), selectedUnit.GetActed());
                            state = PLAYER_STATE.SELETED;
                            buttonTimer = 0;
                        }
                    }

                    break;
                }
            //Player is looking at unit stats
            case PLAYER_STATE.STATS:
                {
                    //Cancel removes stats and changes state
                    if (Input.GetAxis("Cancel") > BUTTON_SEN)
                    {
                        buttonTimer = 0;
                        uiManager.HideStats();
                        state = PLAYER_STATE.HOVER_MOVEMENT;
                    }
                    break;
                }
            //Player has selected a controllable unit
            case PLAYER_STATE.SELETED:
                {
                    //Change state back to hover move
                    if (Input.GetAxis("Cancel") > BUTTON_SEN)
                    {
                        buttonTimer = 0;
                        uiManager.ShowAction(false);
                        selectedUnit = null;
                        state = PLAYER_STATE.HOVER_MOVEMENT;
                    }

                    //Logic is perfromed on button click.
                    break;
                }
            //Player is moving unit
            case PLAYER_STATE.MOVING:
                {
                    //Move
                    if (PlayerMovement())
                    {
                        //Highlight move path
                        gridMangaer.HighlightPath(posX, posZ);
                    }

                    //Check for menu key press
                    if (Input.GetAxis("Cancel") > BUTTON_SEN)
                    {
                        //Reset player's position
                        Vector3 pos = selectedUnit.transform.position;
                        pos.y = POS_Y;
                        transform.position = pos;
                        //Return to selected state
                        state = PLAYER_STATE.SELETED;
                        buttonTimer = 0;
                        uiManager.ShowAction(true);
                        gridMangaer.ClearLinePath();
                    }

                    //Check for selection press to confrim move  
                    if (Input.GetAxis("Selected") > BUTTON_SEN)
                    {
                        //Ensure valid path
                        Vector3[] path = gridMangaer.GetPath();
                        if (path.Length > 0)
                        {
                            //Wait for unit to move
                            StartCoroutine(MoveUnit(path));
                            buttonTimer = 0;
                            state = PLAYER_STATE.NO_CONTROL;
                            gridMangaer.ClearLinePath();
                            gridMangaer.UnSelectTile();
                        }
                    }

                    break;
                }
            //Player is attacking with an unit
            case PLAYER_STATE.ATTACKING:
                {
                    //Allow player to move cursor
                    PlayerMovement();

                    //Check for menu key press
                    if (Input.GetAxis("Cancel") > BUTTON_SEN)
                    {
                        if (attackPreviw == false)
                        {
                            state = PLAYER_STATE.SELETED;
                            buttonTimer = 0;
                            uiManager.ShowAction(true);
                            gridMangaer.ClearAttackRange();

                            if (!selectedUnit.GetMoved())
                            {
                                gridMangaer.HighlightTileUnit(selectedUnit.GetX(), selectedUnit.GetZ(), selectedUnit.GetMovement());
                            }
                        }
                        else
                        {
                            buttonTimer = 0;
                            uiManager.DisplayAttackPreview(false);
                            attackPreviw = false;
                        }
                    }

                    //Check for selection press to confrim move  
                    if (Input.GetAxis("Selected") > BUTTON_SEN)
                    {
                        Unit other = unitManager.CheckTile(posX, posZ, false);
                        if (other != null)
                        {
                            if (attackPreviw == false)
                            {
                                attackPreviw = true;
                                //Calucate states
                                float hit = unitManager.CalulacteHitChance(selectedUnit, other);
                                float damage = unitManager.CalulcateDamage(selectedUnit, other);
                                uiManager.AttackPreviewStates(hit, damage);
                            }
                            else
                            {
                                attackPreviw = false;
                                uiManager.DisplayAttackPreview(false);
                                unitManager.TakeDamage(selectedUnit, other);
                                Attack();
                            }
                        }
                        buttonTimer = 0;

                    }


                    break;
                }
            default:
                {
                    break;
                }

        }

    }

    /// <summary>
    /// Move player is keypress
    /// </summary>
    private bool PlayerMovement()
    {
        int verticalMovment = 0;
        int horizontalMovement = 0;

        //Right
        if (Input.GetAxis("Horizontal") > BUTTON_SEN)
        {
            //Bounds check
            if (posX < gridBounds.xMax)
            {
                horizontalMovement = 1;
            }
        }
        //Left
        else if (Input.GetAxis("Horizontal") < -BUTTON_SEN)
        {
            //Bounds check
            if (posX > gridBounds.xMin)
            {
                horizontalMovement = -1;
            }
        }

        //UP
        if (Input.GetAxis("Vertical") > BUTTON_SEN)
        {
            //Bounds check
            if (posZ < gridBounds.zMax)
            {
                verticalMovment = 1;
            }
        }
        //Down
        else if (Input.GetAxis("Vertical") < -BUTTON_SEN)
        {
            //Bounds check
            if (posZ > gridBounds.zMin)
            {
                verticalMovment = -1;
            }
        }

        //Update position if needed
        if (verticalMovment != 0 || horizontalMovement != 0)
        {
            posX += horizontalMovement;
            posZ += verticalMovment;
            SetPosition(posX, posZ);
            buttonTimer = 0;
            return true;
        }
        return false;
    }

    /// <summary>
    /// Hoover over tile, display info
    /// </summary>
    private void HoverTile()
    {
        uiManager.DisplayTileInfo(gridMangaer.GetTileType(posX, posZ));
        unitManager.HooverTile(posX, posZ);
    }

    /// <summary>
    /// Set the camera position
    /// </summary>
    /// <param name="a_x"></param>
    /// <param name="a_z"></param>
    private void SetPosition(int a_x, int a_z)
    {
      Vector3 tilePos = gridMangaer.GetWorldPost(a_x, a_z);
       posX = a_x;
     posZ = a_z;
      //  Set  position
       transform.position = tilePos;
    }

    /// <summary>
    /// Start the game
    /// </summary>
    private void StartGame()
    {
        //Set starting position
        SetPosition(START_POS_X, START_POS_Z);
        GivePlayerControl(true);
        uiManager.DisplayTileInfo(gridMangaer.GetTileType(posX, posZ));
        attackPreviw = false;
        selectedUnit = null;
    }

    /// <summary>
    /// Move selected unit
    /// </summary>
    /// <param name="path"></param>
    /// <returns></returns>
    private IEnumerator MoveUnit(Vector3[] path)
    {
        //Move unit
        bool quit = false;
        //Current path node
        int i = 0;
        //Move unit 1 tile per speed
        while (!quit)
        {
            selectedUnit.MoveUnit(path[i]);
            i++;
            if (i == path.Length)
            {
                quit = true;
            }
            yield return new WaitForSeconds(MOVEMENT_TIME);

        }

        //Unit has moved
        selectedUnit.SetMoved(true);

        //Check if unit can still act
        if (selectedUnit.GetActed() == true)
        {
            Wait();
        }
        //Action
        else
        {
            state = PLAYER_STATE.SELETED;
            uiManager.SelectUnit(selectedUnit.GetMoved(), selectedUnit.GetActed());
        }
        yield break;
    }

    /// <summary>
    /// Attack with unit
    /// </summary>
    private void Attack()
    {
        //Check if unit can still move
        selectedUnit.SetActed(true);
        if (selectedUnit.GetMoved() == true)
        {
            Wait();
        }
        //Action
        else
        {
            state = PLAYER_STATE.SELETED;
            uiManager.SelectUnit(selectedUnit.GetMoved(), selectedUnit.GetActed());
        }

        gridMangaer.ClearAttackRange();
    }

    /// <summary>
    /// Unit has waited
    /// </summary>
    public void Wait()
    {
        state = PLAYER_STATE.HOVER_MOVEMENT;
        selectedUnit.Wait();
        unitManager.Wait(selectedUnit);
        buttonTimer = 0;
        uiManager.ShowAction(false);
    }

    /// <summary>
    /// Button pressed, start move with unit
    /// </summary>
    public void SelctedMove()
    {
        buttonTimer = 0;
        uiManager.ShowAction(false);
        state = PLAYER_STATE.MOVING;
    }

    /// <summary>
    /// Button pressed, change to attacking state 
    /// </summary>
    public void SelectedAttack()
    {
        buttonTimer = 0;
        uiManager.ShowAction(false);
        gridMangaer.UnSelectTile();
        gridMangaer.AttackRange(selectedUnit.GetX(), selectedUnit.GetZ(), selectedUnit.GetAttackRangeMax());
        state = PLAYER_STATE.ATTACKING;
    }

    /// <summary>
    /// Start player turn, giving control
    /// </summary>
    public void StartTurn()
    {
        state = PLAYER_STATE.HOVER_MOVEMENT;
        buttonTimer = 0;
        selectedUnit = null;
    }

    /// <summary>
    /// Active or deactive player's control
    /// </summary>
    /// <param name="a_true"></param>
    public void GivePlayerControl(bool a_true)
    {
        state = PLAYER_STATE.HOVER_MOVEMENT;
    }

    //Various state of player control
    private enum PLAYER_STATE
    {
        //Player has no control
        NO_CONTROL = 0,
        //Player is moving without selected unit
        HOVER_MOVEMENT = 1,
        //Stats of unit is displaying;
        STATS = 2,
        //A controllable unit has been selected
        SELETED = 3,
        //Player is moving with unit
        MOVING = 4,
        //The player is attacking a unit
        ATTACKING = 5,
        COUNT = 6
    }
}