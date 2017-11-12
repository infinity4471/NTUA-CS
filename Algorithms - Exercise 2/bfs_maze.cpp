#include <cstdio>
#include <algorithm>
#include <vector>
#include <stack>
#include <queue>

#define MAXN 2100

#define X first
#define Y second
#define mp make_pair
#define pb push_back

using namespace std;

typedef pair< int, int > node;

int N, M, K, visited[ MAXN + 1 ][ MAXN + 1 ], adj[ MAXN + 1 ][ MAXN + 1 ]; // contains a bitmask, left 1, down 2, right 4, up 8

node par[ MAXN + 1 ][ MAXN + 1 ];

queue< node > Q;
stack< node > S;

void check( int x, int y, node p ) {
  if( visited[ x ][ y ] || x <= 0 || y <= 0 || x > N || y > M ) return;
  Q.push( mp( x, y ) );
  visited[ x ][ y ] = true;
  par[ x ][ y ] = p;
}

void bfs( node s ) {
  Q.push( s );
  par[ s.X ][ s.Y ] = mp( -1, -1 );

  while( !Q.empty() ) {
    node u = Q.front();
    Q.pop();

    int x = u.X, y = u.Y;

    if( adj[ x ][ y ] & 1 ) check( x, y - 1, u );
    if( adj[ x ][ y ] & 2 ) check( x + 1, y, u );
    if( adj[ x ][ y ] & 4 ) check( x, y + 1, u );
    if( adj[ x ][ y ] & 8 ) check( x - 1, y, u );
  }
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

  bfs( mp( u, v ) );

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
