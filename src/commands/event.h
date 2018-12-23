#pragma once

#include "command.h"
#include <set>

class EventCommand : public ICommand
{
  public:
    inline static std::set<std::string> m_Hints = {};

    virtual void Initialize() override
    {
        static hk::inject_jump<void, int64_t, char*, char, bool> add_event_hook(0x140089870);
        add_event_hook.inject([](int64_t _, char* eventName, char flags, bool unknown) {
			m_Hints.insert(eventName);
            add_event_hook.call(_, eventName, flags, unknown);
		});
    }

    virtual const char* GetCommand() override
    {
        return "event";
    }

    virtual bool Handler(const std::string& arguments) override
    {
        if (arguments == "load_game" || arguments == "new_game" || arguments == "continue_game") {
            return false;
        }

        hk::func_call<void>(0x1447428C0, arguments.c_str(), nullptr);
        return true;
    }

    virtual std::vector<std::string> GetHints(const std::string& arguments) override
    {
        std::vector<std::string> result;
        std::copy_if(m_Hints.begin(), m_Hints.end(), std::back_inserter(result),
                     [&](const std::string& item) { return item.find(arguments) != std::string::npos; });

        return result;
    }
};
