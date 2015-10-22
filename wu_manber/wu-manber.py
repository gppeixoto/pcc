def char_mask(pat, alphabet):
	ret = {}
	for c in alphabet:
		ret[c] = bitarray(m*'1')
	for i in range(m):
		ret[pat[i]][m-i-1] = 0
	return ret


def scan(txt, pat, err, alphabet):
	n = len(txt)
	m = len(pat)
	occ = []
	chmasks = char_mask(pat, alphabet)
	print chmasks
	S = [bitarray((m-q) * '1' + q*'0') for q in range(0,err+1)]
	for j in range(n):
		chmsk = chmasks[txt[j]]
		Sjminusoneqminusone = S[0]
		S[0] = (S[0] << 1) | chmsk
		for q in range(1,err+1):
			Sjminusoneq = S[q]
			S[q] = shift(S[q] | chmsk)\
					& shift(Sjminusoneqminusone)\
					& shift(S[q-1])\
					& Sjminusoneqminusone
			Sjminusoneqminusone = Sjminusoneq
			if not S[0][0]:#bit mais significativo for 0
				occ.append(j)

	return occ


def main():
	txt = "abadac"
	pat = "cada"
	err = 2
	alphabet = "abcd"
	occ = scan(txt, pat, err, alphabet)
	print occ

if __name__ == '__main__':
	main()