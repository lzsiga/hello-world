# Frank DaCosta: Writing BASIC adventure programs for the TRS-80
# Basements and Beasties

# constant data

from typing import Set, Dict
from bb_types import *

# special name to represent the locations of objects hold by the player
Player= object()

# special name to represent the locations of dead monsters
Void= object()

FallingDeath= Death("Fall", "You fall to your doom...");
BurningDeath= Death("Burn", "You burn in the flames!");

BottomOfPit   = Room("Bottom of Pit", "")
Ruins         = Room("Ruins", "")
WeaponsRoom   = DarkRoom("Weapons Room", "")
LostBattle    = DarkRoom("Lost Battle", "")
TombRoom      = DarkRoom("Tomb Room", "")
OracleRoom    = DarkRoom("Oracle Room", "")
TreasureVault = DarkRoom("Treasure Vault", "")
GuardPost     = DarkRoom("Guard Post", "")
MazeA         = DarkRoom("You are lost in a maze!", "You are lost in a maze!")
NarrowLedge   = DarkRoom("Narrow Ledge", "")
Cell          = DarkRoom("Cell", "")
Office        = DarkRoom("Office", "")
LunchRoom     = DarkRoom("Lunch Room", "")
CobwebRoom    = DarkRoom("Cobweb Room", "")
MazeB         = DarkRoom("You are lost in a maze!", "You are lost in a maze!")
MazeC         = DarkRoom("You are lost in a maze!", "You are lost in a maze!")
RushingStream = DarkRoom("Rushing Stream", "")
SlimyCavern   = DarkRoom("Slimy Cavern", "")
SteamyCave    = DarkRoom("Steamy Cave", "")
FierySpire    = DarkRoom("Fiery Spire", "")

allDarkRooms= \
  {WeaponsRoom, LostBattle,  TombRoom,      OracleRoom,  TreasureVault, GuardPost, \
   MazeA,       NarrowLedge, Cell,          Office,      LunchRoom,     CobwebRoom, \
   MazeB,       MazeC,       RushingStream, SlimyCavern, SteamyCave,    FierySpire}

allRooms= allDarkRooms.union({BottomOfPit, Ruins})

Torch=   Obj(Ruins,         "Torch",          "There is a burning torch here!")
Axe=     Obj(WeaponsRoom,   "Axe",            "There is a hefty axe here!")
Key=     Obj(NarrowLedge,   "Key",            "There is a large key here!")
Grenade= Obj(Office,        "Grenade",        "There is an enchanted grenade here!")
JCrown=  Obj(LostBattle,    "Jeweled Crown",  "There is a crown of jewels here!")
GCube=   Obj(TreasureVault, "Golden Cube",    "There is a golden cube here!")
DBeetle= Obj(FierySpire,    "Diamond Beetle", "There is a diamond here, carved like a beetle!")
SBelt=   Obj(Cell,          "Silver Belt",    "There is a fine silver belt here!")
PRing=   Obj(TombRoom,      "Platinum Ring",  "There is a ring here of pure platinum!")
Onyx=    Obj(SteamyCave,    "Onyx",           "There is a polished onyx here!")
Coin=    Obj(TreasureVault, "Coin",           "There is a coin here worth millions!")
HourG=   Obj(OracleRoom,    "Hourglass",      "There is an ancient hourglass here!")

allTreasures= {JCrown, GCube, DBeetle, SBelt, PRing, Onyx, Coin, HourG}
allObjs= allTreasures.union({Torch, Axe, Key, Grenade})

Grate28= Grate()
Door411= Door()
Door612= Door()

allGates= {Grate28, Door411, Door612}

Mantis= Monster(LostBattle,  "Mantis", "A Giant Mantis crouches nearby, ready to pounce!")
Iguana= Monster(SlimyCavern, "Iguana", "A huge Iguana paces restlessly nearby, keeping an eye on you!")
Spider= Monster(CobwebRoom,  "Spider", "A giant white Spider mandibles twitching, towers above you!")
Terror= Monster(OracleRoom,  "Nameless Terror",
  "The Nameless Terror arises from a pit" \
  ", blocking your retreat with slimy tentacles!")

allMonsters= {Mantis, Iguana, Spider, Terror}

LostBattle.monster= Mantis
OracleRoom.monster= Terror
CobwebRoom.monster= Spider
SlimyCavern.monster= Iguana

