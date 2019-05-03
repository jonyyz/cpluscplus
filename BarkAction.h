#pragma once

#include "IAction.h"

class BarkAction : public IAction
{
  public:
    BarkAction() {}
    virtual ~BarkAction() {}

    void Execute();
};
