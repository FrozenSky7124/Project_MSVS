#include <iostream>
#include <string>
#include <boost/regex.hpp>
#include <boost/foreach.hpp>
#include <boost/assign.hpp>
#include <boost/timer/timer.hpp>

int timerTest()
{
	std::vector<int> v = (boost::assign::list_of(1), 2, 3, 4, 5);
	BOOST_FOREACH(int x, v)
	{
		std::cout << x << ",";
	}
	std::cout << std::endl;

	boost::timer::auto_cpu_timer t;
	for (long i = 0; i < 1000; ++i) std::sqrt(123.456L);
	std::cout << t.format() << " s " << std::endl;

	return 0;
}

int main()
{
	std::string line;
	boost::regex pat("^Subject: (Re: |Aw: )*(.*)");

	while (std::cin)
	{
		std::getline(std::cin, line);
		boost::smatch matches;
		if (boost::regex_match(line, matches, pat))
			std::cout << matches[2] << std::endl;
	}

	timerTest();
}