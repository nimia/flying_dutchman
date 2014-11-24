f = open("/localwork/dijkstra/protein.links.v9.1.txt")
dest = open("/localwork/dijkstra/proteins_out", "w")
title_line = f.readline()
next_node_id = 1

def factory():
	global next_node_id
	result = next_node_id
	next_node_id += 1
	return result

from collections import defaultdict
d = defaultdict(factory)

i = 0
for line in f:
    l = line.split()
    dest.write(str(d[l[0]]) + ' ' + str(d[l[1]]) + ' ' + l[2] + '\n')
    i += 1
    if i % 1000000 == 0:
        print i

dest.close()
f.close()

