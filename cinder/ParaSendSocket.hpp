/*
Copyright (C) 2015 Martin Bock, Gary Klindt, Annelene Wittenfeld

This program is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

#include <iostream>
#include <sstream>
#include <string>
#include <boost/asio.hpp>
#include <boost/bind.hpp>
#include <boost/date_time/posix_time/posix_time.hpp>

#define PSS_IP "192.168.0.101"
#define PSS_PORT 12345

class ParaSendSocket {

    private:

        boost::asio::io_service ioserv;
        boost::asio::ip::udp::socket* sock;
        boost::asio::ip::udp::endpoint* destination;

    public:

        ParaSendSocket(const ParaSendSocket& pss)
        {
            ParaSendSocket(pss.rows,pss.cols);
        }

        ParaSendSocket() :
        {
            sock = new boost::asio::ip::udp::socket(ioserv);
            sock->open(boost::asio::ip::udp::v4());
            destination = new boost::asio::ip::udp::endpoint(boost::asio::ip::address::from_string(PSS_IP), PSS_PORT);
        }

        void reset()
        {
            sock->close();
            sock = new boost::asio::ip::udp::socket(ioserv);
            sock->open(boost::asio::ip::udp::v4());
            destination = new boost::asio::ip::udp::endpoint(boost::asio::ip::address::from_string(PSS_IP), PSS_PORT);
        }

        void operator()(std::vector<float> imgDesc)
        {
            if (!sock->is_open()){
                return;
            }

            std::stringstream sender;
            for (std::vector<float>::iterator i = imgDesc.begin(); i != imgDesc.end(); ++i){
                sender << (*i) << " ";
            }
            sender << std::endl;

            std::string stringmsg = sender.str();
            const char* msg = stringmsg.c_str();
            boost::asio::const_buffers_1 buffer((void*)msg, strlen(msg));

            sock->send_to(buffer, *destination);
        }

};

