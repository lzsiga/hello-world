# Frank DaCosta: Writing BASIC adventure programs for the TRS-80
# Basements and Beasties

from typing import Set, Dict
from random import randrange
from bb_types import *
from bb_static import *

# variable data
class PlayerState:
  def __init__(ps):
    ps.reset()

  def reset(ps):
    ps.visitedRooms= set()
    ps.ownedObjs= set()
    ps.position= BottomOfPit
  # reset state of the inanimated obstacles
    ps.resetGates()
    ps.resetMonsters()
    ps.resetPositions()
    ps.killedStatic= 0
    ps.killedActive= 0

  def resetMonsters(ps):
    ps.monsterState= {}
    for m in allMonsters:
       ps.monsterState[m]= isAlive
    ps.monsterState[Orc]= isDead

  def resetGates(ps):
    ps.gateState= {}
    for g in allGates:
       ps.gateState[g]= isClosed

  # the player and the rooms can have belongings,
  # the monsters cannot (for now)
  # now everyone loses everything
  # then every object goes to its original place
  def resetPositions(ps):
    ps.belongings: Dict[object, Set] = dict()
    ps.belongings[Player]= set()
    for r in allRooms:
       ps.belongings[r]= set()
    ps.positions= dict()
    for o in allObjs:
      toRoom= o.originalPosition
      ps.positions[o]= toRoom
      ps.belongings[toRoom].add(o)
    for m in allMonsters:
      toRoom= m.originalPosition
      ps.positions[m]= toRoom
      ps.belongings[toRoom].add(m)

  def describeRoom(ps, room="Default", which="Default"):
    if room=="Default": room= ps.position
    if which=="Default":
      if room in ps.visitedRooms: which= "Short"
      else:                       which= "Long"
    if which=="Short":  print (room.sDesc)
    elif which=="Long": print (room.lDesc)
    else:               print ("Serious problem in method PlayerState.describeRoom")
    ps.listObjsInRoom(room)

  def listObjsInRoom(ps, room="Default", separator=True):
    if room=="Default": room= ps.positions[Player];
    if len(ps.belongings[room])==0: return
    if (separator): print()
    for o in ps.belongings[room]:
      print (o.lDesc)

  def enterRoom(ps, intoRoom):
    ps.position= intoRoom
    if intoRoom.isDark and ps.positions[Torch]!=intoRoom and\
       ps.positions[Torch]!=Player:
        print ("It is pitch dark! You may fall into a pit!")
        # random death
    else:
        ps.describeRoom(intoRoom)
        if not intoRoom in ps.visitedRooms:
            ps.visitedRooms.add(intoRoom)

#    =================
#    === getObject ===
#    =================
#    give the Object to the Player removing it from
#    place where it is (should be the room the Player is now)
#    no checks here

  def getObject(ps, obj):
    fromRoom= ps.positions[obj]
    ps.belongings[Player].add(obj)
    ps.belongings[fromRoom].remove(obj)
    ps.positions[obj]= Player

#    =================
#    === putObject ===
#    =================
#    put the Object from the Player to the specified room
#    or the Room where to Player is now
#    no checks here
#    Note: 'toRoom' is used when the Player dies and
#    we put the Torch back to the Ruins

  def putObject(ps, obj, toRoom='Default'):
    if toRoom=='Default':
        toRoom= ps.position
    fromRoom= ps.positions[obj]
    ps.belongings[Player].remove(obj)
    ps.belongings[toRoom].add(obj)
    ps.positions[obj]= toRoom
  '''
    =================
    === deadEnemy ===
    =================
    The Orc is a special case:
    it will be resurrected sooner or later
  '''
  def deadEnemy(ps, enemy):
    ps.positions[enemy]= Void
    ps.monsterState[enemy]= isDead
    ps.belongings[ps.position].remove(enemy)
    if enemy==Orc:
        ps.killedStatic += 1
    else:
        ps.killedActive += 1

'''
    =======================
    === decodeDirection ===
    =======================
'''
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
        mvlist= moveto
        for l in mvlist:
            if isinstance(l, Gate):
                if ps.gateState[l]==isClosed:
                    if isinstance(l, Grate):
                        print(Messages['ClosedGrate'])
                    else:
                        print(Messages['ClosedDoor'])
                    return
            elif isinstance(l, Monster):
                if ps.monsterState[l]==isAlive:
                    print('The creature will not let your pass!')
                    return
            elif isinstance(l, Room):
                moveto= l
                break
    ps.position= moveto
    ps.enterRoom(ps.position)
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
    ps.getObject(obj)
    print(Messages['Taken'] % (obj.sDesc))
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
        print(Messages['Inventory'])
        for s in stuff:
            print(' ', s.sDesc)
