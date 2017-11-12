#include <cstdio>
#include <algorithm>
#include <vector>
#include <stack>

#define MAXN 2100

#define X first
#define Y second
#define mp make_pair
#define pb push_back

using namespace std;

typedef pair< int, int > node;

int N, M, K, visited[ MAXN + 1 ][ MAXN + 1 ], adj[ MAXN + 1 ][ MAXN + 1 ]; // contains a bitmask, left 1, down 2, right 4, up 8

node par[ MAXN + 1 ][ MAXN + 1 ];

stack< node > S;

void dfs( node p, node u ) {
  if( visited[ u.X ][ u.Y ] || u.X <= 0 || u.Y <= 0 || u.X > N || u.Y > M ) return;

  par[ u.X ][ u.Y ] = p;
  visited[ u.X ][ u.Y ] = true;
  int bitmask = adj[ u.X ][ u.Y ];

  if( bitmask & 1 ) dfs( u, mp( u.X, u.Y - 1 ) );
  if( bitmask & 2 ) dfs( u, mp( u.X + 1, u.Y ) );
  if( bitmask & 4 ) dfs( u, mp( u.X, u.Y + 1 ) );
  if( bitmask & 8 ) dfs( u, mp( u.X - 1, u.Y ) );

}

int main( void ) {
  scanf("%d%d%d", &N, &M, &K );

  for( int i = 1; i <= N; i++ ) {
    for( int j = 1; j <= M; j++ ) {
      adj[ i ][ j ] = 15;
    }
  }

  for( int i = 1; i <= K; i++ ) {
    char c;
    int u, v;
    scanf("\n%c%d%d", &c, &u, &v );
    if( c == 'V' ) {
      adj[ u ][ v ] -= 4;
      adj[ u ][ v + 1 ] -= 1;
    } else {
      adj[ u ][ v ] -= 2;
      adj[ u + 1 ][ v ] -= 8;
    }
  }

  int u, v;
  scanf("%d%d", &u, &v );
  dfs( mp( -1, -1 ), mp( u, v ) );
  scanf("%d%d", &u, &v );
  if( visited[ u ][ v ] ) {
    printf("YES\n");
    node t = mp( u, v );
    while( t != mp( -1, -1 ) ) {
      S.push( t );
      t = par[ t.X ][ t.Y ];
    }
    while( !S.empty() ) {
      t = S.top();
      printf("%d %d\n", t.X, t.Y );
      S.pop();
    }
  } else {
    printf("NO\n");
  }
  return 0;
}
