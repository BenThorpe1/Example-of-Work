using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class Morality : TextAdvanture
{
    [SerializeField]
    private TextManager manger;
    private const int GOOD_OPTION = 0;
    private const int BAD_OPTION = 1;

    // Start is called before the first frame update
    void Awake()
    {
        conversation = TextString();
    }

    public override bool  MoveStory(int a_i)
    {
        bool advanced = false;
        Node currentNode = conversation[index];
        if (currentNode.options[a_i] != 0)
        {
            //create a branch
            if (currentNode.branchValue == true)
            {
                float mv = manger.GetMoralityValue();
                //Good option is 2
                if (mv > 0)
                {
                    index = currentNode.options[GOOD_OPTION];
                }
                else
                {
                    index = currentNode.options[BAD_OPTION];
                }

            }
            else
            {
                index = currentNode.options[a_i];
            }
            advanced = true;
            previousMorality = currentNode.morality[a_i];
        }
        return advanced;
    }

    private List<Node> TextString()
    {
        List<Node> tree = new List<Node>();
        //Starting Path
        //0
        tree.Add(CreateNode("Welcome to begain press 1", 1));
        //1   
        tree.Add(CreateNode("A group of young adventures finds themself at the edge of a small village \n 1 = Enter Village \n 2 = Walk around ", 2, 12, 0, 0, 0, -1));
        //Village , 2  
        tree.Add(CreateNode("The group haeds into the village, The streets are mostly empty as the night sky darkens. \n 1 = Find a place to sleep \n 2 = Explore the streets", 7, 3));
        //Explore Night Village 3
        tree.Add(CreateNode("As the group strolls though the streets, they notice the buildings are torn and damaged. Laying on the streets is a wounded man. \n 1 = Approach the wounded man. \n 2 = Keep on Walking. \n 3 = Find an inn", 4, 5, 7, 0, 1, -1));
        //Talk to the wounded man 4.
        tree.Add(CreateNode("Scars covers the man as he shivers begging the party to help. \n 1 = Take the man to a nearby inn. \n 2 = Walk away", 6, 5, 0, 0, 2, -2));
        //Further Walking 5
        tree.Add(CreateNode("The party leaves the wounded man and continue the wonder around. The group begains to grow tiered and head towards an inn. \n 1 = Procced to inn", 7));
        //Find inn with inqjured man 6
        tree.Add(CreateNode("The party carries the injured man into a nearby inn.The inn keeper informs the group they are only willing to help if the groups pays extra. \n 1 = Pay for Help. \n 2 = Refuse to help", 8, 9, 0, 0, 2, -2));
        //Find inn 7
        tree.Add(CreateNode("The inn is crowded with battered men, termbled as they endless drink. \n 1 = Sit down \n 2 = Go to sleep", 10, 14));
        //Inn refuse injured man 8
        tree.Add(CreateNode("The group refuses to pay for help, and the wonded man headed out. \n 1 = Continue", 7));
        //Help injued man 9
        tree.Add(CreateNode("Some workers carried the man into a small room. \n 1 = Continue", 7));
        //Sit in inn 10
        tree.Add(CreateNode("Across the room, the voice of two armored men, talking about recent invasion that have destroyed majority of the village. \n 1 = Talk the the armored men. \n 2 = Go to sleep ", 11, 14));
        //TAlking to armored men 11
        tree.Add(CreateNode("The group hears about how raiders have been pillaging the village and will arrvive tomorrow  \n 1 = Contine", 14));
        //Leave Village 12
        tree.Add(CreateNode("You left the village, as night begains to fall, the group looks for a place to rest \n 1 = Enter Village \n 2 = Camp outside", 2, 13));
        //Sleep outside 13
        tree.Add(CreateNode("Sleep outside \n 1 = Continue", 15));
        //Sleep in inn 14
        tree.Add(CreateNode("The groups spends to night at the inn. \n 1 = Continue", 16));

        //Wake up outside 15
        tree.Add(CreateNode("The crackling sound of fires awaken the group to a gang of raiders. \n 1 = Leave the area \n 2 = Go into the village", 17, 20, 0, 0, -3, 1));

        //Wake up inside 16
        tree.Add(CreateNode("The crackling sound of fires awaken the group to a gang of raiders. The inn still remains relativly safe yet the raiders will soon " +
            "arrrive. \n 1 = Find a place to hide. \n 2 = Prepare to fight the raiders. \n 3 = Look for a way to escape the village. \n 4 = Attempt to rescue villages", 21, 24, 17, 27,-1,2,0,1));

        //Attempt to escape 17
        tree.Add(CreateNode("The adventures fearing the raiders attempt to flee the village. \n 1 = Continue", 18, 19, 0, 0, 0, 0, 0, 0, true));
        //Escape Good 18
        tree.Add(CreateNode("The groups careful navigate through the village, avoiding all raiders. The group manages to leave the village, with only few other survivors. \n 1 =Contiune", 30));
        //Escape Bad 19
        tree.Add(CreateNode("The groups careful navigate through the village, avoiding all raiders. The group manages to leave the village, yet realise they were the only survivors. \n 1 = Contiune", 30));

        //Outside 20
        tree.Add(CreateNode("The adventures prepare to head intop the village. Raiders have begain pillaging the village. \n 1 = Fight the raiders. \n 2 = Attempt to rescue villagers", 24, 27));

        //Hiding 21
        tree.Add(CreateNode("The groups barricaded thier cowarding. After a long wait trembling with fear, the fighting quited down \n 1 = Continue", 22, 23, 0, 0, 0, 0, 0, 0, true));
        //Hiding Good 22
        tree.Add(CreateNode("The village lays in ruin, left homeless the adventures agrees to escout the suvivors to another settlement \n 1 = Continue", 30));
        //Hiding Bad 23
        tree.Add(CreateNode("The village lays in ruins, streets lined with blood. The group looks around for any survivor, unable to find any. \n 1 = Continue", 30));

        //Fight 24
        tree.Add(CreateNode("The adventures gear thier equip and begain fighting the raiders. After a prolonged encounter, the raiders begain retreating. \n 1 = Continue"
            , 25, 26, 0, 0, 0, 0, 0, 0, true));
        //Fight Good 25
        tree.Add(CreateNode("The village lays in ruins with only a few surivors remaining. The adventures agreed to escout the remaining villagers to another settlement." +
            " \n 1 = Continue", 30));
        //Fight Bad 26
        tree.Add(CreateNode("The village lays in ruins with all the town folk dead. Feeling a sense of failure the group haeds towards another village." +
            "\n 1 = Continue", 30));

        //Rescue 27
        tree.Add(CreateNode("The groups attempts to rescue the villagers. \n 1 = Continue", 28, 29, 0, 0, 0, 0, 0, 0, true));
        //Resuce Good 28
        tree.Add(CreateNode("The groups attempts to rescue the villagers, managering to a save a few of the villagers." +
      " The group leaves the village to search for a new home for the surviving villagers. \n 1 = Continue ", 30));
        //Rescue Bad 29
        tree.Add(CreateNode("The groups attempts to rescue the villagers however they were too late." +
            " All that remains are the raiders, and lifeless corpses. The adventures leaves the ruined village with a sense of failure. \n 1 = Continue ", 30));


        //End 30
        tree.Add(CreateNode("This the story ends"));


        return tree;
    }

}
