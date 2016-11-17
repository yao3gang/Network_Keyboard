#include "bond.h"

Cbond *Cbond::_instance = 0;
int Cbond::b_inited = 0;

void Cbond::bondUpdateTime(SDateTime *pdt)
{
    SDateTime dt = *pdt;

    if (b_inited)
    {
        emit signalUpdateTime(dt);
    }
}

//extern function
int notifyGuiUpdateTime(SDateTime *pdt)
{
    gp_bond->bondUpdateTime(pdt);
    return 0;
}