'''
    ============
    === drop ===
    ============
'''
def drop(ps, obj):
    if ps.positions[obj]!=Player:
        print(Messages['DontHaveIt'])
        return
    if (obj==Grenade):
        bomb(ps)
        return
    ps.putObject(obj)
    print(Messages['Dropped'] % (obj.sDesc))
'''
    ============
    === dead ===
    ============
'''
def dead(ps):
    print('Well, fine adventurer! You are in a real jam! Fortunately, we can bring you back!')
    print("...POOF!!...\n")
# 'copy' is important here (cf RuntimeError: Set changed size during iteration)
    stuff= ps.belongings[Player].copy()
    for s in stuff:
        if s==Torch:
            toRoom= Ruins
        else:
            toRoom= ps.position
        ps.putObject(s, toRoom)
    ps.position= BottomOfPit
    ps.enterRoom(ps.position)
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

    print(Messages['FightIt'] % (enemy))

    if enemy==Spider or enemy==Terror:
        print('Your axe swings are dynamic... but ineffective!')
        counterAttack(ps, enemy)
    else:
        if randrange(0,100) < 70:
            print('Your magic axe connects! The creature vanishes in a puff of foul smoke!')
            ps.deadEnemy(enemy)
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
    elif ps.positions[Terror] == ps.position:
        enemy= Terror
    else:
        enemy= None
    if enemy != None:
        print('The grenade explodes in a silent flash of weird blue light...'\
              'and the creature is gone!')
        ps.deadEnemy(enemy)
    else:
        print('The grenade falls to the floor and nothing happens.')
    ps.putObject(Grenade)
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
            print('Pickup what?!')
        return

    if cmd=='DROP' or cmd=='THROW':
        if cmdparts==[]:
            print(Messages['NoObject'])
            return
        else:
            strObject= cmdparts[0].upper()
            obj= decodeObject(strObject)
            if obj!=None:
                drop(ps, obj)
                return
            print(Messages['DontHaveIt'])
            return
        return

    if cmd=='INVENTORY' or cmd=='LIST':
        inventory(ps)
        return

    if cmd=='LOOK':
        ps.describeRoom(which='Long')
        return

    if cmd=='FIGHT' or cmd=='ATTACK':
        if cmdparts==[]:
            enemy= findEnemyHere(ps)
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
            ps.enterRoom(ps.position)
        else:
            print('Nothing happens.')
        return

    if cmd=='HELP':
        print('Your cries go unheard, pitiful wretch.')
        return

    if cmd=='WAIT':
        print('Time passes...')
        return

    if cmd=='OPEN' or cmd=='UNLOCK':
        gate= ps.position.gate
        if gate==None:
            print('There is no gate.')
            return
        if cmdparts != [] and cmdparts[0].upper() != gate.sDesc.upper():
            print('I see nothing of the sort here.')
            return
        if ps.gateState[gate]==isOpen:
            print('You don\'t need to.')
            return
        if not ps.positions[Key] in [Player, ps.position]:
            print('You have no key!')
            return
        print('The gate is open!')
        ps.gateState[gate]= isOpen
        return

    if cmd=='CLOSE' or cmd=='LOCK':
        gate= ps.position.gate
        if gate==None:
            print('There is no gate.')
            return
        if cmdparts != [] and cmdparts[0].upper() != gate.sDesc.upper():
            print('I see nothing of the sort here.')
            return
        if ps.gateState[gate]==isClosed:
            print('You don\'t need to.')
            return
        print('The gate is closed and locked!')
        ps.gateState[gate]= isClosed
        return

    if cmd=='SCORE':
        score= 5*len(ps.visitedRooms) + 20*ps.killedStatic + 25*ps.killedActive
        for t in allTreasures:
            if ps.positions[t]==BottomOfPit:
                score += 10
        print('Your score is:', score)
        return

    print(Messages['UnknownCommand'])
'''
============
=== main ===
============
'''
def main():
    ps= PlayerState()
    ps.enterRoom(ps.position)
    while True:
        cmdline= ''
        while cmdline=='':
            cmdline= input("\n* ")
        print('')
        executeCommand(ps, cmdline)

main()
