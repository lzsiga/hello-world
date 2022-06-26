# Frank DaCosta: Writing BASIC adventure programs for the TRS-80
# Basements and Beasties

from typing import Set, Dict
from random import randrange
from bb_types import *
from bb_static import *

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
    for m in allMonsters:
      toRoom= m.originalPosition
      self.positions[m]= toRoom
      self.belongings[toRoom].add(m)

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
    if strDir=='UP' or strDir=='U':
        ret= 'up';
    elif strDir=='DOWN' or strDir=='D':
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
    ====================
    === decodeObject ===
    ====================
'''
def decodeObject(strObject):
    for o in allObjs:
        names= o.sDesc.split();
        for n in names:
            if strObject==n.upper():
                return o
    return None
'''
    ===================
    === decodeEnemy ===
    ===================
'''
def decodeEnemy(strMonster):
    for o in allEnemies:
        names= o.sDesc.split();
        for n in names:
            if strMonster==n.upper():
                return o
    return None
'''
    =====================
    === findEnemyHere ===
    =====================
'''
def findEnemyHere(ps):
    stuff= ps.belongings[ps.position]
    for s in stuff:
        if isinstance(s, Monster):
            return s
    return None
'''
    =============
    === xmove ===
    =============
'''
def xmove(ps, direction):
#   print(ps.position.travel)
    moveto= ps.position.travel.get(direction)
    if moveto==None:
        print (Messages['NoWay'])
        return
    if isinstance(moveto, list):
        for l in moveto:
            if isinstance(l, Gate):
                if not l.isOpen:
                    if isinstance(l, Grate):
                        print(Messages['ClosedGrate'])
                    else:
                        print(Messages['ClosedDoor'])
                    return
            elif isinstance(l, Monster):
                print('  Szorny:', l, type(l))
            elif isinstance(l, Room):
                print('  Szoba:', l, type(l))
        return
    ps.position= moveto
    ps.describeRoom(ps.position)
'''
    ==============
    === pickup ===
    ==============
'''
def pickup(ps, obj):
    if ps.positions[obj]==Player:
        print(Messages['AlreadyTaken'])
        return
    if ps.positions[obj]!=ps.position:
        print(Messages['ObjNotHere'])
        return
    if len(ps.belongings[Player]) >= MaxItems:
        print(Messages['HandsFull'])
        return

    ps.belongings[Player].add(obj)
    ps.positions[obj]= Player
    print(Messages['Taken'] % (obj.sDesc))
    ps.belongings[ps.position].remove(obj)
'''
    =================
    === inventory ===
    =================
'''
def inventory(ps):
    stuff= ps.belongings[Player]
    if len(stuff)==0:
        print(Messages['EmptyInventory'])
    else:
        print(Messages['Inventory'] % (len(stuff)))
        for s in stuff:
            print(' ', s.sDesc)
'''
    ============
    === drop ===
    ============
'''
def drop(ps, obj):
    if ps.positions[obj]==Player:
        print(Messages['AlreadyTaken'])
        return
    if ps.positions[obj]!=ps.position:
        print(Messages['ObjNotHere'])
        return
    if len(ps.belongings[Player]) >= MaxItems:
        print(Messages['HandsFull'])
        return

    ps.belongings[Player].add(obj)
    ps.positions[obj]= Player
    print(Messages['Taken'] % (obj.sDesc))
    ps.belongings[ps.position].remove(obj)
'''
    ============
    === dead ===
    ============
'''
def dead(ps):
    print('Well, fine adventurer! You are in a real jam! Fortunately, we can bring you back!')
    print('...POOF!!...')
    stuff= ps.belongings[Player]
    for s in stuff:
        if s==Torch:
            toRoom= Ruins
        else:
            toRoom= ps.position
        ps.positions[s]= toRoom
        ps.belongings[Player].remove(s)
        ps.belongings[toRoom].add(s)
    ps.describeRoom(ps.position)
'''
    =====================
    === counterAttack ===
    =====================
'''
def counterAttack(ps,enemy):
    print('The hideous monster leaps at your throat!')
    if randrange(0,100) < 30:
        print('It finishes you off!!')
        dead(ps)
    else:
        print('Somehow you fend it off!')
'''
    =============
    === fight ===
    =============
