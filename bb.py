# Frank DaCosta: Writing BASIC adventure programs for the TRS-80
# Basement and Beasties

from typing import Set, Dict
from bb_types import *

FallingDeath= Death("You fall to your Doom...");
BurningDeath= Death("You burn in the flames!");

# https://archive.org/details/Writing_BASIC_Adventure_Programs_for_the_TRS-80_1982_Frank_DaCosta/page/n25/mode/2up
#pages 25-26 [orig: 18-19], 148-150 [orig: 141-143]
BottomOfPit   = Room("Bottom of Pit", "")
Ruins         = Room("Ruins", "")
WeaponsRoom   = DarkRoom("Weapons Room", "")
LostBattle    = DarkRoom("Lost Battle", "")
TombRoom      = DarkRoom("Tomb Room", "")
OracleRoom    = DarkRoom("Oracle Room", "")
TreasureVault = DarkRoom("Treasure Vault", "")
GuardPost     = DarkRoom("Guard Post", "");
MazeA         = DarkRoom("You are lost in a maze", "You are lost in a maze")
NarrowLedge   = DarkRoom("Narrow Ledge", "")
Cell          = DarkRoom("Cell", "")
Office        = DarkRoom("Office", "")
LunchRoom     = DarkRoom("Lunch Room", "")
CobwebRoom    = DarkRoom("Cobweb Room", "")
MazeB         = DarkRoom("You are lost in a maze", "You are lost in a maze")
MazeC         = DarkRoom("You are lost in a maze", "You are lost in a maze")
RushingStream = DarkRoom("Rushing Stream", "")
SlimyCavern   = DarkRoom("Slimy Cavern", "")
SteamyCave    = DarkRoom("Steamy Cave", "")
FierySpire    = DarkRoom("Fiery Spire", "")

allDarkRooms= \
  {WeaponsRoom, LostBattle,  TombRoom,      OracleRoom,  TreasureVault, GuardPost, \
   MazeA,       NarrowLedge, Cell,          Office,      LunchRoom,     CobwebRoom, \
   MazeB,       MazeC,       RushingStream, SlimyCavern, SteamyCave,    FierySpire}

allRooms= {BottomOfPit, Ruins}
allRooms.update(allDarkRooms)

Grate28= Grate()
Door411= Door()
Door612= Door()

Torch= Obj(Ruins,      "Torch", "There is a burning torch here!")
Crown= Obj(LostBattle, "Jewelled crown", "There is a crown of jewels here!")

allObjs= {Torch, Crown}
allTreasure= {Crown}

allGates= [Grate28, Door411, Door612]

Mantis= Monster("Mantis", "A Giant Mantis crouches nearby, ready to pounce!")
Iguana= Monster("Iguana", "A huge Iguana paces restlessly nearby, keeping an eye on you!")
Spider= Monster("Spider", "A giant white Spider mandibles twitching, towers above you!")
Terror= Monster("Nameless Terror",
  "The Nameless Terror arises from a pit" \
  ", blocking your retreat with slimy tentacles!")

allMonsters=  [Mantis, Iguana, Spider, Terror]

LostBattle.monster= Mantis
OracleRoom.monster= Terror
CobwebRoom.monster= Spider
SlimyCavern.monster= Iguana

Ruins.gate= GuardPost.gate= Grate28
LostBattle.gate= Cell.gate= Door411
OracleRoom.gate= Office.gate= Door612

# 148-150 [orig: 141-143]
BottomOfPit.lDesc= \
  "You stand at the bottom of a large pit." \
  " At your feet is a narrow hole just wide enough to climb into."
Ruins.lDesc= \
  "Here are the ruins of an ancient Troll-castle." \
  " Nearby is a grate leading down into darknes..."
WeaponsRoom.lDesc= \
  "This was apparently once a weapons room, though cases are all empty now." \
  " There's a hole in the roof, an archway to the east" \
  ", and a jagged hole in the southest wall."
LostBattle.lDesc= \
  "The signs of a great battle between Trolls and terrible Beast-Men are evident..." \
  " From the looks of it, the Trolls lost." \
  " Bodies are everywhere. There is a jagged hole to the west, a hall northeast" \
  ", and a south door"
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
  " There is a southeast doot out."
GuardPost.lDesc= \
  "This was once the main guardpost to the underground kingdom of the Trolls." \
  " There is entrance-grate set in the roof and a south exit door."
Cell.lDesc= \
  "This is a small prison cell. Through the bars, you can see a nice office... unreachable." \
  " There's a north door."
Office.lDesc= \
  "Here is a business office with empty, ransacked desks and cabinets." \
  " A barred window in the wall shows a small prison cell of some sorts." \
  " There are two doors, to the northwest and east" \
  ", and a rocky hole in the south wall."
LunchRoom.lDesc= \
  "This is the lunch room comlete with coke machine... empty, unfortunately." \
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
  "default": BottomOfPit, "magical": OracleRoom,
  "down": WeaponsRoom, "ne": Ruins, "e": Ruins})

Ruins.travel.down= [Grate28, GuardPost]
Ruins.travel.put("default", Ruins)
Ruins.travel.put("sw", BottomOfPit)
Ruins.travel.put("w", BottomOfPit)

