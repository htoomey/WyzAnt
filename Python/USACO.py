"""
NAME: Harold Toomey
TASK: USACO Automated Test Template
LANG: PYTHON2
"""

fin = open ('test.in', 'r')
fout = open ('test.out', 'w')
x,y = map(int, fin.readline().split())
sum = x + y
fout.write (str(sum) + '\n')
fout.close()