Ruins.gate= GuardPost.gate= Grate28
LostBattle.gate= Cell.gate= Door411
OracleRoom.gate= Office.gate= Door612

Orc= ActiveMonster("Orc", "There is an Angry Orc nearby!")

allEnemies= allMonsters.union({Orc})

BottomOfPit.lDesc= \
  "You stand at the bottom of a large pit." \
  " At your feet is a narrow hole just wide enough to climb into."
Ruins.lDesc= \
  "Here are the ruins of an ancient Troll-castle." \
  " Nearby is a grate leading down into darkness..."
WeaponsRoom.lDesc= \
  "This was apparently once a weapons room, though cases are all empty now." \
  " There's a hole in the roof, an archway to the east" \
  ", and a jagged hole in the southeast wall."
LostBattle.lDesc= \
  "The signs of a great battle between Trolls and terrible Beast-Men are evident..." \
  " From the looks of it, the Trolls lost." \
  " Bodies are everywhere. There is a jagged hole to the west, a hall northeast" \
  ", and a south door."
TombRoom.lDesc= \
  "The walls are lined with coffin cases..." \
  " This is the Troll cemetery, it seems." \
  " A southwest door leads out."
OracleRoom.lDesc= \
  "This is a small dark room smelling of Magic." \
  " The Oracle has left a message on the wall." 
TreasureVault.lDesc= \
  "At last! The Treasure Vault!" \
  " What a shame that so much of the original wealth has been removed!" \
  " There is a southeast door out."
GuardPost.lDesc= \
  "This was once the main guardpost to the underground kingdom of the Trolls." \
  " There is entrance-grate set in the roof and a south exit door."
NarrowLedge.lDesc= \
  "You walk along a narrow ledge running northwest and southeast." \
  " To the west is a rapid stream far below," \
  " and to the east is a bottomless chasm!"
Cell.lDesc= \
  "This is a small prison cell. Through the bars, you can see a nice office... unreachable." \
  " There's a north door."
Office.lDesc= \
  "Here is a business office with empty, ransacked desks and cabinets." \
  " A barred window in the wall shows a small prison cell of some sorts." \
  " There are two doors, to the northwest and east" \
  ", and a rocky hole in the south wall."
LunchRoom.lDesc= \
  "This is the lunch room complete with coke machine... empty, unfortunately." \
  " There is a door to the west."
CobwebRoom.lDesc= \
  "What a creepy place! There are cobwebs everywhere!" \
  " A door leads north, a hall goes northwest, and there is a hole in the floor."
RushingStream.lDesc= \
  "You are splashing about in a cold, rushing stream!" \
  " Nothing you do seems to stop your perilous ride towards a nearby stony cave entrance!"
SlimyCavern.lDesc= \
  "You lie on the sands of a dark, slimy cavern by a stream." \
  " The walls are covered with disgusting ooze." \
  " There is a hole in the northern rocks and a path northeastward."  
SteamyCave.lDesc= \
  "Sweat beads on your face as you stand in a steamy cave." \
  " Smoke rises from a hole in the floor" \
  ", and there is another ragged hole in the roof within reach." \
  " There's also a path going southwest."
FierySpire.lDesc= \
  "You are crouching on a fiery spire, a pinnacle surrounded by flames." \
  " A low roof with a hole hangs a foot above your head." \
  " It is unbearably hot!"

#pages 25-26 [orig: 18-19]
BottomOfPit.travel.merge({ \
  "magical": OracleRoom, "n": BottomOfPit,  \
  "se": BottomOfPit, "s": BottomOfPit, "sw": BottomOfPit, \
  "w": BottomOfPit, "nw": BottomOfPit, \
  "down": WeaponsRoom, "ne": Ruins, "e": Ruins})

Ruins.travel.put("down", [Grate28, GuardPost])
Ruins.travel.put("nw", Ruins)
Ruins.travel.put("sw", BottomOfPit)
Ruins.travel.put("w", BottomOfPit)
Ruins.travel.put("n", Ruins)
Ruins.travel.put("ne", Ruins)
Ruins.travel.put("e", Ruins)
Ruins.travel.put("se", Ruins)
Ruins.travel.put("s", Ruins)

WeaponsRoom.travel.put("up", BottomOfPit)
WeaponsRoom.travel.put("e",  LostBattle)
WeaponsRoom.travel.put("se", NarrowLedge)

