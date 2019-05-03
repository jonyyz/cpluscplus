#pragma once

class IAction
{
  public:
    virtual ~IAction() {};

    virtual void Execute() = 0;
};
