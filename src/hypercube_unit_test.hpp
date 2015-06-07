#ifndef HYPERCUBE_UNIT_TEST_HPP
#define HYPERCUBE_UNIT_TEST_HPP

#include "Space/Shape/HyperCube.hpp"
#include "util.hpp"

/*! \brief Check if the linearization is correct
 *
 * \param cmb vector of combination to checl
 *
 */

template<unsigned int dim> void check_lin(std::vector<comb<dim>> cmb)
{
	// Check the linearization
	for (size_t i = 0 ; i < cmb.size() ; i++)
	{
		BOOST_REQUIRE_EQUAL(HyperCube<dim>::LinId(cmb[i]),i);
	}
}

/*! \brief Check if the combinations are dinstict
 *
 * \param combs combinations to check
 *
 */

template<unsigned int dim> bool isDinstict(std::vector<comb<dim>> combs)
{
	// Check if the combination are dinstinct

	for (size_t i = 0 ; i < combs.size() ; i++)
	{
		for (size_t j = i+1 ; j < combs.size() ; j++)
		{
			if (combs[i] == combs[j])
				return false;
		}

	}

	return true;
}

/*! \brief isSubdecomposition check if combs are elements that exist in c as sub-elements
 *
 * Check if combs are lower dimensional elements of c
 *
 * \param combs elements to check
 * \param c element that must contain all the combs
 *
 */

template<unsigned int dim> bool isSubdecomposition(std::vector<comb<dim>> combs, comb<dim> c)
{
	for (size_t i = 0 ; i < combs.size() ; i++)
	{
		if (c.isSub(combs[i]) == false)
			return false;
	}

	return true;
}

template<unsigned int dim> bool isValid(std::vector<comb<dim>> combs)
{
	// Check if the combinations are valid

	for (size_t i = 0 ; i < combs.size() ; i++)
	{
		if (combs[i].isValid() == false)
			return false;
	}

	return true;
}

BOOST_AUTO_TEST_SUITE( Hyper_cube )

