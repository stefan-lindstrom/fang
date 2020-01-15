/*************************************************************************
*   File: constants.c                                   Part of CircleMUD *
*  Usage: Numeric and string contants used by the MUD                     *
*                                                                         *
*  All rights reserved.  See license.doc for complete information.        *
*                                                                         *
*  Copyright (C) 1993, 94 by the Trustees of the Johns Hopkins University *
*  CircleMUD is based on DikuMUD, Copyright (C) 1990, 1991.               *
************************************************************************ */

#include "conf.h"
#include "sysdep.h"

#include "constants.h"
#include "structs.h"

const char circlemud_version[] = {
"Based on CircleMUD, version 3.00 beta patchlevel 11\r\n"
};


/* strings corresponding to ordinals/bitvectors in structs.h ***********/


/* (Note: strings for class definitions in class.c instead of here) */

const char *extended_syslogs[] =
{
    "CLONE",
    "SET",
    "PLAYERLEVELS",
    "PLAYERDEATHS",
    "CONNECTIONS",
    "OLC",
    "SNOOP",
    "ZONERESETS",
    "ZONEERRORS",
    "NOHELP",
    "DGSERRORS",
    "TOOLCRAFT",
    "LOAD",
    "NOPOLICY",
    "GUILDIESET",
    "GUILDSET",
    "SKILLSET",
    "REPORT",
    "AWARD",
    "REVIVE",
    "COMM",
    "GOD",
    "RETAINER",
    "DEBUG",        
    "ERROR",       
    "GUILDING",
    "\n"
};

const struct mirroring_data mirrored_rooms[] = {
 /* from, to */
   { 32650, 32651 },
   { 32652, 32653 },
   { 32654, 32655 },
   { 32656, 32657 },
   { 32658, 32659 },
   { -1, -1 }         /* this must be last! */
};

/* cardinal directions */
const char *dirs[] =
{
  "north",
  "east",
  "south",
  "west",
  "up",
  "down",
  "northeast",
  "southeast",
  "southwest",
  "northwest",
  "\n"
};


/* ROOM_x */
const char *room_bits[] = {
  "DARK",
  "!GATE",
  "!MOB",
  "INDOORS",
  "PEACEFUL",
  "SOUNDPROOF",
  "!TRACK",
  "!MAGIC",
  "TUNNEL",
  "PRIVATE",
  "GODROOM",
  "VERY_DARK",
  "GUILD",
  "!RECALL",
  "OLC",
  "*",				/* BFS MARK */
  "WAGON",
  "HOUSE",
  "STEDDING",
  "DREAM",
  "NEWBIEGOTO",
  "!SPAM",
  "\n"
};


/* EX_x */
const char *exit_bits[] = {
  "DOOR",
  "CLOSED",
  "LOCKED",
  "PICKPROOF",
  "HIDDEN",
  "\n"
};


/* SECT_ */
const char *sector_types[] = {
  "Inside",
  "City",
  "Field",
  "Forest",
  "Hills",
  "Mountains",
  "Water (Swim)",
  "Water (No Swim)",
  "Underwater",
  "In Flight",
  "\n"
};


/* SEX_x */
const char *genders[] =
{
  "Neutral",
  "Male",
  "Female"
};


/* POS_x */
const char *position_types[] = {
  "Dead",
  "Mortally wounded",
  "Incapacitated",
  "Stunned",
  "Sleeping",
  "Resting",
  "Sitting",
  "Fighting",
  "Standing",
  "Riding",
  "\n"
};


/* PLR_x */
const char *player_bits[] = {
  "KILLER",
  "THIEF",
  "FROZEN",
  "DONTSET",
  "WRITING",
  "MAILING",
  "CSH",
  "SITEOK",
  "NOSHOUT",
  "NOTITLE",
  "DELETED",
  "LOADRM",
  "!WIZL",
  "!DEL",
  "INVST",
  "CRYO",
  "NOTELL",
  "NOASK",
  "NOSNOOP",
  "!GECHO",
  "!GAIN",
  "MULTIMAIL",
  "GUILDMAIL",
  "SITTER",
  "HEAD",
  "!MAIL",
  "CUSTOMPROMPT",
  "!GECHO",
  "!POST",
  "MULTI",
  "\n"
};

/* MOB_x (Mob skills) */
const char *mobskill_bits[] = {
  "BASH",
  "TRIP",
  "DIRTKICK",
  "KICK",
  "DISARM",
  "STUN",
  "\n"
};

/* MOB_x */
const char *action_bits[] = {
  "SPEC",
  "SENTINEL",
  "SCAVENGER",
  "ISNPC",
  "AWARE",
  "AGGR",
  "STAY-ZONE",
  "WIMPY",
  "MOUNT",
  "UNUSED",
  "TRACKER",
  "MEMORY",
  "HELPER",
  "!CHARM",
  "!MAGIC",
  "!SLEEP",
  "!BASH",
  "!BLIND",
  "!TRIP",
  "!STUN",
  "HUMAN",
  "SHADOWSPAWN",
  "CHANNELER",
  "!KILL",
  "UNIQUE",
  "\n"
};


/* PRF_x */
const char *preference_bits[] = {
  "BRIEF",
  "COMPACT",
  "DEAF",
  "!TELL",
  "D_HP",
  "D_MANA",
  "D_MOVE",
  "AUTOEX",
  "!HASS",
  "QUEST",
  "!SPAM",
  "!REP",
  "LIGHT",
  "C1",
  "C2",
  "!WIZ",
  "L1",
  "L2",
  "!AUC",
  "!IC",
  "!OOC",
  "RMFLG",
  "AFK",
  "!GDT",
  "GRASP",
  "T_GRASP",
  "!WT",
  "D_EXP",
  "D_GOLD",
  "LOOKAH",
  "!TT",
  "PERCWIMPY",
  "\n"
};

