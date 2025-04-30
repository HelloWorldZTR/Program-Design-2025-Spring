#include <stdio.h>
#include <memory.h>

#define N 1000000

int parent[N];
int lchild[N], rchild[N];
int t;
int n;
int m;

int main() {
    scanf("%d", &t);
    while(t--) {
        // memset(parent, -1, sizeof(parent));
        memset(lchild, -1, sizeof(lchild));
        memset(rchild, -1, sizeof(rchild));
        scanf("%d %d", &n, &m);
        int x, y, z;
        for(int i=0; i<n; i++) {
            scanf("%d %d %d", &x, &y, &z);
            parent[y] = parent[z] = x;
            lchild[x] = y;
            rchild[x] = z;
        }
        for(int i=0; i<m; i++) {
            int op;
            scanf("%d", &op);
            if (op == 1) {
                scanf("%d %d", &x, &y);
                int px = parent[x], py = parent[y];
                // Swap x and y from their parents
                if (px == py) {// x y has the same parent
                    int t = lchild[px];
                    lchild[px] = rchild[py];
                    rchild[py] = t;
                }
                else { // x y has different parents
                    if (lchild[px] == x) {
                        lchild[px] = y;
                    } else {
                        rchild[px] = y;
                    }
                    if (lchild[py] == y) {
                        lchild[py] = x;
                    } else {
                        rchild[py] = x;
                    }
                }
                parent[x] = py;
                parent[y] = px;
                // Update thier children's parent
                // parent[lchild[x]] = y;
                // parent[rchild[x]] = y;
                // parent[lchild[y]] = x;
                // parent[rchild[y]] = x;

            } else if (op == 2) {
                int q;
                scanf("%d", &q);
                while(lchild[q] != -1) {
                    q = lchild[q];
                }
                printf("%d\n", q);
            }
        }
    }
    return 0;
}