#pragma once
#include "simple_tcp.hpp"
#include <string>
#include <iostream>
#include <windows.h>

namespace simple_http {
	class HTTPReply {
		std::string version;
		std::string code;
		std::string reply;

	public:
		HTTPReply(std::string version, std::string code) {
			this->version = version;
			this->code = code;
		}

		void add_attr(std::string key, std::string val) {
			this->reply += key+": "+val+"\r\n";
		}

		void purge() {
			this->reply = "";
		}

		std::string parse(std::string data) {
			auto header = "HTTP/"+this->version+" "+this->code;
			this->add_attr("Content-Length", std::to_string(data.length()));
			return header+"\r\n"+this->reply+"\r\n"+data;
		}
	};

	class HTTPServer {
		simple_tcp::SimpleTCP* ssocket;
		bool run_first_time;
	public:

		HTTPServer(std::string ip, int port=80) {
			this->ssocket = new simple_tcp::SimpleTCP();
			this->ssocket = this->ssocket->init();
			this->ssocket->bind_sock(ip, port);
			this->ssocket->listen_sock(0);
			run_first_time = true;
		}
		~HTTPServer() {
			this->ssocket->kill();
			delete this->ssocket;
		}

		void listen() {
			std::cout<<"Nasluchiwanie..."<<std::endl;
			while(true) {
				if(run_first_time) {
					run_first_time = false;
					system("start http://127.0.0.1:8080");
				}
				auto client_sock = ssocket->accept_sock();
				if(!client_sock->is_invalid()) {
					std::cout<<"Zestawiono polaczenie z klientem!"<<std::endl;
					auto rep = new HTTPReply("1.1", "200 OK");
					rep->add_attr("Connection", "close");
					rep->add_attr("Content-Type", "text/html; charset=utf-8");
					rep->add_attr("Access-Control-Allow-Origin", "*");
					std::string msg = "<html><head><title>2Tb>>>2Ta</title></head><body><center><h1>Makao!</h1><p>2Ta to mi może gałe opierdolić, a nie lepsza...</p><br><br><br><a href=\"https://youtu.be/uP8b05wUH1Q\" target=_blank>A TUTAJ LINK DO FILMIKU OPISUJĄCEGO CAŁY PROCES ROBIENIA TEGO PROGRAMU</a></center></body></html>";
					client_sock->send_string(rep->parse(msg));
					std::cout<<"Wyslano dane:\n"+rep->parse(msg)<<std::endl;
					client_sock->kill();
					std::cout<<"Zabito polaczenie z klientem."<<std::endl;
					delete rep;
				}
				delete client_sock;
			}
			std::cout<<"Koniec nasluchiwania!"<<std::endl;
		}
	};
}