const char *preference2_bits[] =
{
  "SLICE",
  "!HUNGER",
  "!KILLMESSG",
  "!DT",
  "!OT",
  "!QUESTION",
  "!OBSCENE",
  "TIMESTAMPS",
  "AFW",
  "!POSEID",
  "!SONG",
  "!WEATHER",
  "!UNUSED!",
  "!PADD",
  "RPTAG",
  "XLOGTAG",
  "\n"
};

const char *permanent_bits[] =
{
  "GENTLED",
  "BLIND",
  "DEAF",
  "MUTE",
  "CRIPPLED",
  "RH_CUT",
  "LH_CUT",
  "TAINTRES",
  "MANACONVED",
  "\n"
};

/* AFF_x */
const char *affected_bits[] =
{
  "BLIND",		/*  0 */
  "INVIS",
  "UNUSED",
  "SUFFOCATING",
  "DET-INVIS",
  "SENSE-LIFE",
  "WRAPPED",
  "RUNNING",
  "NOTEACH",
  "GROUP",
  "GAGGED",		/* 10 */
  "INFRA",
  "POISON",
  "SHIELDED",
  "LIPREADING",
  "SLEEP",
  "!TRACK",
  "DREAMING",
  "ARMOR",
  "SNEAK",
  "HIDE",		/* 20 */
  "CAMOUFLAGED",
  "CHARM",
  "BANDAGED",
  "VEILED",
  "TAILING",
  "MORALE",
  "STUNNED",
  "!SEARCH",
  "!REINFORCE",
  "MASTERY",		/* 30 */
  "!QUIT",
  "\n"
};

/* AFF2   */
const char *affected2_bits[] =
{
  "TAR-GATE",		/*  0 */
  "!DIRTKICK",
  "CONFUSION",
  "UNBALANCE",
  "CONCENTRATE",
  "STUBBORN",
  "ARRESTED",
  "NOFINDHERB",
  "COMPULSION",
  "SHIELDWALL",		
  "NOPATROL",		/* 10 */
  "BADMORALE",
  "!FLEE",
  "TRACTION",
  "RAGE",
  "DARKDREAM",
  "HALT",
  "DEFENSE",
  "MASKED",
  "ARROGANT",
  "OFFENDED",		/* 20 */
  "DISCOVERED",
  "SUBDUED",
  "LONGSLEEP",
  "NOHERBIDENT",
  "TARBLUR",
  "!BARGAIN",
  "BARGAINING",
  "!CHARM",
  "CHARMING",
  "AWARE",
  "ADRENALINE",
  "\n"
};

/* AFF3 */
const char *affected3_bits[] =
{
  "!KNIFETOSS",		/*  0 */
  "\n"
};

/* RAFF_XXX */
const char *raffected_bits[] = {
  "DARK",
  "LIGHT",
  "EAVESDROPPING",
  "DOME OF AIR",
  "DOME OF FIRE",
  "DOME OF FROST",
  "WARD HUMAN",
  "FOG",
  "COLD",
  "HOT",
  "FORTIFIED",
  "!UNDEF",
  "!UNDEF",
  "!UNDEF",
  "!UNDEF",
  "!UNDEF",
  "!UNDEF",
  "!UNDEF",
  "!UNDEF",
  "!UNDEF",
  "!UNDEF",
  "!UNDEF",
  "!UNDEF",
  "!UNDEF",
  "!UNDEF",
  "!UNDEF",
  "!UNDEF",
  "!UNDEF",
  "!UNDEF",
  "!UNDEF",
  "!UNDEF",
  "!UNDEF",
  "!UNDEF",
  "!UNDEF",
  "\n"
};

/* CON_x */
const char *connected_types[] = {
  "Playing",		/*  0 */
  "Disconnecting",	/*  1 */
  "Get name",
  "Confirm name",
  "Get password",
  "Get new PW",
  "Confirm new PW",
  "Select sex",
  "Select class",
  "Reading MOTD",
  "Main Menu",		/* 10 */
  "Get descript.",
  "Changing PW 1",
  "Changing PW 2",
  "Changing PW 3",
  "Self-Delete 1",
  "Self-Delete 2",
  "Object edit",
  "Room edit",
  "Zone edit",
  "Mobile edit",	/* 20 */
  "Shop edit",
  "Creating",
  "Crafting",
  "Tooling",
  "Quest edit",
  "Gossiping",
  "Horse edit",
  "Info edit",
  "Text edit",
  "Help edit",		/* 30 */
  "Trigger edit",
  "Portal Stone Edit",
  "Action edit",
  "\n"
};


/* WEAR_x - for eq list */
const char *where[] = {
  "<used as light>      ",
  "<worn on finger>     ",
  "<worn on finger>     ",
  "<worn around neck>   ",
  "<worn around neck>   ",
  "<worn on body>       ",
  "<worn on head>       ",
  "<worn on legs>       ",
  "<worn on feet>       ",
  "<worn on hands>      ",
  "<worn on arms>       ",
  "<worn as shield>     ",
  "<worn about body>    ",
  "<worn about waist>   ",
  "<worn around wrist>  ",
  "<worn around wrist>  ",
  "<wielded right hand> ",
  "<wielded left hand>  ",
  "<held>               ",
  "<wielded both hands> ",
  "<worn on ear>        ",
  "<worn on ear>        ", 
  "<worn on nose>       ",
};


