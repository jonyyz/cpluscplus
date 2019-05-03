#include "ActionRegistry.h"

void ActionRegistry::Register(const std::string &name, IAction *action)
{
  this->actions.insert_or_assign(name, action);
}

void ActionRegistry::Execute(const std::string &name)
{
  this->actions[name]->Execute();
}
