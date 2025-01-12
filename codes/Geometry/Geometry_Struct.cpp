// 判斷數值正負：{1:正數,0:零,-1:負數}
int sign(long long x) {return (x >= 0) ? ((bool)x) : -1; }
int sign(double x) {
    return (abs(x) < 1e-9) ? 0 : (x > 0 ? 1 : -1);
}

template<typename T>
struct point {
    T x, y;
    point() {}
    point(const T &x, const T &y) : x(x), y(y) {}

    point operator+(point b) {return {x+b.x, y+b.y}; }
    point operator-(point b) {return {x-b.x, y-b.y}; }
    point operator*(T b) {return {x*b, y*b}; }
    point operator/(T b) {return {x/b, y/b}; }
    bool operator==(point b) {return x==b.x && y==b.y; }
    // 逆時針極角排序
    bool operator<(point &b) {return (x*b.y > b.x*y); }
    friend ostream& operator<<(ostream& os, point p) {
        os << "(" << p.x << ", " << p.y << ")";
        return os;
    }
    // 判斷 ab 到 ac 的方向：{1:逆時鐘,0:重疊,-1:順時鐘}
    friend int ori(point a, point b, point c) {
        return sign((b-a)^(c-a));
    }
    friend int btw(point a, point b, point c) {
        return ori(a, b, c) == 0 && sign((a-c)*(b-c)) <= 0;
    }
    // 判斷線段 ab, cd 是否相交
    friend bool banana(point a, point b, point c, point d) {
        int s1 = ori(a, b, c);
        int s2 = ori(a, b, d);
        int s3 = ori(c, d, a);
        int s4 = ori(c, d, b);
        if (btw(a, b, c) || btw(a, b, d) || btw(c, d, a) || btw(c, d, b)) return 1;
        return (s1 * s2 < 0) && (s3 * s4 < 0);
    }

    T operator*(point b) {return x * b.x + y * b.y; }
    T operator^(point b) {return x * b.y - y * b.x; }
    T abs2() {return (*this) * (*this); }

    // 旋轉 Arg(b) 的角度（小心溢位）
    point rotate(point b) {return {x*b.x - y*b.y, x*b.y + y*b.x}; }
};

template<typename T>
struct line {
    point<T> p1, p2;
    // ax + by + c = 0
    T a, b, c; // |a|, |b| ≤ 2C, |c| ≤ 8C²
    line() {}
    line(const point<T> &x,const point<T> &y) : p1(x), p2(y){
        build();
    }
    void build() {
		a = p1.y - p2.y;
		b = p2.x - p1.x;
		c = (-a*p1.x)-b*p1.y;
    }
    // 判斷點和有向直線的關係：{1:左邊,0:在線上,-1:右邊}
	int ori(point<T> &p) {
		return sign((p2-p1) ^ (p-p1));
	}
	// 判斷直線斜率是否相同
	bool parallel(line &l) {
		return ((p1-p2) ^ (l.p1-l.p2)) == 0;
	}
	// 兩直線交點
    point<long double> line_intersection(line &l) {
        using P = point<long double>;
		point<T> a = p2-p1, b = l.p2-l.p1, s = l.p1-p1;
		return P(p1.x,p1.y) + P(a.x,a.y) * (((long double)(s^b)) / (a^b));
	}
};

