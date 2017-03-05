//
//  hull.h
//
//	@brief: convex hull algorithm class
//
//  by Jiahuan.Liu
//	jiahaun.liu@outlook.com
//
//  03/03/2017
//

#include <mpblocks/clarkson93.hpp>
#include <unordered_set>
#include <cstdint>

using namespace mpblocks;
using namespace mpblocks::clarkson93;

#define MAX_NUM 100000

typedef float Val_t;
#define NDim 2
typedef ExampleTraits2<Val_t, NDim> Traits;
typedef Triangulation<Traits> Triangulation_t;
typedef Triangulation_t::Point Point;

class PointVec: public std::vector<Point>
{
    using val_t = Val_t ;
    using base_t = std::vector<Point>;

public:
    PointVec() {}

    PointVec(int num);

    void random();

    static void initRand(long seed = time(NULL));
};

class Hull
{
public:
    using hash_t = int64_t;
    using Simplex = Triangulation_t::Simplex;
    using PointRef = Triangulation_t::PointRef;
    using PointHashSet = std::unordered_set<hash_t>;

public:
    Hull();

    bool insert(PointRef p);

    void insert(std::vector<PointRef>& pointRefs);

    void insert(PointVec& points);

    void clear();
	
	void printPeaks();

    std::vector<PointRef>& getPeaks();

private:
    hash_t _hash(Point& p);

    hash_t _hash(PointRef p);

    void _addPeak(PointRef p);

private:
    Triangulation_t         _hull;
    std::vector<PointRef>   _peaks;
    PointHashSet            _peakSet;
    PointRef                _init[NDim + 2];
    size_t                  _size;
};

void testHull();