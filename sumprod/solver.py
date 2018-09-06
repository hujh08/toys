#!/usr/bin/env python3

# solve sum and product question, see readme for details

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

# calculate possible sum and product
summs={}
prods={}
for a in range(mini, maxi+1):
	for b in range(a, maxi+1):
		pr=(a, b)
		s=a+b
		p=a*b
		if s not in summs:
			summs[s]=[]
		if p not in prods:
			prods[p]=[]
		summs[s].append(pr)
		prods[p].append(pr)
print('possible sum and prod:')
print('    number of summs:', len(summs))
print('    number of prods:', len(prods))
print()

# handle dialogue
'''
1st sentence
the sum can't be sum of two integers
		whose product can't decomposed uniquely
'''
for p, pairs in list(prods.items()):
	# print(p, len(pairs))
	if len(pairs)==1:
		s=sum(pairs[0])
		if s in summs:
			del summs[s]
		del prods[p]
print('after 1st sentence:')
print('    number of summs:', len(summs))
print('    number of prods:', len(prods))
print()

'''
2nd sentence
after knowing possible sums, only one decomp of prod is possible
'''
ps0=prods
ss0=summs
prods={}
summs={}
for p, pairs in ps0.items():
	decomps=[pr for pr  in pairs if sum(pr) in ss0]
	if len(decomps)==1:
		prods[p]=decomps
		s=sum(decomps[0])
		if s not in summs:
			summs[s]=[]
		summs[s].append(decomps[0])
print('after 2nd sentence:')
print('    number of summs:', len(summs))
print('    number of prods:', len(prods))
print()

'''
2nd sentence
sum have unique decomposition
'''
integers=[]
for s, pairs in summs.items():
	if len(pairs)==1:
		integers.append(pairs[0])
print('possible integers:', integers)
