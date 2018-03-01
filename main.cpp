#include <iostream>
#include <sstream>
#include <vector>
#include <cstdio>

struct ride
{
    int start_i, start_j, end_i, end_j;
    int start_t, end_t;
} ;

struct sol
{
    vector<int> ok_ride;
    vector<vector<int> > sol;
};

int rows, cols, n_cars, n_rides, start_bonus, steps;
std::vector<ride> rides;

void load_test(const char *test_name)
{
    std::stringstream s;
    s << "tests/" << test_name << ".in";
    FILE *f = fopen(s.str().c_str(), "r");

    int n;
    fscanf(f, "%d %d %d %d %d %d", &rows, &cols, &n_cars, &n_rides, &start_bonus, &steps);
    for(int i = 0; i < n; i++)
    {
	ride x;
	fscanf(f, "%d %d %d %d %d %d", &x.start_i, &x.start_j, &x.end_i, &x.end_j, &x.start_t, &x.end_t);
	rides.push_back(x);
    }

    fclose(f);
}

void clean_rides()
{
  vector <ride> nwrides;
  for (int i=0; i<rides.size(); i++)
  {
    int startlen=rides[i].start_i+rides[i].start_j;
    int ridelen=abs(rides[i].end_i-rides[i].start_i)+abs(rides[i].end_j-rides[i].start_j);
    if (startlen+ridelen<=rides[i].end_t && rides[i].end_t-rides[i].start_t<=ridelen)
    {
      nwrides.push_back(rides[i]);
    }
  }
  rides=nwrides;
}

int main(int argc, char *argv[])
{
    if(argc != 2)
    {
	std::cerr << "Usage: " << argv[0] << " <test name>" << std::endl;
	return 1;
    }

    if(argv[1][1] == '\0')
    {
	if(argv[1][0] == 'a') load_test("a_example");
	else if(argv[1][0] == 'b') load_test("b_should_be_easy");
	else if(argv[1][0] == 'c') load_test("c_no_hurry");
	else if(argv[1][0] == 'd') load_test("d_metropolis");
	else if(argv[1][0] == 'e') load_test("e_high_bonus");
	else load_test(argv[1]);
    }
    else load_test(argv[1]);

    std::cout << "Test " << argv[1] << std::endl;
    std::cout << "map is " << rows << "x" << cols << std::endl;
    std::cout << "num of cars = " << n_cars << std::endl;
    std::cout << "num of rides = " << n_rides << std::endl;
    std::cout << "bonus for starting on time = " << start_bonus << std::endl;
    std::cout << "num of steps = " << steps << std::endl;
    
    return 0;
}