LostBattle.travel.put("ne", [Mantis, TombRoom])
LostBattle.travel.put("s",  [Door411, Cell])
LostBattle.travel.put("w",  WeaponsRoom)

TombRoom.travel.put("sw", LostBattle)

OracleRoom.travel.put("magical", BottomOfPit)
OracleRoom.travel.put("se", [Terror, Door612, Office])
OracleRoom.travel.put("down", FallingDeath)

TreasureVault.travel.put("se", CobwebRoom);

GuardPost.travel.put("up", [Grate28, Ruins])
GuardPost.travel.put("s", CobwebRoom)

MazeA.travel.put("n", MazeA)
MazeA.travel.put("nw", MazeA)
MazeA.travel.put("s", MazeA)
MazeA.travel.put("se", MazeB)
MazeA.travel.put("e", MazeC)

NarrowLedge.travel.merge({ \
  "down": RushingStream, \
  "n": FallingDeath, "ne": FallingDeath, "e": FallingDeath, \
  "se": MazeC, "s": RushingStream, "sw": RushingStream, "w": RushingStream, \
  "nw": WeaponsRoom})

Cell.travel.put("n", [Door411, LostBattle])

Office.travel.put("nw", [Door612, OracleRoom])
Office.travel.put("e", LunchRoom)
Office.travel.put("s", SlimyCavern)

LunchRoom.travel.put("w", Office)

CobwebRoom.travel.put("n", GuardPost)
CobwebRoom.travel.put("s", SteamyCave)
CobwebRoom.travel.put("down", SteamyCave)
CobwebRoom.travel.put("nw", [Spider, TreasureVault])

MazeB.travel.put("n",  MazeB)
MazeB.travel.put("e",  MazeB)
MazeB.travel.put("s",  MazeB)
MazeB.travel.put("sw", MazeC)
MazeB.travel.put("w",  MazeA)

MazeC.travel.merge({\
  "n": MazeB, "ne": MazeC,       "e": MazeC, \
  "s": MazeC, "w":  NarrowLedge, "nw": MazeA})

RushingStream.travel.merge({\
  "n":    SlimyCavern, "ne": SlimyCavern, "e":  SlimyCavern, \
  "se":   SlimyCavern, "s":  SlimyCavern, "sw": SlimyCavern, \
  "w":    SlimyCavern, "nw": SlimyCavern, "up": SlimyCavern, \
  "down": SlimyCavern})

SlimyCavern.travel.put("n",  Office)
SlimyCavern.travel.put("ne", [Iguana, SteamyCave])

SteamyCave.travel.merge({\
  "up": CobwebRoom, "down": FierySpire,
  "n":  CobwebRoom, "sw": SlimyCavern})

FierySpire.travel.merge({\
    "n":    BurningDeath, "ne": BurningDeath, "e":  BurningDeath, \
    "se":   BurningDeath, "s":  BurningDeath, "sw": BurningDeath, \
    "w":    BurningDeath, "nw": BurningDeath, "up": SteamyCave,   \
    "down": BurningDeath})

MaxItems= 5

Messages=dict()
Messages['BadDirection']=   'Cannot move: bad direction (valid: Up, Down, N, NE, E, SE, S, SW, W, NW)'
Messages['NoWay']=          'You can\'t go that way.'
Messages['UnknownCommand']= 'I don\'t understand this command.'
Messages['ClosedGrate']=    'The grate is closed and locked!'
Messages['ClosedDoor']=     'The door is tightly shut and locked.'
Messages['NoObject']=       'Bad command: direct object missing.'
Messages['PickupMonster']=  'You manifest some pretty suicidal tendencies, fella!'
Messages['ObstacleObject']= 'You cannot pickup an Obstacle!'
Messages['UnmoveableObject']= 'You try unsuccessfully... Immovable!'
Messages['AlreadyTaken']=   'You already have it!'
Messages['EmptyInventory']= 'Your hands are empty.'
Messages['Inventory']=      'You have the following:'
Messages['Taken']=          'You got the %s!'
Messages['Dropped']=        'You dropped the %s!'
Messages['ObjNotHere']=     'That object isn\'t here.'
Messages['HandsFull']=      'Your arms are full... you can carry no more.'
Messages['DontHaveIt']=     'You don\'t have it!'
Messages['FightIt']=        'You attack the %s!'
