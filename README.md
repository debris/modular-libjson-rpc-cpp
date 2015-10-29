# modular-libjson-rpc-cpp
An attempt to modularize libjson-rpc-cpp server.

## The Problem

Sometimes rpc server has several or more methods. Currently we have to specify all of these methods in a single .json file and implement all of them.

## Solution

Modularized server which dispatches calls to various modules.

```c++
ModularServer<AInterface, BInterface, CInterface> ms(new AModule(), new BModule(), new CModule());

ms.addConnector(new jsonrpc::HttpServer(8080, "", "", 2));
ms.StartListening();

while (true)
	std::this_thread::sleep_for(std::chrono::milliseconds(1000));
```

## Example

Instead of altering libjson-rpc-cpp code, I've created a 'polyfill` which implements this new functionality. This polyfill consist of two files:
- [`ModularServer.h`](https://github.com/debris/modular-libjson-rpc-cpp/blob/master/ModularServer.h)
- [`mod.sh`](https://github.com/debris/modular-libjson-rpc-cpp/blob/master/mod.sh)
