#include "lsd.hpp"

#include <iostream>
using namespace std;

int main(int argc, char **argv)
{
	if(argc != 3)
	{
		cout << "usage:./test lsd_ip lsd_port" << '\n';
		cout << "lsd_ip,lsd_port can be change in /lsd_server/config.json" << '\n';
	}
    LSD *shit = LSD::get_lsd(argv[1], atoi(argv[2]));
    int i = shit->login("Sunxiaoqiang", "1");
    if(i == 0)
    {
        cout << "login success" << endl;
    }
    return 0;
}
