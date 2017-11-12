#include <cstdio>
#include <algorithm>

#define MAXN 310
#define INF 1100100

using namespace std;

int dp[ MAXN + 1 ][ MAXN + 1 ];

int main( void ) {
  for( int i = 0; i <= MAXN; i++ ) dp[ i ][ 1 ] = i;
  for( int n = 1; n <= MAXN; n++ ) {
    for( int k = 2; k <= MAXN; k++ ) {
      dp[ n ][ k ] = INF;
      for( int i = n; i >= 0; i-- ) {
        dp[ n ][ k ] = min( dp[ n ][ k ], 1 + max( dp[ n - i ][ k ], dp[ i - 1 ][ k - 1 ] ) );
      }
    }
  }
  for( int k = 1; k <= 25; k++ ) {
    for( int n = 1; n <= 25; n++ ) {
      printf("%d ", dp[ n ][ k ] );
    }
    printf("\n");
  }
  return 0;
}
