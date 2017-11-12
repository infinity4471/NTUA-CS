from random import randrange

def modexp( a, b, p ):
  if b == 0:
    return 1
  y = 1
  x = a
  n = b
  while n > 1:
    bit = n  % 2
    if bit == 0:
      x = ( x * x ) % p
      n = n / 2
    else:
      y = ( x * y ) % p
      x = ( x * x ) % p
      n = ( n - 1 ) / 2
  return ( x * y ) % p

def fermat( p ):
  if( p == 1 ):
    return False
  if( p == 2 ):
    return True
  a = randrange( 2, p - 2 )
  if( modexp( a, p - 1, p ) == 1 ):
    return True
  return False

n = input();
prime = True
for i in range(100):
  if( fermat( n ) == 0 ):
    prime = False
    break

print prime