template<typename T>
struct polygon {
    vector<point<T>> v;
    polygon() {}
    polygon(const vector<point<T>> &u) : v(u) {}
    // simple 為 true 的時候會回傳任意三點不共線的凸包
    void make_convex_hull(int simple) {
        auto cmp = [&](point<T> &p, point<T> &q) {
            return (p.x == q.x) ? (p.y < q.y) : (p.x < q.x);
        };
        simple = (bool)simple;
        sort(v.begin(), v.end(), cmp);
        v.resize(unique(v.begin(), v.end()) - v.begin());
        vector<point<T>> hull;
        for (int t = 0; t < 2; ++t){
            int sz = hull.size();
            for (auto &i:v) {
                while (hull.size() >= sz+2 && ori(hull[hull.size()-2], hull.back(), i) < simple) {
                    hull.pop_back();
                }
                hull.push_back(i);
            }
            hull.pop_back();
            reverse(v.begin(), v.end());
        }
        swap(hull, v);
    }
//  可以在有 n 個點的簡單多邊形內，用 O(n) 判斷一個點：
//  {1 : 在多邊形內, 0 : 在多邊形上, -1 : 在多邊形外}
    int in_polygon(point<T> a){
        const T MAX_POS = 1e9 + 5; // [記得修改] 座標的最大值
        point<T> pre = v.back(), b(MAX_POS, a.y + 1);
        int cnt = 0;

        for (auto &i:v) {
            if (btw(pre, i, a)) return 0;
            if (banana(a, b, pre, i)) cnt++;
            pre = i;
        }

        return cnt%2 ? 1 : -1;
    }
/// 警告：所有凸包專用的函式都只接受逆時針排序且任三點不共線的凸包 ///
//  可以在有 n 個點的凸包內，用 O(log n) 判斷一個點：
//  {1 : 在凸包內, 0 : 在凸包邊上, -1 : 在凸包外}
    int in_convex(point<T> p) {
        int n = v.size();
        int a = ori(v[0], v[1], p), b = ori(v[0], v[n-1], p);
        if (a < 0 || b > 0) return -1;
        if (btw(v[0], v[1], p)) return 0;
        if (btw(v[0], v[n - 1], p)) return 0;
        int l = 1, r = n - 1, mid;
        while (l + 1 < r) {
            mid = (l + r) >> 1;
            if (ori(v[0], v[mid], p) >= 0) l = mid;
            else r = mid;
        }
        int k = ori(v[l], v[r], p);
        if (k <= 0) return k;
        return 1;
    }
//  凸包專用的環狀二分搜，回傳 0-based index
    int cycle_search(auto &f) {
        int i = 0, n = v.size();
        for (int j = 1 << __lg(n); j > 0; j >>= 1) {
            int nxt = (i + j) % n;
            for (int k = 0; k < 2; ++k) {
                if (f(i, nxt)) {
                    i = nxt;
                    break;
                }
                nxt = (i + n - j) % n;
            }
        }
        return i;
    }
//  可以在有 n 個點的凸包內，用 O(log n) 判斷一條直線：
//  {1 : 穿過凸包, 0 : 剛好切過凸包, -1 : 沒碰到凸包}
    int line_cut_convex(line<T> p) {
        /// TO DO
    }
    int segment_cut_convex(line<T> p) {
        /// TO DO
    }
//  回傳點過凸包的兩條切線的切點的 0-based index
    pair<int,int> convex_tangent_point(point<T> p) {
        auto gt = [&](int neg) {
            auto f = [&](int x, int y) {
                return ori(p, v[x], v[y]) == neg;
            };
            return cycle_search(f);
        };
        int x = gt(1), y = gt(-1), n = v.size();
        int z = (v[x] == p) ? x : y;
        if (v[z] == p) {
            return {(z + n - 1) % n, (z + 1) % n};
        }
        else {
            return {x, y};
        }
    }
    friend int halfplane_intersection(vector<line<T>> &s, polygon<T> &P) {
        #define neg(p) ((p.y == 0 ? p.x : p.y) < 0)
        auto angle_cmp = [&](line<T> &A, line<T> &B) {
            point<T> a = A.p2-A.p1, b = B.p2-B.p1;
            return neg(a) < neg(b) || (neg(a) == neg(b) && (a^b) > 0);
        };
        #undef neg
        sort(s.begin(), s.end(), angle_cmp); // 線段左側為該線段半平面
        int L, R, n = s.size();
        vector<point<T>> px(n);
        vector<line<T>> q(n);
        q[L = R = 0] = s[0];
        for(int i = 1; i < n; ++i) {
            while(L < R && s[i].ori(px[R-1]) <= 0) --R;
            while(L < R && s[i].ori(px[L])   <= 0) ++L;
            q[++R] = s[i];
            if(q[R].parallel(q[R-1])) {
                --R;
                if(q[R].ori(s[i].p1) > 0) q[R] = s[i];
            }
            if(L < R) px[R-1] = q[R-1].line_intersection(q[R]);
        }
        while(L < R && q[L].ori(px[R-1]) <= 0) --R;
        P.v.clear();
        if(R - L <= 1) return 0;
        px[R] = q[R].line_intersection(q[L]);
        for(int i = L; i <= R; ++i) P.v.push_back(px[i]);
        return R - L + 1;
    }
};
/// TO DO : .svg maker
