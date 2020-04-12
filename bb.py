# Frank DaCosta: Writing BASIC adventure programs for the TRS-80
# Basement and Beasties

# Just for the record: list of valid directions
# used as accessing 'travel'
# 'default' is used as default for horizontal directions (not "up", "down", "magical")
# if there is no 'default', 'False' returned
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
  def __init__(self, sDesc):
    self.sDesc= sDesc
    self.visible= True
    self.isOpen= False

class Monster(Obstacle):
  def __init__(self, sDesc, lDesc):
    super().__init__(sDesc)
    self.sDesc= sDesc
    self.lDesc= lDesc

Grate28= Obstacle("Grate")
Door411= Obstacle("Door")
Door612= Obstacle("Door")
Mantis= Monster("Mantis", "A Giant Mantis crouches nearby, ready to pounce.")
Terror= Monster("Nameless Terror",
  "The Nameless Terror arises from a pit" \
  ", blocking your retreat with slimy tentacles!")

# https://archive.org/details/Writing_BASIC_Adventure_Programs_for_the_TRS-80_1982_Frank_DaCosta/page/n25/mode/2up
#pages 25-26 [orig: 18-19], 148-150 [orig: 141-143]
BottomOfPit = Room("Bottom of Pit", "")
Ruins       = Room("Ruins", "")
WeaponsRoom = DarkRoom("Weapons Room", "")
LostBattle  = DarkRoom("Lost Battle", "")
TombRoom    = DarkRoom("Tomb Room", "")
OracleRoom  = DarkRoom("Oracle Room", "")
#7
GuardPost   = DarkRoom("Guard Post", "");
#9
NarrowLedge = DarkRoom("Narrow Ledge", "")
Cell        = DarkRoom("Cell", "")
#12
#13
CobwebRoom  = DarkRoom("Cobweb Room", "")

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
  "This is a small dark room smelling of Magic."\
  " The Oracle has left a message on the wall." 
#7
GuardPost.lDesc= \
  "This was once the main guardpost to the underground kingdom of the Trolls." \
  " There is entrance-grate set in the roof and a south exit door."
Cell.lDesc= \
  "This is a small prison cell. Through the bars, you can see a nice office... unreachable." \
  " There's a north door."
CobwebRoom.lDesc= \
  "What a creepy place! There are cobwebs everywhere!" \
  " A door leads north, a hall goes northwest, and there is a hole in the floor."

#pages 25-26 [orig: 18-19]
BottomOfPit.travel.put("default", BottomOfPit)
BottomOfPit.travel.put("magical", OracleRoom)
BottomOfPit.travel.put("down", WeaponsRoom)
BottomOfPit.travel.put("ne", Ruins)
BottomOfPit.travel.put("e", Ruins)

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

OracleRoom.travel.put("magical", BottomOfPit)
OracleRoom.travel.put("se", [Terror, Door612, CobwebRoom])

Cell.travel.put("n", [Door411, LostBattle])