/* WEAR_x - for stat */
const char *equipment_types[] = {
  "Used as light",
  "Worn on right finger",
  "Worn on left finger",
  "First worn around neck",
  "Second worn around neck",
  "Worn on body",
  "Worn on head",
  "Worn on legs",
  "Worn on feet",
  "Worn on hands",
  "Worn on arms",
  "Worn as shield",
  "Worn about body",
  "Worn around waist",
  "Worn around right wrist",
  "Worn around left wrist",
  "Wielded in right hand",
  "Wielded in left hand",
  "Held",
  "Wielded in both hands",
  "Worn in right ear",
  "Worn in left ear",
  "Worn in nose", 
  "\n"
};


/* ITEM_x (ordinal object types) */
const char *item_types[] = {
  "UNDEFINED",
  "LIGHT",
  "UNUSED",
  "HERB",
  "HEALING",
  "WEAPON",
  "BOW",
  "ARROW",
  "TREASURE",
  "ARMOR",
  "UNUSED",
  "WORN",
  "OTHER",
  "TRASH",
  "WAGON",
  "CONTAINER",
  "NOTE",
  "LIQ CONTAINER",
  "KEY",
  "FOOD",
  "MONEY",
  "PEN",
  "BOAT",
  "FOUNTAIN",
  "FURNITURE",
  "MOUNT FOOD",
  "NONAME BOARD",
  "DAGGER",
  "\n"
};

const char *furniture_types[] = {
  "Sit",
  "Sleep",
  "Sit and Sleep",
  "put",
  "\n"
};
  
const char *furniture_usages[] = {
  "on",    
  "in",  
  "at",
  "around",   
  "under",
  "above",
  "/n"
};

/* ITEM_WEAR_ (wear bitvector) */
const char *wear_bits[] = {
  "TAKE",
  "FINGER",
  "NECK",
  "BODY",
  "HEAD",
  "LEGS",
  "FEET",
  "HANDS",
  "ARMS",
  "SHIELD",
  "ABOUT",
  "WAIST",
  "WRIST",
  "S_WIELD",
  "HOLD",
  "D_WIELD",
  "EAR",
  "NOSE",
  "\n"
};


/* ITEM_x (extra bits) */
const char *extra_bits[] = {
  "ANGREAL",
  "SA'ANGREAL",
  "TER'ANGREAL",
  "!DONATE",
  "DREAM",
  "INVISIBLE",
  "!TOOL",
  "!DROP",
  "!MALE",
  "!FEMALE",
  "SUNG",
  "!PULL",
  "!BREAK",
  "!SAVE",
  "QUEST",
  "UNUSED15",
  "!SELL",
  "THROW",
  "ENHANCED",
  "KEYWORDS_ADDED",
  "UNIQUE",
  "\n"
};


/* APPLY_x */
const char *apply_types[] = {
  "NONE",
  "STR",
  "DEX",
  "INT",
  "WIS",
  "CON",
  "CHA",
  "UNDEFINED",
  "UNDEFINED",
  "ABS",
  "UNDEFINED",
  "UNDEFINED",
  "MAXMANA",
  "MAXHIT",
  "MAXMOVE",
  "UNDEFINED",
  "UNDEFINED",
  "ARMOR",
  "HITROLL",
  "DAMROLL",
  "XATTACKS",
  "UNDEFINED",
  "UNDEFINED",
  "UNDEFINED",
  "UNDEFINED",
  "\n"
};


/* CONT_x */
const char *container_bits[] = {
  "CLOSEABLE",
  "PICKPROOF",
  "CLOSED",
  "LOCKED",
  "\n",
};


/* LIQ_x */
const char *drinks[] =
{
  "water",
  "beer",
  "wine",
  "ale",
  "dark ale",
  "oosquai",
  "whiskey",
  "lemonade",
  "firebreather",
  "local speciality",
  "slime mold juice",
  "milk",
  "tea",
  "kaf",
  "blood",
  "salt water",
  "clear water",
  "brandy",
  "maple syrup",
  "soapy water",
  "eggnog",
  "\n"
};


/* other constants for liquids ******************************************/


/* one-word alias for each drink */
const char *drinknames[] =
{
  "water",
  "beer",
  "wine",
  "ale",
  "ale",
  "oosquai",
  "whiskey",
  "lemonade",
  "firebreather",
  "local",
  "juice",
  "milk",
  "tea",
  "kaf",
  "blood",
  "salt",
  "water",
  "brandy",
  "syrup",
  "soap",
  "eggnog",
  "\n"
};


/* effect of drinks on hunger, thirst, and drunkenness -- see values.doc */
const int drink_aff[][3] = {
  {0, 1, 10},
  {3, 2, 5},
  {5, 2, 5},
  {2, 2, 5},
  {1, 2, 5},
  {3, 2, 5},
  {6, 1, 4},
  {0, 1, 8},
  {10, 0, 0},
  {3, 3, 3},
  {0, 4, -8},
  {0, 3, 6},
  {0, 1, 6},
  {0, 1, 6},
  {0, 2, -1},
  {0, 1, -2},
  {0, 0, 13},
  {5, 0, 4},
  {0, 5, 5},
  {0, 1, -2},
  {0, 0, 10},
};

