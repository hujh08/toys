#!/usr/bin/env python3

# find out all the primes in a range

mini=2        # minium number
maxi=100      # maxium number

import sys
argv=sys.argv[1:]
if len(argv)==1:
	maxi=int(argv[0])
elif len(argv)>1:
	mini=int(argv[0])
	maxi=int(argv[1])

if mini<=1:
	raise Exception('minium number must be larger than 1')

print('range:', [mini, maxi])
print()

compos=set()  # composite number
primes=set()
n=2
while n<=maxi:
	if n>=mini:
		primes.add(n)
	m=2*n
	while m<=maxi:
		compos.add(m)
		m+=n

	# next prime
	n=n+1
	while n in compos:
		n=n+1
primes_sorted=sorted(primes)
print("primes:", primes_sorted)
