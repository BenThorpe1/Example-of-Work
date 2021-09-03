using System.Collections;
using System.Collections.Generic;
using UnityEngine;

/// <summary>
/// Repserents the conversation tree
/// </summary>
public class ConversationTree : TextAdvanture
{

    // Start is called before the first frame update
    void Awake()
    {
        conversation = TextString();
    }

    private List<Node> TextString()
    {
        List<Node> tree = new List<Node>();
        //Starting Path
        //0
        tree.Add(CreateNode("Welcome to begain press 1", 1));
        //1   
        tree.Add(CreateNode("A group of young adventures finds themself at the edge of a small village \n 1 = Enter Village \n 2 = Walk around ", 2, 12));
        //Village , 2  
        tree.Add(CreateNode("The group haeds into the village, The streets are mostly empty as the night sky darkens. \n 1 = Find a place to sleep \n 2 = Explore the streets", 7, 3));
        //Explore Night Village 3
        tree.Add(CreateNode("As the group strolls though the streets, they notice the buildings are torn and damaged. Laying on the streets is a wounded man. \n 1 = Approach the wounded man. \n 2 = Keep on Walking. \n 3 = Find an inn", 4, 5, 7));
        //Talk to the wounded man 4.
        tree.Add(CreateNode("Scars covers the man as he shivers begging the party to help. \n 1 = Take the man to a nearby inn. \n 2 = Walk away", 6, 5));
        //Further Walking 5
        tree.Add(CreateNode("The party leaves the wounded man and continue the wonder around. The group begains to grow tiered and head towards an inn. \n 1 = Procced to inn", 7));
        //Find inn with inqjured man 6
        tree.Add(CreateNode("The party carries the injured man into a nearby inn.The inn keeper informs the group they are only willing to help if the groups pays extra. \n 1 = Pay for Help. \n 2 = Refuse to help", 8, 9));
        //Find inn 7
        tree.Add(CreateNode("The inn is crowded with battered men, trembling as they endless drink. \n 1 = Sit down \n 2 = Go to sleep", 10, 14));
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
        tree.Add(CreateNode("The crackling sound of fires awaken the group to a gang of raiders. \n 1 = Leave the area \n 2 = Go into the village", 17, 19));
        //Wake up inside 16
        tree.Add(CreateNode("The crackling sound of fires awaken the group to a gang of raiders. The inn still remains relativly safe yet the raiders will soon " +
            "arrrive. \n 1 = Find a place to hide. \n 2 = Prepare to fight the raiders. \n 3 = Look for a way to escape the village. \n 4 = Attempt to rescue villages", 18, 24, 20, 23));
        //Leave outside 17
        tree.Add(CreateNode("The group hastely leave the area abandoning the village. \n 1 = Continue", 27));
        //Hiding 18
        tree.Add(CreateNode("The groups barricaded thier cowarding. After a long wait trembling with fear, the fighting quited down \n 1 = Contiune", 26));
        //Outside 19
        tree.Add(CreateNode("The adventures prepare to head intop the village. Raiders have begain pillaging the village. \n 1 = Fight the raiders. \n 2 = Attempt to rescue villagers", 25, 21));
        //Escape  20
        tree.Add(CreateNode("The groups careful navigate through the village, avoiding all raiders. The group manages to leave the village, yet realise they were the only survivors. \n 1 = Contiune", 27));
        //Rescue failed 21
        tree.Add(CreateNode("The groups attempts to rescue the villagers however they were too late. All that remains are the raiders, and lifeless corpses. \n 1 = Fight the raiders \n 2 = Escape the village", 25, 20));
        //Escape with villagers 22
        tree.Add(CreateNode("The groups careful navigate through the village, avoiding all raiders. The group manages to leave the village, with only few other survivors. \n 1 =Contiune", 27));
        //Rescue 23
        tree.Add(CreateNode("The raiders are brutally killing every person they encounts. The adventures rallys all nearby townfolk into the inn and decides on a plan to save the villagers. \n 1 = Fight the Raiders. \n 2 = Escape the village", 24, 22));
        //Fight with surivors 24
        tree.Add(CreateNode("The adventures gear thier equip and begain fighting the raiders. After a prolonged encounter, the raiders begain retreating. " +
            "The village lays in ruins with only a few surivors remaining. The adventures agreed to escout the remaining villagers to another settlement. \n 1 = Continue", 27));
        //Fight with out Survivors 25
        tree.Add(CreateNode("The adventures gear thier equip and begain fighting the raiders. After a prolonged encounter, the raiders begain retreating. " +
        "The village lays in ruins with all the town folk dead. Feeling a sense of failure the group haeds towards another village.\n 1 = Continue", 27));
        //Detroyed 26
        tree.Add(CreateNode("The village lays in ruins, streets lined with blood. The group looks around for any survivor, unable to find any. \n 1 = Continue", 27));
        //End 27
        tree.Add(CreateNode("This the story ends"));

        return tree;
    }

}
