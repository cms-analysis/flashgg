import sys

card = open(sys.argv[1])

for line in card.readlines():
  if line.startswith('rate'):
    els=line.split()
    print els[0],
    els = els[1:]
    for i, el in enumerate(els):
      if i%6==6:
        rate = 0.9935*float(el)
      elif i%6==7:
        rate = 0.99*float(el)
      else:
        rate = float(el)
      print rate,
    print '\n'
  else:
    print line


