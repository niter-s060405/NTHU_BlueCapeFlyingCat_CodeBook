//  可以在有 n 個點的凸包內，用 O(log n) 判斷一個線段：
//  {1 : 線段上存在某一點位於凸包內部（邊上不算）,
//   0 : 線段上存在某一點碰到凸包的邊但線段上任一點均不在凸包內部,
//  -1 : 線段完全在凸包外面}
int segment_pass_convex_interior(line<T> L) {
    if (in_convex(L.p1) == 1 || in_convex(L.p2) == 1) return 1;
    L.build();
    point<T> p(L.a, L.b);
    auto gt = [&](int neg) {
        auto f = [&](int x, int y) {
            return sign((v[x] - v[y]) * p) == neg;
        };
        return cycle_search(f);
    };
    int i = gt(1), j = gt(-1), n = v.size();
    T x = -(v[i] * p), y = -(v[j] * p);
    if (L.c < x || y < L.c) return -1;
    if (L.c == x || L.c == y) return 0;

    if (i > j) swap(i, j);
    auto g = [&](int x, int lim) {
        int now = 0, nxt;
        for (int i = 1 << __lg(lim); i > 0; i /= 2) {
            if (now + i > lim) continue;
            nxt = (x + i) % n;
            if (L.ori(v[x]) * L.ori(v[nxt]) > 0) {
                x = nxt;
                now += i;
            }
        } //   ↓ BE CAREFUL
        return -(ori(v[x], v[(x + 1) % n], L.p1) * ori(v[x], v[(x + 1) % n], L.p2));
    };
    int ret = max(g(i, j - i), g(j, n - (j - i)));
    return (ret == 0) ? (in_convex(L.p1) == 0 && in_convex(L.p2) == 0) : ret;
} 