/* color of the various drinks */
const char *color_liquid[] =
{
  "clear",
  "brown",
  "clear",
  "brown",
  "dark",
  "darkish red",
  "golden",
  "red",
  "green",
  "clear",
  "light green",
  "white",
  "brown",
  "black",
  "red",
  "clear",
  "crystal clear",
  "golden brown",
  "rich brown",
  "murky",
  "creamy white"
};


/* level of fullness for drink containers */
const char *fullness[] =
{
  "less than half ",
  "about half ",
  "more than half ",
  ""
};

/* Wear status strings */
const char *obj_status[] =
{
  "(broken)",
  "(bad shape)",
  "(very worn)",
  "(worn)",
  "(scratched)",
  "(brand new)"
};

/* Room program strings for redit */

const char *room_mainterms[] =
{
  "If mud time is ",
  "if a new mud hour.",
  "Random event, chance in %% each tick: ",
  "If player enters the room.",
  "If mob with enters the room, and mobs vnum is ",
  "If player leaves the room.",
  "If mob leaves the room, and mobs vnum is ",
  "If mob dies in room, and mobs vnum is ",
  "If player dies in room.",
  "If player performs action in room.",
  "If player in the room says ",
  "If player in the room drops an object with vnum "
};

const char *room_sideterms[] =
{
  "If player is of gender ",
  "If players level is between ",
  "If player is WolfKin.",
  "If player is Ta'varen.",
  "If player belongs to guild ",
  "If player is Darkfriend.",
  "If player is carrying object with vnum ",
  "If player is wearing object with vnum ",
  "If players inventory contains object with vnum ",
  "If player is of class ",
  "If player is hurt.",
  "If player is hungry.",
  "If player is thirsty.",
  "If player is tired.",
  "If player has nobility ",
  "If player is sneaking.",
  "If player is blinded.",
  "If player is grasping.",
  "If player is tgrasping.",
  "If player is sleeping.",
  "If player is invisible.",
  "If player has questflag on.",
  "If player can channel.",
  "If player can't channel.",
  "If player has done quest number ",
  "If player is authorized."
};

const char *room_actions[] =
{
  "Force player to ",
  "Stop players action.",
  "Pause in seconds ",
  "Kill mob (if in room) with vnum ",
  "Kill the player ",
  "Open an exit between rooms (room1, room2, dir room1, dir room2) ",
  "Remove an exit between rooms (room1, room2, dir room1, dir room2) ",
  "Set mode to "
  "Give player object with vnum ",
  "Put in room object with vnum ",
  "Load in room mob with vnum ",
  "Add on players HitPoints with ",
  "Add on players mana with ",
  "Add on players moves with ",
};

/* Obj program strings needed by oedit */
const char *obj_mainterms[] =
{
  "  ",
  "If mud time is ",
  "If a new mud hour.",
  "Random event, chance in % each tick: ",
  "If player enters the room the obj is in.",
  "If player leaves the room the obj is in.",
  "If player takes the object.",
  "If player drops the object.",
  "If player wears the object.",
  "If player removes the object.",
  "If player junks the object.",
  "If player donates the object.",
  "If player wearing or carrying the object dies.",
  "If player steals the object.",
  "If player in same room performs action ",
  "If player in same room at obj performs action ",
  "If player wearing obj performs action ",
  "If player wearing obj at obj performs action ",
  "If player carrying obj performs action ",
  "If player carrying obj at obj performs action ",
  "If player in same room says ",
  "If player wearing obj says ",
  "If player carrying obj says ",
  "*"
};

const char *omt_file[] =
{
  "  ",
  "OnTime",
  "OnTick",
  "OnRandom",
  "OnEntry",
  "OnLeave",
  "OnTake",
  "OnDrop",
  "OnWear",
  "OnRemove",
  "OnJunk",
  "OnDonate",
  "OnDie",
  "OnSteal",
  "OnAction",
  "OnActionTo",
  "OnActionWear",
  "OnActionToWear",
  "OnActionCarry",
  "OnActionToCarry",
  "OnSayRoom",
  "OnSayWear",
  "OnSayCarry"
};

const char *obj_sideterms[] =
{
  "   ",
  "Player is of sex ",
  "Player is between levels ",
  "Player is wolfkin.",
  "Player is a darkfriend.",
  "Player is carrying obj vnum ",
  "Player is wearing obj vnum ",
  "Player is of race ",
  "Player is hurt.",
  "Player is tired.",
  "Player is guilded in guild ",
  "Player is of the nobility.",
  "Player is sneaking.",
  "Player can't see obj.",
  "Player is grasping the One Power.",
  "Player is Ta'veren.",
  "Objects wear status is equal to or more than ",
  "Player is sleeping.",
  "Player has quest flag set.",
  "Player is wearing an item with name ",
  "Player is carrying an item with name ",
  "Object is outside.",
  "Player is a channeler.",
  "Player has completed Autoquest nr ",
  "Player is authorized.",
  "Player is of guildrank ",
  "Players guildrank is less than ",
  "Players guildrank is higher than ",
  "*"
};


const char *ost_file[] =
{
  "   ",
  "Sex",
  "Level",
  "Wolfkin",
  "Darkfriend",
  "Carrying",
  "Wearing",
  "Race",
  "Hurt",
  "Tired",
  "Guild",
  "Nobility",
  "Sneaking",
  "Blind",
  "Grasping",
  "Taveren",
  "Status",
  "Sleeping",
  "Questing",
  "WearingN",
  "CarryingN",
  "Outside",
  "Channeler",
  "Quested",
  "Authorized",
  "OGrankIs",
  "OGrankLess",
  "OGrankMore"
};

