//
//  Marginality.cpp
//
//	by Jiahuan.Liu
//	jiahaun.liu@outlook.com
//
//  03/07/2017
//

#include <math.h>
#include <stdio.h>

#include "Marginality.h"
#include "HullAlg.h"

const int Scale = 1e3;

using val_t = Marginality::val_t;

val_t Marginality::entropy(val_t v1, val_t v2)
{
    val_t sum = v1 + v2;
    return entropy(v1, v2, sum);
}

val_t Marginality::entropy(val_t v1, val_t v2, val_t sum)
{
    if (v1 == 0 || v2 == 0) return 0;
    val_t p1 = v1 / sum, p2 = v2 / sum;
    return -(p1 * log2(p1) + p2 * log2(p2)) * Scale;
}

void Marginality::accumulate(val_t& v1, val_t v2)
{
    v1 *= v2;
}

void Marginality::initialize(val_t& v)
{
    v = 1;
}

void testMarginalitySort()
{
    PointVec test0;
	test0.emplace_back(0, 0);
	test0.emplace_back(0.2, 0);
	test0.emplace_back(0, 0.2);
	test0.emplace_back(0.15, 0.1);
	test0.emplace_back(0.1, 0.15);
	test0.emplace_back(0.2, 0.2);
	test0.emplace_back(0.1, 0.05);
	test0.emplace_back(0.05, 0.1);

    auto refs0 = HullAlg::getRefs(test0);
	auto getRefFromRefItr = [](PointRefVec::iterator itr){return *itr;};
    auto getRefFromPtItr = [](PointVec::iterator itr){return &(*itr);};

    auto res_refs = Marginality::sort(refs0.begin(), refs0.end(), getRefFromRefItr);
    auto res = Marginality::sort(test0.begin(), test0.end(), getRefFromPtItr);
}