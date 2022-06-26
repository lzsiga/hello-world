# Frank DaCosta: Writing BASIC adventure programs for the TRS-80
# Basements and Beasties

# Type and constant-definitions that aren't specific to this game
# Also declaration of variables "allRooms", "allDarkRooms", "allGates",
# "allMonsters", "allObjs", "allTreasures"

from typing import Set

# directions: keys to "travel" dictionaries
# "default" is used as default for horizontal directions (not "up", "down", "magical")
# if there is no "default", "False" returned

horizontal_directions={"n", "ne", "e", "se", "s", "sw", "w", "nw"}
directions= horizontal_directions.union({"default", "up", "down", "magical"})

class Travel:
  def __init__(self):
    self.dict= {}

  def __str__(self):
    return str(self.dict)

  def get(self, dir):
    if dir in self.dict:
      return self.dict[dir]
    else:
      return None

  def put(self, key, value):
    self.dict[key]= value

  def merge(self, newvalues):
    self.dict.update(newvalues)

class Room:
  def __init__(self, sDesc, lDesc):
    self.sDesc= sDesc
    self.lDesc= lDesc
    self.travel= Travel();
    self.isDark= False
    self.monster= False
    self.gate= False
  def __repr__(self):
    return self.sDesc
  def __str__(self):
    return self.sDesc

allRooms: Set[Room]

class DarkRoom(Room):
  def __init__(self, sDesc, lDesc):
    super().__init__(sDesc, lDesc)
    self.isDark= True
  def __str__(self):
    return self.sDesc

allDarkRooms: Set[DarkRoom]

class Obstacle:
  msg= {"stop": "You cannot go there"}
  def __init__(self, sDesc):
    self.sDesc= sDesc
    self.visible= True
    self.isOpen= False
  def __repr__(self):
    return self.sDesc

# these are just names to track states of the obstacles
isOpen=   True
isClosed= False
isDead=   True
isAlive=  False

# Base class for Door and Grate
class Gate(Obstacle):
  msg= {}
  msg.update (Obstacle.msg)
  msg["nokey"]= "You have no key!"
  msg["closing"]= "It slams shut and the lock catches."
  def __init__(self, sDesc):
    super().__init__(sDesc)

allGates: Set[Gate]

class Door(Gate):
  msg= {}
  msg.update (Gate.msg)
  msg["stop"]= "The door is closed and locked."
  msg["opening"]= "The door swings open wide."
  def __init__(self, sDesc="Door"):
    super().__init__(sDesc)

class Grate(Gate):
  msg= {}
  msg.update (Gate.msg)
  msg["stop"]= "The grate is closed and locked."
  msg["opening"]= "With a creak, the grate falls open."
  def __init__(self, sDesc="Grate"):
    super().__init__(sDesc)

class Monster(Obstacle):
  msg= {}
  msg.update (Obstacle.msg)
  msg["stop"]= "The monster blocks your way!"
  def __init__(self, originalPosition, sDesc, lDesc):
    super().__init__(sDesc)
    self.originalPosition= originalPosition
    self.lDesc= lDesc

allMonsters: Set[Monster]

class ActiveMonster:
  def __init__(self, sDesc, lDesc):
    self.sDesc= sDesc
    self.lDesc= lDesc

# Objects (objs): the things you can take/drop/use
# The constant data only
# the actual position of the objects is variable so
# it will be stored elsewhere
class Obj:
  def __init__(self, originalPosition, sDesc, lDesc):
    self.originalPosition= originalPosition
    self.sDesc= sDesc
    self.lDesc= lDesc
    self.isTreasure= False
  def __repr__(self):
    return self.sDesc

allObjs: Set[Obj]

class Treasure(Obj):
  def __init__(self, originalPosition, sDesc, lDesc):
    super().__init__(originalPosition, sDesc, lDesc)
    self.isTreasure= True

allTreasures: Set[Obj]

class Death:
  def __init__(self, sDesc, lDesc):
    self.sDesc= sDesc;
    self.lDesc= lDesc
  def __repr__(self):
    return self.sDesc
