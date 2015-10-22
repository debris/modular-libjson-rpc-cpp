#include <thread>
#include <jsonrpccpp/server/connectors/httpserver.h>
#include "ModularServer.h"
#include "AbstractWebThreeStubServerModule.h"

/// example
/// example
/// example
/// example
/// example

class SingleMethodServer: public ServerInterface<SingleMethodServer> {
public:
	SingleMethodServer(std::string const& _method): m_method(_method)
	{
		bindAndAddMethod(jsonrpc::Procedure(m_method, jsonrpc::PARAMS_BY_POSITION, jsonrpc::JSON_STRING,  NULL), &SingleMethodServer::singleMethod);
	}
	
	inline virtual void singleMethod(const Json::Value &request, Json::Value &response) {
		response = "hello from " + m_method;
	};
	
private:
	std::string m_method;
};

class AbstractEth: public ServerInterface<AbstractEth> {
public:
	AbstractEth()
	{
		bindAndAddMethod(jsonrpc::Procedure("eth_gasPrice", jsonrpc::PARAMS_BY_POSITION, jsonrpc::JSON_STRING,  NULL), &AbstractEth::eth_gasPriceI);
	}
	
	inline virtual void eth_gasPriceI(const Json::Value &request, Json::Value &response) {
		response = eth_gasPrice();
	};
	
	virtual std::string eth_gasPrice() = 0;
};

class Eth: public AbstractEth {
public:
	virtual std::string eth_gasPrice() override {
		return "1000";
	}
};

int main(int argc, char** argv) {
	
	ModularServer<SingleMethodServer, SingleMethodServer, Eth> ms(new jsonrpc::HttpServer(8080, "", "", 2),
																  new SingleMethodServer("first"),
																  new SingleMethodServer("second"),
																  new Eth);
	ms.StartListening();

	
	while (true)
		std::this_thread::sleep_for(std::chrono::milliseconds(1000));
	
	// curl -X POST --data '{"id":8,"jsonrpc":"2.0","method":"eth_gasPrice","params":[]}' -H "Content-Type: application/json" http://localhost:8080
	// curl -X POST --data '{"id":8,"jsonrpc":"2.0","method":"first","params":[]}' -H "Content-Type: application/json" http://localhost:8080
	
	return 0;
}

