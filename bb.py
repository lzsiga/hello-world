# Basement and Beasties
# Frank DaCosta: Writing BASIC adventure programs for the TRS-80

class Room:
  def __init__(self, sDesc):
    self.sDesc= sDesc
    self.lDesc= "Later"
    self.Dark= False
    self.up=   False
    self.down= False
    self.n=    False
    self.ne=   False
    self.e=    False
    self.se=   False
    self.s=    False
    self.sw=   False
    self.w=    False
    self.nw=   False

class DarkRoom(Room):
  def __init__(self, sDesc):
    super().__init__(sDesc)
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
Mantis= Monster("Mantis", "A Giant Mantis crouches nearby, ready to pounce.")

# https://archive.org/details/Writing_BASIC_Adventure_Programs_for_the_TRS-80_1982_Frank_DaCosta/page/n25/mode/2up
#pages 25-26, 141-142
BottomOfPit = Room("Bottom of Pit")
Ruins       = Room("Ruins")
WeaponsRoom = DarkRoom("Weapons Room")
LostBattle  = DarkRoom("Lost Battle")
#5
#6
#7
GuardPost   = DarkRoom("Guard Post");
#9
NarrowLedge = DarkRoom("Narrow Ledge")

# pages 141-142
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
#5
#6
#7
GuardPost.lDesc= \
  "This was once the main guardpost to the underground kingdom of the Trolls." \
  " There is entrance-grate set in the roof and a south exit door."

# pages 25-26
BottomOfPit.down= WeaponsRoom
BottomOfPit.n= BottomOfPit
BottomOfPit.ne= Ruins
BottomOfPit.e= Ruins
BottomOfPit.se= BottomOfPit
BottomOfPit.s= BottomOfPit
BottomOfPit.sw= BottomOfPit
BottomOfPit.w= BottomOfPit
BottomOfPit.nw= BottomOfPit

Ruins.down= [Grate28, GuardPost]
Ruins.n= Ruins
Ruins.ne= Ruins
Ruins.e= Ruins
Ruins.se= Ruins
Ruins.s= Ruins
Ruins.sw= BottomOfPit
Ruins.w= BottomOfPit
Ruins.nw= Ruins

WeaponsRoom.up= BottomOfPit
WeaponsRoom.e=  LostBattle
WeaponsRoom.se= NarrowLedge

LostBattle.ne= [Mantis, TombRoom]
LostBattle.s=  [Door411, Cell]
LostBattle.w=  WeaponsRoom
