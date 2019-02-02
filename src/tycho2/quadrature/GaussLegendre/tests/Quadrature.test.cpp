#define CATCH_CONFIG_MAIN
#include "catch2/catch.hpp"
#include "tycho2.hpp"
#include "range/v3/all.hpp"

SCENARIO("GaussLegendre can return points and weights "
	 "of the corresponding order", "[GaussLegendre]"){
  namespace quad = tycho2::quadrature;

  auto make_weighted_power_sum = [](int power){
    return [n=power](auto x, auto w){return w * std::pow(x, n);};
  };

  GIVEN("A Gauss-Legendre Rule of second order"){

    auto gauss_legendre = quad::GaussLegendre<2>{};
    
    WHEN("Querying for points and weights"){

      auto ref_pts = std::array<double,2>{-0.5773502691896257,
					  0.5773502691896257};
      auto pts = gauss_legendre.points();
	
      REQUIRE(pts == ref_pts);
      
      auto ref_wts = std::array<double,2>{1.0,1.0};
      
      auto wts = gauss_legendre.weights();

      REQUIRE(wts == ref_wts);

      AND_THEN("using the points and weights to compute the "
      	       "integral of f(x)=x^2 and f(x)=x^6 on (-1,1)"){

	auto x_squared = 
	  ranges::view::zip_with(make_weighted_power_sum(2), pts, wts);
	auto result = ranges::accumulate(x_squared, 0.0);
	REQUIRE(result==2.0/3.0);

	auto x_sixth = 
	  ranges::view::zip_with(make_weighted_power_sum(6), pts, wts);
	result = ranges::accumulate(x_sixth, 0.0);
	REQUIRE_FALSE(result==2.0/7.0);	

      }
    }
  }
}