#include <thread>
#include <jsonrpccpp/server/connectors/httpserver.h>
#include "example/AInterfaceModule.h"
#include "example/BInterfaceModule.h"
#include "example/CInterfaceModule.h"

class AModule: public AInterface
{
public:
	virtual std::string first(const std::string& param1) override
	{
		return "first";
	}
	
	virtual std::string second() override
	{
		return "second";
	}
};

class BModule: public BInterface
{
public:
	virtual std::string up(const std::string& param1) override
	{
		return "up";
	}
	
	virtual std::string down() override
	{
		return "down";
	}
};

class CModule: public CInterface
{
public:
	virtual std::string hello(const std::string& param1) override
	{
		return "hello";
	}
	
	virtual std::string world() override
	{
		return "world";
	}
};

int main(int argc, char** argv) {

	ModularServer<AInterface, BInterface, CInterface> ms(new AModule(), new BModule(), new CModule());
	
	ms.addConnector(new jsonrpc::HttpServer(8080, "", "", 2));
	ms.StartListening();

	while (true)
		std::this_thread::sleep_for(std::chrono::milliseconds(1000));


//	 curl -X POST --data '{"id":8,"jsonrpc":"2.0","method":"up","params":[""]}' -H "Content-Type: application/json" http://localhost:8080
//	 curl -X POST --data '{"id":8,"jsonrpc":"2.0","method":"first","params":[""]}' -H "Content-Type: application/json" http://localhost:8080
	
	return 0;
}

