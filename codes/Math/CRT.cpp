/*
Tested :
    https://open.kattis.com/problems/generalchineseremainder
    https://adalab.cs.nthu.edu.tw/contest/75/problem/W7-5 (https://adalab.cs.nthu.edu.tw/status/2ea6c14c9a0d8d8b260d699500c2fda8)
    https://qoj.ac/submission/1712391
Author :
    std_abs
*/
using ll = __int128;
//a * p.first + b * p.second = gcd(a, b)
pair<ll, ll> extgcd(ll a, ll b) {
    if (b == 0) return {1, 0};
    auto [y, x] = extgcd(b, a % b);
    return pair<ll, ll>(x, y - (a / b) * x);
}

pair<ll, ll> CRT(ll x1, ll m1, ll x2, ll m2) {
    ll g = __gcd(m1, m2);
    if ((x2 - x1) % g) return make_pair(-1, -1);// no sol
    m1 /= g, m2 /= g;
    pair<ll, ll> p = extgcd(m1, m2);
    ll lcm = m1 * m2 * g;
    ll res = p.first * (x2 - x1) * m1 + x1;
    // be careful with overflow
    return make_pair((res % lcm + lcm) % lcm, lcm);
}

pair<ll, ll> CRT_arr(vector<ll> &a, vector<ll> &m) {
    pair<ll, ll> res(a[0], m[0]);
    for (int i = 1; i < a.size(); ++i) {
        res = CRT(res.first, res.second, a[i], m[i]);
        if (res.first == -1) break;
    }
    return res;
}
