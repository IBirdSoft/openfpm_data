//
// Created by tommaso on 18/06/19.
//

#define BOOST_TEST_DYN_LINK

#include <boost/test/unit_test.hpp>
#include "Grid/Geometry/grid_smb.hpp"
#include "Grid/Geometry/grid_zmb.hpp"

template <unsigned int dim, typename BGT>
void testStandardLinearizations(BGT geometry)
{
    grid_key_dx<dim, int> origin({0,0});
    BOOST_REQUIRE_EQUAL(geometry.LinId(origin), 0);

    grid_key_dx<dim, int> block0a({7,0});
    BOOST_REQUIRE_EQUAL(geometry.LinId(block0a), 7);
    grid_key_dx<dim, int> block0b({0,1});
    BOOST_REQUIRE_EQUAL(geometry.LinId(block0b), 8);
    grid_key_dx<dim, int> block0c({7,7});
    BOOST_REQUIRE_EQUAL(geometry.LinId(block0c), 63);

    grid_key_dx<dim, int> block1a({8+7,0});
    BOOST_REQUIRE_EQUAL(geometry.LinId(block1a), 64+7);
    grid_key_dx<dim, int> block1b({8+0,1});
    BOOST_REQUIRE_EQUAL(geometry.LinId(block1b), 64+8);
    grid_key_dx<dim, int> block1c({8+7,7});
    BOOST_REQUIRE_EQUAL(geometry.LinId(block1c), 64+63);

    grid_key_dx<dim, int> block3a({7,8+0});
    BOOST_REQUIRE_EQUAL(geometry.LinId(block3a), (64*3)+7);
    grid_key_dx<dim, int> block3b({0,8+1});
    BOOST_REQUIRE_EQUAL(geometry.LinId(block3b), (64*3)+8);
    grid_key_dx<dim, int> block3c({7,8+7});
    BOOST_REQUIRE_EQUAL(geometry.LinId(block3c), (64*3)+63);
}

BOOST_AUTO_TEST_SUITE(BlockGeometry_tests)

BOOST_AUTO_TEST_CASE(testLinId)
{
	constexpr unsigned int dim = 2;
	const size_t sz[dim] = {3*8,7*8};
	grid_smb<dim, 8> geometry(sz);

	testStandardLinearizations<dim>(geometry);
}

BOOST_AUTO_TEST_CASE(testCopyConstructor)
{
	constexpr unsigned int dim = 2;
	const size_t sz[dim] = {3*8,7*8};
	grid_smb<dim, 8> geometry0(sz);

	// Here copy-construct
	grid_smb<dim, 8> geometry(geometry0);

	// Then test...
	testStandardLinearizations<dim>(geometry);
}

BOOST_AUTO_TEST_CASE(testCopyAssignOp)
{
	constexpr unsigned int dim = 2;
	const size_t sz[dim] = {3*8,7*8};
	grid_smb<dim, 8> geometry0(sz);

	// Here copy-assign
	grid_smb<dim, 8> geometry(sz);
	geometry = geometry0;

	// Then test...
	testStandardLinearizations<dim>(geometry);
}

template<typename gsmb_type>
void test_swap()
{
	const size_t sz[2] = {3*8,7*8};
	gsmb_type geometry0(sz);

	const size_t sz2[2] = {3*8+50,7*8+50};
	gsmb_type geometry1(sz2);

    geometry1.swap(geometry0);


    BOOST_REQUIRE_EQUAL(geometry1.size(0),3*8);
    BOOST_REQUIRE_EQUAL(geometry1.size(1),7*8);

    BOOST_REQUIRE_EQUAL(geometry0.size(0),3*8+50);
    BOOST_REQUIRE_EQUAL(geometry0.size(1),7*8+50);
}

BOOST_AUTO_TEST_CASE(testSwap)
{
    constexpr unsigned int dim = 2;

    test_swap<grid_smb<dim, 8>>();
    test_swap<grid_zmb<dim, 8, long int>>();
}

BOOST_AUTO_TEST_SUITE_END()
