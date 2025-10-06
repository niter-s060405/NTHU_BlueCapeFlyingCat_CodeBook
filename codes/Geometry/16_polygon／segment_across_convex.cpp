//  可以在有 n 個點的凸包內，用 O(log n) 判斷一個線段：
//  {1 : 存在一個凸包上的邊可以把這個線段切成兩半,
//   0 : 有碰到凸包但沒有任何凸包上的邊可以把它切成兩半,
//  -1 : 沒碰到凸包}
/// 除非線段兩端點都不在凸包邊上，否則此函數回傳 0 的時候不一定表示線段沒有通過凸包內部 ///
int segment_across_convex(line<T> L) {
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
            if (L.ori(v[x]) * L.ori(v[nxt]) >= 0) {
                x = nxt;
                now += i;
            }
        } //   ↓ BE CAREFUL
        return -(ori(v[x], v[(x + 1) % n], L.p1) * ori(v[x], v[(x + 1) % n], L.p2));
    };
    return max(g(i, j - i), g(j, n - (j - i)));
} 
