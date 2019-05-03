#pragma once

#include <string>
#include <map>

#include "IAction.h"

class ActionRegistry
{
  private:
    std::map<std::string, IAction *> actions;

  public:
    ActionRegistry() {};
    virtual ~ActionRegistry() {};

    void Register(const std::string &, IAction *action);
    void Execute(const std::string &);
};
