#include <iostream>
#include <stdio.h>
#include <vector>
#include <boost/foreach.hpp>
#include <boost/assign.hpp>
#include <boost/timer/timer.hpp>

int main()
{
	std::vector<int> v = (boost::assign::list_of(1), 2, 3, 4, 5);
	BOOST_FOREACH(int x, v)
	{
		std::cout << x << ",";
	}
	std::cout << std::endl;

	boost::timer::auto_cpu_timer t;
	for (long i = 0; i < 100000000; ++i) std::sqrt(123.456L);
	std::cout << t.format() << " s " << std::endl;

	getchar();
	return 0;
}