WeaponsRoom.travel.put("up", BottomOfPit)
WeaponsRoom.travel.put("e",  LostBattle)
WeaponsRoom.travel.put("se", NarrowLedge)

LostBattle.travel.put("ne", [Mantis, TombRoom])
LostBattle.travel.put("s",  [Door411, Cell])
LostBattle.travel.put("w",  WeaponsRoom)

TombRoom.travel.put("sw", LostBattle)

OracleRoom.travel.put("magical", BottomOfPit)
OracleRoom.travel.put("se", [Terror, Door612, CobwebRoom])
OracleRoom.travel.put("down", FallingDeath)

TreasureVault.travel.put("se", CobwebRoom);

GuardPost.travel.put("up", [Grate28, Ruins])
GuardPost.travel.put("s", CobwebRoom)

MazeA.travel.put("n", MazeA)
MazeA.travel.put("s", MazeA)
MazeA.travel.put("se", MazeA)

NarrowLedge.travel.merge({ \
  "down": RushingStream, \
  "n": FallingDeath, "ne": FallingDeath, "e": FallingDeath, \
  "se": MazeC, "s": RushingStream, "sw": RushingStream, "w": RushingStream, \
  "nw": WeaponsRoom})

Cell.travel.put("n", [Door411, LostBattle])

Office.travel.put("nw", [Door612, OracleRoom])
Office.travel.put("e", LunchRoom)
Office.travel.put("s", SlimyCavern)

LunchRoom.travel.put("w", "Office")

CobwebRoom.travel.put("n", GuardPost)
CobwebRoom.travel.put("s", SteamyCave)
CobwebRoom.travel.put("down", SteamyCave)
CobwebRoom.travel.put("nw", TreasureVault)

MazeB.travel.put("n",  MazeB)
MazeB.travel.put("e",  MazeB)
MazeB.travel.put("s",  MazeB)
MazeB.travel.put("sw", MazeC)
MazeB.travel.put("w",  MazeA)

MazeC.travel.merge({\
  "n": MazeB, "ne": MazeC,       "e": MazeB, \
  "s": MazeB, "w":  NarrowLedge, "nw": MazeA})

RushingStream.travel.put("default", SlimyCavern)
RushingStream.travel.put("up", SlimyCavern)
RushingStream.travel.put("down", SlimyCavern)

SlimyCavern.travel.put("n",  Office)
SlimyCavern.travel.put("ne", SteamyCave)

SteamyCave.travel.merge({\
  "up": CobwebRoom, "down": FierySpire,
  "n":  CobwebRoom, "sw": [Iguana, SlimyCavern]})

FierySpire.travel.put("default", BurningDeath)
FierySpire.travel.put("up", SteamyCave)

# variable data
class PlayerState:
  def __init__(self):
    self.reset()

  def reset(self):
    self.visitedRooms= set()
    self.ownedObjs= set()
    self.position= BottomOfPit
  # reset state of the inanimated obstacles
    self.resetGates()
    self.resetMonsters()
    self.resetPositions()

  def resetMonsters(self):
    self.monsterState= {}
    for m in allMonsters:
       self.monsterState[m]= isAlive

  def resetGates(self):
    self.gateState= {}
    for g in allGates:
       self.gateState[g]= isClosed

  # the player and the rooms can have belongings,
  # the monsters cannot (for now)
  # now everyone loses everything
  # then every object goes to its original place
  def resetPositions(self):
    self.belongings: Dict[object, Set] = dict()
    self.belongings[Player]= set()
    for r in allRooms:
       self.belongings[r]= set()
    self.positions= dict()
    for o in allObjs:
      toRoom= o.originalPosition
      self.positions[o]= toRoom
      self.belongings[toRoom].add(o)

  def describeRoom(self, room="Default", which="Default"):
    if room=="Default": room= self.position
    if which=="Default":
      if room in self.visitedRooms: which= "Short"
      else:                         which= "Long"
    if which=="Short":  print (room.sDesc)
    elif which=="Long": print (room.lDesc)
    else:               print ("Serious problem in method PlayerState.describeRoom")
    self.listObjsInRoom(room)

  def listObjsInRoom(self, room="Default", separator=True):
    if room=="Default": room= self.positions[Player];
    if len(self.belongings[room])==0: return
    if (separator): print()
    for o in self.belongings[room]:
      print (o.lDesc)

  def enterRoom(self, intoroom):
    self.position= intoroom
    if intoroom.isDark:
      print ("It is pitch dark! You may fall into a pit!")
      # random death
    else:
      self.describeRoom(intoroom)
      self.visitedRooms.add(intoroom)
#endclass PlayerState

ps= PlayerState()

"""
ps.describeRoom(Office)
ps.describeRoom(which="Long")
ps.describeRoom(Ruins,"Short")
ps.describeRoom(SlimyCavern,"Long")
print(ps.monsterState)
"""

print ("=== Test: enterRoom(Ruins) ===")
ps.enterRoom(Ruins)

print ("\n=== Test: enterRoom(BottomOfPit) ===")
ps.enterRoom(BottomOfPit)

print ("\n=== Test: enterRoom(Ruins) ===")
ps.enterRoom(Ruins)

print ("\n=== Test: enterRoom(Office) ===")
ps.enterRoom(Office)
