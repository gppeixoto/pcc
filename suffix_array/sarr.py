import math

def print_sarr(txt, sa):
    n = len(txt)
    print 'i', '\tSA[i]', '\ti-esimo sufixo'
    print '-'*25
    for i in range(n):
        print i, '\t', sa[i], '\t', txt[sa[i]:]

def build_sarr_naive(txt):
    n = len(txt)
    suffs = [(txt[i:], i) for i in range(n)]
    suffs.sort()
    print suffs
    return [sp[1] for sp in suffs]

def scmp(x, y, m):
    lx = len(x)
    ly = len(y)
    i = 0
    while i < lx and i < ly and i < m and x[i]==y[i]:
        i += 1
    if i == m:
        return 0
    elif i == lx:
        return 0 if i == ly else -1
    elif i == ly:
        return 1
    else:
        return -1 if x[i] < y[i] else 1

def succ(txt, pat, sarr):
    print 'succ'
    n = len(txt)
    m = len(pat)
    if scmp(pat, txt[sarr[0]:], m) <= 0:
        return 0
    elif scmp(pat, txt[sarr[n-1]:], n) > 0:
        return n
    else:
        l = 0
        r = n-1
        while r-l>1:
            h = (l+r)/2
            print pat, txt[sarr[h]:], scmp(pat, txt[sarr[h]:], m)
            if scmp(pat, txt[sarr[h]:], m) < 0:
                r = h
            else:
                l = h
        return r

def pred(txt, pat, sarr):
    print 'pred'
    n = len(txt)
    m = len(pat)
    if scmp(pat, txt[sarr[n-1]:], m) >= 0:
        return n-1
    elif scmp(pat, txt[sarr[0]:], m) < 0:
        return -1
    else:
        l = 0
        r = n-1
        while r-l>1:
            h = (l+r)/2
            print pat, txt[sarr[h]:], scmp(pat, txt[sarr[h]:], m)
            if scmp(pat, txt[sarr[h]:], m) < 0:
                r = h
            else:
                l = h
        return l

def match(txt, pat, sarr):
    p = pred(txt, pat, sarr)
    s = succ(txt, pat, sarr)
    print 'p: %d, s+1: %d' % (p, s+1)
    return sorted([sarr[i] for i in range(s, p+1)])


def main():
    txt = "baobab"
    sarr = build_sarr_naive(txt)
    print_sarr(txt, sarr)
    print 'succ(%s) = %d\n' % ("ba", succ(txt, 'ba', sarr))
    print 'pred(%s) = %d\n' % ("ba", pred(txt, 'ba', sarr))
    
    print '-'*10, '\nmatch\n', match(txt, 'ba', sarr)

def fast_build_sarr(txt):
    n = len(txt)
    l = int(math.ceil(math.log(n, 2)))

    v = zip(txt, range(n))
    v = sorted(v, key=lambda x:x[0])
    p = n*[-1]
    k = 0
    for i in range(n):
        if i > 0 and v[i][0] != v[i-1][0]:
            k += 1
        p[v[i][1]] = k
    
    for k in range(l):
        j = 2*k
        v = n*[(-1,-1)]
        for i in range(n):
            if i+j < n:
                v[i] = (p[i], p[i+j], i)
            else:
                v[i] = (p[i], -1, i)
        v = sorted(v)
        k = 0
        for i in range(n):
            if i > 0 and v[i][0:2] != v[i-1][0:2]:
                k += 1
            p[v[i][2]] = k
    return p


if __name__ == '__main__':
    main()
