import sys

def build_fail(start):
    Q = []
    for c in [chr(code) for code in xrange(255)]:
        if start.goto[c] != start:
            Q.append(start.goto[c])
            start.goto[c].fail = start
    while len(Q) > 0:
        cur = Q.pop(0) # dequeuing
        for a in cur.goto:
            nextnode = cur.goto[a]
            Q.append(cur.goto[a])
            brd = cur.fail
            while a not in brd.goto:
                brd = brd.fail
            nextnode.fail = brd.goto[a]
            nextnode.occ.extend(nextnode.fail.occ)

def build_goto(start, patterns):
    for pat in patterns:
    cur = start
    j = 0
    while j < len(pat) and pat[j] in cur.goto:
        cur = cur.goto[pat[j]]
        j += 1
    while j < len(pat):
        newnode = node()
        cur.goto[pat[j]] = newnode
        cur = newnode
        j += 1
    cur.occ.append(pat)
    for asciicode in xrange(255):
        if chr(asciicode) not in start.goto:
            start.goto(chr(asciicode)) = start

def build_fsm(patterns):
    start = node()
    build_goto(start, patterns)
    build_fail(start)
    return start

def aho_corasick(txt, patterns):
    fsm = build_fsm(patterns)
    cur = start
    n = len(txt)
    occ = []
    for i in xrange(n):
        while txt[i] not in cur.goto:
            cur = cur.fail
        cur = cur.goto[txt[i]]
        for pat in cur.occ:
            occ.append((pat, i-len(pat)+1))
    return occ
        # cur = cur.goto[txt[i]]

def main():
    ftxt = open(sys.argv[1], "r")
    










