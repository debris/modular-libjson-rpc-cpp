#pragma once

#include <string>
#include <map>
#include <vector>
#include <jsonrpccpp/common/procedure.h>
#include <jsonrpccpp/server/iprocedureinvokationhandler.h>

template <class I> using AbstractMethodPointer = void(I::*)(Json::Value const& _parameter, Json::Value& _result);
template <class I> using AbstractNotificationPointer = void(I::*)(Json::Value const& _parameter);

template <template <class I> class C, class I>
class ProcedureBinding
{
public:
	using CallPointer = C<I>;
	
	ProcedureBinding(jsonrpc::Procedure const& _procedure, CallPointer _call)
	: m_procedure(_procedure), m_call(_call) {}
	
	jsonrpc::Procedure const& procedure() const { return m_procedure; }
	CallPointer call() const { return m_call; }
	
private:
	jsonrpc::Procedure m_procedure;
	CallPointer m_call;
};

template <class I> using MethodBinding = ProcedureBinding<AbstractMethodPointer, I>;
template <class I> using NotificationBinding = ProcedureBinding<AbstractNotificationPointer, I>;

template <class I>
class ServerInterface
{
public:
	using MethodPointer = AbstractMethodPointer<I>;
	using NotificationPointer = AbstractNotificationPointer<I>;
	using Methods = std::vector<MethodBinding<I>>;
	using Notifications = std::vector<NotificationBinding<I>>;
	
	virtual Methods const& methods() const { return m_methods; }
	virtual Notifications const& notifications() const { return m_notifications; }
	
protected:
	void bindAndAddMethod(jsonrpc::Procedure const& _proc, MethodPointer _pointer) { m_methods.emplace_back(_proc, _pointer); }
	void bindAndAddNotification(jsonrpc::Procedure const& _proc, NotificationPointer _pointer) { m_notifications.emplace_back(_proc, _pointer); }
	
private:
	Methods m_methods;
	Notifications m_notifications;
};

template <class... Is>
class ModularServer: public jsonrpc::IProcedureInvokationHandler
{
public:
	virtual void HandleMethodCall(jsonrpc::Procedure& _proc, const Json::Value& _input, Json::Value& _output) override {}
	virtual void HandleNotificationCall(jsonrpc::Procedure& _proc, const Json::Value& _input) override {}
};

template <class I, class... Is> class ModularServer<I, Is...> : ModularServer<Is...>
{
public:
	using MethodPointer = AbstractMethodPointer<I>;
	using NotificationPointer = AbstractNotificationPointer<I>;
	
	ModularServer<I, Is...>(I* _i, Is*... _is): ModularServer<Is...>(_is...), m_interface(_i)
	{
		for (auto const& method: m_interface->methods())
			m_methods[method.procedure().GetProcedureName()] = method.call();
		
		for (auto const& notification: m_interface->notifications())
			m_notifications[notification.procedure().GetProcedureName()] = notification.call();
	}
	
	virtual void HandleMethodCall(jsonrpc::Procedure& _proc, const Json::Value& _input, Json::Value& _output) override
	{
		auto pointer = m_methods.find(_proc.GetProcedureName());
		if (pointer != m_methods.end())
			(m_interface.get()->*(pointer->second))(_input, _output);
		else
			ModularServer<Is...>::HandleMethodCall(_proc, _input, _output);
	}
	
	virtual void HandleNotificationCall(jsonrpc::Procedure& _proc, const Json::Value& _input) override
	{
		auto pointer = m_notifications.find(_proc.GetProcedureName());
		if (pointer != m_notifications.end())
			(m_interface.get()->*(pointer->second))(_input);
		else
			ModularServer<Is...>::HandleNotificationCall(_proc, _input);
	}
	
private:
	std::unique_ptr<I> m_interface;
	std::map<std::string, MethodPointer> m_methods;
	std::map<std::string, NotificationPointer> m_notifications;
};
