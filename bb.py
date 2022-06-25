# Frank DaCosta: Writing BASIC adventure programs for the TRS-80
# Basement and Beasties

from typing import Set, Dict
from bb_types import *
from bb_static import *

# special name to represent the locations of objects hold by the player
Player= object()

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

def decodeDirection (strDir):
    if strDir=='UP':
        ret= 'up';
    elif strDir=='DOWN':
        ret= 'down'
    elif strDir=='NORTH' or strDir=='N':
        ret= 'n'
    elif strDir=='NORTHEAST' or strDir=='NE':
        ret= 'ne'
    elif strDir=='EAST' or strDir=='E':
        ret= 'e'
    elif strDir=='SOUTHEAST' or strDir=='SE':
        ret= 'se'
    elif strDir=='SOUTH' or strDir=='S':
        ret= 's'
    elif strDir=='SOUTHWEST' or strDir=='SW':
        ret= 'sw'
    elif strDir=='WEST' or strDir=='W':
        ret= 'w'
    elif strDir=='NORTHWEST' or strDir=='NW':
        ret= 'nw'
    else:
        ret= None
    return ret

'''
print ("=== Test: describeRoom(Office) ===")
ps.describeRoom(Office)
ps.describeRoom(which="Long")
ps.describeRoom(Ruins,"Short")
ps.describeRoom(SlimyCavern,"Long")
print(ps.monsterState)

print ("\n=== Test: enterRoom(Ruins) ===")
ps.enterRoom(Ruins)

print ("\n=== Test: enterRoom(BottomOfPit) ===")
ps.enterRoom(BottomOfPit)

print ("\n=== Test: enterRoom(Ruins) ===")
ps.enterRoom(Ruins)

print ("\n=== Test: enterRoom(Office) ===")
ps.enterRoom(Office)
'''

def xmove(ps, direction):
    print(ps.position.travel)
    moveto= ps.position.travel.get(direction)
    if moveto==None:
        print (Messages['NoWay'])
        return
    if isinstance(moveto, list):
        print ('Kezeletlen eset, itt egy lista:')
        for l in moveto:
            if isinstance(l, Gate):
                print('  Ajto/Racs:', l, type(l))
            elif isinstance(l, Monster):
                print('  Szorny:', l, type(l))
            elif isinstance(l, Room):
                print('  Szoba:', l, type(l))
        return
    ps.position= moveto
    ps.describeRoom(ps.position)

def executeCommand(ps, cmdline):
    cmdparts= cmdline.split()
    cmd= cmdparts[0].upper()
    cmdparts.pop(0)
#   print(cmd, cmdparts)

    # quit
    if cmd=='Q' or cmd=='QUIT' or cmd=='EXIT' or cmd=='BYE':
        exit(0)

    # move, with go/jump
    if cmd=='GO' or cmd=='JUMP':
        if cmdparts==[]:
            direction= 'default'
        else:
            strDir= cmdparts[0].upper()
            direction= decodeDirection(strDir)
            if direction==None:
                print(Messages['BadDirection']);
                return
        xmove(ps, direction)
        return

    # move, without go/jump
    direction= decodeDirection(cmd)
    if direction != None:
        xmove(ps, direction)
        return

    print(Messages['UnknownCommand'])

def main():
    ps= PlayerState()
    ps.describeRoom(ps.position)
    while True:
        cmdline= ''
        while cmdline=='':
            cmdline= input('* ')
        executeCommand(ps, cmdline)

main()
