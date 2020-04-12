# Frank DaCosta: Writing BASIC adventure programs for the TRS-80
# Basement and Beasties

# Just for the record: list of valid directions
# used as accessing "travel"
# "default" is used as default for horizontal directions (not "up", "down", "magical")
# if there is no "default", "False" returned
horizontal_directions={"n", "ne", "e", "se", "s", "sw", "w", "nw"}
directions= horizontal_directions.union({"default", "up", "down", "magical"})

class Travel:
  def __init__(self):
    self.dict= {}
  def get(self, dir):
    if dir in self.dict:
      return self.dict[key]
    if dir in horizontal_directions and "default" in self.dict:
      return self.dict["default"]
    else:
      return False
  def put(self, key, value):
    self.dict[key]= value
  def merge(self, newvalues):
    self.dict.update(newvalues)

class Room:
  def __init__(self, sDesc, lDesc):
    self.sDesc= sDesc
    self.lDesc= lDesc
    self.travel= Travel();
    self.Dark= False

class DarkRoom(Room):
  def __init__(self, sDesc, lDesc):
    super().__init__(sDesc, lDesc)
    self.Dark= True

class Obstacle:
  msg= {"stop": "You cannot go there"}
  def __init__(self, sDesc):
    self.sDesc= sDesc
    self.visible= True
    self.isOpen= False

class InanimateObstacle(Obstacle):
  msg= {}
  msg.update (Obstacle.msg)
  msg["nokey"]= "You have no key!"
  msg["closing"]= "It slams shut and the lock catches."
  def __init__(self, sDesc):
    super().__init__(sDesc)

class Door(InanimateObstacle):
  msg= {}
  msg.update (InanimateObstacle.msg)
  msg["stop"]= "The door is closed and locked."
  msg["opening"]= "The door swings open wide."
  def __init__(self, sDesc="Door"):
    super().__init__(sDesc)

class Grate(InanimateObstacle):
  msg= {}
  msg.update (InanimateObstacle.msg)
  msg["stop"]= "The grate is closed and locked."
  msg["opening"]= "With a creak, the grate falls open."
  def __init__(self, sDesc="Grate"):
    super().__init__(sDesc)

class Monster(Obstacle):
  msg= {}
  msg.update (Obstacle.msg)
  msg["stop"]= "The monster blocks your way!"
  def __init__(self, sDesc, lDesc):
    super().__init__(sDesc)
    self.lDesc= lDesc

Grate28= Grate()
Door411= Door()
Door612= Door()

Mantis= Monster("Mantis", "A Giant Mantis crouches nearby, ready to pounce!")
Iguana= Monster("Iguana", "A huge Iguana paces restlessly nearby, keeping an eye on you!")
Spider= Monster("Spider", "A giant white Spider mandibles twitching, towers above you!")
Terror= Monster("Nameless Terror",
  "The Nameless Terror arises from a pit" \
  ", blocking your retreat with slimy tentacles!")

class Death:
  def __init__(self, sDesc):
    self.sDesc= sDesc;

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
    self.visitedRooms= {}
    self.ownedItems= {}
    self.position= BottomOfPit
  def describeRoom(self, room="Default", which="Default"):
    if room=="Default": room= self.position
    if which=="Default":
      if room in self.visitedRooms: which= "Short"
      else:                         which= "Long"
    if which=="Short":  print (room.sDesc)
    elif which=="Long": print (room.lDesc)
    else:               print ("Serious problem in method PlayerState.describeRoom")

ps= PlayerState()

ps.describeRoom(Office);
ps.describeRoom(which='Long');
ps.describeRoom(Ruins,"Short");
ps.describeRoom(SlimyCavern,"Long");
