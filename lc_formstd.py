import sys

# check for redundant brackets like (x), ((x)), (((x)))
# examples for invalid input: (), (x, (((x))

def redundantBracket(s):
    slen= len(s)
    i= 0
    depth= 0
    while i<slen and s[i]=='(':
        i += 1
        depth += 1
    if i+1 >= slen or depth==0 or s[i]==')' or s[i+1]!=')':
        return -1
    i += 1
    maxdepth= depth
    while i<slen and s[i]==')' and depth>0:
        i += 1
        depth -= 1
    if depth==0:
        return maxdepth
    else:
        return -1

def checkerSimplifier(s):
    res= ''
    depth= 0
    killset= set()
    prevchar= '('
    slen= len(s)
    for i in range(slen):
        c= s[i]
        if c==' ':
            pass
        elif c=='(':
            depth += 1
            if prevchar=='(':
#               print('\'%c\' after \'%c\' at %d depth=%d' %(c, prevchar, i, depth))
                killset.add(depth)
            elif redundantBracket(s[i:])>0:
                killset.add(depth)
            else:
                res += c
        elif c==')':
            if prevchar=='(':
                return None
            if depth==0:
                return None
            if depth in killset:
                killset.remove(depth)
#               print('\'%c\' at %d when depth=%d in killset' %(c, i, depth))
            else:
                res += c
            depth -= 1
        else:
            res += c
        prevchar= c
    if depth!=0:
        return None
    return res

# returns the length of last part in the expression,
# eg 'Bxy'='Bx'+'y' returns 1, 'B(xy)' returns 4
# Important: call 'checkerSimplifier' before and after 'lastpart'
# e.g: '((BC)(xy))' -> 'BC(xy)'    # checkSimplifier
#      'BC(xy)'     -> 'BC'+'(xy)' # lastPart
#      '(xy)'       ->  'xy'       # checkSimplifier
def lastPart(s):
    res= ''
    depth= 0
    slen= len(s)
    for i in range(slen-1, -1, -1):
        c= s[i]
        if c==')':
            depth += 1
        elif c=='(':
            depth -= 1
            if depth==0:
                return slen-i
        else:
            if depth==0:
                return slen-i
    return slen

def split(s, maxn):
    slen= len(s)
    n= 0
    result= []
    if maxn<1:
        return []
    while slen!=0 and maxn>n+1:
        len2= lastPart(s)
        len1= slen-len2
        tmp= checkerSimplifier(s[len1:slen])
        s= s[0:len1]
        slen= len1
        result.insert(0, tmp)
        n += 1
    if slen!=0 and max:
        result.insert(0, s)
        n += 1
    return result

# extractVarRec: it is given that 's' contains at least one 'c'

def extractVarRec(s, c):
    if s=='B'+c+'M': return 'L'  # special case
    parts= split(s, 2)
#   print('s=\'%s\' c=%s len(parts)=%d' %(s,s,len(parts)))
    if len(parts)==1: return 'I'
    # it is composite
    part1= parts[0]
    part2= parts[1]
    hasc1= (part1.find(c) >= 0)
    hasc2= (part2.find(c) >= 0)
#   print('p1=%s hasc1=%d, p2=%s hasc2=%d' %(part1, hasc1, part2, hasc2))
    if hasc1 and hasc2:
        if part1==c and part2==c:
            result= 'M'
        elif part1==c:
#           result= 'W(Q(' + extractVarRec(part2, c) + '))'
            result= 'O(' + extractVarRec(part2, c) + ')'     #Oyx = x(yx)
        elif part2==c:
            result= 'W(' + extractVarRec(part1, c) + ')'
        elif part1==part2:
            result= 'BM(' + extractVarRec(part1, c) + ')'
        else:
            result= 'S(' + extractVarRec(part1, c) + ')(' \
                         + extractVarRec(part2, c) + ')'
    elif hasc1 and (not hasc2):
        if part1==c:
            result= 'T(' + part2 + ')'
        else:
            result= 'C(' + extractVarRec(part1, c) + ')(' + part2 +')'
    elif (not hasc1) and hasc2:
        if part2==c:
            result= part1
        else:
            result= 'B(' + part1 + ')(' + extractVarRec(part2, c) + ')'
    else:
        result= 'K('+s+')'
    return checkerSimplifier(result)

def extractVar(s, clist):
    tmp= s
    clen= len(clist)
    for ic in range(clen-1, -1, -1):
        c= clist[ic]