'''
def fight(ps,enemy):
    if enemy==None or ps.monsterState[enemy]==isDead:
        print('Save your stamina, turkey! I see no real threat!')
        return
    if ps.positions[Axe] != Player:
        print('With what weapon?')
        return
    if enemy==Spider or enemy==Terror:
        print('Your axe swings are dynamic... but ineffective!')
        counterAttack(ps, enemy)
    else:
        if randrange(0,100) < 70:
            print('Your magic axe connects! The creature vanishes in a puff of foul smoke!')
            ps.positions[enemy]= Void
            ps.monsterState[enemy]= isDead
            ps.belongings[ps.position].remove(enemy)
        else:
            print('Missed it! FIE!')
            counterAttack(ps, enemy)
'''
    ============
    === bomb ===
    ============
'''
def bomb(ps):
    if ps.positions[Grenade] != Player:
        print('You have no bomb!')
        return
    if ps.positions[Spider] == ps.position:
        enemy= Spider
    elif ps.position[Terror] == ps.position:
        enemy= Terror
    else:
        enemy= None
    if enemy != None:
        print('The grenade explodes in a silent flash of weird blue light...'\
              'and the creature is gone!')
        ps.positions[enemy]= Void
        ps.monsterState[enemy]= isDead
    else:
        print('The grenade falls to the floor and nothing happens.')
    ps.positions[Grenade]= ps.position
    ps.position.belongings.add(Grenade)
    ps.belongings[Player].remove(Grenade)
'''
    ======================
    === executeCommand ===
    ======================
'''
def executeCommand(ps, cmdline):
    cmdparts= cmdline.split()
    cmd= cmdparts[0].upper()
    cmdparts.pop(0)
#   print(cmd, cmdparts)

    # quit
    if cmd=='Q' or cmd=='QUIT' or cmd=='EXIT' or cmd=='BYE':
        print('Do visit the Basement again!')
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

    if cmd=='PICK' or cmd=='PICKUP' or cmd=='GET' or cmd=='TAKE' or cmd=='STEAL':
        if cmdparts==[]:
            print(Messages['NoObject'])
            return
        else:
            strObject= cmdparts[0].upper()
            obj= decodeObject(strObject)
            if obj!=None:
                pickup(ps, obj)
                return
            enemy= decodeEnemy(strObject)
            if enemy!=None:
                print(Messages['PickupMonster'])
                return
        return

    if cmd=='INVENTORY' or cmd=='LIST':
        inventory(ps)
        return

    if cmd=='FIGHT' or cmd=='ATTACK':
        if cmdparts==[]:
            print('Talaljuk ki, melyik szorny van itt: ', ps.position)
            enemy= findEnemyHere(ps)
#           print('Ez lenne az:', enemy)
        else:
            strEnemy= cmdparts[0].upper()
            enemy= decodeEnemy(strEnemy)
#           print('Ezzel akarsz harcolni: ', enemy)
        fight(ps, enemy)
        return

    if cmd=='BOMB' or cmd=='BLOW':
       bomb(ps)
       return

    if cmd=='READ':
        if ps.position==OracleRoom:
            print('The danger here is pretty thick, but say <AARDVARK> you\'ll get out quick!')
        else:
            print('Nothing here to read... how dull!')
        return

    if cmd=='SAY':
        if cmdparts!=[] and cmdparts[0].upper()=='AARDVARK' and\
           ps.position in (BottomOfPit, OracleRoom):
            if ps.position==BottomOfPit:
                ps.position= OracleRoom
            elif ps.position==OracleRoom:
                ps.position= BottomOfPit
            ps.describeRoom(ps.position)
        else:
            print('Nothing happens.')
        return

    if cmd=='HELP':
        print('Your cries go unheard, pitiful wretch.')
        return

    if cmd=='WAIT':
        print('Time passes...')
        return

    print(Messages['UnknownCommand'])
'''
============
=== main ===
============
'''
def main():
    ps= PlayerState()
    ps.describeRoom(ps.position)
    while True:
        cmdline= ''
        while cmdline=='':
            cmdline= input("\n* ")
        print('')
        executeCommand(ps, cmdline)

main()
