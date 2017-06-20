import scipy as sp
import scipy.stats

text_file1 = open("temp4.txt", "r")
text_file2 = open("temp5.txt", "r")
IDs1 = text_file1.read().split('\n')
IDs2 = text_file2.read().split('\n')
IDs1.pop()
IDs2.pop()
IDs1 = map(int, IDs1)
IDs2 = map(int, IDs2)



tau, p = sp.stats.kendalltau(IDs1, IDs2)

f = open( 'temp3.txt', 'w' )
f.write( str(tau) )
f.close()