const char *obj_actions[] =
{
  "  ",
  "Emote ",
  "Force player to ",
  "Stop players action.",
  "Pause program (in seconds) ",
  "Make the obj go to room vnum ",
  "Destroy object.",
  "Wear object down (in %%) ",
  "Emote to player ",
  "*"
};

const char *obj_act_file[] =
{
  "  ",
  "Emote",
  "Force",
  "Stop",
  "Pause",
  "Goto",
  "Destroy",
  "WearDown",
  "EmoteTo"
};

/* Mob program strings needed by medit */
const char *mainterms[] =
{
  "   ",
  "If mudtime is ",
  "If the mob enters a room.",
  "If the mob leaves a room.",
  "If the mob dies.",
  "If a player enters the room a mob is in.",
  "If a player leaves the room a mob is in.",
  "If a player says ",
  "If a player to the mob says ",
  "If a player performs the action ",
  "If a player at the mob performs the action ",
  "If a player attacks the mob.",
  "If the mob attacks a player.",
  "If a player dies while fighting the mob.",
  "If a player to the mob gives ",
  "If a new mud hour.",
  "At random interval, %% chance each tick: ",
  "If a player tells the mob ",
  "*"
};

const char *mt_file[] =
{
  "   ",
  "OnTime",
  "OnMobEntry",
  "OnMobLeave",
  "OnMobDie",
  "OnEntry",
  "OnLeave",
  "OnSay",
  "OnSayTo",
  "OnAction",
  "OnActionTo",
  "OnAttack",
  "OnMobAttack",
  "OnDie",
  "OnGive",
  "OnTick",
  "OnRandom",
  "OnTell"
};

const char *sideterms[] =
{
  "   ",
  "player is of sex ",
  "player is between levels ",
  "player is a Wolfkin",
  "player is a guilded Darkfriend",
  "player is carrying obj with vnum ",
  "player is wearing obj with vnum",
  "player is of race",
  "player is hurt",
  "player is tired",
  "player is guilded in guild ",
  "player is a nobleperson",
  "player is sneaking",
  "player is blinded",
  "player is grasping the One Power",
  "player is a Ta'veren",
  "mobile is in mode ",
  "player is sleeping",
  "mobile can't see player",
  "player is questing",
  "player is wearing obj with name ",
  "player is carrying obj with name ",
  "player is outdoors",
  "player can channel",
  "player has completed AutoQuest number ",
  "player is authorized",
  "Player has grank ",
  "player has grank less than ",
  "player's grank is higher than ",
  "*"
};

const char *st_file[] =
{
  "   ",
  "Sex",
  "Level",
  "Wolfkin",
  "Darkfriend",
  "Carrying",
  "Wearing",
  "Race",
  "Hurt",
  "Tired",
  "Guild",
  "Nobility",
  "Sneaking",
  "Blind",
  "Grasping",
  "Taveren",
  "Mode",
  "Sleeping",
  "Invisible",
  "Questing",
  "WearingName",
  "CarryingName",
  "Outside",
  "CanChannel",
  "AutoQuested",
  "Authorized",
  "MGrankIs",
  "MGrankLess",
  "MGrankMore"
};

const char *actions[] =
{
  "   ",
  "Move to room with vnum ",
  "Say ",
  "Say to player ",
  "Perform action ",
  "Perform at player action ",
  "Force player to ",
  "Put mobile in mode",
  "Give player item with vnum",
  "Cast spell ",
  "Emote ",
  "Stop players action",
  "Move ",
  "Pause (in seconds) ",
  "Change players doing to ",
  "Purge mobile",
  "Shout ",
  "*"
};

const char *act_file[] =
{
  "   ",
  "MoveTo",
  "Say",
  "SayTo",
  "Action",
  "ActionTo",
  "Force",
  "Mode",
  "Give",
  "Cast",
  "Emote",
  "Stop",
  "Move",
  "Pause",
  "Doing",
  "PurgeMob",
  "Shout"
};

