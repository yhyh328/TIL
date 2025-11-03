n, m = map(int, input().split())

x, y = input().split()

z = []

memo = [3, 2, 1, 2, 4, 
        3, 1, 3, 1, 1, 
        3, 1, 3, 2, 1, 
        2, 2, 2, 1, 2, 
        1, 1, 1, 2, 2, 1]

i = 0

while (i < n and i < m):
    z.append(memo[ord(x[i]) - 65])
    z.append(memo[ord(y[i]) - 65])
    i += 1

while (i < n):
    z.append(memo[ord(x[i]) - 65])
    i += 1

while (i < m):
    z.append(memo[ord(y[i]) - 65])
    i += 1

while (len(z) != 2):
    t = []
    for j in range(len(z) - 1):
        a = z[j]
        b = z[j + 1]
        t.append((a + b) % 10)
    z = t    

ans = str(z[1]) + "%"

if (z[0] != 0):
    ans = str(z[0]) + ans

print(ans)