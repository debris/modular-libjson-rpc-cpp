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

///


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
	
	ModularServer<SingleMethodServer, SingleMethodServer, Eth> ms(
																  new SingleMethodServer("first"),
																  new SingleMethodServer("second"),
																  new Eth);

	
	
	jsonrpc::Procedure proc("eth_gasPrice", jsonrpc::PARAMS_BY_POSITION, jsonrpc::JSON_STRING,  NULL);
	Json::Value input(Json::nullValue);
	Json::Value output(Json::nullValue);
	
	ms.HandleMethodCall(proc, input, output);
	
	return 0;
}