const char *spell_wear_off_msg[] = {
  "RESERVED DB.C",		/* 0 */
  "You feel less protected.",	/* 1 */
  "!Airsword!",
  "!Gate!",
  "You feel a cloak of blindness disolve.",
  "The Air weaves wear off, making you visible again.",	/* 5 */
  "!Call Lightning",
  "!Firesword!",
  "You notice that you can move again.",
  "The obstruction in your throat disappears.",
  "The Shield disappears.",		/* 10 */
  "!Link!",
  "!Gentle!",
  "!Create Water!",
  "!Heal!",		
  "!Heal Gentling!", 			/* 15 */
  "!Airshield!",
  "!Burn!",
  "!Airblow!",
  "!Ice Arrows!",
  "You feel your magic awareness dwindle.", /* 20 */
  "!Meteor!",
  "!Light!",
  "!Earthquake!",
  "!RR!",		
  "!Lightning!",			/* 25 */
  "!Fireball!",
  "!Darkness!",
  "You can see again!",
  "You are once again your own master.",		
  "!Cure Light!",			/* 30 */
  "!Cure Serious!",			
  "!Cure Critical!",
  "You feel less protected.",			
  "You can now wake up again.", 	
  "You feel your keen senses grow duller again.", /* 35 */
  "Your night vision slowly disappears.",
  "!Chill!",
  "!Earthshield!",			
  "!Watercascade!",			
  "!Icestorm!",				/* 40 */
  "!Djinn!",
  "The pain searing through your body suddenly vanishes!",
  "!Fly!",		
  "!Block Gateway!",			
  "You are no longer invisible as your Air flows vanish.", /* 45 */
  "!Sliceweave!",
  "!Whirlwind!",
  "!Delve!",	
  "!Minor Heal!",			
  "!Dome of Air!",			/* 50 */
  "!Ball of Light!",
  "!Disintegrate!",
  "!Ball of Air!",
  "!Open Lock!",			
  "!Call Fire!",			/* 55 */
  "!Shaft of Fire!",
  "!Dome of Fire!",
  "!Devastation!",
  "!Destroy weapon!",
  "You can now breathe normally again, thank the Creator!", /* 60 */
  "At long last, the vicious venom is no longer in your veins.",
  "!Invigorate!",
  "!Storm!",
  "!Dart of Air!",
  "!Call Frost!",
  "!Shaft of Frost!",
  "The wind protecting you slows down and stops.",
  "You can now think clearly.",
  "You can now move normally again.",
  "Your balance is now restored.", /* 70 */
  "!Explosion!",
  "!Dome of Frost!",
  "!Fist of Air!",
  "!Firestorm!",
  "!Refresh!",
  "Your eartharmor dwindles to dust, you are once again unprotected.",
  "!Destroy armor!",
  "!Fire Arrows!",
  "!Earth missile!",
  "!Kinheal!", /* 80 */            
  "Your Shield of Saidin starts to fade, and soon vanishes alltogether.",
  "!Cripple!",
  "!Deaf!",
  "!Heal II!",
  "!Thunderburst!",
  "Your stormshield slowly fades away.",
  "!Group Restore!",
  "!Ward Human!",
  "!Identify!",
  "!Fog!", /* 90 */
  "You are now free to move around again.",
  "!Mend!",
  "!Frostbite!",
  "!Roomseal!",
  "!Targate!",
  "!UNUSED!",
  "!UNUSED!",
  "!UNUSED!",
  "Your Watershield slowly fades away.",
  "!UNUSED!", /* 100 */
  "Your fellow Seafolk leave.",
  "!UNUSED!","!UNUSED!","!UNUSED!","!UNUSED!",
  "!UNUSED!","!UNUSED!","!UNUSED!","!UNUSED!","!UNUSED!", /* 110 */
  "!UNUSED!","!UNUSED!","!UNUSED!","!UNUSED!","!UNUSED!",
  "!UNUSED!","!UNUSED!","!UNUSED!","!UNUSED!","!UNUSED!", /* 120 */
  "!UNUSED!","!UNUSED!","!UNUSED!","!UNUSED!","!UNUSED!",
  "!UNUSED!","!UNUSED!","!UNUSED!","!UNUSED!","!UNUSED!", /* 130 */
  "!UNUSED!","!UNUSED!","!UNUSED!","!UNUSED!","!UNUSED!",
  "!UNUSED!","!UNUSED!","!UNUSED!","!UNUSED!","!UNUSED!", /* 140 */
  "!UNUSED!","!UNUSED!","!UNUSED!","!UNUSED!","!UNUSED!",
  "!UNUSED!","!UNUSED!","!UNUSED!","!UNUSED!","!UNUSED!", /* 150 */
  "!UNUSED!","!UNUSED!","!UNUSED!","!UNUSED!","!UNUSED!",
  "!UNUSED!","!UNUSED!","!UNUSED!","!UNUSED!","!UNUSED!", /* 160 */
  "!UNUSED!","!UNUSED!","!UNUSED!","!UNUSED!","!UNUSED!",
  "!UNUSED!","!UNUSED!","!UNUSED!","!UNUSED!","!UNUSED!", /* 170 */
  "!UNUSED!","!UNUSED!","!UNUSED!","!UNUSED!","!UNUSED!",
  "!UNUSED!","!UNUSED!","!UNUSED!","!UNUSED!","!UNUSED!", /* 180 */
  "!UNUSED!","!UNUSED!","!UNUSED!","!UNUSED!","!UNUSED!",
  "!UNUSED!","!UNUSED!","!UNUSED!","!UNUSED!","!UNUSED!", /* 190 */
  "!UNUSED!","!UNUSED!","!UNUSED!","!UNUSED!","!UNUSED!",
  "!UNUSED!","!UNUSED!","!UNUSED!","!UNUSED!","!UNUSED!", /* 200 */
  "!Backstab!",
  "!Bash!",
  "You are no longer hidden.",
  "!Kick!",
  "!Pick Lock!",   /* 205 */
  "!Punch!",
  "!Rescue!",
  "You stop sneaking.",
  "!Steal!",
  "!Track!", /* 210 */
  "!First Aid!",
  "!Dodge!",
  "!Second Attack!",
  "!Third Attack!",
  "!Fourth Attack!",
  "!Shield Attack!",
  "!Trip!",
  "The effect of the stunning blow wears off.",
  "You can now use Enhance again.",
  "!Roll with blow!",                                  /* 220 */
  "!Endurance!",
  "!March!",
  "!Fast Healing!",
  "!Shoot!",
  "!Diagnose!",
  "!Berserk!",
  "!Tool!",
  "You can now be healed with herblore again...duh! :>",
  "!Herbmaster!",
  "!Fifth Attack!", /* 230 */
  "!Grasp!",
  "!Release!",
  "!Tgrasp!",
  "!Trelease!",
  "Speardance!",
  "!Disarm!",
  "!UNUSED!","!UNUSED!","!UNUSED!","!UNUSED!", /* 240 */
  "Your vision returns as your tears wash away the dirt in your eyes.",
  "!Martial Arts!",
  "!Dream!",
  "!Duck!",
  "!Play!",
  "You can now teach again.",
  "!Portal Stones!",
  "You're no longer hiding your tracks.",
  "!Sliceweave!",
  "!Learn!", /* 250 */
  "!Privacy!",
  "!Dreamlist!",
  "!Dreampull!",
  "!Breakshield!",
  "You feel ready to train another mount again.",
  "!Craft!",
  "!Powertrap!",
  "!Jump!",
  "!Meditation!",
  "!Appraise!", /* 260 */
  "!Enhanced Damage!",
  "You lower your defense.",
  "You no longer mask your channeling ability.",
  "Your fortifications crumbles to dust.",
  "You feel refreshed enough to use your abilities again.",
  "You're no longer trailing.",
  "Phew! You left that unpleasant situation behind.",
  "You have regained your freedom!",
  "!Dreamenter!",
  "!UNUSED!", /* 270 */
  "!UNUSED!","!UNUSED!","!UNUSED!","!UNUSED!","!UNUSED!",
  "&n",
  "Your bretheren leave to find the peace of the sea.",
  "You stop eavesdropping.",
  "!UNUSED!",
  "The energy seems to leave your body.", /* 280 */
  "Your fingers feel nimble once more.",
  "The world around you seems to dull.",
  "!UNUSED!",
  "&n",
  "You no longer seem so confused.",
  "!UNUSED!","!UNUSED!","!UNUSED!","!UNUSED!",
  "!Fine!", /* 290 */
  "!Throw!",
  "Your reinforcements leave, and you can now raise the armscry again.",
  "!UNUSED!",
  "!UNUSED!",
  "!Supply!",
  "!UNUSED!",
  "!Repair!",
  "!Camp!",
  "!Travel!",
  "!Survival!",  /* 300 */
  "You can now use medicine again.",
  "You no longer feel particularly stubborn.",
  "!Dtalk!",
  "You can no longer hold your trollocs under control and they rush off, probably in search of food.",
  "!sever!",
  "!Hunt!",
  "!Test!",
  "!Obscure!",
  "You can no longer concentrate on reading lips.",
  "!Dark Gift!", /* 310 */
  "!Tail!",
  "!FX!",
  "You can no longer listen to whispers in your immediate surroundings.",
  "You no longer feel that well protected by your shieldwall.",
  "The guardsmen you called leave, continuing their patrol elsewhere.",
  "!Tactics!",
  "!Flank!",
  "!Brace!",
  "!Aim!",
  "!Strike!", /* 320 */
  "!Bandheal!",
  "!Bandmarch!",
  "!Inquisition!",
  "You feel your fervor dwindle.",
  "You distance yourself from the Void.", /* 325 */
  "!Warderheal!",
  "!Wardercombat!",
  "!Wardersense!",
  "!Forage!",
  "!Mediate!", /* 330 */
  "!Knowledge!",
  "!Knowhealth!",
  "!Quick Mind!",
  "!Seek!",
  "!Repel!",
  "!Arrest!",
  "You now feel inspired to preach again.","!UNUSED!","!UNUSED!",
  "!Sfx!", /* 340 */
  "!Disguise!",
  "You can now call for backup again.",
  "The incredible rage that burns in your veins cools down.",
  "!Pound!",
  "You're no longer camouflaged.",
  "!Peace!",
  "!Herbmastery!",
  "!Veil!",
  "!Hadntalk!",
  "Your vision is now normal.", 			  /* 350 */
  "You can now use Traction again.",
  "!Retreat!",
  "You now feel rested enough to dig for water again.",
  "!Disengage!",
  "!Stab!",
  "!Haste!",
  "!Critical Hit!",
  "!Stalk!",
  "!Run!",
  "You feel your high morale dwindling.", /* 360 */
  "Your reinforcements decide you're a git and refuse to follow you.",
  "Your burned-in brand mark -finally- heals.",
  "!Truelight!",
  "!Charge!",
  "!Leadership!",
  "You can now recruit more personal guards.",
  "Ravage",
  "Response",
  "!UNUSED!",
  "!Treesing!", /* 370 */
  "!UNUSED!",
  "You're now able to shadow people again.",
  "You no longer feel so arrogant...how embarrassing.",
  "You've now thought out a new suitable insult to offend people with.",
  "!UNUSED!",
  "You're no longer affected by blademastery",
  "You're no longer affected by spearmastery",
  "You can now employ people again.",
  "!UNUSED!",
  "!UNUSED!", /* 380 */
  "!Trolloc!",
  "!Ogier!",
  "!Old Tongue!",
  "!UNUSED!",
  "!Findherb!",
  "!Herbident!","!UNUSED!","!UNUSED!","!UNUSED!",
  "The Void vanishes, as you can't keep your concentration.", /* 390 */
  "!Enhanced Spell!",
  "!Target!",
  "!UNUSED!","!UNUSED!",
  "You feel less inspired now...",
  "!Minor Disguise!",
  "!Intimidate!",
  "!UNUSED!",
  "!Endure!",
  "!UNUSED!", /* 400 */
  "!UNUSED!","!UNUSED!","!UNUSED!","!UNUSED!","!UNUSED!",
  "!UNUSED!",
  "You may now quit the game again.",
  "You are no longer affected by longsleep."
};



