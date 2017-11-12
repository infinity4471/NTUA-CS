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

def miller_rabin( p ):
  if p == 1:
      return False
  if p == 2 or p == 3 or p == 5:
    return True
  phi = p - 1
  d = phi
  r = 0

  while d % 2 == 0:
    d /= 2
    r += 1
  for i in range( 50 ):
    a = randrange( 2, p - 2 )
    exp = modexp( a, d, p )

    if exp == 1 or exp == p - 1:
      continue
    for j in range( r - 1 ):
      exp = pow( exp, 2, p )

      if exp == 1:
        return False
      if exp == p - 1:
        break
    else:
      return False
  return True

p = input()
print miller_rabin( p )