#       print('extractVar(\'%s\',\'%c\')' %(tmp,c))
        if tmp==c:            tmp= 'I'
        elif tmp.find(c)==-1: tmp= checkerSimplifier('K('+tmp+')')
        else:                 tmp= extractVarRec(tmp, c)
    return tmp

def Test1(s, exp):
    result= checkerSimplifier(s)
    print('\'%s\' -> \'%s\'' %(s, result), end='')
    if result==exp: print(' ok', end='')
    else:           print(' exp=\'%s\'' %(exp), end='')
    print()

def Test2(s, exp):
    tmp= checkerSimplifier(s)
    lastlen= lastPart(tmp)
    print('lastPart(\'%s\')=%d' %(tmp, lastlen), end='')
    if lastlen==exp: print(' ok', end='')
    else:            print(' exp=\'%d\'' %exp, end='')
    print()

def Test3(s, maxn, exp):
    tmp= checkerSimplifier(s)
    parts= split(tmp, maxn)
    print('split(\'%s\',%s)=[%s]' %(tmp, maxn, ', '.join(parts)), end='')
    if parts==exp: print(' ok', end='')
    else:            print(' exp=[%s]' %(', '.join(exp)), end='')
    print()

def Test4(s, clist, exp):
    tmp= checkerSimplifier(s)
    result= extractVar(tmp, clist)
    print('Test4: \'%s\',\'%s\' = \'%s%s\'' %(tmp, clist, result, clist), end='')
    if exp!='?':
        if result==exp: print(' ok', end='')
        else:           print(' exp=\'%s%s\'' %(exp,clist), end='')
    print()

def main(argv):
    Test1('(B((Bx)))y', 'B(Bx)y')
    Test1('((SK)y)((z))', 'SKyz')
    Test1('((SK)y)((z))(((SK)y)((z)))', 'SKyz(SKyz)')
    Test1('((SK)y)((zw))', 'SKy(zw)')
    Test1('(SK)','SK')
    Test1('y((z))', 'yz')

    Test2('a(bc)', 4)
    Test2('ab', 1)
    Test2('a', 1)

    Test3('a',          1, ['a'])
    Test3('a',          2, ['a'])
    Test3('ab',         2, ['a', 'b'])
    Test3('a(bc)(def)', 1, ['a(bc)(def)'])
    Test3('a(bc)(def)', 2, ['a(bc)', 'def'])
    Test3('a(bc)(def)', 3, ['a', 'bc', 'def'])
    Test3('(a(bc))(def)', 3, ['a', 'bc', 'def'])
    Test3('a((bc)(def))', 3, ['a', 'bc(def)'])

    Test4('yx', 'x', 'y') # optimal: 'y'
    Test4('yx', 'y', 'Tx') # optimal: 'Tx'
    Test4('(Bx)(Cx)', 'x', 'SBC')
    Test4('x(yy)', 'y',  'BxM')
    Test4('x(yy)', 'xy', 'L')
    Test4('BxM', 'x', 'L')
    Test4('x(Tx)', 'x', 'OT')
    Test4('Txx', 'x', 'WT')
    Test4('T(xx)', 'x', 'BTM')
    Test4('Zyx(+(nx)(py))','y','?')
    Test4('Zyx(+(nx)(py))','xy','?')
    Test4('Zyx(+(nx)(py))','+xy','?')
    Test4('xyz','xyz','I')
    Test4('xzy','xyz','C')
    Test4('yxz','xyz','T')
    Test4('yzx','xyz','CC')
    Test4('zxy','xyz','BCT')
    Test4('zyx','xyz','C(BCT)')
    Test4('x', 'xy', 'K')
    Test4('y', 'xy', 'KI')
    Test4('x(yz)', 'xyz', '?')
    Test4('yzx', 'xyz', '?')
    Test4('y(xz)', 'xyz', 'Q')
    Test4('xyzy', 'xyz', '?')
    Test4('xz(yz)', 'yz', '?')
    Test4('xz(yz)', 'xyz', '?')
    Test4('x(Ax)', 'Ax', '?')
    Test4('x(Ax)', 'xA', '?')
    Test4('x', 'y', 'Kx')
    Test4('y', 'y', 'I')
    Test4('y', 'xy', 'KI')
    Test4('w(Auvu)(Auvv)','w', '?')
    Test4('w(Auvu)(Auvv)','vw', '?')
    Test4('w(Auvu)(Auvv)','uvw', '?')
    Test4('w(Auvu)(Auvv)','Auvw', '?')

main(sys.argv)