const char *npc_class_types[] = {
  "Normal",
  "Undead",
  "\n"
};



const int rev_dir[] =
{
  2,
  3,
  0,
  1,
  5,
  4,
  8,
  9,
  6,
  7
};


const int movement_loss[] =
{
  1,	/* Inside     */
  1,	/* City       */
  2,	/* Field      */
  3,	/* Forest     */
  4,	/* Hills      */
  6,	/* Mountains  */
  4,	/* Swimming   */
  1,	/* Unswimable */
  1,	/* Flying     */
  5     /* Underwater */
};

const char *languages[] = {
  "Common",
  "Trolloc",
  "Ogier",
  "Old Tongue"
};

const char *weekdays[] = {
  "the Day of the Moon",
  "the Day of the Bull",
  "the Day of the Deception",
  "the Day of Thunder",
  "the Day of Freedom",
  "the day of the Great Gods",
"the Day of the Sun"};


const char *month_name[] = {
  "Taisham",
  "Jumara",
  "Saban",
  "Aine",
  "Adar",
  "Saven",
  "Amadaine",
  "Tammaz",
  "Maighdal",
  "Choren",
  "Shaldine",
  "Nesan",
  "Danu", 
  "<>",  // Just have these lines to avoid index-out-of-bounds initially.
  "<>",
  "<>",
  "<>"
};


