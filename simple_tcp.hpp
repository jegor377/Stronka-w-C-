#pragma once
#include <string>
#include <winsock2.h>

namespace simple_tcp {
	class SimpleTCP {
		SOCKET sd;
		SOCKADDR_IN addr;

	public:
		SimpleTCP() {}
		SimpleTCP(SOCKET sd, SOCKADDR_IN addr) {
			this->sd = sd;
			this->addr = addr;
		}

		SimpleTCP* init() {
			this->sd = socket(AF_INET, SOCK_STREAM, 0);
			return this;
		}

		void kill() {
			closesocket(this->sd);
		}

		void connect_sock(std::string ip, int port) {
			this->set_addr(ip, port);
			connect(this->sd, (SOCKADDR*)&this->addr, sizeof(this->addr));
		}

		void bind_sock(std::string ip, int port) {
			this->set_addr(ip, port);
			bind(this->sd, (SOCKADDR*)&this->addr, sizeof(this->addr));
		}

		void listen_sock(int v) {
			listen(this->sd, v);
		}

		SimpleTCP* accept_sock() {
			SOCKADDR_IN client_addr;
			SOCKET client_socket;
			int client_addr_size = sizeof(client_addr);
			client_socket = accept(this->sd, (SOCKADDR*)&client_addr, &client_addr_size);
			return new SimpleTCP(client_socket, client_addr);
		}

		bool is_invalid() {
			return this->sd == INVALID_SOCKET;
		}

		char* recv_data(int len) {
			char* buff = new char[len];
			memset(buff, 0, len);
			recv(this->sd, buff, len, 0);
			return buff;
		}

		void recv_buff(char* buff, int len) {
			memset(buff, 0, len);
			recv(this->sd, buff, len, 0);
		}

		void send_data(char* data, int len) {
			send(this->sd, data, len, 0);
		}

		void send_string(std::string data) {
			send(this->sd, data.c_str(), data.length(), 0);
		}
	private:
		void set_addr(std::string ip, int port) {
			this->addr.sin_addr.s_addr = inet_addr(ip.c_str());
			this->addr.sin_family = AF_INET;
			this->addr.sin_port = htons(port);
		}
	};

	WSADATA init() {
		WSADATA WSAData;
		WSAStartup(MAKEWORD(2,0), &WSAData);
		return WSAData;
	}

	void kill() {
		WSACleanup();
	}
}