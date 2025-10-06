template<typename T>
struct point {
    T x, y;
    point() {}
    point(const T &x, const T &y) : x(x), y(y) {}
    explicit operator point<ld>() {return point<ld>(x, y); }
};
