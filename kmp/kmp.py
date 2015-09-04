import sys

def init_next(pat):
	m = len(pat)
	i = 0
	j = -1
	b = (m+1) * [-1]
	while i < m:
		while j >= 0 and pat[i] != pat[j]:
			j = b[j]
		i+=1
		j+=1
		b[i] = j
	return b
	
def kmp(txt, pat, b):
	occ = []
	n = len(txt)
	m = len(pat)
	i = 0
	j = 0
	while i < n:
		while j >= 0 and txt[i] != pat[j]:
			j = b[j]
		i+=1
		j+=1
		if j == m:
			print "pat found at index %s in txt" % (i-j)
			occ.append(i-j)
			j = b[j]
	return occ
	
def main():
	ftxt = open(sys.argv[1], "r")
	txt = ""
	for line in ftxt:
		txt = txt + line
	fpat = open(sys.argv[2], "r")
	for line in fpat:
		b = init_next(line)
		print "occ: ", kmp(txt,line,b)

if __name__ == "__main__":
	main()