const int sharp[] = {
  0,
  0,
  0,
  1,				/* Slashing */
  0,
  0,
  0,
  0,				/* Bludgeon */
  0,
  0,
  0,
0};				/* Pierce   */

char REM_MAIN_TXT[] = "You release any weaves you may be maintaining\r\n";

const char *toggle_scmds[] = {
      "nospam",
      "nohassle",		/* NOHASSLE	*/
      "brief",
      "compact",
      "notell",
      "auction",
      "shout",
      "question",
      "ooc",
      "wiznet",		/* NOWIZ	*/
      "quest",
      "roomflags",		/* ROOMFLAGS	*/
      "repeat",
      "holylight",		/* HOLYLIGHT	*/
      "\n",		/* SLOWNS	*/
      "autoexit",
      "gdt",
      "autogold",
      "autoloot",
      "autojunk",
      "autoassist",	
      "autosplit",
      "wt",
      "lookahead",
      "commspam",
      "pwimpy",
      "sliceweave",
      "hunger",
      "death",
      "dt",
      "ot",
      "obscene",
      "poseid",
      "sing",
      "ic",
      "noweather",
      "!unsued!",
      "pchan",
      "rptag",
      "xlogtag",
      "timestamps",
      "\n"
};

/*talents struct array*/
const struct talents master_talent_list[NUM_TALENTS] = { {"Dreamer", 1, 0, 0},   
                         {"Sniffer", 1, 0, 0},
                             {"Blocking Gateways", 0, 0, 1}, 
                         {"Gating(Use travelling instead)", 0, 0, 0},
                         {"Skimming", 0, 0, 1},         
                         {"Inverting Weaves", 0, 0, 1},    /*5*/
                         {"Heal Stilling", 0, 0, 1},
                         {"Balefire", 0, 0, 1},
                         {"Creating Angreal or Ter'angreal", 1, 0, 0},
                         {"Affinity for Healing", 1, 1, 0},      
                         {"Affinity for Warding", 1, 1, 0},    /*10*/
                         {"Power Forging", 1, 0, 1},
                         {"Disguising", 0, 0, 1},
                         {"Mask Channeling", 0, 0, 1},
                         {"Foretelling", 1, 0, 0},      
                         {"Invisibility", 0, 0, 1},     /*15*/
                         {"Reading Auras", 1, 0, 0},
                         {"Picking Apart Weaves", 0, 0, 1},
                         {"Reading Residues", 1, 0, 0},
                         {"Gating into TAR", 0, 0, 1},    
                         {"Treesinging", 1, 0, 0},     /*20*/
                         {"Minor Compulsion", 1, 0, 0},
                         {"Compulsion", 1, 0, 1},
                         {"!UNUSED!", 0, 0, 1},
                         {"Illusion Creation", 1, 1, 0},     
                         {"Affinity for Weather(use Cloud Dancing)", 0, 0, 0},   /*25*/
                         {"Affinity for Pain(use Aff. for Healing)", 0, 0, 0},
                         {"Cloud Dancing", 1, 1, 0},
                         {"Dreamwalking", 1, 0, 0},
                         {"Weak Ta'veren", 0, 0, 0},
                         {"Travelling", 0, 0, 1},   /*30*/
                         {"Ta'veren", 1, 1, 0},
                         {"Earth Singing", 1, 1, 0}};

const char *cg_names[] = {
  "Newbie",
  "GuildLeader",
  "StoryTeller",
  "GL-Builder",
  "Builder",
  "Administrator",
  "StoryMaster",
  "HeadGL",
  "HeadBuilder",
  "Owner",
  "Weaver",
  "HeadCoder",
  "HeadCouncil",
  "NewbiePR",
  "WebMaster",
  "DGS",
  "AEdit",
  "\n"
};

const char *cg_bits[] = {
  "NEWBIE",
  "GL",
  "ST",
  "GL_BUILDER",
  "BUILDER",
  "ADMIN",
  "SM",
  "HGL",
  "HB",
  "OWNER",
  "WEAVER",
  "HCODER",
  "HCOUNCIL",
  "NEWBIEPR",
  "WEBMASTER",
  "DGS",
  "AEDIT",
  "\n"
};

const char *mobprog_types[] = {
  "INFILE",
  "ACT",
  "SPEECH",
  "RAND",
  "FIGHT",
  "DEATH",
  "HITPRCNT",
  "ENTRY",
  "GREET",
  "ALL_GREET",
  "GIVE",
  "BRIBE",
  "\n"
};