BOOST_AUTO_TEST_CASE( Hyper_cube_use)
{
	std::cout << "Hyper-cube unit test start" << "\n";
	// Test Hyper-Cube functionality

	size_t ele[8];

	//! [Get vertex and edge on a line]

	// Get the number of vertex (elements of dimension 0) of a line (dimension 1)
	ele[0] = HyperCube<1>::getNumberOfElements_R(0);
	// Get the number of edge (elements of dimension 1) of a line (dimension 1)
	ele[1] = HyperCube<1>::getNumberOfElements_R(1);

	// Get combination for each dimensions
	std::vector<comb<1>> v_c1_0 = HyperCube<1>::getCombinations_R(0);

	//! [Get vertex and edge on a line]

	// Check
	BOOST_REQUIRE_EQUAL(ele[0],2);
	BOOST_REQUIRE_EQUAL(ele[1],1);

	// Fill the expected vector

	comb<1> c[] = {{{1}},{{-1}}};

	// Check the linearization
	check_lin(v_c1_0);

	boost_check_array(&c[0],&v_c1_0[0],2);

	//! [Get vertex edge and surfaces of a square]
	// Number of vertex
	ele[0] = HyperCube<2>::getNumberOfElements_R(0);
	// Number of edge
	ele[1] = HyperCube<2>::getNumberOfElements_R(1);
	// Number of faces
	ele[2] = HyperCube<2>::getNumberOfElements_R(2);

	// Get combination for vertex (1,1) (-1,1) (-1,1) (-1,-1)
	std::vector<comb<2>> v_c2_0 = HyperCube<2>::getCombinations_R(0);
	// Get combination for edges  (1,0) (-1,0) (0,1) (0,-1)
	std::vector<comb<2>> v_c2_1 = HyperCube<2>::getCombinations_R(1);

	//! [Get vertex edge and surfaces of a square]

	// Check
	BOOST_REQUIRE_EQUAL(ele[0],4);
	BOOST_REQUIRE_EQUAL(ele[1],4);
	BOOST_REQUIRE_EQUAL(ele[2],1);

	// Check combination

	comb<2> c2_0[] = {{{1,1}},{{1,-1}},{{-1,1}},{{-1,-1}}};
	comb<2> c2_1[] = {{{1,0}},{{-1,0}},{{0,1}},{{0,-1}}};
	check_lin(v_c2_0);
	check_lin(v_c2_1);

	boost_check_array(&c2_0[0],&v_c2_0[0],4);
	boost_check_array(&c2_1[0],&v_c2_1[0],4);

	//! [Get vertex edge surfaces and volumes of a cube]
	// Number of vertex
	ele[0] = HyperCube<3>::getNumberOfElements_R(0);
	// Number of edge
	ele[1] = HyperCube<3>::getNumberOfElements_R(1);
	// Number of faces
	ele[2] = HyperCube<3>::getNumberOfElements_R(2);
	// Number of Cubes
	ele[3] = HyperCube<3>::getNumberOfElements_R(3);

	// Get combination for vertex
	std::vector<comb<3>> v_c3_0 = HyperCube<3>::getCombinations_R(0);
	// Get combinations for edge
	std::vector<comb<3>> v_c3_1 = HyperCube<3>::getCombinations_R(1);
	// Get combinations for surfaces
	std::vector<comb<3>> v_c3_2 = HyperCube<3>::getCombinations_R(2);

	//! [Get vertex edge surfaces and volumes of a cube]

	// Check
	BOOST_REQUIRE_EQUAL(ele[0],8);
	BOOST_REQUIRE_EQUAL(ele[1],12);
	BOOST_REQUIRE_EQUAL(ele[2],6);
	BOOST_REQUIRE_EQUAL(ele[3],1);

	// Check combination

	comb<3> c3_0[] = {{{1,1,1}},{{1,1,-1}},{{1,-1,1}},{{1,-1,-1}},{{-1,1,1}},{{-1,1,-1}},{{-1,-1,1}},{{-1,-1,-1}}};
	comb<3> c3_1[] = {{{1,1,0}},{{1,-1,0}},{{-1,1,0}},{{-1,-1,0}},{{1,0,1}},{{1,0,-1}},{{-1,0,1}},{{-1,0,-1}},{{0,1,1}},{{0,1,-1}},{{0,-1,1}},{{0,-1,-1}}};
	comb<3> c3_2[] = {{{1,0,0}},{{-1,0,0}},{{0,1,0}},{{0,-1,0}},{{0,0,1}},{{0,0,-1}}};
	check_lin(v_c3_0);
	check_lin(v_c3_1);
	check_lin(v_c3_2);

	boost_check_array(&c3_0[0],&v_c3_0[0],8);
	boost_check_array(&c3_1[0],&v_c3_1[0],12);
	boost_check_array(&c3_2[0],&v_c3_2[0],6);

	// Tesseract
	// Number of vertex
	ele[0] = HyperCube<4>::getNumberOfElements_R(0);
	// Number of edge
	ele[1] = HyperCube<4>::getNumberOfElements_R(1);
	// Number of faces
	ele[2] = HyperCube<4>::getNumberOfElements_R(2);
	// Number of Cubes
	ele[3] = HyperCube<4>::getNumberOfElements_R(3);
	// Number of Tessaract
	ele[4] = HyperCube<4>::getNumberOfElements_R(4);

	// Get combination for each dimensions
	std::vector<comb<4>> v_c4_0 = HyperCube<4>::getCombinations_R(0);
	std::vector<comb<4>> v_c4_1 = HyperCube<4>::getCombinations_R(1);
	std::vector<comb<4>> v_c4_2 = HyperCube<4>::getCombinations_R(2);
	std::vector<comb<4>> v_c4_3 = HyperCube<4>::getCombinations_R(3);
	check_lin(v_c4_0);
	check_lin(v_c4_1);
	check_lin(v_c4_2);
	check_lin(v_c4_3);

	// Check
	BOOST_REQUIRE_EQUAL(ele[0],16);
	BOOST_REQUIRE_EQUAL(ele[1],32);
	BOOST_REQUIRE_EQUAL(ele[2],24);
	BOOST_REQUIRE_EQUAL(ele[3],8);
	BOOST_REQUIRE_EQUAL(ele[4],1);

	// Penteract
	// Number of vertex
	ele[0] = HyperCube<5>::getNumberOfElements_R(0);
	// Number of edge
	ele[1] = HyperCube<5>::getNumberOfElements_R(1);
	// Number of faces
	ele[2] = HyperCube<5>::getNumberOfElements_R(2);
	// Number of Cubes
	ele[3] = HyperCube<5>::getNumberOfElements_R(3);
	// Number of Tessaract
	ele[4] = HyperCube<5>::getNumberOfElements_R(4);
	// Number of Penteract
	ele[5] = HyperCube<5>::getNumberOfElements_R(5);

	// Get combination for each dimensions
	std::vector<comb<5>> v_c5_0 = HyperCube<5>::getCombinations_R(0);
	std::vector<comb<5>> v_c5_1 = HyperCube<5>::getCombinations_R(1);
	std::vector<comb<5>> v_c5_2 = HyperCube<5>::getCombinations_R(2);
	std::vector<comb<5>> v_c5_3 = HyperCube<5>::getCombinations_R(3);
	std::vector<comb<5>> v_c5_4 = HyperCube<5>::getCombinations_R(4);
	check_lin(v_c5_0);
	check_lin(v_c5_1);
	check_lin(v_c5_2);
	check_lin(v_c5_3);
	check_lin(v_c5_4);

	// Check
	BOOST_REQUIRE_EQUAL(ele[0],32);
	BOOST_REQUIRE_EQUAL(ele[1],80);
	BOOST_REQUIRE_EQUAL(ele[2],80);
	BOOST_REQUIRE_EQUAL(ele[3],40);
	BOOST_REQUIRE_EQUAL(ele[4],10);
	BOOST_REQUIRE_EQUAL(ele[5],1);


	// Test SubHypercube 2D and 3D

	//! [Get the vertices of a square]
	std::vector<comb<2>> sc2_0 = HyperCube<2>::getCombinations_R(0);
	//! [Get the vertices of a square]

	for (size_t i = 0 ; i < sc2_0.size() ; i++)
	{
		// Expecting one element equal to c2[i]
		//! [Getting the vertex as a sub-hypercube]
		std::vector<comb<2>> combs = SubHyperCube<2,0>::getCombinations_R(sc2_0[i],0);
		//! [Getting the vertex as a sub-hypercube]
		BOOST_REQUIRE_EQUAL(combs.size(),1);
		BOOST_REQUIRE_EQUAL(isDinstict(combs),true);
		BOOST_REQUIRE_EQUAL(isValid(combs),true);
		BOOST_REQUIRE_EQUAL(isSubdecomposition(combs,sc2_0[i]),true);
	}

	//! [Getting the edge of a square]
	std::vector<comb<2>> sc2_1 = HyperCube<2>::getCombinations_R(1);
	//! [Getting the edge of a square]

	for (size_t i = 0 ; i < sc2_1.size() ; i++)
	{
		// Expecting two elements, valid, distinct,  sub-decomposition of c2[i]

		//! [Getting the vertex of the line of the original square]
		std::vector<comb<2>> combs = SubHyperCube<2,1>::getCombinations_R(sc2_1[i],0);
		//! [Getting the vertex of the line of the original square]
		BOOST_REQUIRE_EQUAL(combs.size(),2);
		BOOST_REQUIRE_EQUAL(isDinstict(combs),true);
		BOOST_REQUIRE_EQUAL(isValid(combs),true);
		BOOST_REQUIRE_EQUAL(isSubdecomposition(combs,sc2_1[i]),true);

		// Expecting one element, valid, distinct,  sub-decomposition of c2[i]
		//! [Getting the edge(line) of the line of the original square]
		combs = SubHyperCube<2,1>::getCombinations_R(sc2_1[i],1);
		//! [Getting the edge(line) of the line of the original square]
		BOOST_REQUIRE_EQUAL(combs.size(),1);
		BOOST_REQUIRE_EQUAL(isDinstict(combs),true);
		BOOST_REQUIRE_EQUAL(isValid(combs),true);
		BOOST_REQUIRE_EQUAL(isSubdecomposition(combs,sc2_1[i]),true);
	}

	//! [Getting the square of a square]
	std::vector<comb<2>> sc2_2 = HyperCube<2>::getCombinations_R(2);
	//! [Getting the square of a square]

	for (size_t i = 0 ; i < sc2_2.size() ; i++)
	{
		// Expecting two elements, valid, distinct,  sub-decomposition of sc2_2[i]
		//! [Getting the vertices of the square of the original square]
		std::vector<comb<2>> combs = SubHyperCube<2,2>::getCombinations_R(sc2_2[i],0);
		//! [Getting the vertices of the square of the original square]
		BOOST_REQUIRE_EQUAL(combs.size(),4);
		BOOST_REQUIRE_EQUAL(isDinstict(combs),true);
		BOOST_REQUIRE_EQUAL(isValid(combs),true);
		BOOST_REQUIRE_EQUAL(isSubdecomposition(combs,sc2_2[i]),true);

		// Expecting one element, valid, distinct,  sub-decomposition of c2[i]
		//! [Getting the lines of the square of the original square]
		combs = SubHyperCube<2,2>::getCombinations_R(sc2_2[i],1);
		//! [Getting the lines of the square of the original square]
		BOOST_REQUIRE_EQUAL(combs.size(),4);
		BOOST_REQUIRE_EQUAL(isDinstict(combs),true);
		BOOST_REQUIRE_EQUAL(isValid(combs),true);
		BOOST_REQUIRE_EQUAL(isSubdecomposition(combs,sc2_2[i]),true);

		// Expecting one element, valid, distinct,  sub-decomposition of c2[i]
		//! [Getting the square of the square of the original square]
		combs = SubHyperCube<2,2>::getCombinations_R(sc2_2[i],2);
		//! [Getting the square of the square of the original square]
		BOOST_REQUIRE_EQUAL(combs.size(),1);
		BOOST_REQUIRE_EQUAL(isDinstict(combs),true);
		BOOST_REQUIRE_EQUAL(isValid(combs),true);
		BOOST_REQUIRE_EQUAL(isSubdecomposition(combs,sc2_2[i]),true);
	}

	////////////// 3D ////////////////

	//! [Getting the vertices of the cube]
	std::vector<comb<3>> sc3_0 = HyperCube<3>::getCombinations_R(0);
	//! [Getting the vertices of the cube]

	for (size_t i = 0 ; i < sc3_0.size() ; i++)
	{
		// Expecting one element equal to sc3[i]
		//! [Getting the vertices of the vertices of the cube]
		std::vector<comb<3>> combs = SubHyperCube<3,0>::getCombinations_R(sc3_0[i],0);
		//! [Getting the vertices of the vertices of the cube]
		BOOST_REQUIRE_EQUAL(combs.size(),1);
		BOOST_REQUIRE_EQUAL(isDinstict(combs),true);
		BOOST_REQUIRE_EQUAL(isValid(combs),true);
//		BOOST_REQUIRE_EQUAL(isSubdecomposition(combs,sc3_0[i]),true);
	}

	//! [Getting the edges of the cube]
	std::vector<comb<3>> sc3_1 = HyperCube<3>::getCombinations_R(1);
	//! [Getting the edges of the cube]

	for (size_t i = 0 ; i < sc3_1.size() ; i++)
	{
		// Expecting two elements, valid, distinct,  sub-decomposition of sc3[i]
		//! [Getting the vertices of the edge of the cube]
		std::vector<comb<3>> combs = SubHyperCube<3,1>::getCombinations_R(sc3_1[i],0);
		//! [Getting the vertices of the vertices of the cube]
		BOOST_REQUIRE_EQUAL(combs.size(),2);
		BOOST_REQUIRE_EQUAL(isDinstict(combs),true);
		BOOST_REQUIRE_EQUAL(isValid(combs),true);
		BOOST_REQUIRE_EQUAL(isSubdecomposition(combs,sc3_1[i]),true);

		// Expecting one element, valid, distinct,  sub-decomposition of c2[i]
		//! [Getting the edges of the edges of the cube]
		combs = SubHyperCube<3,1>::getCombinations_R(sc3_1[i],1);
		//! [Getting the edges of the edges of the cube]
		BOOST_REQUIRE_EQUAL(combs.size(),1);
		BOOST_REQUIRE_EQUAL(isDinstict(combs),true);
		BOOST_REQUIRE_EQUAL(isValid(combs),true);
		BOOST_REQUIRE_EQUAL(isSubdecomposition(combs,sc3_1[i]),true);
	}

	//! [Getting the surfaces of the cube]
	std::vector<comb<3>> sc3_2 = HyperCube<3>::getCombinations_R(2);
	//! [Getting the surfaces of the cube]

	for (size_t i = 0 ; i < sc3_2.size() ; i++)
	{
		// Expecting two elements, valid, distinct,  sub-decomposition of sc3_2[i]
		//! [Getting the vertices of the surfaces of the cube]
		std::vector<comb<3>> combs = SubHyperCube<3,2>::getCombinations_R(sc3_2[i],0);
		//! [Getting the vertices of the surfaces of the cube]
		BOOST_REQUIRE_EQUAL(combs.size(),4);
		BOOST_REQUIRE_EQUAL(isDinstict(combs),true);
		BOOST_REQUIRE_EQUAL(isValid(combs),true);
		BOOST_REQUIRE_EQUAL(isSubdecomposition(combs,sc3_2[i]),true);

		// Expecting one element, valid, distinct,  sub-decomposition of c2[i]
		//! [Getting the edges of the surfaces of the cube]
		combs = SubHyperCube<3,2>::getCombinations_R(sc3_2[i],1);
		//! [Getting the edges of the surfaces of the cube]
		BOOST_REQUIRE_EQUAL(combs.size(),4);
		BOOST_REQUIRE_EQUAL(isDinstict(combs),true);
		BOOST_REQUIRE_EQUAL(isValid(combs),true);
		BOOST_REQUIRE_EQUAL(isSubdecomposition(combs,sc3_2[i]),true);

		// Expecting one element, valid, distinct,  sub-decomposition of c2[i]
		//! [Getting the surfaces of the surfaces of the cube]
		combs = SubHyperCube<3,2>::getCombinations_R(sc3_2[i],2);
		//! [Getting the surfaces of the surfaces of the cube]
		BOOST_REQUIRE_EQUAL(combs.size(),1);
		BOOST_REQUIRE_EQUAL(isDinstict(combs),true);
		BOOST_REQUIRE_EQUAL(isValid(combs),true);
		BOOST_REQUIRE_EQUAL(isSubdecomposition(combs,sc3_2[i]),true);
	}

/*	comb<5> c0;
	for (int i = 0 ; i < 5 ; i++)
		c0.c[i] = 1;

	std::vector<comb<5>> combs_0_0 = SubHyperCube<5,0>::getCombinations_R(c0,0);
	std::vector<comb<5>> combs_1_0 = SubHyperCube<5,1>::getCombinations_R(c,0);
	std::vector<comb<5>> combs_1_1 = SubHyperCube<5,1>::getCombinations_R(c,1);
	std::vector<comb<5>> combs_2_0 = SubHyperCube<5,2>::getCombinations_R(c,0);
	std::vector<comb<5>> combs_2_1 = SubHyperCube<5,2>::getCombinations_R(c,1);
	std::vector<comb<5>> combs_2_2 = SubHyperCube<5,2>::getCombinations_R(c,2);
	std::vector<comb<5>> combs_3_0 = SubHyperCube<5,3>::getCombinations_R(c,0);
	std::vector<comb<5>> combs_3_1 = SubHyperCube<5,3>::getCombinations_R(c,1);
	std::vector<comb<5>> combs_3_2 = SubHyperCube<5,3>::getCombinations_R(c,2);
	std::vector<comb<5>> combs_3_3 = SubHyperCube<5,3>::getCombinations_R(c,3);
	std::vector<comb<5>> combs_4_0 = SubHyperCube<5,4>::getCombinations_R(c,0);
	std::vector<comb<5>> combs_4_1 = SubHyperCube<5,4>::getCombinations_R(c,1);
	std::vector<comb<5>> combs_4_2 = SubHyperCube<5,4>::getCombinations_R(c,2);
	std::vector<comb<5>> combs_4_3 = SubHyperCube<5,4>::getCombinations_R(c,3);
	std::vector<comb<5>> combs_4_4 = SubHyperCube<5,4>::getCombinations_R(c,4);
	std::vector<comb<5>> combs_5_0 = SubHyperCube<5,5>::getCombinations_R(c,0);
	std::vector<comb<5>> combs_5_1 = SubHyperCube<5,5>::getCombinations_R(c,1);
	std::vector<comb<5>> combs_5_2 = SubHyperCube<5,5>::getCombinations_R(c,2);
	std::vector<comb<5>> combs_5_3 = SubHyperCube<5,5>::getCombinations_R(c,3);
	std::vector<comb<5>> combs_5_4 = SubHyperCube<5,5>::getCombinations_R(c,4);
	std::vector<comb<5>> combs_5_5 = SubHyperCube<5,5>::getCombinations_R(c,5);*/

	std::cout << "Hyper-cube unit test end" << "\n";
}

BOOST_AUTO_TEST_SUITE_END()

#endif
