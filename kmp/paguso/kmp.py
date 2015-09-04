import sys
verbose = 1

def brute_force(txt, pat):
    n = len(txt)
    m = len(pat)
    # assumption: n > m
    i = 0 # store window current position
    occ = []
    while i < n-m+1:
        if verbose:
            print "    %si=%d" % (i*" ", i)
            print "T: %s" % txt
        j = 0 # store how many matches until current
        while j < m and txt[i+j]==pat[j]:
            j += 1
        if verbose:
            print "   %s%s%s" % (i*" ", (j)*"=", "" if (j==m) else "!")
            print "P: %s%s" % (i*" ", pat)
            print "   %sj=%d" % (i*" ", i)
        if j==m: #match starting in i
            occ.append(i)
        i += 1
    return occ

def init_next(pat):
    """
    returns table with strict borders for each prefix length
    from prefix[:0]="" until prefix[:]=pat
    """
    m = len(pat)
    B = (m+1)*[-1]
    if m==1 or (m > 1 and pat[0]!=pat[1]):
        B[1] = 0
    i = 1
    j = 0
    while i < m:
        while i+j < m and pat[j]==pat[i+j]:
            j += 1
            if i+j==m or (pat[i+j]!=pat[j]): #strict border
                B[i+j] = j
            else:
                B[i+j]=B[j]
        if B[j]==-1 and pat[0]!=pat[i+j]:
            B[i+j]=0
        i += j-B[j]
        j = max(0, B[j])
    return B

def sbrd(pat, j):
    m = len(pat)
    l = j-1
    while l >= 0:
        k = 0 # how many matches
        while k < l and pat[k]==pat[j-l+k]:
            k += 1
        if k==l and (j==m or pat[k]!=pat[j]): #strict border
            return l
        l -= 1
    return -1

def kmp(txt, pat):
    nxt = init_next(pat)
    n = len(txt)
    m = len(pat)
    # assumption: n > m
    i = 0 # store window current position
    occ = []
    while i < n-m+1:
        if verbose:
            print "    %si=%d" % (i*" ", i)
            print "T: %s" % txt
        j = 0 # store how many matches until current
        while j < m and txt[i+j]==pat[j]:
            j += 1
        if verbose:
            print "   %s%s%s" % (i*" ", (j)*"=", "" if (j==m) else "!")
            print "P: %s%s" % (i*" ", pat)
            print "   %sj=%d" % (i*" ", i)
        if j==m: #match starting in i
            occ.append(i)
        sb = sbrd(pat, j)
        assert(sb==nxt[j])
        if verbose:
            print "   %ssb=%d" % (i*" ", sb)
        i += j - sb
        j = max(0, sb)
    return occ


def main():
    with open(sys.argv[1], "r") as ftxt:
        txt = ""
        for line in ftxt:
            txt += line
        print "TXT=\"%s\"" % txt
    with open(sys.argv[2], "r") as fpat:
        for pat in fpat:
            print "PAT=\"%s\"" % pat 
            print "OCC={0}".format(str(kmp(txt, pat)))
            
if __name__ == "__main__":
    main()





